/********************************************************************************
**  
**	File:	 Titan.cpp
** 
**	Description: TITAN XIの運動学
**
**	Created: 2003/10/22(Wed)	Updated: 2004/04/12(Mon)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
********************************************************************************/

#include "Titan.h"

/*******************************************************************
**  デフォルトコンストラクタ
*******************************************************************/
Titan::Titan()
{
	InitTitan();
}

/*******************************************************************
**  コンストラクタ
*******************************************************************/
Titan::Titan(const Matrix& body) : Robot(body)
{
	InitTitan();
}

/*******************************************************************
**  初期化
*******************************************************************/
void Titan::InitTitan(void)
{
	EngineAngle=0;
	return;
}

/*******************************************************************
**  デストラクタ
*******************************************************************/
Titan::~Titan()
{

}

/*******************************************************************
**  脚の付け根位置を得る
*******************************************************************/
Matrix Titan::GetLegBasePosition(int legID) const
{
	Matrix pos(3);

	pos.LoadMatrix( GetLegTransformMatrixInGlobal(legID, 0), 1, 4, 3, 4 );


	return (pos);
}

/*******************************************************************
**  第一関節位置を得る
*******************************************************************/
Matrix Titan::GetHipPosition(int legID) const
{
	Matrix pos(3);

	pos.LoadMatrix( GetLegTransformMatrixInGlobal(legID, 1), 1, 4, 3, 4 );

	return (pos);
}

/*******************************************************************
**  第二関節位置を得る
*******************************************************************/
Matrix Titan::GetKneePosition(int legID) const
{
	Matrix pos(3);

	pos.LoadMatrix( GetLegTransformMatrixInGlobal(legID, 2), 1, 4, 3, 4 );

	return (pos);
}

/*******************************************************************
**  第三関節位置を得る
*******************************************************************/
Matrix Titan::GetAnklePosition(int legID) const
{
	Matrix pos(3);

	pos.LoadMatrix( GetLegTransformMatrixInGlobal(legID, 3), 1, 4, 3, 4 );
	
	return (pos);
}

/*******************************************************************
**  第四関節位置を得る
*******************************************************************/
Matrix Titan::GetFootPosition(int legID) const
{
	Matrix pos(3);

	pos.LoadMatrix( GetLegTransformMatrixInGlobal(legID, 4), 1, 4, 3, 4 );
	
	return (pos);
}

/*******************************************************************
**  胴体位置を得る
*******************************************************************/
Matrix Titan::GetBodyPosition(void) const
{
	Matrix pos(3);

	pos.LoadMatrix( Body, 1, 4, 3, 4 );
	return (pos);

}

/*******************************************************************
**  胴体姿勢を得る
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
**  ドリルの姿勢角を設定
*******************************************************************/
bool Titan::SetDrillAngle(double angle)
{
	if(angle < ANGLE_DRILL_MIN)return false;
	if(angle > ANGLE_DRILL_MAX)return false;
	DrillAngle = angle;//060728 doi
	return true;
}

/*******************************************************************
**  エンジンリフトの姿勢角を設定
*******************************************************************/
bool Titan::SetEngineAngle(double angle)
{
	if(angle < ANGLE_LIFT_MIN)return false;
	if(angle > ANGLE_LIFT_MAX)return false;
	EngineAngle = angle;//060728 doi
	return true;
}

/*******************************************************************
**  左側のワイヤ張力を設定
*******************************************************************/
void Titan::SetWireLeftTension(double tension)
{
}

/*******************************************************************
**  右側のワイヤ張力を設定
*******************************************************************/
void Titan::SetWireRightTension(double tension)
{
}

/*******************************************************************
**  追尾装置の中心と法線で胴体位置計測．
**  リフト角度，ロール情報も使用　 060817 doi
*******************************************************************/
KINE Titan::SetBodyTransform(Matrix &c_in, Matrix &n_in, double roll_in)
{
	//追尾装置の中心と法線で胴体位置計測．リフト角度も使用
	//060525資料参照
	Matrix Tb(DH_NUM,DH_NUM);//胴体からグローバル
	Matrix Tl(DH_NUM,DH_NUM);//リフトからグローバルへ
	Matrix lTb(DH_NUM,DH_NUM);//胴体からリフトへ

	Tl.LoadIdentity();
	Tl(1,4)=c_in(1);
	Tl(2,4)=c_in(2);
	Tl(3,4)=c_in(3);//式(15)
	Matrix xl(3),yl(3),zl(3);
	Matrix z2(3),y2(3);
	Matrix z(3);
	z(1)=0;z(2)=0;z(3)=1;
	xl=-n_in;
	z2=OuterProduct(xl,OuterProduct(z,xl));//式（10)
	y2=OuterProduct(z2,xl);//式（11)

	zl=z2*cos(roll_in)-y2*sin(roll_in);//式（12）
	yl=OuterProduct(zl,xl);//式（13）

	zl.Normalize();
	yl.Normalize();

	Tl(1,1)=xl(1);Tl(1,2)=yl(1);Tl(1,3)=zl(1);
	Tl(2,1)=xl(2);Tl(2,2)=yl(2);Tl(2,3)=zl(2);
	Tl(3,1)=xl(3);Tl(3,2)=yl(3);Tl(3,3)=zl(3);//式（14）


	double lift=GetEngineAngle();
	lTb.LoadIdentity();

lTb(1,1)=cos(lift);	lTb(1,2)=0;	lTb(1,3)=-sin(lift);
	lTb(2,1)=0;				lTb(2,2)=1;	lTb(2,3)=0;
	lTb(3,1)=sin(lift);	lTb(3,2)=0;	lTb(3,3)=cos(lift);//式(16,17)

	double L1=1245+350+90;//060821
	double L2=-75+1575+30+120+30;//060821計測
	double L3=380;//060817
	double L4=1600-1405;//060817
	double L5=-600;//060821
	
	lTb(1,4)=L2+L3*sin(lift)-L4*cos(lift);
	lTb(2,4)=L5;
	lTb(3,4)=-L1-L3*cos(lift)-L4*sin(lift);//式(18)

	Tb=Tl*lTb;//式(1)
	//Tb=Tl;//debug

	KINE kine=SetBodyTransformWithDK(Tb);
return kine;
}

/*******************************************************************
**  引数のシリンダ長さからドリルの姿勢角を計算
*******************************************************************/
double Titan::GetDrillAngFromLen(double length)
{
	double angle=0;

	return (angle);
}
	
/*******************************************************************
**  メンバ変数のシリンダ長さからドリルの姿勢角を計算
*******************************************************************/
void Titan::CalcDrillAngFromLen(void) 
{
}

/*******************************************************************
**  引数のドリルの姿勢角からシリンダ長さを計算
*******************************************************************/
double Titan::GetDrillLenFromAng(double angle)
{
	double length=0;
	
	return (length);
}

/*******************************************************************
**  メンバ変数のドリル姿勢角からシリンダ長さを計算
*******************************************************************/
void Titan::CalcDrillLenFromAng(void)
{
}

/*******************************************************************
**  引数のエンジンシリンダ長からエンジン姿勢角を計算
*******************************************************************/
double Titan::GetEngineAngFromLen(double length)
{
	double angle=0;

	return (angle);
}

/*******************************************************************
**  メンバ変数のエンジンシリンダ長からエンジン姿勢角を計算
*******************************************************************/
void Titan::CalcEngineAngFromLen(void)
{
}

/*******************************************************************
**  引数のエンジン姿勢角からエンジンシリンダ長を計算
*******************************************************************/
double Titan::GetEngineLenFromAng(double angle)
{
	double length=0;

	return (length);
}

/*******************************************************************
**  メンバ変数のエンジン姿勢角からエンジンシリンダ長を計算
*******************************************************************/
void Titan::CalcEngineLenFromAng(void)
{
}
