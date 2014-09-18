//100308
//TITAN XIの運動学関連のラッパー関数群
#include "../titan11/Kinematics/Titan.h"


/************************/
/*  グローバル変数 */
/************************/
Matrix m44_temp;//4x4行列
Matrix m31_temp;//3x1行列
Titan *titan_temp;

//-----------------------------
int InitKinematicsUtil(void)
{
  //運動学計算の内部メモリ初期化，メモリ確保
  m44_temp.SetSize(4,4);
  m31_temp.SetSize(3,1);
  titan_temp=new Titan;//明示的にnewしてやらないと初期化がうまくいかない
  //titan_temp.InitTitan();
  //titan_temp.CreateLegInstance();

return 1;
}
//-----------------------------
int SetDK(int legno, double th1_in,double th2_in,double th3_in)
{
  //行列に関節角セット
  m31_temp(1)=th1_in;  m31_temp(2)=th2_in;  m31_temp(3)=th3_in;
  //順運動学
  titan_temp->SetLegJntAng(legno, m31_temp);


  return 1;
}
//-----------------------------
int SetIK(int legno, double x_in, double y_in, double z_in)
{
  KINE res;
  //脚先６自由度をあらわす行列に値セット
  m44_temp.LoadIdentity();
  m44_temp(1,4)=x_in;m44_temp(2,4)=y_in;m44_temp(3,4)=z_in;
  //逆運動学
  res=titan_temp->SetFootConfiguration(legno, m44_temp);

  return 1;
}
//-----------------------------
int GetAng(int legno, double *th1_out,double *th2_out,double *th3_out)
{
  //関節角取得(IK計算後の処理)
  m31_temp=titan_temp->GetLegJntAng(legno);

  //関節角を表す行列から値読み出し．
  *th1_out=m31_temp(1);
  *th2_out=m31_temp(2);
  *th3_out=m31_temp(3);

    return 1;

}
//-----------------------------
int GetPos(int legno, double *x_out, double *y_out, double *z_out)
{
  //脚先端位置計算(DK計算後の処理)
  m31_temp=titan_temp->GetFootPosition(legno);

  //行列から値読み出し．
  *x_out=m31_temp(1);*y_out=m31_temp(2);*z_out=m31_temp(3);

  return 1;
}
//-----------------------------
int GetCylinder(int legno, double *L1_out, double *L2_out, double *L3_out)
{
  //シリンダ長さ計算
	titan_temp->CalcLegLenFromAng(legno);
	//シリンダ長さ出力（３行１列行列へ）
  m31_temp=titan_temp->GetLegCldrLen(legno);

  //行列から値読み取り
  *L1_out=m31_temp(1);  *L2_out=m31_temp(2);  *L3_out=m31_temp(3);

  return 1;
}
//-----------------------------

