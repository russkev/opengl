#pragma once

#include "Bitmap.h"
#include "Targa.h"

struct Texture_old
{

	// // BIG 6 // //
	Texture_old();										//Constructor
	Texture_old(const char *filename);					//Constructor
	~Texture_old();										//Destructor
	Texture_old(const Texture_old&) = delete;				//Copy constructor
	Texture_old(Texture_old&&);								//Move constructor
	Texture_old& operator = (const Texture_old&) = delete;	//Copy assign
	Texture_old& operator = (Texture_old&&);				//Move assign

	void upload_to_shader(unsigned int s_program_id, const char *gl_texture_name);

private:
	void init_targa(const char *filename);
private:
	tga_image	m_image;
	int			m_id;
	static int	m_next_id;
};
