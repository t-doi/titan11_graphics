/*****************************************************************************
**
**	File:	RobotData.h
** 
**	説明:	ロボットオブジェクトのデータハンドラー
**
**	Created: 2004/11/18(Thu)	Updated: 2005/01/14(Fri)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

#ifndef	__RobotData_h__
#define __RobotData_h__

#include "../Kinematics/Robot/Robot.h"
#include "../Math/Matrix/Matrix.h"

class RobotData
{
/*************************
**						**
**		メンバ変数		**
**						**
*************************/
public:
protected:
/***********************************************************************
** Titanのそれぞれのデータ
***********************************************************************/
	// 脚に関するデータ
	Matrix			LegTransform[LEG_NUM][JOINT_NUM+2];
	Matrix			FootPosition[LEG_NUM];
	Matrix			AnklePosition[LEG_NUM];
	Matrix			KneePosition[LEG_NUM];
	Matrix			HipPosition[LEG_NUM];
	Matrix			BasePosition[LEG_NUM];
	Matrix			JointAngle[LEG_NUM];
	Matrix			CylinderLength[LEG_NUM];
	Matrix			CylinderThrust[LEG_NUM];
	Robot::LegPhase	LegPhases[LEG_NUM];
	
	// 胴体に関するデータ
	Matrix			BodyPosition;
	Matrix			BodyPosture;
	Matrix			BodyTransform;

private:

/*************************
**						**
**		メンバ関数		**
**						**
*************************/
public:
/***********************************************************************
** コンストラクタとデストラクタ
***********************************************************************/
	RobotData();
	RobotData(const RobotData& data);
    virtual ~RobotData();

/***********************************************************************
**	演算子
***********************************************************************/
	RobotData&	operator =(const RobotData& data);			// 代入
	bool		operator ==(const RobotData& data);			// 等価
	bool		operator !=(const RobotData& data);			// 非等価

/***********************************************************************
**	クラスの複製
***********************************************************************/
	// TitanDataクラスを取得
	virtual RobotData*		Clone(void) const;

/***********************************************************************
**	初期化
***********************************************************************/
	void		ResetRobotData(void);
	

/***********************************************************************
**	データの更新
***********************************************************************/
	// TitanDataクラスのデータを更新
	void		UpdateRobotData(const Robot& robot);
	void		UpdateRobotData(const RobotData& data);
	KINE		SetToRobotDK(Robot* robot_out);//データ書き出し．060731 doi
	KINE		SetToRobotIK(Robot* robot_out);//データ書き出し．060801 doi

/***********************************************************************
**	個別データのアクセッサ
***********************************************************************/
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

/***********************************************************************
**	個別データのセッタ
***********************************************************************/
	void	SetLegTransform(int legID, int joint, const Matrix& transform);
	void	SetFootPosition(int legID, const Matrix& foot);
	void	SetAnklePosition(int legID, const Matrix& ankle);
	void	SetKneePosition(int legID, const Matrix& knee);
	void	SetHipPosition(int legID, const Matrix& hip);
	void	SetBasePosition(int legID, const Matrix& base);
	void	SetJointAngle(int legID, const Matrix& angle);
	void	SetCylinderLength(int legID, const Matrix& length);
	void	SetCylinderThrust(int legID, const Matrix& thrust);
	void	SetLegPhase(int legID, Robot::LegPhase phase);
	
	// 胴体に関するデータ
	void	SetBodyPosition(const Matrix& position);
	void	SetBodyPosture(const Matrix& posture);
	void	SetBodyTransform(const Matrix& transform);

private:
/*******************************************************************
**  代入演算子とコピーコンストラクタから呼ばれる
**	コピーのヘルパー関数
*******************************************************************/
	void		Copy(const RobotData& data);

/*******************************************************************
**  等価演算子から呼ばれる
**	ヘルパー関数
*******************************************************************/
	bool		IsEqual(const RobotData& data);

public: //071010 doi
/***********************************************************************
**	初期化
***********************************************************************/
	void		InitRobotData(void);
};

#endif // __RobotData_h__
