/*****************************************************************************
**  
**	File:	  Leg.cpp
** 
**	����: TITAN XI��1�r����p�v���O����
**
**	Created: 2003/09/19(Fri)	Updated: 2005/01/14(Fri)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

#include "Leg.h"

/******************************
**  �f�t�H���g�R���X�g���N�^
******************************/
Leg::Leg()
{
	InitLegParameter();

	SetLegID(1);
	SetBaseParameter(0, 0, 0, 0);

}

/******************************
**  �R���X�g���N�^
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
**  �f�X�g���N�^
******************************/
Leg::~Leg()
{
	delete [] CldrLmtMin;
	delete [] CldrLmtMax;
	delete [] CldrThrustMin;
	delete [] CldrThrustMax;
}

/******************************
**  �r�̏������p�֐�
******************************/
void Leg::InitLegParameter(void)
{
// �ȉ��Ń����o�ϐ��̏��������s��

	// �z��̗v�f���m��
	CldrLmtMin		=	new double[JOINT_NUM];
	CldrLmtMax		=	new double[JOINT_NUM];
	CldrThrustMin	=	new double[JOINT_NUM];
	CldrThrustMax	=	new double[JOINT_NUM];

	// �}�g���N�X, �x�N�g���̗v�f���ݒ�
	Foot.SetSize(DH_NUM, DH_NUM);
	FootForce.SetSize(POS_DOF);

	Jacobian.SetSize(POS_DOF, POS_DOF);
	InvJacobian.SetSize(POS_DOF, POS_DOF);
	JacobianL.SetSize(POS_DOF, POS_DOF);
	InvJacobianL.SetSize(POS_DOF, POS_DOF);
		
	CldrLen.SetSize(JOINT_NUM);
	CldrSpd.SetSize(JOINT_NUM);
	CldrThrust.SetSize(JOINT_NUM);
	
	// �V�����_�Ɋւ���ϐ��̏�����
	CldrLen.LoadZero();
	CldrSpd.LoadZero();
	CldrThrust.LoadZero();

	// ���E�l�ݒ�
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

	// �r��̕ϐ��Ɋւ��鏉����
	Foot.LoadIdentity();		// �f�t�H���g�͍P���ϊ�
	Jacobian.LoadZero();
	InvJacobian.LoadZero();
	JacobianL.LoadZero();
	InvJacobianL.LoadZero();

	FootForce.LoadZero();

	LegID			= 0;
	return;
}

/*******************************************************************
**	�r��ID���Z�b�g
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
**	�e�����ϊ��s��𓾂�
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
**  �ȉ��͍��W�ϊ��Ɋւ���֐��Q  **
***********************************/

/************************************************************************
**  �����ϊ��s��̌v�Z
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

		case 4:		// 3�n����4�n�ւ̕ϊ�(�{�̂Ƒ��������s)
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
**	�t�����ϊ��s��̌v�Z
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

		case 4:		// 4�n����3�n�ւ̕ϊ�
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
**	�ȉ��͉^���w�Ɋւ���v�Z  **
*******************************/

/*******************************************************************
**	���^���w�̌v�Z
*******************************************************************/
KINE Leg::CalcDirectKinematics(void)
{
	// ���ʔ���p�̃L�l�}�e�B�N�X
	KINE kine = NoKineError;

	// �֐߂������E���ɂ��邩
	kine = CheckJointRange();

	if (kine == NoKineError)
	{
		// �e�����ϊ��s��̌v�Z
		Transformation[0] = CoordinateTransformation(0);
		for (int i=1; i<(JOINT_NUM+1); i++)
			Transformation[i] = Transformation[i-1]*CoordinateTransformation(i);

		// �����̓����ϊ��s��
		Foot = Transformation[POS_DOF]*( Leg::CoordinateTransformation(4) );

		// �ݒ肵�����͈͓��ɋr�悪���邩
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
**	�t�^���w�̌v�Z
**	��͓I�ɉ�������, ����_�������
**	���ۂ̋O�����l���鎞�ɂ�, ���񃊃��N�������I�t�Z�b�g����ׂ�
*******************************************************************/
/***********************
**	�����͖{�̂ƕ��s  **
***********************/
KINE Leg::CalcInverseKinematics(void)
{
	KINE kine;

	double temp;					// ��1�֐ߊp�̃f�[�^�ێ��p
	double C3, S3;					// �֐ߊp�x3�ɂ��sin, cos
	double a, b, c, d;				// �֐ߊp�x2���v�Z���邽�߂̈ꎞ�I�ȕϐ�
	
	Matrix Trans(DH_NUM, DH_NUM);	// �r�x�[�X���W�n�ł̋r��̓����ϊ��s��
	Trans.LoadIdentity();			// Control Point Status
	
	// // �r�x�[�X���W�n�ɕϊ�
	Trans = InvCoordinateTransformation(0)*Foot;

        Matrix Ankle(DH_NUM, DH_NUM);	// �r�x�[�X���W�n�ł̑���̓����ϊ��s�� 060723 doi
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
	kine = CheckReachRange(Ankle);    //060720 doi    ����_�Ń`�F�b�N����

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
		// ���񍂂��������I�t�Z�b�g���ċr��̓����ϊ��s��Transformation[3]�ɂ���
		C3 = (Trans(1,4)*Trans(1,4)+Trans(2,4)*Trans(2,4)+(Trans(3,4) - Link[0]+FOOT)*(Trans(3,4) - Link[0]+FOOT)
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
		d = Trans(3,4)-Link[0]+FOOT;

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

	Matrix Trans(DH_NUM, DH_NUM);		// �r�x�[�X���W�n�ł̋r��̓����ϊ��s��
	Trans.LoadIdentity();

	kine = CheckReachRange();
	
	if (kine == KineErrSingular)
	{
		exit(1);
	}

	// �r�x�[�X���W�n�ւ̕ϊ�
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
                        return (0.0);//�r�ԍ��ُ�
			//break;
                        }
	}
  
  return theta;
}

/*********************************************************
**	�ȉ��͉^���w�Ɋւ���֐���p�����֐��Q(�p�ɂɎg��)  **
*********************************************************/

/*******************************************************************
**	�r�̍\�����Z�b�g (�t�L�l���������㏇�L�l������)
*******************************************************************/
KINE Leg::SetConfiguration(const Matrix& target)
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
**	�r��̓����ϊ��s����Z�b�g
*******************************************************************/
/*************************************************
**	�����͋r�悩��^���̈ʒu(�����͖{�̂ƕ��s)�@**
*************************************************/
KINE Leg::SetEndPos(const Matrix& endPos)
{
	Matrix tmp(DH_NUM, DH_NUM);
	tmp.LoadIdentity();

	KINE kine;

	// �����̍s��̗v�f�����`�F�b�N
	if (endPos.GetRow() !=DH_NUM || endPos.GetCol() != DH_NUM)
	{
		fprintf(stderr, "Leg::SetLegEndPos Invalid Matrix\n\n");
		return (KineErrSome);
	}

	tmp = Foot;
	Foot = endPos;

	kine = CalcInverseKinematics();

if(kine==NoKineError)return kine;//060719 doi.�G���[�������ꍇ

    Foot=tmp;//060717 doi.�G���[������ꍇ�D
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
**	�e�֐ߊp�𓯎��ɃZ�b�g
*******************************************************************/
KINE Leg::SetJntAng(const Matrix& jntAng)
{
	Matrix tmp(JOINT_NUM);
	KINE kine;
	
	// �v�f���̃`�F�b�N
	if (jntAng.GetRow() != JOINT_NUM || jntAng.GetCol() != 1)
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
        		JntAng = tmp;//NoKineError�łȂ��̂Ŗ߂��D
			break;
    }
    
    return kine;

}

/*******************************************************************
**	���݂�JntAng�x�N�g����JntStat�}�g���N�X���X�V
*******************************************************************/
void Leg::UpdateJntState(void)
{
	for (int i=1; i<(JOINT_NUM+1); i++)
		Transformation[i] = Transformation[i-1]*CoordinateTransformation(i);

	return;
}

/*******************************************************************
**	�r��ɂ�����͂��Z�b�g
*******************************************************************/
KINE Leg::SetFootForce(const Matrix& force)
{
	Matrix tmp(POS_DOF);
	KINE kine;
	
	// �v�f���̃`�F�b�N
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
**	�ȉ��̓����o�ϐ����w�肵���͈͓��ɂ��邩�ǂ����̃`�F�b�N
*******************************************************************/

/*******************************************************************
**	�V�����_�����̃`�F�b�N
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
**	�r��ɂ�����͂̃`�F�b�N
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
**	�V�����_���͂̃`�F�b�N
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
**	�ȉ��̓��R�r�v�Z�Ƃ���Ɋ֌W����֐��Q  **
*********************************************/

/*******************************************************************
**	���R�r�s��̌v�Z
**	���񕪂܂�(3���R�x����)
*******************************************************************/
void Leg::CalcJacobian(void)
{
	// �V�����_�����ɂ��֐ߊp�̕ω���(�Δ����l)  
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
            //LegID�ُ�
            return;
        }

	A[1] = DiffArcCosine(CosLawtoAng(DTC9, DTC12, CldrLen(2)))*(-CldrLen(2)/(DTC9*DTC12));
	
	A[2] = DiffArcCosine(CosLawtoAng(DTC15, DTC18, CldrLen(3)))*(-CldrLen(3)/(DTC15*DTC18));

	// �V�����_���֐ߊp�̃��R�r���v�Z
	JacobianL(1,1) = A[0];
	JacobianL(2,2) = A[1];
	JacobianL(3,3) = A[2];

	// �֐ߊp����[�ʒu�̃��R�r���v�Z
	Arm::CalcJacobian();

	// ���S�ȃ��R�r���v�Z
	Jacobian = JacobianA*JacobianL;

	return;
}

/*******************************************************************
**	�t���R�r�s��̌v�Z
*******************************************************************/
void Leg::CalcInverseJacobian(void)
{
	CalcJacobian();
	InvJacobian = Jacobian.Inversion();

	return;
}

/***********************************************************************************************
**	�r��ɂ�����͂���V�����_���͂��v�Z
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
**	�ȉ��̓V�����_���Ɗ֐ߊp�Ɋ֌W����֐��Q  **
***********************************************/

/*******************************************************************
**	�V�����_��(����)����e�֐ߊp�̒l�����𓾂�
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
**	�V�����_��(CldrLen�x�N�g��)����e�֐ߊp���v�Z�����
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
**	CalcAngFromLen��S�čs��
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
**	�֐ߊp(����)����V�����_���̒l�����𓾂�
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
                        //jntID�ُ�
                        return (0.0);
			//break;
                        }
	}

	return length;
}

/*******************************************************************
**	�֐ߊp(JntAng�x�N�g��)����V�����_���̒l���v�Z�����
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
**	CalcLenFromAng��S�čs��
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
**	���͈͂̓����𓾂�
*******************************************************************/
double Leg::GetInnerMotionRange(double height)
{
	double range=0;

	// ����̍����ɕϊ����Ă���
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
**	���͈͂̊O���𓾂�
*******************************************************************/
double Leg::GetOuterMotionRange(double height)
{
	double range=0;

	// ����̍����ɕϊ����Ă���
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
