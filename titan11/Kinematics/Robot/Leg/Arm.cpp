/*****************************************************************************
**
**	File:	Arm.cpp
** 
**	説明:	TITAN XIの単腕の運動学の実行部
**
**	Created: 2004/10/28(Thu)	Updated: 2005/01/14(Fri)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

#include "Arm.h"

/******************************
**  デフォルトコンストラクタ
******************************/
Arm::Arm()
{
	InitArmParameter();

	SetBaseParameter(0, 0, 0, 0);

}

/******************************
**  コンストラクタ
******************************/
Arm::Arm(double x, double y, double z, double theta)
{
	InitArmParameter();

	SetBaseParameter(x, y, z, theta);
}


/******************************
**  デストラクタ
******************************/
Arm::~Arm()
{
	delete [] Transformation;
	delete [] AngLmtMin;
	delete [] AngLmtMax;
	delete [] Link;
}

/************************************************************************
**	パラメータの初期化
************************************************************************/
void Arm::InitArmParameter(void)
{
// 以下でメンバ変数の初期化を行う

// 配列の要素数確保
	Transformation	=	new Matrix[JOINT_NUM+1];
	AngLmtMin		=	new double[JOINT_NUM];
	AngLmtMax		=	new double[JOINT_NUM];
	Link			=	new double[JOINT_NUM];

// マトリクス, ベクトルの要素数設定
int i;
  for (i=0; i<(JOINT_NUM+1); i++)
		Transformation[i].SetSize(DH_NUM, DH_NUM);

	JacobianA.SetSize(POS_DOF, POS_DOF);
	InvJacobianA.SetSize(POS_DOF, POS_DOF);

	JntAng.SetSize(JOINT_NUM);
	JntAngVel.SetSize(JOINT_NUM);
	JntTorque.SetSize(JOINT_NUM);
	EndForce.SetSize(POS_DOF, POS_DOF);

	BaseParameter.SetSize(4);
	
// 関節に関する変数の初期化
// Matrix
	// デフォルトは恒等変換
	for (i=0; i<(JOINT_NUM+1); i++)
		Transformation[i].LoadIdentity();

	JacobianA.LoadZero();
	InvJacobianA.LoadZero();
	JntAng.LoadZero();
	JntAngVel.LoadZero();
	JntTorque.LoadZero();
	EndForce.LoadZero();
	BaseParameter.LoadZero();

	// 限界値設定
	AngLmtMin[0]	= ANGLE1_MIN*DEG2RAD;
	AngLmtMin[1]	= ANGLE2_MIN*DEG2RAD;
	AngLmtMin[2]	= ANGLE3_MIN*DEG2RAD;

	AngLmtMax[0]	= ANGLE1_MAX*DEG2RAD;
	AngLmtMax[1]	= ANGLE2_MAX*DEG2RAD;
	AngLmtMax[2]	= ANGLE3_MAX*DEG2RAD;

	EndFrcLmtMin[0] = FORCE_X_MIN;
	EndFrcLmtMin[1] = FORCE_Y_MIN;
	EndFrcLmtMin[2] = FORCE_Z_MIN;

	EndFrcLmtMax[0] = FORCE_X_MAX;
	EndFrcLmtMax[1] = FORCE_Y_MAX;
	EndFrcLmtMax[2] = FORCE_Z_MAX;

	ReachLmtMin[0]	= REACH_RADIUS_MIN;
	ReachLmtMin[1]	= REACH_HEIGHT_MIN;

	ReachLmtMax[0]	= REACH_RADIUS_MAX;
	ReachLmtMax[1]	= REACH_HEIGHT_MAX;

	Link[0]			= LINK1;
	Link[1]			= LINK2;
	Link[2]			= LINK3;

	// 姿勢指数はデフォルト(前向き，上肘)
	SetOriIndicator();

	// 最初はエラー無
	LastJntErrNum = 0;
}

/************************************************************************
**	各Transformationマトリクスを得る
************************************************************************/
Matrix Arm::GetTransformMatrix(int jntID) const
{
	if ( !IsParamWithin(jntID, 0, JOINT_NUM) )
	{
		fprintf(stderr, "Leg::GetJntStat Invalid joint number\n\n");
		
		return (IDENTITY_MATRIX_DH);
	}

	return (Transformation[jntID]);
}

/***********************************
**  以下は座標変換に関する関数群  **
***********************************/

/************************************************************************
**  同次変換行列の計算
************************************************************************/
Matrix Arm::CoordinateTransformation(int jntID) const
{
/**************************************************************************************************************************
**	A(1,1) = cos(theta);	A(1,2) = -cos(alpha)*sin(theta);	A(1,3) = sin(alpha)*sin(theta);		A(1,4) = a*cos(theta);
**	A(2,1) = sin(theta);	A(2,2) = cos(alpha)*cos(theta);		A(2,3) = -sin(alpha)*cos(theta);	A(2,4) = a*sin(theta);
**	A(3,1) = 0;				A(3,2) = sin(alpha);				A(3,3) = cos(alpha);				A(3,4) = d;
**	A(4,1) = 0;				A(4,2) = 0;							A(4,3) = 0;							A(4,4) = 1;
**************************************************************************************************************************/
/*************************
**	(n, s, a, p)		**
**	(0, 0, 0, 1)		**
**						**
**	(nx, sx, ax, px)	**
**	(ny, sy, ay, py)	**
**	(nz, sz, az, pz)	**
**	(0,  0,  0,  1 )	**
*************************/
	if ( !IsParamWithin(jntID, 0, JOINT_NUM) )
	{
		fprintf(stderr, "Leg::TransformMatrix Invalid Joint Number\n\n");
		
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

		default:
			break;
	}

	return A;
}

/************************************************************************
**	逆同次変換行列の計算
************************************************************************/
Matrix Arm::InvCoordinateTransformation(int jntID) const
{
/**************************************************************************************************************************
**	A(1,1) = cos(theta);				A(1,2) = sin(theta);				A(1,3) = 0;				A(1,4) = -a;
**	A(2,1) = -cos(alpha)*sin(theta);	A(2,2) = cos(alpha)*cos(theta);		A(2,3) = sin(alpha);	A(2,4) = -d*sin(alpha);
**	A(3,1) = sin(alpha)*sin(theta);		A(3,2) = -sin(alpha)*cos(theta);	A(3,3) = cos(alpha);	A(3,4) = -d*cos(alpha);
**	A(4,1) = 0;							A(4,2) = 0;							A(3,4) = 0;				A(4,4) = 1;
**************************************************************************************************************************/
/*****************************************
**	(nT, -nT*p)							**
**	(sT, -sT*p)							**
**	(aT, -aT*P)							**
**	(0,   1   )							**
**										**
**	(nx, ny, nz, -(nx*px+ny*py+nz*pz))	**
**	(sx, sy, sz, -(sx*px+sy*py+sz*pz))	**
**	(ax, ay, az, -(ax*px+ay*py+az*pz))	**
**	(0,  0,  0,  1 )					**
*****************************************/

	if ( !IsParamWithin(jntID, 0, JOINT_NUM) )
	{
		fprintf(stderr, "Leg::InvTransformMatrix Invalid Joint Number\n\n");

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

		default:
			break;
	}

	return A;
}

/***********************************************************************************
**	BaseParameterマトリクスへの値のセット
**	x, y, zのオフセットと回転角度のセット  
***********************************************************************************/
void Arm::SetBaseParameter(double x, double y, double z, double theta)
{
	BaseParameter(1) = x;
	BaseParameter(2) = y;
	BaseParameter(3) = z;
	BaseParameter(4) = theta;

	return;

}

/*******************************
**	以下は運動学に関する計算  **
*******************************/
/*******************************************************************
**	脚の姿勢指標をセット
*******************************************************************/
bool Arm::SetOriIndicator(int front, int up)
{
	if (abs(front) != 1 || abs(up) != 1)
	{
		fprintf(stderr, "Arm::SetOriIndicator(): Invalid parameter\n\n");
		return (false);
	}

	Indicator[0] = front;
	Indicator[1] = up;

	return (true);

}

/*******************************************************************
**	順運動学の計算
*******************************************************************/
KINE Arm::CalcDirectKinematics(void)
{
	// 結果判定用のキネマティクス
	KINE kine;

	// 関節が可動限界内にあるか
	kine = CheckJointRange();

	if (kine == NoKineError)
	{

		// 各同次変換行列の計算
		Transformation[0] = CoordinateTransformation(0);
		for (int i=1; i<(JOINT_NUM+1); i++)
			Transformation[i] = Transformation[i-1]*CoordinateTransformation(i);

		// 設定した可動範囲内に脚先があるか
		kine = CheckReachRange();

	}
	else
	{
		fprintf(stderr, "Arm::CalcDirectKinematics() Error");
	}

	return kine;
}

/*******************************************************************
**	逆運動学の計算
**	解析的に解くため, 手首点から解く
**	実際の軌道を考える時には, 手首リンク分だけオフセットするべき
*******************************************************************/
/***********************
**	足裏は本体と平行  **
***********************/
KINE Arm::CalcInverseKinematics(void)
{
	KINE kine;

	double temp;					// 第1関節角のデータ保持用
	double C3, S3;					// 関節角度3によるsin, cos
	double a, b, c, d;				// 関節角度2を計算するための一時的な変数
	
	Matrix Trans(DH_NUM, DH_NUM);	// 脚ベース座標系での脚先の同次変換行列
	Trans.LoadIdentity();			// Control Point Status
	
	// // 脚ベース座標系に変換
	Trans = InvCoordinateTransformation(0)*Transformation[POS_DOF];

	kine = CheckReachRange();

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
		C3 = (Trans(1,4)*Trans(1,4)+Trans(2,4)*Trans(2,4)+(Trans(3,4) - Link[0])*(Trans(3,4) - Link[0])
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
		d = Trans(3,4)-Link[0];

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
double Arm::GetJntAngFromEndPos(int jntID)
{
	if ( !IsParamWithin(jntID, 1, JOINT_NUM) )
	{
		fprintf(stderr, "Leg::GetJntAngFromEndPos Invalid Matrix\n\n");

		return (0.0);
	}

	KINE kine;
	double theta;
	double C3, S3;
	double a, b, c, d;

	theta=0;//070207 doi
	
	Matrix Trans(DH_NUM, DH_NUM);		// 脚ベース座標系での脚先の同次変換行列
	Trans.LoadIdentity();

	kine = CheckReachRange();
	
	if (kine == KineErrSingular)
	{
		fprintf(stderr, "Leg::GetJntAngFromEndPos() Singular\n\n");
		return (0.0);
	}

	// 脚ベース座標系への変換
	Trans = CoordinateTransformation(0)*Transformation[POS_DOF];

	switch (jntID)
	{
		case 1:
			theta = atan2(Indicator[0]*Trans(2,4), Indicator[0]*Trans(1,4));
			break;

		case 2:
			C3 = (Trans(1,4)*Trans(1,4)+Trans(2,4)*Trans(2,4)+(Trans(3,4)-Link[0])*(Trans(3,4)-Link[0])
							-Link[1]*Link[1]-Link[2]*Link[2])/(2*Link[1]*Link[2]);
			S3 = -Indicator[1]*sqrt(1-C3*C3);
			a = Link[1]+Link[2]*C3;
			b = Link[2]*S3;
			c = Indicator[0]*sqrt(Trans(1,4)*Trans(1,4)+Trans(2,4)*Trans(2,4));
			d = Trans(3,4)-Link[0];

			theta = atan2((a*d-b*c), (a*c+b*d));
			break;

		case 3:
			C3 = (Trans(1,4)*Trans(1,4)+Trans(2,4)*Trans(2,4)+(Trans(3,4) - Link[0])*(Trans(3,4) - Link[0])
							-Link[1]*Link[1]-Link[2]*Link[2])/(Link[1]*Link[2]);
			
			theta = atan2(-Indicator[1]*sqrt(1-C3*C3), C3);
			break;

		default:
			break;
	}
  
  return theta;
}

/*********************************************************
**	以下は運動学に関する関数を用いた関数群(頻繁に使う)  **
*********************************************************/

/*******************************************************************
**	腕先の同次変換行列をセット(脚先の同次変換行列を指定)
*******************************************************************/
KINE Arm::SetEndPos(const Matrix& endPos)
{
	Matrix tmp(DH_NUM, DH_NUM); tmp.LoadIdentity();
	KINE kine;

	// 引数のチェック
	if ( (endPos.GetRow() != DH_NUM) || (endPos.GetCol() != DH_NUM) )
	{
		fprintf(stderr, "Leg::SetEndPos Invalid Matrix\n\n");
		
		return (KineErrSome);
	}

	tmp = Transformation[POS_DOF];
	Transformation[POS_DOF] = endPos;

	kine = CalcInverseKinematics();

    if(kine==NoKineError)return kine;//060720 doi

    Transformation[POS_DOF] = tmp;//060720 doi
    return kine;//060720 doi
    
    /*
	switch (kine)
    {
	case (NoKineError):
    		break;
    	
    	case (KineErrReach):
    		Transformation[POS_DOF] = tmp;
    		break;
    	
    	case (KineErrSingular):
    		Transformation[POS_DOF] = tmp;
    		break;
    	
    	case (KineErrSome):
			Transformation[POS_DOF] = tmp;
			break;

		default:
			break;
    }
  */
	return kine;
}

/*******************************************************************
**	各関節角を同時にセット
*******************************************************************/
KINE Arm::SetJntAng(const Matrix& jntAng)
{
	Matrix tmp(JOINT_NUM);
	KINE kine;
	
	// 引数チェック
	if ( (jntAng.GetRow() != JOINT_NUM) || (jntAng.GetCol() != 1) )
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
			break;
    }
    
    return kine;

}

/*******************************************************************
**	脚の構成をセット (逆キネを解いた後順キネを解く)
*******************************************************************/
KINE Arm::SetConfiguration(const Matrix& target)
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
**	現在のJntAngベクトルでJntStatマトリクスを更新
*******************************************************************/
void Arm::UpdateJntState(void)
{
	for (int i=1; i<JOINT_NUM+1; i++)
		Transformation[i] = Transformation[i-1]*CoordinateTransformation(i);

	return;
}

/*******************************************************************
**	以下はメンバ変数が指定した範囲内にあるかどうかのチェック
*******************************************************************/

/*******************************************************************
**	脚先が到達範囲内にあるかどうかのチェック
**　引数を与えた場合　胴体座標系での足首位置
**　引数無し　Transformation[POS_DOF]をそのまま使う
*******************************************************************/
KINE Arm::CheckReachRange(Matrix &Trans_in)//060720 doi
{
    //KINE kine;

    double x=Trans_in(1,4);//060720 doi
    double y=Trans_in(2,4);//060720 doi
    double z=Trans_in(3,4);//060720 doi
    double r=sqrt(x*x+y*y);//060719 doi

	// 脚の到達半径の最低値チェック
	if (r< ReachLmtMin[0])
	{
		return (KineErrUnderReachRadius);
	}
	// 脚の到達半径の最高値チェック
	else if (r > ReachLmtMax[0])
	{
		return (KineErrOverReachRadius);
	}
	// 脚の高さ方向(z方向)チェック
	else if (z < ReachLmtMin[1])	// 高さ方向の範囲
	{
        return (KineErrUnderReachHeight);
	}
	else if(z > ReachLmtMax[1])
	{
		return (KineErrOverReachHeight);
	}

    return (NoKineError);
}

KINE Arm::CheckReachRange(void)
{
#ifndef DEBUG
KINE kine;
	Matrix Trans(DH_NUM, DH_NUM);			// 脚先の脚のベース座標系での同次変換行列
	
	// ここで脚先を脚のベース座標系に変換
	Trans = InvCoordinateTransformation(0)*Transformation[POS_DOF];
     kine=CheckReachRange(Trans);    
    return kine;
#endif

	return (NoKineError);
}

/*******************************************************************
**	関節角度のチェック
*******************************************************************/
KINE Arm::CheckJointRange(void)
{
	int i;
  KINE kn;
  
	for (i=1; i<=JOINT_NUM; i++)
	{
          kn=CheckJointRange(i);
          if(kn!=NoKineError)
            {
              //fprintf(stderr,"CheckJointRange(%d) Error#%d.\n",i,kn);//051006 doi
              return kn;
            }
        }
  return kn;
 }
//------------------------------------------------------------
KINE Arm::CheckJointRange(int jntID)//関節個別にチェック
{
	int i=jntID-1;

        if ( (JntAng(i+1) < AngLmtMin[i]) )
	{
                 printf("JntAng(%d+1)<AngLmtMin[%d]=(%f,%f)\n",i,i,JntAng(i+1),AngLmtMin[i]);//051006 doi.debug
                 LastJntErrNum = (i+1);
		return (KineErrJointUnderLimit);
	}
	else if ( (JntAng(i+1) > AngLmtMax[i]) )
	{
                 printf("JntAng(%d+1)>AngLmtMax[%d]=(%f,%f)\n",i,i,JntAng(i+1),AngLmtMax[i]);//051006 doi.debug
                 LastJntErrNum = (i+1);
		return (KineErrJointOverLimit);
	}

	LastJntErrNum = 0;
//  printf("CheckJointRange OK.ID=%d\n",jntID);//051006 doi debug
	return (NoKineError);
}


/*******************************************************************
**	脚先にかかる力のチェック
*******************************************************************/
KINE Arm::CheckEndForce(void)
{
	int i;

	for (i=0; i<POS_DOF; i++)
	{
        if ( (EndForce(i+1) < EndFrcLmtMin[i]) )
		{
			return (KineErrForceUnderLimit);
		}
		else if ( (EndForce(i+1) > EndFrcLmtMax[i]) )
		{
			return (KineErrForceOverLimit);
		}
	}

	return (NoKineError);
}

/*******************************************************************
**	先端にかかる力をセット
*******************************************************************/
KINE Arm::SetEndForce(const Matrix& force)
{
	Matrix tmp(POS_DOF);
	KINE kine;
	
	if ( (force.GetRow() != POS_DOF) || (force.GetCol() !=1) )
	{
		fprintf(stderr, "Leg::SetEndForce Invalid Matrix\n\n");
		
		return (KineErrSome);
	}

	tmp = EndForce;
	EndForce = force;

	kine = CheckEndForce();

	switch (kine)
	{
		case (NoKineError):
			break;

		case (KineErrReach):
			EndForce = force;
			break;

		case (KineErrSingular):
			EndForce = force;
			break;

		case (KineErrSome):
			EndForce = force;
			break;

		default:
			break;
	}

	return kine;
}

/*********************************************
**	以下はヤコビ計算とそれに関係する関数群  **
*********************************************/

/*******************************************************************
**	ヤコビ行列の計算
**	手首分まで(3自由度だけ)
*******************************************************************/
void Arm::CalcJacobian(void)
{
	// 順運動学の偏微分による計算
	JacobianA(1,1) = -sin( JntAng(1) )*( Link[1]*cos( JntAng(2) )+Link[2]*cos( JntAng(2)+JntAng(3) ) );
	JacobianA(1,2) = cos( JntAng(2) )*( -Link[1]*sin( JntAng(2) )-Link[2]*sin( JntAng(2)+JntAng(3) ) );
	JacobianA(1,3) = -Link[2]*cos( JntAng(1) )*sin( JntAng(2)+JntAng(3) );
	JacobianA(2,1) = cos( JntAng(1) )*( Link[1]*cos( JntAng(2) )+Link[2]*cos( JntAng(2)+JntAng(3) ) );
	JacobianA(2,2) = sin( JntAng(1) )*( -Link[1]*sin( JntAng(2) )-Link[2]*sin( JntAng(2)+JntAng(3) ) );
	JacobianA(2,3) = -Link[2]*sin( JntAng(1) )*sin( JntAng(2)+JntAng(3) );
	JacobianA(3,1) = 0;
	JacobianA(3,2) = Link[1]*cos( JntAng(2) )+Link[2]*cos( JntAng(2)+JntAng(3) );
	JacobianA(3,3) = Link[2]*cos( JntAng(2)+JntAng(3) );

	return;
}

/*******************************************************************
**	逆ヤコビ行列の計算
*******************************************************************/
void Arm::CalcInverseJacobian(void)
{
	CalcJacobian();
	InvJacobianA = JacobianA.Inversion();

	return;
}

/*****************************************************************************
**	関節トルクの計算
**		CalcJntTorque		: ヤコビ行列を使い, τ=JT*Fを使う
*****************************************************************************/
KINE Arm::CalcJntTorque(void)
{
	JntTorque = JacobianA*EndForce;

	return (NoKineError);
}
