#include "CCamera.h"

//конструктор по умолчанию
CCamera::CCamera(void)
{
	ifstream fin("Position.txt");
	if (!fin.is_open())
	{
		cout << "file not open";
		eye = vec3(0.1f, 0.1f, 0.1f);
		at = eye + cameraFront;
		up = vec3(0.0f, 1.0f, 0.0f);
	}
	else
	{
		GLfloat x, y, z;

		fin >> x;
		fin >> y;
		fin >> z;
		eye = vec3(x, y, z);

		fin >> x;
		fin >> y;
		fin >> z;
		at = vec3(x, y, z);

		fin >> x;
		fin >> y;
		fin >> z;
		up = vec3(x, y, z);

		fin.close();
	}
	ViewMatrix = lookAt(eye, at, up);
	SavePos();
}
  
// сохраняет позицию камеры
void CCamera::SavePos(void)
{
	ofstream fout("Position.txt");

	fout << eye.x << " ";
	fout << eye.y << " ";
	fout << eye.z << " ";

	fout << at.x << " ";
	fout << at.y << " ";
	fout << at.z << " ";

	fout << up.x << " ";
	fout << up.y << " ";
	fout << up.z << " ";

	fout.close();
}

//деструктор
CCamera::~CCamera(void)
{
	SavePos();
}

//установка матрицы проекции
void CCamera::SetProjectionMatrix(float fovy, float aspect, float zNear, float zFar)
{
	Fovy = fovy;
	Aspect = aspect;
	ZNear = zNear;
	ZFar = zFar;
	ProjectionMatrix = perspective(
									radians(fovy),
									aspect,
									zNear,
									zFar
								 );
}

//получение матрицы проекции
mat4 CCamera::GetProjectionMatrix(void)
{
	return ProjectionMatrix;
}

//получить матрицу камеры (наблюдения)
mat4 CCamera::GetViewMatrix(void)
{
	return ViewMatrix;
}

//передвинуть камеру и точку налюдения в горизонтальной плоскости oXZ
void CCamera::MoveOXZ(bool CameraLeft, bool CameraRight,
						bool CameraForward, bool CameraBackward,float Simulation_Time_Passed)
{
	if (CameraBackward)
		eye += cameraSpeed * cameraFront * Simulation_Time_Passed;
	if (CameraForward)
		eye -= cameraSpeed * cameraFront * Simulation_Time_Passed;
	if (CameraLeft)
		eye -= glm::normalize(glm::cross(cameraFront, up)) * cameraSpeed * Simulation_Time_Passed;
	if (CameraRight)
		eye += glm::normalize(glm::cross(cameraFront, up)) * cameraSpeed * Simulation_Time_Passed;
	at = eye + cameraFront;
	ViewMatrix = lookAt(eye, at, up);
	SavePos();
}

//вращения в горизонтальной и вертикальной плоскости 
void CCamera::Rotate(float Xpos, float Ypos, float Simulation_Time_Passed)
{
	if (firstMouse)
	{
		lastX = Xpos;
		lastY = Ypos;
		firstMouse = false;
	}
	else
	{
		GLfloat xoffset = Xpos - lastX;
		GLfloat yoffset = lastY - Ypos;
		lastX = Xpos;
		lastY = Ypos;

		GLfloat sensitivity = 10 * Simulation_Time_Passed;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		//вертикаль
		if (((pitch + yoffset) < 85) && ((pitch + yoffset) > -85))
		{
			pitch += yoffset;
		}
		//горизонт
		yaw += xoffset;

		vec3 front;
		front.x = cos(radians(yaw)) * cos(radians(pitch));
		front.y = sin(radians(pitch));
		front.z = sin(radians(yaw)) * cos(radians(pitch));
		cameraFront = normalize(front);
		at = eye + cameraFront;
		ViewMatrix = lookAt(eye, at, up);
		SavePos();
	}

	
}

//изменение fovy
void CCamera::Zoom(float dFovy)
{
	Fovy -= dFovy;
	SetProjectionMatrix(Fovy, Aspect, ZNear, ZFar);
}