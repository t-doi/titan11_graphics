/*****************************************************************************
**
**	File:	OpenGL.cpp
**
**	����:	OpenGL�֌W�̕`��N���X(Windows��p)
**
**	Created: 2004/02/09(Mon)	Updated: 2005/02/03(Thu)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

#include "OpenGL.h"

/*******************************************************************
**	�R���X�g���N�^
*******************************************************************/
OpenGL::OpenGL()
{
	SetDefault();
}

/*******************************************************************
**	�f�X�g���N�^
*******************************************************************/
OpenGL::~OpenGL()
{
}

/*******************************************************************
**	�ϐ����f�t�H���g�ɃZ�b�g
*******************************************************************/
void OpenGL::SetDefault(void)
{
	// �����_�����O�R���e�L�X�g�̏�����
	m_hRC = NULL;

	VType = PERSPECTIVE;

	InitEnvironment();

	return;
}

void OpenGL::InitEnvironment(void)
{
	// ���C�e�B���O�̐ݒ�
	m_ambientLight0[0] = 1.0f;		// ����
	m_ambientLight0[1] = 1.0f;
	m_ambientLight0[2] = 1.0f;
	m_ambientLight0[3] = 0.5f;

	m_diffuseLight0[0] = 1.0f;		// �g�U��
	m_diffuseLight0[1] = 1.0f;
	m_diffuseLight0[2] = 1.0f;
	m_diffuseLight0[3] = 1.0f;

	m_specularLight0[0] = 1.0f;		// ���ʌ�
	m_specularLight0[1] = 1.0f;
	m_specularLight0[2] = 1.0f;
	m_specularLight0[3] = 1.0f;

	m_positionLight0[0] = 0.0f;		// �����ʒu
	m_positionLight0[1] = -0.5f;
	m_positionLight0[2] = 1.0f;
	m_positionLight0[3] = 1.0f;

	m_directionLight0[0] = 0.0f;	// �X�|�b�g���C�g����
	m_directionLight0[1] = 0.0f;
	m_directionLight0[2] = -1.0f;

	// �}�e���A���̐F�̐ݒ�
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
**	OpenGL�̏�����
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
**	OpenGL�̔j��
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
**	�s�N�Z���t�H�[�}�b�g�̐ݒ�
**		OpenGL�̏������Ɋ֌W
*******************************************************************/
bool OpenGL::SetWindowPixelFormat(void)
{
/*	PIXELFORMATDESCRIPTOR pfd;

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);	// ���̍\���̂̃T�C�Y
	pfd.nVersion = 1;							// Version number : must be 1
	pfd.dwFlags = PFD_DRAW_TO_WINDOW |			// �E�B���h�E�ւ̕`����T�|�[�g
				  PFD_SUPPORT_OPENGL |			// OpenGL�̃T�|�[�g
				  PFD_DOUBLEBUFFER |			// �_�u���o�b�t�@����
				  PFD_STEREO_DONTCARE;			// monoscopic �� stereoscopic���T�|�[�g
	pfd.iPixelType = PFD_TYPE_RGBA;				// RGBA �^�C�v
	pfd.cColorBits = 32;						// ���p�ł���J���[�̐������߂�
	pfd.cRedBits = 8;							// RGBA�J���[�o�b�t�@�̐ԃr�b�g��
	pfd.cRedShift = 16;							// RGBA�J���[�o�b�t�@�̐ԃr�b�g�̂��߂̃V�t�g�J�E���g
	pfd.cGreenBits = 8;							// RGBA�J���[�o�b�t�@�̗΃r�b�g��
	pfd.cGreenShift = 8;						// RGBA�J���[�o�b�t�@�̗΃r�b�g�̂��߂̃V�t�g�J�E���g
	pfd.cBlueBits = 8;							// RGBA�J���[�o�b�t�@�̐r�b�g��
	pfd.cBlueShift = 0;							// RGBA�J���[�o�b�t�@�̐r�b�g�̂��߂̃V�t�g�J�E���g
	pfd.cAlphaBits = 0;							// RGBA�J���[�o�b�t�@�̃A���t�@�r�b�g��
	pfd.cAlphaShift = 0;						// RGBA�J���[�o�b�t�@�̃A���t�@�r�b�g�̂��߂̃V�t�g�J�E���g
	pfd.cAccumBits = 64;						// �A�L���[�����[�V�����o�b�t�@�̃s�N�Z������̃r�b�g��
	pfd.cAccumRedBits = 16;						// �A�L���[�����[�V�����o�b�t�@�̃s�N�Z������̐ԃr�b�g��
	pfd.cAccumGreenBits = 16;					// �A�L���[�����[�V�����o�b�t�@�̃s�N�Z������̗΃r�b�g��
	pfd.cAccumBlueBits = 16;					// �A�L���[�����[�V�����o�b�t�@�̃s�N�Z������̐r�b�g��
	pfd.cAccumAlphaBits = 0;					// �A�L���[�����[�V�����o�b�t�@�̃s�N�Z������̃A���t�@�r�b�g��
	pfd.cDepthBits = 32;						// �f�v�X�o�b�t�@�̃s�N�Z������̃r�b�g��
	pfd.cStencilBits = 8;						// �X�e���V���o�b�t�@�̃s�N�Z������̃r�b�g��
	pfd.cAuxBuffers = 0;						// �⏕�o�b�t�@�̐�. Win32��ł̓T�|�[�g�O
	pfd.iLayerType = PFD_MAIN_PLANE;			// ���C���^�C�v�̋L�q. ���݂͎g�p����ĂȂ�
	pfd.bReserved = 0;							// �I�[�o�[���C�ƃA���_�[���C��plane�̐�. (0�łȂ���΂Ȃ�Ȃ�)
	pfd.dwLayerMask = 0;						// ���C���}�X�N�̋L�q. ���݂ł͎g�p����Ă��Ȃ�
	pfd.dwVisibleMask = 0;						// �r�W�u���}�X�N�̐ݒ�
	pfd.dwDamageMask = 0;						// �_���[�W�}�X�N�̐ݒ�. ���݂ł͎g�p����Ă��Ȃ�

	// �v�����ꂽ�s�N�Z���t�H�[�}�b�g�ɍł��߂��s�N�Z���t�H�[�}�b�g��Ԃ�
	int pixelFormat = ChoosePixelFormat(m_hDC, &pfd);

	if (pixelFormat == 0)		// Choose default
	{
		pixelFormat = 1;

		// �w�肵���s�N�Z���t�H�[�}�b�g�Ɋւ�����𓾂�
		if ( DescribePixelFormat(m_hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd) == 0 )
			return false;
	}

	// �w�肵���f�o�C�X�R���e�L�X�g�̃s�N�Z���t�H�[�}�b�g��ݒ肷��
	if ( !SetPixelFormat(m_hDC, pixelFormat, &pfd) )
		return false;

*/	return true;

}

/*******************************************************************
**	�R���e�L�X�g�̐ݒ�
**		OpenGL�̏������Ɋ֌W
*******************************************************************/
bool OpenGL::CreateGLContext(void)
{
	// OpenGL�̃����_�����O�R���e�L�X�g���쐬
	m_hRC = wglCreateContext(m_hDC);
	if ( !m_hRC )	// �쐬�s��
		return false;

	// ���݂̃����_�����O�R���e�L�X�g�ɃZ�b�g
	if ( !wglMakeCurrent(m_hDC, m_hRC) )
		return false;

	return true;
}

/*******************************************************************
**	OpenGL�̃V�[���̃����_�����O
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
**	�I�u�W�F�N�g�őΏۂƂ���V�[���̕`��
*******************************************************************/
void OpenGL::DrawScene(void)
{

	glFlush();

	return;
}

/*******************************************************************
**	ViewType�̐ݒ�
*******************************************************************/
void OpenGL::SetViewType(OpenGL::ViewType type)
{
	VType = type;

	return;
}

/*******************************************************************
**	�V�[���̎��_�ݒ�
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
**	���C�e�B���O�̐ݒ�
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
**	�}�e���A���J���[�̐ݒ�
**		RGB��alpha�l�̓���
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
**	�}�e���A���̐F���擾
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
////////// 3D���f�����O //////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/*******************************************************************
**	���W���̕`��
**		width			: ���W���̕�
**		length			: ���W���̒���
*******************************************************************/
void OpenGL::DrawCoordinateAxis(double length, const double width)
{
	glPushMatrix();
		// x���̕`��(��)
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
		// y���̕`��(��)
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
		// z���̕`��(��)
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
**	�|���S�����f�����O(�@�����v�Z)
**		�Y��(0, 1, 2)�̏���CW(���v���)�œ���
**		�e���_�̍��W�����
*******************************************************************/
void OpenGL::DrawPolygonSurface(double x0, double y0, double z0,
								double x1, double y1, double z1,
								double x2, double y2, double z2)
{

	double norm;
	double x, y, z;
	double vx1,vy1,vz1,vx2,vy2,vz2;

	// �x�N�g���̍����v�Z
	vx1 = x0 - x1;
	vy1 = y0 - y1;
	vz1 = z0 - z1;

	vx2 = x2 - x1;
	vy2 = y2 - y1;
	vz2 = z2 - z1;

	// �O�όv�Z
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
**	�~���`��
*******************************************************************/
void OpenGL::DrawCylinder(double radius, double height)
{
	DrawCylinder2(radius,height);
	return;

	// quadric object �����������
    GLUquadricObj *quad = gluNewQuadric();


    // �ʂ̓h��ׂ����w�肷��i����ł͂Ȃ��A�e�������~����`��
    gluQuadricDrawStyle(quad, GLU_FILL);

    // �X���[�X�V�F�[�f�B���O���s���悤�ݒ肷��
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
	//���p�`�ŋߎ����ĉ~����`��
	double th=0;
	double div=8;//������
	double dth=2*PI/div;
	double x,y,z;
	int i;

	glPushMatrix();
		glTranslated( (GLdouble)0,(GLdouble)0, (GLdouble)(-0.5*height) );
	//���
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

	//����
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


	//����
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
**	�~���`��
*******************************************************************/
void OpenGL::DrawSphere(double radius)
{

	// quadric object ����������� */
    GLUquadricObj *quad = gluNewQuadric();

    // �ʂ̓h��ׂ����w�肷��(����ł͂Ȃ��A�e�������~����`��)
    gluQuadricDrawStyle(quad, GLU_FILL);

    // �X���[�X�V�F�[�f�B���O���s���悤�ݒ肷��
    gluQuadricNormals(quad, GLU_SMOOTH);

    // ����
	gluSphere(quad, (GLdouble)radius, (GLint)16, (GLint)16 );
	return;
}

/*******************************************************************
**	���`��
**		����: �n�_�ƏI�_�̍��W, ���̑���
*******************************************************************/
void OpenGL::DrawLine(double x0, double y0, double z0,
					  double x1, double y1, double z1, double width)
{
	// ���̑�������
	glLineWidth( (GLfloat)width );

	// ���̕`��
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
**	Box�`��
**		����: �Ίp����̒��_
*******************************************************************/
void OpenGL::DrawBox(double x0, double y0, double z0, double x1, double y1, double z1)
{
	// ��1�ʕ`��(x-y���ʂɕ��s�Ȗ�)
	//////////////////////////////////
	glBegin(GL_POLYGON);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f( (GLfloat)x0, (GLfloat)y1, (GLfloat)z1 );
		glVertex3f( (GLfloat)x0, (GLfloat)y0, (GLfloat)z1 );
		glVertex3f( (GLfloat)x1, (GLfloat)y0, (GLfloat)z1 );
		glVertex3f( (GLfloat)x1, (GLfloat)y1, (GLfloat)z1 );
	glEnd();

	// ��2�ʕ`��(y-z���ʂɕ��s�Ȗ�)
	//////////////////////////////////
	glBegin(GL_POLYGON);
		glNormal3f(1.0f, 0.0f, 0.0f);
		glVertex3f( (GLfloat)x1, (GLfloat)y1, (GLfloat)z1 );
		glVertex3f( (GLfloat)x1, (GLfloat)y0, (GLfloat)z1 );
		glVertex3f( (GLfloat)x1, (GLfloat)y0, (GLfloat)z0 );
		glVertex3f( (GLfloat)x1, (GLfloat)y1, (GLfloat)z0 );
	glEnd();

	// ��3�ʕ`��(x-y���ʂɕ��s�Ȗ�)
	//////////////////////////////////
	glBegin(GL_POLYGON);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f( (GLfloat)x1, (GLfloat)y1, (GLfloat)z0 );
		glVertex3f( (GLfloat)x1, (GLfloat)y0, (GLfloat)z0 );
		glVertex3f( (GLfloat)x0, (GLfloat)y0, (GLfloat)z0 );
		glVertex3f( (GLfloat)x0, (GLfloat)y1, (GLfloat)z0 );
	glEnd();

	// ��4�ʕ`��(y-z���ʂɕ��s�Ȗ�)
	//////////////////////////////////
	glBegin(GL_POLYGON);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f( (GLfloat)x0, (GLfloat)y1, (GLfloat)z0 );
		glVertex3f( (GLfloat)x0, (GLfloat)y0, (GLfloat)z0 );
		glVertex3f( (GLfloat)x0, (GLfloat)y0, (GLfloat)z1 );
		glVertex3f( (GLfloat)x0, (GLfloat)y1, (GLfloat)z1 );
	glEnd();

	// ��5�ʕ`��(x-z���ʂɕ��s�Ȗ�)
	//////////////////////////////////
	glBegin(GL_POLYGON);
		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3f( (GLfloat)x0, (GLfloat)y0, (GLfloat)z1 );
		glVertex3f( (GLfloat)x0, (GLfloat)y0, (GLfloat)z0 );
		glVertex3f( (GLfloat)x1, (GLfloat)y0, (GLfloat)z0 );
		glVertex3f( (GLfloat)x1, (GLfloat)y0, (GLfloat)z1 );
	glEnd();

	// ��6�ʕ`��(x-z���ʂɕ��s�Ȗ�)
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
**	Frame�`��(���ʂ�4���̐���)
**		����: �Ίp����̒��_
*******************************************************************/
void OpenGL::DrawFrame(double x0, double y0, double z0, double x1, double y1, double z1)
{
	// �F���O���[�ɐݒ�
	glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,materialAmbDiffGray);
	// ��������
	glLineWidth(4.0f);

	// ���ʕ`��
	glBegin(GL_LINE_LOOP);
		glVertex3f( (GLfloat)x1, (GLfloat)y1, (GLfloat)z0 );
		glVertex3f( (GLfloat)x1, (GLfloat)y0, (GLfloat)z0 );
		glVertex3f( (GLfloat)x0, (GLfloat)y0, (GLfloat)z0 );
		glVertex3f( (GLfloat)x0, (GLfloat)y1, (GLfloat)z0 );
	glEnd();

	// �ȉ��Ő����`��
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
////////// 2D���f�����O //////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/*******************************************************************
**	���ʔ`��(�Ίp���̒��_�ƍ���)
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
