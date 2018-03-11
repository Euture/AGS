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
	float cameraSpeed = 1;

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
	//�������� � �������������� � ������������ ��������� 
	void Rotate(float dHorizAngle, float dVertAngle);
	//�����������/���������
	void Zoom(float dR);
};