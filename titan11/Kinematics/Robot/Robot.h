/********************************************************************************
**  
**	File:			Robot.h
** 
**	Description:	TITAN XI�̖{�̂̉^���w
**
**	Created: 2003/10/22(Wed)	Updated: 2005/01/14(Fri)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
********************************************************************************/

/***********************************************************************
**  ���̃N���X�͎�Ƀ��{�b�g�S�̂̃L�l�}�e�B�N�X��\��
***********************************************************************/

#ifndef	__Robot_h__
#define __Robot_h__

#include "Leg/Leg.h"
#include "../KineParameter.h"

class Robot
{
/*************************
**						**
**		�����o�ϐ�		**
**						**
*************************/
public:
/*******************************************************************
**	�{�̂𓮂������@
**		SUPPORT_LEG		: �x���r�̂�
**		ALL_LEG			: �S�Ă̋r
**		CRAWLER			: �N���[��
*******************************************************************/
	enum MotionStyle{SUPPORT_LEG, ALL_LEG, CRAWLER};		
	// �f�t�H���g��ALL_LEG

/*******************************************************************
**	���̂̏�ԕ\��
**		MOTION			: �ړ�
**		STATIONARY		: �Î~
*******************************************************************/
	enum BodyPhase{MOTION, STATIONARY};

/*******************************************************************
**	�r�̏�ԕ\��
**		SUPPORT			: �x���r
**		SWING			: �V�r
*******************************************************************/
	enum LegPhase{SUPPORT, SWING};

/*******************************************************************
**  �f�t�H���g�̋r�\��
**		RETRACT			: �i�[�`��
**		RECTANGLE		: �����`�`��
**		PRESET			: �O�����Đݒ肵���p�x�Ɏw��
*******************************************************************/
	enum LegPattern{RETRACT, RECTANGLE, PRESET};

private:

protected:
/***************************************************
**	���{�b�g�{�̂̓����ϊ��s��̓O���[�o�����W�n  **
***************************************************/
/*******************************************************************
**  �{�̂̂��߂̃����o�ϐ�
**		Body						: �{�̐}�S�̓����ϊ��s��
**		BodyMotionStyle				: �{�̂̐��i���@
**		BodyPhase					: �{�̂̏�ԑ�
*******************************************************************/
	Matrix			Body;
	MotionStyle		BodyMotionStyle;
	BodyPhase		BodyPH;

/*******************************************************************
**  �r�̂��߂̃����o�ϐ�
**		MultiLeg				: �r�̃L�l�}�e�B�N�X�I�u�W�F�N�g
**		LegPH					: �r�̏�ԑ�
*******************************************************************/
	Leg**			Legs;
	LegPhase*		LegPH;

/*******************************************************************
**  �Ō�ɃG���[�̌����ƂȂ����r��ID(1, 2, 3, 4)
**	0�͑S��OK��, �G���[�Ȃ�
*******************************************************************/
	int				LastLegErrID;

/*************************
**						**
**		�����o�֐�		**
**						**
*************************/
public:
/*******************************************************************
**  �R���X�g���N�^�ƃf�X�g���N�^�Ƃ��̑�
*******************************************************************/
	Robot();
	Robot(const Matrix& body);
	~Robot();

/******************************************************************************
**  �r�p�̃C���X�^���X�쐬�֐�
******************************************************************************/
	void		CreateLegInstance(void);
	void		DestoryLegInstance(void);

/***************************************************
**	���{�b�g�{�̂̓����ϊ��s��̓O���[�o�����W�n  **
***************************************************/
/******************************************************************************
**  SetTransformMatrix				: �{�̏d�S�̓������W�ϊ��s��IK�ŃZ�b�g
**  SetTransformMatrixWithDK				: �{�̏d�S�̓������W�ϊ��s��DK�ŃZ�b�g
**	GetTransformMatrix				: �{�̏d�S�̓������W�ϊ��s����l��
**	SetMotionStyle					: �{�̂𓮂������@��ݒ�
**	SetBodyPhase					: ���̂̏�Ԃ��Z�b�g
**	GetBodyPhase					: ���̂̏�Ԃ��l��
******************************************************************************/
	// �{�̂Ƒ��������s
	KINE		SetBodyTransform(const Matrix& body);
	KINE		SetBodyTransformWithDK(const Matrix& body);//060813 doi
	Matrix		GetBodyTransform(void) const{return (Body);}

	void		SetMotionStyle(MotionStyle style);
	MotionStyle	GetMotionStyle(void) const {return (BodyMotionStyle);}

	void		SetBodyPhase(BodyPhase phase);
	BodyPhase	GetBodyPhase(void) const{return (BodyPH);}

/*******************************************************************
**  �r�p�̃����o�֐�
*******************************************************************/
/*******************************************************************
**  SetLegPattern 		: �r�̍\�����f�t�H���g�̍\���ɃZ�b�g
**		LegID			: �K�p����rID (0�͑S�Ă̋r, 1,2,3,4)
*******************************************************************/
	int			SetLegPattern (int legID, LegPattern pattern);

/*******************************************************************
**	GetLeg					: �r�̃C���X�^���X�p�̃A�N�Z�b�T
**	SetLegPhase				: �r�̈ʑ����Z�b�g
**	GetLegPhase				: �r�̈ʑ����擾
**		LegID				: �K�p����rID (1,2,3,4)
*******************************************************************/
	Leg*		GetLeg(int legID) const;
	void		SetLegPhase(int legID, LegPhase phase);
	LegPhase	GetLegPhase(int legID) const;

/*******************************************************************
**  �Ō�̃G���[���N�������r��ID���擾
*******************************************************************/
	int			GetLastLegErrID(void) const		{return LastLegErrID;}


/*******************************************************************
**  �r�̃����o�֐����Ăяo��
*******************************************************************/

/*********************************
**  �g�p����Ƃ��͍��W�n�ɒ���  **
*********************************/
/**************************************************************
**  SetLegEndPos			: endpos�̓O���[�o�����W�n�ŋL�q�D4x4�s��
**	//SetLegConfiguration		: target�̓O���[�o�����W�n
**	SetFootConfiguration		: target�̓O���[�o�����W�n(4x4).IK+DK
**	SetLegJntAng
**	SetLegFootForce
**	UpdateLegJntStat		: �w�肵���r��JntStat���X�V
**************************************************************/
	// �{�̂Ƒ��������s
	KINE	SetLegEndPos(int legID, const Matrix& endPos);
//	KINE	SetLegConfiguration(int legID, const Matrix& target);
	KINE	SetFootConfiguration(int legID, const Matrix& target);
	KINE	SetLegJntAng(int legID, const Matrix& jntAng);
	KINE	SetLegFootForce(int legID, const Matrix& force);
	void	UpdateLegJntState(int legID);

/*******************************************************************
**  �e��̌v�Z���s��Leg�N���X�̃����o�֐��Ăяo��
*******************************************************************/
	KINE	CalcLegDirectKinematics(int legID);
	KINE	CalcLegInverseKinematics(int legID);

	void	CalcLegJacobian(int legID);
	void	CalcLegInverseJacobian(int legID);

	KINE	CalcLegCylinderThrust(int legID);
	KINE	CalcLegAngFromLen(int legID, int jntID);
	KINE	CalcLegAngFromLen(int legID);
	KINE	CalcLegLenFromAng(int legID, int jntID);
	KINE	CalcLegLenFromAng(int legID);

	double	CalcLegInnerMotionRange(int legID, double height);
	double	CalcLegOuterMotionRange(int legID, double height);

/*******************************************************************
**  �r�̃����o�ϐ����擾
**	���W�n�͓��̍��W�n�œ�����
*******************************************************************/
	Matrix	GetLegTransformMatrix(int legID, int jntID) const;
	Matrix	GetLegFootTransform(int legID) const;
	Matrix	GetLegFootForce(int legID) const;

	Matrix	GetLegJacobian(int legID) const;
	Matrix	GetLegInvJacobian(int legID) const;

	Matrix	GetLegBaseParameter(int legID) const;

	Matrix	GetLegJntAng(int legID) const;
	Matrix	GetLegJntAngVel(int legID) const;

	Matrix	GetLegCldrLen(int legID) const;
	Matrix	GetLegCldrSpd(int legID) const;
	Matrix	GetLegCldrThrust(int legID) const;

/*******************************************************************
**  �r�̃����o�ϐ����擾
**	���W�n�̓O���[�o�����W�n�œ�����
*******************************************************************/
	Matrix	GetLegTransformMatrixInGlobal(int legID, int jntID) const;
	Matrix	GetLegFootTransformInGlobal(int legID);

/*******************************************************************
**  ���̑�
*******************************************************************/
	int		GetLegLastJntErr(int legID) const;

//*******************************************************************�r�̃����o�֐��Ăяo���͂����܂�


/*******************************************************************
**  ���̑��̃����o�֐�
*******************************************************************/
/***************************************************************************************
**  TransformGlobaltoLocal			: �O���[�o������{�̃��[�J���ւ̍��W�ϊ�
**	TransformLocaltoGlobal			: �{�̃��[�J������O���[�o���ւ̍��W�ϊ�
***************************************************************************************/
	Matrix	TransformationGlobaltoLocal(const Matrix& matrix) const;
	Matrix	TransformationLocaltoGlobal(const Matrix& matrix) const;

private:
/*******************************************************************
**	�R�s�[�R���X�g���N�^�Ƒ�����Z�q�̋֎~
*******************************************************************/
	Robot(const Robot& robot);
	Robot& operator =(const Robot& robot);

/*******************************************************************
**	�������̃w���p�[�֐�
*******************************************************************/
	void		InitRobotParameter(void);

};

#endif // __Robot_h__
