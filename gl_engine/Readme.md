# Guide for adding a new shader type
1. In `../data` folder, create appropriate fragment / vertex / etc. shader(s).
2. If a new type of texture is required, go to the factory inside `shading/Texture.h` and make a new create method there.
3. Inside `shading/Texture.cpp`, fill out the definition with the appropriate set methods for the gl texture.
4. Go to `material/MaterialLibrary.h` and make a new material that inherits from the `Material` class.
5. Use `inline static const` to fill out the names of any uniforms you are using in the shader.
6. Add update and init methods as required (see existing materials for examples).
7. Giva a definition for these files in the `material/MaterialLibrary.cpp` file. Models and materials should have any matrix transform methods you require.

## Textures
Textures can be solid colours or files.

#### Note about `m_data`:

  This usually contains a pointer to the actual pixel data of the texture. 
  However, if multiple image files are required (in the case of a skybox
  for example), `m_data` will only point to the first image. If more are needed, 
  it is better to cycle through the vector m_surface and access the data from
  each element using `m_surface.at(i)->data`.
