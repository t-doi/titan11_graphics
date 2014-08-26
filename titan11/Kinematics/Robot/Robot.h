/********************************************************************************
**  
**	File:			Robot.h
** 
**	Description:	TITAN XIの本体の運動学
**
**	Created: 2003/10/22(Wed)	Updated: 2005/01/14(Fri)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
********************************************************************************/

/***********************************************************************
**  このクラスは主にロボット全体のキネマティクスを表現
***********************************************************************/

#ifndef	__Robot_h__
#define __Robot_h__

#include "Leg/Leg.h"
#include "../KineParameter.h"

class Robot
{
/*************************
**						**
**		メンバ変数		**
**						**
*************************/
public:
/*******************************************************************
**	本体を動かす方法
**		SUPPORT_LEG		: 支持脚のみ
**		ALL_LEG			: 全ての脚
**		CRAWLER			: クローラ
*******************************************************************/
	enum MotionStyle{SUPPORT_LEG, ALL_LEG, CRAWLER};		
	// デフォルトはALL_LEG

/*******************************************************************
**	胴体の状態表示
**		MOTION			: 移動
**		STATIONARY		: 静止
*******************************************************************/
	enum BodyPhase{MOTION, STATIONARY};

/*******************************************************************
**	脚の状態表示
**		SUPPORT			: 支持脚
**		SWING			: 遊脚
*******************************************************************/
	enum LegPhase{SUPPORT, SWING};

/*******************************************************************
**  デフォルトの脚構成
**		RETRACT			: 格納形態
**		RECTANGLE		: 長方形形態
**		PRESET			: 前もって設定した角度に指定
*******************************************************************/
	enum LegPattern{RETRACT, RECTANGLE, PRESET};

private:

protected:
/***************************************************
**	ロボット本体の同次変換行列はグローバル座標系  **
***************************************************/
/*******************************************************************
**  本体のためのメンバ変数
**		Body						: 本体図心の同次変換行列
**		BodyMotionStyle				: 本体の推進方法
**		BodyPhase					: 本体の状態相
*******************************************************************/
	Matrix			Body;
	MotionStyle		BodyMotionStyle;
	BodyPhase		BodyPH;

/*******************************************************************
**  脚のためのメンバ変数
**		MultiLeg				: 脚のキネマティクスオブジェクト
**		LegPH					: 脚の状態相
*******************************************************************/
	Leg**			Legs;
	LegPhase*		LegPH;

/*******************************************************************
**  最後にエラーの原因となった脚のID(1, 2, 3, 4)
**	0は全てOKで, エラーなし
*******************************************************************/
	int				LastLegErrID;

/*************************
**						**
**		メンバ関数		**
**						**
*************************/
public:
/*******************************************************************
**  コンストラクタとデストラクタとその他
*******************************************************************/
	Robot();
	Robot(const Matrix& body);
	~Robot();

/******************************************************************************
**  脚用のインスタンス作成関数
******************************************************************************/
	void		CreateLegInstance(void);
	void		DestoryLegInstance(void);

/***************************************************
**	ロボット本体の同時変換行列はグローバル座標系  **
***************************************************/
/******************************************************************************
**  SetTransformMatrix				: 本体重心の同次座標変換行列IKでセット
**  SetTransformMatrixWithDK				: 本体重心の同次座標変換行列DKでセット
**	GetTransformMatrix				: 本体重心の同次座標変換行列を獲得
**	SetMotionStyle					: 本体を動かす方法を設定
**	SetBodyPhase					: 胴体の状態をセット
**	GetBodyPhase					: 胴体の状態を獲得
******************************************************************************/
	// 本体と足裏が平行
	KINE		SetBodyTransform(const Matrix& body);
	KINE		SetBodyTransformWithDK(const Matrix& body);//060813 doi
	Matrix		GetBodyTransform(void) const{return (Body);}

	void		SetMotionStyle(MotionStyle style);
	MotionStyle	GetMotionStyle(void) const {return (BodyMotionStyle);}

	void		SetBodyPhase(BodyPhase phase);
	BodyPhase	GetBodyPhase(void) const{return (BodyPH);}

/*******************************************************************
**  脚用のメンバ関数
*******************************************************************/
/*******************************************************************
**  SetLegPattern 		: 脚の構成をデフォルトの構成にセット
**		LegID			: 適用する脚ID (0は全ての脚, 1,2,3,4)
*******************************************************************/
	int			SetLegPattern (int legID, LegPattern pattern);

/*******************************************************************
**	GetLeg					: 脚のインスタンス用のアクセッサ
**	SetLegPhase				: 脚の位相をセット
**	GetLegPhase				: 脚の位相を取得
**		LegID				: 適用する脚ID (1,2,3,4)
*******************************************************************/
	Leg*		GetLeg(int legID) const;
	void		SetLegPhase(int legID, LegPhase phase);
	LegPhase	GetLegPhase(int legID) const;

/*******************************************************************
**  最後のエラーが起こった脚のIDを取得
*******************************************************************/
	int			GetLastLegErrID(void) const		{return LastLegErrID;}


/*******************************************************************
**  脚のメンバ関数を呼び出し
*******************************************************************/

/*********************************
**  使用するときは座標系に注意  **
*********************************/
/**************************************************************
**  SetLegEndPos			: endposはグローバル座標系で記述．4x4行列
**	//SetLegConfiguration		: targetはグローバル座標系
**	SetFootConfiguration		: targetはグローバル座標系(4x4).IK+DK
**	SetLegJntAng
**	SetLegFootForce
**	UpdateLegJntStat		: 指定した脚のJntStatを更新
**************************************************************/
	// 本体と足裏が平行
	KINE	SetLegEndPos(int legID, const Matrix& endPos);
//	KINE	SetLegConfiguration(int legID, const Matrix& target);
	KINE	SetFootConfiguration(int legID, const Matrix& target);
	KINE	SetLegJntAng(int legID, const Matrix& jntAng);
	KINE	SetLegFootForce(int legID, const Matrix& force);
	void	UpdateLegJntState(int legID);

/*******************************************************************
**  各種の計算を行うLegクラスのメンバ関数呼び出し
*******************************************************************/
	KINE	CalcLegDirectKinematics(int legID);
	KINE	CalcLegInverseKinematics(int legID);

	void	CalcLegJacobian(int legID);
	void	CalcLegInverseJacobian(int legID);

	KINE	CalcLegCylinderThrust(int legID);
	KINE	CalcLegAngFromLen(int legID, int jntID);
	KINE	CalcLegAngFromLen(int legID);
	KINE	CalcLegLenFromAng(int legID, int jntID);
	KINE	CalcLegLenFromAng(int legID);

	double	CalcLegInnerMotionRange(int legID, double height);
	double	CalcLegOuterMotionRange(int legID, double height);

/*******************************************************************
**  脚のメンバ変数を取得
**	座標系は胴体座標系で得られる
*******************************************************************/
	Matrix	GetLegTransformMatrix(int legID, int jntID) const;
	Matrix	GetLegFootTransform(int legID) const;
	Matrix	GetLegFootForce(int legID) const;

	Matrix	GetLegJacobian(int legID) const;
	Matrix	GetLegInvJacobian(int legID) const;

	Matrix	GetLegBaseParameter(int legID) const;

	Matrix	GetLegJntAng(int legID) const;
	Matrix	GetLegJntAngVel(int legID) const;

	Matrix	GetLegCldrLen(int legID) const;
	Matrix	GetLegCldrSpd(int legID) const;
	Matrix	GetLegCldrThrust(int legID) const;

/*******************************************************************
**  脚のメンバ変数を取得
**	座標系はグローバル座標系で得られる
*******************************************************************/
	Matrix	GetLegTransformMatrixInGlobal(int legID, int jntID) const;
	Matrix	GetLegFootTransformInGlobal(int legID);

/*******************************************************************
**  その他
*******************************************************************/
	int		GetLegLastJntErr(int legID) const;

//*******************************************************************脚のメンバ関数呼び出しはここまで


/*******************************************************************
**  その他のメンバ関数
*******************************************************************/
/***************************************************************************************
**  TransformGlobaltoLocal			: グローバルから本体ローカルへの座標変換
**	TransformLocaltoGlobal			: 本体ローカルからグローバルへの座標変換
***************************************************************************************/
	Matrix	TransformationGlobaltoLocal(const Matrix& matrix) const;
	Matrix	TransformationLocaltoGlobal(const Matrix& matrix) const;

private:
/*******************************************************************
**	コピーコンストラクタと代入演算子の禁止
*******************************************************************/
	Robot(const Robot& robot);
	Robot& operator =(const Robot& robot);

/*******************************************************************
**	初期化のヘルパー関数
*******************************************************************/
	void		InitRobotParameter(void);

};

#endif // __Robot_h__
