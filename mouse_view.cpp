//030930 マウスで視点変化
//doi
//#include "stdafx.h"
//#define M_PI 3.141592 //VC++用
#include "vcsetting.h"
#include <math.h>
#include "mouse_view.h"
//------------------------
MouseView::MouseView()
{
default_state();
}
//------------------------
void MouseView::default_state()
{
  x0=0;
  y0=0;
  azimuth=-120;
  elevation=30;
//  distance=50000;
  distance=15
  ;
  for(int i=0;i<3;i++)
    v_center[i]=0;
  init(0,0);
}
//------------------------
void MouseView::init(int x_in, int y_in)
{
  x0=x_in;
  y0=y_in;
  azimuth0=azimuth;
  elevation0=elevation;
  distance0=distance;
  for(int i=0;i<3;i++)
    v_center0[i]=v_center[i];
}
//------------------------
void MouseView::Rotate(int x_in, int y_in)
{
  int dx=x_in-x0;
  int dy=y_in-y0;
  azimuth=(-0.5)*(double)dx+azimuth0;
  elevation=(0.5)*(double)dy+elevation0;
}
//------------------------
void MouseView::Zoom(int x_in, int y_in)
{
  int dx=x_in-x0;
  int dy=y_in-y0;
  double dl=dx+dy;
  double velocity=50;
  distance=velocity*dl+distance0;
}
//------------------------
void MouseView::Translate(int x_in, int y_in)
{
  int dx=x_in-x0;
  int dy=y_in-y0;
  double up[3],view[3],right[3];
  up_dir(&up[0],&up[1],&up[2]);
  view_dir(&view[0],&view[1],&view[2]);
  right_dir(&right[0],&right[1],&right[2]);

//  double vx=5.0e-1;
//  double vy=5.0e-1;
  double vx=5.0e+1;
  double vy=5.0e+1;
  for(int i=0;i<3;i++)
    {
      v_center[i]=(-vx*dx*right[i]+vy*dy*up[i])+v_center0[i];
    }

}
//---------------------------------------------------------------------------
char MouseView::outer_product(double vx1_in,double vy1_in,double vz1_in,
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
void MouseView::view_dir(double *x_out, double *y_out, double *z_out)
{
//視線方向
double x=-cos(elevation*M_PI/180)*cos(azimuth*M_PI/180);
double y=-cos(elevation*M_PI/180)*sin(azimuth*M_PI/180);
double z=-sin(elevation*M_PI/180);

  *x_out=x;
  *y_out=y;
  *z_out=z;
}
//---------------------------------------------------------------------------
void MouseView::up_dir(double *x_out, double *y_out, double *z_out)
{
//視野の上方向
double x=cos((elevation+90)*M_PI/180)*cos(azimuth*M_PI/180);
double y=cos((elevation+90)*M_PI/180)*sin(azimuth*M_PI/180);
double z=sin((elevation+90)*M_PI/180);
  *x_out=x;
  *y_out=y;
  *z_out=z;
}
//---------------------------------------------------------------------------
void MouseView::right_dir(double *x_out, double *y_out, double *z_out)
{
//視野の右方向
double x,y,z;
double v[3],u[3];
view_dir(&v[0],&v[1],&v[2]);
up_dir(&u[0],&u[1],&u[2]);
outer_product(v[0],  v[1],  v[2],  u[0],  u[1],  u[2],
              &x, &y, &z);
/*
x=cos((elevation)*M_PI/180)*cos((azimuth+90)*M_PI/180);
y=cos((elevation)*M_PI/180)*sin((azimuth+90)*M_PI/180);
//z=sin((elevation)*M_PI/180);
z=0;
*/

  *x_out=x;
  *y_out=y;
  *z_out=z;
}
//---------------------------------------------------------------------------
void MouseView::eye(double *x_out, double *y_out, double *z_out)
{
//視点
double v_d[3];
view_dir(&v_d[0],&v_d[1],&v_d[2]);
double x=-distance*v_d[0]+v_center[0];
double y=-distance*v_d[1]+v_center[1];
double z=-distance*v_d[2]+v_center[2];
  *x_out=x;
  *y_out=y;
  *z_out=z;
}
