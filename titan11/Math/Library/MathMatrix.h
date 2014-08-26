/*****************************************************************************
**
**	File:	MathMatrix.h
** 
**	����:	���w�̎�ȍs����� 
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
**	IdentityMatrix(int row, int column)		: x�����̉�]�s���Ԃ�
**	ZeroMatrix(int row, int column)			: y�����̉�]�s���Ԃ�
*******************************************************************************/
extern Matrix	IdentityMatrix(int row, int column=1);
extern Matrix	ZeroMatrix(int row, int column=1);

/*******************************************************************************
**	3�����̈ʒu�x�N�g���𓾂�
*******************************************************************************/
extern Matrix	PositionVecter(double x, double y, double z);

/*******************************************************************************
**	RotaionX(double ang)		: x�����̉�]�s���Ԃ�
**	RotaionY(double ang)		: y�����̉�]�s���Ԃ�
**	RotaionZ(double ang)		: z�����̉�]�s���Ԃ�
*******************************************************************************/
extern Matrix	RotationX(double ang);
extern Matrix	RotationY(double ang);
extern Matrix	RotationZ(double ang);
/*******************************************************************************
**	RotationVector				: �C�ӎ����̉�]�s���Ԃ�
*******************************************************************************/
extern Matrix	RotationVector(const Matrix& axis, double ang);

/*****************************************************************************
**	RPYtoRotMatrix				: Roll, Pitch, Yaw�p�����]�s����v�Z
**	RotMatrixtoRPY				: ��]�s�񂩂�Roll, Pitch, Yaw�p���v�Z
*****************************************************************************/
extern Matrix	RPYtoRotMatrix(double roll, double pitch, double yaw);
extern Matrix	RPYtoRotMatrix(const Matrix& rpy);
extern Matrix	RotMatrixtoRPY(const Matrix& rot, int dir=1);	// dir should be 1 or -1

/*****************************************************************************
**	InnerProduct				: x�x�N�g����y�x�N�g��������ς��v�Z
**	OuterProduct				: x�x�N�g����y�x�N�g������O�ς��v�Z
*****************************************************************************/
extern double	InnerProduct(const Matrix& x, const Matrix& y);
extern Matrix	OuterProduct(const Matrix& x, const Matrix& y);

/*****************************************************************************
**	InnerProduct				: ����������ς��v�Z
**	OuterProduct				: ��������O�ς��v�Z
*****************************************************************************/
extern	double	InnerProduct(double x1, double y1, double z1,
							 double x2, double y2, double z2);
extern	int		OuterProduct(double x1, double y1, double z1,
							 double x2, double y2, double z2,
							 double* x, double* y, double* z);

#endif // __MathMatrix_h__
