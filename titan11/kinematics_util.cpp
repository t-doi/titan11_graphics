//100308
//TITAN XI�̉^���w�֘A�̃��b�p�[�֐��Q
#include "../titan11/Kinematics/Titan.h"


/************************/
/*  �O���[�o���ϐ� */
/************************/
Matrix m44_temp;//4x4�s��
Matrix m31_temp;//3x1�s��
Titan *titan_temp;

//-----------------------------
int InitKinematicsUtil(void)
{
  //�^���w�v�Z�̓����������������C�������m��
  m44_temp.SetSize(4,4);
  m31_temp.SetSize(3,1);
  titan_temp=new Titan;//�����I��new���Ă��Ȃ��Ə����������܂������Ȃ�
  //titan_temp.InitTitan();
  //titan_temp.CreateLegInstance();

return 1;
}
//-----------------------------
int SetDK(int legno, double th1_in,double th2_in,double th3_in)
{
  //�s��Ɋ֐ߊp�Z�b�g
  m31_temp(1)=th1_in;  m31_temp(2)=th2_in;  m31_temp(3)=th3_in;
  //���^���w
  titan_temp->SetLegJntAng(legno, m31_temp);


  return 1;
}
//-----------------------------
int SetIK(int legno, double x_in, double y_in, double z_in)
{
  KINE res;
  //�r��U���R�x������킷�s��ɒl�Z�b�g
  m44_temp.LoadIdentity();
  m44_temp(1,4)=x_in;m44_temp(2,4)=y_in;m44_temp(3,4)=z_in;
  //�t�^���w
  res=titan_temp->SetFootConfiguration(legno, m44_temp);

  return 1;
}
//-----------------------------
int GetAng(int legno, double *th1_out,double *th2_out,double *th3_out)
{
  //�֐ߊp�擾(IK�v�Z��̏���)
  m31_temp=titan_temp->GetLegJntAng(legno);

  //�֐ߊp��\���s�񂩂�l�ǂݏo���D
  *th1_out=m31_temp(1);
  *th2_out=m31_temp(2);
  *th3_out=m31_temp(3);

    return 1;

}
//-----------------------------
int GetPos(int legno, double *x_out, double *y_out, double *z_out)
{
  //�r��[�ʒu�v�Z(DK�v�Z��̏���)
  m31_temp=titan_temp->GetFootPosition(legno);

  //�s�񂩂�l�ǂݏo���D
  *x_out=m31_temp(1);*y_out=m31_temp(2);*z_out=m31_temp(3);

  return 1;
}
//-----------------------------
int GetCylinder(int legno, double *L1_out, double *L2_out, double *L3_out)
{
  //�V�����_�����v�Z
	titan_temp->CalcLegLenFromAng(legno);
	//�V�����_�����o�́i�R�s�P��s��ցj
  m31_temp=titan_temp->GetLegCldrLen(legno);

  //�s�񂩂�l�ǂݎ��
  *L1_out=m31_temp(1);  *L2_out=m31_temp(2);  *L3_out=m31_temp(3);

  return 1;
}
//-----------------------------

