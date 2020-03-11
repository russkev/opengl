#define SDL_MAIN_HANDLED

#include "scenes/DeferredRenderDemo.h"
#include "scenes/ShaderBallHD.h"


int main()
{
	gl_demo::deferred_demo();
	gl_demo::ShaderBallHD hd_demo;
	hd_demo.demo_01();
	hd_demo.demo_02();

	return 0;
}