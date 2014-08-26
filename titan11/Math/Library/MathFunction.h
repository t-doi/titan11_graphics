/*****************************************************************************
**
**	File:	MathFunction.h
** 
**	説明:	数学の主な公式 
**
**	Created:	2004/12/07(Tue)		Updated:	2004/12/07(Tue)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

#ifndef __MathFunction_h__
#define __MathFunction_h__

#include <math.h>
#include "../MathConstant.h"

/////////////////////////////////////////////////
//////////  三角関数  ///////////////////////////
/////////////////////////////////////////////////
/***************************************************************************************************
**	CosLawtoAng()				: 余弦定理から余弦(cosine)を計算	
**								: 返り値 cos()=(x^2+y^2-z^2)/(2*x*y)
**	CosLawtoLen()				: 余弦定理から長さを計算
**								: 返り値 length=sqrt(x^2+y^2-2*x*y*cos(theta))
**	DiffArcCosine()				: 逆余弦(arccosine)の微分
**								: 返り値 -1/sqrt(1-x^2)
***************************************************************************************************/
extern double	CosLawtoAng(double x, double y, double z);
extern double	CosLawtoLen(double x, double y, double theta);
extern double	DiffArcCosine(double x);

/////////////////////////////////////////////////
//////////  その他  /////////////////////////////
/////////////////////////////////////////////////
/*****************************************************************************
**	範囲内にあるかどうか				: 内分点の比率を出す
*****************************************************************************/
extern bool		IsParamWithin(double param, double min, double max);
extern bool		IsParamWithin(int param, int min, int max);

/*****************************************************************************
**	DividingPointRatio				: 内分点の比率を出す
*****************************************************************************/
extern double	DividingPointRatio(double inter, double min, double max);

/*****************************************************************************
**	RoundOff			: 四捨五入
**		value				: 目的の数
**		place				: 何桁目を四捨五入するか(指数指定)
*****************************************************************************/
extern int		RoundOff(double value, int place);

#endif // __MathFunction_h__
