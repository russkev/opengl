# opengl

Working with OpenGL to create a functional realtime rendering engine.

OpenGL version: 4.40

Built and compiled using Visual Studio Core Community 2017

C++ for main program and GLSL for shaders.

SDL for window display.

Current features:
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
    - Blinn master shader
    - Self illuminated shader
    - Shadow mapping for all light types
    - Handles multiple lights and shadows
    - Image loading for textures
    - 2D Text overlay
 - Post effects
    - HDR Tone mapping
    
Big thank you to:
  - [Aleksandras Ševčenko](https://github.com/Coldberg) for being an excellent guide while I've been fumbling around.
  - [Joey Devries](https://joeydevries.com/#home) for his excellent tutorials and his example game engine here on github.
  - [OpenGL Tutorial](http://www.opengl-tutorial.org/). Another excellent resource.
