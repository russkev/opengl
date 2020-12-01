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
![Demo 01](https://raw.githubusercontent.com/russkev/opengl/master/screenshots/demo_01.jpg)
![Demo 02](https://raw.githubusercontent.com/russkev/opengl/master/screenshots/demo_02.jpg)
![Demo 04](https://raw.githubusercontent.com/russkev/opengl/master/screenshots/demo_04.jpg)
![Demo 05](https://raw.githubusercontent.com/russkev/opengl/master/screenshots/demo_05.jpg)
![Demo 06](https://raw.githubusercontent.com/russkev/opengl/master/screenshots/demo_06.jpg)
![Demo 07](https://raw.githubusercontent.com/russkev/opengl/master/screenshots/demo_07a.jpg)


# Included Libraries
 - Glew
 - GLM
 - SDL
 - ZLib

# Acknowledgements
  - [Aleksandras Ševčenko](https://github.com/Coldberg) has been an excellent guide while I've been fumbling around.

# References
  - [Joey Devries](https://joeydevries.com/#home) has some excellent tutorials and an example game engine here on github.
  - [OpenGL Tutorial](http://www.opengl-tutorial.org/). Another excellent resource.
