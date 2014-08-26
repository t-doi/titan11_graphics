/*****************************************************************************
**
**	File:	  MouseViewCtrl.cpp
**
**	����: 	TITAN XI�̌^��`�t�@�C��
**
**	Created: 2004/02/14(Sat)	Updated: 2005/01/06(Thu)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

#include "MouseViewCtrl.h"


/*******************************************************************
**	�R���X�g���N�^
*******************************************************************/
MouseViewCtrl::MouseViewCtrl()
{
	SetDefault();
}

/*******************************************************************
**	�f�X�g���N�^
*******************************************************************/
MouseViewCtrl::~MouseViewCtrl()
{
}

/*******************************************************************
**	��ʏ�̑���N�_�ƂȂ�_���Z�b�g
*******************************************************************/
void MouseViewCtrl::SetCtrlPoint(int x, int y)
{
	InitX = x;
	InitY = y;

	InitAzimuth		= Azimuth;
	InitElevation	= Elevation;
	InitDistance	= Distance;

	for (int i=0; i<3; i++)
		InitViewCenterPos[i] = ViewCenterPos[i];

	return;
}

/*******************************************************************
**	�����o�ϐ��̒l���f�t�H���g�ɃZ�b�g
*******************************************************************/
void MouseViewCtrl::SetDefault(void)
{
	InitX = 0;
	InitY = 0;

	Azimuth		= 30;
	Elevation	= 30;
	Distance	= 50000;

	for (int i=0; i<3; i++)
		ViewCenterPos[i] = 0.0;

	SetCtrlPoint(0, 0);

	//MouseViewCtrl::MouseMode = DragMode::STOP;
	MouseViewCtrl::MouseMode = STOP;

	return;
}

/*******************************************************************
**	�}�E�X�̃C�x���g���[�h��ݒ�
*******************************************************************/
void MouseViewCtrl::SetDragMode(MouseViewCtrl::DragMode mode)
{
	MouseMode = mode;
}

/*******************************************************************
**	���_�̏��𒼐ڐݒ�
*******************************************************************/
void MouseViewCtrl::SetAzimuth(double azimuth)
{
	Azimuth = azimuth;
	return;
}

void MouseViewCtrl::SetElevation(double elevation)
{
	Elevation = elevation;
	return;
}

void MouseViewCtrl::SetDistance(double distance)
{
	Distance = distance;
	return;
}

/*******************************************************************
**	���i����
*******************************************************************/
void MouseViewCtrl::Translate(int x, int y)
{
	double dx = (double)(x - InitX);
	double dy = (double)(y - InitY);

	double upview[3], view[3], right[3];

	GetUpViewing(&upview[0], &upview[1], &upview[2]);

	GetViewing(&view[0], &view[1], &view[2]);

	OuterProduct(view[0], view[1], view[2],
				 upview[0], upview[1], upview[2],
				 &right[0], &right[1], &right[2]
				 );

	double vx = 5.0E-4;
	double vy = 5.0E-4;

	for (int i=0; i<3; i++)
	{
		ViewCenterPos[i] = ( vx*dx*right[i] + vy*dy*upview[i] ) + InitViewCenterPos[i];
	}


	return;
}

/*******************************************************************
**	��]����
*******************************************************************/
void MouseViewCtrl::Rotate(int x, int y)
{
	double dx = (double)(x - InitX);
	double dy = (double)(y - InitY);

	Azimuth		= (-0.5)*(double)dx + InitAzimuth;
	Elevation	= (0.5)*(double)dy + InitElevation;

	return;
}

/*******************************************************************
**	�k�ڑ���
*******************************************************************/
void MouseViewCtrl::Scale(int x, int y)
{
	double dx = (double)(x - InitX);
	double dy = (double)(y - InitY);

	double dl = dy;

	Distance = (100.0)*dl + InitDistance;

	return;
}

/*******************************************************************
**	�}�E�X�̃h���b�O���[�h�ɂ��View�̕ύX
*******************************************************************/
void MouseViewCtrl::TransformView(int x, int y)
{
	switch ( MouseMode )
	{
		case MouseViewCtrl::TRANSLATE:
			Translate(x, y);
			break;

		case MouseViewCtrl::ROTATE:
			Rotate(x, y);
			break;

		case MouseViewCtrl::SCALE:
			Scale(x, y);
			break;

		case MouseViewCtrl::STOP:
			return;
			break;

		default:
			return;
			break;
	}
}

/*******************************************************************
**	���������̎擾
*******************************************************************/
void MouseViewCtrl::GetViewing(double* x, double* y, double* z)
{
	double xo = cos(Elevation*PI/180)*cos(Azimuth*PI/180);
	double yo = cos(Elevation*PI/180)*sin(Azimuth*PI/180);
	double zo = sin(Elevation*PI/180);

	*x = xo;
	*y = yo;
	*z = zo;

	return;
}

/*******************************************************************
**	����̏�����̎擾
*******************************************************************/
void MouseViewCtrl::GetUpViewing(double* x, double* y, double* z)
{
	double xo = Distance*cos( (Elevation+90)*PI/180 )*cos(Azimuth*PI/180);
	double yo = Distance*cos( (Elevation+90)*PI/180 )*sin(Azimuth*PI/180);
	double zo = Distance*sin( (Elevation+90)*PI/180 );

	*x = xo;
	*y = yo;
	*z = zo;

	return;
}
