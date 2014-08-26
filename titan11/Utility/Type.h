/*****************************************************************************
**
**	File:	Type.h
** 
**	説明:	TITAN XIの型定義ファイル
**
**	Created: 2004/01/27(Tue)	Updated: 2005/01/14(Fri)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

#ifndef	__Type_h__
#define __Type_h__

/*******************************************************************
**  キネマティクスのための列挙子
**		キネマティクス関係の返り値
*******************************************************************/
typedef enum kinematics
{
	NoKineError,						//  0 // エラーなし
	KineErrReach,						//  1 // 指定した場所に届かない
	KineErrSingular,					//  2 // 特異姿勢
	KineErrSome,						//  3 // 何かエラー
	KineErrOverReachRadius,				//  4 // 脚先の水平指定距離が制限範囲以上
	KineErrUnderReachRadius,			//  5 // 脚先の水平指定距離が制限範囲以下
	KineErrOverReachHeight,				//  6 // 脚先の垂直指定距離が制限範囲以上
	KineErrUnderReachHeight,			//  7 // 脚先の垂直指定距離が制限範囲以上
	KineErrJointOverLimit,				//  8 // 関節角度が制限範囲以上
	KineErrJointUnderLimit,				//  9 // 関節角度が制限範囲以下
	KineErrForceOverLimit,				// 10 // 脚先力が制限範囲以上
	KineErrForceUnderLimit,				// 11 // 脚先力が制限範囲以下
	KineErrCylinderOverLimit,			// 12 // シリンダ長さが制限範囲以上
	KineErrCylinderUnderLimit,			// 13 // シリンダ長さが制限範囲以下
	KineErrCldrThrustOverLimit,			// 14 // シリンダ推力が制限範囲以上
	KineErrCldrThrustUnderLimit,		// 15 // シリンダ推力が制限範囲以下
} KINE;

/*******************************************************************
**  MotionGeneratorのための列挙子
**		MotionGeneratorの関数の返り値
*******************************************************************/
typedef enum MotionState
{
	RUN,							// 0 // 実行中
	END,							// 1 // 動作終了
	REQUEST,						// 2 // 動作要求
	SUSPEND,						// 3 // 中断
	WAIT,							// 4 // 待機
	INVALID,						// 5 // 無効
} MOTION;

/*******************************************************************
**  GaitSequence			: 歩容のシーケンス
**		LF_SWING				: 左前遊脚
**		LR_SWING				: 左後遊脚
**		RF_SWING				: 右前遊脚
**		RR_SWING				: 右後遊脚
**		BODY_MOVE				: 胴体推進
**		MOTIONLESS				: 静止
*******************************************************************/
typedef	enum GaitSequence
{
	LF_SWING,							// 0 // 左前遊脚(LEG1)
	RF_SWING,							// 1 // 右前遊脚(LEG2)070302 doi
	LR_SWING,							// 2 // 左後遊脚(LEG3)070302 doi
	RR_SWING,							// 3 // 右後遊脚(LEG4)
	BODY_MOVE,							// 4 // 胴体推進
	MOTIONLESS,							// 5 // 静止
	ALL_LEG_SWING,						// 6 // 全脚遊脚 060804 doi
} GAITSEQUENCE;

/*******************************************************************
**  GaitDirection		: 歩く方向
**		FORWARD				: 前
**		BACKWARD			: 後
**		LEFTSIDE			: 左
**		RIGHTSIDE			: 右
*******************************************************************/
typedef	enum GaitDirection
{
	FORWARD,							// 0 // 前
	BACKWARD,							// 1 // 後
	LEFTSIDE,							// 2 // 左
	RIGHTSIDE							// 3 // 右
}GAITDIRECTION;

/*******************************************************************
**  PressureControlMode		:油圧制御モード
**		PCM_NONE				: なし
**		PCM_KEEP				: 接地持続
**		PCM_LIMIT_SW				: リミットスイッチ動作
*******************************************************************/
#define  PCM_NONE 0
#define  PCM_KEEP 1
#define  PCM_LIMIT_SW 2


#endif	// __Type_h__
