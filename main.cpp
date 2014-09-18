#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <commctrl.h>//�X���C�_�[�p
#include <stdio.h>

#include "./titan11/Graphics/TitanGraphics.h"
#include "./titan11/Kinematics/Titan.h"
#include "./titan11/kinematics_util.h"
#include <GL/gl.h>
#include "GL/freeglut.h"


#include "resource.h"
#include "mouse_view.h"
#include "OGL.h"
#include "OGL2D.h"
#include "d_trajectory/d_robot_trajectory.h"

//TITAN��`������
TitanGraphics	TitanXIGra;//�`��N���X
TitanData		TitanXIViewData;//�\���p�̃L�l�}�e�B�N�X�f�[�^�A�N�Z�X
Titan			*TitanXI_1;//�^���w�v�Z�Ȃǂ̎���
Titan			*TitanXI_0;//������ԋL�^�p�i�X���C�_�쓮�Ɏg�p)

HINSTANCE hInstance; //�E�C���h�E�P
HINSTANCE hInstance2; //�E�C���h�E�Q

OGL ogl1;//OpenGL���p���C�u����
MouseView mv1;//�}�E�X���[�V�����Ǘ��N���X

double ViewTheta;
double ViewPhi;
double WH_ratio;//��ʂ̏c����
int W_Height=600;//�E�C���h�E����.�`��Ŏg���B
int W_Width=900;//�E�C���h�E��.�`��Ŏg���B

char t11messagebuf[5][256];//�e�r�̏�ԕ\��
char EnableWalkFlag=0;//���s����t���O
int CurrentTime=0;
int TimerIntervalMsec=50;

//-------------------
void init_draw_titan()
{
	// ������
	TitanXI_1=new Titan;
	TitanXI_0=new Titan;
	//�����p��
	//TitanXI_1->SetLegPattern(0,Robot::RECTANGLE);
	//TitanXI_0->SetLegPattern(0,Robot::RECTANGLE);
	TitanXI_1->SetLegPattern(0,Robot::RETRACT);
	TitanXI_0->SetLegPattern(0,Robot::RETRACT);

	TitanXIViewData.UpdateRobotData(*TitanXI_1);
	TitanXIViewData.InitTitanData();
	TitanXIGra.SetTitanData(&TitanXIViewData);
	TitanXIGra.InitDisplayList();
	for(int i=0;i<4;i++)
    {
        sprintf((char*)(&t11messagebuf[i][0]),"-");
    }
}
//---------------------
void ogl_2d_tri_test(double x0, double y0)//�O�p�`�J�[�\�� 2D�`��
{
    double r=0.1;
    ogl_2d_triangle(x0,y0,x0-0.5*r,y0-r,x0+0.5*r,y0-r);
}
//---------------------
void disp_overlay( void ) //2D-overlay�`��D�}�E�X�Y�[���Ƃ͖��֌W�ɃT�C�Y�Œ�D
{

//�}�E�X�Y�[���ɖ��֌W�̌Œ�g�B�񎟌��w��
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glLoadIdentity();
    glColor3f(0.5f,0.5f,0.5f);
    glRasterPos2d(0,0);
	ogl_2d_tri_test(0,-0.1);//�O�p�`�̃J�[�\��

    //ogl_2d_tri_test_p((double)cursor_pos_global[0]/10,(double)cursor_pos_global[1]/10);//�O�p�`�J�[�\��
    //ogl_2d_circle_test_p((double)cursor_pos_global[0]/10,(double)cursor_pos_global[1]/10);//�~�`�J�[�\��


    //�ʒu�Œ�e�L�X�g
    void *font1=GLUT_BITMAP_9_BY_15;
    glRasterPos2d(-0.9,0.9);//��ʍ��W�Ń��b�Z�[�W�\���ʒu�w��
    //glutBitmapString(font1, reinterpret_cast<const unsigned char*>("Doi Robotics Lab."));
    //glutBitmapString(font1, (unsigned char *)("Doi Robotics Lab.2014.8.26"));

    glRasterPos2d(-0.9,0.9);//��ʍ��W�Ń��b�Z�[�W�\���ʒu�w��
    glutBitmapString(font1, (unsigned char *)(&t11messagebuf[0][0]));
    glRasterPos2d(-0.9,0.8);//��ʍ��W�Ń��b�Z�[�W�\���ʒu�w��
    glutBitmapString(font1, (unsigned char *)(&t11messagebuf[1][0]));
    glRasterPos2d(-0.9,0.7);//��ʍ��W�Ń��b�Z�[�W�\���ʒu�w��
    glutBitmapString(font1, (unsigned char *)(&t11messagebuf[2][0]));
    glRasterPos2d(-0.9,0.6);//��ʍ��W�Ń��b�Z�[�W�\���ʒu�w��
    glutBitmapString(font1, (unsigned char *)(&t11messagebuf[3][0]));
    glRasterPos2d(-0.9,0.5);//��ʍ��W�Ń��b�Z�[�W�\���ʒu�w��
    glutBitmapString(font1, (unsigned char *)(&t11messagebuf[4][0]));


  glPopMatrix();

    //�ړ�����e�L�X�g
    glRasterPos3d(3000,0,0);//�O�������W�Ń��b�Z�[�W�\���ʒu�w��
    glutBitmapString(font1, (unsigned char *)("(3000,0,0)"));
    glRasterPos3d(0,3000,0);//�O�������W�Ń��b�Z�[�W�\���ʒu�w��
    glutBitmapString(font1, (unsigned char *)("(0,3000,0)"));
    glRasterPos3d(0,0,3000);//�O�������W�Ń��b�Z�[�W�\���ʒu�w��
    glutBitmapString(font1, (unsigned char *)("(0,0,3000)"));


}
//-------------------------------------
void display()
{
//���_�̌���

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
double wh_ratio;//�@w/h�̔�
wh_ratio=(double)W_Width/(double)W_Height;
ogl1.set_viewpoint(mv1.distance,mv1.azimuth,mv1.elevation,
  mv1.v_center[0], mv1.v_center[1], mv1.v_center[2], wh_ratio);

    disp_overlay();//�񎟌��`��D�O�����`��ɏd�˂�`
	ogl1.set_light();//�Ɩ��Z�b�g

	//TitanXIGra.RenderScene();//TITAN XI�`��DDC�⃉�C�e�B���O�ȂǁC�`�悷�ׂĂ��s��

	TitanXIGra.DrawGridFloor(-10000,-10000,10000,10000,-1200,2000,2000);//�O���b�h�`��
	TitanXIGra.DrawCoordinateAxis(3000,5);
	TitanXIGra.DrawTitan();//TITAN XI�̂ݕ`��

	glFlush();
	glutSwapBuffers();
}
//-------------------
BOOL CALLBACK DialogProc2(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND hSlider[12];
    static int nSlider[]={50,50,50, 50,50,50, 50,50,50, 50,50,50};
	Matrix pos[4];//�r��ʒu�x�N�g��
	Matrix ang(3);//�֐ߊp
	double clen[3];//�V�����_����


	switch(uMsg)
    {

        case WM_INITDIALOG:
			//�X���C�_�̏����ݒ�
             hSlider[0]=GetDlgItem(hwndDlg,IDC_SLIDER1);
             hSlider[1]=GetDlgItem(hwndDlg,IDC_SLIDER2);
             hSlider[2]=GetDlgItem(hwndDlg,IDC_SLIDER3);
             hSlider[3]=GetDlgItem(hwndDlg,IDC_SLIDER4);
             hSlider[4]=GetDlgItem(hwndDlg,IDC_SLIDER5);
             hSlider[5]=GetDlgItem(hwndDlg,IDC_SLIDER6);
             hSlider[6]=GetDlgItem(hwndDlg,IDC_SLIDER7);
             hSlider[7]=GetDlgItem(hwndDlg,IDC_SLIDER8);
             hSlider[8]=GetDlgItem(hwndDlg,IDC_SLIDER9);
             hSlider[9]=GetDlgItem(hwndDlg,IDC_SLIDER10);
             hSlider[10]=GetDlgItem(hwndDlg,IDC_SLIDER11);
             hSlider[11]=GetDlgItem(hwndDlg,IDC_SLIDER12);
			 for(int i=0;i<12;i++)
			 {
				SendMessage(
                hSlider[i], TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 200)
				);
				SendMessage(hSlider[i], TBM_SETPOS, (WPARAM)TRUE, 100);
			 }
			 //�^���w���C�u����������
			InitKinematicsUtil();
			return TRUE;

        case WM_CLOSE:
            EndDialog(hwndDlg, 0);
            return TRUE;

         case WM_HSCROLL://�����X���C�_�̂ǂꂩ������
			 //printf("HSCROLL:");//debug
			for(int i=0;i<12;i++)
			{
            nSlider[i]=(int) SendMessage(hSlider[i], TBM_GETPOS, 0, 0);
			nSlider[i]-=100;//�����l���[���ɂȂ�悤�ɒl�V�t�g
			nSlider[i]*=20;//�ω��ʊg��
			}
			/*
            printf("Slider[%d, %d, %d,  %d,%d,%d,  %d,%d,%d,  %d,%d,%d]\n",
				nSlider[0], nSlider[1], nSlider[2], nSlider[3],
				nSlider[4], nSlider[5], nSlider[6], nSlider[7],
				nSlider[8], nSlider[9], nSlider[10], nSlider[11]);
			*/

			double temp[3];
			//�����ʒu�擾
			for(int i=0;i<4;i++)
			{
				pos[i].SetSize(4,4);//�����ϊ��s��i4x4�j�ŗp��
				pos[i]=TitanXI_0->GetLegTransformMatrixInGlobal(i+1,4);
				//GetPos(i+1,&temp[0], &temp[1], &temp[2]);
				//pos[i](1)=temp[0];pos[i](2)=temp[1];pos[i](3)=temp[2];
			}

			//�����ʒu�ɃX���C�_�ω��ʉ��Z
			for(int i=0;i<4;i++)
			{
			pos[i](1,4)+=nSlider[3*i+0]; pos[i](2,4)+=nSlider[3*i+1]; pos[i](3,4)+=nSlider[3*i+2];
			}

			KINE res;
			//Titan�\���̂ɔ��f
			for(int i=0;i<4;i++)
			{
				//TitanXI_1->SetFootConfiguration(i+1,pos[i]);//��[�ʒu�ݒ��IK+DK
				SetIK(i+1,pos[i](1,4),pos[i](2,4),pos[i](3,4));
				GetAng(i+1,&temp[0], &temp[1], &temp[2]);
				ang(1)=temp[0];ang(2)=temp[1];ang(3)=temp[2];
				res=TitanXI_1->SetLegJntAng(i+1,ang);

				//�V�����_�����\��
				GetCylinder(i+1,&clen[0],&clen[1],&clen[2]);
				//printf("DialogProc2():Leg%d-CldrLen[%.2f,%.2f,%.2f]\n",
				//	i+1,clen[0],clen[1],clen[2]);
				sprintf((char*)(&t11messagebuf[i][0]),
            "Leg%d-Pos[%5.0f,%5.0f,%5.0f,]: CldrLen[%4.0f,%4.0f,%4.0f] :Ang(deg)[%4.1f, %4.1f, %4.1f] - DialogProc2()\n",
					i+1, pos[i](1,4), pos[i](2,4),pos[i](3,4),
                            clen[0],clen[1],clen[2],
                            ang(1)*180/M_PI,ang(2)*180/M_PI,ang(3)*180/M_PI);

			}


			//�`��p�f�[�^�\���̂ɔ��f
			TitanXIViewData.UpdateRobotData(*TitanXI_1);
			  	glutPostRedisplay();


           return TRUE;
        case WM_VSCROLL://�����X���C�_�̂ǂꂩ������
            printf("VSCROLL");
            nSlider[1]=(int) SendMessage(hSlider[1], TBM_GETPOS, 0, 0);
            //printf("Slider[1]=%d\n",nSlider[1]);
           return TRUE;
       }
	return FALSE;
}
//-----------------------
void mbutton(int button , int state , int x , int y) //�}�E�X�{�^�����������A�グ�C�x���g
{
//printf("mouse event x[%d], y[%d]\n",x,y);
//int xPos=x;
//int yPos=y;
  mv1.drag_mode=DRAG_MODE_STOP;

	if (state== GLUT_DOWN)
          {
            if(button==GLUT_LEFT_BUTTON)
              {
				//printf("LBUTTONDOWN xPos=%d, yPos=%d\n",xPos,yPos);
				//TitanXIGra.SetMouseViewDragMode(MouseViewCtrl::SCALE);
				mv1.drag_mode=DRAG_MODE_ZOOM;
              }
            if(button==GLUT_MIDDLE_BUTTON)
              {
				//printf("MBUTTONDOWN xPos=%d, yPos=%d\n",xPos,yPos);
				//TitanXIGra.SetMouseViewDragMode(MouseViewCtrl::ROTATE);
                mv1.drag_mode=DRAG_MODE_ROTATE;
             }
            if(button==GLUT_RIGHT_BUTTON)
              {
				//printf("RBUTTONDOWN xPos=%d, yPos=%d\n",xPos,yPos);
				//TitanXIGra.SetMouseViewDragMode(MouseViewCtrl::TRANSLATE);
                mv1.drag_mode=DRAG_MODE_TRANSLATE;
              }
				//TitanXIGra.SetMouseViewPoint(xPos,yPos);
            mv1.init(x,y);
            mv1.flag_drag=1;
          }
  	if (state== GLUT_UP)
          {
            mv1.flag_drag=0;
			 //TitanXIGra.SetMouseViewDragMode(MouseViewCtrl::STOP);

          }
  glutPostRedisplay();
}
//-----------------------
void mmotion(int x , int y) //�}�E�X�ړ��C�x���g
{
	int xPos=x;
	int yPos=y;
	if(mv1.flag_drag!=1)return;
	//printf("(x,y)=(%d,%d)",x,y);
	//TitanXIGra.MouseViewTransform(xPos,yPos);
  if(mv1.drag_mode==DRAG_MODE_ROTATE)
    {
      mv1.Rotate(x,y);
    }
    if(mv1.drag_mode==DRAG_MODE_ZOOM)
    {
      mv1.Zoom(x,y);
    }
    if(mv1.drag_mode==DRAG_MODE_TRANSLATE)
    {
      mv1.Translate(x,y);
    }
  	glutPostRedisplay();
}

//-----------------------
//�A�j���[�V��������
void timer1(int value)
{
	int interval=TimerIntervalMsec;
	int res;
	Matrix body_temp(4,4);

	body_temp=TitanXI_1->GetBodyTransform();
	sprintf((char*)(&t11messagebuf[4][0]),
         "Body Pos(%.0f, %.0f, %.0f)-timer1()",
         body_temp(1,4),body_temp(2,4),body_temp(3,4));

	//�����ɃA�j���[�V��������������
	if(EnableWalkFlag==1)
	{
		CurrentTime+=interval;

		Matrix pos[4];//�r��ʒu�x�N�g��
        Matrix ang(3);//�֐ߊp
        double clen[3];//�V�����_����
        double temp[3];
        double p[4][3];//�r��ʒu
        int i;
        for(i=0;i<4;i++)
        {
        pos[i].SetSize(4,4);//�����ϊ��s��i4x4�j�ŗp��
    //	pos[i]=TitanXI_0->GetLegTransformMatrixInGlobal(i+1,4);
        }


        //�O�������֐�������o��
        double speed=10;
        res=RTget_trajectory((CurrentTime*0.001*speed),
        &(p[0][0]), &(p[0][1]), &(p[0][2]),
        &(p[1][0]), &(p[1][1]), &(p[1][2]),
        &(p[2][0]), &(p[2][1]), &(p[2][2]),
        &(p[3][0]), &(p[3][1]), &(p[3][2])
        );
        if(res<0)
        {
            printf("timer1() Error RTget_trajectory()=%d, CurrentTime=%d\n",
                   res,CurrentTime);
                   EnableWalkFlag=0;
            return;
        }

        for(i=0;i<4;i++)//�^���w�v�Z�ƃO���t�B�b�N�ɔ��f
        {
		pos[i](1,4)=p[i][0];
		pos[i](2,4)=p[i][1];
		pos[i](3,4)=p[i][2];
		SetIK(i+1,pos[i](1,4),pos[i](2,4),pos[i](3,4));
		GetAng(i+1,&temp[0], &temp[1], &temp[2]);
		ang(1)=temp[0];ang(2)=temp[1];ang(3)=temp[2];
		res=TitanXI_1->SetLegJntAng(i+1,ang);

		//�V�����_�����\��
		GetCylinder(i+1,&clen[0],&clen[1],&clen[2]);
		sprintf((char*)(&t11messagebuf[i][0]),
           "Leg%d-Pos[%5.0f,%5.0f,%5.0f,]: CldrLen[%4.0f,%4.0f,%4.0f] :Ang(deg)[%4.1f, %4.1f, %4.1f] - timer1()\n",
				i+1, pos[i](1,4), pos[i](2,4),pos[i](3,4),
                clen[0],clen[1],clen[2],
                ang(1)*180/M_PI,ang(2)*180/M_PI,ang(3)*180/M_PI);
		printf("debug-%s",(char*)(&t11messagebuf[i][0]));//debug
        }

        //�`��p�f�[�^�\���̂ɔ��f
        TitanXIViewData.UpdateRobotData(*TitanXI_1);
        display();

	glutTimerFunc(interval,timer1,0);
	}
}
//-----------------------
void keyf(unsigned char key , int x , int y)//��ʃL�[����
{
	//printf("key[%c],x[%d],y[%d]\n",key,x,y);
    switch(key)
    {
    case 'q':
    case 'Q':
    case '\033':  /* '\033' �� ESC �� ASCII �R�[�h */
        {
            printf("Exit\n");
            exit(0);
            break;
        }
     case 'w':
    	{
    		if(EnableWalkFlag==0)
    		{
    			EnableWalkFlag=1;
    			CurrentTime=0;
    			RTSetTrajectoryICCRAWL();//�ԟ[�N���[���O���Z�b�g
    			printf("ICCRAWL Set\n");
    			printf("START Walk(EnableWalkFlag=%d)\n",EnableWalkFlag);
    			glutTimerFunc(TimerIntervalMsec,timer1,0);
    		}
    		else
    		{
    			EnableWalkFlag=0;
	    		printf("STOP Walk(EnableWalkFlag=%d)\n",EnableWalkFlag);
    		}
    		break;
    	}
    case 's'://�X�^���o�C
    		if(EnableWalkFlag==0)
    		{
    			EnableWalkFlag=1;
    			CurrentTime=0;
    			RTSetTrajectoryICCRAWL_standby();//�ԟ[�N���[�������O���Z�b�g
    			printf("ICCRAWL-standby Set\n");
    			printf("START Walk(EnableWalkFlag=%d)\n",EnableWalkFlag);
    			glutTimerFunc(TimerIntervalMsec,timer1,0);
    		}
    		else
    		{
    			EnableWalkFlag=0;
	    		printf("STOP Walk(EnableWalkFlag=%d)\n",EnableWalkFlag);
    		}
    		break;
    default:
        break;
    }
    glutPostRedisplay();
}
/*
GLUT_KEY_F1	F1 function key
GLUT_KEY_F2	F2 function key
GLUT_KEY_F3	F3 function key
GLUT_KEY_F4	F4 function key
GLUT_KEY_F5	F5 function key
GLUT_KEY_F6	F6 function key
GLUT_KEY_F7	F7 function key
GLUT_KEY_F8	F8 function key
GLUT_KEY_F9	F9 function key
GLUT_KEY_F10	F10 function key
GLUT_KEY_F11	F11 function key
GLUT_KEY_F12	F12 function key
GLUT_KEY_LEFT	�����L�[
GLUT_KEY_UP	����L�[
GLUT_KEY_RIGHT	�E���L�[
GLUT_KEY_DOWN	�����L�[
GLUT_KEY_PAGE_UP	Page up �L�[
GLUT_KEY_PAGE_DOWN	Page down �L�[
GLUT_KEY_HOME	Home �L�[
GLUT_KEY_END	End �L�[
GLUT_KEY_INSERT	Inset �L�[
*/

void keyf2(int key , int x , int y)//����L�[�̈���
{
	switch(key)
	{
		case GLUT_KEY_UP:
			printf("GLUT_KEY_UP\n");
                        break;
		case GLUT_KEY_DOWN:
			printf("GLUT_KEY_DOWN\n");
                        break;
		case GLUT_KEY_RIGHT:
			printf("GLUT_KEY_RIGHT\n");
			break;
		case GLUT_KEY_LEFT:
			printf("GLUT_KEY_LEFT\n");
                        break;

	}
	printf("key[%c],x[%d],y[%d]\n",key,x,y);
  	glutPostRedisplay();
}
//-----------------------
void js_func(unsigned int buttonMask, int x, int y, int z)//�W���C�X�e�B�b�N�C�x���g
{
  /*
    GLUT_JOYSTICK_BUTTON_A
    GLUT_JOYSTICK_BUTTON_B
    GLUT_JOYSTICK_BUTTON_C
    GLUT_JOYSTICK_BUTTON_D
    */
  printf("Joystick x,y,z=(%d,%d,%d)\n",x,y,z);
  /*
  switch (buttonMask)
    {
    case GLUT_JOYSTICK_BUTTON_A:
      printf("A ON");
      break;
    case GLUT_JOYSTICK_BUTTON_B:
      printf("B ON");
      break;
    }
    */
  	glutPostRedisplay();
}

//-----------------------
void reshape(int width, int height)//�ĕ`��̊֐�
{
W_Width=width;
W_Height=height;
//printf("(width,height)=(%d,%d)\n",width,height);
glViewport(0,0,W_Width,W_Height);
  	glutPostRedisplay();
}
//-----------------------

int main(int argc , char ** argv)
{
	printf(" TITAN XI Simulator  \n");
	printf("---------------------\n");
	printf("press[s] to standby  \n");
	printf("press[w] to walk/stop\n");

	//TitanXIGra.InitOpenGL(hwndDlg);//�ŏ���1�񂾂�
	CreateDialog(hInstance2, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc2);//�_�C�A���O�Q

	int id;

 	glutInit(&argc , argv);
	glutInitWindowPosition(200 , 50);
	glutInitWindowSize((int)W_Width , (int)W_Height);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	mv1.distance=30000;
	mv1.azimuth=170;
	mv1.elevation=80;



	id=glutCreateWindow("TITAN-XI GLUT test 2014.9.18");

	init_draw_titan();//TITAN �`�揀��
	InitKinematicsUtil();//�^���w�v�Z����



    glutDisplayFunc(display);//�`�揈��
	glutKeyboardFunc(keyf);//�L�[���̓C�x���g
	glutSpecialFunc(keyf2);//����L�[���̓C�x���g
	glutReshapeFunc(reshape);//�ĕ`��C�x���g
	glutJoystickFunc(js_func,100);//�W���C�X�e�B�b�N�C�x���g
	//glutTimerFunc(TimerIntervalMsec,timer1,0);

	glutMouseFunc(mbutton);//�}�E�X�{�^�����������A�グ
	glutMotionFunc(mmotion);//�}�E�X�ړ��C�x���g

	glutMainLoop();
	return 0;

}
