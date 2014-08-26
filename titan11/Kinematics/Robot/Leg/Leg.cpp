/*****************************************************************************
**  
**	File:	  Leg.cpp
** 
**	説明: TITAN XIの1脚制御用プログラム
**
**	Created: 2003/09/19(Fri)	Updated: 2005/01/14(Fri)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

#include "Leg.h"

/******************************
**  デフォルトコンストラクタ
******************************/
Leg::Leg()
{
	InitLegParameter();

	SetLegID(1);
	SetBaseParameter(0, 0, 0, 0);

}

/******************************
**  コンストラクタ
******************************/
Leg::Leg(int legID, double x, double y, double z, double theta) : Arm(x, y, z, theta)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "Leg Constructor() Leg::Leg Invalid LegID");
		exit(1);
	}

	InitLegParameter();

	SetLegID(legID);
	SetBaseParameter(x, y, z, theta);
}

/******************************
**  デストラクタ
******************************/
Leg::~Leg()
{
	delete [] CldrLmtMin;
	delete [] CldrLmtMax;
	delete [] CldrThrustMin;
	delete [] CldrThrustMax;
}

/******************************
**  脚の初期化用関数
******************************/
void Leg::InitLegParameter(void)
{
// 以下でメンバ変数の初期化を行う

	// 配列の要素数確保
	CldrLmtMin		=	new double[JOINT_NUM];
	CldrLmtMax		=	new double[JOINT_NUM];
	CldrThrustMin	=	new double[JOINT_NUM];
	CldrThrustMax	=	new double[JOINT_NUM];

	// マトリクス, ベクトルの要素数設定
	Foot.SetSize(DH_NUM, DH_NUM);
	FootForce.SetSize(POS_DOF);

	Jacobian.SetSize(POS_DOF, POS_DOF);
	InvJacobian.SetSize(POS_DOF, POS_DOF);
	JacobianL.SetSize(POS_DOF, POS_DOF);
	InvJacobianL.SetSize(POS_DOF, POS_DOF);
		
	CldrLen.SetSize(JOINT_NUM);
	CldrSpd.SetSize(JOINT_NUM);
	CldrThrust.SetSize(JOINT_NUM);
	
	// シリンダに関する変数の初期化
	CldrLen.LoadZero();
	CldrSpd.LoadZero();
	CldrThrust.LoadZero();

	// 限界値設定
	CldrLmtMin[0] = CLDR_LEN1_MIN;
	CldrLmtMin[1] = CLDR_LEN2_MIN;
	CldrLmtMin[2] = CLDR_LEN3_MIN;

	CldrLmtMax[0] = CLDR_LEN1_MAX;
	CldrLmtMax[1] = CLDR_LEN2_MAX;
	CldrLmtMax[2] = CLDR_LEN3_MAX;

	CldrThrustMin[0] = CLDR_THRUST1_MIN;
	CldrThrustMin[1] = CLDR_THRUST2_MIN;
	CldrThrustMin[2] = CLDR_THRUST3_MIN;

	CldrThrustMax[0] = CLDR_THRUST1_MAX;
	CldrThrustMax[1] = CLDR_THRUST2_MAX;
	CldrThrustMax[2] = CLDR_THRUST3_MAX;

	FootFrcLmtMin[0] = FORCE_X_MIN;
	FootFrcLmtMin[1] = FORCE_Y_MIN;
	FootFrcLmtMin[2] = FORCE_Z_MIN;

	FootFrcLmtMax[0] = FORCE_X_MAX;
	FootFrcLmtMax[1] = FORCE_Y_MAX;
	FootFrcLmtMax[2] = FORCE_Z_MAX;

	// 脚先の変数に関する初期化
	Foot.LoadIdentity();		// デフォルトは恒等変換
	Jacobian.LoadZero();
	InvJacobian.LoadZero();
	JacobianL.LoadZero();
	InvJacobianL.LoadZero();

	FootForce.LoadZero();

	LegID			= 0;
	return;
}

/*******************************************************************
**	脚のIDをセット
*******************************************************************/
int Leg::SetLegID(int id)
{											// o 1         2 o
	if ( !IsParamWithin(id, 1, LEG_NUM) )	//  |           |
	{										//   |         |
		return -1;							//    |-------|
	}										//    |       |
											//    |       |
	LegID = id;								//    |       |
											//    |       |
	return LegID;							//    |-------|
											//   |         |
}											//  |           |
											// o 3         4 o


/************************************************************************
**	各同次変換行列を得る
************************************************************************/
Matrix Leg::GetTransformMatrix(int jntID) const
{
	if ( !IsParamWithin(jntID, 0, JOINT_NUM+1) )
	{
		fprintf(stderr, "Leg::GetTransformMatrix() Invalid Joint Number\n\n");
		
		return (IDENTITY_MATRIX_DH);
	}

	Matrix T(DH_NUM, DH_NUM);
	switch (jntID)
	{
		case 0:
			T = Transformation[jntID];
			break;

		case 1:
			T = Transformation[jntID];
			break;

		case 2:
			T = Transformation[jntID];
			break;
			
		case 3:
			T = Transformation[jntID];
			break;

		case 4:
			T = Foot;
			break;

		default:
			break;
	}

	return T;
}

/***********************************
**  以下は座標変換に関する関数群  **
***********************************/

/************************************************************************
**  同次変換行列の計算
************************************************************************/
Matrix Leg::CoordinateTransformation(int jntID) const
{
	if ( !IsParamWithin(jntID, 0, JOINT_NUM+1) )
	{
		fprintf(stderr, "Leg::CoordinateTransformation() Invalid Joint Number\n\n");
		
		return (IDENTITY_MATRIX_DH);
	}

	Matrix A(DH_NUM, DH_NUM);

	switch (jntID)
	{
		case 0:		// グローバルから脚のベースへの変換
			A(1,1) = cos( BaseParameter(4) );	A(1,2) = -sin( BaseParameter(4) );	A(1,3) = 0;						A(1,4) = BaseParameter(1);
			A(2,1) = sin( BaseParameter(4) );	A(2,2) = cos( BaseParameter(4) );	A(2,3) = 0;						A(2,4) = BaseParameter(2);
			A(3,1) = 0;							A(3,2) = 0;							A(3,3) = 1;						A(3,4) = BaseParameter(3);
			A(4,1) = 0;							A(4,2) = 0;							A(4,3) = 0;						A(4,4) = 1;
			break;

		case 1:		// 0系から1系への変換
			A(1,1) = cos(JntAng(jntID));		A(1,2) = 0;							A(1,3) = sin(JntAng(jntID));	A(1,4) = 0;
			A(2,1) = sin(JntAng(jntID));		A(2,2) = 0;							A(2,3) = -cos(JntAng(jntID));	A(2,4) = 0;
			A(3,1) = 0;							A(3,2) = 1;							A(3,3) = 0;						A(3,4) = Link[jntID-1];
			A(4,1) = 0;							A(4,2) = 0;							A(4,3) = 0;						A(4,4) = 1;
			break;

		case 2:		// 1系から2系への変換
			A(1,1) = cos(JntAng(jntID));		A(1,2) = -sin(JntAng(jntID));		A(1,3) = 0;						A(1,4) = Link[jntID-1]*cos(JntAng(jntID));
			A(2,1) = sin(JntAng(jntID));		A(2,2) = cos(JntAng(jntID));		A(2,3) = 0;						A(2,4) = Link[jntID-1]*sin(JntAng(jntID));
			A(3,1) = 0;							A(3,2) = 0;							A(3,3) = 1;						A(3,4) = 0;
			A(4,1) = 0;							A(4,2) = 0;							A(4,3) = 0;						A(4,4) = 1;
			break;

		case 3:		// 2系から3系への変換
			A(1,1) = cos(JntAng(jntID));		A(1,2) = -sin(JntAng(jntID));		A(1,3) = 0;						A(1,4) = Link[jntID-1]*cos(JntAng(jntID));
			A(2,1) = sin(JntAng(jntID));		A(2,2) = cos(JntAng(jntID));		A(2,3) = 0;						A(2,4) = Link[jntID-1]*sin(JntAng(jntID));
			A(3,1) = 0;							A(3,2) = 0;							A(3,3) = 1;						A(3,4) = 0;
			A(4,1) = 0;							A(4,2) = 0;							A(4,3) = 0;						A(4,4) = 1;
			break;

		case 4:		// 3系から4系への変換(本体と足裏が平行)
		{
			double angle = -( PI/2 + JntAng(2) + JntAng(3) );
			A(1,1) = cos(angle);	A(1,2) = -sin(angle);	A(1,3) = 0;			A(1,4) = FOOT*cos(angle);
			A(2,1) = sin(angle);	A(2,2) = cos(angle);	A(2,3) = 0;			A(2,4) = FOOT*sin(angle);
			A(3,1) = 0;				A(3,2) = 0;				A(3,3) = 1;			A(3,4) = 0;
			A(4,1) = 0;				A(4,2) = 0;				A(4,3) = 0;			A(4,4) = 1;
			break;
		}

		default:
			break;
	}

	return A;
}

/************************************************************************
**	逆同次変換行列の計算
************************************************************************/
Matrix Leg::InvCoordinateTransformation(int jntID) const
{
	if ( !IsParamWithin(jntID, 0, JOINT_NUM) )
	{
		fprintf(stderr, "Leg::InvCoordinateTransformation Invalid Joint Number\n\n");
		
		return (IDENTITY_MATRIX_DH);
	}

	Matrix A(DH_NUM, DH_NUM);

	switch (jntID)
	{
		case 0:		// 脚のベースからグローバルへの変換
			A(1,1) = cos( BaseParameter(4) );	A(1,2) = sin( BaseParameter(4) );	A(1,3) = 0;			A(1,4) = -( cos( BaseParameter(4) )*BaseParameter(1) + sin( BaseParameter(4) )*BaseParameter(2) );
			A(2,1) = -sin( BaseParameter(4) );	A(2,2) = cos( BaseParameter(4) );	A(2,3) = 0;			A(2,4) = -( -sin( BaseParameter(4) )*BaseParameter(1) + cos( BaseParameter(4) )*BaseParameter(2) );
			A(3,1) = 0;							A(3,2) = 0;							A(3,3) = 1;			A(3,4) = -BaseParameter(3);
			A(4,1) = 0;							A(4,2) = 0;							A(4,3) = 0;			A(4,4) = 1;
			break;

		case 1:		// 1系から0系への変換
			A(1,1) = cos(JntAng(jntID));		A(1,2) = sin(JntAng(jntID));		A(1,3) = 0;			A(1,4) = 0;
			A(2,1) = 0;							A(2,2) = 0;							A(2,3) = 1;			A(2,4) = -Link[jntID-1];
			A(3,1) = sin(JntAng(jntID));		A(3,2) = -cos(JntAng(jntID));		A(3,3) = 0;			A(3,4) = 0;
			A(4,1) = 0;							A(4,2) = 0;							A(4,3) = 0;			A(4,4) = 1;
			break;

		case 2:		// 2系から1系への変換
			A(1,1) = cos(JntAng(jntID));		A(1,2) = sin(JntAng(jntID));		A(1,3) = 0;			A(1,4) = -Link[jntID-1];
			A(2,1) = -sin(JntAng(jntID));		A(2,2) = cos(JntAng(jntID));		A(2,3) = 0;			A(2,4) = 0;
			A(3,1) = 0;							A(3,2) = 0;							A(3,3) = 1;			A(3,4) = 0;
			A(4,1) = 0;							A(4,2) = 0;							A(4,3) = 0;			A(4,4) = 1;
			break;

		case 3:		// 3系から2系への変換
			A(1,1) = cos(JntAng(jntID));		A(1,2) = sin(JntAng(jntID));		A(1,3) = 0;			A(1,4) = -Link[jntID-1];
			A(2,1) = -sin(JntAng(jntID));		A(2,2) = cos(JntAng(jntID));		A(2,3) = 0;			A(2,4) = 0;
			A(3,1) = 0;							A(3,2) = 0;							A(3,3) = 1;			A(3,4) = 0;
			A(4,1) = 0;							A(4,2) = 0;							A(4,3) = 0;			A(4,4) = 1;
			break;

		case 4:		// 4系から3系への変換
		{
			double angle = -( PI/2 + JntAng(2) + JntAng(3) );
			A(1,1) = cos(angle);	A(1,2) = sin(angle);	A(1,3) = 0;			A(1,4) = -FOOT;
			A(2,1) = -sin(angle);	A(2,2) = cos(angle);	A(2,3) = 0;			A(2,4) = 0;
			A(3,1) = 0;				A(3,2) = 0;				A(3,3) = 1;			A(3,4) = 0;
			A(4,1) = 0;				A(4,2) = 0;				A(4,3) = 0;			A(4,4) = 1;
			break;
		}

		default:
			break;
	}

	return A;
}

/*******************************
**	以下は運動学に関する計算  **
*******************************/

/*******************************************************************
**	順運動学の計算
*******************************************************************/
KINE Leg::CalcDirectKinematics(void)
{
	// 結果判定用のキネマティクス
	KINE kine = NoKineError;

	// 関節が可動限界内にあるか
	kine = CheckJointRange();

	if (kine == NoKineError)
	{
		// 各同次変換行列の計算
		Transformation[0] = CoordinateTransformation(0);
		for (int i=1; i<(JOINT_NUM+1); i++)
			Transformation[i] = Transformation[i-1]*CoordinateTransformation(i);

		// 足裏の同次変換行列
		Foot = Transformation[POS_DOF]*( Leg::CoordinateTransformation(4) );

		// 設定した可動範囲内に脚先があるか
		kine = CheckReachRange();

	}
	else
	{
		fprintf(stderr, "Leg::CalcDirectKinematics() Error#%d. \n",kine);//060813 doi
            ;
	}

	return kine;
}

/*******************************************************************
**	逆運動学の計算
**	解析的に解くため, 足首点から解く
**	実際の軌道を考える時には, 足首リンク分だけオフセットするべき
*******************************************************************/
/***********************
**	足裏は本体と平行  **
***********************/
KINE Leg::CalcInverseKinematics(void)
{
	KINE kine;

	double temp;					// 第1関節角のデータ保持用
	double C3, S3;					// 関節角度3によるsin, cos
	double a, b, c, d;				// 関節角度2を計算するための一時的な変数
	
	Matrix Trans(DH_NUM, DH_NUM);	// 脚ベース座標系での脚先の同次変換行列
	Trans.LoadIdentity();			// Control Point Status
	
	// // 脚ベース座標系に変換
	Trans = InvCoordinateTransformation(0)*Foot;

        Matrix Ankle(DH_NUM, DH_NUM);	// 脚ベース座標系での足首の同次変換行列 060723 doi
        Ankle = Trans;//060723 doi
        Ankle(3,4)=Ankle(3,4)+FOOT;//060808 doi
        double ZAnkle=Ankle(3,4);//060808 doi
        //double ZTrans=Trans(3,4);//060808 doi
        //double ZFoot=Foot(3,4);//060808 doi
		if(ZAnkle>10000000)
		{
			printf("Leg::CalcInverseKinematics() ZAnkle=%.0f\n",ZAnkle);//debug
		}

    
//	kine = CheckReachRange();
//	kine = CheckReachRange(Foot);    //060720 doi
	kine = CheckReachRange(Ankle);    //060720 doi    足首点でチェックする

	if (kine == NoKineError)
	{
		// 最初の関節角を計算
		if (fabs(Trans(1,4)) < 0.001 && fabs(Trans(2,4)) < 0.001)
		{
			return (KineErrSingular);		// 今回は起きない
		}
		else
		{
			temp = JntAng(1);
			// 第1関節角を更新
			JntAng(1) = atan2(Indicator[0]*Trans(2,4), Indicator[0]*Trans(1,4));
		}

		// 3番目の関節角を計算
		// 余弦(cos)を計算
		// 足首高さ分だけオフセットして脚先の同次変換行列Transformation[3]にする
		C3 = (Trans(1,4)*Trans(1,4)+Trans(2,4)*Trans(2,4)+(Trans(3,4) - Link[0]+FOOT)*(Trans(3,4) - Link[0]+FOOT)
			-Link[1]*Link[1]-Link[2]*Link[2])/(2*Link[1]*Link[2]);

		if (fabs(C3) > 1)		// 3番目の関節角が計算可能かどうかチェック
		{
			// 逆キネマティクスが解けないので値を戻す
			JntAng(1) = temp;
			LastJntErrNum = 3;

			return (KineErrReach);
		}
		else if ( fabs(C3) == 1 )
		{
			return (KineErrSingular);
		}

		// 正弦(sin)を計算
		S3 = -Indicator[1]*sqrt(1-C3*C3);

		// 第3関節角を更新
		JntAng(3) = atan2(S3, C3);

		// 2番目の関節角を計算
		a = Link[1]+Link[2]*C3;
		b = Link[2]*S3;
		c = Indicator[0]*sqrt(Trans(1,4)*Trans(1,4)+Trans(2,4)*Trans(2,4));
		d = Trans(3,4)-Link[0]+FOOT;

		// 第2関節角を更新
		JntAng(2) = atan2( (a*d-b*c), (a*c+b*d) );
		
		kine = CheckJointRange();
	}

	return kine;
}

/*******************************************************************
**	関節角を個別に計算 (値を得るだけ)
**	関節角のチェック用関数
*******************************************************************/
double Leg::GetJntAngFromEndPos(int jntID)
{
	if ( !IsParamWithin(jntID, 1, JOINT_NUM+1) )
	{
		fprintf(stderr, "Leg::GetJntAngFromLegEndPos Invalid Matrix\n\n");
		return (0.0);
	}

	KINE kine;
	double theta;
	double C3, S3;
	double a, b, c, d;

	Matrix Trans(DH_NUM, DH_NUM);		// 脚ベース座標系での脚先の同次変換行列
	Trans.LoadIdentity();

	kine = CheckReachRange();
	
	if (kine == KineErrSingular)
	{
		exit(1);
	}

	// 脚ベース座標系への変換
	Trans = InvCoordinateTransformation(0)*Foot;

	switch (jntID)
	{
		case 1:
			theta = atan2(Indicator[0]*Trans(2,4), Indicator[0]*Trans(1,4));
			break;

		case 2:
			C3 = (Trans(1,4)*Trans(1,4)+Trans(2,4)*Trans(2,4)+(Trans(3,4)-Link[0]+FOOT)*(Trans(3,4)-Link[0]+FOOT)
							-Link[1]*Link[1]-Link[2]*Link[2])/(2*Link[1]*Link[2]);
			S3 = -Indicator[1]*sqrt(1-C3*C3);
			a = Link[1]+Link[2]*C3;
			b = Link[2]*S3;
			c = Indicator[0]*sqrt(Trans(1,4)*Trans(1,4)+Trans(2,4)*Trans(2,4));
			d = Trans(3,4)-Link[0]+FOOT;

			theta = atan2((a*d-b*c), (a*c+b*d));
			break;

		case 3:
			C3 = (Trans(1,4)*Trans(1,4)+Trans(2,4)*Trans(2,4)+(Trans(3,4) - Link[0])*(Trans(3,4) - Link[0])
							-Link[1]*Link[1]-Link[2]*Link[2])/(Link[1]*Link[2]);
			
			theta = atan2(-Indicator[1]*sqrt(1-C3*C3), C3);
			break;

		case 4:
		{
			double theta2=0;
			double theta3=0;

			C3 = (Trans(1,4)*Trans(1,4)+Trans(2,4)*Trans(2,4)+(Trans(3,4)-Link[0]+FOOT)*(Trans(3,4)-Link[0]+FOOT)
							-Link[1]*Link[1]-Link[2]*Link[2])/(2*Link[1]*Link[2]);
			S3 = -Indicator[1]*sqrt(1-C3*C3);
			a = Link[1]+Link[2]*C3;
			b = Link[2]*S3;
			c = Indicator[0]*sqrt(Trans(1,4)*Trans(1,4)+Trans(2,4)*Trans(2,4));
			d = Trans(3,4)-Link[0]+FOOT;

			theta3 = atan2(S3, C3);
			theta2 = atan2((a*d-b*c), (a*c+b*d));

            theta = -( PI/2 + theta2 + theta3 );
		}

		default:
                        {
                        return (0.0);//脚番号異常
			//break;
                        }
	}
  
  return theta;
}

/*********************************************************
**	以下は運動学に関する関数を用いた関数群(頻繁に使う)  **
*********************************************************/

/*******************************************************************
**	脚の構成をセット (逆キネを解いた後順キネを解く)
*******************************************************************/
KINE Leg::SetConfiguration(const Matrix& target)
{
	KINE kine1, kine2;

	kine1 = SetEndPos(target);

	// 脚先が到達できたら
	if (kine1 == NoKineError)
	{
		// 順運動学を解く
		kine2 = CalcDirectKinematics();

		return (kine2);
	}

	return (kine1);

}

/*******************************************************************
**	脚先の同次変換行列をセット
*******************************************************************/
/*************************************************
**	足裏は脚先から真下の位置(足裏は本体と平行)　**
*************************************************/
KINE Leg::SetEndPos(const Matrix& endPos)
{
	Matrix tmp(DH_NUM, DH_NUM);
	tmp.LoadIdentity();

	KINE kine;

	// 引数の行列の要素数をチェック
	if (endPos.GetRow() !=DH_NUM || endPos.GetCol() != DH_NUM)
	{
		fprintf(stderr, "Leg::SetLegEndPos Invalid Matrix\n\n");
		return (KineErrSome);
	}

	tmp = Foot;
	Foot = endPos;

	kine = CalcInverseKinematics();

if(kine==NoKineError)return kine;//060719 doi.エラーが無い場合

    Foot=tmp;//060717 doi.エラーがある場合．
    return kine;
    
    /*
	switch (kine)
        {
		case (NoKineError):
    		break;
    	
    	case (KineErrReach):
    		Foot = tmp;
    		break;
    	
    	case (KineErrSingular):
    		Foot = tmp;
    		break;
    	
    	case (KineErrSome):
			Foot = tmp;
			break;

		default:
			break;
    }
  
	return kine;
*/
}

/*******************************************************************
**	各関節角を同時にセット
*******************************************************************/
KINE Leg::SetJntAng(const Matrix& jntAng)
{
	Matrix tmp(JOINT_NUM);
	KINE kine;
	
	// 要素数のチェック
	if (jntAng.GetRow() != JOINT_NUM || jntAng.GetCol() != 1)
	{
		fprintf(stderr, "Leg::SetJntAng Invalid Matrix\n\n");
		return (KineErrSome);
	}

	// 現在の関節角を一時的に保持
	tmp = JntAng;
	JntAng = jntAng;
	
	// 順運動学を解く
	kine = CalcDirectKinematics();
	
	switch (kine)
    {
    	case (NoKineError):
			break;

    	case (KineErrReach):
			JntAng = tmp;
			break;

    	case (KineErrSingular):
			JntAng = tmp;
			break;
    		
    	case (KineErrSome):
			JntAng = tmp;
                        break;

		default:
        		JntAng = tmp;//NoKineErrorでないので戻す．
			break;
    }
    
    return kine;

}

/*******************************************************************
**	現在のJntAngベクトルでJntStatマトリクスを更新
*******************************************************************/
void Leg::UpdateJntState(void)
{
	for (int i=1; i<(JOINT_NUM+1); i++)
		Transformation[i] = Transformation[i-1]*CoordinateTransformation(i);

	return;
}

/*******************************************************************
**	脚先にかかる力をセット
*******************************************************************/
KINE Leg::SetFootForce(const Matrix& force)
{
	Matrix tmp(POS_DOF);
	KINE kine;
	
	// 要素数のチェック
	if ( (force.GetRow() != POS_DOF) || (force.GetCol() != 1) )
	{
		fprintf(stderr, "Leg::SetFootForce Invalid Matrix\n\n");
		return (KineErrSome);
	}

	tmp = FootForce;
	FootForce = force;

	kine = CalcCylinderThrust();

	switch (kine)
	{
		case (NoKineError):
			break;

		case (KineErrReach):
			FootForce = force;
			break;

		case (KineErrSingular):
			FootForce = force;
			break;

		case (KineErrSome):
			FootForce = force;
			break;

		default:
			break;
	}

	return kine;
}

/*******************************************************************
**	以下はメンバ変数が指定した範囲内にあるかどうかのチェック
*******************************************************************/

/*******************************************************************
**	シリンダ長さのチェック
*******************************************************************/
KINE Leg::CheckCylinderRange(int jntID)
{
int i=jntID-1;
//  for (int i=0; i<JOINT_NUM; i++)
	{
          //printf("CldrLen(%d+1)=%f\n",i,CldrLen(i+1));//debug
          //printf("CldrLmtin[%d]=%f\n",i,CldrLmtMin[i]);//debug          
        if ( (CldrLen(i+1) < CldrLmtMin[i]) )
		{
			LastJntErrNum = (i+1);

			return (KineErrCylinderUnderLimit);
		}
		else if ( (CldrLen(i+1) > CldrLmtMax[i]) )
		{
			LastJntErrNum = (i+1);

			return (KineErrCylinderOverLimit);
		}
	}
    
	return (NoKineError);
}

KINE Leg::CheckCylinderRange(void)
{
  KINE kn;
	for (int i=0; i<JOINT_NUM; i++)
	{
          kn=CheckCylinderRange(i);
          if(kn!=NoKineError)
            return kn;
	}
    
	return (NoKineError);
}

/*******************************************************************
**	脚先にかかる力のチェック
*******************************************************************/
KINE Leg::CheckFootForce(void)
{
	for (int i=0; i<POS_DOF; i++)
	{
        if ( (FootForce(i+1) < FootFrcLmtMin[i]) )
		{
			return (KineErrForceUnderLimit);
		}
		else if ( (FootForce(i+1) > FootFrcLmtMax[i]) )
		{
			return (KineErrForceOverLimit);
		}
	}

	return (NoKineError);
}

/*******************************************************************
**	シリンダ推力のチェック
*******************************************************************/
KINE Leg::CheckCylinderThrust(void)
{
	for (int i=0; i<JOINT_NUM; i++)
	{
        if ( (CldrThrust(i+1) < CldrThrustMin[i]) )
		{
			/*
			printf("CheckCylinderThrust() - CldrThrust(%d)=%.2lf < CldrThrustMin[%d]=%.2lf\n",
			i+1,CldrThrust(i+1),i,CldrThrustMin[i]
			);//060620 doi
			*/
			LastJntErrNum = (i+1);

			return (KineErrCldrThrustUnderLimit);
		}
		else if ( (CldrThrust(i+1) > CldrThrustMax[i]) )
		{
			/*
			printf("CheckCylinderThrust() - CldrThrust(%d)=%.2lf > CldrThrustMax[%d]=%.2lf\n",
			i+1,CldrThrust(i+1),i,CldrThrustMin[i]
			);//060620 doi
			*/
			LastJntErrNum = (i+1);

			return (KineErrCldrThrustOverLimit);
		}
	}

	return (NoKineError);
}



/*********************************************
**	以下はヤコビ計算とそれに関係する関数群  **
*********************************************/

/*******************************************************************
**	ヤコビ行列の計算
**	足首分まで(3自由度だけ)
*******************************************************************/
void Leg::CalcJacobian(void)
{
	// シリンダ長さによる関節角の変化量(偏微分値)  
	double A[POS_DOF];

	if (LegID == 2 || LegID == 3)
	{
		A[0] = DiffArcCosine(CosLawtoAng(DTC3, DTC6, CldrLen(1)))*(-CldrLen(1)/(DTC3*DTC6));
	}
	else if (LegID == 1 || LegID == 4)
	{
		A[0] = -DiffArcCosine(CosLawtoAng(DTC3, DTC6, CldrLen(1)))*(-CldrLen(1)/(DTC3*DTC6));
	}
        else
        {
            //LegID異常
            return;
        }

	A[1] = DiffArcCosine(CosLawtoAng(DTC9, DTC12, CldrLen(2)))*(-CldrLen(2)/(DTC9*DTC12));
	
	A[2] = DiffArcCosine(CosLawtoAng(DTC15, DTC18, CldrLen(3)))*(-CldrLen(3)/(DTC15*DTC18));

	// シリンダ→関節角のヤコビを計算
	JacobianL(1,1) = A[0];
	JacobianL(2,2) = A[1];
	JacobianL(3,3) = A[2];

	// 関節角→先端位置のヤコビを計算
	Arm::CalcJacobian();

	// 完全なヤコビを計算
	Jacobian = JacobianA*JacobianL;

	return;
}

/*******************************************************************
**	逆ヤコビ行列の計算
*******************************************************************/
void Leg::CalcInverseJacobian(void)
{
	CalcJacobian();
	InvJacobian = Jacobian.Inversion();

	return;
}

/***********************************************************************************************
**	脚先にかかる力からシリンダ推力を計算
***********************************************************************************************/
KINE Leg::CalcCylinderThrust(void)
{
	KINE kine;

	kine = CheckFootForce();

	if (kine == NoKineError)
	{
		CldrThrust = (Jacobian.Transposition() * FootForce);

		kine = CheckCylinderThrust();
	}

	return kine;
}

/***********************************************
**	以下はシリンダ長と関節角に関係する関数群  **
***********************************************/

/*******************************************************************
**	シリンダ長(引数)から各関節角の値だけを得る
*******************************************************************/
double Leg::GetAngFromLen(int jntID, double len)
{
	double theta;
	theta=0;//070207 doi

	if ( !IsParamWithin(jntID, 1, JOINT_NUM) )
	{
		fprintf(stderr, "Leg::GetAngFromLen Invalid Joint ID\n\n");
		return (0.0);
	}

	switch (jntID)
	{
		case 1:
			switch (Leg::LegID)
			{
				case 1:
					theta = PI - atan2(DTC1, DTC2) - CosLawtoAng(DTC3, DTC6, len) - atan2(DTC5, DTC4) - BaseParameter(4);
					break;

				case 2:
					theta = atan2(DTC1, DTC2) + CosLawtoAng(DTC3, DTC6, len) + atan2(DTC5, DTC4) - PI - BaseParameter(4);
					break;

				case 3:
					theta = atan2(DTC1, DTC2) + CosLawtoAng(DTC3, DTC6, len) + atan2(DTC5, DTC4) - PI + (PI - BaseParameter(4));
					break;

				case 4:
					theta = PI - atan2(DTC1, DTC2) - CosLawtoAng(DTC3, DTC6, len) - atan2(DTC5, DTC4) - (PI + BaseParameter(4));
					break;

				default:
					break;
			}

			break;

		case 2:
			theta = CosLawtoAng(DTC9, DTC12, len) +atan2(DTC7, DTC8) - atan2(DTC11, DTC10) - PI/2;
			break;

		case 3:
			theta = CosLawtoAng(DTC15, DTC18,len) + atan2(DTC17, DTC16) - atan2(DTC14, DTC13) - PI;
			break;

		default:
			break;
	}

	return theta;
}

/*******************************************************************
**	シリンダ長(CldrLenベクトル)から各関節角を計算し代入
*******************************************************************/
KINE Leg::CalcAngFromLen(int jntID)
{
	KINE kine;

	if ( !IsParamWithin(jntID, 1, JOINT_NUM) )
	{
		fprintf(stderr, "Leg::CalcAngFromLen Invalid Joint ID\n\n");
		return (KineErrSome);
	}

	JntAng(jntID) = GetAngFromLen(jntID, CldrLen(jntID));

//	kine = CheckJointRange();
	kine = CheckJointRange(jntID);//060125 doi

	return (kine);
}

/*******************************************************************
**	CalcAngFromLenを全て行う
*******************************************************************/
KINE Leg::CalcAngFromLen(void)
{
	KINE kine;

	for (int i=1; i<=JOINT_NUM; i++)
	{
		kine = CalcAngFromLen(i);

		if (kine != NoKineError)
			return (KineErrReach);
	}

	return (kine);
}

/*******************************************************************
**	関節角(引数)からシリンダ長の値だけを得る
*******************************************************************/
double Leg::GetLenFromAng(int jntID, double ang)
{
	double length;
	double theta;

	if ( !IsParamWithin(jntID, 1, JOINT_NUM) )
	{
		fprintf(stderr, "Leg::GetLenFromAng Invalid joint ID\n\n");
		return (0.0);
	}

	switch (jntID)
	{
		case 1:
			switch (Leg::LegID)
			{
				case 1:
					theta = PI - ang - atan2(DTC1, DTC2) - atan2(DTC5, DTC4) - BaseParameter(4);
					length = CosLawtoLen(DTC3, DTC6, theta);
					break;

				case 2:
					theta = ang - atan2(DTC1, DTC2) - atan2(DTC5, DTC4) + PI + BaseParameter(4);
					length = CosLawtoLen(DTC3, DTC6, theta);
					break;

				case 3:
					theta =ang - atan2(DTC1, DTC2) - atan2(DTC5, DTC4) + PI - (PI - BaseParameter(4));
					length = CosLawtoLen(DTC3, DTC6, theta);
					break;

				case 4:
					theta = PI -ang - atan2(DTC1, DTC2) -  atan2(DTC5, DTC4) + (PI - BaseParameter(4));
					length = CosLawtoLen(DTC3, DTC6, theta);
					break;

				default:
                                        return (0.0);
					break;
			}

			break;

		case 2:
			theta = ang - atan2(DTC7, DTC8) + atan2(DTC11, DTC10) + PI/2;
			length = CosLawtoLen(DTC9, DTC12, theta);
			break;

		case 3:
			theta = ang - atan2(DTC17, DTC16) + atan2(DTC14, DTC13) + PI;
			length =  CosLawtoLen(DTC15, DTC18, theta);
			break;

		default:
                        {
                        //jntID異常
                        return (0.0);
			//break;
                        }
	}

	return length;
}

/*******************************************************************
**	関節角(JntAngベクトル)からシリンダ長の値を計算し代入
*******************************************************************/
KINE Leg::CalcLenFromAng(int jntID)
{
	KINE kine;
//printf("---------CalcLenFromAng(%d)-----------\n",jntID);//debug
	if ( !IsParamWithin(jntID, 1, JOINT_NUM) )
	{
		fprintf(stderr, "Leg::CalcLenFromAng Invalid Joint ID\n\n");
		return (KineErrSome);
	}

//	CldrLen(jntID) = GetLenFromAng(jntID, JntAng(jntID));
	double len = GetLenFromAng(jntID, JntAng(jntID));  //051006 doi
  //          printf("len(%d)=%f\n",jntID,len);//051006 doi.debug
  if(len<10.0)
          {
//            fprintf(stderr, "CalcLenFromAng(%d) Error len=%f\n",jntID,len);//051006 doi
            //printf("CalcLenFromAng(%d) Error len=%f\n",jntID,len);//051006 doi
              ;
          }
  CldrLen(jntID)=len;
  
//	kine = CheckCylinderRange();
  	kine = CheckCylinderRange(jntID);

	return (kine);
}

/*******************************************************************
**	CalcLenFromAngを全て行う
*******************************************************************/
KINE Leg::CalcLenFromAng(void)
{
	KINE kine;
//printf("---------CalcLenFromAng-----------\n");//debug
  
	for (int i=1; i<=JOINT_NUM; i++)
	{
		kine = CalcLenFromAng(i);

		if (kine != NoKineError)
                  {
                    //printf("CalcLenFromAng(%d) Error", i);
			return (KineErrReach);
                  }
	}

	return (kine);

}

/*******************************************************************
**	可動範囲の内側を得る
*******************************************************************/
double Leg::GetInnerMotionRange(double height)
{
	double range=0;

	// 足首の高さに変換してから
//	int param = RoundOff( (height+FOOT), 2);
	int param = RoundOff( (height+FOOT), 1);    
    //printf("GetInnerMotionRange():(height, param)=(%.2f, %d)\n",height, param);//debug
	switch (param)
	{
		case -1700:
			range = 1427.0;
			break;

		case -1600:
			range = 1243.0;
			break;

		case -1500:
			range = 1027.0;
			break;

		case -1400:
			range = 866.0;
			break;

		case -1300:
			range = 735.0;
			break;

		case -1200:
			range = 625.0;
			break;

		case -1100:
			range = 531.0;
			break;

		case -1000:
			range = 451.0;
			break;

		case -900:
			range = 381.0;
			break;

		case -800:
			range = 321.0;
			break;

		case -700:
			range = 269.0;
			break;

		case -600:
			range = 209.0;
			break;

		case -500:
			range = 414.0;
			break;

		case -400:
			range = 545.0;
			break;

		case -300:
			range = 634.0;
			break;

		case -200:
			range = 698.0;
			break;

		case -100:
			range = 743.0;
			break;

		case 0:
			range = 770.0;
			break;

		case 100:
			range = 788.0;
			break;

		case 200:
			range = 956.0;
			break;

		case 300:
			range = 1308.0;
			break;

		case 400:
			range = 1519.0;
			break;

		case 500:
			range = 1619.0;
			break;

		case 600:
			range = 1808.0;
			break;

		case 700:
			range = 1917.0;
			break;

		case 800:
			range = 2009.0;
			break;

		case 900:
			range = 2089.0;
			break;

		case 1000:
			range = 2158.0;
			break;

		case 1100:
			range = 2218.0;
			break;

		case 1200:
			range = 2269.0;
			break;

		case 1300:
			range = 2313.0;
			break;

		case 1400:
			range = 2350.0;
			break;

		case 1500:
			range = 2381.0;
			break;

		default:
			range = 0;
			break;
	}

	return (range+200.0);
}

/*******************************************************************
**	可動範囲の外側を得る
*******************************************************************/
double Leg::GetOuterMotionRange(double height)
{
	double range=0;

	// 足首の高さに変換してから
//	int param = RoundOff( (height+FOOT), 2);
	int param = RoundOff( (height+FOOT), 1);    

	switch (param)
	{
		case -1700:
			range = 2164.0;
			break;

		case -1600:
			range = 2539.0;
			break;

		case -1500:
			range = 2754.0;
			break;

		case -1400:
			range = 2916.0;
			break;

		case -1300:
			range = 3047.0;
			break;

		case -1200:
			range = 3157.0;
			break;

		case -1100:
			range = 3207.0;
			break;

		case -1000:
			range = 3247.0;
			break;

		case -900:
			range = 3285.0;
			break;

		case -800:
			range = 3318.0;
			break;

		case -700:
			range = 3349.0;
			break;

		case -600:
			range = 3376.0;
			break;

		case -500:
			range = 3400.0;
			break;

		case -400:
			range = 3421.0;
			break;

		case -300:
			range = 3439.0;
			break;

		case -200:
			range = 3454.0;
			break;

		case -100:
			range = 3466.0;
			break;

		case 0:
			range = 3475.0;
			break;

		case 100:
			range = 3481.0;
			break;

		case 200:
			range = 3484.0;
			break;

		case 300:
			range = 3483.0;
			break;

		case 400:
			range = 3478.0;
			break;

		case 500:
			range = 3478.0;
			break;

		case 600:
			range = 3460.0;
			break;

		case 700:
			range = 3447.0;
			break;

		case 800:
			range = 3430.0;
			break;

		case 900:
			range = 3411.0;
			break;

		case 1000:
			range = 3388.0;
			break;

		case 1100:
			range = 3363.0;
			break;

		case 1200:
			range = 3334.0;
			break;

		case 1300:
			range = 3302.0;
			break;

		case 1400:
			range = 3266.0;
			break;

		case 1500:
			range = 3227.0;
			break;

		default:
			range = 0;
			break;
	}

	return (range-200.0);
}
