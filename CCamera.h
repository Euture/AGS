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

//классдля работы с камерой
class CCamera
{
	//вектора для ViewMatrix
	vec3 eye;
	vec3 at;
	vec3 up;
	//скорость камеры
	float cameraSpeed = 1;

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
	void MoveOXZ(bool CameraLeft, bool CameraRight,
				 bool CameraForward, bool CameraBackward, float Simulation_Time_Passed);
	//вращения в горизонтальной и вертикальной плоскости 
	void Rotate(float dHorizAngle, float dVertAngle);
	//приближение/отдаление
	void Zoom(float dR);
};