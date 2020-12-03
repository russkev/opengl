# GL Engine
GL Engine is simple real time rendering engine built with C++ and OpenGL. The 
aim of the project is to get acquainted with OpenGL and use it to implement some
advanced real time lighting features.

  - Working with OpenGL to create a functional realtime rendering engine.
  
  - OpenGL version: 4.40
  
  - Built and compiled using Visual Studio Core Community 2019
  
  - C++ for main program and GLSL for shaders.
  
  - SDL for window display.

# Features
 - Cameras
    - Target camera for orbiting
    - Free camera
    - Orthogonal camera
 - Lights
    - Directional
    - Point
    - Spot
 - Meshes
    - Various primitive shapes
    - Custom OBJ loader
 - Shading
    - Blinn master shader with texture maps for diffuse, specularity, roughness, etc.
    - Normal mapping
    - Parallax mapping
    - Self illuminated shader
    - Shadow mapping for all light types
    - Handles multiple lights and shadows
    - Image loading for textures
    - 2D Text overlay
 - Post effects
    - HDR Tone mapping
    - Deferred rendering
    - Screen space ambient occlusion
    - Glow effect

# Screenshots
![Demo 01](assets/screenshots/demo_01.jpg)
![Demo 02](assets/screenshots/demo_02.jpg)
![Demo 04](assets/screenshots/demo_04.jpg)
![Demo 05](assets/screenshots/demo_05.jpg)
![Demo 06](assets/screenshots/demo_06.jpg)
![Demo 07](assets/screenshots/demo_07a.jpg)

# Build Instructions
This project requires Windows and Visual Studio 2019. You'll also need a graphics
card that supports OpenGL 4.4 or higher. 

To get going, simply clone the repository and open `build/gl_engine.sln`.

Ensure that the `gl_demo` is the startup project (it should be **bold** in the `Solution Explorer`).
If not, right click on it inside the `Solution Explorer` and click `Set as Startup Project`.

If you have Visual Studio set up to automatically download NuGet packages, press `F5`
to build and run the solution. The required packages should automatically download. 

If they don't, click `Tools` -> `NuGet Package Manager` -> `Manage NuGet Packages for Solution...` 
You should get a warning saying that some packages are missing. Click `Restore`.


 - [Follow these instructions](https://docs.microsoft.com/en-us/cpp/build/vcpkg?view=msvc-160) to install vcpkg



## Boost
Boost is only required for unit testing. It doesn't get installed automatically with NuGet.

To get Boost working, do the following steps:

  - Download Boost (I've tested on 1.72 but it should work with other versions)
  - Open `x64 Native Tools Command Prompt for VS 2019`
  - Navigate to the Boost installation directory
  - Type `bootstrap.bat`
  - When that is finished, type `b2 address-model=64 toolset=msvc-14.2`
  - Open the Visual Studio Solution
  - In the `Solution Explorer`, right click on the `gl_test` project and click `properties`
  - Go to `Configuration Properties` -> `C/C++` -> `General`
  - Add / change the path to the `Boost` directory to `Additional Include Directory`

# Included Libraries
 - Glew
 - GLM
 - SDL

# Acknowledgements
  - [Aleksandras Ševčenko](https://github.com/Coldberg) has been an excellent guide while I've been fumbling around.

# References
  - [Joey Devries](https://joeydevries.com/#home) has some excellent tutorials and an example game engine here on github.
  - [OpenGL Tutorial](http://www.opengl-tutorial.org/). Another excellent resource.
