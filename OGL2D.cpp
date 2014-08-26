//100125 OGL2D.cpp
//OpenGLを使った二次元グラフィック
#include "vcsetting.h"

#include <GL/gl.h>
#include <math.h>
#include "OGL2D.h"

//---------------------
void ogl_2d_rectangle_core(double x0, double y0,double x1, double y1)
{
  glVertex2d(x0,y0);
  glVertex2d(x1,y0);
  glVertex2d(x1,y1);
  glVertex2d(x0,y1);
  
}
void ogl_2d_rectangle(double x0, double y0,double x1, double y1)
{
    glBegin(GL_LINE_LOOP);
    ogl_2d_rectangle_core(x0,y0,x1,y1);
  glEnd();
}
void ogl_2d_rectangle_p(double x0, double y0,double x1, double y1)
{
    glBegin(GL_POLYGON);
    ogl_2d_rectangle_core(x0,y0,x1,y1);
  glEnd();
}
//---------------------
void ogl_2d_line(double x0, double y0,double x1, double y1)
{
  glBegin(GL_LINE);
  glVertex2d(x0,y0);
  glVertex2d(x1,y1);
  glEnd();
    
}
//---------------------
void ogl_2d_triangle_core(double x0, double y0, double x1, double y1, double x2, double y2)
{
  glVertex2d(x0,y0);
  glVertex2d(x1,y1);
  glVertex2d(x2,y2);
}
void ogl_2d_triangle(double x0, double y0, double x1, double y1, double x2, double y2)
{
  glBegin(GL_LINE_LOOP);
    ogl_2d_triangle_core(x0,y0,x1,y1,x2,y2);
  glEnd();
    
}
void ogl_2d_triangle_p(double x0, double y0, double x1, double y1, double x2, double y2)
{
  glBegin(GL_POLYGON);
    ogl_2d_triangle_core(x0,y0,x1,y1,x2,y2);
  glEnd();
}
//---------------------
void ogl_2d_circle_core(double x0, double y0, double r)
{
    double pi=3.141592;
    double div=64;//多角形近似頂点数
    double dth=2*pi/div;
        for(int i=0;i<div;i++)
        {
        glVertex2d(x0+r*cos(dth*i),y0+r*sin(dth*i));
        }
}
void ogl_2d_circle(double x0, double y0, double r)
{
    glBegin(GL_LINE_LOOP);
    ogl_2d_circle_core(x0,y0,r);
    glEnd();
}
void ogl_2d_circle_p(double x0, double y0, double r)
{
    glBegin(GL_POLYGON);
    ogl_2d_circle_core(x0,y0,r);
    glEnd();
}
