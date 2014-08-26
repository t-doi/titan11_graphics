//---------------------------------------------------------------------------
//OpenGLによる描画
//00801doi
//040216 modified
#ifndef OGLH
#define OGLH

#define WIN32 1
#ifdef WIN32
#include <windows.h>
#endif
//---------------------------------------------------------------------------
class OGL
{
private:
public:
#ifdef WIN32
  HGLRC rc1;
#endif
  OGL();
    ~OGL();
    char outer_product(double vx1_in,double vy1_in,double vz1_in,
                       double vx2_in,double vy2_in,double vz2_in,
                       double *vx_out,double *vy_out,double *vz_out
                       );
    //↑外積->あまり使わないほうが良い．vertexの関数を使うべき．
    void Facet(double x0,double y0,double z0,
                       double x1,double y1,double z1,
                       double x2,double y2,double z2);
    //↑ポリゴンモデリング．法線も
    void Frame(double x0,double x1,double y0,double y1,double z0,double z1);
    void Box(double x0,double x1,double y0,double y1,double z0,double z1);
    void set_material(double r_in,double g_in,double b_in,double alpha_in);
    void Red(void);
    void Green(void);
    void Blue(void);
    void set_rc(void *dc_in);
    void draw(void *dc_in);
    void set_viewpoint(double l,double theta_in,double phi_in);
    void set_viewpoint(double l,double theta_in,double phi_in,
              double cx_in, double cy_in, double cz_in, double wh_ratio_in);
    void set_light(void);
    void coordinate(int size);//x,y,zの順にRGBの色で座標系を表示
    void set_background(char IsWhite);//
};
//---------------------------------------------------------------------------
#endif
