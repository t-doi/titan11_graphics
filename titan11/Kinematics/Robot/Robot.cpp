/********************************************************************************
**  
**	File:	 Robot.cpp
** 
**	Description: TITAN XI�̖{�̂̉^���w
**
**	Created: 2003/10/22(Wed)	Updated: 2005/03/10(Thu)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
********************************************************************************/

#include "Robot.h"

/*******************************************************************
**  �f�t�H���g�R���X�g���N�^
*******************************************************************/
Robot::Robot()
{
	//////////////////////////
	////// ���W�n�̒�` //////
	//////////////////////////
	//		 �E��n			//
	//						//
	//				  y		//
	//	x   1	     2��	//
	//	����|--------|����	//
	//	  ��|   x	 |	 x	//
	//	  y | �@��   |		//
	//	    | ����   |		//
	//	    |y	     |y		//
	//	x   |	     |��	//
	//	����|--------|����	//
	//	  ��3		 4   x	//
	//	   y				//
	//////////////////////////

	// �������m��
	CreateLegInstance();

	InitRobotParameter();

}

/*******************************************************************
**  �R���X�g���N�^
*******************************************************************/
Robot::Robot(const Matrix& body)
{
	//////////////////////////
	////// ���W�n�̒�` //////
	//////////////////////////
	//		 �E��n			//
	//						//
	//				  y		//
	//	x   1	     2��	//
	//	����|--------|����	//
	//	  ��|   x	 |	 x	//
	//	  y | �@��   |		//
	//	    | ����   |		//
	//	    |y	     |y		//
	//	x   |	     |��	//
	//	����|--------|����	//
	//	  ��3		 4   x	//
	//	   y				//
	//////////////////////////

	// �����̔z��`�F�b�N
	if ( (body.GetRow() != DH_NUM) || (body.GetCol() != DH_NUM) )
	{
		fprintf(stderr, "Robot::Robot Invalid Matrix");
	}

	// �������̊m��
	CreateLegInstance();

	InitRobotParameter();

	// ���̂̓����ϊ��s����X�V
	Body = body;
}

/*******************************************************************
**  �f�X�g���N�^
*******************************************************************/
Robot::~Robot()
{
	DestoryLegInstance();
}

/*******************************************************************
**  �������m��
*******************************************************************/
void Robot::CreateLegInstance(void)
{
	// �r���̔z����m��
	LegPH			= new LegPhase[LEG_NUM];

	// �N���X�|�C���^�̔z��m��
	Legs			= new Leg*[LEG_NUM];

	// �N���X�̃C���X�^���X�𐶐�
	for (int i=0; i<LEG_NUM; i++)
	{
		Legs[i]		= new Leg( (i+1), LEG_ROOT_POSX[i], LEG_ROOT_POSY[i], LEG_ROOT_POSZ[i], LEG_ROOT_ANGLE[i] );
	}
//printf("Robot::CreateLegInstance()\n");//071010 doi debug
}

/*******************************************************************
**  ���������
*******************************************************************/
void Robot::DestoryLegInstance(void)
{
	delete [] LegPH;

	// �C���X�^���X��j��
	for (int i=0; i<LEG_NUM; i++)
		delete Legs[i];

	delete [] Legs;
}

/*******************************************************************
**  �p�����[�^������
*******************************************************************/
void Robot::InitRobotParameter(void)
{
	// �ŏ��͎��[�`��
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
**  �{�̂̓����ϊ��s����Z�b�g(�������{�̂ƕ��s)
*******************************************************************/
KINE Robot::SetBodyTransform(const Matrix& body)
{
	// �v�f���̃`�F�b�N
	if ( body.GetRow() != DH_NUM || body.GetCol() != DH_NUM )
	{
		fprintf(stderr, "Robot::SetBodyTransform() Invalid prameter\n\n");
		return (KineErrSome);
	}

	KINE kine;

	// �r��̐V�����ڕW�l(�O���[�o�����W)
	Matrix newLegEndPos[LEG_NUM];
	// ���݂̖{�̂̓����ϊ��s��̈ꎞ�I�ȕێ��l
	Matrix oldBody(DH_NUM, DH_NUM);

	// ���{�b�g�̓����ϊ��s����X�V����O�̃f�[�^��ۑ�
	oldBody = Body;
	for (int i=0; i<LEG_NUM; i++)
	{
		newLegEndPos[i].SetSize(DH_NUM, DH_NUM);
		newLegEndPos[i] = TransformationLocaltoGlobal( Legs[i]->GetFootTransform() );
	}

	// ���{�b�g�̓����ϊ��s����X�V
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
					// �V���Ȏw���l�ŋr�����B�ł��邩�̃`�F�b�N
//					kine = SetLegConfiguration( (j+1), newLegEndPos[j] );
					kine = SetFootConfiguration( (j+1), newLegEndPos[j] );
		
					// �����r�悪�V�����w�ߒl�ɓ��B�ł��Ȃ�������
					if (kine != NoKineError)
					{
						// �{�̂̏�Ԃ��X�V����O�ɖ߂�
						Body = oldBody;
						//����ʒu���X�V�O�ɖ߂��D060801 doi
						for(int  l=0;l<LEG_NUM;l++)
						{
						//SetLegConfiguration( (l+1), newLegEndPos[l] );
						SetFootConfiguration( (l+1), newLegEndPos[l] );
						}

						// �r�̃G���[ID����������
						LastLegErrID = j+1;
						return (kine);
					}

					// �r�̎��s�Ȃ�
					LastLegErrID = 0;
				
				}
			}
			break;

		case ALL_LEG:
            for (int k=0; k<LEG_NUM; k++)
			{
				// �V���Ȏw���l�ŋr�����B�ł��邩�̃`�F�b�N
				//kine = SetLegConfiguration( (k+1), newLegEndPos[k] );
				kine = SetFootConfiguration( (k+1), newLegEndPos[k] );
		
				// �����r�悪�V�����w�ߒl�ɓ��B�ł��Ȃ�������
				if (kine != NoKineError)
				{
					// �{�̂̏�Ԃ��X�V����O�ɖ߂�
					Body = oldBody;

					//����ʒu���X�V�O�ɖ߂��D060801 doi
					for(int  l=0;l<LEG_NUM;l++)
					{
					//SetLegConfiguration( (l+1), newLegEndPos[l] );
					SetFootConfiguration( (l+1), newLegEndPos[l] );
					}

					// �r�̃G���[ID����������
					LastLegErrID = k+1;
					return (kine);
				}

				// �r�̎��s�Ȃ�
				LastLegErrID = 0;

			}
			break;

		case CRAWLER:
			break;
	}

	return (NoKineError);
}

/*******************************************************************
**  �{�̂̓����ϊ��s����Z�b�g(DK�x�[�X)
*******************************************************************/
KINE Robot::SetBodyTransformWithDK(const Matrix& body)//060813 doi
{
    //DK�x�[�X�œ��̈ʒu�ݒ�
	// �v�f���̃`�F�b�N
	if ( body.GetRow() != DH_NUM || body.GetCol() != DH_NUM )
	{
		fprintf(stderr, "Robot::SetBodyTransform() Invalid prameter\n\n");
		return (KineErrSome);
	}

	KINE kine;
        int i;

	// �r��̐V�����ڕW�l(�O���[�o�����W)
	Matrix newLegEndPos[LEG_NUM];
	// ���݂̖{�̂̓����ϊ��s��̈ꎞ�I�ȕێ��l
	Matrix oldBody(DH_NUM, DH_NUM);

	// ���{�b�g�̓����ϊ��s����X�V����O�̃f�[�^��ۑ�
	oldBody = Body;

	// ���{�b�g�̓����ϊ��s����X�V
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
**  �{�̂𓮂������@���Z�b�g
*******************************************************************/
void Robot::SetMotionStyle(MotionStyle style)
{
	BodyMotionStyle = style;

	return;
}

/*******************************************************************
**  ���̂̏�ԑ����Z�b�g
*******************************************************************/
void Robot::SetBodyPhase(BodyPhase phase)
{
	BodyPH = phase;

	return;
}

/*******************************************************************
**  �r�̍\�����f�t�H���g�̍\���ɃZ�b�g
*******************************************************************/
int Robot::SetLegPattern (int legID, LegPattern pattern)
{
	Matrix angle[LEG_NUM];

	for (int i=0; i<LEG_NUM; i++)
		angle[i].SetSize(JOINT_NUM);

	switch (pattern)
	{
		case RETRACT:		// �i�[�`��

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

		case RECTANGLE:		// �����`

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

		case PRESET:		// �ݒ肵���p�x�Ɏw��(�����ł�45��)

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
**  �r�̃C���X�^���X���擾
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
**  �r�̏�ԑ����Z�b�g
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
**  ���̂̏�ԑ����Z�b�g
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
**  ������ȉ��͋r�̃����o�֐����Ăяo�� 
*************************************************************************************************************/

/*******************************************************************
**  �r��̓����ϊ��s����Z�b�g
*******************************************************************/
/*************************************
**  ����(endpos)�̓O���[�o�����W�n  **
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

	// �O���[�o������{�̃��[�J���֎w���l��ϊ�
	position = TransformationGlobaltoLocal(endPos);
	kine = Legs[legID-1]->SetEndPos(position);
	
	return (kine);
}

/*******************************************************************
**  SetEndPos���, CalcDK������
*******************************************************************/
/*************************************
**  ����(target)�̓O���[�o�����W�n  **
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

	// �O���[�o������{�̃��[�J���֎w���l��ϊ�
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
**  �r�̊֐ߊp���Z�b�g
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
**  �r�̊eJntStat���X�V
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
**  �e��̌v�Z���s��Leg�N���X�̃����o�֐��Ăяo��
***********************************************************************************************/
/*******************************************************************
**  �r�̏��^���w�v�Z
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
**  �r�̋t�^���w�v�Z(�����͖{�̂ƕ��s)
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
**  �r�̃��R�r�s��̌v�Z
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
**  �r�̋t���R�r�s��̌v�Z
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
**  �r��ɂ�����͂��Z�b�g
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
**  �r�̃V�����_����, CldrThrust�x�N�g���̌v�Z
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
**  �r�̃V�����_���x�N�g������p�x�x�N�g��JntAng�ɒl���Z�b�g
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
**  �r�̊p�x�x�N�g��JntAng����V�����_�x�N�g��CldrLen�ɒl���Z�b�g
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
**  �r�̃����o�ϐ����擾
**	���W�n�͓��̍��W�n�œ�����
****************************************************************************************/
/*******************************************************************
**  �r�̊e�֐ߏ�Ԃ𓾂�
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
**  �r��̏�Ԃ𓾂�
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
**  �r��ɂ�����͂𓾂�
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
**  �e�r�̃��R�r�s��𓾂�
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
**  �e�r�̋t���R�r�s��𓾂�
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
**  �e�r�̃x�[�X����p�����[�^BaseParam�𓾂�
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
**  �e�r�̊֐ߊp�x�x�N�g���𓾂�
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
**  �e�r�̊֐ߊp���x�x�N�g���𓾂�
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
**  �e�r�̃V�����_���x�N�g���𓾂�
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
**  �e�r�̃V�����_���x�x�N�g���𓾂�
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
**  �e�r�̃V�����_���̓x�N�g���𓾂�
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
//***********************************************************************************************�����܂œ��̍��W�n�œ�����

/*******************************************************************
**  �r�̃����o�ϐ����擾
**	���W�n�̓O���[�o�����W�n�œ�����
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
**  ���̑�
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
**  �O���[�o������{�̃��[�J���ւ̍��W�ϊ�
*******************************************************************/
Matrix Robot::TransformationGlobaltoLocal(const Matrix& matrix) const
{
	Matrix result(DH_NUM, DH_NUM);
	Matrix transform(DH_NUM, DH_NUM);

	// �v�f���̃`�F�b�N
	if ( (matrix.GetRow() != DH_NUM) || (matrix.GetCol() != DH_NUM) )
	{
		fprintf(stderr, "Robot::TransformationGlobaltoLocal() Invalid matrix\n\n");
		return (IDENTITY_MATRIX_DH);
	}

	//�ȉ��t�s��v�Z�D�v�Z�������̂��߂��H 060808 doi
	//transform=Body.Inversion();//060808 doi
	//��]�����̋t�ϊ���]�u�s��Ōv�Z
	transform(1,1) = Body(1,1);		transform(1,2) = Body(2,1);		transform(1,3) = Body(3,1);
	transform(2,1) = Body(1,2);		transform(2,2) = Body(2,2);		transform(2,3) = Body(3,2);
	transform(3,1) = Body(1,3);		transform(3,2) = Body(2,3);		transform(3,3) = Body(3,3);
	transform(4,1) = 0;				transform(4,2) = 0;				transform(4,3) = 0;
	//���i�����̋t�ϊ���]�u�s��Ōv�Z
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
**  �{�̃��[�J������O���[�o���ւ̍��W�ϊ�
*******************************************************************/
Matrix Robot::TransformationLocaltoGlobal(const Matrix& matrix) const
{
	Matrix result(DH_NUM, DH_NUM);

	// �v�f���̃`�F�b�N
	if ( (matrix.GetRow() != DH_NUM) || (matrix.GetCol() != DH_NUM) )
	{
		fprintf(stderr, "Robot::TransformationLocaltoGlobal() Invalid matrix\n\n");
		return (IDENTITY_MATRIX_DH);
	}

	result = Body*matrix;

	return (result);
}
