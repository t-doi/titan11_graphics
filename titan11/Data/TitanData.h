/*****************************************************************************
**
**	File:	TitanData.h
** 
**	説明:	TITAN XIのデータハンドラー
**
**	Created: 2004/10/28(Thu)	Updated: 2005/01/14(Fri)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

#ifndef	__TitanData_h__
#define __TitanData_h__

#include "RobotData.h"
//#include "GaitData.h"

class Titan;
//class GaitGenerator;

class TitanData
{
/*************************
**						**
**		メンバ変数		**
**						**
*************************/
public:
private:
/***********************************************************************
** データクラス
***********************************************************************/
	RobotData		RobotD;
	//GaitData		Gait;

/***********************************************************************
** Titanのデータ
***********************************************************************/
	double		DrillAngle;
	double		DrillCylinderLength;
	double		EngineAngle;
	double		EngineCylinderLength;
	double		WireLeftTension;
	double		WireRightTension;
	int			CalculatingPosition;
/***********************************************************************
** 油圧制御モード 070318
***********************************************************************/
       char             PressureControlMode[4];//各脚の油圧制御モード

/*************************
**						**
**		メンバ関数		**
**						**
*************************/
public:
/***********************************************************************
** コンストラクタとデストラクタ
***********************************************************************/
	TitanData();
	TitanData(const TitanData& data);
    virtual ~TitanData();

/***********************************************************************
**	演算子
***********************************************************************/
	TitanData&	operator =(const TitanData& data);			// 代入
	bool		operator ==(const TitanData& data);			// 等価
	bool		operator !=(const TitanData& data);			// 非等価

/***********************************************************************
**	クラスの複製
***********************************************************************/
	TitanData*		Clone(void) const;
	
/***********************************************************************
**	データの更新
***********************************************************************/
	// TitanDataクラスのデータを更新
//	void		UpdateTitanData(const Titan& titan, const GaitGenerator& gait);
	void		UpdateTitanData(const Titan& titan);
	void		UpdateTitanData(const TitanData& data);
	void		UpdateRobotData(const Robot& robot);
	void		UpdateRobotData(const RobotData& data);

/***********************************************************************
**	個別データのアクセッサ
***********************************************************************/
	RobotData	GetRobotData(void) const			{return RobotD;}
//	GaitData	GetGaitData(void) const				{return Gait;}
	double		GetDrillAngle(void) const			{return DrillAngle;}
	double		GetDrillCylinderLength(void) const	{return DrillCylinderLength;}
	double		GetEngineAngle(void) const			{return EngineAngle;}
	double		GetEngineCylinderLength(void) const	{return EngineCylinderLength;}
	double		GetWireLeftTension(void) const		{return WireLeftTension;}
	double		GetWireRightTension(void) const		{return WireRightTension;}
        char            GetPressureControlMode(int LegID) {return PressureControlMode[LegID-1];}//070318doi
  
/***********************************************************************
**	個別データのセッタ
***********************************************************************/
	void		SetRobotData(const RobotData& data);
//        void		SetGaitData(const GaitData& data);
	void		SetDrillAngle(double angle);
	void		SetDrillCylinderLength(double length);
	void		SetEngineAngle(double angle);
	void		SetEngineCylinderLength(double length);
	void		SetWireLeftTension(double tension);
	void		SetWireRightTension(double tension);
        void            SetPressureControlMode(int LegID, char PCMode_in);//070318
  
/***********************************************************************
**	メンバクラスのアクセッサ
***********************************************************************/
// RobotData
	Matrix			GetLegTransform(int legID, int joint) const;
	Matrix			GetFootPosition(int legID) const;
	Matrix			GetAnklePosition(int legID) const;
	Matrix			GetKneePosition(int legID) const;
	Matrix			GetHipPosition(int legID) const;
	Matrix			GetBasePosition(int legID) const;
	Matrix			GetJointAngle(int legID) const;
	Matrix			GetCylinderLength(int legID) const;
	Matrix			GetCylinderThrust(int legID) const;
	Robot::LegPhase	GetLegPhase(int legID) const;

	// 胴体に関するデータ
	Matrix			GetBodyPosition(void) const;
	Matrix			GetBodyPosture(void) const;
	Matrix			GetBodyTransform(void) const;

	KINE		SetToRobotDK(Robot* robot_out);//データ書き出し．060731 doi
	KINE		SetToRobotIK(Robot* robot_out);//データ書き出し．060801 doi

// GaitData
	double			GetStabilityMargin(void) const;
	double			GetDutyFactor(void) const;
	double			GetCycle(void) const;
	double			GetElapsedTime(void) const;
	int				GetCalculatingPosition(void) const;

	Matrix			MoveDirection;//060525 doi
  
/***********************************************************************
**	メンバクラスのセッタ
***********************************************************************/
// RobotData
	void			SetLegTransform(int legID, int joint, const Matrix& transform);
	void			SetFootPosition(int legID, const Matrix& foot);
	void			SetAnklePosition(int legID, const Matrix& ankle);
	void			SetKneePosition(int legID, const Matrix& knee);
	void			SetHipPosition(int legID, const Matrix& hip);
	void			SetBasePosition(int legID, const Matrix& base);
	void			SetJointAngle(int legID, const Matrix& angle);
	void			SetCylinderLength(int legID, const Matrix& length);
	void			SetCylinderThrust(int legID, const Matrix& thrust);
	void			SetLegPhase(int legID, Robot::LegPhase phase);
	
	// 胴体に関するデータ
	void			SetBodyPosition(const Matrix& position);
	void			SetBodyPosture(const Matrix& posture);
	void			SetBodyTransform(const Matrix& transform);

// GaitData
/*	void			SetStabilityMargin(double margin);
	void			SetDutyFactor(double duty);
	void			SetCycle(double cycle);
	void			SetElapsedTime(double time);
	void			SetCalculatingPosition(int position);
*/
private:
/*******************************************************************
**  代入演算子とコピーコンストラクタから呼ばれる
**	コピーのヘルパー関数
*******************************************************************/
	void		Copy(const TitanData& data);

/*******************************************************************
**  等価演算子から呼ばれる
**	ヘルパー関数
*******************************************************************/
	bool		IsEqual(const TitanData& data);

public: //071010 doi
/***********************************************************************
**	初期化
***********************************************************************/
	void		InitTitanData(void);

};

#endif	// __TitanData_h__
