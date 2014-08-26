/*****************************************************************************
**
**	File:	MathFunction.cpp
** 
**	����:	���w�̎�Ȍ��� 
**
**	Created:	2004/12/07(Tue)		Updated:	2004/12/07(Tue)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

#include "MathFunction.h"

/***************************************************************************************************
**	CosLawtoAng()				: �]���藝����]��(cosine)���v�Z	
**								: �Ԃ�l cos()=(x^2+y^2-z^2)/(2*x*y)
***************************************************************************************************/
double CosLawtoAng(double x, double y, double z)
{
	return ((x*x+y*y-z*z)/(2*x*y));
	
}

/***************************************************************************************************
**	CosLawtoLen()				: �]���藝���璷�����v�Z
**								: �Ԃ�l length=sqrt(x^2+y^2-2*x*y*cos(theta))
***************************************************************************************************/
double CosLawtoLen(double x, double y, double theta)
{
	return (sqrt(x*x+y*y-2*x*y*cos(theta)));
}

/***************************************************************************************************
**	DiffArcCosine()				: �t�]��(arccosine)�̔���
**								: �Ԃ�l -1/sqrt(1-x^2)
***************************************************************************************************/
double DiffArcCosine(double x)
{
	return (-1/sqrt(1-x*x));
}

/*****************************************************************************
**	Within							: �͈͓��ɂ��邩�ǂ���
*****************************************************************************/
bool IsParamWithin(double param, double min, double max)
{
	if ( (param<min) || (max<param) )
	{
		return (false);
	}

	return (true);
}

bool IsParamWithin(int param, int min, int max)
{
	if ( (param<min) || (max<param) )
	{
		return (false);
	}

	return (true);
}

/*****************************************************************************
**	DividingPointRatio				: �����_�̔䗦���o��
*****************************************************************************/
double DividingPointRatio(double inter, double min, double max)
{
	return ( (inter - min)/(max - min) );
}

/*****************************************************************************
**	RoundOff				: �l�̌ܓ�
*****************************************************************************/
int RoundOff(double value, int place)
{
	// �ړI�̌��������_��ʂɂ���
	double param = param = value/pow((double)10, (double)(place+1));

	// �����_1�ʂ��l�̌ܓ�
	// ���̎��ɂ�0.5�����Z���ăL���X�g
	double result = ( ( param>0 ) ? (int)( param + 0.5 ) : (int)( param - 0.5 ) );

	return ( (int)( result*pow((double)10, (double)(place+1)) ) );
}
