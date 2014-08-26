/*****************************************************************************
**
**	File:	Arm.h
** 
**	説明:	TITAN XIの単腕の運動学
**
**	Created: 2004/10/28(Thu)	Updated: 2005/01/14(Fri)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

/***********************************************************************
**  このクラスは主に単腕用のキネマティクスを表現
***********************************************************************/

#ifndef	__Arm_h__
#define __Arm_h__

#include "../../../Math/MathLibrary.h"
#include "../../../Utility/Type.h"
#include "../../KineParameter.h"

class Arm
{
/*************************
**						**
**		メンバ変数		**
**						**
*************************/
public:
/*******************************************************************
**	Armクラスに関係する定数
*******************************************************************/
private:
protected:
/***********************************************************************
**  座標系は全てグローバル (ロボットに搭載したら機体座標)
***********************************************************************/
/******************************************************************************
	Transformation		: (4,4) グローバル座標からの各フレームの同次変換行列
	EndForce			: (3)	脚先での力のベクトル
	JntAng				: (3)	各関節角度のベクトル
	JntAngVel			: (3)	各関節角速度のベクトル
	JacobianA			: (3,3)	関節角と先端変位のヤコビ行列
	InvJacobianA		: (3,3)	関節角と先端変位の逆ヤコビ行列
	BaseParameter		: (4) 脚のベースパラメータ
						: (1)x, (2)y, (3)z, (4)angle

	AngLmtMin			: 各関節の可動限界(最低)
	AngLmtMax			: 各関節の可動限界(最高)
	EndFrcLmtMin[3]		: 脚先にかかる最低の力
	EndFrcLmtMax[3]		: 脚先にかかる最高の力
	ReachLmtMin[2]		: 脚先が到達する最低距離
	ReachLmtMax[2]		: 脚先が到達する最高距離

	Link				: 各リンク長
	Indicator[2]		: 脚の姿勢指標
						: Indicator[0] = HIP, Indicator[1] = KNEE
******************************************************************************/

	Matrix*		Transformation;
	Matrix		EndForce;
	Matrix		JntAng;
	Matrix		JntAngVel;
	Matrix		JntTorque;
	Matrix		JacobianA;
	Matrix		InvJacobianA;
	Matrix		BaseParameter;

	double*		AngLmtMin;
	double*		AngLmtMax;
	double		EndFrcLmtMin[3];
	double		EndFrcLmtMax[3];
	double		ReachLmtMin[2];
	double		ReachLmtMax[2];
	double*		Link;
	int			Indicator[2];

/*******************************************************************
**  最後にエラーの原因となった関節番号(1, 2, 3)
**	0は全てOKで, エラーなし
*******************************************************************/
	int			LastJntErrNum;

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
	Arm()										: デフォルトコンストラクタ
	Arm(double, double, double, double)			: コンストラクタ
	~Arm()										: デストラクタ
***********************************************************************/
				Arm();
				Arm(double x, double y, double z, double theta);
	virtual		~Arm();				

/*******************************************************************************************************
**	座標変換行列の計算
**		JntID								: 0は脚ベース, 1,2,3まで
**		CoordinateTransformation			: 同次変換行列を計算
**		InvCoordinateTransformation			: 逆同次変換行列を計算
**		SetBaseParameter					: 脚のベースまでの同次変換行列を決定するパラメータをセット
*******************************************************************************************************/
	virtual Matrix	CoordinateTransformation(int jntID) const;
	virtual Matrix	InvCoordinateTransformation(int jntID) const;
	void			SetBaseParameter(double x, double y, double z, double theta);

/************************************************************************
**	SetOriIndicator()			: 姿勢指標をセット	// 引数: (1 or -1)
************************************************************************/
	bool			SetOriIndicator(int front=1, int up=1);

/*******************************************************************************************************
**	 順/逆キネマティクスの計算
**		CalcDirectKinematics			: JntAngベクトルにより計算
**		CalcInverseKinematics			: 最後のJntStatマトリクスにより計算(逆キネを解析的にとく為)
*******************************************************************************************************/
	virtual KINE	CalcDirectKinematics(void);
	virtual KINE	CalcInverseKinematics(void);

/*******************************************************************************************
**	関節角を個別に計算 (逆キネマティクスとほぼ一緒)
**		GetJntAngFromEndPos		: 値のみを計算
*******************************************************************************************/
	virtual double	GetJntAngFromEndPos(int jntID);

/******************************************************************************
**	順/逆キネマティクス (ヤコビ行列) の計算をして, 脚の運動計画を行う
**		SetEndPos				: 脚先の位置をセット
**		SetConfiguration		: SetEndPos後に, CalcDKを解く
**		SetJntAng				: JntAngベクトルをセット
**		SetEndForce				: EndForceベクトルをセット
**		UpdateTransformation	: 現在のJntAngベクトルで同次変換行列を更新
******************************************************************************/
	virtual KINE	SetEndPos(const Matrix& endPos);
	virtual KINE	SetConfiguration(const Matrix& target);
	virtual KINE	SetJntAng(const Matrix& jntAng);
	virtual KINE	SetEndForce(const Matrix& force);
	virtual void	UpdateJntState(void);

/*************************************************************************************
**	脚先が指令どおりの行動を行えるかのチェック
**		CheckReachStatus		: 逆キネマティクスにより到達範囲チェック
**		CheckJointRange			: 関節可動範囲チェック
**		CheckEndForce			: 脚先にかかる力のチェック
*************************************************************************************/
	KINE			CheckReachRange(void);
	KINE			CheckReachRange(Matrix &Trans_in);//060720 doi
	KINE			CheckJointRange(void);
	KINE			CheckJointRange(int jntID);  
	KINE			CheckEndForce(void);

/*****************************************************************************
**	順/逆のヤコビ行列を計算
**		CalcJacobian				: ヤコビ行列の計算(3X3)
**		CalcInverseJacobian			: 逆ヤコビ行列の計算
*****************************************************************************/
	virtual void	CalcJacobian(void);
	virtual void	CalcInverseJacobian(void);

/*****************************************************************************
**	関節トルクの計算
**		CalcJntTorque		: ヤコビ行列を使い, τ=JT*Fを使う
*****************************************************************************/
	KINE			CalcJntTorque(void);

/*******************************************************************
**	それぞれのメンバ変数を得る
*******************************************************************/
	virtual Matrix	GetTransformMatrix(int jntID) const;
	Matrix			GetBaseParameter(void) const	{return BaseParameter;}
	Matrix			GetEndForce(void) const			{return EndForce;}
	Matrix			GetJacobianA(void) const		{return JacobianA;}
	Matrix			GetInvJacobianA(void) const		{return InvJacobianA;}
	Matrix			GetJntAng(void)	const			{return JntAng;}
	Matrix			GetJntAngVel(void) const		{return JntAngVel;}
	int				GetLastJntErr(void) const		{return LastJntErrNum;}

private:
/*******************************************************************
**	コピーコンストラクタと代入演算子の禁止
*******************************************************************/
		Arm(const Arm& arm);
		Arm& operator =(const Arm& arm);

/*******************************************************************
**	初期化のヘルパー関数
*******************************************************************/
	void		InitArmParameter(void);

};
#endif // __Arm_h__
