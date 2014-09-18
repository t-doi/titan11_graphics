//140918
//�O�������̊֐��Q
//�}�C�R���ɑΉ�������
#include <stdio.h>
#include "d_trajectory.h"


//--------------------------------------------
//���v�Z
double CalcProportionalValue(double v_max, double v_min, double ratio)
{
	double tmp;
	tmp=(v_max-v_min)*ratio+v_min;
	return tmp;
}

//--------------------------------------------
//�O���z��̏�����
void TRInitTrajectory(double t_array[],int size)
{
	int i;
	//for(i=0;i<TRSIZE;i++)
	for(i=0;i<size;i++)
	{
		t_array[i]=-1;//����0���ȑO�͋O���Ɋ܂߂Ȃ�
	}
}
//--------------------------------------------
//�����ɑΉ�����z��}���ԍ���Ԃ�
//����[i-1]��[i]�̊Ԃ̎�����i���Ԃ�
//���s������-1���Ԃ�
int TRGetInsertIndex(double t_in,double t_array[],int size)
{
	int i;
	for(i=0;i<size;i++)//�}���ꏊ��T��
	{
		if(t_array[i]<0)//�f�[�^�������Ă��Ȃ��Ƃ���
		{
			//printf("debug-TRGetInsertIndex()t_array[i]<0: t_array[i]=%.0f, i=%d t_in=%.1f \n",t_array[i], i,t_in);
			return i;
		}
		else if(t_in<=t_array[i])
		{
			//printf("debug-TRGetInsertIndex()t_in<t_array[i]: t_array[]=%.0f, i=%d t_in=%.1f \n",t_array[i], i,t_in);
			//printf("debug-TRGetInsertIndex() t_array[%.0f, %.0f, %.0f, ...]\n",
			//	t_array[0],t_array[1],t_array[2]);
			return i;
		}
		else if(i==size-1)
		{
			return -1;//�}���ꏊ�Ȃ�
			//printf("TRGetInsertIndex()Error i=%d\n",i);
		}
	}
}
//--------------------------------------------
int TRSetTrajectory(int index_in, double v_in, double v_array[],int size)
{
	//index_in�ȍ~��1���Ƀf�[�^�V�t�g����
	//�f�[�^��}������

	//���s������-1��Ԃ�
	//����������z��ԍ���Ԃ�
	int i;

	if(index_in<0)return index_in;


	//�f�[�^�V�t�g
	for(i=size-2;i>=index_in;i--)
	{
		v_array[i+1]=v_array[i];
	}

	//�f�[�^�}��
	v_array[index_in]=v_in;

	//printf("debug-TRSetTrajectory(): index_in=%d, v_in=%.0f\n",index_in,v_in);
	//printf("debug-TRSetTrajectory(): v_array[%.0f, %.0f, %.0f, ...]\n",
	//	v_array[0],v_array[1],v_array[2]);

	return index_in;
}

//--------------------------------------------
int TRGetTrajectory(double t_in, double *v_out, double t_array[], double v_array[],int size)
{
	//�O�����o��
	//���v�Z�ō��W���o��
	int i=-1;
	double ratio=0;
	double v;//x,y,z;

	i=TRGetInsertIndex(t_in,t_array,size);
	if((i<=0)||(i>=size))
	{
		*v_out=0;
		return -1;
	}
	if(t_array[i]<0)return -1;//�����G���[
	if(t_array[i-1]<0)return -1;//�����G���[
	ratio=(t_in-t_array[i-1])/(t_array[i]-t_array[i-1]);
	v=CalcProportionalValue(v_array[i],v_array[i-1],ratio);
	*v_out=v;
	return i;

}
