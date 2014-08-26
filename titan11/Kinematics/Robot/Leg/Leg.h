/*****************************************************************************
**
**	File:	  Leg.h
** 
**	説明: TITAN XIの1脚制御用
**
**	Created: 2003/09/19(Fri)	Updated: 2005/01/14(Fri)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

/***********************************************************************
**  このクラスは主に1脚用のキネマティクスを表現
**	インスタンス重視で考える
***********************************************************************/

#ifndef	__Leg_h__
#define __Leg_h__

#include "Arm.h"

class Leg : public Arm
{
/*************************
**						**
**		メンバ変数		**
**						**
*************************/
public:
private:
/***********************************************************************
**  座標系は全てグローバル (ロボットに搭載したら機体座標)
***********************************************************************/
/******************************************************************************
	Foot				: (4,4) 足首先の同時変換行列
	Jacobian			: (3,3)	ヤコビ行列
	InvJacobian			: (3,3)	逆ヤコビ行列
	JacobianL			: (3,3) シリンダと関節角のヤコビ
	InvJacobianL		: (3,3) シリンダと関節角の逆ヤコビ
	FootForce			: (3)	脚先での力のベクトル
	CldrLen				: (3)	各シリンダ長さ
	CldrSpd				: (3)	各シリンダ速度
	CldrThrust			: (3)	各シリンダ推力

	CldrLmtMin			: 各シリンダの最低長さ
	CldrLmtMax			: 各シリンダの最高長さ
	CldrThrustMin		: シリンダ推力の最小
	CldrThrustMax		: シリンダ推力の最大
	FootFrcLmtMin[3]	: 脚先にかかる最低の力
	FootFrcLmtMax[3]	: 脚先にかかる最高の力

	LegID				: 脚のIDナンバ (=1,2,3,4)
******************************************************************************/

	Matrix		Foot;
	Matrix		Jacobian;
	Matrix		InvJacobian;
	Matrix		JacobianL;
	Matrix		InvJacobianL;
	Matrix		FootForce;
	Matrix		CldrLen;
	Matrix		CldrSpd;
	Matrix		CldrThrust;

	double*		CldrLmtMin;
	double*		CldrLmtMax;
	double*		CldrThrustMin;
	double*		CldrThrustMax;
	double		FootFrcLmtMin[3];
	double		FootFrcLmtMax[3];
	int			LegID;

/*************************
**						**
**		メンバ関数		**
**						**
*************************/
public:
/***********************************************************************
** コンストラクタとデストラクタ
***********************************************************************/
/***********************************************************************
	Leg(int, double, double, double, double)	: コンストラクタ
	Leg()										: デフォルトコンストラクタ
	~Leg()										: デストラクタ
***********************************************************************/
	Leg(int legID, double x, double y, double z, double theta);
	Leg();
	~Leg();				

/************************************************************************
**	SetLegID()					: 脚のIDをセット	// 引数: (1,2,3,4)
**	GetLegID()					: 脚のIDを得る	
************************************************************************/
	int			SetLegID(int id);
	int			GetLegID(void){return LegID;}

/*******************************************************************************************************
**	座標変換行列の計算
**		JntID								: 0は脚ベース, 1,2,3まで
**		CoordinateTransformation			: 同次変換行列を計算
**		InvCoordinateTransformation			: 逆同次変換行列を計算
**		SetBaseParameter					: 脚のベースまでの同次変換行列を決定するパラメータをセット
*******************************************************************************************************/
	Matrix		CoordinateTransformation(int jntID) const;
	Matrix		InvCoordinateTransformation(int jntID) const;

/*******************************************************************************************************
**	 順/逆キネマティクスの計算
**		CalcDirectKinematics			: JntAngベクトルにより計算
**		CalcInverseKinematics			: 足裏が常に胴体と平行として足裏の位置から計算
*******************************************************************************************************/
	KINE		CalcDirectKinematics(void);
	KINE		CalcInverseKinematics(void);				// 本体と足裏が常に平行

/*******************************************************************************************
**	関節角を個別に計算 (逆キネマティクスとほぼ一緒)
**		GetJntAngFromEndStat		: 値のみを計算
*******************************************************************************************/
	double		GetJntAngFromEndPos(int jntID);

/******************************************************************************
**	順/逆キネマティクス (ヤコビ行列) の計算をして, 脚の運動計画を行う
**		SetLegEndPos			: 最後のJntStatマトリクスをセット
**		SetJntAng				: JntAngベクトルをセット
**		SetLegConfiguration		: SetLegEndPos後に, CalcDKを解く
**		UpdateJntStatus			: 現在のJntAngベクトルでJntStatを更新
**		SetFootForce			: FootForceベクトルをセット
******************************************************************************/
	KINE		SetEndPos(const Matrix& endPos);						// 本体と足裏が常に平行
	KINE		SetConfiguration(const Matrix& target);					// 本体と足裏が常に平行
	KINE		SetJntAng(const Matrix& jntAng);
	KINE		SetFootForce(const Matrix& force);	
	void		UpdateJntState(void);

/*************************************************************************************
**	脚先が指令どおりの行動を行えるかのチェック
**		CheckCylinderRange		: シリンダ可動範囲チェック
**		CheckFootForce			: 脚先にかかる力のチェック
**		CheckCylinderThrust		: シリンダ推力のチェック
*************************************************************************************/
	KINE		CheckCylinderRange(void);//051006 doi
	KINE		CheckCylinderRange(int jntID);//051006 doi
	KINE		CheckFootForce(void);
	KINE		CheckCylinderThrust(void);

/*****************************************************************************
**	順/逆のヤコビ行列を計算
**		CalcJacobian				: ヤコビ行列の計算(3X3)
**		CalcInverseJacobian			: 逆ヤコビ行列の計算
*****************************************************************************/
	void		CalcJacobian(void);
	void		CalcInverseJacobian(void);

/*****************************************************************************
**	シリンダ推力の計算
**		CalcCylinderThrust		: ヤコビ行列を使い, τ=JT*Fを使う
*****************************************************************************/
	KINE		CalcCylinderThrust(void);

/************************************************************************************
**	シリンダ長さから関節角度を得る
**		GetAngFromLen			: 引数から値のみを計算
**		CalcAngFromLen			: CldrLenベクトルから値を計算してJntAngに代入
**								: 引数なしは全ての関節角度を計算
************************************************************************************/
	double		GetAngFromLen(int jntID, double len);
	KINE		CalcAngFromLen(int jntID);
	KINE		CalcAngFromLen(void);
	
/**************************************************************************************
**	関節角度からシリンダ長さを得る
**		GetLenFromAng			: 引数から値のみを計算
**		CalcLenFromAng			: JntAngベクトルから計算してCldrLenベクトルに代入
**								: 引数なしは全てのシリンダ長さを計算
**************************************************************************************/
	double		GetLenFromAng(int jntID, double ang);
	KINE		CalcLenFromAng(int jntID);
	KINE		CalcLenFromAng(void);

/**************************************************************************************
**	可動範囲を得る
**		GetOuterMotionRange			: 可動範囲の外側を得る
**		GetInnerMotionRange			: 可動範囲の内側を得る
**************************************************************************************/
	double		GetOuterMotionRange(double height);
	double		GetInnerMotionRange(double height);

/*******************************************************************
**	それぞれのメンバ変数を得る
*******************************************************************/
	Matrix		GetTransformMatrix(int jntID) const;
	Matrix		GetFootTransform(void) const		{return Foot;}
	Matrix		GetFootForce(void) const			{return FootForce;}
	Matrix		GetJacobian(void) const				{return Leg::Jacobian;}
	Matrix		GetInvJacobian(void) const			{return Leg::InvJacobian;}
	Matrix		GetCldrLen(void) const				{return CldrLen;}
	Matrix		GetCldrSpd(void) const				{return CldrSpd;}
	Matrix		GetCldrThrust(void)	const			{return CldrThrust;}

private:
/*******************************************************************
**	コピーコンストラクタと代入演算子の禁止
*******************************************************************/
	Leg(const Leg& leg);
	Leg& operator =(const Leg& leg);

/*******************************************************************
**	初期化のヘルパー関数
*******************************************************************/
	void	InitLegParameter(void);

};
#endif // __Leg_h__
