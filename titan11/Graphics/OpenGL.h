/*****************************************************************************
**
**	File:	OpenGL.h
** 
**	説明:	OpenGL関係の描画クラス(Windows専用)
**
**	Created: 2004/02/09(Mon)	Updated: 2005/02/03(Thu)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

#ifndef	__OpenGL_h__
#define __OpenGL_h__

#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
//#include <gl/GLAUX.h>
#include <math.h>
#include <stdio.h>

#include "../Math/MathLibrary.h"

class OpenGL
{
/*************************
**						**
**		メンバ変数		**
**						**
*************************/
public:
/*******************************************************************
**	視点パターン
*******************************************************************/
    enum ViewType{PERSPECTIVE, TOP, SIDE, FRONT};

/*******************************************************************
**	主な色パターン
*******************************************************************/
	enum COLOR{RED, GREEN, BLUE, YELLOW, GRAY, BLACK, WHITE, SKY, ORANGE, PURPLE};

/*******************************************************************
**	ハンドルの変数
**		m_hWnd			: ウィンドウ
**		m_hDC			: デバイスコンテキスト
**		m_hRC			: レンダリングコンテキスト
*******************************************************************/
    HWND	m_hWnd;
	HDC		m_hDC;
	HGLRC	m_hRC;

private:

/*******************************************************************
**	視点パターン
*******************************************************************/
	ViewType VType;

protected:

/*******************************************************************
**	ライティング
*******************************************************************/
	GLfloat m_ambientLight0[4];			// 環境光
	GLfloat m_diffuseLight0[4];			// 拡散光
	GLfloat m_specularLight0[4];		// 鏡面光
	GLfloat m_positionLight0[4];		// 光源位置
	GLfloat m_directionLight0[3];		// スポットライト方向

/*******************************************************************
**	マテリアル色
*******************************************************************/
	GLfloat	materialAmbDiffRed[4];
	GLfloat	materialAmbDiffGreen[4];
	GLfloat materialAmbDiffBlue[4];
	GLfloat materialAmbDiffYellow[4];
	GLfloat materialAmbDiffGray[4];
	GLfloat materialAmbDiffBlack[4];
	GLfloat materialAmbDiffSky[4];
	GLfloat materialAmbDiffOrange[4];
	GLfloat materialAmbDiffPurple[4];
	GLfloat materialAmbDiffWhite[4];

/*************************
**						**
**		メンバ関数		**
**						**
*************************/
public:
/*******************************************************************
**	コンストラクタとデストラクタ
*******************************************************************/
    OpenGL();
    ~OpenGL();

/*******************************************************************
**	OpenGLの初期化と破棄
**		InitOpenGL			: 初期化
**		DestroyOpenGL			: 破棄
*******************************************************************/
	bool			InitOpenGL(HWND hWnd);
	void			DestroyOpenGL(void);
	void			SetDefault(void);
	virtual void	InitEnvironment(void);

/*******************************************************************
**	メンバ変数の取得
*******************************************************************/
	HWND	GetHWND(void){ return (m_hWnd);}

/*******************************************************************
**	OpenGLのイメージのレンダリング
*******************************************************************/
	virtual void	RenderScene(void);

/*******************************************************************
**	OpenGLのイメージ描画
*******************************************************************/
	virtual void	DrawScene(void);

/*******************************************************************
**	視点の設定
*******************************************************************/
	virtual void	SetSceneView(void);
	void			SetViewPoint(double distance, double azimuth, double elevation,
								 double x = 0, double y = 0, double z = 0,
								 double aspect_ratio = 1);
	void		SetViewType(ViewType type);
	ViewType	GetViewType(void){return (VType);}

/*******************************************************************
**	ライティングの設定
*******************************************************************/
	virtual void	SetLighting(void);

/*******************************************************************
**	マテリアルカラーの設定
*******************************************************************/
	void	SetMaterial(double red, double green, double blue, double alpha);
	void	SetMaterial(GLfloat* materialAmbDiffColor);

/*******************************************************************
**	マテリアルカラーの取得
*******************************************************************/
	GLfloat*	SelectColor(COLOR color);

/*********************
**	各種モデリング  **
*********************/

//////////////////////////////////////////////////////////////////////
////////// 3Dモデリング //////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/*******************************************************************
**	座標軸描画
*******************************************************************/
	void	DrawCoordinateAxis(double length, double width = 1.0); 

/*******************************************************************
**	ポリゴンモデリング
*******************************************************************/
	void	DrawPolygonSurface(double x0, double y0, double z0,
							   double x1, double y1, double z1,
							   double x2, double y2, double z2);
	void	DrawPolygonSurface(const Matrix& p, const Matrix& q, const Matrix& r);

/*******************************************************************
**	円柱描画
*******************************************************************/
	void	DrawCylinder2(double radius, double height);//16角形で単純化
	void	DrawCylinder(double radius, double height);

/*******************************************************************
**	球描画
*******************************************************************/
	void	DrawSphere(double radius);

/*******************************************************************
**	線描画(線画)
*******************************************************************/
	void	DrawLine(double x0, double y0, double z0,
					 double x1, double y1, double z1, double width = 1.0);
	void	DrawLine(const Matrix& p, const Matrix& q, double width = 1.0);

/*******************************************************************
**	Box描画(線画)
*******************************************************************/
	void	DrawBox(double x0, double y0, double z0, double x1, double y1, double z1);
	void	DrawBox(const Matrix& p, const Matrix& q);

/*******************************************************************
**	フレーム描画(平面と4隅の垂線)
*******************************************************************/
	void	DrawFrame(double x0, double y0, double z0, double x1, double y1, double z1);
	void	DrawFrame(const Matrix& p, const Matrix& q);

//////////////////////////////////////////////////////////////////////
////////// 2Dモデリング //////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/*******************************************************************
**	平面板描画(対角線の頂点と高さ)
*******************************************************************/
	void	DrawPlaneXY(double x0, double y0, double x1, double y1, double z);
	void	DrawPlaneYZ(double y0, double z0, double y1, double z1, double x);
	void	DrawPlaneZX(double z0, double x0, double z1, double x1, double y);


private:
/*******************************************************************
**	OpenGLの初期化に関係
**		SetWindowPixelFormat		: ピクセルフォーマットの設定
**		CreateGLContext				: コンテキストの設定
*******************************************************************/
	bool	SetWindowPixelFormat(void);
	bool	CreateGLContext(void);
};

#endif // __OpenGL_h__
