/********************************************************************************
**  
**	File:	 Robot.cpp
** 
**	Description: TITAN XIの本体の運動学
**
**	Created: 2003/10/22(Wed)	Updated: 2005/03/10(Thu)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
********************************************************************************/

#include "Robot.h"

/*******************************************************************
**  デフォルトコンストラクタ
*******************************************************************/
Robot::Robot()
{
	//////////////////////////
	////// 座標系の定義 //////
	//////////////////////////
	//		 右手系			//
	//						//
	//				  y		//
	//	x   1	     2↑	//
	//	←◎|--------|◎→	//
	//	  ↓|   x	 |	 x	//
	//	  y | 　↑   |		//
	//	    | ←◎   |		//
	//	    |y	     |y		//
	//	x   |	     |↑	//
	//	←◎|--------|◎→	//
	//	  ↓3		 4   x	//
	//	   y				//
	//////////////////////////

	// メモリ確保
	CreateLegInstance();

	InitRobotParameter();

}

/*******************************************************************
**  コンストラクタ
*******************************************************************/
Robot::Robot(const Matrix& body)
{
	//////////////////////////
	////// 座標系の定義 //////
	//////////////////////////
	//		 右手系			//
	//						//
	//				  y		//
	//	x   1	     2↑	//
	//	←◎|--------|◎→	//
	//	  ↓|   x	 |	 x	//
	//	  y | 　↑   |		//
	//	    | ←◎   |		//
	//	    |y	     |y		//
	//	x   |	     |↑	//
	//	←◎|--------|◎→	//
	//	  ↓3		 4   x	//
	//	   y				//
	//////////////////////////

	// 引数の配列チェック
	if ( (body.GetRow() != DH_NUM) || (body.GetCol() != DH_NUM) )
	{
		fprintf(stderr, "Robot::Robot Invalid Matrix");
	}

	// メモリの確保
	CreateLegInstance();

	InitRobotParameter();

	// 胴体の同次変換行列を更新
	Body = body;
}

/*******************************************************************
**  デストラクタ
*******************************************************************/
Robot::~Robot()
{
	DestoryLegInstance();
}

/*******************************************************************
**  メモリ確保
*******************************************************************/
void Robot::CreateLegInstance(void)
{
	// 脚相の配列を確保
	LegPH			= new LegPhase[LEG_NUM];

	// クラスポインタの配列確保
	Legs			= new Leg*[LEG_NUM];

	// クラスのインスタンスを生成
	for (int i=0; i<LEG_NUM; i++)
	{
		Legs[i]		= new Leg( (i+1), LEG_ROOT_POSX[i], LEG_ROOT_POSY[i], LEG_ROOT_POSZ[i], LEG_ROOT_ANGLE[i] );
	}
//printf("Robot::CreateLegInstance()\n");//071010 doi debug
}

/*******************************************************************
**  メモリ解放
*******************************************************************/
void Robot::DestoryLegInstance(void)
{
	delete [] LegPH;

	// インスタンスを破棄
	for (int i=0; i<LEG_NUM; i++)
		delete Legs[i];

	delete [] Legs;
}

/*******************************************************************
**  パラメータ初期化
*******************************************************************/
void Robot::InitRobotParameter(void)
{
	// 最初は収納形態
	SetLegPattern(0, RETRACT);
//	SetLegPattern(0, RECTANGLE);
	BodyMotionStyle = ALL_LEG;
	BodyPH = STATIONARY;

	for (int i=0; i<LEG_NUM; i++)
		LegPH[i] = SUPPORT;
	
	Body.SetSize(DH_NUM, DH_NUM);
	Body.LoadIdentity();

	LastLegErrID = 0;
}


/*******************************************************************
**  本体の同次変換行列をセット(足裏が本体と平行)
*******************************************************************/
KINE Robot::SetBodyTransform(const Matrix& body)
{
	// 要素数のチェック
	if ( body.GetRow() != DH_NUM || body.GetCol() != DH_NUM )
	{
		fprintf(stderr, "Robot::SetBodyTransform() Invalid prameter\n\n");
		return (KineErrSome);
	}

	KINE kine;

	// 脚先の新しい目標値(グローバル座標)
	Matrix newLegEndPos[LEG_NUM];
	// 現在の本体の同次変換行列の一時的な保持値
	Matrix oldBody(DH_NUM, DH_NUM);

	// ロボットの同次変換行列を更新する前のデータを保存
	oldBody = Body;
	for (int i=0; i<LEG_NUM; i++)
	{
		newLegEndPos[i].SetSize(DH_NUM, DH_NUM);
		newLegEndPos[i] = TransformationLocaltoGlobal( Legs[i]->GetFootTransform() );
	}

	// ロボットの同次変換行列を更新
	Body = body;
	/*
	double x=Body(1,4);//debug
	double y=Body(2,4);//debug
	double z=Body(3,4);//debug
	*/
	switch (BodyMotionStyle)
	{
		case SUPPORT_LEG:
			for (int j=0; j<LEG_NUM; j++)
			{
				if (LegPH[j] = SUPPORT)
				{
					// 新たな指示値で脚が到達できるかのチェック
//					kine = SetLegConfiguration( (j+1), newLegEndPos[j] );
					kine = SetFootConfiguration( (j+1), newLegEndPos[j] );
		
					// もし脚先が新しい指令値に到達できなかったら
					if (kine != NoKineError)
					{
						// 本体の状態を更新する前に戻す
						Body = oldBody;
						//足先位置も更新前に戻す．060801 doi
						for(int  l=0;l<LEG_NUM;l++)
						{
						//SetLegConfiguration( (l+1), newLegEndPos[l] );
						SetFootConfiguration( (l+1), newLegEndPos[l] );
						}

						// 脚のエラーIDを書き込み
						LastLegErrID = j+1;
						return (kine);
					}

					// 脚の失敗なし
					LastLegErrID = 0;
				
				}
			}
			break;

		case ALL_LEG:
            for (int k=0; k<LEG_NUM; k++)
			{
				// 新たな指示値で脚が到達できるかのチェック
				//kine = SetLegConfiguration( (k+1), newLegEndPos[k] );
				kine = SetFootConfiguration( (k+1), newLegEndPos[k] );
		
				// もし脚先が新しい指令値に到達できなかったら
				if (kine != NoKineError)
				{
					// 本体の状態を更新する前に戻す
					Body = oldBody;

					//足先位置も更新前に戻す．060801 doi
					for(int  l=0;l<LEG_NUM;l++)
					{
					//SetLegConfiguration( (l+1), newLegEndPos[l] );
					SetFootConfiguration( (l+1), newLegEndPos[l] );
					}

					// 脚のエラーIDを書き込み
					LastLegErrID = k+1;
					return (kine);
				}

				// 脚の失敗なし
				LastLegErrID = 0;

			}
			break;

		case CRAWLER:
			break;
	}

	return (NoKineError);
}

/*******************************************************************
**  本体の同次変換行列をセット(DKベース)
*******************************************************************/
KINE Robot::SetBodyTransformWithDK(const Matrix& body)//060813 doi
{
    //DKベースで胴体位置設定
	// 要素数のチェック
	if ( body.GetRow() != DH_NUM || body.GetCol() != DH_NUM )
	{
		fprintf(stderr, "Robot::SetBodyTransform() Invalid prameter\n\n");
		return (KineErrSome);
	}

	KINE kine;
        int i;

	// 脚先の新しい目標値(グローバル座標)
	Matrix newLegEndPos[LEG_NUM];
	// 現在の本体の同次変換行列の一時的な保持値
	Matrix oldBody(DH_NUM, DH_NUM);

	// ロボットの同次変換行列を更新する前のデータを保存
	oldBody = Body;

	// ロボットの同次変換行列を更新
	Body = body;

	/*
	double x=Body(1,4);//debug
	double y=Body(2,4);//debug
	double z=Body(3,4);//debug
	*/
        for(i=0;i<LEG_NUM;i++)
        {
            kine=CalcLegDirectKinematics(i+1);
            if(kine!=NoKineError)
            {
                Body=oldBody;
                return kine;
            }
        }
	return (NoKineError);
}

/*******************************************************************
**  本体を動かす方法をセット
*******************************************************************/
void Robot::SetMotionStyle(MotionStyle style)
{
	BodyMotionStyle = style;

	return;
}

/*******************************************************************
**  胴体の状態相をセット
*******************************************************************/
void Robot::SetBodyPhase(BodyPhase phase)
{
	BodyPH = phase;

	return;
}

/*******************************************************************
**  脚の構成をデフォルトの構成にセット
*******************************************************************/
int Robot::SetLegPattern (int legID, LegPattern pattern)
{
	Matrix angle[LEG_NUM];

	for (int i=0; i<LEG_NUM; i++)
		angle[i].SetSize(JOINT_NUM);

	switch (pattern)
	{
		case RETRACT:		// 格納形態

			angle[0](1)=-90*DEG2RAD; angle[0](2)=60*DEG2RAD; angle[0](3)=-150*DEG2RAD;
			angle[1](1)=90*DEG2RAD;  angle[1](2)=60*DEG2RAD; angle[1](3)=-150*DEG2RAD;
			angle[2](1)=90*DEG2RAD;  angle[2](2)=60*DEG2RAD; angle[2](3)=-150*DEG2RAD;
			angle[3](1)=-90*DEG2RAD;  angle[3](2)=60*DEG2RAD; angle[3](3)=-150*DEG2RAD;

			if (legID == 0)
			{
				for (int j=0; j<LEG_NUM; j++)
					Legs[j]->SetJntAng(angle[j]);
			}
			else if ( IsParamWithin(legID, 1, LEG_NUM) )
			{
				Legs[legID-1]->SetJntAng(angle[legID-1]);
			}

			break;

		case RECTANGLE:		// 長方形

			angle[0](1)=0*DEG2RAD; angle[0](2)=0*DEG2RAD; angle[0](3)=-90*DEG2RAD;
			angle[1](1)=0*DEG2RAD; angle[1](2)=0*DEG2RAD; angle[1](3)=-90*DEG2RAD;
			angle[2](1)=0*DEG2RAD; angle[2](2)=0*DEG2RAD; angle[2](3)=-90*DEG2RAD;
			angle[3](1)=0*DEG2RAD; angle[3](2)=0*DEG2RAD; angle[3](3)=-90*DEG2RAD;

			if (legID == 0)
			{
				for (int k=0; k<LEG_NUM; k++)
					Legs[k]->SetJntAng(angle[k]);
			}
			else if ( IsParamWithin(legID, 1, LEG_NUM) )
			{
				Legs[legID-1]->SetJntAng(angle[legID-1]);
			}

			break;

		case PRESET:		// 設定した角度に指定(ここでは45°)

			angle[0](1)=-45*DEG2RAD; angle[0](2)=45*DEG2RAD; angle[0](3)=-90*DEG2RAD;
			angle[1](1)=45*DEG2RAD;  angle[1](2)=45*DEG2RAD; angle[1](3)=-90*DEG2RAD;
			angle[2](1)=45*DEG2RAD;  angle[2](2)=45*DEG2RAD; angle[2](3)=-90*DEG2RAD;
			angle[3](1)=45*DEG2RAD;  angle[3](2)=45*DEG2RAD; angle[3](3)=-90*DEG2RAD;

			if (legID == 0)
			{
				for (int l=0; l<LEG_NUM; l++)
					Legs[l]->SetJntAng(angle[l]);
			}
			else if ( IsParamWithin(legID, 1, LEG_NUM) )
			{
				Legs[legID-1]->SetJntAng(angle[legID-1]);
			}
			break;

		default:
			break;
	}

	return (1);

}

/*******************************************************************
**  脚のインスタンスを取得
*******************************************************************/
Leg* Robot::GetLeg(int legID) const
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::GetLeg() Invalid leg number\n\n");
		return (0);
	}

	return ( Legs[legID-1] );

}

/*******************************************************************
**  脚の状態相をセット
*******************************************************************/
void Robot::SetLegPhase(int legID, LegPhase phase)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::SetLegPhase() Invalid leg number\n\n");
		return;
	}

	LegPH[legID-1] = phase;

}

/*******************************************************************
**  胴体の状態相をセット
*******************************************************************/
Robot::LegPhase Robot::GetLegPhase(int legID) const
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::GetLegPhase() Invalid LegID");
		return (SUPPORT);
	}

	return (LegPH[legID-1]);
}

/*************************************************************************************************************
**  これより以下は脚のメンバ関数を呼び出し 
*************************************************************************************************************/

/*******************************************************************
**  脚先の同次変換行列をセット
*******************************************************************/
/*************************************
**  引数(endpos)はグローバル座標系  **
*************************************/
KINE Robot::SetLegEndPos(int legID, const Matrix& endPos)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::SetLegEndPos() Invalid leg number\n\n");
		return (KineErrSome);
	}

	KINE kine;
	Matrix position(DH_NUM, DH_NUM);

	// グローバルから本体ローカルへ指示値を変換
	position = TransformationGlobaltoLocal(endPos);
	kine = Legs[legID-1]->SetEndPos(position);
	
	return (kine);
}

/*******************************************************************
**  SetEndPos後に, CalcDKを解く
*******************************************************************/
/*************************************
**  引数(target)はグローバル座標系  **
*************************************/
//KINE Robot::SetLegConfiguration(int legID, const Matrix& target)
KINE Robot::SetFootConfiguration(int legID, const Matrix& target)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::SetLegConfiguration() Invalid leg number\n\n");
		return (KineErrSome);
	}

	KINE kine;
	Matrix goal(DH_NUM, DH_NUM);

	// グローバルから本体ローカルへ指示値を変換
	goal = TransformationGlobaltoLocal(target);
	double Zgoal=goal(3,4);//060808 doi
	//double Ztarget=target(3,4);//060808 doi
	if(Zgoal > 100000)
	{
		goal = TransformationGlobaltoLocal(target);//debug
		//int temp=0;//debug
	}
	kine = Legs[legID-1]->SetConfiguration(goal);

	if ( kine != NoKineError)
	{
		LastLegErrID = legID;
	}

	return (kine);
}

/*******************************************************************
**  脚の関節角をセット
*******************************************************************/
KINE Robot::SetLegJntAng(int legID, const Matrix& jntAng)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::SetLegJntAng() Invalid Leg Number\n\n");
		return (KineErrSome);
	}

	KINE kine;

	kine = Legs[legID-1]->SetJntAng(jntAng);

	return (kine);

}

/*******************************************************************
**  脚の各JntStatを更新
*******************************************************************/
void Robot::UpdateLegJntState(int legID)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::UpdateLegJntState() Invalid leg number\n\n");
		return;
	}

	Legs[legID-1]->UpdateJntState();

	return;
}

/***********************************************************************************************
**  各種の計算を行うLegクラスのメンバ関数呼び出し
***********************************************************************************************/
/*******************************************************************
**  脚の順運動学計算
*******************************************************************/
KINE Robot::CalcLegDirectKinematics(int legID)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::CalcLegDirectKinematics() Invalid leg number\n\n");
		return (KineErrSome);
	}

	return ( Legs[legID-1]->CalcDirectKinematics() );
}

/*******************************************************************
**  脚の逆運動学計算(足裏は本体と平行)
*******************************************************************/
KINE Robot::CalcLegInverseKinematics(int legID)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::CalcLegInverseKinematics() Invalid leg number\n\n");
		return (KineErrSome);
	}

	return ( Legs[legID-1]->CalcInverseKinematics() );
}


/*******************************************************************
**  脚のヤコビ行列の計算
*******************************************************************/
void Robot::CalcLegJacobian(int legID)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::CalcLegJacobian() Invalid leg number\n\n");
		return;
	}

	return ( Legs[legID-1]->CalcJacobian() );
}

/*******************************************************************
**  脚の逆ヤコビ行列の計算
*******************************************************************/
void Robot::CalcLegInverseJacobian(int legID)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::CalcLegInverseJacobian() Invalid leg number\n\n");
		return;
	}

	return ( Legs[legID-1]->CalcInverseJacobian() );
}

/*******************************************************************
**  脚先にかかる力をセット
*******************************************************************/
KINE Robot::SetLegFootForce(int legID, const Matrix& force)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::SetLegFootForce() Invalid leg number\n\n");
		return (KineErrSome);
	}

	return ( Legs[legID-1]->SetFootForce(force) );
}

/*******************************************************************
**  脚のシリンダ推力, CldrThrustベクトルの計算
*******************************************************************/
KINE Robot::CalcLegCylinderThrust(int legID)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::CalcLegCylinderThrust() Invalid leg number\n\n");
		return (KineErrSome);
	}

	return ( Legs[legID-1]->CalcCylinderThrust() );
}

/*******************************************************************
**  脚のシリンダ長ベクトルから角度ベクトルJntAngに値をセット
*******************************************************************/
KINE Robot::CalcLegAngFromLen(int legID, int jntID)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::CalcLegAngFromLen() Invalid leg number\n\n");
		return (KineErrSome);
	}

	return ( Legs[legID-1]->CalcAngFromLen(jntID) );
}

KINE Robot::CalcLegAngFromLen(int legID)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::CalcLegAngFromLen() Invalid leg number\n\n");
		return (KineErrSome);
	}

	return ( Legs[legID-1]->CalcAngFromLen() );
}

/*******************************************************************
**  脚の角度ベクトルJntAngからシリンダベクトルCldrLenに値をセット
*******************************************************************/
KINE Robot::CalcLegLenFromAng(int legID, int jntID)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::CalcLegLenFromAng() Invalid leg number\n\n");
		return (KineErrSome);
	}

	return ( Legs[legID-1]->CalcLenFromAng(jntID) );
}

KINE Robot::CalcLegLenFromAng(int legID)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::CalcLegLenFromAng() Invalid Leg Number\n\n");
		return (KineErrSome);
	}

	return ( Legs[legID-1]->CalcLenFromAng() );
}

double Robot::CalcLegInnerMotionRange(int legID, double height)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::CalcLegInnerMotionRange() Invalid Leg Number\n\n");
		return (KineErrSome);
	}

	return ( Legs[legID-1]->GetInnerMotionRange(height) );
}

double Robot::CalcLegOuterMotionRange(int legID, double height)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::CalcLegOuterMotionRange() Invalid Leg Number\n\n");
		return (KineErrSome);
	}

	return ( Legs[legID-1]->GetOuterMotionRange(height) );
}

/****************************************************************************************
**  脚のメンバ変数を取得
**	座標系は胴体座標系で得られる
****************************************************************************************/
/*******************************************************************
**  脚の各関節状態を得る
*******************************************************************/
Matrix Robot::GetLegTransformMatrix(int legID, int jntID) const
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::GetLegTransoformMatrix() Invalid Leg Number\n\n");
		
		return (IDENTITY_MATRIX_DH);
	}
    //printf("Robot::GetTransformMatrix() LegID=%d\n", legID);//071010 doi debug
	return ( Legs[legID-1]->GetTransformMatrix(jntID) );
}

/*******************************************************************
**  脚先の状態を得る
*******************************************************************/
Matrix Robot::GetLegFootTransform(int legID) const
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::GetLegFootTransform() Invalid leg number\n\n");
		
		return (IDENTITY_MATRIX_DH);
	}

	return ( Legs[legID-1]->GetFootTransform() );
}

/*******************************************************************
**  脚先にかかる力を得る
*******************************************************************/
Matrix Robot::GetLegFootForce(int legID) const
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::GetLegFootForce() Invalid leg number\n\n");
		
		return (ZERO_VECTOR_3);
	}

	return ( Legs[legID-1]->GetFootForce() );
}

/*******************************************************************
**  各脚のヤコビ行列を得る
*******************************************************************/
Matrix Robot::GetLegJacobian(int legID) const
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::GetLegJacobian() Invalid leg number\n\n");

		return (ZERO_MATRIX_3X3);
	}

	return ( Legs[legID-1]->GetJacobian() );
}

/*******************************************************************
**  各脚の逆ヤコビ行列を得る
*******************************************************************/
Matrix Robot::GetLegInvJacobian(int legID) const
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::GetLegInvJacobian() Invalid leg number\n\n");

		return (ZERO_MATRIX_3X3);
	}

	return ( Legs[legID-1]->GetInvJacobian() );
}

/*******************************************************************
**  各脚のベース決定パラメータBaseParamを得る
*******************************************************************/
Matrix Robot::GetLegBaseParameter(int legID) const
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::GetLegBaseParameter() Invalid leg number\n\n");

		return (ZERO_VECTOR_4);
	}

	return ( Legs[legID-1]->GetBaseParameter() );
}

/*******************************************************************
**  各脚の関節角度ベクトルを得る
*******************************************************************/
Matrix Robot::GetLegJntAng(int legID) const
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::GetLegJntAng() Invalid leg number\n\n");

		return (ZERO_VECTOR_JOINT);
	}

	return ( Legs[legID-1]->GetJntAng() );
}

/*******************************************************************
**  各脚の関節角速度ベクトルを得る
*******************************************************************/
Matrix Robot::GetLegJntAngVel(int legID) const
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::GetLegJntAngVel() Invalid leg number\n\n");

		return (ZERO_VECTOR_JOINT);
	}

	return ( Legs[legID-1]->GetJntAngVel() );
}

/*******************************************************************
**  各脚のシリンダ長ベクトルを得る
*******************************************************************/
Matrix Robot::GetLegCldrLen(int legID) const
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::GetLegCldrLen() Invalid leg number\n\n");

		return (ZERO_VECTOR_JOINT);
	}

	return ( Legs[legID-1]->GetCldrLen() );
}

/*******************************************************************
**  各脚のシリンダ速度ベクトルを得る
*******************************************************************/
Matrix Robot::GetLegCldrSpd(int legID) const
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::GetLegCldrSpd() Invalid leg number\n\n");

		return (ZERO_VECTOR_JOINT);
	}

	return ( Legs[legID-1]->GetCldrSpd() );
}

/*******************************************************************
**  各脚のシリンダ推力ベクトルを得る
*******************************************************************/
Matrix Robot::GetLegCldrThrust(int legID) const
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::GetLegCldrThrust() Invalid leg number\n\n");

		return (ZERO_VECTOR_JOINT);
	}

	return ( Legs[legID-1]->GetCldrThrust() );
}
//***********************************************************************************************ここまで胴体座標系で得られる

/*******************************************************************
**  脚のメンバ変数を取得
**	座標系はグローバル座標系で得られる
*******************************************************************/
Matrix Robot::GetLegTransformMatrixInGlobal(int legID, int jntID) const
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::GetLegTransformMatrixInGlobal() Invalid leg number\n\n");

		return (IDENTITY_MATRIX_DH);
	}

	if ( !IsParamWithin(jntID, 0, JOINT_NUM+1) )
	{
		fprintf(stderr, "Robot::GetLegTransformMatrixInGlobal() Invalid joint number\n\n");

		return (IDENTITY_MATRIX_DH);
	}

	return ( TransformationLocaltoGlobal( Legs[legID-1]->GetTransformMatrix(jntID) ) );
}

Matrix Robot::GetLegFootTransformInGlobal(int legID)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::GetLegFootTransformInGlobal() Invalid leg number\n\n");

		return (IDENTITY_MATRIX_DH);
	}

	return ( TransformationLocaltoGlobal( Legs[legID-1]->GetFootTransform() ) );
}

/*******************************************************************
**  その他
*******************************************************************/
int Robot::GetLegLastJntErr(int legID) const
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Robot::GetLegLastJntErr() Invalid leg number\n\n");

		return (-1);
	}

	return ( Legs[legID-1]->GetLastJntErr() );
}

/*******************************************************************
**  グローバルから本体ローカルへの座標変換
*******************************************************************/
Matrix Robot::TransformationGlobaltoLocal(const Matrix& matrix) const
{
	Matrix result(DH_NUM, DH_NUM);
	Matrix transform(DH_NUM, DH_NUM);

	// 要素数のチェック
	if ( (matrix.GetRow() != DH_NUM) || (matrix.GetCol() != DH_NUM) )
	{
		fprintf(stderr, "Robot::TransformationGlobaltoLocal() Invalid matrix\n\n");
		return (IDENTITY_MATRIX_DH);
	}

	//以下逆行列計算．計算高速化のためか？ 060808 doi
	//transform=Body.Inversion();//060808 doi
	//回転成分の逆変換を転置行列で計算
	transform(1,1) = Body(1,1);		transform(1,2) = Body(2,1);		transform(1,3) = Body(3,1);
	transform(2,1) = Body(1,2);		transform(2,2) = Body(2,2);		transform(2,3) = Body(3,2);
	transform(3,1) = Body(1,3);		transform(3,2) = Body(2,3);		transform(3,3) = Body(3,3);
	transform(4,1) = 0;				transform(4,2) = 0;				transform(4,3) = 0;
	//並進成分の逆変換を転置行列で計算
	transform(1,4) = -(Body(1,1)*Body(1,4)+Body(2,1)*Body(2,4)+Body(3,1)*Body(3,4));
	transform(2,4) = -(Body(1,2)*Body(1,4)+Body(2,2)*Body(2,4)+Body(3,2)*Body(3,4));
	transform(3,4) = -(Body(1,3)*Body(1,4)+Body(2,3)*Body(2,4)+Body(3,3)*Body(3,4));
	transform(4,4) = 1;

	result = transform*matrix;
	/*
	double b11=Body(1,1);//debug
	double b14=Body(1,4);//debug
	double b24=Body(2,4);//debug
	double b34=Body(3,4);//debug
	*/


	return (result);

}

/*******************************************************************
**  本体ローカルからグローバルへの座標変換
*******************************************************************/
Matrix Robot::TransformationLocaltoGlobal(const Matrix& matrix) const
{
	Matrix result(DH_NUM, DH_NUM);

	// 要素数のチェック
	if ( (matrix.GetRow() != DH_NUM) || (matrix.GetCol() != DH_NUM) )
	{
		fprintf(stderr, "Robot::TransformationLocaltoGlobal() Invalid matrix\n\n");
		return (IDENTITY_MATRIX_DH);
	}

	result = Body*matrix;

	return (result);
}
