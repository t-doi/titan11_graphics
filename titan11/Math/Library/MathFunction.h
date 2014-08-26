/*****************************************************************************
**
**	File:	MathFunction.h
** 
**	����:	���w�̎�Ȍ��� 
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
//////////  �O�p�֐�  ///////////////////////////
/////////////////////////////////////////////////
/***************************************************************************************************
**	CosLawtoAng()				: �]���藝����]��(cosine)���v�Z	
**								: �Ԃ�l cos()=(x^2+y^2-z^2)/(2*x*y)
**	CosLawtoLen()				: �]���藝���璷�����v�Z
**								: �Ԃ�l length=sqrt(x^2+y^2-2*x*y*cos(theta))
**	DiffArcCosine()				: �t�]��(arccosine)�̔���
**								: �Ԃ�l -1/sqrt(1-x^2)
***************************************************************************************************/
extern double	CosLawtoAng(double x, double y, double z);
extern double	CosLawtoLen(double x, double y, double theta);
extern double	DiffArcCosine(double x);

/////////////////////////////////////////////////
//////////  ���̑�  /////////////////////////////
/////////////////////////////////////////////////
/*****************************************************************************
**	�͈͓��ɂ��邩�ǂ���				: �����_�̔䗦���o��
*****************************************************************************/
extern bool		IsParamWithin(double param, double min, double max);
extern bool		IsParamWithin(int param, int min, int max);

/*****************************************************************************
**	DividingPointRatio				: �����_�̔䗦���o��
*****************************************************************************/
extern double	DividingPointRatio(double inter, double min, double max);

/*****************************************************************************
**	RoundOff			: �l�̌ܓ�
**		value				: �ړI�̐�
**		place				: �����ڂ��l�̌ܓ����邩(�w���w��)
*****************************************************************************/
extern int		RoundOff(double value, int place);

#endif // __MathFunction_h__
