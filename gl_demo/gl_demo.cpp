#define SDL_MAIN_HANDLED

#include "scenes/ParallaxDemo.h"
#include "scenes/SpecularityDemo.h"
#include "scenes/DeferredRenderDemo.h"
#include "scenes/AODemo.h"
#include "scenes/ShaderBallHD.h"


int main()
{
	//gl_demo::parallax_demo();
	//gl_demo::specularity_demo();
	//gl_demo::deferred_demo();
	//gl_demo::ao_demo();
	gl_demo::ShaderBallHD hd_demo;
	hd_demo.demo_01();
	hd_demo.demo_02();

	return 0;
}