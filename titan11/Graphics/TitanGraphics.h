/*****************************************************************************
**
**	File:	TitanGraphics.h
** 
**	説明:	OpenGL関係の描画クラス(Windows専用)
**
**	Created: 2004/02/10(Tue)	Updated: 2005/02/03(Thu)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

#ifndef	__TitanGraphics_h__
#define __TitanGraphics_h__

#include "OpenGL.h"
#include "../Data/TitanData.h"
#include "../Utility/MouseViewCtrl.h"
//#include "../Map/GridMap.h"
//#include "../Map/CellMap.h"

class TitanGraphics : public OpenGL
{
/*************************
**						**
**		メンバ変数		**
**						**
*************************/

public:
/*******************************************************************
**	画像表示する地図パターン形式
**		INVISIBLE			: 不可視(描画しない)
**		FILL				: 塗り潰し(高さ0で)
**		POLYGON				: 多面体で表示
**		CONTOUR				: 等高線で表示
*******************************************************************/
//	typedef enum MapPatterrn{INVISIBLE, WIRE, POLYGON, CONTOUR,}MAPPATTERN;

private:

/*******************************************************************
**	画像表示するTitanオブジェクトへのポインタ
*******************************************************************/
	TitanData*	pTitanData;

/*******************************************************************
**	画像表示する地図オブジェクトのポインタ
*******************************************************************/
//	GridMap*	pGridMap;
//	CellMap*	pCellMap;


/***********************************************************************************
**	ディスプレイリスト
**		DisListLegBone			: 脚リンクのリスト
**		DisListMap				: 地図のリスト 050913 doi
***********************************************************************************/
	int		DisListLegBone[4];
//	int		DisListMap[4];

/*******************************************************************
**	画像表示する地図パターン
*******************************************************************/
//	MAPPATTERN		MapDrawPattern;

/*****************************************************************************
**	描画フラグ
**		IsDisListPrepared		: ディスプレイリストが準備されたかどうか
**		IsMapLoaded				: 地図情報が読み込まれたか
**		IsGridDrawn				: 格子を描画するか
**		IsSupportPolygonDrawn	: 支持脚多角形を描画するか
**		IsAxisDrawn				: 座標系を描画するか
//**		IsMapDrawn				: 格子地図を描画するか
**		IsCellMapDrawn			: 着地禁止地図を描画するか
**		IsTerrainDrawn			: 地形を描画するか
*****************************************************************************/
	bool	IsDisListPrepared;
	bool	IsGridDrawn;
	bool	IsSupportPolygonDrawn;
	bool	IsAxisDrawn;
	bool	IsCellMapDrawn;
	bool	IsTerrainDrawn;

/*******************************************************************
**	マウスコントロールオブジェクト
*******************************************************************/
	MouseViewCtrl	MouseView;

protected:

/*************************
**						**
**		メンバ関数		**
**						**
*************************/

public:
/*******************************************************************
**	コンストラクタとデストラクタ
*******************************************************************/
	TitanGraphics(TitanData* titan, OpenGL::ViewType);
	TitanGraphics();
	~TitanGraphics();

/*******************************************************************
**	初期化
*******************************************************************/
	void	InitTitanGraphics(void);
	void	InitEnvironment(void);
	void	InitDisplayList(void);

/*******************************************************************
**	データソースの設定
*******************************************************************/
	void	SetTitanData(TitanData* titan);
//	void	SetGridMap(GridMap* pGridMap_in);//050913 doi
//	void	SetCellMap(CellMap* pCellMap_in);//050913 doi

/*******************************************************************
**	ライティングの設定
*******************************************************************/
	void	SetLighting(void);

/*******************************************************************
**	視点の設定
*******************************************************************/
	void	SetSceneView(void);

/*******************************************************************
**	地図の描画設定
*******************************************************************/
//	void	SetMapDrawPattern(MAPPATTERN pattern);

/*******************************************************************
**	セル地図の描画高さを設定
*******************************************************************/
//	void	SetCellMapHeight(double height);

/*******************************************************************
**	OpenGLのイメージのレンダリング
*******************************************************************/
	void	RenderScene(void);

/*******************************************************************
**	OpenGLのイメージ描画
*******************************************************************/
	void	DrawScene(void);

/*******************************************************************
**	Titanの描画
**		DrawTitan			: Titanの全体描画
**		DrawBody			: Titanの本体描画
**		DrawLeg				: Titanの脚描画
**		DrawLift			: Titanのエンジンリフトと回転追尾機構描画
**		DrawDrill			: Titanのドリル描画
*******************************************************************/
	void	DrawTitan(void);
	void	DrawBody(void);
	void	DrawLeg(int legID);
	void	DrawLift(double LiftAngle, double CircleAngle);
	void	DrawDrill(double DrillAngle);
/*******************************************************************
**	矢印描画 060525 doi
**		DrawArrow
*******************************************************************/
	void	DrawArrow(double x0_in, double y0_in, double z0_in, 
		double vx_in, double vy_in, double vz_in); 
	void	DrawArrow(void);


/*******************************************************************
**	地図の描画
*******************************************************************/
//	void	DrawMap(void);

/*******************************************************************
**	支持脚多角形(三角形or四角形)の描画
*******************************************************************/
	void	DrawSupportPolygon(void);

/*******************************************************************
**	グリッドの床を描画
*******************************************************************/
	void	DrawGridFloor(double x0, double y0, double x1, double y1,
						  double z,  double dx, double dy);
	void	DrawGridFloor(const Matrix& p, const Matrix& q, double dx, double dy);

/*******************************************************************
**	描画フラグの設定
*******************************************************************/
	void	ShowGrid(void)				{ IsGridDrawn			= true; return; }
	void	ShowSupportPolygon(void)	{ IsSupportPolygonDrawn = true; return; }
	void	ShowAxis(void)				{ IsAxisDrawn			= true; return; }
	void	ShowCellMap(void)			{ IsCellMapDrawn		= true; return; }
	void	ShowTerrain(void)			{ IsTerrainDrawn		= true; return; }

	void	HideGrid(void)				{ IsGridDrawn			= false; return; }
	void	HideSupportPolygon(void)	{ IsSupportPolygonDrawn = false; return; }
	void	HideAxis(void)				{ IsAxisDrawn			= false; return; }
//	void	HideCellMap(void)			{ IsCellMapDrawn		= false; return; }
//	void	HideTerrain(void)			{ IsTerrainDrawn		= false; return; }

/*******************************************************************
**	マウスによる視点変更の初期化
*******************************************************************/
	void	SetMouseViewPoint(int x, int y);

/*******************************************************************
**	マウスのイベントモードを取得
*******************************************************************/
	MouseViewCtrl::DragMode GetMouseViewDragMode(void);

/*******************************************************************
**	マウスのイベントをセット
*******************************************************************/
	void	SetMouseViewDragMode(MouseViewCtrl::DragMode mode);

/*******************************************************************
**	マウスのドラッグモードによりViewの変更
*******************************************************************/
	void	MouseViewTransform(int x, int y);

/*******************************************************************
**	視覚情報の直接設定
*******************************************************************/
	void	SetMouseViewAzimuth(double azimuth);
	void	SetMouseViewElevation(double elevation);
	void	SetMouseViewDistance(double distance);

/*******************************************************************
**	座標変換(4x4)
*******************************************************************/
	void	TransformGLMatrix(const Matrix& matrix);

private:
/*******************************************************************
**	地図情報を描画するためのヘルパー関数
*******************************************************************/
//	void	DrawCellMap(void);
//	void	DrawWireMap(void);
//	void	DrawPolygonMap(void);
//	void	DrawContourMap(void);
//	void	DrawCellMapOnGridMap(void);//050919doi
	void	DrawGrayPolygon(Matrix &p1, Matrix &p2, Matrix &p3);//法線の向きで濃さを変えたポリゴン

protected:
};

#endif // __TitanGraphics_h__