/*****************************************************************************
**  
**	File:	  MathSymbol.h
**
**	Description: This include file contains macros frequently used.
**
**	Created: 2004/11/20(Sat)	Updated: 2005/02/02(Wed)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
******************************************************************************/

#ifndef __MathSymbol_h__
#define __MathSymbol_h__

#include "Library/MathMatrix.h"

/*****************************************************************************
**	�s���x�N�g���̂��߂̒萔
*****************************************************************************/
const int POS_DOF				= 3;		// �ʒu����̂��߂̎��R�x
const int DH_NUM				= 4;		// �����ϊ��s��̍s�Ɨ�̗v�f��


/*****************************************************************************
**	��\�I�ȍs���x�N�g��
*****************************************************************************/
const Matrix	IDENTITY_MATRIX_DH	= IdentityMatrix(DH_NUM, DH_NUM);
const Matrix	ZERO_MATRIX_3X3		= ZeroMatrix(3, 3);
const Matrix	ZERO_VECTOR_3		= ZeroMatrix(3);
const Matrix	ZERO_VECTOR_4		= ZeroMatrix(4);
const Matrix	GRAVITY				= PositionVecter(0, 0, -1);

#endif // __MathSymbol_h__
