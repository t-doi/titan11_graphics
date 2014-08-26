/*****************************************************************************
**
**	File:	TitanData.h
** 
**	����:	TITAN XI�̃f�[�^�n���h���[
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
**		�����o�ϐ�		**
**						**
*************************/
public:
private:
/***********************************************************************
** �f�[�^�N���X
***********************************************************************/
	RobotData		RobotD;
	//GaitData		Gait;

/***********************************************************************
** Titan�̃f�[�^
***********************************************************************/
	double		DrillAngle;
	double		DrillCylinderLength;
	double		EngineAngle;
	double		EngineCylinderLength;
	double		WireLeftTension;
	double		WireRightTension;
	int			CalculatingPosition;
/***********************************************************************
** �������䃂�[�h 070318
***********************************************************************/
       char             PressureControlMode[4];//�e�r�̖������䃂�[�h

/*************************
**						**
**		�����o�֐�		**
**						**
*************************/
public:
/***********************************************************************
** �R���X�g���N�^�ƃf�X�g���N�^
***********************************************************************/
	TitanData();
	TitanData(const TitanData& data);
    virtual ~TitanData();

/***********************************************************************
**	���Z�q
***********************************************************************/
	TitanData&	operator =(const TitanData& data);			// ���
	bool		operator ==(const TitanData& data);			// ����
	bool		operator !=(const TitanData& data);			// �񓙉�

/***********************************************************************
**	�N���X�̕���
***********************************************************************/
	TitanData*		Clone(void) const;
	
/***********************************************************************
**	�f�[�^�̍X�V
***********************************************************************/
	// TitanData�N���X�̃f�[�^���X�V
//	void		UpdateTitanData(const Titan& titan, const GaitGenerator& gait);
	void		UpdateTitanData(const Titan& titan);
	void		UpdateTitanData(const TitanData& data);
	void		UpdateRobotData(const Robot& robot);
	void		UpdateRobotData(const RobotData& data);

/***********************************************************************
**	�ʃf�[�^�̃A�N�Z�b�T
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
**	�ʃf�[�^�̃Z�b�^
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
**	�����o�N���X�̃A�N�Z�b�T
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

	// ���̂Ɋւ���f�[�^
	Matrix			GetBodyPosition(void) const;
	Matrix			GetBodyPosture(void) const;
	Matrix			GetBodyTransform(void) const;

	KINE		SetToRobotDK(Robot* robot_out);//�f�[�^�����o���D060731 doi
	KINE		SetToRobotIK(Robot* robot_out);//�f�[�^�����o���D060801 doi

// GaitData
	double			GetStabilityMargin(void) const;
	double			GetDutyFactor(void) const;
	double			GetCycle(void) const;
	double			GetElapsedTime(void) const;
	int				GetCalculatingPosition(void) const;

	Matrix			MoveDirection;//060525 doi
  
/***********************************************************************
**	�����o�N���X�̃Z�b�^
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
	
	// ���̂Ɋւ���f�[�^
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
**  ������Z�q�ƃR�s�[�R���X�g���N�^����Ă΂��
**	�R�s�[�̃w���p�[�֐�
*******************************************************************/
	void		Copy(const TitanData& data);

/*******************************************************************
**  �������Z�q����Ă΂��
**	�w���p�[�֐�
*******************************************************************/
	bool		IsEqual(const TitanData& data);

public: //071010 doi
/***********************************************************************
**	������
***********************************************************************/
	void		InitTitanData(void);

};

#endif	// __TitanData_h__
