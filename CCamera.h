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

//�������� ������ � �������
class CCamera
{
	//������� ��� ViewMatrix
	vec3 eye;
	vec3 at;
	vec3 up;

	//�������� ������
	GLfloat cameraSpeed = 1;

	//����� ������ ������ �������� �� at
	vec3 cameraFront = vec3(-1.0f, 0.0f, -1.0f);

	//���������� ����
	GLfloat lastX;
	GLfloat lastY;

	//��������
	GLfloat yaw = -90.0f;
	//���������
	GLfloat pitch = 5.1f;

	//������ ��� �����������
	GLfloat Fovy;
	GLfloat Aspect;
	GLfloat ZNear;
	GLfloat ZFar;

	//������� ����������
	mat4 ViewMatrix;
	//������� ��������
	mat4 ProjectionMatrix;

public:
	//����������� �� ���������
	CCamera(void);
	//����������
	~CCamera(void);

	//��������� ������� ������ � ����
	void SavePos(void);

	//��������� ������� ��������
	void SetProjectionMatrix(float fovy, float aspect, float zNear, float zFar);
	//��������� ������� ��������
	mat4 GetProjectionMatrix(void);

	//�������� ������� ������ (����������)
	mat4 GetViewMatrix(void);

	//����������� ������ � ����� ��������� � �������������� ��������� oXZ
	void MoveOXZ(bool CameraLeft, bool CameraRight,
				 bool CameraForward, bool CameraBackward, float Simulation_Time_Passed);

	//��� ���������� ������ ����� ��� ������� ������ ������
	bool firstMouse = true;
	//�������� � �������������� � ������������ ��������� 
	void Rotate(float Xpos, float Ypos, float Simulation_Time_Passed);

	//��������� fovy
	void Zoom(float dFovy);
};