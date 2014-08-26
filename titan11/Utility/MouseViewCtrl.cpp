/*****************************************************************************
**
**	File:	  MouseViewCtrl.cpp
**
**	説明: 	TITAN XIの型定義ファイル
**
**	Created: 2004/02/14(Sat)	Updated: 2005/01/06(Thu)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

#include "MouseViewCtrl.h"


/*******************************************************************
**	コンストラクタ
*******************************************************************/
MouseViewCtrl::MouseViewCtrl()
{
	SetDefault();
}

/*******************************************************************
**	デストラクタ
*******************************************************************/
MouseViewCtrl::~MouseViewCtrl()
{
}

/*******************************************************************
**	画面上の操作起点となる点をセット
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
**	メンバ変数の値をデフォルトにセット
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
**	マウスのイベントモードを設定
*******************************************************************/
void MouseViewCtrl::SetDragMode(MouseViewCtrl::DragMode mode)
{
	MouseMode = mode;
}

/*******************************************************************
**	視点の情報を直接設定
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
**	並進操作
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
**	回転操作
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
**	縮尺操作
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
**	マウスのドラッグモードによりViewの変更
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
**	視線方向の取得
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
**	視野の上方向の取得
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
