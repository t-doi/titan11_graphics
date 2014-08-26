/*****************************************************************************
**
**	File:	MathMatrix.h
** 
**	説明:	数学の主な行列公式 
**
**	Created:	2004/12/07(Tue)		Updated:	2004/12/07(Tue)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

#ifndef __MathMatrix_h__
#define __MathMatrix_h__

#include <math.h>
#include "../MathConstant.h"
#include "../Matrix/Matrix.h"

/*******************************************************************************
**	IdentityMatrix(int row, int column)		: x軸回りの回転行列を返す
**	ZeroMatrix(int row, int column)			: y軸回りの回転行列を返す
*******************************************************************************/
extern Matrix	IdentityMatrix(int row, int column=1);
extern Matrix	ZeroMatrix(int row, int column=1);

/*******************************************************************************
**	3次元の位置ベクトルを得る
*******************************************************************************/
extern Matrix	PositionVecter(double x, double y, double z);

/*******************************************************************************
**	RotaionX(double ang)		: x軸回りの回転行列を返す
**	RotaionY(double ang)		: y軸回りの回転行列を返す
**	RotaionZ(double ang)		: z軸回りの回転行列を返す
*******************************************************************************/
extern Matrix	RotationX(double ang);
extern Matrix	RotationY(double ang);
extern Matrix	RotationZ(double ang);
/*******************************************************************************
**	RotationVector				: 任意軸回りの回転行列を返す
*******************************************************************************/
extern Matrix	RotationVector(const Matrix& axis, double ang);

/*****************************************************************************
**	RPYtoRotMatrix				: Roll, Pitch, Yaw角から回転行列を計算
**	RotMatrixtoRPY				: 回転行列からRoll, Pitch, Yaw角を計算
*****************************************************************************/
extern Matrix	RPYtoRotMatrix(double roll, double pitch, double yaw);
extern Matrix	RPYtoRotMatrix(const Matrix& rpy);
extern Matrix	RotMatrixtoRPY(const Matrix& rot, int dir=1);	// dir should be 1 or -1

/*****************************************************************************
**	InnerProduct				: xベクトルとyベクトルから内積を計算
**	OuterProduct				: xベクトルとyベクトルから外積を計算
*****************************************************************************/
extern double	InnerProduct(const Matrix& x, const Matrix& y);
extern Matrix	OuterProduct(const Matrix& x, const Matrix& y);

/*****************************************************************************
**	InnerProduct				: 引数から内積を計算
**	OuterProduct				: 引数から外積を計算
*****************************************************************************/
extern	double	InnerProduct(double x1, double y1, double z1,
							 double x2, double y2, double z2);
extern	int		OuterProduct(double x1, double y1, double z1,
							 double x2, double y2, double z2,
							 double* x, double* y, double* z);

#endif // __MathMatrix_h__
