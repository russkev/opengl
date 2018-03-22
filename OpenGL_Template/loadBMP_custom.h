#pragma once
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <GL/glew.h>
#include <memory>

struct loadBMP_custom {
public:
	// // MEMBER VARIABLES // //
	FILE * m_file;
	unsigned char m_header[54];					// Each BMP file begins with a 54 byte header
	unsigned int  m_dataPos;					// Popsition in file where actual data begins
	unsigned int  m_width, m_height;	 
	unsigned int  m_imageSize;					// = width * height * 3
	
	std::unique_ptr<unsigned char[]> m_data;	// Actual RGB data

public:
	// // CONSTRUCTOR // //
	loadBMP_custom(const char * imagepath) /*: m_file(fopen(imagepath, "r"))*/ {
		m_file = fopen(imagepath, "rb");
		if (!m_file) { 
			perror("ERROR: Image could not be opened");
			return;
		};
		//std::vector<unsigned char> data;//[786432];
		//data.resize(786432);
		//auto testFread = fread(data.data(), 1, 786432, m_file);
		if (fread(m_header, 1, 54, m_file) != 54) {
			printf("ERROR: Not a correct BMP file : Header not correct size\n");
			return;
		}
		if (m_header[0] != 'B' || m_header[1] != 'M') {
			printf("ERROR: Not a correct BMP file : Header does not contain 'BM'\n");
			return;
		}

		// Read ints from byte array //
		m_dataPos		= *(int*)&(m_header[0x0A]);
		m_imageSize		= *(int*)&(m_header[0x22]);
		m_width			= *(int*)&(m_header[0x12]);
		m_height		= *(int*)&(m_header[0x16]);

		// Some BMP files are misformatted, guess missing information //
		if (m_imageSize == 0) { m_imageSize = m_width * m_height * 3; } // 3: one byte for each of RGB //
		if (m_dataPos   == 0) { m_dataPos = 54; }

		// Create a Buffer //

		m_data = std::make_unique<unsigned char[]>(m_imageSize);


		// // TEST // //

		FILE * testFile;
		testFile = fopen(imagepath, "r");
		std::vector<unsigned char> data;//[786432];
		data.resize(m_imageSize);
		fread(data.data(), 1, m_imageSize, m_file);

		// // END TEST // //


		// Read the actual data from the file into the buffer //
		fread(m_data.get(), 1, m_imageSize, m_file);
		// Everything is in memory now, close the file //
		fclose(m_file);

		// Create one OpenGL texture //
		GLuint textureID;
		glGenTextures(1, &textureID);
		// "Bind" the newly created texture : all future texture functions will modify this texture //
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Give the image to OpenGL //
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_BGR, GL_UNSIGNED_BYTE, m_data.get());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	};
};
