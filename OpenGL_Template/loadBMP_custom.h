#pragma once
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>

struct loadBMP_custom {
public:
	FILE * m_file;

public:
	loadBMP_custom(const char * imagepath) : m_file(fopen(imagepath, "r")) {};
};
