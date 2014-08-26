//---------------------------------------------------------------------------
//OpenGLによる描画
//000929doi
//------------------------------------------------------
//使い方の例
//ogl1->set_rc(Canvas->Handle);
//glMatrixMode(GL_MODELVIEW);
//ogl1->set_viewpoint(15,theta,phi);
//ogl1->set_light();
//ogl1->Frame(-1,1,-1,1,-0.5,0.5);
//ogl1->Blue();
//ogl1->Box(0,1,-1,1,-0.5,0.5);
//ogl1->draw(Canvas->Handle);
//------------------------------------------------------
#include "vcsetting.h"//VC++用

extern "C"
{
#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glut.h"
}
#include <math.h>
#include "OGL.h"

//---------------------------------------------------------------------------
OGL::OGL()
{
}
//---------------------------------------------------------------------------
OGL::~OGL()
{
#ifdef WIN32
  wglMakeCurrent(NULL,NULL);
  wglDeleteContext(rc1);
#endif
}
//---------------------------------------------------------------------------
void OGL::set_rc(void *dc_in)
{
#ifdef WIN32
  HDC dc1=(HDC)dc_in;
PIXELFORMATDESCRIPTOR pfd=
    {
    sizeof(PIXELFORMATDESCRIPTOR),
    1,
    PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL|
            PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA,
    24,
    0,0,0,0,0,0,
    0,0,0,0,0,0,0,

    32,
    0,0,
    PFD_MAIN_PLANE,
    0,
    0,0,0
    };

int pixelFormat;
//pixelFormat=ChoosePixelFormat(dc1,&pfd);//Cygwinでは使わない

//if(pixelFormat==0);ShowMessage("get pfd fail");
//if(SetPixelFormat(dc1,pixelFormat,&pfd)==false);//ShowMessage("set fail");//Cygwinではダメ？
rc1=wglCreateContext(dc1);
//if(rc1==0);ShowMessage("rc error");
if(wglMakeCurrent(dc1,rc1)==0)
    {
    ;
    //ShowMessage("bind error");
    //return;
    }
#endif
}
//---------------------------------------------------------------------------
void OGL::draw(void *dc_in)
{
glFlush();
//glFinish();//これを使うとNTが落ちる
//  if(SwapBuffers((HDC)dc_in)==false)//Cygwinではダメ？
  {
   ;
   //ShowMessage("swap error");
  }

/*
  if(wglMakeCurrent((HDC)dc_in,NULL)==0)
  {
  ;
  //ShowMessage("bind error 2");
  }

wglDeleteContext(rc1);
//↑メモリの解放．これをやらないとNTが固まる
*/
}
//---------------------------------------------------------------------------
void OGL::Box(double x0,double x1,double y0,double y1,double z0,double z1)
{
//箱を描く関数
glBegin(GL_POLYGON);
    glNormal3f(0.0f,0.0f,1.0f);
    glVertex3d(x0,y1,z1);
    glVertex3d(x0,y0,z1);
    glVertex3d(x1,y0,z1);
    glVertex3d(x1,y1,z1);
glEnd();
glBegin(GL_POLYGON);
    glNormal3f(1.0f,0.0f,0.0f);
    glVertex3d(x1,y1,z1);
    glVertex3d(x1,y0,z1);
    glVertex3d(x1,y0,z0);
    glVertex3d(x1,y1,z0);
glEnd();
glBegin(GL_POLYGON);
    glNormal3f(0.0f,0.0f,-1.0f);
    glVertex3d(x1,y1,z0);
    glVertex3d(x1,y0,z0);
    glVertex3d(x0,y0,z0);
    glVertex3d(x0,y1,z0);
glEnd();
glBegin(GL_POLYGON);
    glNormal3f(-1.0f,0.0f,0.0f);
    glVertex3d(x0,y1,z0);
    glVertex3d(x0,y0,z0);
    glVertex3d(x0,y0,z1);
    glVertex3d(x0,y1,z1);
glEnd();
glBegin(GL_POLYGON);
    glNormal3f(0.0f,-1.0f,0.0f);
    glVertex3d(x0,y0,z1);
    glVertex3d(x0,y0,z0);
    glVertex3d(x1,y0,z0);
    glVertex3d(x1,y0,z1);
glEnd();
glBegin(GL_POLYGON);
    glNormal3f(0.0f,1.0f,0.0f);
    glVertex3d(x0,y1,z0);
    glVertex3d(x0,y1,z1);
    glVertex3d(x1,y1,z1);
    glVertex3d(x1,y1,z0);
glEnd();
}
//---------------------------------------------------------------------------
void OGL::set_viewpoint(double l,double theta_in,double phi_in)
{
set_viewpoint(l,theta_in,phi_in,0,0,0, 1.0);
}
//---------------------------------------------------------------------------
void OGL::set_viewpoint(double l,double theta_in,double phi_in,
              double cx_in, double cy_in, double cz_in, double wh_ratio_in)
{
//視点の設定
//set_view()の代わりに使う．
//距離Lだけ離れたところから3次元シーンを見る

glLoadIdentity();
double p_theta=30;
double p_near=l*0.1;
double p_far=l*10;
  gluPerspective(p_theta,wh_ratio_in,p_near,p_far);
  set_background(1);

//  glFrustum(-1.0,1.0,-1.0,1.0,1.0,7.0);

double eye_x,eye_y,eye_z;
double center_x,center_y,center_z;
double up_x,up_y,up_z;
center_x=cx_in;
center_y=cy_in;
center_z=cz_in;
eye_x=l*cos(phi_in*M_PI/180)*cos(theta_in*M_PI/180)+center_x;
eye_y=l*cos(phi_in*M_PI/180)*sin(theta_in*M_PI/180)+center_y;
eye_z=l*sin(phi_in*M_PI/180)+center_z;
up_x=l*cos((phi_in+90)*M_PI/180)*cos(theta_in*M_PI/180);
up_y=l*cos((phi_in+90)*M_PI/180)*sin(theta_in*M_PI/180);
up_z=l*sin((phi_in+90)*M_PI/180);

gluLookAt(
eye_x,
eye_y,
eye_z,
center_x,
center_y,
center_z,
up_x,
up_y,
up_z
);


}
//---------------------------------------------------------------------------
void OGL::set_background(char IsWhite)
{
if(IsWhite==1)
  glClearColor(1.0,1.0,1.0,1.0);//背景白
else
  glClearColor(0.0,0.0,0.0,0.0);//背景黒

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//ClearColorを決めてから

}
//---------------------------------------------------------------------------
void OGL::set_light()
{
//glShadeModel(GL_SMOOTH);
glShadeModel(GL_FLAT);
glEnable(GL_DEPTH_TEST);


GLfloat ambientLight0[]={0.2f,0.2f,0.2f,1.0f};
GLfloat diffuseLight0[]={0.9f,0.9f,0.9f,1.0f};
GLfloat specularLight0[]={0.7f,0.7f,0.7f,1.0f};
GLfloat ambientLight1[]={0.1f,0.1f,0.1f,1.0f};
GLfloat diffuseLight1[]={0.25f,0.25f,0.25f,1.0f};
GLfloat specularLight1[]={0.5f,0.5f,0.5f,1.0f};

GLfloat scale=100.0f;
//GLfloat positionLight0[]={100.0f,-1000.0f,10.0f,0.0f};
//GLfloat directionLight0[]={-0.1f,1.0f,-0.01f};
GLfloat positionLight0[]={-1*scale,0.4*scale,-1*scale,1.0f};
GLfloat directionLight0[]={-scale,0.4*scale,-1*scale};
GLfloat positionLight1[]={-scale,-0.5*scale,-0.01*scale,0.0f};
GLfloat directionLight1[]={scale,0.5*scale,0.01*scale};

//ライト０
glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight0);
glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight0);
glLightfv(GL_LIGHT0,GL_SPECULAR,specularLight0);
glLightfv(GL_LIGHT0,GL_POSITION,positionLight0);
//glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,directionLight0);

//ライト１
glLightfv(GL_LIGHT1,GL_AMBIENT,ambientLight1);
glLightfv(GL_LIGHT1,GL_DIFFUSE,diffuseLight1);
glLightfv(GL_LIGHT1,GL_SPECULAR,specularLight1);
glLightfv(GL_LIGHT1,GL_POSITION,positionLight1);
glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,directionLight1);


glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
//glEnable(GL_LIGHT1);
}
//---------------------------------------------------------------------------
void OGL::Frame(double x0,double x1,double y0,double y1,double z0,double z1)
{
//枠描画
//GLfloat materialWhite[]={1.0f,1.0f,1.0f,1.0f};
GLfloat materialGray[]={0.9f,0.9f,0.9f,0.5f};
glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,materialGray);
glLineWidth(4.0f);
glBegin(GL_LINE_LOOP);
    glVertex3d(x1,y1,z0);
    glVertex3d(x1,y0,z0);
    glVertex3d(x0,y0,z0);
    glVertex3d(x0,y1,z0);
glEnd();

glBegin(GL_LINES);
    glVertex3d(x1,y0,z1);
    glVertex3d(x1,y0,z0);
glEnd();
glBegin(GL_LINES);
    glVertex3d(x0,y0,z0);
    glVertex3d(x0,y0,z1);
glEnd();
glBegin(GL_LINES);
    glVertex3d(x1,y1,z0);
    glVertex3d(x1,y1,z1);
glEnd();
glBegin(GL_LINES);
    glVertex3d(x0,y1,z0);
    glVertex3d(x0,y1,z1);
glEnd();
}
//---------------------------------------------------------------------------
char OGL::outer_product(double vx1_in,double vy1_in,double vz1_in,
                       double vx2_in,double vy2_in,double vz2_in,
                       double *vx_out,double *vy_out,double *vz_out
                       )
{
//外積
//閉じた形なので，よそでも使える
*vx_out=(vy1_in*vz2_in-vz1_in*vy2_in);
*vy_out=(vz1_in*vx2_in-vx1_in*vz2_in);
*vz_out=(vx1_in*vy2_in-vy1_in*vx2_in);
return 1;
}
//---------------------------------------------------------------------------
void OGL::Facet(double x0,double y0,double z0,
                       double x1,double y1,double z1,
                       double x2,double y2,double z2)
{
//↑ポリゴンモデリング．法線も
//0,1,2と，時計周りに入力
GLdouble vx,vy,vz;
GLdouble vx1,vy1,vz1,vx2,vy2,vz2;
double size;

//外積計算
vx1=x0-x1;
vy1=y0-y1;
vz1=z0-z1;
vx2=x2-x1;
vy2=y2-y1;
vz2=z2-z1;

outer_product(vx1,vy1,vz1,vx2,vy2,vz2,&vx,&vy,&vz);
//↑外積計算


//正規化
size=sqrt(vx*vx+vy*vy+vz*vz);
if(size==0)return;
vx=vx/size;
vy=vy/size;
vz=vz/size;

glBegin(GL_POLYGON);//(GL_LINE_LOOP);
    glNormal3f(vx,vy,vz);
    glVertex3d(x0,y0,z0);
    glVertex3d(x1,y1,z1);
    glVertex3d(x2,y2,z2);
glEnd();

}
//---------------------------------------------------------------------------
void OGL::set_material(double r_in,double g_in,double b_in,double alpha_in)
{
GLfloat r,g,b,a;
r=r_in;
g=g_in;
b=b_in;
a=alpha_in;
GLfloat materialtemp[]={r,g,b,a};
glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,materialtemp);

GLfloat materialspecular[]={0.5f,0.5f,0.5f,0.5f};
glMaterialfv(GL_FRONT,GL_SPECULAR,materialspecular);
}
//---------------------------------------------------------------------------
void OGL::Red(void)
{
set_material(1.0,0.0,0.0,0.5);
}
//---------------------------------------------------------------------------
void OGL::Green(void)
{
set_material(0.0,1.0,0.0,0.5);
}
//---------------------------------------------------------------------------
void OGL::Blue(void)
{
set_material(0.0,0.0,1.0,0.5);
}
//---------------------------------------------------------------------------
void OGL::coordinate(int size)
{
//x,y,zの順にRGBの色で座標系を表示
//sizeで長さを指定する
Red();
glBegin(GL_LINES);
 glVertex3d(0,0,0);
 glVertex3d(size,0,0);
glEnd();
Green();
glBegin(GL_LINES);
 glVertex3d(0,0,0);
 glVertex3d(0,size,0);
glEnd();
Blue();
glBegin(GL_LINES);
 glVertex3d(0,0,0);
 glVertex3d(0,0,size);
glEnd();

}
//---------------------------------------------------------------------------

