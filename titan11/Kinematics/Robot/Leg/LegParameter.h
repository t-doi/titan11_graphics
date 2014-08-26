/*****************************************************************************
**
**	File:	  LegParameter.h
**
**	説明: TITAN XIの脚に関するパラメータ
**
**	Created: 2003/09/19(Fri)	Updated: 2004/01/17(Sat)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/
#ifndef	__LegParameter_h__
#define __LegParameter_h__

/*******************************************************************
**	関節の数
*******************************************************************/
#define JntNum				4
/*******************************************************************
**	位置決定のための自由度
*******************************************************************/
#define PosDOF				3
/*******************************************************************
**	デフォルトの足裏関節角度
*******************************************************************/
#define DefaultFootAngle	0
/*******************************************************************
**	同時変換行列の行と列の要素数
*******************************************************************/
#define DHNum				4

/*******************************************************************
**	それぞれのシリンダの可動範囲 (最低 > 最高) (単位 : mm)
*******************************************************************/
#define	CldrLen1_MIN		1000.000
#define	CldrLen1_MAX		1800.000
#define	CldrLen2_MIN		886.000
#define	CldrLen2_MAX		1437.000
#define	CldrLen3_MIN		872.000
#define	CldrLen3_MAX		1409.000
#define	CldrLen4_MIN		200.000
#define	CldrLen4_MAX		200.000


/*******************************************************************
**	それぞれのシリンダの最低/最高出力 (単位 : kgf)
*******************************************************************/
#define CldrThrust1_MIN		0.000
#define CldrThrust1_MAX		0.000
#define CldrThrust2_MIN		0.000
#define CldrThrust2_MAX		0.000
#define CldrThrust3_MIN		0.000
#define CldrThrust3_MAX		0.000
#define CldrThrust4_MIN		0.000
#define CldrThrust4_MAX		0.000

/*******************************************************************
**	各関節の可動範囲 (単位 : degree)
*******************************************************************/
#define ANGLE1_MIN			-180.000
#define ANGLE1_MAX			180.000
#define ANGLE2_MIN			-180.000
#define ANGLE2_MAX			180.000
#define ANGLE3_MIN			-180.000
#define ANGLE3_MAX			180.000
#define ANGLE4_MIN			-180.000
#define ANGLE4_MAX			180.000

/*******************************************************************
**	脚の到達範囲 (単位 : mm)
*******************************************************************/
// 半径方向
#define REACH_RADIUS_MIN	200.000
#define REACH_RADIUS_MAX	3500.000
// 高さ方向
#define REACH_HEIGHT_MIN	-3000.000
#define REACH_HEIGHT_MAX	3000.000

/*******************************************************************
**	脚先にかかる力の許容範囲 (単位 : kgf)
*******************************************************************/
#define FORCE_X_MIN			-3000.000
#define FORCE_X_MAX			3000.000
#define FORCE_Y_MIN			-3000.000
#define FORCE_Y_MAX			3000.000
#define FORCE_Z_MIN			-3000.000
#define FORCE_Z_MAX			3000.000

/*******************************************************************
**	脚の各距離パラメータ (単位 : mm)
*******************************************************************/
// リンク長
#define	LINK1				225.000
#define	LINK2				1900.000
#define	LINK3				1800.000
#define LINK4				200.000
#define FOOT				200.000			// LINK4の別名(長さは常に同じにする事)

// 各注目距離
#define	DTC1				100.000
#define	DTC2				1054.7
#define	DTC3				1059.43
#define	DTC4				197.9898987
#define	DTC5				197.9898987
#define	DTC6				280.000
#define	DTC7				300.000
#define	DTC8				375.000
#define	DTC9				670.820
#define	DTC10				800.000
#define	DTC11				200.000
#define	DTC12				824.621
#define	DTC13				1070.000
#define	DTC14				300.000
#define	DTC15				1111.260
#define	DTC16				325.000
#define	DTC17				20.000
#define	DTC18				325.614
#define	DTC19				200.000
#define	DTC20				200.000
#define	DTC21				200.000
#define	DTC22				200.000
#define	DTC23				200.000
#define	DTC24				200.000
#define	DTC25				200.000
#define	DTC26				200.000
#define	DTC27				200.000
#define	DTC28				200.000
#define	DTC29				200.000
#define	DTC30				200.000

#endif  // __LegParameter_h__
