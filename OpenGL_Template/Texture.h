#pragma once

#include "loadBMP_custom.h"
#include "targa.h"

struct Texture
{
	// // BIG 6 // //
	Texture();										//Constructor
	Texture(const char *filename);					//Constructor
	~Texture();										//Destructor
	Texture(const Texture&) = delete;				//Copy constructor
	Texture(Texture&&);								//Move constructor
	Texture& operator = (const Texture&) = delete;	//Copy assign
	Texture& operator = (Texture&&);				//Move assign

	void upload_to_shader(unsigned int s_program_id, const char *gl_texture_name, const GLint s_texture_num);
private:
	void init_targa(const char *filename);
private:
	tga_image	m_image;
	//GLuint		m_program_id;
	//GLuint		m_tex_id;
	//GLuint		m_tex_loc;
	//GLuint		m_tex_num;
};