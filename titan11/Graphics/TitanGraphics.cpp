/*****************************************************************************
**
**	File:	TitanGraphics.cpp
**
**	����:	OpenGL�֌W�̕`��N���X(Windows��p)
**
**	Created: 2004/02/10(Tue)	Updated: 2005/02/25(Fri)
**  Updated 050913 doi
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

#include "TitanGraphics.h"
//#include "../Plan/GaitParameter.h"

/*******************************************************************
**	�O���b�h�\�����邽�߂̒萔
*******************************************************************/
const double GRID_START_X	= -100000.0;
const double GRID_START_Y	= -100000.0;
const double GRID_END_X		= 100000.0;
const double GRID_END_Y		= 100000.0;
const double GRID_DX		= 2000.0;
const double GRID_DY		= 2000.0;

/*******************************************************************
**	�R���X�g���N�^
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
**	�f�t�H���g�R���X�g���N�^
*******************************************************************/
TitanGraphics::TitanGraphics()
{
	InitTitanGraphics();
}

/*******************************************************************
**	�f�X�g���N�^
*******************************************************************/
TitanGraphics::~TitanGraphics()
{
	// �f�B�X�v���C���X�g�̏���
	glDeleteLists(DisListLegBone[0], 4);
//	glDeleteLists(DisListMap[0], 4);

/*
	// �n�}�I�u�W�F�N�g�̍폜

	// �|�C���^����łȂ���΍폜 050913 doi
	if ( pGridMap!=NULL )
	delete pGridMap;

	// �|�C���^����łȂ���΍폜 050913 doi
	if ( pCellMap!=NULL )
	delete pCellMap;
*/
}

/*******************************************************************
**	������
*******************************************************************/
void TitanGraphics::InitTitanGraphics(void)
{

	// �}�E�X�̑��샂�[�h�̏�����
	MouseView.SetDefault();

	// OpenGL���̏�����
	InitEnvironment();

	// �|�C���^�̏�����
	pTitanData = 0;


//	pGridMap=NULL;//050913 doi
//	pCellMap=NULL;//050913 doi

	// �n�}�̕`��p�^�[���̐ݒ�
//	MapDrawPattern = WIRE;

	// �e�t���O�̏�����
	IsDisListPrepared		= false;
	IsGridDrawn				= true;
	IsSupportPolygonDrawn	= true;
	IsAxisDrawn				= true;
//	IsCellMapDrawn			= false;

	return;
}

/*******************************************************************
**	�ϐ����f�t�H���g�ɃZ�b�g
*******************************************************************/
void TitanGraphics::InitEnvironment(void)
{
	// ���C�e�B���O�̐ݒ�
	m_ambientLight0[0] = 1.0f;		// ����
	m_ambientLight0[1] = 1.0f;
	m_ambientLight0[2] = 1.0f;
	m_ambientLight0[3] = 0.8f;

	m_diffuseLight0[0] = 1.0f;		// �g�U��
	m_diffuseLight0[1] = 1.0f;
	m_diffuseLight0[2] = 1.0f;
	m_diffuseLight0[3] = 1.0f;

	m_specularLight0[0] = 1.0f;		// ���ʌ�
	m_specularLight0[1] = 1.0f;
	m_specularLight0[2] = 1.0f;
	m_specularLight0[3] = 1.0f;

	m_positionLight0[0] = 0.0f;		// �����ʒu
	m_positionLight0[1] = -2500.0f;
	m_positionLight0[2] = 5000.0f;
	m_positionLight0[3] = 0.0f;

	m_directionLight0[0] = 0.0f;	// �X�|�b�g���C�g����
	m_directionLight0[1] = 0.0f;
	m_directionLight0[2] = -5000.0f;

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
**	���C�e�B���O�̐ݒ�
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
**	�f�B�X�v���C���X�g�̃Z�b�g
*******************************************************************/
void TitanGraphics::InitDisplayList(void)
{
	if (IsDisListPrepared != false)return;

	// �f�B�X�v���C���X�g�̐���
	int list_0 = glGenLists(8);

	int i;

	// ��������
	if (list_0 == 0)
	{
		fprintf(stderr, "Display List error!\n\n");
		return;//error
	}

	for (i=0; i<4; i++)//�r�����N
	{
		DisListLegBone[i] = (list_0+i);
	}
	for(i=0;i<4;i++)//�n�}
	{
//		DisListMap[i]=(list_0+i+4);
	}

	//�r�����N�f�B�X�v���C���X�g
	//COLOR CJoint	= WHITE;
	COLOR CJoint	= SKY;
	//COLOR CLink		= GRAY;
	COLOR CLink		= SKY;


	double l1,l2,l3,l4,l5,l6;//�r���f�����O�p�����[�^.060726memo�Q��

	// Hip
	glNewList(DisListLegBone[0], GL_COMPILE);
	l1=100;
	l2=400;
	l3=430+200;
	l4=200;
	l5=300;
	l6=100;
		SetMaterial( SelectColor(CJoint) );
		//double hip_height=430;//�r�t�����̉������ˋN����
		glPushMatrix();
		//DrawBox(-200,  -hip_height, -150, 300, 0, 150);//051107�r�t�����̉������ˋN
		DrawPolygonSurface((GLfloat)-l6,(GLfloat)0,(GLfloat)(-0.5*l4),
							(GLfloat)-l6,(GLfloat)l1,(GLfloat)(-0.5*l4),
							(GLfloat)l5,(GLfloat)0,(GLfloat)(-0.5*l4));//���ʂP
		DrawPolygonSurface((GLfloat)-l6,(GLfloat)l1,(GLfloat)(-0.5*l4),
							(GLfloat)l5,(GLfloat)l1,(GLfloat)(-0.5*l4),
							(GLfloat)l5,(GLfloat)0,(GLfloat)(-0.5*l4));//���ʂQ
		DrawPolygonSurface((GLfloat)-l6,(GLfloat)0,(GLfloat)(-0.5*l4),
							(GLfloat)l5,(GLfloat)0,(GLfloat)(-0.5*l4),
							(GLfloat)-l6,(GLfloat)-l2,(GLfloat)(-0.5*l4));//���ʂR
		DrawPolygonSurface((GLfloat)-l6,(GLfloat)-l2,(GLfloat)(-0.5*l4),
							(GLfloat)l5,(GLfloat)0,(GLfloat)(-0.5*l4),
							(GLfloat)l5,(GLfloat)-l3,(GLfloat)(-0.5*l4));//���ʂS
		DrawPolygonSurface((GLfloat)-l6,(GLfloat)0,(GLfloat)(0.5*l4),
							(GLfloat)l5,(GLfloat)0,(GLfloat)(0.5*l4),
							(GLfloat)-l6,(GLfloat)l1,(GLfloat)(0.5*l4));//���ʂT
		DrawPolygonSurface((GLfloat)-l6,(GLfloat)l1,(GLfloat)(0.5*l4),
							(GLfloat)l5,(GLfloat)0,(GLfloat)(0.5*l4),
							(GLfloat)l5,(GLfloat)l1,(GLfloat)(0.5*l4));//���ʂU
		DrawPolygonSurface((GLfloat)-l6,(GLfloat)0,(GLfloat)(0.5*l4),
							(GLfloat)-l6,(GLfloat)-l2,(GLfloat)(0.5*l4),
							(GLfloat)l5,(GLfloat)0,(GLfloat)(0.5*l4));//���ʂV
		DrawPolygonSurface((GLfloat)-l6,(GLfloat)-l2,(GLfloat)(0.5*l4),
							(GLfloat)l5,(GLfloat)-l3,(GLfloat)(0.5*l4),
							(GLfloat)l5,(GLfloat)0,(GLfloat)(0.5*l4));//���ʂW

		glPopMatrix();
	glEndList();

	// Thigh�i�ځj
	glNewList(DisListLegBone[1], GL_COMPILE);
		glTranslated(-LINK2, 0, 0);
	   	SetMaterial( SelectColor(CJoint) );
		DrawCylinder(50, 400);
		SetMaterial( SelectColor(CLink) );
		//DrawBox(0, -100, -100, LINK2, 500, 100);//���^�ŋߎ�
		l1=900;
		l2=DTC14;
		l3=400;
		l4=200;
		DrawPolygonSurface((GLfloat)0,(GLfloat)0,(GLfloat)(-0.5*l4),
							(GLfloat)l1, (GLfloat)(l2+l3), (GLfloat)(-0.5*l4),
							(GLfloat)l1, (GLfloat)l2, (GLfloat)(-0.5*l4));//���ʂP
		DrawPolygonSurface((GLfloat)LINK2, (GLfloat)0, (GLfloat)(-0.5*l4),
							(GLfloat)l1, (GLfloat)l2, (GLfloat)(-0.5*l4),
							(GLfloat)l1, (GLfloat)(l2+l3), (GLfloat)(-0.5*l4));//���ʂQ
		DrawPolygonSurface((GLfloat)0, (GLfloat)0, (GLfloat)(0.5*l4),
							(GLfloat)l1, (GLfloat)l2, (GLfloat)(0.5*l4),
							(GLfloat)l1, (GLfloat)(l2+l3), (GLfloat)(0.5*l4));//���ʂR
		DrawPolygonSurface((GLfloat)LINK2, (GLfloat)0, (GLfloat)(0.5*l4),
							(GLfloat)l1, (GLfloat)(l2+l3), (GLfloat)(0.5*l4),
							(GLfloat)l1, (GLfloat)l2, (GLfloat)(0.5*l4));//���ʂS
		DrawPolygonSurface((GLfloat)0,(GLfloat)0,(GLfloat)(-0.5*l4),
							(GLfloat)0,(GLfloat)0,(GLfloat)(0.5*l4),
							(GLfloat)l1, (GLfloat)(l2+l3), (GLfloat)(-0.5*l4));//��ʂP
		DrawPolygonSurface((GLfloat)0,(GLfloat)0,(GLfloat)(0.5*l4),
							(GLfloat)l1, (GLfloat)(l2+l3), (GLfloat)(0.5*l4),
							(GLfloat)l1, (GLfloat)(l2+l3), (GLfloat)(-0.5*l4));//��ʂQ
		DrawPolygonSurface((GLfloat)LINK2,(GLfloat)0,(GLfloat)(0.5*l4),
							(GLfloat)LINK2,(GLfloat)0,(GLfloat)(-0.5*l4),
							(GLfloat)l1, (GLfloat)(l2+l3), (GLfloat)(-0.5*l4));//��ʂR
		DrawPolygonSurface((GLfloat)LINK2,(GLfloat)0,(GLfloat)(0.5*l4),
							(GLfloat)l1, (GLfloat)(l2+l3), (GLfloat)(-0.5*l4),
							(GLfloat)l1, (GLfloat)(l2+l3), (GLfloat)(0.5*l4));//��ʂS
	glEndList();

	 // Shank�i���j
	glNewList(DisListLegBone[2], GL_COMPILE);
	l1=300;
	l2=300;
	l3=200;
		glTranslated(-LINK3, 0, 0);
		SetMaterial( SelectColor(CJoint) );
		DrawCylinder(50, 400);
		SetMaterial( SelectColor(CLink) );
		//DrawBox(0,-100,-100, LINK3, 300, 100);//Box�ŋߎ�
		DrawPolygonSurface((GLfloat)0,(GLfloat)0,(GLfloat)(-0.5*l3),
							(GLfloat)l1,(GLfloat)l2,(GLfloat)(-0.5*l3),
							(GLfloat)l1,(GLfloat)0,(GLfloat)(-0.5*l3));//���ʂP
		DrawPolygonSurface((GLfloat)LINK3,(GLfloat)0,(GLfloat)(-0.5*l3),
							(GLfloat)l1,(GLfloat)0,(GLfloat)(-0.5*l3),
							(GLfloat)l1,(GLfloat)l2,(GLfloat)(-0.5*l3));//���ʂQ
		DrawPolygonSurface((GLfloat)0,(GLfloat)0,(GLfloat)(0.5*l3),
							(GLfloat)l1,(GLfloat)0,(GLfloat)(0.5*l3),
							(GLfloat)l1,(GLfloat)l2,(GLfloat)(0.5*l3));//���ʂR
		DrawPolygonSurface((GLfloat)LINK3,(GLfloat)0,(GLfloat)(0.5*l3),
							(GLfloat)l1,(GLfloat)l2,(GLfloat)(0.5*l3),
							(GLfloat)l1,(GLfloat)0,(GLfloat)(0.5*l3));//���ʂS
		DrawPolygonSurface((GLfloat)0,(GLfloat)0,(GLfloat)(-0.5*l3),
							(GLfloat)0,(GLfloat)0,(GLfloat)(0.5*l3),
							(GLfloat)l1,(GLfloat)l2,(GLfloat)(0.5*l3));//��ʂP
		DrawPolygonSurface((GLfloat)0,(GLfloat)0,(GLfloat)(-0.5*l3),
							(GLfloat)l1,(GLfloat)l2,(GLfloat)(0.5*l3),
							(GLfloat)l1,(GLfloat)l2,(GLfloat)(-0.5*l3));//��ʂQ
		DrawPolygonSurface((GLfloat)LINK3,(GLfloat)0,(GLfloat)(0.5*l3),
							(GLfloat)LINK3,(GLfloat)0,(GLfloat)(-0.5*l3),
							(GLfloat)l1,(GLfloat)l2,(GLfloat)(-0.5*l3));//��ʂR
		DrawPolygonSurface((GLfloat)LINK3,(GLfloat)0,(GLfloat)(0.5*l3),
							(GLfloat)l1,(GLfloat)l2,(GLfloat)(-0.5*l3),
							(GLfloat)l1,(GLfloat)l2,(GLfloat)(0.5*l3));//��ʂS
	glEndList();

	 // Foot
	glNewList(DisListLegBone[3] , GL_COMPILE);
		glTranslated(-FOOT, 0, 0);
		SetMaterial( SelectColor(CJoint) );
		DrawCylinder(50, 200);
		SetMaterial( SelectColor(CLink) );
		//DrawBox(0, -250,-250, FOOT, 250, 250);//�������l�p������ꍇ�D
		glPushMatrix();
		glRotatef((GLfloat)-90,(GLfloat)0,(GLfloat)1,(GLfloat)0);
		glTranslatef((GLfloat)0,(GLfloat)0,(GLfloat)(-0.5*FOOT));
		DrawCylinder(250, 50);//�~�Տ�̑���
		glPopMatrix();
	glEndList();

	//�n�}�f�B�X�v���C���X�g
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
**	�f�[�^�\�[�X�̐ݒ�
*******************************************************************/
void TitanGraphics::SetTitanData(TitanData* titan)
{
	// �����`�F�b�N
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
**	���_�̐ݒ�
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
**	�n�}�̕`��ݒ�
*******************************************************************/
/*
void TitanGraphics::SetMapDrawPattern(MAPPATTERN pattern)
{
	MapDrawPattern = pattern;

	return;
}
*/
/*******************************************************************
**	�Z���n�}�̕`�捂����ݒ�
*******************************************************************/
/*
void TitanGraphics::SetCellMapHeight(double height)
{
	// �|�C���^����Ȃ�Ȃɂ����Ȃ� 050913 doi
	if ( pCellMap==NULL )return;

	pCellMap->SetCellHeight(height);

	return;
}
*/
/*******************************************************************
**	OpenGL�̃V�[���̃����_�����O
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
		return;//091016�ǉ�
	}

	//SwapBuffers( wglGetCurrentDC() );
	//wglMakeCurrent(m_hDC, NULL);//DC����H

	return;
}

/*******************************************************************
**	OpenGL�̃V�[���̕`��
*******************************************************************/
void TitanGraphics::DrawScene(void)
{
	// ���_����
	SetSceneView();

	// �f�B�X�v���C���X�g������
	InitDisplayList();

	// �t���A�`��
	if ( IsGridDrawn )
		{
			double GAIT_HEIGHT=1200;//140825
		DrawGridFloor(GRID_START_X, GRID_START_Y, GRID_END_X, GRID_END_Y, -1*GAIT_HEIGHT, GRID_DX, GRID_DY);
	}

	// ���W�n�`��
	if ( IsAxisDrawn )
		DrawCoordinateAxis(3000, 4.0);

	// Titan�`��
	DrawTitan();

	// �x���r���p�`�`��
	if ( IsSupportPolygonDrawn )
		//DrawSupportPolygon();

	// �n�`�`��
	if(IsTerrainDrawn)
	{
		//DrawMap();
	}


	return;
}
/*******************************************************************
**	���`�� 060525 doi
**		DrawArrow
*******************************************************************/
void	TitanGraphics::DrawArrow(double x0_in, double y0_in, double z0_in,
		double vx_in, double vy_in, double vz_in)
{
		//SetMaterial(SelectColor(YELLOW));
		SetMaterial(SelectColor(RED));
		//������
		DrawLine(x0_in, y0_in,z0_in, x0_in+vx_in, y0_in+vy_in,z0_in+vz_in, 5.0);
		/*
		glBegin(GL_LINES);
			glVertex3d(x0_in, y0_in,z0_in);
			glVertex3d(x0_in+vx_in, y0_in+vy_in,z0_in+vz_in);
		glEnd();
		*/
		//�O�p
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


		p=pTitanData->GetRobotData().GetBodyPosition();//���̂������`��
		double x=dir(1);//debug
		double y=dir(2);//debug
		double z=dir(3);//debug
		//printf("Move Dir[%.2f, %.2f]\r",x,y);
		DrawArrow(p(1),p(2),p(3)+2000,dir(1),dir(2),dir(3));

		/*
		for(int i=0;i<4;i++)//�r�������`��
		{
			p=pTitanData->GetRobotData().GetFootPosition(i+1);
			//p(3)=p(3)+FOOT;//����ʒu
			p(3)=p(3)+300;//�n�`�ɖ�����Ȃ�����
			DrawArrow(p(1),p(2),p(3),dir(1),dir(2),dir(3));
		}
		*/
}


/*******************************************************************
**	Titan�̕`��
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
**	Titan�̖{�̕`��
*******************************************************************/
void TitanGraphics::DrawLift(double LiftAngle, double CircleAngle)
{
	//LiftAngle--���t�g�X�΁D���̂ƕ��s�̂Ƃ��[��
	//CircleAngle--�ǔ���]�@�\�D���̂ɐ����̂Ƃ��[��
	//�P��[deg]
		//���t�g 060727memo�Q��
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

		//��]���`��
		glPushMatrix();
		glRotatef((GLfloat)90,1,0,0);
		glTranslatef((GLfloat)(0),(GLfloat)0,(GLfloat)(600));
		DrawCylinder(100,800);
		glTranslatef((GLfloat)(0),(GLfloat)0,(GLfloat)(-1200));
		DrawCylinder(100,800);
		glPopMatrix();

		glRotatef((GLfloat)LiftAngle,(GLfloat)0,(GLfloat)1,(GLfloat)0);
		glTranslatef((GLfloat)(x1),(GLfloat)0,(GLfloat)(z1));
		DrawBox(	-l1, -0.5*l5, 0,  0,	-0.5*l6, l2);//���ŋߎ��D�E��
		DrawBox(	-l1, 0.5*l6, 0,  0,	0.5*l5, l2);//���ŋߎ��D����

			//��]�@�\

			xc=-l1-100;
			yc=500;
			zc=l2+300;
			double lc=200;
			double angle2=CircleAngle*(RAD2DEG);
			glTranslatef((GLfloat)xc,(GLfloat)yc,(GLfloat)zc);
			glRotatef((GLfloat)-angle2,(GLfloat)0,(GLfloat)1,(GLfloat)0);
			glRotatef((GLfloat)90,(GLfloat)0,(GLfloat)1,(GLfloat)0);
			SetMaterial(0.9,0.3,0.3,1.0);
			DrawCylinder(300,30);//�~��
			SetMaterial(0.9,0.9,0.9,1.0);
			DrawCylinder(50,60);//�~�Ւ��S
		glPopMatrix();

}
void TitanGraphics::DrawDrill(double DrillAngle)
{
	//angle--�X�Γx�D���̂ƕ��s�Ń[��
	//�P��[rad]
		//�h���� 060727memo�Q��
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

			glRotatef((GLfloat)90,1,0,0);//��]���`��
				DrawCylinder(100,300);
			glRotatef((GLfloat)-90,1,0,0);//���ɖ߂�

		glRotatef(DrillAngle,0,1,0);
		DrawPolygonSurface(0,-0.5*l5,0,		-0.5*l3, -0.5*l5,l6,	0.5*l3, -0.5*l5, l6);//�h�����t�����O�p����
		DrawPolygonSurface(0,0.5*l5,0,		0.5*l3, 0.5*l5,	l6,		-0.5*l3, 0.5*l5, l6);//�h�����t�����O�p�E��

		DrawBox(l4-l2,-0.5*l5,l6, l4,0.5*l5,l1+0.5*l5);//�h�����̃K�C�h
		double drill_Xoffset=2000;//�h�������S����`������
		SetMaterial(1,0,0,1);
		double thick=0.5;//���̑���
		DrawLine((l4+drill_Xoffset),0,l1, (l4-l2),0,l1,thick);
		/*
		glBegin(GL_LINES);//�h�������S��
			glVertex3d((GLdouble)(l4+drill_Xoffset),(GLdouble)0,(GLdouble)l1);
			glVertex3d((GLdouble)(l4-l2),(GLdouble)0,(GLdouble)l1);
		glEnd();
		*/
		glPopMatrix();

}

void TitanGraphics::DrawBody(void)
{
	// �O���[�o�����瓷�̍��W�ւ̕ϊ��s��
	Matrix gAb(4,4);

	glPushMatrix();

		// ���̍��W�n�ɕϊ�
		gAb = pTitanData->GetBodyTransform();
		TransformGLMatrix(gAb);

		//���̃v���[�g
		SetMaterial( 0.2,0.2,0.9,1 );
		DrawBox(	-0.5*BODY_LENGTH, -0.5*BODY_WIDTH, -0.5*100,
					 0.5*BODY_LENGTH,  0.5*BODY_WIDTH,  0.5*100
				);

		//�E�C���`
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


		//���N���[��
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

		//���t�g
		double CircleAngle=0;//�ǔ����u�X�Ίp[deg]
		SetMaterial( 0.4,0.4,0.6,1 );
		//SetMaterial( SelectColor(GRAY) );
		DrawLift(pTitanData->GetEngineAngle()*RAD2DEG,CircleAngle);

		//�h����
		SetMaterial( SelectColor(SKY) );
		DrawDrill(pTitanData->GetDrillAngle()*RAD2DEG);

	glPopMatrix();

	return;
}

/*******************************************************************
**	Titan�̋r�`��
*******************************************************************/
void TitanGraphics::DrawLeg(int legID)
{
	// �����̃`�F�b�N
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "TitanGraphics::DrawLeg() Invalid prameter\n\n");
		return;
	}

	Matrix trans(DH_NUM, DH_NUM);
	Matrix gAb(DH_NUM, DH_NUM);		// global �� body
	Matrix bAl(DH_NUM, DH_NUM);		// body �� leg

	gAb = pTitanData->GetBodyTransform();

	glPushMatrix();

		// ���̍��W�n�ɕϊ�
		TransformGLMatrix(gAb);

		// �r�̃x�[�X���W�n���擾���āC�r���W�ɕϊ�
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
**	�n�}�̕`��
*******************************************************************/
/*
void TitanGraphics::DrawMap(void)
{
//return;//debug
	// �|�C���^����Ȃ�`�悵�Ȃ�
	if ( pGridMap==NULL )return;

	// �n�}���ǂݍ��܂�Ă��Ȃ�������`�悵�Ȃ�
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
**	�x���r���p�`(�O�p�`or�l�p�`)�̕`��
*******************************************************************/
void TitanGraphics::DrawSupportPolygon(void)
{
	int i;
	Robot::LegPhase phase[LEG_NUM];

	for (i=0; i<4; i++)
	{
		phase[i] = pTitanData->GetLegPhase(i+1);
	}

	// ���p�`�̐����F����
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
**	�O���b�h�̏���`��
**		����
**			���̑Ίp���̎n�_�ƏI�_�ƍ����ƍׂ���
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

	// x�����̕`��
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

	// y�����̕`��
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
**	��ʏ�̑���N�_�ƂȂ�_���Z�b�g
*******************************************************************/
void TitanGraphics::SetMouseViewPoint(int x, int y)
{
	MouseView.SetCtrlPoint(x, y);

	return;
}

/*******************************************************************
**	�}�E�X�̃C�x���g���[�h���擾
*******************************************************************/
MouseViewCtrl::DragMode TitanGraphics::GetMouseViewDragMode(void)
{
	return ( MouseView.GetDragMode() );
}

/*******************************************************************
**	�}�E�X�̃C�x���g���[�h��ݒ�
*******************************************************************/
void TitanGraphics::SetMouseViewDragMode(MouseViewCtrl::DragMode mode)
{
	MouseView.SetDragMode(mode);

	return;
}

/*******************************************************************
**	�}�E�X�̃h���b�O���[�h�ɂ��View�̕ύX
*******************************************************************/
void TitanGraphics::MouseViewTransform(int x, int y)
{
	MouseView.TransformView(x, y);

	return;
}

/*******************************************************************
**	���o���̒��ڐݒ�
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
**	OpenGL�p�̍��W�ϊ�(4x4)
*******************************************************************/
void TitanGraphics::TransformGLMatrix(const Matrix& matrix)
{
	double T[16];

	int k=0;

	// �}�g���N�X�v�f��z��Ɋi�[
	for (int i=1; i<=4; i++)
	{
		for (int j=1; j<=4; j++)
		{
			T[k] = matrix(j,i);
			k++;
		}
	}

	// ���݂̃}�g���N�X�Ɗ|����
	glMultMatrixd(T);

	return;
}

/*******************************************************************
**	�n�}����`�悷�邽�߂̃w���p�[�֐�
*******************************************************************/

/*******************************************************************
**	�h��ׂ��̒n�}�����
*******************************************************************/
/*
void TitanGraphics::DrawCellMap(void)
{
	// �|�C���^����Ȃ�Ȃɂ����Ȃ� 050913 doi
	if ( pCellMap==NULL )return;

	//Displaylist���p�ӂ���Ă���΂�����g���D050913 doi
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
**	�n�}����ō��
*******************************************************************/
/*
void TitanGraphics::DrawWireMap(void)
{
	// �|�C���^����Ȃ�Ȃɂ����Ȃ� 050913 doi
	if ( pGridMap==NULL )return;

	//Displaylist���p�ӂ���Ă���΂�����g���D050913 doi
	if(IsDisListPrepared)
	{
	glCallList(DisListMap[2]);
	return;
	}

	int iMax = pGridMap->GetRow();
	int jMax = pGridMap->GetCol();

	// ��������x, y�����̎n�_
	Matrix pi(3);
	Matrix pj(3);

	// ���������I�_
	Matrix p(3);

	SetMaterial( SelectColor(PURPLE) );

	// �ȉ��̃��[�v�ŕ`��
	for (int i=1; i<=iMax; i++)
	{
		for (int j=1; j<=jMax; j++)
		{
			// ���݂̊i�q�_�ԍ��̍��W���擾
			p(1) = pGridMap->GetNodePosX(i);
			p(2) = pGridMap->GetNodePosY(j);
			p(3) = (*pGridMap)(i, j);

			// y�����̐��`��
			if ( (j-1)>=1 )
			{
				pj(1) = p(1);
				pj(2) = pGridMap->GetNodePosY(j-1);
				pj(3) = (*pGridMap)(i, j-1);
				DrawLine(p, pj, 1.5);
			}

			// x�����̐��`��
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
**	���ʑ̂̒n�}�����
*******************************************************************/
/*
void	TitanGraphics::DrawGrayPolygon(Matrix &p1, Matrix &p2, Matrix &p3)
{
	//�@���̌����ŔZ����ς����|���S��
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
	// �|�C���^����Ȃ�Ȃɂ����Ȃ� 050913 doi
	if ( pGridMap==NULL )return;

	//Displaylist���p�ӂ���Ă���΂�����g���D
	if(IsDisListPrepared)
	{
	glCallList(DisListMap[2]);
	return;
	}

	int iMax = pGridMap->GetRow();
	int jMax = pGridMap->GetCol();

	// ���������Q�Ɠ_
	Matrix pi(3);
	Matrix pj(3);
	Matrix pk(3);

	// ���������N�_
	Matrix p(3);


	// �ȉ��Ń|���S���`��
	for (int i=1; i<=iMax; i++)
	{
		for (int j=1; j<=jMax; j++)
		{
			// �N�_�̍��W�v�Z
			p(1) = pGridMap->GetNodePosX(i);
			p(2) = pGridMap->GetNodePosY(j);
			p(3) = (*pGridMap)(i,j);

			//SetMaterial( SelectColor(PURPLE) );
			SetMaterial( SelectColor(BLACK) );
			// 1�ڂ̎Q�Ɠ_���v�Z
			if ( (j-1)>=1 )
			{
				pj(1) = p(1);
				pj(2) = pGridMap->GetNodePosY(j-1);
				pj(3) = (*pGridMap)(i,j-1);
				DrawLine(p, pj, 0.1);
			}

			// 2�ڂ̎Q�Ɠ_���v�Z
			if ( (i-1)>=1 )
			{
				pi(1) = pGridMap->GetNodePosX(i-1);
				pi(2) = p(2);
				pi(3) = (*pGridMap)(i-1,j);
				DrawLine(p, pi, 0.1);
			}

			// 3�ڂ̎Q�Ɠ_���v�Z
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
**	�������̒n�}�����
*******************************************************************/
/*
void TitanGraphics::DrawContourMap(void)
{
	// �|�C���^����Ȃ�Ȃɂ����Ȃ� 050913 doi
	if ( pGridMap==NULL )return;

	//Displaylist���p�ӂ���Ă���΂�����g���D050913 doi
	if(IsDisListPrepared)
	{
	glCallList(DisListMap[2]);
	return;
	}

	return;
}
*/
/*******************************************************************
**	CellMap��GridMap�ɉ����`�ŕ\��
*******************************************************************/
/*
void TitanGraphics::DrawCellMapOnGridMap(void)
{
	double clearance=300;//GridMap���畂��������

	// �|�C���^����Ȃ�Ȃɂ����Ȃ� 050913 doi
	if ( pCellMap==NULL )return;
	if ( pGridMap==NULL )return;

	//Displaylist���p�ӂ���Ă���΂�����g���D050913 doi
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
