/*****************************************************************************
**
**	File:	Arm.cpp
** 
**	����:	TITAN XI�̒P�r�̉^���w�̎��s��
**
**	Created: 2004/10/28(Thu)	Updated: 2005/01/14(Fri)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

#include "Arm.h"

/******************************
**  �f�t�H���g�R���X�g���N�^
******************************/
Arm::Arm()
{
	InitArmParameter();

	SetBaseParameter(0, 0, 0, 0);

}

/******************************
**  �R���X�g���N�^
******************************/
Arm::Arm(double x, double y, double z, double theta)
{
	InitArmParameter();

	SetBaseParameter(x, y, z, theta);
}


/******************************
**  �f�X�g���N�^
******************************/
Arm::~Arm()
{
	delete [] Transformation;
	delete [] AngLmtMin;
	delete [] AngLmtMax;
	delete [] Link;
}

/************************************************************************
**	�p�����[�^�̏�����
************************************************************************/
void Arm::InitArmParameter(void)
{
// �ȉ��Ń����o�ϐ��̏��������s��

// �z��̗v�f���m��
	Transformation	=	new Matrix[JOINT_NUM+1];
	AngLmtMin		=	new double[JOINT_NUM];
	AngLmtMax		=	new double[JOINT_NUM];
	Link			=	new double[JOINT_NUM];

// �}�g���N�X, �x�N�g���̗v�f���ݒ�
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
	
// �֐߂Ɋւ���ϐ��̏�����
// Matrix
	// �f�t�H���g�͍P���ϊ�
	for (i=0; i<(JOINT_NUM+1); i++)
		Transformation[i].LoadIdentity();

	JacobianA.LoadZero();
	InvJacobianA.LoadZero();
	JntAng.LoadZero();
	JntAngVel.LoadZero();
	JntTorque.LoadZero();
	EndForce.LoadZero();
	BaseParameter.LoadZero();

	// ���E�l�ݒ�
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

	// �p���w���̓f�t�H���g(�O�����C��I)
	SetOriIndicator();

	// �ŏ��̓G���[��
	LastJntErrNum = 0;
}

/************************************************************************
**	�eTransformation�}�g���N�X�𓾂�
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
**  �ȉ��͍��W�ϊ��Ɋւ���֐��Q  **
***********************************/

/************************************************************************
**  �����ϊ��s��̌v�Z
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
		case 0:		// �O���[�o������r�̃x�[�X�ւ̕ϊ�
			A(1,1) = cos( BaseParameter(4) );	A(1,2) = -sin( BaseParameter(4) );	A(1,3) = 0;						A(1,4) = BaseParameter(1);
			A(2,1) = sin( BaseParameter(4) );	A(2,2) = cos( BaseParameter(4) );	A(2,3) = 0;						A(2,4) = BaseParameter(2);
			A(3,1) = 0;							A(3,2) = 0;							A(3,3) = 1;						A(3,4) = BaseParameter(3);
			A(4,1) = 0;							A(4,2) = 0;							A(4,3) = 0;						A(4,4) = 1;
			break;

		case 1:		// 0�n����1�n�ւ̕ϊ�
			A(1,1) = cos(JntAng(jntID));		A(1,2) = 0;							A(1,3) = sin(JntAng(jntID));	A(1,4) = 0;
			A(2,1) = sin(JntAng(jntID));		A(2,2) = 0;							A(2,3) = -cos(JntAng(jntID));	A(2,4) = 0;
			A(3,1) = 0;							A(3,2) = 1;							A(3,3) = 0;						A(3,4) = Link[jntID-1];
			A(4,1) = 0;							A(4,2) = 0;							A(4,3) = 0;						A(4,4) = 1;
			break;

		case 2:		// 1�n����2�n�ւ̕ϊ�
			A(1,1) = cos(JntAng(jntID));		A(1,2) = -sin(JntAng(jntID));		A(1,3) = 0;						A(1,4) = Link[jntID-1]*cos(JntAng(jntID));
			A(2,1) = sin(JntAng(jntID));		A(2,2) = cos(JntAng(jntID));		A(2,3) = 0;						A(2,4) = Link[jntID-1]*sin(JntAng(jntID));
			A(3,1) = 0;							A(3,2) = 0;							A(3,3) = 1;						A(3,4) = 0;
			A(4,1) = 0;							A(4,2) = 0;							A(4,3) = 0;						A(4,4) = 1;
			break;

		case 3:		// 2�n����3�n�ւ̕ϊ�
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
**	�t�����ϊ��s��̌v�Z
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
		case 0:		// �r�̃x�[�X����O���[�o���ւ̕ϊ�
			A(1,1) = cos( BaseParameter(4) );	A(1,2) = sin( BaseParameter(4) );	A(1,3) = 0;			A(1,4) = -( cos( BaseParameter(4) )*BaseParameter(1) + sin( BaseParameter(4) )*BaseParameter(2) );
			A(2,1) = -sin( BaseParameter(4) );	A(2,2) = cos( BaseParameter(4) );	A(2,3) = 0;			A(2,4) = -( -sin( BaseParameter(4) )*BaseParameter(1) + cos( BaseParameter(4) )*BaseParameter(2) );
			A(3,1) = 0;							A(3,2) = 0;							A(3,3) = 1;			A(3,4) = -BaseParameter(3);
			A(4,1) = 0;							A(4,2) = 0;							A(4,3) = 0;			A(4,4) = 1;
			break;

		case 1:		// 1�n����0�n�ւ̕ϊ�
			A(1,1) = cos(JntAng(jntID));		A(1,2) = sin(JntAng(jntID));		A(1,3) = 0;			A(1,4) = 0;
			A(2,1) = 0;							A(2,2) = 0;							A(2,3) = 1;			A(2,4) = -Link[jntID-1];
			A(3,1) = sin(JntAng(jntID));		A(3,2) = -cos(JntAng(jntID));		A(3,3) = 0;			A(3,4) = 0;
			A(4,1) = 0;							A(4,2) = 0;							A(4,3) = 0;			A(4,4) = 1;
			break;

		case 2:		// 2�n����1�n�ւ̕ϊ�
			A(1,1) = cos(JntAng(jntID));		A(1,2) = sin(JntAng(jntID));		A(1,3) = 0;			A(1,4) = -Link[jntID-1];
			A(2,1) = -sin(JntAng(jntID));		A(2,2) = cos(JntAng(jntID));		A(2,3) = 0;			A(2,4) = 0;
			A(3,1) = 0;							A(3,2) = 0;							A(3,3) = 1;			A(3,4) = 0;
			A(4,1) = 0;							A(4,2) = 0;							A(4,3) = 0;			A(4,4) = 1;
			break;

		case 3:		// 3�n����2�n�ւ̕ϊ�
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
**	BaseParameter�}�g���N�X�ւ̒l�̃Z�b�g
**	x, y, z�̃I�t�Z�b�g�Ɖ�]�p�x�̃Z�b�g  
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
**	�ȉ��͉^���w�Ɋւ���v�Z  **
*******************************/
/*******************************************************************
**	�r�̎p���w�W���Z�b�g
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
**	���^���w�̌v�Z
*******************************************************************/
KINE Arm::CalcDirectKinematics(void)
{
	// ���ʔ���p�̃L�l�}�e�B�N�X
	KINE kine;

	// �֐߂������E���ɂ��邩
	kine = CheckJointRange();

	if (kine == NoKineError)
	{

		// �e�����ϊ��s��̌v�Z
		Transformation[0] = CoordinateTransformation(0);
		for (int i=1; i<(JOINT_NUM+1); i++)
			Transformation[i] = Transformation[i-1]*CoordinateTransformation(i);

		// �ݒ肵�����͈͓��ɋr�悪���邩
		kine = CheckReachRange();

	}
	else
	{
		fprintf(stderr, "Arm::CalcDirectKinematics() Error");
	}

	return kine;
}

/*******************************************************************
**	�t�^���w�̌v�Z
**	��͓I�ɉ�������, ���_�������
**	���ۂ̋O�����l���鎞�ɂ�, ��񃊃��N�������I�t�Z�b�g����ׂ�
*******************************************************************/
/***********************
**	�����͖{�̂ƕ��s  **
***********************/
KINE Arm::CalcInverseKinematics(void)
{
	KINE kine;

	double temp;					// ��1�֐ߊp�̃f�[�^�ێ��p
	double C3, S3;					// �֐ߊp�x3�ɂ��sin, cos
	double a, b, c, d;				// �֐ߊp�x2���v�Z���邽�߂̈ꎞ�I�ȕϐ�
	
	Matrix Trans(DH_NUM, DH_NUM);	// �r�x�[�X���W�n�ł̋r��̓����ϊ��s��
	Trans.LoadIdentity();			// Control Point Status
	
	// // �r�x�[�X���W�n�ɕϊ�
	Trans = InvCoordinateTransformation(0)*Transformation[POS_DOF];

	kine = CheckReachRange();

	if (kine == NoKineError)
	{
		// �ŏ��̊֐ߊp���v�Z
		if (fabs(Trans(1,4)) < 0.001 && fabs(Trans(2,4)) < 0.001)
		{
			return (KineErrSingular);		// ����͋N���Ȃ�
		}
		else
		{
			temp = JntAng(1);
			// ��1�֐ߊp���X�V
			JntAng(1) = atan2(Indicator[0]*Trans(2,4), Indicator[0]*Trans(1,4));
		}

		// 3�Ԗڂ̊֐ߊp���v�Z
		// �]��(cos)���v�Z
		C3 = (Trans(1,4)*Trans(1,4)+Trans(2,4)*Trans(2,4)+(Trans(3,4) - Link[0])*(Trans(3,4) - Link[0])
			-Link[1]*Link[1]-Link[2]*Link[2])/(2*Link[1]*Link[2]);

		if (fabs(C3) > 1)		// 3�Ԗڂ̊֐ߊp���v�Z�\���ǂ����`�F�b�N
		{
			// �t�L�l�}�e�B�N�X�������Ȃ��̂Œl��߂�
			JntAng(1) = temp;
			LastJntErrNum = 3;

			return (KineErrReach);
		}
		else if ( fabs(C3) == 1 )
		{
			return (KineErrSingular);
		}

		// ����(sin)���v�Z
		S3 = -Indicator[1]*sqrt(1-C3*C3);

		// ��3�֐ߊp���X�V
		JntAng(3) = atan2(S3, C3);

		// 2�Ԗڂ̊֐ߊp���v�Z
		a = Link[1]+Link[2]*C3;
		b = Link[2]*S3;
		c = Indicator[0]*sqrt(Trans(1,4)*Trans(1,4)+Trans(2,4)*Trans(2,4));
		d = Trans(3,4)-Link[0];

		// ��2�֐ߊp���X�V
		JntAng(2) = atan2( (a*d-b*c), (a*c+b*d) );

		kine = CheckJointRange();
		
	}

	return kine;
}

/*******************************************************************
**	�֐ߊp���ʂɌv�Z (�l�𓾂邾��)
**	�֐ߊp�̃`�F�b�N�p�֐�
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
	
	Matrix Trans(DH_NUM, DH_NUM);		// �r�x�[�X���W�n�ł̋r��̓����ϊ��s��
	Trans.LoadIdentity();

	kine = CheckReachRange();
	
	if (kine == KineErrSingular)
	{
		fprintf(stderr, "Leg::GetJntAngFromEndPos() Singular\n\n");
		return (0.0);
	}

	// �r�x�[�X���W�n�ւ̕ϊ�
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
**	�ȉ��͉^���w�Ɋւ���֐���p�����֐��Q(�p�ɂɎg��)  **
*********************************************************/

/*******************************************************************
**	�r��̓����ϊ��s����Z�b�g(�r��̓����ϊ��s����w��)
*******************************************************************/
KINE Arm::SetEndPos(const Matrix& endPos)
{
	Matrix tmp(DH_NUM, DH_NUM); tmp.LoadIdentity();
	KINE kine;

	// �����̃`�F�b�N
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
**	�e�֐ߊp�𓯎��ɃZ�b�g
*******************************************************************/
KINE Arm::SetJntAng(const Matrix& jntAng)
{
	Matrix tmp(JOINT_NUM);
	KINE kine;
	
	// �����`�F�b�N
	if ( (jntAng.GetRow() != JOINT_NUM) || (jntAng.GetCol() != 1) )
	{
		fprintf(stderr, "Leg::SetJntAng Invalid Matrix\n\n");
		
		return (KineErrSome);
	}

	// ���݂̊֐ߊp���ꎞ�I�ɕێ�
	tmp = JntAng;
	JntAng = jntAng;
	
	// ���^���w������
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
**	�r�̍\�����Z�b�g (�t�L�l���������㏇�L�l������)
*******************************************************************/
KINE Arm::SetConfiguration(const Matrix& target)
{
	KINE kine1, kine2;

	kine1 = SetEndPos(target);

	// �r�悪���B�ł�����
	if (kine1 == NoKineError)
	{
		// ���^���w������
		kine2 = CalcDirectKinematics();

		return (kine2);
	}

	return (kine1);

}

/*******************************************************************
**	���݂�JntAng�x�N�g����JntStat�}�g���N�X���X�V
*******************************************************************/
void Arm::UpdateJntState(void)
{
	for (int i=1; i<JOINT_NUM+1; i++)
		Transformation[i] = Transformation[i-1]*CoordinateTransformation(i);

	return;
}

/*******************************************************************
**	�ȉ��̓����o�ϐ����w�肵���͈͓��ɂ��邩�ǂ����̃`�F�b�N
*******************************************************************/

/*******************************************************************
**	�r�悪���B�͈͓��ɂ��邩�ǂ����̃`�F�b�N
**�@������^�����ꍇ�@���̍��W�n�ł̑���ʒu
**�@���������@Transformation[POS_DOF]�����̂܂܎g��
*******************************************************************/
KINE Arm::CheckReachRange(Matrix &Trans_in)//060720 doi
{
    //KINE kine;

    double x=Trans_in(1,4);//060720 doi
    double y=Trans_in(2,4);//060720 doi
    double z=Trans_in(3,4);//060720 doi
    double r=sqrt(x*x+y*y);//060719 doi

	// �r�̓��B���a�̍Œ�l�`�F�b�N
	if (r< ReachLmtMin[0])
	{
		return (KineErrUnderReachRadius);
	}
	// �r�̓��B���a�̍ō��l�`�F�b�N
	else if (r > ReachLmtMax[0])
	{
		return (KineErrOverReachRadius);
	}
	// �r�̍�������(z����)�`�F�b�N
	else if (z < ReachLmtMin[1])	// ���������͈̔�
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
	Matrix Trans(DH_NUM, DH_NUM);			// �r��̋r�̃x�[�X���W�n�ł̓����ϊ��s��
	
	// �����ŋr����r�̃x�[�X���W�n�ɕϊ�
	Trans = InvCoordinateTransformation(0)*Transformation[POS_DOF];
     kine=CheckReachRange(Trans);    
    return kine;
#endif

	return (NoKineError);
}

/*******************************************************************
**	�֐ߊp�x�̃`�F�b�N
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
KINE Arm::CheckJointRange(int jntID)//�֐ߌʂɃ`�F�b�N
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
**	�r��ɂ�����͂̃`�F�b�N
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
**	��[�ɂ�����͂��Z�b�g
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
**	�ȉ��̓��R�r�v�Z�Ƃ���Ɋ֌W����֐��Q  **
*********************************************/

/*******************************************************************
**	���R�r�s��̌v�Z
**	��񕪂܂�(3���R�x����)
*******************************************************************/
void Arm::CalcJacobian(void)
{
	// ���^���w�̕Δ����ɂ��v�Z
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
**	�t���R�r�s��̌v�Z
*******************************************************************/
void Arm::CalcInverseJacobian(void)
{
	CalcJacobian();
	InvJacobianA = JacobianA.Inversion();

	return;
}

/*****************************************************************************
**	�֐߃g���N�̌v�Z
**		CalcJntTorque		: ���R�r�s����g��, ��=JT*F���g��
*****************************************************************************/
KINE Arm::CalcJntTorque(void)
{
	JntTorque = JacobianA*EndForce;

	return (NoKineError);
}
