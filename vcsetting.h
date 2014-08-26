//140821
//コンパイラの差を吸収するヘッダファイル

#define VISUALSTUDIO //code::blocksなどの時はコメントアウト

#ifdef VISUALSTUDIO
	#include <windows.h> //VC++用
	#define M_PI 3.141592 //VC++用
#endif


