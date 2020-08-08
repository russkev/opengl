// gl_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

//#define BOOST_ALL_NO_LIB
//#define SDL_MAIN_HANDLED
#define BOOST_TEST_MAIN
//#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE Main


#include <boost/test/unit_test.hpp>

//int main(int argc, char* args[]);

#include "../gl_engine/render/Window.cpp"
#include "../gl_engine/shading/Texture.cpp"
#include "../gl_engine/shading/Framebuffer.cpp"

//#include <iostream>
//#define BOOT_TEST_MODULE
//#include <boost/test/included/unit_test.hpp>
//#include "../gl_engine/utils/VectorUtils.h"
//#include <string>
//
//BOOST_AUTO_TEST_CASE(my_boost_test)
//{
//	BOOST_CHECK(VectorUtils::minValue(1, 2) == 1);
//}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
