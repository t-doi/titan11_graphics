/*****************************************************************************
**
**	File:	RobotData.cpp
** 
**	説明:	ロボットオブジェクトのデータハンドラー
**
**	Created: 2004/11/18(Thu)	Updated: 2005/01/14(Fri)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

#include "RobotData.h"

/***********************************************************************
** デフォルトコンストラクタ
***********************************************************************/
RobotData::RobotData()
{
	InitRobotData();
	ResetRobotData();
}

/***********************************************************************
** コピーコンストラクタ
***********************************************************************/
RobotData::RobotData(const RobotData& data)
{
	InitRobotData();
	Copy(data);
}

/***********************************************************************
** デストラクタ
***********************************************************************/
RobotData::~RobotData()
{
}

/***********************************************************************
** 代入演算子
***********************************************************************/
RobotData& RobotData::operator =(const RobotData& data)
{
	if ( &data != this)
	{
		Copy(data);
	}

	return (*this);

}

/***********************************************************************
** 等価演算子
***********************************************************************/
bool RobotData::operator ==(const RobotData& data)
{
	return ( IsEqual(data)  );
}

/***********************************************************************
** 非等価演算子
***********************************************************************/
bool RobotData::operator !=(const RobotData& data)
{
	return !( IsEqual(data) );
}

/***********************************************************************
** データの初期化
***********************************************************************/
void RobotData::ResetRobotData(void)
{
	for (int i=0; i<LEG_NUM; i++)
	{
		for (int j=0; j<(JOINT_NUM+2); j++)
		{
			LegTransform[i][j].LoadZero();
		}

		FootPosition[i].LoadZero();
		AnklePosition[i].LoadZero();
		KneePosition[i].LoadZero();
		HipPosition[i].LoadZero();
		BasePosition[i].LoadZero();
		JointAngle[i].LoadZero();
		CylinderLength[i].LoadZero();
		CylinderThrust[i].LoadZero();
		LegPhases[i] = Robot::SUPPORT;
	}
	
	BodyPosition.LoadZero();
	BodyPosture.LoadZero();
	BodyTransform.LoadZero();

	return;
}

/***********************************************************************
** アクセッサ(データを取得)
***********************************************************************/
RobotData* RobotData::Clone(void) const
{
	return ( new RobotData(*this) );
}


/***********************************************************************
** データを更新
***********************************************************************/
void RobotData::UpdateRobotData(const RobotData& data)
{
	Copy(data);

	return;
}


/***********************************************************************
** データを更新
***********************************************************************/
void RobotData::UpdateRobotData(const Robot& robot)
{
//  printf("UpdateRobotData()-1\n");//debug 071010  
int i,j;
        for (i=0; i<LEG_NUM; i++)
	{
//                  printf("UpdateRobotData()-1a-1 i=%d, j=%d\n",i,j);//debug 071010

                for (j=0; j<(JOINT_NUM+2); j++)
		{
//                  printf("UpdateRobotData()-1a-2 i=%d, j=%d\n",i,j);//debug 071010                  
			LegTransform[i][j] = robot.GetLegTransformMatrix( (i+1), j );

                }
//  printf("UpdateRobotData()-1a\n");//debug 071010
		FootPosition[i].LoadMatrix(	robot.GetLegTransformMatrixInGlobal(i+1, 4), 1, 4, 3, 4 );
		AnklePosition[i].LoadMatrix( robot.GetLegTransformMatrixInGlobal(i+1, 3), 1, 4, 3, 4 );
		KneePosition[i].LoadMatrix( robot.GetLegTransformMatrixInGlobal(i+1, 2), 1, 4, 3, 4 );
		HipPosition[i].LoadMatrix( robot.GetLegTransformMatrixInGlobal(i+1, 1), 1, 4, 3, 4 );
		BasePosition[i].LoadMatrix( robot.GetLegTransformMatrixInGlobal(i+1, 0), 1, 4, 3, 4 );
  //printf("UpdateRobotData()-1b\n");//debug 071010
                JointAngle[i]		= robot.GetLegJntAng(i+1);
		CylinderLength[i]	= robot.GetLegCldrLen(i+1);
		CylinderThrust[i]	= robot.GetLegCldrThrust(i+1);
		LegPhases[i]		= robot.GetLegPhase(i+1);
	}

//  printf("UpdateRobotData()-2\n");//debug 071010
	BodyTransform = robot.GetBodyTransform();
	
	BodyPosition.LoadMatrix( BodyTransform, 1, 4, 3, 4 );

	Matrix rot(3, 3); rot.LoadMatrix(BodyTransform, 1, 1, 3, 3);
	BodyPosture = RotMatrixtoRPY(rot);

	return;
}

/***********************************************************************
** データを更新
***********************************************************************/
//データ書き出し．060731 doi
KINE RobotData::SetToRobotDK(Robot* robot_out)
{
	//DKベースでデータコピー
	//胴体の位置セット
	KINE kine;

       //printf("SetToRobotDK()\n");//debug
    
	robot_out->SetMotionStyle(Robot::CRAWLER);
	kine=robot_out->SetBodyTransform(BodyTransform);
	
	if(kine!=NoKineError)
        {
            printf("SetToRobotDK()KineError#%d",kine);//debug
            return kine;
        }
//       printf("SetToRobotDK() - Body set\n");//debug
	//脚関節角セットと順運動学
	for (int i=0; i<LEG_NUM; i++)
	{
		robot_out->SetLegPhase(i+1,LegPhases[i]);//支持脚か遊脚か

                //for(int j=0;j<JOINT_NUM;j++)
                {
                    robot_out->SetLegJntAng(i+1,JointAngle[i]);
                    if(kine!=NoKineError)
                    {
                    printf("SetToRobotDK()KineError#%d in Leg%d",kine,i+1);//debug
                    return kine;
                    }
                }
	}
    
  //     printf("SetToRobotDK() -end\n");//debug
	return kine;
}

//データ書き出し．060801 doi
KINE RobotData::SetToRobotIK(Robot* robot_out)
{
	//IKベースでデータコピー
	//胴体の位置セット
	KINE kine;

//        printf("SetToRobotIK()\n");//debug

//    printf("BodyTransform(%.0f, %.0f, %.0f)\n", BodyTransform(1,4), BodyTransform(2,4), BodyTransform(3,4));
    
	robot_out->SetMotionStyle(Robot::CRAWLER);
	kine=robot_out->SetBodyTransform(BodyTransform);
	if(kine!=NoKineError)
        {
            printf("SetToRobotIK() Error#%d\n",kine);
            return kine;
        }

        Matrix foot(DH_NUM, DH_NUM);
	//脚関節角セットと逆運動学
	for (int i=0; i<LEG_NUM; i++)
	{
		robot_out->SetLegPhase(i+1,LegPhases[i]);//支持脚か遊脚か
                foot=robot_out->TransformationLocaltoGlobal(GetLegTransform(i+1,4));
		//robot_out->SetLegConfiguration(i+1,foot);//足裏同時変換行列をセット．IK+DK
		robot_out->SetLegEndPos(i+1,foot);//足裏同時変換行列をセット．IKのみ   
		if(kine!=NoKineError)
                {
                    printf("SetToRobotIK() Error#%d in Leg%d\n ",kine, i+1);
                    return kine;
                }
	}

	return kine;
}

/***********************************************************************
**	個別データのアクセッサ
***********************************************************************/
Matrix RobotData::GetLegTransform(int legID, int joint) const
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) || !IsParamWithin(joint, 0, (JOINT_NUM+1)) )
	{
		fprintf(stderr, "RobotData::GetFootPosition() Invalid parameter\n\n");
		
		return (ZERO_VECTOR_3);
	}

	return (LegTransform[legID-1][joint]);
}

Matrix RobotData::GetFootPosition(int legID) const
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "RobotData::GetFootPosition() Invalid leg ID\n\n");
		
		return (ZERO_VECTOR_3);
	}

	return (FootPosition[legID-1]);
}

Matrix RobotData::GetAnklePosition(int legID) const
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "RobotData::GetAnklePosition() Invalid leg ID\n\n");
		
		return (ZERO_VECTOR_3);
	}

	return (AnklePosition[legID-1]);
}

Matrix RobotData::GetKneePosition(int legID) const
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "RobotData::GetKneePosition() Invalid leg ID\n\n");
		
		return (ZERO_VECTOR_3);
	}

	return (KneePosition[legID-1]);
}
Matrix RobotData::GetHipPosition(int legID) const
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "RobotData::GetHipPosition() Invalid leg ID\n\n");
		
		return (ZERO_VECTOR_3);
	}

	return (HipPosition[legID-1]);
}
Matrix RobotData::GetBasePosition(int legID) const
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "RobotData::GetBasePosition() Invalid leg ID\n\n");
		
		return (ZERO_VECTOR_3);
	}

	return (BasePosition[legID-1]);
}
Matrix RobotData::GetJointAngle(int legID) const
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "RobotData::GetJointAngle() Invalid leg ID\n\n");
		
		return (ZERO_VECTOR_JOINT);
	}

	return (JointAngle[legID-1]);
}
Matrix RobotData::GetCylinderLength(int legID) const
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "RobotData::GetCylinderLength() Invalid leg ID\n\n");
		
		return (ZERO_VECTOR_JOINT);
	}

	return (CylinderLength[legID-1]);
}
Matrix RobotData::GetCylinderThrust(int legID) const
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "RobotData::GetCylinderThrust() Invalid leg ID\n\n");
		
		return (IDENTITY_MATRIX_DH);
	}

	return (CylinderThrust[legID-1]);
}

Robot::LegPhase RobotData::GetLegPhase(int legID) const
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "RobotData::GetLegPhase() Invalid leg ID\n\n");
	}

	return ( LegPhases[legID-1] );
}

Matrix RobotData::GetBodyPosition(void) const
{
	return (BodyPosition);
}

Matrix RobotData::GetBodyPosture(void) const
{
	return (BodyPosture);
}

Matrix RobotData::GetBodyTransform(void) const
{
	return (BodyTransform);
}

/***********************************************************************
**	個別データのセッタ
***********************************************************************/
void RobotData::SetLegTransform(int legID, int joint, const Matrix& transform)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) || !IsParamWithin(joint, 0, (JOINT_NUM+1)) ||
		transform.GetRow() != DH_NUM || transform.GetCol() != DH_NUM )
	{
//		fprintf(stderr, "RobotData::SetFootPosition() Invalid parameter\n\n");
		fprintf(stderr, "RobotData::SetLegTransform() Invalid parameter\n\n");            
		return;
	}

	LegTransform[legID-1][joint] = transform;
	return;
}

void RobotData::SetFootPosition(int legID, const Matrix& foot)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "RobotData::SetFootPosition() Invalid leg ID\n\n");
		return;
	}

	FootPosition[legID-1] = foot;
	return;
}
void RobotData::SetAnklePosition(int legID, const Matrix& ankle)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "RobotData::SetAnklePosition() Invalid leg ID\n\n");
		return;
	}

	if ( ankle.GetRow() != 3 || ankle.GetCol() != 1 )
	{
		fprintf(stderr, "RobotData::SetAnklePosition() Invalid Matrix\n\n");
		return;
	}

	AnklePosition[legID-1] = ankle;
	return;
}
void RobotData::SetKneePosition(int legID, const Matrix& knee)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "RobotData::SetKneePosition() Invalid leg ID\n\n");
		return;
	}

	if ( knee.GetRow() != 3 || knee.GetCol() != 1 )
	{
		fprintf(stderr, "RobotData::SetKneePosition() Invalid Matrix\n\n");
		return;
	}

	KneePosition[legID-1] = knee;
	return;
}
void RobotData::SetHipPosition(int legID, const Matrix& hip)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "RobotData::SetHipPosition() Invalid leg ID\n\n");
		return;
	}

	if ( hip.GetRow() != 3 || hip.GetCol() != 1 )
	{
		fprintf(stderr, "RobotData::SetHipPosition() Invalid Matrix\n\n");
		return;
	}

	HipPosition[legID-1] = hip;
}

void RobotData::SetBasePosition(int legID, const Matrix& base)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "RobotData::SetBasePosition() Invalid leg ID\n\n");
		return;
	}

	if ( base.GetRow() != 3 || base.GetCol() != 1 )
	{
		fprintf(stderr, "RobotData::SetBasePosition() Invalid Matrix\n\n");
		return;
	}

	BasePosition[legID-1] = base;
	return;
}

void RobotData::SetJointAngle(int legID, const Matrix& angle)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "RobotData::SetJointAngle() Invalid leg ID\n\n");
		return;
	}

	if ( angle.GetRow() != JOINT_NUM || angle.GetCol() != 1 )
	{
		fprintf(stderr, "RobotData::SetJointAngle() Invalid Matrix\n\n");
		return;
	}

	JointAngle[legID-1] = angle;
	return;
}

void RobotData::SetCylinderLength(int legID, const Matrix& length)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "RobotData::SetCylinderLength() Invalid leg ID\n\n");
		return;
	}

	if ( length.GetRow() != JOINT_NUM || length.GetCol() != 1 )
	{
		fprintf(stderr, "RobotData::SetCylinderLength() Invalid Matrix\n\n");
		return;
	}

	CylinderLength[legID-1] = length;
}

void RobotData::SetCylinderThrust(int legID, const Matrix& thrust)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "RobotData::SetCylinderThrust() Invalid leg ID\n\n");
		return;
	}

	if ( thrust.GetRow() != JOINT_NUM || thrust.GetCol() != 1 )
	{
		fprintf(stderr, "RobotData::SetCylinderThrust() Invalid Matrix\n\n");
		return;
	}

	CylinderThrust[legID-1] = thrust;
}

void RobotData::SetLegPhase(int legID, Robot::LegPhase phase)
{
	if ( !IsParamWithin(legID, 1, LEG_NUM) )
	{
		fprintf(stderr, "RobotData::SetLegPhase() Invalid leg ID\n\n");
		return;
	}

	LegPhases[legID-1] = phase;
	return;
}

void RobotData::SetBodyPosition(const Matrix& position)
{
	if ( position.GetRow() != 3 || position.GetCol() != 1 )
	{
		fprintf(stderr, "RobotData::SetBodyPosition() Invalid Matrix\n\n");
		return;
	}

	BodyPosition = position;
}

void RobotData::SetBodyPosture(const Matrix& posture)
{
	if ( posture.GetRow() != 3 || posture.GetCol() != 1 )
	{
		fprintf(stderr, "RobotData::SetBodyPosture() Invalid Matrix\n\n");
		return;
	}

	BodyPosture = posture;
}

void RobotData::SetBodyTransform(const Matrix& transform)
{
	if ( transform.GetRow() != DH_NUM || transform.GetCol() != DH_NUM )
	{
		fprintf(stderr, "RobotData::SetBodyTransform() Invalid Matrix\n\n");
		return;
	}

	BodyTransform = transform;
}

/*******************************************************************
**  代入演算子とコピーコンストラクタから呼ばれる
**	コピーのヘルパー関数
*******************************************************************/
void RobotData::Copy(const RobotData& data)
{
	for (int i=0; i<LEG_NUM; i++)
	{
		for (int j=0; j<(JOINT_NUM+2); j++)
		{
			LegTransform[i][j] = data.GetLegTransform( (i+1), j);
		}

		FootPosition[i]		= data.GetFootPosition(i+1);
		AnklePosition[i]	= data.GetAnklePosition(i+1);
		KneePosition[i]		= data.GetKneePosition(i+1);
		HipPosition[i]		= data.GetHipPosition(i+1);
		BasePosition[i]		= data.GetBasePosition(i+1);
		JointAngle[i]		= data.GetJointAngle(i+1);
		CylinderLength[i]	= data.GetCylinderLength(i+1);
		CylinderThrust[i]	= data.GetCylinderThrust(i+1);
		LegPhases[i]		= data.GetLegPhase(i+1);
	}
	
	BodyPosition			= data.GetBodyPosition();
	BodyPosture				= data.GetBodyPosture();

	BodyTransform = data.GetBodyTransform();

	return;
}

/*******************************************************************
**  等価演算子と非等価演算子から呼ばれる
**	ヘルパー関数
*******************************************************************/
bool RobotData::IsEqual(const RobotData& data)
{
  int i,j,k,l,m;
	for (i=0; i<LEG_NUM; i++)
	{
		for (j=1; j<=POS_DOF; j++)
		{
			if ( FootPosition[i](j)		!= data.GetFootPosition(i+1)(j) )	{return (false);}
			if ( AnklePosition[i](j)	!= data.GetAnklePosition(i+1)(j) )	{return (false);}
			if ( KneePosition[i](j)		!= data.GetKneePosition(i+1)(j) )	{return (false);}
			if ( HipPosition[i](j)		!= data.GetHipPosition(i+1)(j) )	{return (false);}
			if ( BasePosition[i](j)		!= data.GetBasePosition(i+1)(j) )	{return (false);}
			if ( JointAngle[i](j)		!= data.GetJointAngle(i+1)(j) )		{return (false);}
			if ( CylinderLength[i](j)	!= data.GetCylinderLength(i+1)(j) )	{return (false);}
			if ( CylinderThrust[i](j)	!= data.GetCylinderThrust(i+1)(j) )	{return (false);}
		}

		for (k=0; k<(JOINT_NUM+2); k++)
		{
			for (l=1; l<=DH_NUM; l++)
			{
				for (m=1; m<=DH_NUM; m++)
				{
					if ( LegTransform[i][k](l, m) != data.GetLegTransform( (i+1), k )(l, m) ) {return (false);}
				}
			}
		}

		if ( LegPhases[i] != data.GetLegPhase(i+1) ) {return (false);}
	}
	
	for (i=1; i<=POS_DOF; i++)
	{
		if ( BodyPosition(i)!= data.GetBodyPosition()(i) )		{return (false);}
		if ( BodyPosture(i)	!= data.GetBodyPosture()(i) )		{return (false);}
	}

	for (j=1; j<=DH_NUM; j++)
	{
		for (k=1; k<=DH_NUM; k++)
		{
			if ( BodyTransform(j, k) != data.GetBodyTransform()(j, k) )
				return (false);
		}
	}

	return (true);
}

/***********************************************************************
** 初期化
***********************************************************************/
void RobotData::InitRobotData(void)
{	
	// 行列の要素数確保
	for (int i=0; i<LEG_NUM; i++)
	{
		for (int j=0; j<(JOINT_NUM+2); j++)
		{
			LegTransform[i][j].SetSize(DH_NUM, DH_NUM);
		}

		FootPosition[i].SetSize(POS_DOF);
		AnklePosition[i].SetSize(POS_DOF);
		KneePosition[i].SetSize(POS_DOF);
		HipPosition[i].SetSize(POS_DOF);
		BasePosition[i].SetSize(POS_DOF);
		JointAngle[i].SetSize(POS_DOF);
		CylinderLength[i].SetSize(POS_DOF);
		CylinderThrust[i].SetSize(POS_DOF);
	}
	
	BodyPosition.SetSize(POS_DOF);
	BodyPosture.SetSize(POS_DOF);
	BodyTransform.SetSize(DH_NUM, DH_NUM);

	return;
}
