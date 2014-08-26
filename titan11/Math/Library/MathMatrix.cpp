/*****************************************************************************
**
**	File:	MathMatrix.cpp
** 
**	説明:	数学の主な行列公式 
**
**	Created:	2004/12/07(Tue)		Updated:	2004/12/07(Tue)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/
	
#include "MathMatrix.h"

/*******************************************************************************
**	IdentityMatrix(int row, int column)		: 単位行列を返す
**	ZeroMatrix(int row, int column)			: 零行列を返す
*******************************************************************************/
Matrix IdentityMatrix(int row, int column)
{
	Matrix identity(row, column);
	identity.LoadIdentity();

	return (identity);
}

Matrix ZeroMatrix(int row, int column)
{
	Matrix zero(row, column);
	zero.LoadZero();

	return (zero);
}

/*******************************************************************************
**	3次元の位置ベクトルを得る
*******************************************************************************/
Matrix PositionVecter(double x, double y, double z)
{
	Matrix pos(3);

	pos(1) = x;
	pos(2) = y;
	pos(3) = z;

	return (pos);
}



/*****************************************************************************
**	Rotation	: それぞれ軸回りの回転行列の生成
**	ang			: 回転角 
*****************************************************************************/
Matrix RotationX(double ang)
{
	Matrix rot(3,3);
	
	rot(1,1) = 1.0;
	rot(1,2) = 0.0;
	rot(1,3) = 0.0;
	rot(2,1) = 0.0;
	rot(2,2) = cos(ang);
	rot(2,3) = sin(ang);
	rot(3,1) = 0.0;
	rot(3,2) =-sin(ang);
	rot(3,3) = cos(ang);

	return (rot);
}

Matrix RotationY(double ang)
{
	Matrix rot(3,3);

	rot(1,1) = cos(ang);
	rot(1,2) = 0.0;
	rot(1,3) =-sin(ang);
	rot(2,1) = 0.0;
	rot(2,2) = 1.0;
	rot(2,3) = 0.0;
	rot(3,1) = sin(ang);
	rot(3,2) = 0.0;
	rot(3,3) = cos(ang);

	return (rot);
}

Matrix RotationZ(double ang)
{
	Matrix rot(3,3);

	rot(1,1) = cos(ang);
	rot(1,2) = sin(ang);
	rot(1,3) = 0.0;
	rot(2,1) =-sin(ang);
	rot(2,2) = cos(ang);
	rot(2,3) = 0.0;	
	rot(3,1) = 0.0;
	rot(3,2) = 0.0;
	rot(3,3) = 1.0;	

	return (rot);
}

Matrix RotationVector(const Matrix& axis, double ang)
{
	Matrix rot(3,3);
	Matrix unit(3);
	unit = axis;
	unit.Normalize();

	rot(1,1) = cos(ang) + unit(1)*unit(1)*( 1-cos(ang) );
	rot(1,2) = unit(1)*unit(2)*( 1-cos(ang) ) - unit(3)*sin(ang);
	rot(1,3) = unit(1)*unit(3)*( 1-cos(ang) ) + unit(2)*sin(ang);
	rot(2,1) = unit(1)*unit(2)*( 1-cos(ang) ) + unit(3)*sin(ang);
	rot(2,2) = cos(ang) + unit(2)*unit(2)*( 1-cos(ang) );
	rot(2,3) = unit(2)*unit(3)*( 1-cos(ang) ) - unit(1)*sin(ang);	
	rot(3,1) = unit(1)*unit(3)*( 1-cos(ang) ) - unit(2)*sin(ang);
	rot(3,2) = unit(2)*unit(3)*( 1-cos(ang) ) + unit(1)*sin(ang);
	rot(3,3) = cos(ang) + unit(3)*unit(3)*( 1-cos(ang) );

	return (rot);
}

/*****************************************************************************
**	RPYtoRotMatrix				: Roll, Pitch, Yaw角から回転行列を計算
*****************************************************************************/
Matrix RPYtoRotMatrix(const Matrix& rpy)
{
	if (rpy.GetRow() != 3 || rpy.GetCol() != 1)
	{
		exit(1);
	}

	double roll, pitch, yaw;
	roll = rpy(1);
	pitch = rpy(2);
	yaw = rpy(3);

	Matrix rot(3,3);

	rot(1,1) = cos(yaw)*cos(pitch);
	rot(1,2) = cos(yaw)*sin(pitch)*sin(roll)-sin(yaw)*cos(roll);
	rot(1,3) = cos(yaw)*sin(pitch)*cos(roll)+sin(yaw)*sin(roll);
	rot(2,1) = sin(yaw)*cos(pitch);
	rot(2,2) = sin(yaw)*sin(pitch)*sin(roll)+cos(yaw)*cos(roll);
	rot(2,3) = sin(yaw)*sin(pitch)*cos(roll)-cos(yaw)*sin(roll);
	rot(3,1) = -sin(pitch);
	rot(3,2) = cos(pitch)*sin(roll);
	rot(3,3) = cos(pitch)*cos(roll);

	return (rot);
}

Matrix RPYtoRotMatrix(double roll, double pitch, double yaw)
{
	Matrix rpy(3);
	Matrix rot(3,3);

	rpy(1) = roll;
	rpy(2) = pitch;
	rpy(3) = yaw;

	rot = RPYtoRotMatrix(rpy);

	return (rot);

}

/*****************************************************************************
**	RotMatrixtoRPY				: 回転行列からRoll, Pitch, Yaw角を計算
*****************************************************************************/
Matrix RotMatrixtoRPY(const Matrix& rot, int dir)
{
	Matrix rpy(3);		// Vector(roll, pitch, yaw)
	double roll, pitch, yaw;
	double a;			// valiable for calculation

	if (rot.GetRow() !=3 || rot.GetCol() != 3 || abs(dir) !=1)
	{
		exit(1);
	}

roll=0;pitch=0;yaw=0;//070207 doi

	switch (dir)
	{
		case 1:		// -90 deg < pitch < 90 deg 
			if ( fabs( rot(3,1) ) != 1 )			// pitch != 90 deg
			{
				yaw = atan2( rot(2,1), rot(1,1) );

				pitch = atan2( -rot(3,1), sqrt( rot(3,2)*rot(3,2) ) );

				roll = atan2( rot(3,2), rot(3,3) );
			}
			else if ( rot(3,1) = 1 )
			{
				pitch = -PI/2;

				a = asin( -rot(2,3) );			// -PI/2 < a < PI/2
		
				if ( rot(2,2) >= 0 )				// many other solutions exist
					roll = a;
				else if ( rot(2,2) < 0 )
					roll = PI - a;

				yaw = 0;
			}
			else if ( rot(3,1) = -1 )
			{
				pitch = PI/2;
	
				a = asin( -rot(2,3) );			// -PI/2 < a < PI/2
		
				if ( rot(2,2) >= 0 )
					roll = a;
				else if ( rot(2,2) < 0 )
					roll = PI - a;

				yaw = 0;
			}
			break;

		case -1:		// 90 deg < pitch < 270 deg
			if (fabs( rot(3,1)) != 1 )			// pitch != 90 deg
			{
				yaw = atan2( -rot(2,1), -rot(1,1) );
		
				pitch = atan2( -rot(3,1), -sqrt( rot(3,2)*rot(3,2) ) );

				roll = atan2( -rot(3,2), -rot(3,3) );
			}
			else if ( rot(3,1) = 1 )
			{
				pitch = -PI/2;

				a = asin( -rot(2,3) );			// -PI/2 < a < PI/2
		
				if ( rot(2,2) >= 0 )				// many other solutions exist	
					roll = a;
				else if ( rot(2,2) < 0 )
					roll = PI - a;

				yaw = 0;
			}
			else if ( rot(3,1) = -1 )
			{
				pitch = PI/2;

				a = asin( -rot(2,3) );			// -PI/2 < a < PI/2
		
				if ( rot(2,2) >= 0 )
					roll = a;
				else if ( rot(2,2) < 0 )
					roll = PI - a;

				yaw = 0;
			}
			break;
		}

	rpy(1) = roll;
	rpy(2) = pitch;
	rpy(3) = yaw;

	return (rpy);

}


/*****************************************************************************
**	InnerProduct				: xベクトルとyベクトルから内積を計算 x･y
*****************************************************************************/
double InnerProduct(const Matrix& x, const Matrix& y)
{
	if (x.GetRow() != y.GetRow() || x.GetCol() != 1 || y.GetCol() != 1)
	{
		exit(1);
	}

	int i;
	double ip=0;

	for (i=1; i<=x.GetRow(); i++)
        ip += x(i)*y(i);

	return ip;
}

/*****************************************************************************
**	OuterProduct				: xベクトルとyベクトルから外積を計算 x X y
*****************************************************************************/
Matrix OuterProduct(const Matrix& x, const Matrix& y)
{
	if (x.GetRow() != 3 || y.GetRow() != 3 || x.GetCol() != 1 || y.GetCol() != 1)
	{
		exit(1);
	}

	Matrix op(3);

	op(1) = x(2)*y(3) - x(3)*y(2);
	op(2) = x(3)*y(1) - x(1)*y(3);
	op(3) = x(1)*y(2) - x(2)*y(1);

	return op;
}

/*****************************************************************************
**	InnerProduct				: 引数から内積を計算
*****************************************************************************/
double InnerProduct(double x1, double y1, double z1, double x2, double y2, double z2)
{
	double ip;

	ip = x1*x2 + y1*y2 + z1*z2;

	return (ip);
}

/*****************************************************************************
**	OuterProduct				: 引数から外積を計算
*****************************************************************************/
int OuterProduct(double x1, double y1, double z1, double x2, double y2, double z2,
				 double* x, double* y, double* z)
{
	*x = (y1*z2 - z1*y2);
	*y = (z1*x2 - x1*z2);
	*z = (x1*y2 - y1*x2);

	return (0);
}
