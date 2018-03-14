#include "CCamera.h"

//����������� �� ���������
CCamera::CCamera(void)
{
	ifstream fin("Position.txt");
	if (!fin.is_open())
	{
		cout << "file not open" << endl;
		Radius = 30;
		Theta = radians(5.0);
		Fi = 0;
		Center = vec3(0, 0, 0);
	}
	else
	{
		fin >> Radius >> Theta >> Fi >> Center.x >> Center.y >> Center.z;
		fin.close();
	}
	Eye.x = Radius * sin(Theta)* cos(Fi);
	Eye.y = Radius * cos(Theta);
	Eye.z = Radius * sin(Theta)* sin(Fi);
	Up = vec3(0, 1, 0);

	ViewMatrix = lookAt(Eye + Center, Center, Up);
	SavePos();
}

//���������� ������� ������
void CCamera::SavePos(void)
{
	ofstream fout("Position.txt");

	fout << Radius << ' '
		<< Theta << ' '
		<< Fi << ' '
		<< Center.x << ' '
		<< Center.y << ' '
		<< Center.z;
	fout.close();

	fout.close();
}

//����������
CCamera::~CCamera(void)
{
	SavePos();
}

//��������� ������� ��������
void CCamera::SetProjectionMatrix(float fovy, float aspect, float zNear, float zFar)
{
	ProjectionMatrix = perspective(
		radians(fovy),
		aspect,
		zNear,
		zFar
	);
}

//��������� ������� ��������
mat4 CCamera::GetProjectionMatrix(void)
{
	return ProjectionMatrix;
}

//�������� ������� ����������
mat4 CCamera::GetViewMatrix(void)
{
	return ViewMatrix;
}

//������������ � oXZ
void CCamera::MoveOXZ(float dForward, float dRight)
{
	dForward = dForward * Speed;
	dRight = dRight * Speed;

	vec3 VForward = normalize(-Eye);

	vec3 DeltaF = vec3(VForward.x * dForward, 0, VForward.z * dForward);

	vec3 DeltaR = normalize(cross(VForward, Up));
	DeltaR = vec3(DeltaR.x * dRight, 0, DeltaR.z * dRight);

	Center = Center + DeltaF + DeltaR;

	ViewMatrix = lookAt(Eye + Center, Center, Up);
	SavePos();
}

//�������� � ������������ � �������������� ���������
void CCamera::Rotate(float dHorizAngle, float dVertAngle)
{
	if ((dVertAngle > 0) & (Theta < Max_Theta)) Theta += dVertAngle / 360;
	if ((dVertAngle < 0) & (Theta > Min_Theta)) Theta += dVertAngle / 360;

	Fi += dHorizAngle / 360;
	if (Fi > 2 * PI) Fi -= 2 * PI;
	if (Fi < 0) Fi += 2 * PI;

	if (Theta < Min_Theta) Theta = Min_Theta;
	if (Theta > Max_Theta) Theta = Max_Theta;

	Eye.x = Radius * sin(Theta) * cos(Fi);
	Eye.y = Radius * cos(Theta);
	Eye.z = Radius * sin(Theta) * sin(Fi);
	ViewMatrix = lookAt(Eye + Center, Center, Up);
	SavePos();
}

//��������� �������
void CCamera::Zoom(float dR)
{
	if ((0 > dR) & (Radius < 50)) Radius = Radius - dR;
	if ((0 < dR) & (Radius > 4)) Radius = Radius - dR;
	Eye.x = Radius * sin(Theta) * cos(Fi);
	Eye.y = Radius * cos(Theta);
	Eye.z = Radius * sin(Theta) * sin(Fi);
	ViewMatrix = lookAt(Eye + Center, Center, Up);
	SavePos();
}