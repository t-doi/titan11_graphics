/*****************************************************************************
**
**	File:	MathGeometryType.h
** 
**	説明:	数学の幾何学型
**
**	Created:	2004/12/07(Tue)		Updated:	2005/02/06(Sun)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

#ifndef __MathGeometryType_h__
#define __MathGeometryType_h__


////////////////////////////////////////////////////////////
////////// 2次元 ///////////////////////////////////////////
////////////////////////////////////////////////////////////
/*****************************************************************************
**	点の座標
*****************************************************************************/
typedef struct
{
	double x;
	double y;
}Point2D;

/*****************************************************************************
**	直線(線分)
*****************************************************************************/
typedef struct
{
	Point2D start;
	Point2D end;
}Line2D;

/*****************************************************************************
**	円
*****************************************************************************/
typedef struct
{
	double x;		// 中心位置 x
	double y;		// 中心位置 y
	double radius;
}Circle2D;

/*****************************************************************************
**	長方形(含，正方形)
*****************************************************************************/
typedef struct
{
	double left;
	double top;
	double right;
	double bottom;
}Rectangle2D;

/*****************************************************************************
**	直線の方程式
*****************************************************************************/
typedef struct LineEquation2D
{
	double a;
	double b;
	double c;
}LineEq2D;

////////////////////////////////////////////////////////////
////////// 3次元 ///////////////////////////////////////////
////////////////////////////////////////////////////////////
/*****************************************************************************
**	点の座標
*****************************************************************************/
typedef struct
{
	double x;
	double y;
	double z;
}Point3D;


#endif // __MathGeometryType_h__
