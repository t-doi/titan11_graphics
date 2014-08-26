/********************************************************************************
**  
**	File:	 Titan.h
** 
**	Description: TITAN XI�̉^���w
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
**		�����o�ϐ�		**
**						**
*************************/
public:
protected:
private:
/*******************************************************************
**  TitanXI�ɌŗL�̃f�[�^
*******************************************************************/
	double	DrillAngle;
	double	DrillCylinderLength;
	
	double	EngineAngle;
	double	EngineCylinderLength;

	double	WireLeftTension;
	double	WireRightTension;

/*******************************************************************
**  TitanXI�ɌŗL�̃f�[�^�̌��E�l
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
**		�����o�֐�		**
**						**
*************************/

public:

/*******************************************************************
**  �R���X�g���N�^�ƃf�X�g���N�^
*******************************************************************/
	Titan();
	Titan(const Matrix& body);
	~Titan();

/*******************************************************************
**  ������
*******************************************************************/
	void		InitTitan(void);

/*******************************************************************
**  �O���[�o�����W�n�Ŋe����W�𓾂�
**		GetLegBase				: �r�t�����ʒu
**		GetHip					: ���֐߈ʒu
**		GetKnee					: ���֐߈ʒu
**		GetAnkle				: ��O�֐߈ʒu
**		GetFoot					: �r��ʒu
**		GetBodyPos				: ���̈ʒu
**		GetBodyAtt				: ���̎p��
*******************************************************************/
	Matrix		GetLegBasePosition(int legID) const;
	Matrix		GetHipPosition(int legID) const;
	Matrix		GetKneePosition(int legID) const;
	Matrix		GetAnklePosition(int legID) const;
	Matrix		GetFootPosition(int legID) const;

	Matrix		GetBodyPosition(void) const;
	Matrix		GetBodyPosture(void) const;

/*******************************************************************
**  TitanXI�ɌŗL�̃f�[�^�𓾂�
*******************************************************************/
	double		GetDrillAngle(void)	 const			{return (DrillAngle);}
	double		GetDrillCylinderLength(void) const	{return (DrillCylinderLength);}
	double		GetEngineAngle(void) const			{return (EngineAngle);}
	double		GetEngineCylinderLength(void) const	{return (EngineCylinderLength);}
	double		GetWireLeftTension(void) const		{return (WireLeftTension);}
	double		GetWireRightTension(void) const		{return (WireRightTension);}

/*******************************************************************
**  TitanXI�ɌŗL�̃f�[�^��ݒ�
*******************************************************************/
//	void		SetDrillAngle(double angle);
//	void		SetEngineAngle(double angle);
	bool		SetDrillAngle(double angle);
	bool		SetEngineAngle(double angle);
	void		SetWireLeftTension(double tension);
	void		SetWireRightTension(double tension);
	KINE		SetBodyTransform(Matrix &c_in, Matrix &n_in, double roll_in);//�ǔ����u�̒��S�Ɩ@���œ��̈ʒu�v���D

/*******************************************************************
**  TitanXI�ɌŗL�̃f�[�^���v�Z
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
