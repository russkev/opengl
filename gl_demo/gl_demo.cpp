#define SDL_MAIN_HANDLED

#include "scenes/ParallaxDemo.h"
#include "scenes/SpecularityDemo.h"
#include "scenes/DeferredRenderDemo.h"


int main()
{
	gl_demo::parallax_demo();
	gl_demo::specularity_demo();
	gl_demo::deferred_demo();
	return 0;
}