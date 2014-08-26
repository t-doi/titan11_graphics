/********************************************************************************
**  
**	File:	 Titan.h
** 
**	Description: TITAN XIの運動学
**
**	Created: 2003/10/22(Wed)	Updated: 2004/04/12(Mon)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
********************************************************************************/

#ifndef	__Titan_h__
#define __Titan_h__

#include "Robot/Robot.h"

class Titan : public Robot
{
/*************************
**						**
**		メンバ変数		**
**						**
*************************/
public:
protected:
private:
/*******************************************************************
**  TitanXIに固有のデータ
*******************************************************************/
	double	DrillAngle;
	double	DrillCylinderLength;
	
	double	EngineAngle;
	double	EngineCylinderLength;

	double	WireLeftTension;
	double	WireRightTension;

/*******************************************************************
**  TitanXIに固有のデータの限界値
*******************************************************************/
	double	DrillAngMin;
	double	DrillAngMax;
	double	DrillCldrLenMin;
	double	DrillCldrLenMax;

	double	EngineAngMin;
	double	EngineAngMax;
	double	EngineCldrMin;
	double	EngineCldrMax;

	double	WireLeftTensionMin;
	double	WireLeftTensionMax;
	double	WireRightTensionMin;
	double	WireRightTensionMax;

/*************************
**						**
**		メンバ関数		**
**						**
*************************/

public:

/*******************************************************************
**  コンストラクタとデストラクタ
*******************************************************************/
	Titan();
	Titan(const Matrix& body);
	~Titan();

/*******************************************************************
**  初期化
*******************************************************************/
	void		InitTitan(void);

/*******************************************************************
**  グローバル座標系で各種座標を得る
**		GetLegBase				: 脚付け根位置
**		GetHip					: 第一関節位置
**		GetKnee					: 第二関節位置
**		GetAnkle				: 第三関節位置
**		GetFoot					: 脚先位置
**		GetBodyPos				: 胴体位置
**		GetBodyAtt				: 胴体姿勢
*******************************************************************/
	Matrix		GetLegBasePosition(int legID) const;
	Matrix		GetHipPosition(int legID) const;
	Matrix		GetKneePosition(int legID) const;
	Matrix		GetAnklePosition(int legID) const;
	Matrix		GetFootPosition(int legID) const;

	Matrix		GetBodyPosition(void) const;
	Matrix		GetBodyPosture(void) const;

/*******************************************************************
**  TitanXIに固有のデータを得る
*******************************************************************/
	double		GetDrillAngle(void)	 const			{return (DrillAngle);}
	double		GetDrillCylinderLength(void) const	{return (DrillCylinderLength);}
	double		GetEngineAngle(void) const			{return (EngineAngle);}
	double		GetEngineCylinderLength(void) const	{return (EngineCylinderLength);}
	double		GetWireLeftTension(void) const		{return (WireLeftTension);}
	double		GetWireRightTension(void) const		{return (WireRightTension);}

/*******************************************************************
**  TitanXIに固有のデータを設定
*******************************************************************/
//	void		SetDrillAngle(double angle);
//	void		SetEngineAngle(double angle);
	bool		SetDrillAngle(double angle);
	bool		SetEngineAngle(double angle);
	void		SetWireLeftTension(double tension);
	void		SetWireRightTension(double tension);
	KINE		SetBodyTransform(Matrix &c_in, Matrix &n_in, double roll_in);//追尾装置の中心と法線で胴体位置計測．

/*******************************************************************
**  TitanXIに固有のデータを計算
*******************************************************************/
	double		GetDrillAngFromLen(double length);
	void		CalcDrillAngFromLen(void);

	double		GetDrillLenFromAng(double angle);
	void		CalcDrillLenFromAng(void);

	double		GetEngineAngFromLen(double length);
	void		CalcEngineAngFromLen(void);

	double		GetEngineLenFromAng(double angle);
	void		CalcEngineLenFromAng(void);

private:
protected:

};

#endif // __Titan_h__
