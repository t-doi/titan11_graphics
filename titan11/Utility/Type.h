/*****************************************************************************
**
**	File:	Type.h
** 
**	����:	TITAN XI�̌^��`�t�@�C��
**
**	Created: 2004/01/27(Tue)	Updated: 2005/01/14(Fri)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

#ifndef	__Type_h__
#define __Type_h__

/*******************************************************************
**  �L�l�}�e�B�N�X�̂��߂̗񋓎q
**		�L�l�}�e�B�N�X�֌W�̕Ԃ�l
*******************************************************************/
typedef enum kinematics
{
	NoKineError,						//  0 // �G���[�Ȃ�
	KineErrReach,						//  1 // �w�肵���ꏊ�ɓ͂��Ȃ�
	KineErrSingular,					//  2 // ���َp��
	KineErrSome,						//  3 // �����G���[
	KineErrOverReachRadius,				//  4 // �r��̐����w�苗���������͈͈ȏ�
	KineErrUnderReachRadius,			//  5 // �r��̐����w�苗���������͈͈ȉ�
	KineErrOverReachHeight,				//  6 // �r��̐����w�苗���������͈͈ȏ�
	KineErrUnderReachHeight,			//  7 // �r��̐����w�苗���������͈͈ȏ�
	KineErrJointOverLimit,				//  8 // �֐ߊp�x�������͈͈ȏ�
	KineErrJointUnderLimit,				//  9 // �֐ߊp�x�������͈͈ȉ�
	KineErrForceOverLimit,				// 10 // �r��͂������͈͈ȏ�
	KineErrForceUnderLimit,				// 11 // �r��͂������͈͈ȉ�
	KineErrCylinderOverLimit,			// 12 // �V�����_�����������͈͈ȏ�
	KineErrCylinderUnderLimit,			// 13 // �V�����_�����������͈͈ȉ�
	KineErrCldrThrustOverLimit,			// 14 // �V�����_���͂������͈͈ȏ�
	KineErrCldrThrustUnderLimit,		// 15 // �V�����_���͂������͈͈ȉ�
} KINE;

/*******************************************************************
**  MotionGenerator�̂��߂̗񋓎q
**		MotionGenerator�̊֐��̕Ԃ�l
*******************************************************************/
typedef enum MotionState
{
	RUN,							// 0 // ���s��
	END,							// 1 // ����I��
	REQUEST,						// 2 // ����v��
	SUSPEND,						// 3 // ���f
	WAIT,							// 4 // �ҋ@
	INVALID,						// 5 // ����
} MOTION;

/*******************************************************************
**  GaitSequence			: ���e�̃V�[�P���X
**		LF_SWING				: ���O�V�r
**		LR_SWING				: ����V�r
**		RF_SWING				: �E�O�V�r
**		RR_SWING				: �E��V�r
**		BODY_MOVE				: ���̐��i
**		MOTIONLESS				: �Î~
*******************************************************************/
typedef	enum GaitSequence
{
	LF_SWING,							// 0 // ���O�V�r(LEG1)
	RF_SWING,							// 1 // �E�O�V�r(LEG2)070302 doi
	LR_SWING,							// 2 // ����V�r(LEG3)070302 doi
	RR_SWING,							// 3 // �E��V�r(LEG4)
	BODY_MOVE,							// 4 // ���̐��i
	MOTIONLESS,							// 5 // �Î~
	ALL_LEG_SWING,						// 6 // �S�r�V�r 060804 doi
} GAITSEQUENCE;

/*******************************************************************
**  GaitDirection		: ��������
**		FORWARD				: �O
**		BACKWARD			: ��
**		LEFTSIDE			: ��
**		RIGHTSIDE			: �E
*******************************************************************/
typedef	enum GaitDirection
{
	FORWARD,							// 0 // �O
	BACKWARD,							// 1 // ��
	LEFTSIDE,							// 2 // ��
	RIGHTSIDE							// 3 // �E
}GAITDIRECTION;

/*******************************************************************
**  PressureControlMode		:�������䃂�[�h
**		PCM_NONE				: �Ȃ�
**		PCM_KEEP				: �ڒn����
**		PCM_LIMIT_SW				: ���~�b�g�X�C�b�`����
*******************************************************************/
#define  PCM_NONE 0
#define  PCM_KEEP 1
#define  PCM_LIMIT_SW 2


#endif	// __Type_h__
