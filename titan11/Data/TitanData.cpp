/*****************************************************************************
**
**	File:	TitanData.cpp
** 
**	����:	TITAN XI�̃f�[�^�n���h���[
**
**	Created: 2004/10/28(Thu)	Updated: 2005/01/14(Fri)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

#include "TitanData.h"
#include "../Kinematics/Titan.h"
//#include "../Plan/Generator/GaitGenerator.h"

/***********************************************************************
** �f�t�H���g�R���X�g���N�^
***********************************************************************/
TitanData::TitanData()
{
	InitTitanData();
}

/***********************************************************************
** �R�s�[�R���X�g���N�^
***********************************************************************/
TitanData::TitanData(const TitanData& data)
{
	Copy(data);
}

/***********************************************************************
** �f�X�g���N�^
***********************************************************************/
TitanData::~TitanData()
{
}

/***********************************************************************
** ������Z�q
***********************************************************************/
TitanData& TitanData::operator =(const TitanData& data)
{
	if ( &data != this)
	{
		Copy(data);
	}

	return (*this);

}

/***********************************************************************
** �������Z�q
***********************************************************************/
bool TitanData::operator ==(const TitanData& data)
{
	return ( IsEqual(data)  );
}

/***********************************************************************
** �񓙉����Z�q
***********************************************************************/
bool TitanData::operator !=(const TitanData& data)
{
	return !( IsEqual(data) );
}

/***********************************************************************
** ������
***********************************************************************/
void TitanData::InitTitanData(void)
{
	// Titan�̑��u�Ɋւ���f�[�^
	DrillAngle				= 0.0;
	DrillCylinderLength		= 0.0;
	EngineAngle				= 0.0;
	EngineCylinderLength	= 0.0;
	WireLeftTension			= 0.0;
	WireRightTension		= 0.0;
	CalculatingPosition		= 0;

	MoveDirection.SetSize(3);//060525 doi

  RobotD.InitRobotData();

        for(int i=0;i<4;i++)
          {
            PressureControlMode[i]=PCM_NONE;
          }

	return;
}

/***********************************************************************
** �N���X�̕������쐬
***********************************************************************/
TitanData* TitanData::Clone(void) const
{
	return ( new TitanData(*this) );
}

/***********************************************************************
** �f�[�^���X�V
***********************************************************************/
//void TitanData::UpdateTitanData(const Titan& titan, const GaitGenerator& gait)
void TitanData::UpdateTitanData(const Titan& titan)
{
	// RobotData�̍X�V
	RobotD.UpdateRobotData(titan);
	//Gait.UpdateGaitData(gait);

	// Titan�̑��u�f�[�^�̍X�V
	DrillAngle			= titan.GetDrillAngle();
	DrillCylinderLength	= titan.GetDrillCylinderLength();
	EngineAngle			= titan.GetEngineAngle();
	EngineCylinderLength= titan.GetDrillCylinderLength();
	WireLeftTension		= titan.GetWireLeftTension();
	WireRightTension	= titan.GetWireRightTension();
	
  
	// Gait�f�[�^�̍X�V

	return;
}

void TitanData::UpdateTitanData(const TitanData& data)
{
	Copy(data);

	return;
}

void TitanData::UpdateRobotData(const Robot& robot)
{
    RobotD.UpdateRobotData(robot);
}

void TitanData::UpdateRobotData(const RobotData& data)
{
    RobotD.UpdateRobotData(data);
}
/***********************************************************************
**	�ʃf�[�^�̃Z�b�^
***********************************************************************/
void TitanData::SetRobotData(const RobotData& data)		{RobotD = data; return;}
//void TitanData::SetGaitData(const GaitData& data)		{Gait = data; return;}
void TitanData::SetDrillAngle(double angle)				{DrillAngle = angle; return;}
void TitanData::SetDrillCylinderLength(double length)	{DrillCylinderLength = length; return;}
void TitanData::SetEngineAngle(double angle)			{EngineAngle = angle; return;}
void TitanData::SetEngineCylinderLength(double length)	{EngineCylinderLength = length; return;}
void TitanData::SetWireLeftTension(double tension)		{WireLeftTension = tension; return;}
void TitanData::SetWireRightTension(double tension)		{WireRightTension = tension; return;}
void TitanData::SetPressureControlMode(int LegID, char PCMode_in){PressureControlMode[LegID-1]=PCMode_in;return;}//070318 doi

/***********************************************************************
**	�����o�N���X�̃A�N�Z�b�T
***********************************************************************/
// RobotData
Matrix TitanData::GetLegTransform(int legID, int joint) const	{ return (RobotD.GetLegTransform(legID, joint)); }
Matrix TitanData::GetFootPosition(int legID) const				{ return (RobotD.GetFootPosition(legID)); }
Matrix TitanData::GetAnklePosition(int legID) const				{ return (RobotD.GetAnklePosition(legID)); }
Matrix TitanData::GetKneePosition(int legID) const				{ return (RobotD.GetKneePosition(legID)); }
Matrix TitanData::GetHipPosition(int legID) const				{ return (RobotD.GetHipPosition(legID)); }
Matrix TitanData::GetBasePosition(int legID) const				{ return (RobotD.GetBasePosition(legID)); }
Matrix TitanData::GetJointAngle(int legID) const				{ return (RobotD.GetJointAngle(legID)); }
Matrix TitanData::GetCylinderLength(int legID) const			{ return (RobotD.GetCylinderLength(legID)); }
Matrix TitanData::GetCylinderThrust(int legID) const			{ return (RobotD.GetCylinderThrust(legID)); }
Robot::LegPhase	TitanData::GetLegPhase(int legID) const			{ return (RobotD.GetLegPhase(legID)); }

// ���̂Ɋւ���f�[�^
Matrix TitanData::GetBodyPosition(void) const					{ return (RobotD.GetBodyPosition()); }
Matrix TitanData::GetBodyPosture(void) const					{ return (RobotD.GetBodyPosture()); }
Matrix TitanData::GetBodyTransform(void) const					{ return (RobotD.GetBodyTransform()); }

// GaitData
/*
double TitanData::GetStabilityMargin(void) const				{ return (Gait.GetStabilityMargin()); }
double TitanData::GetDutyFactor(void) const						{ return (Gait.GetDutyFactor()); }
double TitanData::GetCycle(void) const							{ return (Gait.GetCycle()); }
double TitanData::GetElapsedTime(void) const					{ return (Gait.GetElapsedTime()); }
*/

KINE   TitanData::SetToRobotDK(Robot* robot_out)                   { return (RobotD.SetToRobotDK(robot_out));}//�f�[�^�����o���D060731 doi
KINE   TitanData::SetToRobotIK(Robot* robot_out)                   { return (RobotD.SetToRobotIK(robot_out));}//�f�[�^�����o���D060801 doi


/***********************************************************************
**	�����o�N���X�̃Z�b�^
***********************************************************************/
// RobotData
void TitanData::SetLegTransform(int legID, int joint, const Matrix& transform)	{RobotD.SetLegTransform(legID, joint, transform); return;}
void TitanData::SetFootPosition(int legID, const Matrix& foot)					{RobotD.SetFootPosition(legID, foot); return;}
void TitanData::SetAnklePosition(int legID, const Matrix& ankle)				{RobotD.SetAnklePosition(legID, ankle); return;}
void TitanData::SetKneePosition(int legID, const Matrix& knee)					{RobotD.SetKneePosition(legID, knee); return;}
void TitanData::SetHipPosition(int legID, const Matrix& hip)					{RobotD.SetHipPosition(legID, hip); return;}
void TitanData::SetBasePosition(int legID, const Matrix& base)					{RobotD.SetBasePosition(legID, base); return;}
void TitanData::SetJointAngle(int legID, const Matrix& angle)					{RobotD.SetJointAngle(legID, angle); return;}
void TitanData::SetCylinderLength(int legID, const Matrix& length)				{RobotD.SetCylinderLength(legID, length); return;}
void TitanData::SetCylinderThrust(int legID, const Matrix& thrust)				{RobotD.SetCylinderThrust(legID, thrust); return;}
void TitanData::SetLegPhase(int legID, Robot::LegPhase phase)					{RobotD.SetLegPhase(legID, phase); return;}
	
// ���̂Ɋւ���f�[�^
void TitanData::SetBodyPosition(const Matrix& position)							{RobotD.SetBodyPosition(position); return;}
void TitanData::SetBodyPosture(const Matrix& posture)							{RobotD.SetBodyPosture(posture); return;}
void TitanData::SetBodyTransform(const Matrix& transform)						{RobotD.SetBodyTransform(transform); return;}

// GaitData
/*
void TitanData::SetStabilityMargin(double margin)								{Gait.SetStabilityMargin(margin); return;}
void TitanData::SetDutyFactor(double duty)										{Gait.SetDutyFactor(duty); return;}
void TitanData::SetCycle(double cycle)											{Gait.SetCycle(cycle); return;}
void TitanData::SetElapsedTime(double time)										{Gait.SetElapsedTime(time); return;}
*/

/*******************************************************************
**  ������Z�q�ƃR�s�[�R���X�g���N�^����Ă΂��
**	�R�s�[�̃w���p�[�֐�
*******************************************************************/
void TitanData::Copy(const TitanData& data)
{
	// RobotData�N���X�̃R�s�[
	RobotD	= data.GetRobotData();
	//Gait	= data.GetGaitData();

	DrillAngle				= data.GetDrillAngle();
	DrillCylinderLength		= data.GetDrillCylinderLength();
	EngineAngle				= data.GetEngineAngle();
	EngineCylinderLength	= data.GetEngineCylinderLength();
	WireLeftTension			= data.GetWireLeftTension();
	WireRightTension		= data.GetWireRightTension();

	MoveDirection.SetSize(3);
	MoveDirection		= data.MoveDirection;//060525 doi

        for(int i=0;i<4;i++)
          {
            PressureControlMode[i]  = data.PressureControlMode[i];
          }
  
	return;
}

/*******************************************************************
**  �������Z�q�Ɣ񓙉����Z�q����Ă΂��
**	�w���p�[�֐�
*******************************************************************/
bool TitanData::IsEqual(const TitanData& data)
{
	//if ( ( RobotD != data.GetRobotData() ) || ( Gait != data.GetGaitData() ) )
	if ( RobotD != data.GetRobotData() )
	{
		return false;
	}

	if ( DrillAngle				!= data.GetDrillAngle() )			{return (false);}
	if ( DrillCylinderLength	!= data.GetDrillCylinderLength() )	{return (false);}
	if ( EngineAngle			!= data.GetEngineAngle() )			{return (false);}
	if ( EngineCylinderLength	!= data.GetEngineCylinderLength() )	{return (false);}
	if ( WireLeftTension		!= data.GetWireLeftTension() )		{return (false);}
	if ( WireRightTension		!= data.GetWireRightTension() )		{return (false);}

	return (true);
}
