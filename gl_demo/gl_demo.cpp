#define SDL_MAIN_HANDLED

#include <iostream>
#include <vector>

#include "MathsFuncs.h"
#include "render/Window.h"
#include "mesh/Mesh.h"
#include "mesh/Cube.h"
#include "mesh/Cylinder.h"
#include "mesh/Arrow.h"
#include "mesh/Vertex.h"
#include "mesh/OBJ.h"
#include "mesh/Plane.h"
#include "mesh/Sphere.h"
#include "mesh/WindowQuad.h"

int main()
{
	double a = 7.4;
	int b = 99;

	glen::Window window{ "Window", 800u, 600u };

	glen::Mesh cube{ glen::Cube::create_cube() };
	glen::Mesh arrow{ glen::Arrow::create_arrow() };
	glen::Mesh cylinder{ glen::Cylinder::create_cylinder() };
	glen::Mesh shader_ball{ glen::OBJ_Loader::load_obj("shaderball_lowpoly_02_tris.obj") };
	glen::Mesh plane{ glen::Plane::create_plane() };
	glen::Mesh sphere{ glen::Sphere::create_sphere(2.0f) };
	glen::Mesh window_quad{ glen::WindowQuad::create_windowQuad() };


	std::cout << "a + b = " <<
		MathFuncs::MyMathFuncs::Add(a, b) << "\n";

    std::cout << "Hello World!\n";

	return 0;
}