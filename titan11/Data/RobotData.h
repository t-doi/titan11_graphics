/*****************************************************************************
**
**	File:	RobotData.h
** 
**	����:	���{�b�g�I�u�W�F�N�g�̃f�[�^�n���h���[
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
**		�����o�ϐ�		**
**						**
*************************/
public:
protected:
/***********************************************************************
** Titan�̂��ꂼ��̃f�[�^
***********************************************************************/
	// �r�Ɋւ���f�[�^
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
	
	// ���̂Ɋւ���f�[�^
	Matrix			BodyPosition;
	Matrix			BodyPosture;
	Matrix			BodyTransform;

private:

/*************************
**						**
**		�����o�֐�		**
**						**
*************************/
public:
/***********************************************************************
** �R���X�g���N�^�ƃf�X�g���N�^
***********************************************************************/
	RobotData();
	RobotData(const RobotData& data);
    virtual ~RobotData();

/***********************************************************************
**	���Z�q
***********************************************************************/
	RobotData&	operator =(const RobotData& data);			// ���
	bool		operator ==(const RobotData& data);			// ����
	bool		operator !=(const RobotData& data);			// �񓙉�

/***********************************************************************
**	�N���X�̕���
***********************************************************************/
	// TitanData�N���X���擾
	virtual RobotData*		Clone(void) const;

/***********************************************************************
**	������
***********************************************************************/
	void		ResetRobotData(void);
	

/***********************************************************************
**	�f�[�^�̍X�V
***********************************************************************/
	// TitanData�N���X�̃f�[�^���X�V
	void		UpdateRobotData(const Robot& robot);
	void		UpdateRobotData(const RobotData& data);
	KINE		SetToRobotDK(Robot* robot_out);//�f�[�^�����o���D060731 doi
	KINE		SetToRobotIK(Robot* robot_out);//�f�[�^�����o���D060801 doi

/***********************************************************************
**	�ʃf�[�^�̃A�N�Z�b�T
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

	// ���̂Ɋւ���f�[�^
	Matrix			GetBodyPosition(void) const;
	Matrix			GetBodyPosture(void) const;
	Matrix			GetBodyTransform(void) const;

/***********************************************************************
**	�ʃf�[�^�̃Z�b�^
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
	
	// ���̂Ɋւ���f�[�^
	void	SetBodyPosition(const Matrix& position);
	void	SetBodyPosture(const Matrix& posture);
	void	SetBodyTransform(const Matrix& transform);

private:
/*******************************************************************
**  ������Z�q�ƃR�s�[�R���X�g���N�^����Ă΂��
**	�R�s�[�̃w���p�[�֐�
*******************************************************************/
	void		Copy(const RobotData& data);

/*******************************************************************
**  �������Z�q����Ă΂��
**	�w���p�[�֐�
*******************************************************************/
	bool		IsEqual(const RobotData& data);

public: //071010 doi
/***********************************************************************
**	������
***********************************************************************/
	void		InitRobotData(void);
};

#endif // __RobotData_h__
