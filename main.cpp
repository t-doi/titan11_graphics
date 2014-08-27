#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <commctrl.h>//スライダー用
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

//TITANを描く準備
TitanGraphics	TitanXIGra;//描画クラス
TitanData		TitanXIViewData;//表示用のキネマティクスデータアクセス
Titan			*TitanXI_1;//運動学計算などの実体
Titan			*TitanXI_0;//初期状態記録用（スライダ駆動に使用)

HINSTANCE hInstance; //ウインドウ１
HINSTANCE hInstance2; //ウインドウ２

OGL ogl1;//OpenGL応用ライブラリ
MouseView mv1;//マウスモーション管理クラス

double ViewTheta;
double ViewPhi;
double WH_ratio;//画面の縦横比
int W_Height=500;//ウインドウ高さ.描画で使う。
int W_Width=500;//ウインドウ幅.描画で使う。


//-------------------
void init_draw_titan()
{
	// 初期化
	TitanXI_1=new Titan;
	TitanXI_0=new Titan;
	TitanXIViewData.UpdateRobotData(*TitanXI_1);
	TitanXIViewData.InitTitanData();
	TitanXIGra.SetTitanData(&TitanXIViewData);
	TitanXIGra.InitDisplayList();
}
//---------------------
void ogl_2d_tri_test(double x0, double y0)//三角形カーソル 2D描画
{
    double r=0.1;
    ogl_2d_triangle(x0,y0,x0-0.5*r,y0-r,x0+0.5*r,y0-r);
}
//---------------------
void disp_overlay( void ) //2D-overlay描画．マウスズームとは無関係にサイズ固定．
{

//マウスズームに無関係の固定枠。二次元指定
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glLoadIdentity();
    glColor3f(0.5f,0.5f,0.5f);
    glRasterPos2d(0,0);
	ogl_2d_tri_test(0,-0.1);//三角形のカーソル

    //ogl_2d_tri_test_p((double)cursor_pos_global[0]/10,(double)cursor_pos_global[1]/10);//三角形カーソル
    //ogl_2d_circle_test_p((double)cursor_pos_global[0]/10,(double)cursor_pos_global[1]/10);//円形カーソル


    //位置固定テキスト
    void *font1=GLUT_BITMAP_9_BY_15;
    glRasterPos2d(-0.9,0.9);//画面座標でメッセージ表示位置指定
    //glutBitmapString(font1, reinterpret_cast<const unsigned char*>("Doi Robotics Lab."));
    glutBitmapString(font1, (unsigned char *)("Doi Robotics Lab.2014.8.26"));

  glPopMatrix();

    //移動するテキスト
    glRasterPos3d(3000,0,0);//三次元座標でメッセージ表示位置指定
    glutBitmapString(font1, (unsigned char *)("(3000,0,0)"));
    glRasterPos3d(0,3000,0);//三次元座標でメッセージ表示位置指定
    glutBitmapString(font1, (unsigned char *)("(0,3000,0)"));
    glRasterPos3d(0,0,3000);//三次元座標でメッセージ表示位置指定
    glutBitmapString(font1, (unsigned char *)("(0,0,3000)"));


}
//-------------------------------------
void display()
{
//視点の決定

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
double wh_ratio;//　w/hの比
wh_ratio=(double)W_Width/(double)W_Height;
ogl1.set_viewpoint(mv1.distance,mv1.azimuth,mv1.elevation,
  mv1.v_center[0], mv1.v_center[1], mv1.v_center[2], wh_ratio);

    disp_overlay();//二次元描画．三次元描画に重ねる形
	ogl1.set_light();//照明セット

	//TitanXIGra.RenderScene();//TITAN XI描画．DCやライティングなど，描画すべてを行う
	
	TitanXIGra.DrawGridFloor(-10000,-10000,10000,10000,-1200,2000,2000);//グリッド描画
	TitanXIGra.DrawCoordinateAxis(3000,5);
	TitanXIGra.DrawTitan();//TITAN XIのみ描画

	glFlush();
	glutSwapBuffers();
}
//-------------------
BOOL CALLBACK DialogProc2(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND hSlider[12];
    static int nSlider[]={50,50,50, 50,50,50, 50,50,50, 50,50,50};
	Matrix pos[4];//脚先位置ベクトル
	Matrix ang(3);//関節角
	double clen[3];//シリンダ長さ


	switch(uMsg)
    {

        case WM_INITDIALOG:
			//スライダの初期設定
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
			 //運動学ライブラリ初期化
			InitKinematicsUtil();
			return TRUE;

        case WM_CLOSE:
            EndDialog(hwndDlg, 0);
            return TRUE;

         case WM_HSCROLL://水平スライダのどれかが反応
			 //printf("HSCROLL:");//debug
			for(int i=0;i<12;i++)
			{
            nSlider[i]=(int) SendMessage(hSlider[i], TBM_GETPOS, 0, 0);
			nSlider[i]-=100;//中央値がゼロになるように値シフト
			nSlider[i]*=20;//変化量拡大
			}
			/*
            printf("Slider[%d, %d, %d,  %d,%d,%d,  %d,%d,%d,  %d,%d,%d]\n",
				nSlider[0], nSlider[1], nSlider[2], nSlider[3], 
				nSlider[4], nSlider[5], nSlider[6], nSlider[7],
				nSlider[8], nSlider[9], nSlider[10], nSlider[11]);
			*/

			double temp[3];
			//初期位置取得
			for(int i=0;i<4;i++)
			{
				pos[i].SetSize(4,4);//同次変換行列（4x4）で用意
				pos[i]=TitanXI_0->GetLegTransformMatrixInGlobal(i+1,4);
				//GetPos(i+1,&temp[0], &temp[1], &temp[2]);
				//pos[i](1)=temp[0];pos[i](2)=temp[1];pos[i](3)=temp[2];
			}

			//初期位置にスライダ変化量加算
			for(int i=0;i<4;i++)
			{
			pos[i](1,4)+=nSlider[3*i+0]; pos[i](2,4)+=nSlider[3*i+1]; pos[i](3,4)+=nSlider[3*i+2];
			}

			KINE res;
			//Titan構造体に反映
			for(int i=0;i<4;i++)
			{
				//TitanXI_1->SetFootConfiguration(i+1,pos[i]);//先端位置設定とIK+DK
				SetIK(i+1,pos[i](1,4),pos[i](2,4),pos[i](3,4));
				GetAng(i+1,&temp[0], &temp[1], &temp[2]);
				ang(1)=temp[0];ang(2)=temp[1];ang(3)=temp[2];
				res=TitanXI_1->SetLegJntAng(i+1,ang);

				//シリンダ長さ表示
				GetCylinder(i+1,&clen[0],&clen[1],&clen[2]);
				printf("DialogProc2():Leg%d-CldrLen[%.2f,%.2f,%.2f]\n",
					i+1,clen[0],clen[1],clen[2]);
			}


			//描画用データ構造体に反映
			TitanXIViewData.UpdateRobotData(*TitanXI_1);
			  	glutPostRedisplay();


           return TRUE;
        case WM_VSCROLL://垂直スライダのどれかが反応
            printf("VSCROLL");
            nSlider[1]=(int) SendMessage(hSlider[1], TBM_GETPOS, 0, 0);
            //printf("Slider[1]=%d\n",nSlider[1]);
           return TRUE;
       }
	return FALSE;
}
//-----------------------
void mbutton(int button , int state , int x , int y) //マウスボタン押し下げ、上げイベント
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
void mmotion(int x , int y) //マウス移動イベント
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
void keyf(unsigned char key , int x , int y)//一般キー入力
{
	printf("key[%c],x[%d],y[%d]\n",key,x,y);
    switch(key)
    {
    case 'q':
    case 'Q':
    case '\033':  /* '\033' は ESC の ASCII コード */
        {
            printf("Exit\n");
            exit(0);
            break;
        }
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
GLUT_KEY_LEFT	左矢印キー
GLUT_KEY_UP	上矢印キー
GLUT_KEY_RIGHT	右矢印キー
GLUT_KEY_DOWN	下矢印キー
GLUT_KEY_PAGE_UP	Page up キー
GLUT_KEY_PAGE_DOWN	Page down キー
GLUT_KEY_HOME	Home キー
GLUT_KEY_END	End キー
GLUT_KEY_INSERT	Inset キー
*/

void keyf2(int key , int x , int y)//特殊キーの扱い
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
void js_func(unsigned int buttonMask, int x, int y, int z)//ジョイスティックイベント
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
void reshape(int width, int height)//再描画の関数
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
	printf("TITAN XI Simulator\n");
	printf("------------------\n");
	
	//TitanXIGra.InitOpenGL(hwndDlg);//最初の1回だけ
	CreateDialog(hInstance2, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc2);//ダイアログ２

	int id;

 	glutInit(&argc , argv);
	glutInitWindowPosition(100 , 50);
	glutInitWindowSize((int)W_Width , (int)W_Height);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	mv1.distance=20000;
	mv1.azimuth=60;



	id=glutCreateWindow("TITAN-XI GLUT test");

	init_draw_titan();//TITAN 描画準備

    glutDisplayFunc(display);//描画処理
	glutKeyboardFunc(keyf);//キー入力イベント
	glutSpecialFunc(keyf2);//特殊キー入力イベント
	glutReshapeFunc(reshape);//再描画イベント
	glutJoystickFunc(js_func,100);//ジョイスティックイベント
	
	glutMouseFunc(mbutton);//マウスボタン押し下げ、上げ
	glutMotionFunc(mmotion);//マウス移動イベント

	glutMainLoop();
	return 0;

}
