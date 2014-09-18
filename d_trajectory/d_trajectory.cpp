//140918
//軌道生成の関数群
//マイコンに対応させる
#include <stdio.h>
#include "d_trajectory.h"


//--------------------------------------------
//比例計算
double CalcProportionalValue(double v_max, double v_min, double ratio)
{
	double tmp;
	tmp=(v_max-v_min)*ratio+v_min;
	return tmp;
}

//--------------------------------------------
//軌道配列の初期化
void TRInitTrajectory(double t_array[],int size)
{
	int i;
	//for(i=0;i<TRSIZE;i++)
	for(i=0;i<size;i++)
	{
		t_array[i]=-1;//時刻0より以前は軌道に含めない
	}
}
//--------------------------------------------
//時刻に対応する配列挿入番号を返す
//時刻[i-1]と[i]の間の時刻→iが返る
//失敗したら-1が返る
int TRGetInsertIndex(double t_in,double t_array[],int size)
{
	int i;
	for(i=0;i<size;i++)//挿入場所を探す
	{
		if(t_array[i]<0)//データが入っていないところ
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
			return -1;//挿入場所なし
			//printf("TRGetInsertIndex()Error i=%d\n",i);
		}
	}
}
//--------------------------------------------
int TRSetTrajectory(int index_in, double v_in, double v_array[],int size)
{
	//index_in以降の1個後ろにデータシフトする
	//データを挿入する

	//失敗したら-1を返す
	//成功したら配列番号を返す
	int i;

	if(index_in<0)return index_in;


	//データシフト
	for(i=size-2;i>=index_in;i--)
	{
		v_array[i+1]=v_array[i];
	}

	//データ挿入
	v_array[index_in]=v_in;

	//printf("debug-TRSetTrajectory(): index_in=%d, v_in=%.0f\n",index_in,v_in);
	//printf("debug-TRSetTrajectory(): v_array[%.0f, %.0f, %.0f, ...]\n",
	//	v_array[0],v_array[1],v_array[2]);

	return index_in;
}

//--------------------------------------------
int TRGetTrajectory(double t_in, double *v_out, double t_array[], double v_array[],int size)
{
	//軌道取り出し
	//比例計算で座標取り出し
	int i=-1;
	double ratio=0;
	double v;//x,y,z;

	i=TRGetInsertIndex(t_in,t_array,size);
	if((i<=0)||(i>=size))
	{
		*v_out=0;
		return -1;
	}
	if(t_array[i]<0)return -1;//時刻エラー
	if(t_array[i-1]<0)return -1;//時刻エラー
	ratio=(t_in-t_array[i-1])/(t_array[i]-t_array[i-1]);
	v=CalcProportionalValue(v_array[i],v_array[i-1],ratio);
	*v_out=v;
	return i;

}
