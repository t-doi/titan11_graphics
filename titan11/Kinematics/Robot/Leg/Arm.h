/*****************************************************************************
**
**	File:	Arm.h
** 
**	����:	TITAN XI�̒P�r�̉^���w
**
**	Created: 2004/10/28(Thu)	Updated: 2005/01/14(Fri)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

/***********************************************************************
**  ���̃N���X�͎�ɒP�r�p�̃L�l�}�e�B�N�X��\��
***********************************************************************/

#ifndef	__Arm_h__
#define __Arm_h__

#include "../../../Math/MathLibrary.h"
#include "../../../Utility/Type.h"
#include "../../KineParameter.h"

class Arm
{
/*************************
**						**
**		�����o�ϐ�		**
**						**
*************************/
public:
/*******************************************************************
**	Arm�N���X�Ɋ֌W����萔
*******************************************************************/
private:
protected:
/***********************************************************************
**  ���W�n�͑S�ăO���[�o�� (���{�b�g�ɓ��ڂ�����@�̍��W)
***********************************************************************/
/******************************************************************************
	Transformation		: (4,4) �O���[�o�����W����̊e�t���[���̓����ϊ��s��
	EndForce			: (3)	�r��ł̗͂̃x�N�g��
	JntAng				: (3)	�e�֐ߊp�x�̃x�N�g��
	JntAngVel			: (3)	�e�֐ߊp���x�̃x�N�g��
	JacobianA			: (3,3)	�֐ߊp�Ɛ�[�ψʂ̃��R�r�s��
	InvJacobianA		: (3,3)	�֐ߊp�Ɛ�[�ψʂ̋t���R�r�s��
	BaseParameter		: (4) �r�̃x�[�X�p�����[�^
						: (1)x, (2)y, (3)z, (4)angle

	AngLmtMin			: �e�֐߂̉����E(�Œ�)
	AngLmtMax			: �e�֐߂̉����E(�ō�)
	EndFrcLmtMin[3]		: �r��ɂ�����Œ�̗�
	EndFrcLmtMax[3]		: �r��ɂ�����ō��̗�
	ReachLmtMin[2]		: �r�悪���B����Œ዗��
	ReachLmtMax[2]		: �r�悪���B����ō�����

	Link				: �e�����N��
	Indicator[2]		: �r�̎p���w�W
						: Indicator[0] = HIP, Indicator[1] = KNEE
******************************************************************************/

	Matrix*		Transformation;
	Matrix		EndForce;
	Matrix		JntAng;
	Matrix		JntAngVel;
	Matrix		JntTorque;
	Matrix		JacobianA;
	Matrix		InvJacobianA;
	Matrix		BaseParameter;

	double*		AngLmtMin;
	double*		AngLmtMax;
	double		EndFrcLmtMin[3];
	double		EndFrcLmtMax[3];
	double		ReachLmtMin[2];
	double		ReachLmtMax[2];
	double*		Link;
	int			Indicator[2];

/*******************************************************************
**  �Ō�ɃG���[�̌����ƂȂ����֐ߔԍ�(1, 2, 3)
**	0�͑S��OK��, �G���[�Ȃ�
*******************************************************************/
	int			LastJntErrNum;

/*************************
**						**
**		�����o�֐�		**
**						**
*************************/
public:
/***********************************************************************
** �R���X�g���N�^�ƃf�X�g���N�^
***********************************************************************/
/***********************************************************************
	Arm()										: �f�t�H���g�R���X�g���N�^
	Arm(double, double, double, double)			: �R���X�g���N�^
	~Arm()										: �f�X�g���N�^
***********************************************************************/
				Arm();
				Arm(double x, double y, double z, double theta);
	virtual		~Arm();				

/*******************************************************************************************************
**	���W�ϊ��s��̌v�Z
**		JntID								: 0�͋r�x�[�X, 1,2,3�܂�
**		CoordinateTransformation			: �����ϊ��s����v�Z
**		InvCoordinateTransformation			: �t�����ϊ��s����v�Z
**		SetBaseParameter					: �r�̃x�[�X�܂ł̓����ϊ��s������肷��p�����[�^���Z�b�g
*******************************************************************************************************/
	virtual Matrix	CoordinateTransformation(int jntID) const;
	virtual Matrix	InvCoordinateTransformation(int jntID) const;
	void			SetBaseParameter(double x, double y, double z, double theta);

/************************************************************************
**	SetOriIndicator()			: �p���w�W���Z�b�g	// ����: (1 or -1)
************************************************************************/
	bool			SetOriIndicator(int front=1, int up=1);

/*******************************************************************************************************
**	 ��/�t�L�l�}�e�B�N�X�̌v�Z
**		CalcDirectKinematics			: JntAng�x�N�g���ɂ��v�Z
**		CalcInverseKinematics			: �Ō��JntStat�}�g���N�X�ɂ��v�Z(�t�L�l����͓I�ɂƂ���)
*******************************************************************************************************/
	virtual KINE	CalcDirectKinematics(void);
	virtual KINE	CalcInverseKinematics(void);

/*******************************************************************************************
**	�֐ߊp���ʂɌv�Z (�t�L�l�}�e�B�N�X�Ƃقڈꏏ)
**		GetJntAngFromEndPos		: �l�݂̂��v�Z
*******************************************************************************************/
	virtual double	GetJntAngFromEndPos(int jntID);

/******************************************************************************
**	��/�t�L�l�}�e�B�N�X (���R�r�s��) �̌v�Z������, �r�̉^���v����s��
**		SetEndPos				: �r��̈ʒu���Z�b�g
**		SetConfiguration		: SetEndPos���, CalcDK������
**		SetJntAng				: JntAng�x�N�g�����Z�b�g
**		SetEndForce				: EndForce�x�N�g�����Z�b�g
**		UpdateTransformation	: ���݂�JntAng�x�N�g���œ����ϊ��s����X�V
******************************************************************************/
	virtual KINE	SetEndPos(const Matrix& endPos);
	virtual KINE	SetConfiguration(const Matrix& target);
	virtual KINE	SetJntAng(const Matrix& jntAng);
	virtual KINE	SetEndForce(const Matrix& force);
	virtual void	UpdateJntState(void);

/*************************************************************************************
**	�r�悪�w�߂ǂ���̍s�����s���邩�̃`�F�b�N
**		CheckReachStatus		: �t�L�l�}�e�B�N�X�ɂ�蓞�B�͈̓`�F�b�N
**		CheckJointRange			: �֐߉��͈̓`�F�b�N
**		CheckEndForce			: �r��ɂ�����͂̃`�F�b�N
*************************************************************************************/
	KINE			CheckReachRange(void);
	KINE			CheckReachRange(Matrix &Trans_in);//060720 doi
	KINE			CheckJointRange(void);
	KINE			CheckJointRange(int jntID);  
	KINE			CheckEndForce(void);

/*****************************************************************************
**	��/�t�̃��R�r�s����v�Z
**		CalcJacobian				: ���R�r�s��̌v�Z(3X3)
**		CalcInverseJacobian			: �t���R�r�s��̌v�Z
*****************************************************************************/
	virtual void	CalcJacobian(void);
	virtual void	CalcInverseJacobian(void);

/*****************************************************************************
**	�֐߃g���N�̌v�Z
**		CalcJntTorque		: ���R�r�s����g��, ��=JT*F���g��
*****************************************************************************/
	KINE			CalcJntTorque(void);

/*******************************************************************
**	���ꂼ��̃����o�ϐ��𓾂�
*******************************************************************/
	virtual Matrix	GetTransformMatrix(int jntID) const;
	Matrix			GetBaseParameter(void) const	{return BaseParameter;}
	Matrix			GetEndForce(void) const			{return EndForce;}
	Matrix			GetJacobianA(void) const		{return JacobianA;}
	Matrix			GetInvJacobianA(void) const		{return InvJacobianA;}
	Matrix			GetJntAng(void)	const			{return JntAng;}
	Matrix			GetJntAngVel(void) const		{return JntAngVel;}
	int				GetLastJntErr(void) const		{return LastJntErrNum;}

private:
/*******************************************************************
**	�R�s�[�R���X�g���N�^�Ƒ�����Z�q�̋֎~
*******************************************************************/
		Arm(const Arm& arm);
		Arm& operator =(const Arm& arm);

/*******************************************************************
**	�������̃w���p�[�֐�
*******************************************************************/
	void		InitArmParameter(void);

};
#endif // __Arm_h__
