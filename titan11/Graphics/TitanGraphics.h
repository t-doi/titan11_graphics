/*****************************************************************************
**
**	File:	TitanGraphics.h
** 
**	����:	OpenGL�֌W�̕`��N���X(Windows��p)
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
**		�����o�ϐ�		**
**						**
*************************/

public:
/*******************************************************************
**	�摜�\������n�}�p�^�[���`��
**		INVISIBLE			: �s��(�`�悵�Ȃ�)
**		FILL				: �h��ׂ�(����0��)
**		POLYGON				: ���ʑ̂ŕ\��
**		CONTOUR				: �������ŕ\��
*******************************************************************/
//	typedef enum MapPatterrn{INVISIBLE, WIRE, POLYGON, CONTOUR,}MAPPATTERN;

private:

/*******************************************************************
**	�摜�\������Titan�I�u�W�F�N�g�ւ̃|�C���^
*******************************************************************/
	TitanData*	pTitanData;

/*******************************************************************
**	�摜�\������n�}�I�u�W�F�N�g�̃|�C���^
*******************************************************************/
//	GridMap*	pGridMap;
//	CellMap*	pCellMap;


/***********************************************************************************
**	�f�B�X�v���C���X�g
**		DisListLegBone			: �r�����N�̃��X�g
**		DisListMap				: �n�}�̃��X�g 050913 doi
***********************************************************************************/
	int		DisListLegBone[4];
//	int		DisListMap[4];

/*******************************************************************
**	�摜�\������n�}�p�^�[��
*******************************************************************/
//	MAPPATTERN		MapDrawPattern;

/*****************************************************************************
**	�`��t���O
**		IsDisListPrepared		: �f�B�X�v���C���X�g���������ꂽ���ǂ���
**		IsMapLoaded				: �n�}��񂪓ǂݍ��܂ꂽ��
**		IsGridDrawn				: �i�q��`�悷�邩
**		IsSupportPolygonDrawn	: �x���r���p�`��`�悷�邩
**		IsAxisDrawn				: ���W�n��`�悷�邩
//**		IsMapDrawn				: �i�q�n�}��`�悷�邩
**		IsCellMapDrawn			: ���n�֎~�n�}��`�悷�邩
**		IsTerrainDrawn			: �n�`��`�悷�邩
*****************************************************************************/
	bool	IsDisListPrepared;
	bool	IsGridDrawn;
	bool	IsSupportPolygonDrawn;
	bool	IsAxisDrawn;
	bool	IsCellMapDrawn;
	bool	IsTerrainDrawn;

/*******************************************************************
**	�}�E�X�R���g���[���I�u�W�F�N�g
*******************************************************************/
	MouseViewCtrl	MouseView;

protected:

/*************************
**						**
**		�����o�֐�		**
**						**
*************************/

public:
/*******************************************************************
**	�R���X�g���N�^�ƃf�X�g���N�^
*******************************************************************/
	TitanGraphics(TitanData* titan, OpenGL::ViewType);
	TitanGraphics();
	~TitanGraphics();

/*******************************************************************
**	������
*******************************************************************/
	void	InitTitanGraphics(void);
	void	InitEnvironment(void);
	void	InitDisplayList(void);

/*******************************************************************
**	�f�[�^�\�[�X�̐ݒ�
*******************************************************************/
	void	SetTitanData(TitanData* titan);
//	void	SetGridMap(GridMap* pGridMap_in);//050913 doi
//	void	SetCellMap(CellMap* pCellMap_in);//050913 doi

/*******************************************************************
**	���C�e�B���O�̐ݒ�
*******************************************************************/
	void	SetLighting(void);

/*******************************************************************
**	���_�̐ݒ�
*******************************************************************/
	void	SetSceneView(void);

/*******************************************************************
**	�n�}�̕`��ݒ�
*******************************************************************/
//	void	SetMapDrawPattern(MAPPATTERN pattern);

/*******************************************************************
**	�Z���n�}�̕`�捂����ݒ�
*******************************************************************/
//	void	SetCellMapHeight(double height);

/*******************************************************************
**	OpenGL�̃C���[�W�̃����_�����O
*******************************************************************/
	void	RenderScene(void);

/*******************************************************************
**	OpenGL�̃C���[�W�`��
*******************************************************************/
	void	DrawScene(void);

/*******************************************************************
**	Titan�̕`��
**		DrawTitan			: Titan�̑S�̕`��
**		DrawBody			: Titan�̖{�̕`��
**		DrawLeg				: Titan�̋r�`��
**		DrawLift			: Titan�̃G���W�����t�g�Ɖ�]�ǔ��@�\�`��
**		DrawDrill			: Titan�̃h�����`��
*******************************************************************/
	void	DrawTitan(void);
	void	DrawBody(void);
	void	DrawLeg(int legID);
	void	DrawLift(double LiftAngle, double CircleAngle);
	void	DrawDrill(double DrillAngle);
/*******************************************************************
**	���`�� 060525 doi
**		DrawArrow
*******************************************************************/
	void	DrawArrow(double x0_in, double y0_in, double z0_in, 
		double vx_in, double vy_in, double vz_in); 
	void	DrawArrow(void);


/*******************************************************************
**	�n�}�̕`��
*******************************************************************/
//	void	DrawMap(void);

/*******************************************************************
**	�x���r���p�`(�O�p�`or�l�p�`)�̕`��
*******************************************************************/
	void	DrawSupportPolygon(void);

/*******************************************************************
**	�O���b�h�̏���`��
*******************************************************************/
	void	DrawGridFloor(double x0, double y0, double x1, double y1,
						  double z,  double dx, double dy);
	void	DrawGridFloor(const Matrix& p, const Matrix& q, double dx, double dy);

/*******************************************************************
**	�`��t���O�̐ݒ�
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
**	�}�E�X�ɂ�鎋�_�ύX�̏�����
*******************************************************************/
	void	SetMouseViewPoint(int x, int y);

/*******************************************************************
**	�}�E�X�̃C�x���g���[�h���擾
*******************************************************************/
	MouseViewCtrl::DragMode GetMouseViewDragMode(void);

/*******************************************************************
**	�}�E�X�̃C�x���g���Z�b�g
*******************************************************************/
	void	SetMouseViewDragMode(MouseViewCtrl::DragMode mode);

/*******************************************************************
**	�}�E�X�̃h���b�O���[�h�ɂ��View�̕ύX
*******************************************************************/
	void	MouseViewTransform(int x, int y);

/*******************************************************************
**	���o���̒��ڐݒ�
*******************************************************************/
	void	SetMouseViewAzimuth(double azimuth);
	void	SetMouseViewElevation(double elevation);
	void	SetMouseViewDistance(double distance);

/*******************************************************************
**	���W�ϊ�(4x4)
*******************************************************************/
	void	TransformGLMatrix(const Matrix& matrix);

private:
/*******************************************************************
**	�n�}����`�悷�邽�߂̃w���p�[�֐�
*******************************************************************/
//	void	DrawCellMap(void);
//	void	DrawWireMap(void);
//	void	DrawPolygonMap(void);
//	void	DrawContourMap(void);
//	void	DrawCellMapOnGridMap(void);//050919doi
	void	DrawGrayPolygon(Matrix &p1, Matrix &p2, Matrix &p3);//�@���̌����ŔZ����ς����|���S��

protected:
};

#endif // __TitanGraphics_h__