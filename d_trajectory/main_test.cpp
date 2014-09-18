#include <stdio.h>
#include "d_trajectory.h"
#include "d_robot_trajectory.h"


//-------
int main(void)
{
int res;	
	printf("軌道生成関数群のテスト\n");
	RTSetTrajectoryICCRAWL();//間歇クロール軌道セット
	
	
	
	double x[4],y[4],z[4];
	double t;
	int i;
	
	printf("---------------------\n");
	printf("t	LEG1	LEG2	LEG3	LEG4\n");

	for(i=0;i<40;i++)
	{
		t=(double)i*1.0/1.0;
		RTget_trajectory(t, 
			&x[0],&y[0],&z[0], &x[1],&y[1],&z[1], &x[2],&y[2],&z[2], &x[3],&y[3],&z[3]);
		printf("%.0f	|%.0f,%.0f,%.0f	|%.0f,%.0f,%.0f	|%.0f,%.0f,%.0f	|%.0f,%.0f,%.0f|\n",
			t, x[0],y[0],z[0], x[1],y[1],z[1], x[2],y[2],z[2], x[3],y[3],z[3]);
	}
	printf("---------------------\n");
return 0;
}


