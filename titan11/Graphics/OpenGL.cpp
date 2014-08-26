/*****************************************************************************
**
**	File:	OpenGL.cpp
**
**	説明:	OpenGL関係の描画クラス(Windows専用)
**
**	Created: 2004/02/09(Mon)	Updated: 2005/02/03(Thu)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

#include "OpenGL.h"

/*******************************************************************
**	コンストラクタ
*******************************************************************/
OpenGL::OpenGL()
{
	SetDefault();
}

/*******************************************************************
**	デストラクタ
*******************************************************************/
OpenGL::~OpenGL()
{
}

/*******************************************************************
**	変数をデフォルトにセット
*******************************************************************/
void OpenGL::SetDefault(void)
{
	// レンダリングコンテキストの初期化
	m_hRC = NULL;

	VType = PERSPECTIVE;

	InitEnvironment();

	return;
}

void OpenGL::InitEnvironment(void)
{
	// ライティングの設定
	m_ambientLight0[0] = 1.0f;		// 環境光
	m_ambientLight0[1] = 1.0f;
	m_ambientLight0[2] = 1.0f;
	m_ambientLight0[3] = 0.5f;

	m_diffuseLight0[0] = 1.0f;		// 拡散光
	m_diffuseLight0[1] = 1.0f;
	m_diffuseLight0[2] = 1.0f;
	m_diffuseLight0[3] = 1.0f;

	m_specularLight0[0] = 1.0f;		// 鏡面光
	m_specularLight0[1] = 1.0f;
	m_specularLight0[2] = 1.0f;
	m_specularLight0[3] = 1.0f;

	m_positionLight0[0] = 0.0f;		// 光源位置
	m_positionLight0[1] = -0.5f;
	m_positionLight0[2] = 1.0f;
	m_positionLight0[3] = 1.0f;

	m_directionLight0[0] = 0.0f;	// スポットライト方向
	m_directionLight0[1] = 0.0f;
	m_directionLight0[2] = -1.0f;

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

	materialAmbDiffGray[0] = 0.6f;
	materialAmbDiffGray[1] = 0.6f;
	materialAmbDiffGray[2] = 0.6f;
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
	materialAmbDiffOrange[1] = 0.5f;
	materialAmbDiffOrange[2] = 0.0f;
	materialAmbDiffOrange[3] = 1.0f;

	materialAmbDiffPurple[0] = 1.0f;
	materialAmbDiffPurple[1] = 0.0f;
	materialAmbDiffPurple[2] = 1.0f;
	materialAmbDiffPurple[3] = 1.0f;

	glPolygonMode(GL_FRONT_AND_BACK, GL_POLYGON);//0608009 doi test
	return;
}

/*******************************************************************
**	OpenGLの初期化
*******************************************************************/
bool OpenGL::InitOpenGL(HWND hWnd)
{

	if ( !::IsWindow(hWnd) )
	{
		fprintf(stderr, "OpenGL::CreateOpenGL Invalid Window Handle");
		return false;
	}

	m_hWnd	= hWnd;
	m_hDC	= ::GetDC(hWnd);

	if ( !SetWindowPixelFormat() )
	{
		fprintf(stderr, "OpenGL::SetWindowPixelFormat Error");
		return false;
	}

	if ( !CreateGLContext() )
	{
		fprintf(stderr, "OpenGL::CreateGLContext Error");
		return false;
	}


	return (true);
}

/*******************************************************************
**	OpenGLの破棄
*******************************************************************/
void OpenGL::DestroyOpenGL()
{
	if(wglGetCurrentContext())
		wglMakeCurrent(NULL, NULL);

	if(m_hRC)
	{
		wglDeleteContext(m_hRC);
		m_hRC = NULL;
	}

	::ReleaseDC(m_hWnd, m_hDC);
	return;
}

/*******************************************************************
**	ピクセルフォーマットの設定
**		OpenGLの初期化に関係
*******************************************************************/
bool OpenGL::SetWindowPixelFormat(void)
{
/*	PIXELFORMATDESCRIPTOR pfd;

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);	// この構造体のサイズ
	pfd.nVersion = 1;							// Version number : must be 1
	pfd.dwFlags = PFD_DRAW_TO_WINDOW |			// ウィンドウへの描画をサポート
				  PFD_SUPPORT_OPENGL |			// OpenGLのサポート
				  PFD_DOUBLEBUFFER |			// ダブルバッファ処理
				  PFD_STEREO_DONTCARE;			// monoscopic か stereoscopicをサポート
	pfd.iPixelType = PFD_TYPE_RGBA;				// RGBA タイプ
	pfd.cColorBits = 32;						// 利用できるカラーの数を決める
	pfd.cRedBits = 8;							// RGBAカラーバッファの赤ビット数
	pfd.cRedShift = 16;							// RGBAカラーバッファの赤ビットのためのシフトカウント
	pfd.cGreenBits = 8;							// RGBAカラーバッファの緑ビット数
	pfd.cGreenShift = 8;						// RGBAカラーバッファの緑ビットのためのシフトカウント
	pfd.cBlueBits = 8;							// RGBAカラーバッファの青ビット数
	pfd.cBlueShift = 0;							// RGBAカラーバッファの青ビットのためのシフトカウント
	pfd.cAlphaBits = 0;							// RGBAカラーバッファのアルファビット数
	pfd.cAlphaShift = 0;						// RGBAカラーバッファのアルファビットのためのシフトカウント
	pfd.cAccumBits = 64;						// アキュームレーションバッファのピクセル当りのビット数
	pfd.cAccumRedBits = 16;						// アキュームレーションバッファのピクセル当りの赤ビット数
	pfd.cAccumGreenBits = 16;					// アキュームレーションバッファのピクセル当りの緑ビット数
	pfd.cAccumBlueBits = 16;					// アキュームレーションバッファのピクセル当りの青ビット数
	pfd.cAccumAlphaBits = 0;					// アキュームレーションバッファのピクセル当りのアルファビット数
	pfd.cDepthBits = 32;						// デプスバッファのピクセル当りのビット数
	pfd.cStencilBits = 8;						// ステンシルバッファのピクセル当りのビット数
	pfd.cAuxBuffers = 0;						// 補助バッファの数. Win32上ではサポート外
	pfd.iLayerType = PFD_MAIN_PLANE;			// レイヤタイプの記述. 現在は使用されてない
	pfd.bReserved = 0;							// オーバーレイとアンダーレイのplaneの数. (0でなければならない)
	pfd.dwLayerMask = 0;						// レイヤマスクの記述. 現在では使用されていない
	pfd.dwVisibleMask = 0;						// ビジブルマスクの設定
	pfd.dwDamageMask = 0;						// ダメージマスクの設定. 現在では使用されていない

	// 要求されたピクセルフォーマットに最も近いピクセルフォーマットを返す
	int pixelFormat = ChoosePixelFormat(m_hDC, &pfd);

	if (pixelFormat == 0)		// Choose default
	{
		pixelFormat = 1;

		// 指定したピクセルフォーマットに関する情報を得る
		if ( DescribePixelFormat(m_hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd) == 0 )
			return false;
	}

	// 指定したデバイスコンテキストのピクセルフォーマットを設定する
	if ( !SetPixelFormat(m_hDC, pixelFormat, &pfd) )
		return false;

*/	return true;

}

/*******************************************************************
**	コンテキストの設定
**		OpenGLの初期化に関係
*******************************************************************/
bool OpenGL::CreateGLContext(void)
{
	// OpenGLのレンダリングコンテキストを作成
	m_hRC = wglCreateContext(m_hDC);
	if ( !m_hRC )	// 作成不可
		return false;

	// 現在のレンダリングコンテキストにセット
	if ( !wglMakeCurrent(m_hDC, m_hRC) )
		return false;

	return true;
}

/*******************************************************************
**	OpenGLのシーンのレンダリング
*******************************************************************/
void OpenGL::RenderScene(void)
{
/*
	if ( wglMakeCurrent(m_hDC, m_hRC) )
	{
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);

		glLoadIdentity();
		glClearColor( 0.0f , 0.0f , 0.5f , 1.0f );	// Back Ground is White
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SetLighting();
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		DrawScene();

	}

	SwapBuffers(wglGetCurrentDC());
	wglMakeCurrent(m_hDC, NULL);
*/
	return;
}

/*******************************************************************
**	オブジェクトで対象とするシーンの描画
*******************************************************************/
void OpenGL::DrawScene(void)
{

	glFlush();

	return;
}

/*******************************************************************
**	ViewTypeの設定
*******************************************************************/
void OpenGL::SetViewType(OpenGL::ViewType type)
{
	VType = type;

	return;
}

/*******************************************************************
**	シーンの視点設定
*******************************************************************/
void OpenGL::SetSceneView(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glFrustum(-1.0, 1.0, -1.0, 1.0, 2.0, 8.0);

	glTranslated(0.0, 0.0, -5.5);

	switch (VType)
	{
		case PERSPECTIVE:
			glRotated(30.0, 1.0, 0.0, 0.0);
			break;

		case TOP:
			glRotated(60.0, 1.0, 0.0, 0.0);
			break;

		case SIDE:
			glRotated(-90.0, 1.0, 0.0, 0.0);
			break;

		case FRONT:
			break;

		default:
			break;

    }

	return;
}

void OpenGL::SetViewPoint(double distance, double azimuth, double elevation,
						  double x, double y, double z, double aspect_ratio)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluPerspective( (GLdouble)20.0, (GLdouble)aspect_ratio, (GLdouble)(distance*0.1), (GLdouble)(distance*1.5) );

	double theta = azimuth;
	double phi = elevation;

	double eye_x,eye_y,eye_z;
	double center_x,center_y,center_z;
	double up_x,up_y,up_z;

	eye_x = distance*cos( phi*PI/180 )*cos( theta*PI/180 );
	eye_y = distance*cos( phi*PI/180 )*sin( theta*PI/180 );
	eye_z = distance*sin( phi*PI/180 );

	center_x = x;
	center_y = y;
	center_z = z;

	up_x = distance*cos( (phi+90)*PI/180 )*cos( theta*PI/180 );
	up_y = distance*cos( (phi+90)*PI/180 )*sin( theta*PI/180 );
	up_z = distance*sin( (phi+90)*PI/180 );

	eye_x += center_x;
	eye_y += center_y;
	eye_z += center_z;

	gluLookAt( (GLdouble)eye_x,		(GLdouble)eye_y,	(GLdouble)eye_z,
			   (GLdouble)center_x,	(GLdouble)center_y,	(GLdouble)center_z,
			   (GLdouble)up_x,		(GLdouble)up_y,		(GLdouble)up_z );

	return;
}

/*******************************************************************
**	ライティングの設定
*******************************************************************/
void OpenGL::SetLighting(void)
{
	glLightfv(GL_LIGHT0,	GL_AMBIENT,			m_ambientLight0);
	glLightfv(GL_LIGHT0,	GL_DIFFUSE,			m_diffuseLight0);
	glLightfv(GL_LIGHT0,	GL_SPECULAR,		m_specularLight0);
	glLightfv(GL_LIGHT0,	GL_POSITION,		m_positionLight0);
	glLightf(GL_LIGHT0,		GL_SPOT_CUTOFF,		45.0f);
	glLightfv(GL_LIGHT0,	GL_SPOT_DIRECTION,	m_directionLight0);
	glLightf(GL_LIGHT0,		GL_SPOT_EXPONENT,	20.0f);

	return;
}

/*******************************************************************
**	マテリアルカラーの設定
**		RGBとalpha値の入力
*******************************************************************/
void OpenGL::SetMaterial(double red, double green, double blue, double alpha)
{

	GLfloat materialAmbDiff[] = {(GLfloat)red, (GLfloat)green, (GLfloat)blue, (GLfloat)alpha};
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbDiff);

	GLfloat materialSpecular[] = {0.5f, 0.5f, 0.5f, 0.5f};
	glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);

	return;
}

void OpenGL::SetMaterial(GLfloat* materialAmbDiffColor)
{
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbDiffColor);
	return;
}

/*******************************************************************
**	マテリアルの色を取得
*******************************************************************/
GLfloat* OpenGL::SelectColor(COLOR color)
{
	GLfloat* ptr;

	switch (color)
	{
		case RED:
			ptr = materialAmbDiffRed;
			break;

		case GREEN:
			ptr = materialAmbDiffGreen;
			break;

		case BLUE:
			ptr = materialAmbDiffBlue;
			break;

		case YELLOW:
			ptr = materialAmbDiffYellow;
			break;

		case GRAY:
			ptr = materialAmbDiffGray;
			break;

		case BLACK:
			ptr = materialAmbDiffBlack;
			break;

		case WHITE:
			ptr = materialAmbDiffWhite;
			break;

		case SKY:
			ptr = materialAmbDiffSky;
			break;

		case ORANGE:
			ptr = materialAmbDiffOrange;
			break;

		case PURPLE:
			ptr = materialAmbDiffPurple;
			break;

		default:
			break;
	}

	return ptr;

}

//////////////////////////////////////////////////////////////////////
////////// 3Dモデリング //////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/*******************************************************************
**	座標軸の描画
**		width			: 座標軸の幅
**		length			: 座標軸の長さ
*******************************************************************/
void OpenGL::DrawCoordinateAxis(double length, const double width)
{
	glPushMatrix();
		// x軸の描画(赤)
		/////////////////////////////////////////////////////////////////////////////
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbDiffRed );
		glLineWidth( (GLfloat)width );
		glBegin(GL_LINES);
			glVertex3f( 0.0f,	0.0f,	0.0f );
			glVertex3f( (GLfloat)length,	0.0f,	0.0f );
		glEnd();

		glTranslatef( (GLfloat)length, 0.0f, 0.0f );
		DrawSphere(150.0);
	glPopMatrix();

	glPushMatrix();
		// y軸の描画(緑)
		/////////////////////////////////////////////////////////////////////////////
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbDiffGreen);
		glLineWidth( (GLfloat)width );
		glBegin(GL_LINES);
			glVertex3f( 0.0f,	0.0f,	0.0f );
			glVertex3f( 0.0f,	(GLfloat)length,	0.0f );
		glEnd();

		glTranslatef( 0.0f, (GLfloat)length, 0.0f );
		DrawSphere(150.0);
	glPopMatrix();

	glPushMatrix();
		// z軸の描画(青)
		/////////////////////////////////////////////////////////////////////////////
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbDiffBlue);
		glLineWidth( (GLfloat)width );
		glBegin(GL_LINES);
			glVertex3f( 0.0f,	0.0f,	0.0f );
			glVertex3f( 0.0f,	0.0f,	(GLfloat)length );
		glEnd();

		glTranslatef( 0.0f, 0.0f, (GLfloat)length );
		DrawSphere(150.0);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbDiffYellow);
	DrawSphere(200.0);

	return;
}

/*******************************************************************
**	ポリゴンモデリング(法線も計算)
**		添字(0, 1, 2)の順にCW(時計回り)で入力
**		各頂点の座標を入力
*******************************************************************/
void OpenGL::DrawPolygonSurface(double x0, double y0, double z0,
								double x1, double y1, double z1,
								double x2, double y2, double z2)
{

	double norm;
	double x, y, z;
	double vx1,vy1,vz1,vx2,vy2,vz2;

	// ベクトルの差を計算
	vx1 = x0 - x1;
	vy1 = y0 - y1;
	vz1 = z0 - z1;

	vx2 = x2 - x1;
	vy2 = y2 - y1;
	vz2 = z2 - z1;

	// 外積計算
	OuterProduct(vx1, vy1, vz1, vx2, vy2, vz2, &x, &y, &z);
	norm = sqrt(x*x + y*y + z*z);

	if ( norm == 0)
	{
		fprintf(stderr, "OpenGL:: DrawPolygonSurface Error\n\n");
		return;
	}

	x = x/norm;
	y = y/norm;
	z = z/norm;

	glBegin(GL_POLYGON);
		glNormal3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );
		glVertex3f( (GLfloat)x0, (GLfloat)y0, (GLfloat)z0 );
		glVertex3f( (GLfloat)x1, (GLfloat)y1, (GLfloat)z1);
		glVertex3f( (GLfloat)x2, (GLfloat)y2, (GLfloat)z2);
	glEnd();

	return;
}

void OpenGL::DrawPolygonSurface(const Matrix& p, const Matrix& q, const Matrix& r)
{
	if ( (p.GetRow() != 3) || (p.GetCol() != 1) ||
		 (q.GetRow() != 3) || (q.GetCol() != 1) ||
		 (r.GetRow() != 3) || (r.GetCol() != 1) )
	{
		fprintf(stderr, "OpenGL::DrawPolygonSurface Invalid Matrix\n\n");
		return;
	}

	DrawPolygonSurface( p(1), p(2), p(3), q(1), q(2), q(3), r(1), r(2), r(3) );

	return;
}

/*******************************************************************
**	円柱描画
*******************************************************************/
void OpenGL::DrawCylinder(double radius, double height)
{
	DrawCylinder2(radius,height);
	return;

	// quadric object を一つ生成する
    GLUquadricObj *quad = gluNewQuadric();


    // 面の塗り潰しを指定する（線画ではなく陰影をつけた円柱を描く
    gluQuadricDrawStyle(quad, GLU_FILL);

    // スムースシェーディングを行うよう設定する
    gluQuadricNormals(quad, GLU_SMOOTH);

	glPushMatrix();
		glTranslated( (GLdouble)0,0, (GLdouble)(-0.5*height) );
		gluCylinder(quad, (GLdouble)radius, (GLdouble)radius, (GLdouble)height, (GLint)16, (GLint)1 );
		gluDisk(quad, (GLdouble)0, (GLdouble)radius, (GLint)16, (GLint)1 );
		glTranslated( (GLdouble)0,0, (GLdouble)height );
		gluDisk(quad, (GLdouble)0, (GLdouble)radius, (GLint)16, (GLint)1 );
	glPopMatrix();

	return;
}
void OpenGL::DrawCylinder2(double radius, double height)
{
	//多角形で近似して円筒を描く
	double th=0;
	double div=8;//分割数
	double dth=2*PI/div;
	double x,y,z;
	int i;

	glPushMatrix();
		glTranslated( (GLdouble)0,(GLdouble)0, (GLdouble)(-0.5*height) );
	//上面
	glBegin(GL_TRIANGLE_FAN);
		glNormal3d((GLdouble)0,(GLdouble)0,(GLdouble)1);
		for(i=0;i<div;i++)
		{
			th=dth*i;
			x=radius*cos(th);
			y=radius*sin(th);
			z=height;
			glVertex3d((GLdouble)x,(GLdouble)y,(GLdouble)z);
		}
	glEnd();

	//側面
		for(i=0;i<div;i++)
		{
			th=dth*i;
			//glBegin(GL_POLYGON);
			glBegin(GL_TRIANGLE_FAN);
			//glBegin(GL_TRIANGLE_STRIP);
			glNormal3d((GLdouble)cos(th),(GLdouble)sin(th),(GLdouble)0);
			z=0;
			x=radius*cos(th);
			y=radius*sin(th);
			glVertex3d((GLdouble)x,(GLdouble)y,(GLdouble)z);
			x=radius*cos(th+dth);
			y=radius*sin(th+dth);
			glVertex3d((GLdouble)x,(GLdouble)y,(GLdouble)z);
			z=height;
			glVertex3d((GLdouble)x,(GLdouble)y,(GLdouble)z);
			x=radius*cos(th);
			y=radius*sin(th);
			glVertex3d((GLdouble)x,(GLdouble)y,(GLdouble)z);
			z=0;
			glVertex3d((GLdouble)x,(GLdouble)y,(GLdouble)z);
			glEnd();
		}


	//下面
	glBegin(GL_TRIANGLE_FAN);
	//glBegin(GL_POLYGON);
		glNormal3d((GLdouble)0,(GLdouble)0,(GLdouble)-1);
		//for(i=0;i<div;i++)
		for(i=div-1;i>=0;i--)
		{
			th=dth*i;
			x=radius*cos(th);
			y=radius*sin(th);
			z=0;
			glVertex3d((GLdouble)x,(GLdouble)y,(GLdouble)z);
		}
	glEnd();

		//gluCylinder(quad, (GLdouble)radius, (GLdouble)radius, (GLdouble)height, (GLint)16, (GLint)1 );
		//gluDisk(quad, (GLdouble)0, (GLdouble)radius, (GLint)16, (GLint)1 );
		//glTranslated( (GLdouble)0,0, (GLdouble)height );
		//gluDisk(quad, (GLdouble)0, (GLdouble)radius, (GLint)16, (GLint)1 );
	glPopMatrix();

	return;
}

/*******************************************************************
**	円柱描画
*******************************************************************/
void OpenGL::DrawSphere(double radius)
{

	// quadric object を一つ生成する */
    GLUquadricObj *quad = gluNewQuadric();

    // 面の塗り潰しを指定する(線画ではなく陰影をつけた円柱を描く)
    gluQuadricDrawStyle(quad, GLU_FILL);

    // スムースシェーディングを行うよう設定する
    gluQuadricNormals(quad, GLU_SMOOTH);

    // 球面
	gluSphere(quad, (GLdouble)radius, (GLint)16, (GLint)16 );
	return;
}

/*******************************************************************
**	線描画
**		引数: 始点と終点の座標, 線の太さ
*******************************************************************/
void OpenGL::DrawLine(double x0, double y0, double z0,
					  double x1, double y1, double z1, double width)
{
	// 線の太さ決定
	glLineWidth( (GLfloat)width );

	// 線の描画
	glBegin(GL_LINES);
		glVertex3f( (GLfloat)x0, (GLfloat)y0, (GLfloat)z0 );
		glVertex3f( (GLfloat)x1, (GLfloat)y1, (GLfloat)z1 );
	glEnd();

	return;
}

void OpenGL::DrawLine(const Matrix& p, const Matrix& q, double width)
{
	if ( (p.GetRow() != 3) || (p.GetCol() != 1) || (q.GetRow() != 3) || (q.GetCol() != 1) )
	{
		fprintf(stderr, "OpenGL::DrawLine Invalid Matrix");
		return;
	}

	DrawLine( p(1), p(2), p(3), q(1), q(2), q(3), width );

	return;
}

/*******************************************************************
**	Box描画
**		引数: 対角線上の頂点
*******************************************************************/
void OpenGL::DrawBox(double x0, double y0, double z0, double x1, double y1, double z1)
{
	// 第1面描画(x-y平面に平行な面)
	//////////////////////////////////
	glBegin(GL_POLYGON);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f( (GLfloat)x0, (GLfloat)y1, (GLfloat)z1 );
		glVertex3f( (GLfloat)x0, (GLfloat)y0, (GLfloat)z1 );
		glVertex3f( (GLfloat)x1, (GLfloat)y0, (GLfloat)z1 );
		glVertex3f( (GLfloat)x1, (GLfloat)y1, (GLfloat)z1 );
	glEnd();

	// 第2面描画(y-z平面に平行な面)
	//////////////////////////////////
	glBegin(GL_POLYGON);
		glNormal3f(1.0f, 0.0f, 0.0f);
		glVertex3f( (GLfloat)x1, (GLfloat)y1, (GLfloat)z1 );
		glVertex3f( (GLfloat)x1, (GLfloat)y0, (GLfloat)z1 );
		glVertex3f( (GLfloat)x1, (GLfloat)y0, (GLfloat)z0 );
		glVertex3f( (GLfloat)x1, (GLfloat)y1, (GLfloat)z0 );
	glEnd();

	// 第3面描画(x-y平面に平行な面)
	//////////////////////////////////
	glBegin(GL_POLYGON);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f( (GLfloat)x1, (GLfloat)y1, (GLfloat)z0 );
		glVertex3f( (GLfloat)x1, (GLfloat)y0, (GLfloat)z0 );
		glVertex3f( (GLfloat)x0, (GLfloat)y0, (GLfloat)z0 );
		glVertex3f( (GLfloat)x0, (GLfloat)y1, (GLfloat)z0 );
	glEnd();

	// 第4面描画(y-z平面に平行な面)
	//////////////////////////////////
	glBegin(GL_POLYGON);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f( (GLfloat)x0, (GLfloat)y1, (GLfloat)z0 );
		glVertex3f( (GLfloat)x0, (GLfloat)y0, (GLfloat)z0 );
		glVertex3f( (GLfloat)x0, (GLfloat)y0, (GLfloat)z1 );
		glVertex3f( (GLfloat)x0, (GLfloat)y1, (GLfloat)z1 );
	glEnd();

	// 第5面描画(x-z平面に平行な面)
	//////////////////////////////////
	glBegin(GL_POLYGON);
		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3f( (GLfloat)x0, (GLfloat)y0, (GLfloat)z1 );
		glVertex3f( (GLfloat)x0, (GLfloat)y0, (GLfloat)z0 );
		glVertex3f( (GLfloat)x1, (GLfloat)y0, (GLfloat)z0 );
		glVertex3f( (GLfloat)x1, (GLfloat)y0, (GLfloat)z1 );
	glEnd();

	// 第6面描画(x-z平面に平行な面)
	//////////////////////////////////
	glBegin(GL_POLYGON);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f( (GLfloat)x0, (GLfloat)y1, (GLfloat)z0 );
		glVertex3f( (GLfloat)x0, (GLfloat)y1, (GLfloat)z1 );
		glVertex3f( (GLfloat)x1, (GLfloat)y1, (GLfloat)z1 );
		glVertex3f( (GLfloat)x1, (GLfloat)y1, (GLfloat)z0 );
	glEnd();

	return;
}

void OpenGL::DrawBox(const Matrix& p, const Matrix& q)
{
	if ( (p.GetRow() != 3) || (p.GetCol() != 1) || (q.GetRow() != 3) || (q.GetCol() != 1) )
	{
		fprintf(stderr, "OpenGL::DrawBox Invalid Matrix");
		return;
	}

	DrawBox( p(1), p(2), p(3), q(1), q(2), q(3) );

	return;

}

/*******************************************************************
**	Frame描画(平面と4隅の垂線)
**		引数: 対角線上の頂点
*******************************************************************/
void OpenGL::DrawFrame(double x0, double y0, double z0, double x1, double y1, double z1)
{
	// 色をグレーに設定
	glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,materialAmbDiffGray);
	// 線幅決定
	glLineWidth(4.0f);

	// 平面描画
	glBegin(GL_LINE_LOOP);
		glVertex3f( (GLfloat)x1, (GLfloat)y1, (GLfloat)z0 );
		glVertex3f( (GLfloat)x1, (GLfloat)y0, (GLfloat)z0 );
		glVertex3f( (GLfloat)x0, (GLfloat)y0, (GLfloat)z0 );
		glVertex3f( (GLfloat)x0, (GLfloat)y1, (GLfloat)z0 );
	glEnd();

	// 以下で垂線描画
	glBegin(GL_LINES);
		glVertex3f( (GLfloat)x1, (GLfloat)y0, (GLfloat)z1 );
		glVertex3f( (GLfloat)x1, (GLfloat)y0, (GLfloat)z0 );
	glEnd();

	glBegin(GL_LINES);
		glVertex3f( (GLfloat)x0, (GLfloat)y0, (GLfloat)z0 );
		glVertex3f( (GLfloat)x0, (GLfloat)y0, (GLfloat)z1 );
	glEnd();

	glBegin(GL_LINES);
		glVertex3f( (GLfloat)x1, (GLfloat)y1, (GLfloat)z0 );
		glVertex3f( (GLfloat)x1, (GLfloat)y1, (GLfloat)z1 );
	glEnd();

	glBegin(GL_LINES);
		glVertex3f( (GLfloat)x0, (GLfloat)y1, (GLfloat)z0 );
		glVertex3f( (GLfloat)x0, (GLfloat)y1, (GLfloat)z1 );
	glEnd();

	return;
}

void OpenGL::DrawFrame(const Matrix& p, const Matrix& q)
{
	if ( (p.GetRow() != 3) || (p.GetCol() != 1) || (q.GetRow() != 3) || (q.GetCol() != 1) )
	{
		fprintf(stderr, "OpenGL::DrawFrame Invalid Matrix");
		return;
	}

	DrawFrame( p(1), p(2), p(3), q(1), q(2), q(3) );

	return;

}

//////////////////////////////////////////////////////////////////////
////////// 2Dモデリング //////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/*******************************************************************
**	平面板描画(対角線の頂点と高さ)
*******************************************************************/
void OpenGL::DrawPlaneXY(double x0, double y0, double x1, double y1, double z)
{
	glBegin(GL_POLYGON);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f( (GLfloat)x0, (GLfloat)y0, (GLfloat)z );
		glVertex3f( (GLfloat)x0, (GLfloat)y1, (GLfloat)z );
		glVertex3f( (GLfloat)x1, (GLfloat)y1, (GLfloat)z );
		glVertex3f( (GLfloat)x1, (GLfloat)y0, (GLfloat)z );
	glEnd();

	return;
}

void OpenGL::DrawPlaneYZ(double y0, double z0, double y1, double z1, double x)
{
	glBegin(GL_POLYGON);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f( (GLfloat)x, (GLfloat)y0, (GLfloat)z0 );
		glVertex3f( (GLfloat)x, (GLfloat)y0, (GLfloat)z1 );
		glVertex3f( (GLfloat)x, (GLfloat)y1, (GLfloat)z1 );
		glVertex3f( (GLfloat)x, (GLfloat)y1, (GLfloat)z0 );
	glEnd();

	return;
}

void OpenGL::DrawPlaneZX(double z0, double x0, double z1, double x1, double y)
{
	glBegin(GL_POLYGON);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f( (GLfloat)x0, (GLfloat)y, (GLfloat)z0 );
		glVertex3f( (GLfloat)x0, (GLfloat)y, (GLfloat)z1 );
		glVertex3f( (GLfloat)x1, (GLfloat)y, (GLfloat)z1 );
		glVertex3f( (GLfloat)x1, (GLfloat)y, (GLfloat)z0 );
	glEnd();

	return;
}
