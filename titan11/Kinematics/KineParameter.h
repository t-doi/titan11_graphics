/*****************************************************************************
**
**	File:	KineParameter.h
**
**	����:	TITAN XI�̉^���w�Ɋւ���p�����[�^
**
**	Created: 2003/09/19(Fri)	Updated: 2005/01/14(Fri)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/
#ifndef	__KineParameter_h__
#define __KineParameter_h__

#include "../Math/MathLibrary.h"

/*******************************************************************
**	Arm�N���X�Ɋ֌W����萔
*******************************************************************/

const int JOINT_NUM				= 3;			// �֐߂̐�
const Matrix ZERO_VECTOR_JOINT	= ZeroMatrix(JOINT_NUM);

// �e�֐߂̉��͈� (�P�� : degree)
const double ANGLE1_MIN			=-95.000;
const double ANGLE1_MAX			= 95.000;
//const double ANGLE2_MIN			= -5.614;
const double ANGLE2_MIN			= -10;//debug
//const double ANGLE2_MAX			= 70.211;
const double ANGLE2_MAX			= 78.000;//debug
//const double ANGLE3_MIN			= -155.481;
const double ANGLE3_MIN			= -180;//debug
const double ANGLE3_MAX			= -39.220;
//const double ANGLE3_MAX			= -42;//debug
const double ANGLE4_MIN			= -180.000;
const double ANGLE4_MAX			= 180.000;

const double ANGLE_DRILL_MIN			= 0.000;//060728 doi
const double ANGLE_DRILL_MAX			= 90.000;//060728 doi
const double ANGLE_LIFT_MIN			= 0.000;//060728 doi
const double ANGLE_LIFT_MAX			= 90.000;//060728 doi

// �r�̓��B�͈� (�P�� : mm)
// ���a����
const double REACH_RADIUS_MIN	= 200.000;
//const double REACH_RADIUS_MIN	= 0.000;//060704 doi
const double REACH_RADIUS_MAX	= 4000.000;
//const double REACH_RADIUS_MAX	= 4100.000;//debug
// ��������
//const double REACH_HEIGHT_MIN	= -3000.000;
const double REACH_HEIGHT_MIN	= -2000.000;//060815 doi
//const double REACH_HEIGHT_MAX	= 3000.000;
const double REACH_HEIGHT_MAX	= 3200.000;

// �r��ɂ�����͂̋��e�͈� (�P�� : kgf)
const double FORCE_X_MIN		= -10000.000;
//const double FORCE_X_MAX		= 10000.000;
const double FORCE_X_MAX		= 20000.000;
const double FORCE_Y_MIN		= -10000.000;
const double FORCE_Y_MAX		= 10000.000;
const double FORCE_Z_MIN		= -10000.000;
const double FORCE_Z_MAX		= 10000.000;

// �r�̊e�����N�� (�P�� : mm)
const double LINK1				= 225.000;
const double LINK2				= 1900.000;
const double LINK3				= 1800.000;

/*******************************************************************
**	Leg�N���X�Ɋ֌W����萔
*******************************************************************/

// ���ꂼ��̃V�����_�̉��͈� (�Œ� > �ō�) (�P�� : mm)
//const double CLDR_LEN1_MIN		= 1000.000;
const double CLDR_LEN1_MIN		= 850.000;
const double CLDR_LEN1_MAX		= 1800.000;
const double CLDR_LEN2_MIN		= 886.000;
//const double CLDR_LEN2_MIN		= 700.000;//debug
const double CLDR_LEN2_MAX		= 1437.000;
//const double CLDR_LEN2_MAX		= 1600.000;//debug
const double CLDR_LEN3_MIN		= 872.000;
//const double CLDR_LEN3_MIN		= 700.000;//debug
const double CLDR_LEN3_MAX		= 1409.000;
//const double CLDR_LEN3_MAX		= 1600.000;//debug

// ���ꂼ��̃V�����_�̍Œ�/�ō��o�� (�P�� : kgf)
const double CLDR_THRUST1_MIN	=	-19617.000;
const double CLDR_THRUST1_MAX	=	23738.000;
const double CLDR_THRUST2_MIN	=	-19617.000;
const double CLDR_THRUST2_MAX	=	23738.000;
const double CLDR_THRUST3_MIN	=	-19617.000;
const double CLDR_THRUST3_MAX	=	23738.000;

// ���񂩂瑫���܂ł̋���
//const double FOOT				=	200.000;
const double FOOT				=	50.000;//�{�[���W���C���g�ɂȂ����̂ŒZ���Ȃ����D051004�v�� doi

// �e���ڋ���
const double	DTC1			=	100.000;
const double	DTC2			=	1054.7;
const double	DTC3			=	1059.43;
const double	DTC4			=	197.9898987;
const double	DTC5			=	197.9898987;
const double	DTC6			=	280.000;
const double	DTC7			=	300.000;
const double	DTC8			=	375.000;
const double	DTC9			=	670.820;
const double	DTC10			=	800.000;
const double	DTC11			=	200.000;
const double	DTC12			=	824.621;
const double	DTC13			=	1070.000;
const double	DTC14			=	300.000;
const double	DTC15			=	1111.260;
const double	DTC16			=	325.000;
const double	DTC17			=	20.000;
const double	DTC18			=	325.614;

/*******************************************************************
**	Robot�N���X�Ɋ֌W����萔
*******************************************************************/

// �r�̐�
const int LEG_NUM				= 4;
const int BODY_NUM				= 1;

// �r�̕t�����̋���
const double BODY_LENGTH		= 3200.000;
const double BODY_WIDTH			= 2000.000;

// �r�̏d��
const double LEG_WEIGHT			= 500.000;

// �{�̏d��
const double BODY_WEIGHT		= 3000.000;

// �r�̍����̃p�����[�^
const double LEG_ROOT_POSX[]	= {BODY_LENGTH/2,	BODY_LENGTH/2,	-BODY_LENGTH/2,	-BODY_LENGTH/2};
const double LEG_ROOT_POSY[]	= {BODY_WIDTH/2,	-BODY_WIDTH/2,	BODY_WIDTH/2,	-BODY_WIDTH/2};
const double LEG_ROOT_POSZ[]	= {0,				0,				0,				0};
const double LEG_ROOT_ANGLE[]	= {PI/2,			-PI/2,			PI/2,			-PI/2};

#endif  // __KineParameter_h__
