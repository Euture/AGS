#pragma once

#include <windows.h>
#include "stdio.h"
#include <iostream>
#include <fstream>
#include "glew.h"
#include "GL/freeglut.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

using namespace glm;
using namespace std;

// КЛАСС ДЛЯ РАБОТЫ С ШЕЙДЕРОМ
class CShader
{
private:

	// вершинный шейдер
	GLuint	Vsh;
	int VshStringCount;
	
	// фрагментный шейдер
	GLuint	Fsh;
	int FshStringCount;
	
	// шейдерная программа (шейдер)
	GLuint	Program;

public:
	// карта uniform переменная-индекс 
	map <string, int> UniformId;
	// загрузить вершинный шейдер
	int LoadVertexShader (char *VertexFileName, bool DebugOutput = true);
	// загрузить фрагментный шейдер
	int LoadFragmentShader (char *FragmentFileName, bool DebugOutput = true);
	// слинковать шейдерную программу
	int Link(bool DebugOutput = true);

	//	активизировать шейдер (сделать текущим)
	void Activate (void);
	//	отключить шейдер (использовать нулевую шейдерную программу)
	static void	Deactivate (void);

	// получение индекса атрибут-переменной
	int GetAttribLocation (char *name)
	{
		return glGetAttribLocation(Program, name);
	};

	char ** ReadF(char *FileName);

	// запись вектора из 4-х комопнент в uniform-переменную
	void SetUniformVec4(string str, vec4 &value);
	// запись вектора из 4-х комопнент в uniform-переменную
	void SetUniformMat4(string str, mat4 &value);
	// вернуть id uniform переменной
	int GetUniformId(const string &name);
};