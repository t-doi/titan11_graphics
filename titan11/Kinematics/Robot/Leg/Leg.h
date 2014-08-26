/*****************************************************************************
**
**	File:	  Leg.h
** 
**	����: TITAN XI��1�r����p
**
**	Created: 2003/09/19(Fri)	Updated: 2005/01/14(Fri)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

/***********************************************************************
**  ���̃N���X�͎��1�r�p�̃L�l�}�e�B�N�X��\��
**	�C���X�^���X�d���ōl����
***********************************************************************/

#ifndef	__Leg_h__
#define __Leg_h__

#include "Arm.h"

class Leg : public Arm
{
/*************************
**						**
**		�����o�ϐ�		**
**						**
*************************/
public:
private:
/***********************************************************************
**  ���W�n�͑S�ăO���[�o�� (���{�b�g�ɓ��ڂ�����@�̍��W)
***********************************************************************/
/******************************************************************************
	Foot				: (4,4) �����̓����ϊ��s��
	Jacobian			: (3,3)	���R�r�s��
	InvJacobian			: (3,3)	�t���R�r�s��
	JacobianL			: (3,3) �V�����_�Ɗ֐ߊp�̃��R�r
	InvJacobianL		: (3,3) �V�����_�Ɗ֐ߊp�̋t���R�r
	FootForce			: (3)	�r��ł̗͂̃x�N�g��
	CldrLen				: (3)	�e�V�����_����
	CldrSpd				: (3)	�e�V�����_���x
	CldrThrust			: (3)	�e�V�����_����

	CldrLmtMin			: �e�V�����_�̍Œᒷ��
	CldrLmtMax			: �e�V�����_�̍ō�����
	CldrThrustMin		: �V�����_���͂̍ŏ�
	CldrThrustMax		: �V�����_���͂̍ő�
	FootFrcLmtMin[3]	: �r��ɂ�����Œ�̗�
	FootFrcLmtMax[3]	: �r��ɂ�����ō��̗�

	LegID				: �r��ID�i���o (=1,2,3,4)
******************************************************************************/

	Matrix		Foot;
	Matrix		Jacobian;
	Matrix		InvJacobian;
	Matrix		JacobianL;
	Matrix		InvJacobianL;
	Matrix		FootForce;
	Matrix		CldrLen;
	Matrix		CldrSpd;
	Matrix		CldrThrust;

	double*		CldrLmtMin;
	double*		CldrLmtMax;
	double*		CldrThrustMin;
	double*		CldrThrustMax;
	double		FootFrcLmtMin[3];
	double		FootFrcLmtMax[3];
	int			LegID;

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
	Leg(int, double, double, double, double)	: �R���X�g���N�^
	Leg()										: �f�t�H���g�R���X�g���N�^
	~Leg()										: �f�X�g���N�^
***********************************************************************/
	Leg(int legID, double x, double y, double z, double theta);
	Leg();
	~Leg();				

/************************************************************************
**	SetLegID()					: �r��ID���Z�b�g	// ����: (1,2,3,4)
**	GetLegID()					: �r��ID�𓾂�	
************************************************************************/
	int			SetLegID(int id);
	int			GetLegID(void){return LegID;}

/*******************************************************************************************************
**	���W�ϊ��s��̌v�Z
**		JntID								: 0�͋r�x�[�X, 1,2,3�܂�
**		CoordinateTransformation			: �����ϊ��s����v�Z
**		InvCoordinateTransformation			: �t�����ϊ��s����v�Z
**		SetBaseParameter					: �r�̃x�[�X�܂ł̓����ϊ��s������肷��p�����[�^���Z�b�g
*******************************************************************************************************/
	Matrix		CoordinateTransformation(int jntID) const;
	Matrix		InvCoordinateTransformation(int jntID) const;

/*******************************************************************************************************
**	 ��/�t�L�l�}�e�B�N�X�̌v�Z
**		CalcDirectKinematics			: JntAng�x�N�g���ɂ��v�Z
**		CalcInverseKinematics			: ��������ɓ��̂ƕ��s�Ƃ��đ����̈ʒu����v�Z
*******************************************************************************************************/
	KINE		CalcDirectKinematics(void);
	KINE		CalcInverseKinematics(void);				// �{�̂Ƒ�������ɕ��s

/*******************************************************************************************
**	�֐ߊp���ʂɌv�Z (�t�L�l�}�e�B�N�X�Ƃقڈꏏ)
**		GetJntAngFromEndStat		: �l�݂̂��v�Z
*******************************************************************************************/
	double		GetJntAngFromEndPos(int jntID);

/******************************************************************************
**	��/�t�L�l�}�e�B�N�X (���R�r�s��) �̌v�Z������, �r�̉^���v����s��
**		SetLegEndPos			: �Ō��JntStat�}�g���N�X���Z�b�g
**		SetJntAng				: JntAng�x�N�g�����Z�b�g
**		SetLegConfiguration		: SetLegEndPos���, CalcDK������
**		UpdateJntStatus			: ���݂�JntAng�x�N�g����JntStat���X�V
**		SetFootForce			: FootForce�x�N�g�����Z�b�g
******************************************************************************/
	KINE		SetEndPos(const Matrix& endPos);						// �{�̂Ƒ�������ɕ��s
	KINE		SetConfiguration(const Matrix& target);					// �{�̂Ƒ�������ɕ��s
	KINE		SetJntAng(const Matrix& jntAng);
	KINE		SetFootForce(const Matrix& force);	
	void		UpdateJntState(void);

/*************************************************************************************
**	�r�悪�w�߂ǂ���̍s�����s���邩�̃`�F�b�N
**		CheckCylinderRange		: �V�����_���͈̓`�F�b�N
**		CheckFootForce			: �r��ɂ�����͂̃`�F�b�N
**		CheckCylinderThrust		: �V�����_���͂̃`�F�b�N
*************************************************************************************/
	KINE		CheckCylinderRange(void);//051006 doi
	KINE		CheckCylinderRange(int jntID);//051006 doi
	KINE		CheckFootForce(void);
	KINE		CheckCylinderThrust(void);

/*****************************************************************************
**	��/�t�̃��R�r�s����v�Z
**		CalcJacobian				: ���R�r�s��̌v�Z(3X3)
**		CalcInverseJacobian			: �t���R�r�s��̌v�Z
*****************************************************************************/
	void		CalcJacobian(void);
	void		CalcInverseJacobian(void);

/*****************************************************************************
**	�V�����_���͂̌v�Z
**		CalcCylinderThrust		: ���R�r�s����g��, ��=JT*F���g��
*****************************************************************************/
	KINE		CalcCylinderThrust(void);

/************************************************************************************
**	�V�����_��������֐ߊp�x�𓾂�
**		GetAngFromLen			: ��������l�݂̂��v�Z
**		CalcAngFromLen			: CldrLen�x�N�g������l���v�Z����JntAng�ɑ��
**								: �����Ȃ��͑S�Ă̊֐ߊp�x���v�Z
************************************************************************************/
	double		GetAngFromLen(int jntID, double len);
	KINE		CalcAngFromLen(int jntID);
	KINE		CalcAngFromLen(void);
	
/**************************************************************************************
**	�֐ߊp�x����V�����_�����𓾂�
**		GetLenFromAng			: ��������l�݂̂��v�Z
**		CalcLenFromAng			: JntAng�x�N�g������v�Z����CldrLen�x�N�g���ɑ��
**								: �����Ȃ��͑S�ẴV�����_�������v�Z
**************************************************************************************/
	double		GetLenFromAng(int jntID, double ang);
	KINE		CalcLenFromAng(int jntID);
	KINE		CalcLenFromAng(void);

/**************************************************************************************
**	���͈͂𓾂�
**		GetOuterMotionRange			: ���͈͂̊O���𓾂�
**		GetInnerMotionRange			: ���͈͂̓����𓾂�
**************************************************************************************/
	double		GetOuterMotionRange(double height);
	double		GetInnerMotionRange(double height);

/*******************************************************************
**	���ꂼ��̃����o�ϐ��𓾂�
*******************************************************************/
	Matrix		GetTransformMatrix(int jntID) const;
	Matrix		GetFootTransform(void) const		{return Foot;}
	Matrix		GetFootForce(void) const			{return FootForce;}
	Matrix		GetJacobian(void) const				{return Leg::Jacobian;}
	Matrix		GetInvJacobian(void) const			{return Leg::InvJacobian;}
	Matrix		GetCldrLen(void) const				{return CldrLen;}
	Matrix		GetCldrSpd(void) const				{return CldrSpd;}
	Matrix		GetCldrThrust(void)	const			{return CldrThrust;}

private:
/*******************************************************************
**	�R�s�[�R���X�g���N�^�Ƒ�����Z�q�̋֎~
*******************************************************************/
	Leg(const Leg& leg);
	Leg& operator =(const Leg& leg);

/*******************************************************************
**	�������̃w���p�[�֐�
*******************************************************************/
	void	InitLegParameter(void);

};
#endif // __Leg_h__
