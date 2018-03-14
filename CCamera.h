#pragma once
#include <windows.h>
#include "stdio.h"
#include "glew.h"
#include "GL/freeglut.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <glm/matrix.hpp>

using namespace glm;
using namespace std;

const float PI = 3.14;
const float Max_Theta = radians(85.0);
const float Min_Theta = radians(5.0);

//классдля работы с камерой
class CCamera
{
	//вектора для ViewMatrix
	vec3 Eye;
	vec3 Center;
	vec3 Up;

	float Radius;
	float Fi;
	float Theta;
	float Speed = 2.0;

	//матрица наблюдения
	mat4 ViewMatrix;
	//матрица проекции
	mat4 ProjectionMatrix;

public:
	//конструктор по умолчанию
	CCamera(void);
	//деструктор
	~CCamera(void);

	//сохраняет позицию камеры в файл
	void SavePos(void);

	//установка матрицы проекции
	void SetProjectionMatrix(float fovy, float aspect, float zNear, float zFar);
	//получение матрицы проекции
	mat4 GetProjectionMatrix(void);

	//получить матрицу камеры (наблюдения)
	mat4 GetViewMatrix(void);

	//передвинуть камеру и точку налюдения в горизонтальной плоскости oXZ
	void MoveOXZ(float dForward, float dRight);

	//для корректной работы мышки при нажатии правой кнопки
	bool firstMouse = true;
	//вращения в горизонтальной и вертикальной плоскости 
	void Rotate(float dHorizAngle, float dVertAngle);

	//изменение fovy
	void Zoom(float dR);
};