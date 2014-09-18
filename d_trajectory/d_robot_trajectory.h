//d_robot_trajectory.cpp/h
//140917
//TITANの脚先軌道生成の関数
//配列を内部で保持する

#ifndef D_ROBOT_TRAJECTORY_H
#define D_ROBOT_TRAJECTORY_H

void RTset_trajectory(double t_in,
	double x1_in,double y1_in,double z1_in,
	double x2_in,double y2_in,double z2_in,
	double x3_in,double y3_in,double z3_in,
	double x4_in,double y4_in,double z4_in
);
int RTget_trajectory(double t_in,
	double *x1_out,double *y1_out,double *z1_out,
	double *x2_out,double *y2_out,double *z2_out,
	double *x3_out,double *y3_out,double *z3_out,
	double *x4_out,double *y4_out,double *z4_out
);
void RTSetTrajectoryICCRAWL(void);
void RTSetTrajectoryICCRAWL_standby(void);//間歇クロールスタンバイ

#endif

