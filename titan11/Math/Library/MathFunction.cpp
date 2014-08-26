/*****************************************************************************
**
**	File:	MathFunction.cpp
** 
**	説明:	数学の主な公式 
**
**	Created:	2004/12/07(Tue)		Updated:	2004/12/07(Tue)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

#include "MathFunction.h"

/***************************************************************************************************
**	CosLawtoAng()				: 余弦定理から余弦(cosine)を計算	
**								: 返り値 cos()=(x^2+y^2-z^2)/(2*x*y)
***************************************************************************************************/
double CosLawtoAng(double x, double y, double z)
{
	return ((x*x+y*y-z*z)/(2*x*y));
	
}

/***************************************************************************************************
**	CosLawtoLen()				: 余弦定理から長さを計算
**								: 返り値 length=sqrt(x^2+y^2-2*x*y*cos(theta))
***************************************************************************************************/
double CosLawtoLen(double x, double y, double theta)
{
	return (sqrt(x*x+y*y-2*x*y*cos(theta)));
}

/***************************************************************************************************
**	DiffArcCosine()				: 逆余弦(arccosine)の微分
**								: 返り値 -1/sqrt(1-x^2)
***************************************************************************************************/
double DiffArcCosine(double x)
{
	return (-1/sqrt(1-x*x));
}

/*****************************************************************************
**	Within							: 範囲内にあるかどうか
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
**	DividingPointRatio				: 内分点の比率を出す
*****************************************************************************/
double DividingPointRatio(double inter, double min, double max)
{
	return ( (inter - min)/(max - min) );
}

/*****************************************************************************
**	RoundOff				: 四捨五入
*****************************************************************************/
int RoundOff(double value, int place)
{
	// 目的の桁を小数点一位にする
	double param = param = value/pow((double)10, (double)(place+1));

	// 小数点1位を四捨五入
	// 負の時には0.5を減算してキャスト
	double result = ( ( param>0 ) ? (int)( param + 0.5 ) : (int)( param - 0.5 ) );

	return ( (int)( result*pow((double)10, (double)(place+1)) ) );
}
