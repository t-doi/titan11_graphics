//d_robot_trajectory.cpp/h
//140917
//TITANの脚先軌道生成の関数
//配列を内部で保持する
#include <stdio.h>
#include "d_trajectory.h"
#include "d_robot_trajectory.h"
#define TRSIZE 20

double t_temp[TRSIZE];

double x1_temp[TRSIZE];
double y1_temp[TRSIZE];
double z1_temp[TRSIZE];

double x2_temp[TRSIZE];
double y2_temp[TRSIZE];
double z2_temp[TRSIZE];

double x3_temp[TRSIZE];
double y3_temp[TRSIZE];
double z3_temp[TRSIZE];

double x4_temp[TRSIZE];
double y4_temp[TRSIZE];
double z4_temp[TRSIZE];


//-------
void RTset_trajectory(double t_in,
	double x1_in,double y1_in,double z1_in,
	double x2_in,double y2_in,double z2_in,
	double x3_in,double y3_in,double z3_in,
	double x4_in,double y4_in,double z4_in
)
{
	int res;

	int i1=TRGetInsertIndex(t_in,t_temp,TRSIZE);
	res=TRSetTrajectory(i1,t_in,t_temp, TRSIZE);

	res=TRSetTrajectory(i1,x1_in,x1_temp, TRSIZE);
	res=TRSetTrajectory(i1,y1_in,y1_temp, TRSIZE);
	res=TRSetTrajectory(i1,z1_in,z1_temp, TRSIZE);

	res=TRSetTrajectory(i1,x2_in,x2_temp, TRSIZE);
	res=TRSetTrajectory(i1,y2_in,y2_temp, TRSIZE);
	res=TRSetTrajectory(i1,z2_in,z2_temp, TRSIZE);

	res=TRSetTrajectory(i1,x3_in,x3_temp, TRSIZE);
	res=TRSetTrajectory(i1,y3_in,y3_temp, TRSIZE);
	res=TRSetTrajectory(i1,z3_in,z3_temp, TRSIZE);

	res=TRSetTrajectory(i1,x4_in,x4_temp, TRSIZE);
	res=TRSetTrajectory(i1,y4_in,y4_temp, TRSIZE);
	res=TRSetTrajectory(i1,z4_in,z4_temp, TRSIZE);

		//printf("debug:RTset_trajectory(): i1=%d, t_in=%.0f (%.0f %.0f %.0f)\n", i1, t_in, x1_in, y1_in, z1_in);
		//printf("debug:RTset_trajectory(): t_temp[%.0f %.0f %.0f ...]\n", t_temp[0], t_temp[1], t_temp[2]);


	return;
}

//-------
int RTget_trajectory(double t_in,
	double *x1_out,double *y1_out,double *z1_out,
	double *x2_out,double *y2_out,double *z2_out,
	double *x3_out,double *y3_out,double *z3_out,
	double *x4_out,double *y4_out,double *z4_out
)
{
	double x,y,z;
	int res;
		res=TRGetTrajectory(t_in,&x,t_temp, x1_temp,TRSIZE);
		if(res<0)return res;
		TRGetTrajectory(t_in,&y,t_temp, y1_temp,TRSIZE);
		TRGetTrajectory(t_in,&z,t_temp, z1_temp,TRSIZE);
	*x1_out=x;
	*y1_out=y;
	*z1_out=z;

		TRGetTrajectory(t_in,&x,t_temp, x2_temp,TRSIZE);
		TRGetTrajectory(t_in,&y,t_temp, y2_temp,TRSIZE);
		TRGetTrajectory(t_in,&z,t_temp, z2_temp,TRSIZE);
	*x2_out=x;
	*y2_out=y;
	*z2_out=z;

		TRGetTrajectory(t_in,&x,t_temp, x3_temp,TRSIZE);
		TRGetTrajectory(t_in,&y,t_temp, y3_temp,TRSIZE);
		TRGetTrajectory(t_in,&z,t_temp, z3_temp,TRSIZE);
	*x3_out=x;
	*y3_out=y;
	*z3_out=z;

		TRGetTrajectory(t_in,&x,t_temp, x4_temp,TRSIZE);
		TRGetTrajectory(t_in,&y,t_temp, y4_temp,TRSIZE);
		TRGetTrajectory(t_in,&z,t_temp, z4_temp,TRSIZE);
	*x4_out=x;
	*y4_out=y;
	*z4_out=z;

	return 1;
}
//------------------------------------
//間歇クロール歩容スタンバイの配列セット
void RTSetTrajectoryICCRAWL_standby(void)
{
    RTSetTrajectoryICCRAWL();
    t_temp[1]=100;//適当．長い時間を入れる
    x1_temp[1]=x1_temp[0];y1_temp[1]=y1_temp[0];z1_temp[1]=z1_temp[0];
    x2_temp[1]=x2_temp[0];y2_temp[1]=y2_temp[0];z2_temp[1]=z2_temp[0];
    x3_temp[1]=x3_temp[0];y3_temp[1]=y3_temp[0];z3_temp[1]=z3_temp[0];
    x4_temp[1]=x4_temp[0];y4_temp[1]=y4_temp[0];z4_temp[1]=z4_temp[0];
    t_temp[2]=-1;//第3要素に負の値を入れる→ここでストップ
}
//------------------------------------
//間歇クロール歩容の配列セット
void RTSetTrajectoryICCRAWL(void)
{
	double speed=1.0;//これの逆数でタイムスケジュールを決める
	double dt=5.0/speed;//1動作の時間
	double stride[3];//1歩のベクトル
	double move1[]={-600,-500,0};//胴体推進時のベクトル1（胴体からみた脚先の動き）
	double move2[]={-600,500,0};//胴体推進時のベクトル2（胴体からみた脚先の動き）
	//double move1[]={0,-500,0};//胴体推進時のベクトル1（胴体からみた脚先の動き）
	//double move2[]={0,500,0};//胴体推進時のベクトル2（胴体からみた脚先の動き）
	double height=800;//足を下に下げる量
	double height_up=300;//初期状態で足を持ち上げる量
	double swing_height=500;//遊脚高さ
	double x0=2400;//脚１の軌道ベース点．ほかの脚はここに対して対称
	double y0=1800;//脚１の軌道ベース点．ほかの脚はここに対して対称

	stride[0]=-(move1[0]+move2[0]);
	stride[1]=-(move1[1]+move2[1]);
	stride[2]=-(move1[2]+move2[2]);

	TRInitTrajectory(t_temp,TRSIZE);//時間配列の初期化

	//テスト
	//RTset_trajectory(0, 1,1,1, 1,1,1, 1,1,1, 1,1,1);
	//RTset_trajectory(2.5, 2,1,1, 1,1,1, 1,1,1, 1,1,1);
	//RTset_trajectory(1, 3,1,1, 1,1,1, 1,1,1, 1,1,1);
	//return;//debug

	//↓初期状態（足上げ）
	//RTset_trajectory(0*dt, x0+move2[0], y0+move2[1], height_up,		x0, -y0, height_up, 	-x0+move2[0], y0+move2[1], height_up, -x0, -y0, height_up);


	//↓立ち上がり
	RTset_trajectory(0*dt, x0+move2[0], y0+move2[1], -height,		x0, -y0, -height, 	-x0+move2[0], y0+move2[1], -height, -x0, -y0, -height);

	//↓脚３遊脚
	RTset_trajectory(1*dt, x0+move2[0], y0+move2[1], -height,		x0, -y0, -height, 	-x0+move2[0], y0+move2[1], -height+swing_height,		 -x0, -y0, -height);

	//↓脚３復帰
	RTset_trajectory(2*dt, x0+move2[0], y0+move2[1], -height,		x0, -y0, -height, 	-x0+move2[0]+stride[0], y0+move2[1]+stride[1], -height+swing_height,		 -x0, -y0, -height);

	//↓脚３下ろす
	RTset_trajectory(3*dt, x0+move2[0], y0+move2[1], -height,		x0, -y0, -height, 	-x0+move2[0]+stride[0], y0+move2[1]+stride[1], -height,		 -x0, -y0, -height);

	//↓脚１遊脚
	RTset_trajectory(4*dt, x0+move2[0], y0+move2[1], -height+swing_height,			x0, -y0, -height,	 	-x0+move2[0]+stride[0], y0+move2[1]+stride[1], -height,		 -x0, -y0, -height);

	//↓脚１復帰
	RTset_trajectory(5*dt, x0+move2[0]+stride[0], y0+move2[1]+stride[1], -height+swing_height,		x0, -y0, -height,	 	-x0+move2[0]+stride[0], y0+move2[1]+stride[1], -height,		 -x0, -y0, -height);

	//↓脚１下ろす
	RTset_trajectory(6*dt, x0+move2[0]+stride[0], y0+move2[1]+stride[1], -height,		x0, -y0, -height,	 	-x0+move2[0]+stride[0], y0+move2[1]+stride[1], -height,		 -x0, -y0, -height);

	//↓胴体推進
	RTset_trajectory(7*dt, x0, y0, -height,		x0+move1[0], -y0+move1[1], -height,	 	-x0, y0, -height,		 -x0+move1[0], -y0+move1[1], -height);

	//↓脚４遊脚
	RTset_trajectory(8*dt, x0, y0, -height,		x0+move1[0], -y0+move1[1], -height,	 	-x0, y0, -height,		 -x0+move1[0], -y0+move1[1], -height+swing_height);

	//↓脚４復帰
	RTset_trajectory(9*dt, x0, y0, -height,		x0+move1[0], -y0+move1[1], -height,	 	-x0, y0, -height,		 -x0+move1[0]+stride[0], -y0+move1[1]+stride[1], -height+swing_height);

	//↓脚４下ろす
	RTset_trajectory(10*dt, x0, y0, -height,		x0+move1[0], -y0+move1[1], -height,	 	-x0, y0, -height,		 -x0+move1[0]+stride[0], -y0+move1[1]+stride[1], -height);

	//↓脚２遊脚
	RTset_trajectory(11*dt, x0, y0, -height,		x0+move1[0], -y0+move1[1], -height+swing_height,	 	-x0, y0, -height,		 -x0+move1[0]+stride[0], -y0+move1[1]+stride[1], -height);

	//↓脚２復帰
	RTset_trajectory(12*dt, x0, y0, -height,		x0+move1[0]+stride[0], -y0+move1[1]+stride[1], -height+swing_height,	 	-x0, y0, -height,		 -x0+move1[0]+stride[0], -y0+move1[1]+stride[1], -height);

	//↓脚２下ろす
	RTset_trajectory(13*dt, x0, y0, -height,		x0+move1[0]+stride[0], -y0+move1[1]+stride[1], -height,	 	-x0, y0, -height,		 -x0+move1[0]+stride[0], -y0+move1[1]+stride[1], -height);

	//↓胴体推進
	RTset_trajectory(14*dt, x0+move2[0], y0+move2[1], -height,		x0, -y0, -height,	 	-x0+move2[0], y0+move2[1], -height,		 -x0, -y0, -height);
}

