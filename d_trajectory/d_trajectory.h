//140918
//�O�������̊֐��Q
//�}�C�R���ɑΉ�������
//���Ԃƕω�����p�����[�^�z��͊֐��̊O�ɒu���C
//�������C�f�[�^�Z�b�g�C�f�[�^��肾�����֐��ōs��
#ifndef D_TRAJECTORY_H
#define D_TRAJECTORY_H

void TRInitTrajectory(double t_array[],int size);
int TRGetInsertIndex(double t_in,double t_array[],int size);//�}���ꏊ
int TRSetTrajectory(int index_in, double v_in, double v_array[],int size);
int TRGetTrajectory(double t_in, double *v_out, double t_array[], double v_array[],int size);
int TRGetIndex(double t_in,double t_array[],int size);

#endif
