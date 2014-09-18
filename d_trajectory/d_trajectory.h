//140918
//軌道生成の関数群
//マイコンに対応させる
//時間と変化するパラメータ配列は関数の外に置き，
//初期化，データセット，データ取りだしを関数で行う
#ifndef D_TRAJECTORY_H
#define D_TRAJECTORY_H

void TRInitTrajectory(double t_array[],int size);
int TRGetInsertIndex(double t_in,double t_array[],int size);//挿入場所
int TRSetTrajectory(int index_in, double v_in, double v_array[],int size);
int TRGetTrajectory(double t_in, double *v_out, double t_array[], double v_array[],int size);
int TRGetIndex(double t_in,double t_array[],int size);

#endif
