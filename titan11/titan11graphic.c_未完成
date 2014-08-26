//100308
//TITAN XIの運動学関連のラッパー関数群
//内部の変数にスタティックにTITANの関節角，脚先位置，座標変換行列などの情報を保持

#ifndef KINEMATICS_UITL_H
#define KINEMATICS_UITL_H
int InitKinematicsUtil(void);//運動学計算の内部メモリ初期化，メモリ確保
int SetDK(int legno, double th1_in,double th2_in,double th3_in);
int SetIK(int legno, double x_in, double y_in, double z_in);
int GetAng(int legno, double *th1_out,double *th2_out,double *th3_out);
int GetPos(int legno, double *x_out, double *y_out, double *z_out);
int GetCylinder(int legno, double *L1_out, double *L2_out, double *L3_out);//シリンダ長さ出力
#endif
