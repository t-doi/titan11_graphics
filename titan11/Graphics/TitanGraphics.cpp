/*****************************************************************************
**
**	File:	TitanGraphics.cpp
**
**	説明:	OpenGL関係の描画クラス(Windows専用)
**
**	Created: 2004/02/10(Tue)	Updated: 2005/02/25(Fri)
**  Updated 050913 doi
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

#include "TitanGraphics.h"
//#include "../Plan/GaitParameter.h"

/*******************************************************************
**	グリッド表示するための定数
*******************************************************************/
const double GRID_START_X	= -100000.0;
const double GRID_START_Y	= -100000.0;
const double GRID_END_X		= 100000.0;
const double GRID_END_Y		= 100000.0;
const double GRID_DX		= 2000.0;
const double GRID_DY		= 2000.0;

/*******************************************************************
**	コンストラクタ
*******************************************************************/
TitanGraphics::TitanGraphics(TitanData* titan, OpenGL::ViewType type)
{
	if ( titan == 0)
	{
		fprintf(stderr, "TitanGraphics: Error in constructor");
	}

	InitTitanGraphics();

	pTitanData = titan;

	SetViewType(type);

}

/*******************************************************************
**	デフォルトコンストラクタ
*******************************************************************/
TitanGraphics::TitanGraphics()
{
	InitTitanGraphics();
}

/*******************************************************************
**	デストラクタ
*******************************************************************/
TitanGraphics::~TitanGraphics()
{
	// ディスプレイリストの消去
	glDeleteLists(DisListLegBone[0], 4);
//	glDeleteLists(DisListMap[0], 4);

/*
	// 地図オブジェクトの削除

	// ポインタが空でなければ削除 050913 doi
	if ( pGridMap!=NULL )
	delete pGridMap;

	// ポインタが空でなければ削除 050913 doi
	if ( pCellMap!=NULL )
	delete pCellMap;
*/
}

/*******************************************************************
**	初期化
*******************************************************************/
void TitanGraphics::InitTitanGraphics(void)
{

	// マウスの操作モードの初期化
	MouseView.SetDefault();

	// OpenGL環境の初期化
	InitEnvironment();

	// ポインタの初期化
	pTitanData = 0;


//	pGridMap=NULL;//050913 doi
//	pCellMap=NULL;//050913 doi

	// 地図の描画パターンの設定
//	MapDrawPattern = WIRE;

	// 各フラグの初期化
	IsDisListPrepared		= false;
	IsGridDrawn				= true;
	IsSupportPolygonDrawn	= true;
	IsAxisDrawn				= true;
//	IsCellMapDrawn			= false;

	return;
}

/*******************************************************************
**	変数をデフォルトにセット
*******************************************************************/
void TitanGraphics::InitEnvironment(void)
{
	// ライティングの設定
	m_ambientLight0[0] = 1.0f;		// 環境光
	m_ambientLight0[1] = 1.0f;
	m_ambientLight0[2] = 1.0f;
	m_ambientLight0[3] = 0.8f;

	m_diffuseLight0[0] = 1.0f;		// 拡散光
	m_diffuseLight0[1] = 1.0f;
	m_diffuseLight0[2] = 1.0f;
	m_diffuseLight0[3] = 1.0f;

	m_specularLight0[0] = 1.0f;		// 鏡面光
	m_specularLight0[1] = 1.0f;
	m_specularLight0[2] = 1.0f;
	m_specularLight0[3] = 1.0f;

	m_positionLight0[0] = 0.0f;		// 光源位置
	m_positionLight0[1] = -2500.0f;
	m_positionLight0[2] = 5000.0f;
	m_positionLight0[3] = 0.0f;

	m_directionLight0[0] = 0.0f;	// スポットライト方向
	m_directionLight0[1] = 0.0f;
	m_directionLight0[2] = -5000.0f;

	// マテリアルの色の設定
	materialAmbDiffRed[0] = 1.0f;
	materialAmbDiffRed[1] = 0.0f;
	materialAmbDiffRed[2] = 0.0f;
	materialAmbDiffRed[3] = 1.0f;

	materialAmbDiffGreen[0] = 0.0f;
	materialAmbDiffGreen[1] = 1.0f;
	materialAmbDiffGreen[2] = 0.0f;
	materialAmbDiffGreen[3] = 1.0f;

	materialAmbDiffBlue[0] = 0.0f;
	materialAmbDiffBlue[1] = 0.0f;
	materialAmbDiffBlue[2] = 1.0f;
	materialAmbDiffBlue[3] = 1.0f;

	materialAmbDiffYellow[0] = 1.0f;
	materialAmbDiffYellow[1] = 0.8f;
	materialAmbDiffYellow[2] = 0.0f;
	materialAmbDiffYellow[3] = 1.0f;

	materialAmbDiffBlack[0] = 0.0f;
	materialAmbDiffBlack[1] = 0.0f;
	materialAmbDiffBlack[2] = 0.0f;
	materialAmbDiffBlack[3] = 1.0f;

	materialAmbDiffGray[0] = 0.7f;
	materialAmbDiffGray[1] = 0.7f;
	materialAmbDiffGray[2] = 0.7f;
	materialAmbDiffGray[3] = 0.5f;

	materialAmbDiffWhite[0] = 1.0f;
	materialAmbDiffWhite[1] = 1.0f;
	materialAmbDiffWhite[2] = 1.0f;
	materialAmbDiffWhite[3] = 1.0f;

	materialAmbDiffSky[0] = 0.0f;
	materialAmbDiffSky[1] = 0.7f;
	materialAmbDiffSky[2] = 1.0f;
	materialAmbDiffSky[3] = 1.0f;

	materialAmbDiffOrange[0] = 1.0f;
	materialAmbDiffOrange[1] = 0.6f;
	materialAmbDiffOrange[2] = 0.3f;
	materialAmbDiffOrange[3] = 1.0f;

	materialAmbDiffPurple[0] = 1.0f;
	materialAmbDiffPurple[1] = 0.0f;
	materialAmbDiffPurple[2] = 0.7f;
	materialAmbDiffPurple[3] = 1.0f;

	return;
}

/*******************************************************************
**	ライティングの設定
*******************************************************************/
void TitanGraphics::SetLighting(void)
{
	glLightfv(GL_LIGHT0,	GL_AMBIENT,			m_ambientLight0);
	glLightfv(GL_LIGHT0,	GL_DIFFUSE,			m_diffuseLight0);
	glLightfv(GL_LIGHT0,	GL_SPECULAR,		m_specularLight0);
	glLightfv(GL_LIGHT0,	GL_POSITION,		m_positionLight0);
	glLightf(GL_LIGHT0,		GL_SPOT_CUTOFF,		180.0f);
	glLightfv(GL_LIGHT0,	GL_SPOT_DIRECTION,	m_directionLight0);
	glLightf(GL_LIGHT0,		GL_SPOT_EXPONENT,	30.0f);

	return;
}

/*******************************************************************
**	ディスプレイリストのセット
*******************************************************************/
void TitanGraphics::InitDisplayList(void)
{
	if (IsDisListPrepared != false)return;

	// ディスプレイリストの生成
	int list_0 = glGenLists(8);

	int i;

	// 生成判定
	if (list_0 == 0)
	{
		fprintf(stderr, "Display List error!\n\n");
		return;//error
	}

	for (i=0; i<4; i++)//脚リンク
	{
		DisListLegBone[i] = (list_0+i);
	}
	for(i=0;i<4;i++)//地図
	{
//		DisListMap[i]=(list_0+i+4);
	}

	//脚リンクディスプレイリスト
	//COLOR CJoint	= WHITE;
	COLOR CJoint	= SKY;
	//COLOR CLink		= GRAY;
	COLOR CLink		= SKY;


	double l1,l2,l3,l4,l5,l6;//脚モデリングパラメータ.060726memo参照

	// Hip
	glNewList(DisListLegBone[0], GL_COMPILE);
	l1=100;
	l2=400;
	l3=430+200;
	l4=200;
	l5=300;
	l6=100;
		SetMaterial( SelectColor(CJoint) );
		//double hip_height=430;//脚付け根の下向き突起高さ
		glPushMatrix();
		//DrawBox(-200,  -hip_height, -150, 300, 0, 150);//051107脚付け根の下向き突起
		DrawPolygonSurface((GLfloat)-l6,(GLfloat)0,(GLfloat)(-0.5*l4),
							(GLfloat)-l6,(GLfloat)l1,(GLfloat)(-0.5*l4),
							(GLfloat)l5,(GLfloat)0,(GLfloat)(-0.5*l4));//側面１
		DrawPolygonSurface((GLfloat)-l6,(GLfloat)l1,(GLfloat)(-0.5*l4),
							(GLfloat)l5,(GLfloat)l1,(GLfloat)(-0.5*l4),
							(GLfloat)l5,(GLfloat)0,(GLfloat)(-0.5*l4));//側面２
		DrawPolygonSurface((GLfloat)-l6,(GLfloat)0,(GLfloat)(-0.5*l4),
							(GLfloat)l5,(GLfloat)0,(GLfloat)(-0.5*l4),
							(GLfloat)-l6,(GLfloat)-l2,(GLfloat)(-0.5*l4));//側面３
		DrawPolygonSurface((GLfloat)-l6,(GLfloat)-l2,(GLfloat)(-0.5*l4),
							(GLfloat)l5,(GLfloat)0,(GLfloat)(-0.5*l4),
							(GLfloat)l5,(GLfloat)-l3,(GLfloat)(-0.5*l4));//側面４
		DrawPolygonSurface((GLfloat)-l6,(GLfloat)0,(GLfloat)(0.5*l4),
							(GLfloat)l5,(GLfloat)0,(GLfloat)(0.5*l4),
							(GLfloat)-l6,(GLfloat)l1,(GLfloat)(0.5*l4));//側面５
		DrawPolygonSurface((GLfloat)-l6,(GLfloat)l1,(GLfloat)(0.5*l4),
							(GLfloat)l5,(GLfloat)0,(GLfloat)(0.5*l4),
							(GLfloat)l5,(GLfloat)l1,(GLfloat)(0.5*l4));//側面６
		DrawPolygonSurface((GLfloat)-l6,(GLfloat)0,(GLfloat)(0.5*l4),
							(GLfloat)-l6,(GLfloat)-l2,(GLfloat)(0.5*l4),
							(GLfloat)l5,(GLfloat)0,(GLfloat)(0.5*l4));//側面７
		DrawPolygonSurface((GLfloat)-l6,(GLfloat)-l2,(GLfloat)(0.5*l4),
							(GLfloat)l5,(GLfloat)-l3,(GLfloat)(0.5*l4),
							(GLfloat)l5,(GLfloat)0,(GLfloat)(0.5*l4));//側面８

		glPopMatrix();
	glEndList();

	// Thigh（腿）
	glNewList(DisListLegBone[1], GL_COMPILE);
		glTranslated(-LINK2, 0, 0);
	   	SetMaterial( SelectColor(CJoint) );
		DrawCylinder(50, 400);
		SetMaterial( SelectColor(CLink) );
		//DrawBox(0, -100, -100, LINK2, 500, 100);//箱型で近似
		l1=900;
		l2=DTC14;
		l3=400;
		l4=200;
		DrawPolygonSurface((GLfloat)0,(GLfloat)0,(GLfloat)(-0.5*l4),
							(GLfloat)l1, (GLfloat)(l2+l3), (GLfloat)(-0.5*l4),
							(GLfloat)l1, (GLfloat)l2, (GLfloat)(-0.5*l4));//側面１
		DrawPolygonSurface((GLfloat)LINK2, (GLfloat)0, (GLfloat)(-0.5*l4),
							(GLfloat)l1, (GLfloat)l2, (GLfloat)(-0.5*l4),
							(GLfloat)l1, (GLfloat)(l2+l3), (GLfloat)(-0.5*l4));//側面２
		DrawPolygonSurface((GLfloat)0, (GLfloat)0, (GLfloat)(0.5*l4),
							(GLfloat)l1, (GLfloat)l2, (GLfloat)(0.5*l4),
							(GLfloat)l1, (GLfloat)(l2+l3), (GLfloat)(0.5*l4));//側面３
		DrawPolygonSurface((GLfloat)LINK2, (GLfloat)0, (GLfloat)(0.5*l4),
							(GLfloat)l1, (GLfloat)(l2+l3), (GLfloat)(0.5*l4),
							(GLfloat)l1, (GLfloat)l2, (GLfloat)(0.5*l4));//側面４
		DrawPolygonSurface((GLfloat)0,(GLfloat)0,(GLfloat)(-0.5*l4),
							(GLfloat)0,(GLfloat)0,(GLfloat)(0.5*l4),
							(GLfloat)l1, (GLfloat)(l2+l3), (GLfloat)(-0.5*l4));//上面１
		DrawPolygonSurface((GLfloat)0,(GLfloat)0,(GLfloat)(0.5*l4),
							(GLfloat)l1, (GLfloat)(l2+l3), (GLfloat)(0.5*l4),
							(GLfloat)l1, (GLfloat)(l2+l3), (GLfloat)(-0.5*l4));//上面２
		DrawPolygonSurface((GLfloat)LINK2,(GLfloat)0,(GLfloat)(0.5*l4),
							(GLfloat)LINK2,(GLfloat)0,(GLfloat)(-0.5*l4),
							(GLfloat)l1, (GLfloat)(l2+l3), (GLfloat)(-0.5*l4));//上面３
		DrawPolygonSurface((GLfloat)LINK2,(GLfloat)0,(GLfloat)(0.5*l4),
							(GLfloat)l1, (GLfloat)(l2+l3), (GLfloat)(-0.5*l4),
							(GLfloat)l1, (GLfloat)(l2+l3), (GLfloat)(0.5*l4));//上面４
	glEndList();

	 // Shank（脛）
	glNewList(DisListLegBone[2], GL_COMPILE);
	l1=300;
	l2=300;
	l3=200;
		glTranslated(-LINK3, 0, 0);
		SetMaterial( SelectColor(CJoint) );
		DrawCylinder(50, 400);
		SetMaterial( SelectColor(CLink) );
		//DrawBox(0,-100,-100, LINK3, 300, 100);//Boxで近似
		DrawPolygonSurface((GLfloat)0,(GLfloat)0,(GLfloat)(-0.5*l3),
							(GLfloat)l1,(GLfloat)l2,(GLfloat)(-0.5*l3),
							(GLfloat)l1,(GLfloat)0,(GLfloat)(-0.5*l3));//側面１
		DrawPolygonSurface((GLfloat)LINK3,(GLfloat)0,(GLfloat)(-0.5*l3),
							(GLfloat)l1,(GLfloat)0,(GLfloat)(-0.5*l3),
							(GLfloat)l1,(GLfloat)l2,(GLfloat)(-0.5*l3));//側面２
		DrawPolygonSurface((GLfloat)0,(GLfloat)0,(GLfloat)(0.5*l3),
							(GLfloat)l1,(GLfloat)0,(GLfloat)(0.5*l3),
							(GLfloat)l1,(GLfloat)l2,(GLfloat)(0.5*l3));//側面３
		DrawPolygonSurface((GLfloat)LINK3,(GLfloat)0,(GLfloat)(0.5*l3),
							(GLfloat)l1,(GLfloat)l2,(GLfloat)(0.5*l3),
							(GLfloat)l1,(GLfloat)0,(GLfloat)(0.5*l3));//側面４
		DrawPolygonSurface((GLfloat)0,(GLfloat)0,(GLfloat)(-0.5*l3),
							(GLfloat)0,(GLfloat)0,(GLfloat)(0.5*l3),
							(GLfloat)l1,(GLfloat)l2,(GLfloat)(0.5*l3));//上面１
		DrawPolygonSurface((GLfloat)0,(GLfloat)0,(GLfloat)(-0.5*l3),
							(GLfloat)l1,(GLfloat)l2,(GLfloat)(0.5*l3),
							(GLfloat)l1,(GLfloat)l2,(GLfloat)(-0.5*l3));//上面２
		DrawPolygonSurface((GLfloat)LINK3,(GLfloat)0,(GLfloat)(0.5*l3),
							(GLfloat)LINK3,(GLfloat)0,(GLfloat)(-0.5*l3),
							(GLfloat)l1,(GLfloat)l2,(GLfloat)(-0.5*l3));//上面３
		DrawPolygonSurface((GLfloat)LINK3,(GLfloat)0,(GLfloat)(0.5*l3),
							(GLfloat)l1,(GLfloat)l2,(GLfloat)(-0.5*l3),
							(GLfloat)l1,(GLfloat)l2,(GLfloat)(0.5*l3));//上面４
	glEndList();

	 // Foot
	glNewList(DisListLegBone[3] , GL_COMPILE);
		glTranslated(-FOOT, 0, 0);
		SetMaterial( SelectColor(CJoint) );
		DrawCylinder(50, 200);
		SetMaterial( SelectColor(CLink) );
		//DrawBox(0, -250,-250, FOOT, 250, 250);//足裏を四角くする場合．
		glPushMatrix();
		glRotatef((GLfloat)-90,(GLfloat)0,(GLfloat)1,(GLfloat)0);
		glTranslatef((GLfloat)0,(GLfloat)0,(GLfloat)(-0.5*FOOT));
		DrawCylinder(250, 50);//円盤状の足裏
		glPopMatrix();
	glEndList();

	//地図ディスプレイリスト
	//void	DrawCellMap(void);
	//void	DrawWireMap(void);
	//void	DrawPolygonMap(void);
	//void	DrawContourMap(void);


/*	glNewList(DisListMap[0] , GL_COMPILE);
	DrawCellMapOnGridMap();//050920 doi
	glEndList();

	glNewList(DisListMap[1] , GL_COMPILE);
	DrawWireMap();
	glEndList();

	glNewList(DisListMap[2] , GL_COMPILE);
	DrawPolygonMap();
	glEndList();

	glNewList(DisListMap[3] , GL_COMPILE);
	DrawContourMap();
	glEndList();
*/

	IsDisListPrepared = true;

}

/*******************************************************************
**	データソースの設定
*******************************************************************/
void TitanGraphics::SetTitanData(TitanData* titan)
{
	// 引数チェック
	if ( titan == 0)
	{
		fprintf(stderr, "TitanGraphics::SetTitanData() Invalid parameter");
	}

	pTitanData = titan;

	return;
}

/*
void	TitanGraphics::SetGridMap(GridMap* pGridMap_in)//050913 doi
{
	if(pGridMap_in==NULL)
	{
		fprintf(stderr, "TitanGraphics::SetGridMap() Invalid parameter");
	}
	pGridMap=pGridMap_in;
}

void	TitanGraphics::SetCellMap(CellMap* pCellMap_in)//050913 doi
{
	if(pCellMap_in==NULL)
	{
		fprintf(stderr, "TitanGraphics::SetCellMap() Invalid parameter");
	}

	pCellMap=pCellMap_in;
}
*/
/*******************************************************************
**	視点の設定
*******************************************************************/
void TitanGraphics::SetSceneView(void)
{
	switch ( GetViewType() )
	{
		case PERSPECTIVE:
			SetViewPoint(	MouseView.Distance, MouseView.Azimuth, MouseView.Elevation,
							MouseView.ViewCenterPos[0],
							MouseView.ViewCenterPos[1],
							MouseView.ViewCenterPos[2],
							1
						);
			break;

		case TOP:
			SetViewPoint(	30000.0, -180.0, 90.0,
							pTitanData->GetBodyPosition()(1),
							pTitanData->GetBodyPosition()(2),
							pTitanData->GetBodyPosition()(3),
							1
						);
			break;

		case SIDE:
			SetViewPoint(	30000.0, 90.0, 0.0,
							pTitanData->GetBodyPosition()(1),
							pTitanData->GetBodyPosition()(2),
							pTitanData->GetBodyPosition()(3),
							1
						);
			break;

		case FRONT:
			SetViewPoint(	30000.0, 0.0, 0.0,
							pTitanData->GetBodyPosition()(1),
							pTitanData->GetBodyPosition()(2),
							pTitanData->GetBodyPosition()(3),
							1
						);
			break;

		default:
			break;

    }
}

/*******************************************************************
**	地図の描画設定
*******************************************************************/
/*
void TitanGraphics::SetMapDrawPattern(MAPPATTERN pattern)
{
	MapDrawPattern = pattern;

	return;
}
*/
/*******************************************************************
**	セル地図の描画高さを設定
*******************************************************************/
/*
void TitanGraphics::SetCellMapHeight(double height)
{
	// ポインタが空ならなにもしない 050913 doi
	if ( pCellMap==NULL )return;

	pCellMap->SetCellHeight(height);

	return;
}
*/
/*******************************************************************
**	OpenGLのシーンのレンダリング
*******************************************************************/
void TitanGraphics::RenderScene(void)
{
	//if ( wglMakeCurrent(m_hDC, m_hRC) )
	{
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);

		glLoadIdentity();
		//glClearColor( 0.05f , 0.1f , 0.2f , 1.0f );
		glClearColor( 1.0f , 1.0f , 1.0f , 1.0f );
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SetLighting();
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		DrawScene();
		//SwapBuffers( wglGetCurrentDC() );
		return;//091016追加
	}

	//SwapBuffers( wglGetCurrentDC() );
	//wglMakeCurrent(m_hDC, NULL);//DC解放？

	return;
}

/*******************************************************************
**	OpenGLのシーンの描画
*******************************************************************/
void TitanGraphics::DrawScene(void)
{
	// 視点決定
	SetSceneView();

	// ディスプレイリスト初期化
	InitDisplayList();

	// フロア描画
	if ( IsGridDrawn )
		{
			double GAIT_HEIGHT=1200;//140825
		DrawGridFloor(GRID_START_X, GRID_START_Y, GRID_END_X, GRID_END_Y, -1*GAIT_HEIGHT, GRID_DX, GRID_DY);
	}

	// 座標系描画
	if ( IsAxisDrawn )
		DrawCoordinateAxis(3000, 4.0);

	// Titan描画
	DrawTitan();

	// 支持脚多角形描画
	if ( IsSupportPolygonDrawn )
		//DrawSupportPolygon();

	// 地形描画
	if(IsTerrainDrawn)
	{
		//DrawMap();
	}


	return;
}
/*******************************************************************
**	矢印描画 060525 doi
**		DrawArrow
*******************************************************************/
void	TitanGraphics::DrawArrow(double x0_in, double y0_in, double z0_in,
		double vx_in, double vy_in, double vz_in)
{
		//SetMaterial(SelectColor(YELLOW));
		SetMaterial(SelectColor(RED));
		//線だけ
		DrawLine(x0_in, y0_in,z0_in, x0_in+vx_in, y0_in+vy_in,z0_in+vz_in, 5.0);
		/*
		glBegin(GL_LINES);
			glVertex3d(x0_in, y0_in,z0_in);
			glVertex3d(x0_in+vx_in, y0_in+vy_in,z0_in+vz_in);
		glEnd();
		*/
		//三角
		double w=500;
		double l=500;
		Matrix dir(3),z(3),y(3);
		Matrix a(3),b(3),c(3),d(3),e(3);
		dir(1)=vx_in;
		dir(2)=vy_in;
		dir(3)=vz_in;
		dir.Normalize();
		a(1)=x0_in+vx_in;
		a(2)=y0_in+vy_in;
		a(3)=z0_in+vz_in;
		z.LoadZero();z(3)=1;
		y=OuterProduct(z,dir);
		z=OuterProduct(dir,y);
		b=a-l*dir+0.5*w*y;
		c=a-l*dir-0.5*w*y;
		d=a-l*dir+0.5*w*z;
		e=a-l*dir-0.5*w*z;

		glBegin(GL_POLYGON);
			glVertex3d(a(1),a(2),a(3));
			glVertex3d(b(1),b(2),b(3));
			glVertex3d(c(1),c(2),c(3));
			glVertex3d(a(1),a(2),a(3));
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3d(a(1),a(2),a(3));
			glVertex3d(d(1),d(2),d(3));
			glVertex3d(e(1),e(2),e(3));
			glVertex3d(a(1),a(2),a(3));
		glEnd();
}

void	TitanGraphics::DrawArrow(void)
{
	Matrix p(3);
	Matrix dir(3);
	dir=pTitanData->MoveDirection;
	//dir.LoadZero();//debug
	//dir(1)=3000;//debug
	if(dir.Norm()<1.0e-10)return;


		p=pTitanData->GetRobotData().GetBodyPosition();//胴体から矢印を描く
		double x=dir(1);//debug
		double y=dir(2);//debug
		double z=dir(3);//debug
		//printf("Move Dir[%.2f, %.2f]\r",x,y);
		DrawArrow(p(1),p(2),p(3)+2000,dir(1),dir(2),dir(3));

		/*
		for(int i=0;i<4;i++)//脚から矢印を描く
		{
			p=pTitanData->GetRobotData().GetFootPosition(i+1);
			//p(3)=p(3)+FOOT;//足首位置
			p(3)=p(3)+300;//地形に埋もれない高さ
			DrawArrow(p(1),p(2),p(3),dir(1),dir(2),dir(3));
		}
		*/
}


/*******************************************************************
**	Titanの描画
*******************************************************************/
void TitanGraphics::DrawTitan(void)
{
	DrawBody();

	for (int i=0; i<LEG_NUM; i++)
	{
		DrawLeg(i+1);
	}
	//DrawArrow();

	return;
}

/*******************************************************************
**	Titanの本体描画
*******************************************************************/
void TitanGraphics::DrawLift(double LiftAngle, double CircleAngle)
{
	//LiftAngle--リフト傾斜．胴体と平行のときゼロ
	//CircleAngle--追尾回転機構．胴体に垂直のときゼロ
	//単位[deg]
		//リフト 060727memo参照
		double l1,l2,l3,l4,l5,l6;
		double x0,x1,z0,z1;
		double xc,yc,zc;


		x0=195;
		x1=75;
		z0=280+100;
		z1=90;
		l1=1800;
		l2=1150;
		l3=600;
		l4=1000;
		l5=2000;
		l6=500;
		glPushMatrix();
		glTranslatef((GLfloat)(x0),(GLfloat)0,(GLfloat)(z0));

		//回転軸描画
		glPushMatrix();
		glRotatef((GLfloat)90,1,0,0);
		glTranslatef((GLfloat)(0),(GLfloat)0,(GLfloat)(600));
		DrawCylinder(100,800);
		glTranslatef((GLfloat)(0),(GLfloat)0,(GLfloat)(-1200));
		DrawCylinder(100,800);
		glPopMatrix();

		glRotatef((GLfloat)LiftAngle,(GLfloat)0,(GLfloat)1,(GLfloat)0);
		glTranslatef((GLfloat)(x1),(GLfloat)0,(GLfloat)(z1));
		DrawBox(	-l1, -0.5*l5, 0,  0,	-0.5*l6, l2);//箱で近似．右側
		DrawBox(	-l1, 0.5*l6, 0,  0,	0.5*l5, l2);//箱で近似．左側

			//回転機構

			xc=-l1-100;
			yc=500;
			zc=l2+300;
			double lc=200;
			double angle2=CircleAngle*(RAD2DEG);
			glTranslatef((GLfloat)xc,(GLfloat)yc,(GLfloat)zc);
			glRotatef((GLfloat)-angle2,(GLfloat)0,(GLfloat)1,(GLfloat)0);
			glRotatef((GLfloat)90,(GLfloat)0,(GLfloat)1,(GLfloat)0);
			SetMaterial(0.9,0.3,0.3,1.0);
			DrawCylinder(300,30);//円盤
			SetMaterial(0.9,0.9,0.9,1.0);
			DrawCylinder(50,60);//円盤中心
		glPopMatrix();

}
void TitanGraphics::DrawDrill(double DrillAngle)
{
	//angle--傾斜度．胴体と平行でゼロ
	//単位[rad]
		//ドリル 060727memo参照
		double l1,l2,l3,l4,l5,l6;
		double x0,x1,z0,z1;
		x0=1600;
		z0=840;
		l1=865;
		l2=5650;
		l3=630;
		l4=1370;
		l5=200;
		l6=480;
		glPushMatrix();
		glTranslatef((GLfloat)x0,(GLfloat)0,(GLfloat)z0);

			glRotatef((GLfloat)90,1,0,0);//回転軸描画
				DrawCylinder(100,300);
			glRotatef((GLfloat)-90,1,0,0);//元に戻す

		glRotatef(DrillAngle,0,1,0);
		DrawPolygonSurface(0,-0.5*l5,0,		-0.5*l3, -0.5*l5,l6,	0.5*l3, -0.5*l5, l6);//ドリル付け根三角左面
		DrawPolygonSurface(0,0.5*l5,0,		0.5*l3, 0.5*l5,	l6,		-0.5*l3, 0.5*l5, l6);//ドリル付け根三角右面

		DrawBox(l4-l2,-0.5*l5,l6, l4,0.5*l5,l1+0.5*l5);//ドリルのガイド
		double drill_Xoffset=2000;//ドリル中心線を描く長さ
		SetMaterial(1,0,0,1);
		double thick=0.5;//線の太さ
		DrawLine((l4+drill_Xoffset),0,l1, (l4-l2),0,l1,thick);
		/*
		glBegin(GL_LINES);//ドリル中心線
			glVertex3d((GLdouble)(l4+drill_Xoffset),(GLdouble)0,(GLdouble)l1);
			glVertex3d((GLdouble)(l4-l2),(GLdouble)0,(GLdouble)l1);
		glEnd();
		*/
		glPopMatrix();

}

void TitanGraphics::DrawBody(void)
{
	// グローバルから胴体座標への変換行列
	Matrix gAb(4,4);

	glPushMatrix();

		// 胴体座標系に変換
		gAb = pTitanData->GetBodyTransform();
		TransformGLMatrix(gAb);

		//胴体プレート
		SetMaterial( 0.2,0.2,0.9,1 );
		DrawBox(	-0.5*BODY_LENGTH, -0.5*BODY_WIDTH, -0.5*100,
					 0.5*BODY_LENGTH,  0.5*BODY_WIDTH,  0.5*100
				);

		//ウインチ
		double x0=-2000;
		double z0=500;
		double inner_y=458;
		double outer_y=730;
		glPushMatrix();
		glTranslatef((GLfloat)x0,(GLfloat)0,(GLfloat)z0);
		glRotatef((GLfloat)(-90),(GLfloat)1,(GLfloat)0,(GLfloat)0);
		glTranslatef((GLfloat)0,(GLfloat)0,(GLfloat)(0.5*(outer_y+inner_y)));
		DrawCylinder(350, (outer_y-inner_y));
		glTranslatef((GLfloat)0,(GLfloat)0,(GLfloat)(-2*0.5*(outer_y+inner_y)));
		DrawCylinder(350, (outer_y-inner_y));
		glPopMatrix();


		//↓クローラ
		SetMaterial( 0.3,0.3,0.3,1 );
		//SetMaterial( SelectColor(GRAY) );
		double crawler_len=2000;
		double crawler_outer_wid=2500;
		double crawler_inner_wid=1500;
		//SetMaterial( SelectColor(BLACK) );
		DrawBox(	-0.5*crawler_len, -0.5*crawler_outer_wid, -600,
					0.5*crawler_len, -0.5*crawler_inner_wid, -200
				);
		DrawBox(	-0.5*crawler_len, 0.5*crawler_inner_wid, -600,
					0.5*crawler_len, 0.5*crawler_outer_wid, -200
				);

		//リフト
		double CircleAngle=0;//追尾装置傾斜角[deg]
		SetMaterial( 0.4,0.4,0.6,1 );
		//SetMaterial( SelectColor(GRAY) );
		DrawLift(pTitanData->GetEngineAngle()*RAD2DEG,CircleAngle);

		//ドリル
		SetMaterial( SelectColor(SKY) );
		DrawDrill(pTitanData->GetDrillAngle()*RAD2DEG);

	glPopMatrix();

	return;
}

/*******************************************************************
**	Titanの脚描画
*******************************************************************/
void TitanGraphics::DrawLeg(int legID)
{
	// 引数のチェック
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "TitanGraphics::DrawLeg() Invalid prameter\n\n");
		return;
	}

	Matrix trans(DH_NUM, DH_NUM);
	Matrix gAb(DH_NUM, DH_NUM);		// global → body
	Matrix bAl(DH_NUM, DH_NUM);		// body → leg

	gAb = pTitanData->GetBodyTransform();

	glPushMatrix();

		// 胴体座標系に変換
		TransformGLMatrix(gAb);

		// 脚のベース座標系を取得して，脚座標に変換
		int i = legID;
		glPushMatrix();
			//bAl = pTitanData->GetLegTransform(i, 0);
			//TransformGLMatrix(bAl);
			trans = pTitanData->GetLegTransform(i, 1);//060726 doi
			TransformGLMatrix(trans);
			glCallList(DisListLegBone[0]);
		glPopMatrix();

		// hip
		glPushMatrix();
			trans = pTitanData->GetLegTransform(i, 2);
			TransformGLMatrix(trans);
			glCallList(DisListLegBone[1]);
		glPopMatrix();

		 // knee
		glPushMatrix();
			trans = pTitanData->GetLegTransform(i, 3);
			TransformGLMatrix(trans);
			glCallList(DisListLegBone[2]);
		glPopMatrix();

		// foot
		glPushMatrix();
			trans = pTitanData->GetLegTransform(i, 4);
			TransformGLMatrix(trans);
			glCallList(DisListLegBone[3]);
		glPopMatrix();

	glPopMatrix();

	return;
}

/*******************************************************************
**	地図の描画
*******************************************************************/
/*
void TitanGraphics::DrawMap(void)
{
//return;//debug
	// ポインタが空なら描画しない
	if ( pGridMap==NULL )return;

	// 地図が読み込まれていなかったら描画しない
	if ( !pGridMap->IsMapLoaded() )return;

	if ( IsCellMapDrawn )
	{
		DrawCellMap();
		//return;//050911 doi
	}

	//if(IsTerrainDrawn)
	{
		switch (MapDrawPattern)
		{
			case INVISIBLE:
				break;

			case WIRE:
				DrawWireMap();
				break;

			case POLYGON:
				DrawPolygonMap();
				break;

			case CONTOUR:
				DrawContourMap();
				break;

			default:
				break;
		}
	}

	return;
}
*/
/*******************************************************************
**	支持脚多角形(三角形or四角形)の描画
*******************************************************************/
void TitanGraphics::DrawSupportPolygon(void)
{
	int i;
	Robot::LegPhase phase[LEG_NUM];

	for (i=0; i<4; i++)
	{
		phase[i] = pTitanData->GetLegPhase(i+1);
	}

	// 多角形の線分色決定
	SetMaterial( SelectColor(YELLOW) );

	if (( phase[0] == Robot::SUPPORT )&&( phase[1] == Robot::SUPPORT )&&( phase[2] == Robot::SUPPORT )&&( phase[3] == Robot::SUPPORT ))
	{
		DrawLine( pTitanData->GetFootPosition(1), pTitanData->GetFootPosition(2), 2.0);
		DrawLine( pTitanData->GetFootPosition(2), pTitanData->GetFootPosition(4), 2.0);
		DrawLine( pTitanData->GetFootPosition(4), pTitanData->GetFootPosition(3), 2.0);
		DrawLine( pTitanData->GetFootPosition(3), pTitanData->GetFootPosition(1), 2.0);
	}
	else if (( phase[0] == Robot::SWING )&&( phase[1] == Robot::SUPPORT )&&( phase[2] == Robot::SUPPORT )&&( phase[3] == Robot::SUPPORT ))
	//if ( phase[0] == Robot::SWING )
	{
		DrawLine( pTitanData->GetFootPosition(2), pTitanData->GetFootPosition(3), 2.0);
		DrawLine( pTitanData->GetFootPosition(3), pTitanData->GetFootPosition(4), 2.0);
		DrawLine( pTitanData->GetFootPosition(4), pTitanData->GetFootPosition(2), 2.0);
	}
	else if (( phase[0] == Robot::SUPPORT )&&( phase[1] == Robot::SWING )&&( phase[2] == Robot::SUPPORT )&&( phase[3] == Robot::SUPPORT ))
	//else if ( phase[1] == Robot::SWING )
	{
		DrawLine( pTitanData->GetFootPosition(1), pTitanData->GetFootPosition(3), 2.0);
		DrawLine( pTitanData->GetFootPosition(3), pTitanData->GetFootPosition(4), 2.0);
		DrawLine( pTitanData->GetFootPosition(4), pTitanData->GetFootPosition(1), 2.0);
	}
	else if (( phase[0] == Robot::SUPPORT )&&( phase[1] == Robot::SUPPORT )&&( phase[2] == Robot::SWING )&&( phase[3] == Robot::SUPPORT ))
	//else if ( phase[2] == Robot::SWING )
	{
		DrawLine( pTitanData->GetFootPosition(1), pTitanData->GetFootPosition(2), 2.0);
		DrawLine( pTitanData->GetFootPosition(2), pTitanData->GetFootPosition(4), 2.0);
		DrawLine( pTitanData->GetFootPosition(4), pTitanData->GetFootPosition(1), 2.0);
	}
	else if (( phase[0] == Robot::SUPPORT )&&( phase[1] == Robot::SUPPORT )&&( phase[2] == Robot::SUPPORT )&&( phase[3] == Robot::SWING ))
	//else if ( phase[3] == Robot::SWING )
	{
		DrawLine( pTitanData->GetFootPosition(1), pTitanData->GetFootPosition(2), 2.0);
		DrawLine( pTitanData->GetFootPosition(2), pTitanData->GetFootPosition(3), 2.0);
		DrawLine( pTitanData->GetFootPosition(3), pTitanData->GetFootPosition(1), 2.0);
	}
	else

	return;


}

/*******************************************************************
**	グリッドの床を描画
**		引数
**			床の対角線の始点と終点と高さと細かさ
*******************************************************************/
void TitanGraphics::DrawGridFloor(double x0, double y0, double x1, double y1,
								  double z, double dx, double dy)
{
	double x,y;

	Matrix p[2];
	p[0].SetSize(3);
	p[1].SetSize(3);

	p[0](3)=z;
	p[1](3)=z;

	SetMaterial( SelectColor(SKY) );

	// x方向の描画
	for (x=x0; x<=x1; x+=dx)
	{
		p[0](1)=x;
		p[0](2)=y0;
		p[0](3)=z;

		p[1](1)=x;
		p[1](2)=y1;
		p[1](3)=z;

		DrawLine( p[0], p[1], 1.0);
	}

	// y方向の描画
	for (y=y0; y<=y1; y+=dy)
	{
		p[0](1)=x0;
		p[0](2)=y;
		p[0](3)=z;

		p[1](1)=x1;
		p[1](2)=y;
		p[1](3)=z;

		DrawLine(p[0], p[1], 1.0);
	}

	return;
}

/*******************************************************************
**	画面上の操作起点となる点をセット
*******************************************************************/
void TitanGraphics::SetMouseViewPoint(int x, int y)
{
	MouseView.SetCtrlPoint(x, y);

	return;
}

/*******************************************************************
**	マウスのイベントモードを取得
*******************************************************************/
MouseViewCtrl::DragMode TitanGraphics::GetMouseViewDragMode(void)
{
	return ( MouseView.GetDragMode() );
}

/*******************************************************************
**	マウスのイベントモードを設定
*******************************************************************/
void TitanGraphics::SetMouseViewDragMode(MouseViewCtrl::DragMode mode)
{
	MouseView.SetDragMode(mode);

	return;
}

/*******************************************************************
**	マウスのドラッグモードによりViewの変更
*******************************************************************/
void TitanGraphics::MouseViewTransform(int x, int y)
{
	MouseView.TransformView(x, y);

	return;
}

/*******************************************************************
**	視覚情報の直接設定
*******************************************************************/
void TitanGraphics::SetMouseViewAzimuth(double azimuth)
{
	MouseView.SetAzimuth(azimuth);
	return;
}

void TitanGraphics::SetMouseViewElevation(double elevation)
{
	MouseView.SetElevation(elevation);
	return;
}

void TitanGraphics::SetMouseViewDistance(double distance)
{
	MouseView.SetDistance(distance);
	return;
}

/*******************************************************************
**	OpenGL用の座標変換(4x4)
*******************************************************************/
void TitanGraphics::TransformGLMatrix(const Matrix& matrix)
{
	double T[16];

	int k=0;

	// マトリクス要素を配列に格納
	for (int i=1; i<=4; i++)
	{
		for (int j=1; j<=4; j++)
		{
			T[k] = matrix(j,i);
			k++;
		}
	}

	// 現在のマトリクスと掛ける
	glMultMatrixd(T);

	return;
}

/*******************************************************************
**	地図情報を描画するためのヘルパー関数
*******************************************************************/

/*******************************************************************
**	塗り潰しの地図を作る
*******************************************************************/
/*
void TitanGraphics::DrawCellMap(void)
{
	// ポインタが空ならなにもしない 050913 doi
	if ( pCellMap==NULL )return;

	//Displaylistが用意されていればそれを使う．050913 doi
	if(IsDisListPrepared)
	{
	glCallList(DisListMap[0]);
	return;
	}

	int iMax = pCellMap->GetRow();
	int jMax = pCellMap->GetCol();

	double left, right, top, bottom;
	if(!pCellMap->IsMapTransformed())//050910 doi
	{
		return;
	}

	SetMaterial( SelectColor(GREEN) );
	for (int i=2; i<=iMax-1; i++)
	{
		for (int j=2; j<=jMax-1; j++)
		{
			if ( pCellMap->IsPointForbiddenArea((int)i, (int)j) )//050910 doi
			{
				pCellMap->GetLocalRectArea(i, j, &left, &top, &right, &bottom);
				DrawPlaneXY( left, top, right, bottom, pCellMap->GetCellHeight() );
			}
		}
	}

	return;
}
*/
/*******************************************************************
**	地図を線で作る
*******************************************************************/
/*
void TitanGraphics::DrawWireMap(void)
{
	// ポインタが空ならなにもしない 050913 doi
	if ( pGridMap==NULL )return;

	//Displaylistが用意されていればそれを使う．050913 doi
	if(IsDisListPrepared)
	{
	glCallList(DisListMap[2]);
	return;
	}

	int iMax = pGridMap->GetRow();
	int jMax = pGridMap->GetCol();

	// 線を引くx, y方向の始点
	Matrix pi(3);
	Matrix pj(3);

	// 線を引く終点
	Matrix p(3);

	SetMaterial( SelectColor(PURPLE) );

	// 以下のループで描画
	for (int i=1; i<=iMax; i++)
	{
		for (int j=1; j<=jMax; j++)
		{
			// 現在の格子点番号の座標を取得
			p(1) = pGridMap->GetNodePosX(i);
			p(2) = pGridMap->GetNodePosY(j);
			p(3) = (*pGridMap)(i, j);

			// y方向の線描画
			if ( (j-1)>=1 )
			{
				pj(1) = p(1);
				pj(2) = pGridMap->GetNodePosY(j-1);
				pj(3) = (*pGridMap)(i, j-1);
				DrawLine(p, pj, 1.5);
			}

			// x方向の線描画
			if ( (i-1)>=1 )
			{
				pi(1) = pGridMap->GetNodePosX(i-1);
				pi(2) = p(2);
				pi(3) = (*pGridMap)(i-1, j);
				DrawLine(p, pi, 1.5);
			}
		}
	}

	return;
}
*/
/*******************************************************************
**	多面体の地図を作る
*******************************************************************/
/*
void	TitanGraphics::DrawGrayPolygon(Matrix &p1, Matrix &p2, Matrix &p3)
{
	//法線の向きで濃さを変えたポリゴン
	Matrix v1(3),v2(3),v3(3);
	v1=p2-p1;
	v2=p3-p1;
	v3=OuterProduct(v1,v2);
	v3.Normalize();
	double z = v3(3);
	double level=z*z*0.7;
	SetMaterial(level,level,level,1.0);
	DrawPolygonSurface(p1,p2,p3);


}
void TitanGraphics::DrawPolygonMap(void)
{
	// ポインタが空ならなにもしない 050913 doi
	if ( pGridMap==NULL )return;

	//Displaylistが用意されていればそれを使う．
	if(IsDisListPrepared)
	{
	glCallList(DisListMap[2]);
	return;
	}

	int iMax = pGridMap->GetRow();
	int jMax = pGridMap->GetCol();

	// 線を引く参照点
	Matrix pi(3);
	Matrix pj(3);
	Matrix pk(3);

	// 線を引く起点
	Matrix p(3);


	// 以下でポリゴン描画
	for (int i=1; i<=iMax; i++)
	{
		for (int j=1; j<=jMax; j++)
		{
			// 起点の座標計算
			p(1) = pGridMap->GetNodePosX(i);
			p(2) = pGridMap->GetNodePosY(j);
			p(3) = (*pGridMap)(i,j);

			//SetMaterial( SelectColor(PURPLE) );
			SetMaterial( SelectColor(BLACK) );
			// 1つ目の参照点を計算
			if ( (j-1)>=1 )
			{
				pj(1) = p(1);
				pj(2) = pGridMap->GetNodePosY(j-1);
				pj(3) = (*pGridMap)(i,j-1);
				DrawLine(p, pj, 0.1);
			}

			// 2つ目の参照点を計算
			if ( (i-1)>=1 )
			{
				pi(1) = pGridMap->GetNodePosX(i-1);
				pi(2) = p(2);
				pi(3) = (*pGridMap)(i-1,j);
				DrawLine(p, pi, 0.1);
			}

			// 3つ目の参照点を計算
			if ( ( (i-1)>=1 ) && ( (j-1)>=1 ) )
			{
				pk(1) = pGridMap->GetNodePosX(i-1);
				pk(2) = pGridMap->GetNodePosY(j-1);
				pk(3) = (*pGridMap)(i-1, j-1);

				//SetMaterial( SelectColor(BLUE) );
				//DrawPolygonSurface(p, pj, pi);
				//DrawPolygonSurface(pk, pj, pi);
				DrawGrayPolygon(p, pj, pi);
				DrawGrayPolygon(pk, pi, pj);
			}
		}
	}


	return;
}
*/
/*******************************************************************
**	等高線の地図を作る
*******************************************************************/
/*
void TitanGraphics::DrawContourMap(void)
{
	// ポインタが空ならなにもしない 050913 doi
	if ( pGridMap==NULL )return;

	//Displaylistが用意されていればそれを使う．050913 doi
	if(IsDisListPrepared)
	{
	glCallList(DisListMap[2]);
	return;
	}

	return;
}
*/
/*******************************************************************
**	CellMapをGridMapに沿う形で表示
*******************************************************************/
/*
void TitanGraphics::DrawCellMapOnGridMap(void)
{
	double clearance=300;//GridMapから浮かす高さ

	// ポインタが空ならなにもしない 050913 doi
	if ( pCellMap==NULL )return;
	if ( pGridMap==NULL )return;

	//Displaylistが用意されていればそれを使う．050913 doi
	if(IsDisListPrepared)
	{
	glCallList(DisListMap[0]);
	return;
	}

	int iMax = pCellMap->GetRow();
	int jMax = pCellMap->GetCol();

	double left, right, top, bottom;
	if(!pCellMap->IsMapTransformed())//050910 doi
	{
		return;
	}

	double x,y,z;
	SetMaterial( SelectColor(GREEN) );
	for (int i=2; i<=iMax-1; i++)
	{
		for (int j=2; j<=jMax-1; j++)
		{
			if ( pCellMap->IsPointForbiddenArea((int)i, (int)j) )//050910 doi
			{
				x=pCellMap->GetNodePosX(i);
				y=pCellMap->GetNodePosY(j);
				pGridMap->GetPointHeight(x,y,&z);
				pCellMap->GetLocalRectArea(i, j, &left, &top, &right, &bottom);
				DrawPlaneXY( left, top, right, bottom, z+clearance );
			}
		}
	}

	return;
}

*/
