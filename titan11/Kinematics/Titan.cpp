/********************************************************************************
**  
**	File:	 Titan.cpp
** 
**	Description: TITAN XI�̉^���w
**
**	Created: 2003/10/22(Wed)	Updated: 2004/04/12(Mon)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
********************************************************************************/

#include "Titan.h"

/*******************************************************************
**  �f�t�H���g�R���X�g���N�^
*******************************************************************/
Titan::Titan()
{
	InitTitan();
}

/*******************************************************************
**  �R���X�g���N�^
*******************************************************************/
Titan::Titan(const Matrix& body) : Robot(body)
{
	InitTitan();
}

/*******************************************************************
**  ������
*******************************************************************/
void Titan::InitTitan(void)
{
	EngineAngle=0;
	return;
}

/*******************************************************************
**  �f�X�g���N�^
*******************************************************************/
Titan::~Titan()
{

}

/*******************************************************************
**  �r�̕t�����ʒu�𓾂�
*******************************************************************/
Matrix Titan::GetLegBasePosition(int legID) const
{
	Matrix pos(3);

	pos.LoadMatrix( GetLegTransformMatrixInGlobal(legID, 0), 1, 4, 3, 4 );


	return (pos);
}

/*******************************************************************
**  ���֐߈ʒu�𓾂�
*******************************************************************/
Matrix Titan::GetHipPosition(int legID) const
{
	Matrix pos(3);

	pos.LoadMatrix( GetLegTransformMatrixInGlobal(legID, 1), 1, 4, 3, 4 );

	return (pos);
}

/*******************************************************************
**  ���֐߈ʒu�𓾂�
*******************************************************************/
Matrix Titan::GetKneePosition(int legID) const
{
	Matrix pos(3);

	pos.LoadMatrix( GetLegTransformMatrixInGlobal(legID, 2), 1, 4, 3, 4 );

	return (pos);
}

/*******************************************************************
**  ��O�֐߈ʒu�𓾂�
*******************************************************************/
Matrix Titan::GetAnklePosition(int legID) const
{
	Matrix pos(3);

	pos.LoadMatrix( GetLegTransformMatrixInGlobal(legID, 3), 1, 4, 3, 4 );
	
	return (pos);
}

/*******************************************************************
**  ��l�֐߈ʒu�𓾂�
*******************************************************************/
Matrix Titan::GetFootPosition(int legID) const
{
	Matrix pos(3);

	pos.LoadMatrix( GetLegTransformMatrixInGlobal(legID, 4), 1, 4, 3, 4 );
	
	return (pos);
}

/*******************************************************************
**  ���̈ʒu�𓾂�
*******************************************************************/
Matrix Titan::GetBodyPosition(void) const
{
	Matrix pos(3);

	pos.LoadMatrix( Body, 1, 4, 3, 4 );
	return (pos);

}

/*******************************************************************
**  ���̎p���𓾂�
*******************************************************************/
Matrix Titan::GetBodyPosture(void) const
{
	Matrix att(3);
	Matrix rot(3, 3);

	rot.LoadMatrix(Body, 1, 1, 3, 3);

	att = RotMatrixtoRPY(rot);
	return (att);
}

/*******************************************************************
**  �h�����̎p���p��ݒ�
*******************************************************************/
bool Titan::SetDrillAngle(double angle)
{
	if(angle < ANGLE_DRILL_MIN)return false;
	if(angle > ANGLE_DRILL_MAX)return false;
	DrillAngle = angle;//060728 doi
	return true;
}

/*******************************************************************
**  �G���W�����t�g�̎p���p��ݒ�
*******************************************************************/
bool Titan::SetEngineAngle(double angle)
{
	if(angle < ANGLE_LIFT_MIN)return false;
	if(angle > ANGLE_LIFT_MAX)return false;
	EngineAngle = angle;//060728 doi
	return true;
}

/*******************************************************************
**  �����̃��C�����͂�ݒ�
*******************************************************************/
void Titan::SetWireLeftTension(double tension)
{
}

/*******************************************************************
**  �E���̃��C�����͂�ݒ�
*******************************************************************/
void Titan::SetWireRightTension(double tension)
{
}

/*******************************************************************
**  �ǔ����u�̒��S�Ɩ@���œ��̈ʒu�v���D
**  ���t�g�p�x�C���[�������g�p�@ 060817 doi
*******************************************************************/
KINE Titan::SetBodyTransform(Matrix &c_in, Matrix &n_in, double roll_in)
{
	//�ǔ����u�̒��S�Ɩ@���œ��̈ʒu�v���D���t�g�p�x���g�p
	//060525�����Q��
	Matrix Tb(DH_NUM,DH_NUM);//���̂���O���[�o��
	Matrix Tl(DH_NUM,DH_NUM);//���t�g����O���[�o����
	Matrix lTb(DH_NUM,DH_NUM);//���̂��烊�t�g��

	Tl.LoadIdentity();
	Tl(1,4)=c_in(1);
	Tl(2,4)=c_in(2);
	Tl(3,4)=c_in(3);//��(15)
	Matrix xl(3),yl(3),zl(3);
	Matrix z2(3),y2(3);
	Matrix z(3);
	z(1)=0;z(2)=0;z(3)=1;
	xl=-n_in;
	z2=OuterProduct(xl,OuterProduct(z,xl));//���i10)
	y2=OuterProduct(z2,xl);//���i11)

	zl=z2*cos(roll_in)-y2*sin(roll_in);//���i12�j
	yl=OuterProduct(zl,xl);//���i13�j

	zl.Normalize();
	yl.Normalize();

	Tl(1,1)=xl(1);Tl(1,2)=yl(1);Tl(1,3)=zl(1);
	Tl(2,1)=xl(2);Tl(2,2)=yl(2);Tl(2,3)=zl(2);
	Tl(3,1)=xl(3);Tl(3,2)=yl(3);Tl(3,3)=zl(3);//���i14�j


	double lift=GetEngineAngle();
	lTb.LoadIdentity();

lTb(1,1)=cos(lift);	lTb(1,2)=0;	lTb(1,3)=-sin(lift);
	lTb(2,1)=0;				lTb(2,2)=1;	lTb(2,3)=0;
	lTb(3,1)=sin(lift);	lTb(3,2)=0;	lTb(3,3)=cos(lift);//��(16,17)

	double L1=1245+350+90;//060821
	double L2=-75+1575+30+120+30;//060821�v��
	double L3=380;//060817
	double L4=1600-1405;//060817
	double L5=-600;//060821
	
	lTb(1,4)=L2+L3*sin(lift)-L4*cos(lift);
	lTb(2,4)=L5;
	lTb(3,4)=-L1-L3*cos(lift)-L4*sin(lift);//��(18)

	Tb=Tl*lTb;//��(1)
	//Tb=Tl;//debug

	KINE kine=SetBodyTransformWithDK(Tb);
return kine;
}

/*******************************************************************
**  �����̃V�����_��������h�����̎p���p���v�Z
*******************************************************************/
double Titan::GetDrillAngFromLen(double length)
{
	double angle=0;

	return (angle);
}
	
/*******************************************************************
**  �����o�ϐ��̃V�����_��������h�����̎p���p���v�Z
*******************************************************************/
void Titan::CalcDrillAngFromLen(void) 
{
}

/*******************************************************************
**  �����̃h�����̎p���p����V�����_�������v�Z
*******************************************************************/
double Titan::GetDrillLenFromAng(double angle)
{
	double length=0;
	
	return (length);
}

/*******************************************************************
**  �����o�ϐ��̃h�����p���p����V�����_�������v�Z
*******************************************************************/
void Titan::CalcDrillLenFromAng(void)
{
}

/*******************************************************************
**  �����̃G���W���V�����_������G���W���p���p���v�Z
*******************************************************************/
double Titan::GetEngineAngFromLen(double length)
{
	double angle=0;

	return (angle);
}

/*******************************************************************
**  �����o�ϐ��̃G���W���V�����_������G���W���p���p���v�Z
*******************************************************************/
void Titan::CalcEngineAngFromLen(void)
{
}

/*******************************************************************
**  �����̃G���W���p���p����G���W���V�����_�����v�Z
*******************************************************************/
double Titan::GetEngineLenFromAng(double angle)
{
	double length=0;

	return (length);
}

/*******************************************************************
**  �����o�ϐ��̃G���W���p���p����G���W���V�����_�����v�Z
*******************************************************************/
void Titan::CalcEngineLenFromAng(void)
{
}
