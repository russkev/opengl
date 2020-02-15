#define SDL_MAIN_HANDLED

#include "scenes/ParallaxDemo.h"
#include "scenes/SpecularityDemo.h"
#include "scenes/DeferredRenderDemo.h"
#include "scenes/AODemo.h"


int main()
{
	//gl_demo::parallax_demo();
	//gl_demo::specularity_demo();
	//gl_demo::deferred_demo();
	gl_demo::ao_demo();
	return 0;
}