#define SDL_MAIN_HANDLED

#include "scenes/DeferredRenderDemo.h"
#include "scenes/ShaderBallHD.h"

#undef main
int main(int argc, char* args[])
{
	gl_demo::deferred_demo();

	gl_demo::ShaderBallHD hd_demo;
	hd_demo.demo_01();
	hd_demo.demo_02();
	hd_demo.demo_03();
	hd_demo.demo_04();
	hd_demo.demo_05();
	hd_demo.demo_06();

	return 0;
} 