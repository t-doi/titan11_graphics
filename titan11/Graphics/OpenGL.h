/*****************************************************************************
**
**	File:	OpenGL.h
** 
**	����:	OpenGL�֌W�̕`��N���X(Windows��p)
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
**		�����o�ϐ�		**
**						**
*************************/
public:
/*******************************************************************
**	���_�p�^�[��
*******************************************************************/
    enum ViewType{PERSPECTIVE, TOP, SIDE, FRONT};

/*******************************************************************
**	��ȐF�p�^�[��
*******************************************************************/
	enum COLOR{RED, GREEN, BLUE, YELLOW, GRAY, BLACK, WHITE, SKY, ORANGE, PURPLE};

/*******************************************************************
**	�n���h���̕ϐ�
**		m_hWnd			: �E�B���h�E
**		m_hDC			: �f�o�C�X�R���e�L�X�g
**		m_hRC			: �����_�����O�R���e�L�X�g
*******************************************************************/
    HWND	m_hWnd;
	HDC		m_hDC;
	HGLRC	m_hRC;

private:

/*******************************************************************
**	���_�p�^�[��
*******************************************************************/
	ViewType VType;

protected:

/*******************************************************************
**	���C�e�B���O
*******************************************************************/
	GLfloat m_ambientLight0[4];			// ����
	GLfloat m_diffuseLight0[4];			// �g�U��
	GLfloat m_specularLight0[4];		// ���ʌ�
	GLfloat m_positionLight0[4];		// �����ʒu
	GLfloat m_directionLight0[3];		// �X�|�b�g���C�g����

/*******************************************************************
**	�}�e���A���F
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
**		�����o�֐�		**
**						**
*************************/
public:
/*******************************************************************
**	�R���X�g���N�^�ƃf�X�g���N�^
*******************************************************************/
    OpenGL();
    ~OpenGL();

/*******************************************************************
**	OpenGL�̏������Ɣj��
**		InitOpenGL			: ������
**		DestroyOpenGL			: �j��
*******************************************************************/
	bool			InitOpenGL(HWND hWnd);
	void			DestroyOpenGL(void);
	void			SetDefault(void);
	virtual void	InitEnvironment(void);

/*******************************************************************
**	�����o�ϐ��̎擾
*******************************************************************/
	HWND	GetHWND(void){ return (m_hWnd);}

/*******************************************************************
**	OpenGL�̃C���[�W�̃����_�����O
*******************************************************************/
	virtual void	RenderScene(void);

/*******************************************************************
**	OpenGL�̃C���[�W�`��
*******************************************************************/
	virtual void	DrawScene(void);

/*******************************************************************
**	���_�̐ݒ�
*******************************************************************/
	virtual void	SetSceneView(void);
	void			SetViewPoint(double distance, double azimuth, double elevation,
								 double x = 0, double y = 0, double z = 0,
								 double aspect_ratio = 1);
	void		SetViewType(ViewType type);
	ViewType	GetViewType(void){return (VType);}

/*******************************************************************
**	���C�e�B���O�̐ݒ�
*******************************************************************/
	virtual void	SetLighting(void);

/*******************************************************************
**	�}�e���A���J���[�̐ݒ�
*******************************************************************/
	void	SetMaterial(double red, double green, double blue, double alpha);
	void	SetMaterial(GLfloat* materialAmbDiffColor);

/*******************************************************************
**	�}�e���A���J���[�̎擾
*******************************************************************/
	GLfloat*	SelectColor(COLOR color);

/*********************
**	�e�탂�f�����O  **
*********************/

//////////////////////////////////////////////////////////////////////
////////// 3D���f�����O //////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/*******************************************************************
**	���W���`��
*******************************************************************/
	void	DrawCoordinateAxis(double length, double width = 1.0); 

/*******************************************************************
**	�|���S�����f�����O
*******************************************************************/
	void	DrawPolygonSurface(double x0, double y0, double z0,
							   double x1, double y1, double z1,
							   double x2, double y2, double z2);
	void	DrawPolygonSurface(const Matrix& p, const Matrix& q, const Matrix& r);

/*******************************************************************
**	�~���`��
*******************************************************************/
	void	DrawCylinder2(double radius, double height);//16�p�`�ŒP����
	void	DrawCylinder(double radius, double height);

/*******************************************************************
**	���`��
*******************************************************************/
	void	DrawSphere(double radius);

/*******************************************************************
**	���`��(����)
*******************************************************************/
	void	DrawLine(double x0, double y0, double z0,
					 double x1, double y1, double z1, double width = 1.0);
	void	DrawLine(const Matrix& p, const Matrix& q, double width = 1.0);

/*******************************************************************
**	Box�`��(����)
*******************************************************************/
	void	DrawBox(double x0, double y0, double z0, double x1, double y1, double z1);
	void	DrawBox(const Matrix& p, const Matrix& q);

/*******************************************************************
**	�t���[���`��(���ʂ�4���̐���)
*******************************************************************/
	void	DrawFrame(double x0, double y0, double z0, double x1, double y1, double z1);
	void	DrawFrame(const Matrix& p, const Matrix& q);

//////////////////////////////////////////////////////////////////////
////////// 2D���f�����O //////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/*******************************************************************
**	���ʔ`��(�Ίp���̒��_�ƍ���)
*******************************************************************/
	void	DrawPlaneXY(double x0, double y0, double x1, double y1, double z);
	void	DrawPlaneYZ(double y0, double z0, double y1, double z1, double x);
	void	DrawPlaneZX(double z0, double x0, double z1, double x1, double y);


private:
/*******************************************************************
**	OpenGL�̏������Ɋ֌W
**		SetWindowPixelFormat		: �s�N�Z���t�H�[�}�b�g�̐ݒ�
**		CreateGLContext				: �R���e�L�X�g�̐ݒ�
*******************************************************************/
	bool	SetWindowPixelFormat(void);
	bool	CreateGLContext(void);
};

#endif // __OpenGL_h__
