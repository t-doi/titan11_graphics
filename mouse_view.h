//030930 マウスで視点変化
//100125 update
//doi

class MouseView
{
private:
  int x0,y0;
  double v_center0[3];
  double azimuth0,elevation0,distance0;
  char outer_product(double vx1_in,double vy1_in,double vz1_in,
                       double vx2_in,double vy2_in,double vz2_in,
                       double *vx_out,double *vy_out,double *vz_out
                       );

public:
  double azimuth,elevation,distance;
  double v_center[3];
  char drag_mode;//マウスドラッグモード
  char flag_drag;//ドラッグのフラグ

  
  MouseView();
  void init(int x_in, int y_in);
  void default_state();
  void Rotate(int x_in, int y_in);
  void Zoom(int x_in, int y_in);
  void Translate(int x_in, int y_in);  

  void up_dir(double *x_out, double *y_out, double *z_out);
  void right_dir(double *x_out, double *y_out, double *z_out);
  void eye(double *x_out, double *y_out, double *z_out);    
  void view_dir(double *x_out, double *y_out, double *z_out);
};
#define DRAG_MODE_STOP		0
#define DRAG_MODE_ZOOM		1
#define DRAG_MODE_ROTATE	2
#define DRAG_MODE_TRANSLATE 3
