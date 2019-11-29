#pragma once

#include "loadBMP_custom.h"
#include "targa/targa.h"

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

	void upload_to_shader(unsigned int s_program_id, const char *gl_texture_name);

private:
	void init_targa(const char *filename);
private:
	tga_image	m_image;
	int			m_id;
	static int	m_next_id;
};
