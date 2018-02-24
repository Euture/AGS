#include <windows.h>
#include "stdio.h"

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CShader.h"

using namespace glm;

// ������������ ������ (���� ������ ����)
CShader		Shader;

// ���������� ��� ������ ������� (�������������� �� ���� �������������)
GLuint	VAO_Index = 0;		// ������ VAO-������
GLuint	VBO_Index = 0;		// ������ VBO-������
int		VertexCount = 0;	// ���������� ������

							// ������������� ���������� ��� ������ �������
void	InitObject(void) {

	// �������� � ���������� VBO
	glGenBuffers(1, &VBO_Index);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
	GLfloat	Verteces[] = {
		-0.5,	0.5,
		-0.5,	-0.5,
		0.5,	0.5,
		0.5,	0.5,
		-0.5,	-0.5,
		0.5,	-0.5
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(Verteces), Verteces, GL_STATIC_DRAW);

	// �������� VAO
	glGenVertexArrays(1, &VAO_Index);
	glBindVertexArray(VAO_Index);
	// ���������� VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
	int		k = Shader.GetAttribLocation("vPosition");
	glVertexAttribPointer(k, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(k);
	// "�������" ������ VAO, ���� �������� �� ���������
	glBindVertexArray(0);

	// �������� ���������� ������
	VertexCount = 6;
	return;
}


// ������� ���������� ��� ����������� ����
// � ��� ����� � �������������, �� �������� glutPostRedisplay
void Display(void)
{
	// �������� ����� �����
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// ���������� ������
	Shader.Activate();

	// ��������� uniform-����������
	vec4 Offset = vec4(0.5, 0.5, 0.0, 0.0);
	Shader.SetUniformVec4("Offset", Offset);

	vec4 Color = vec4(1.0, 1.0, 0.0, 1.0);
	Shader.SetUniformVec4("Color", Color);

	vec4 Color1 = vec4(1.0, 0.0, 0.0, 1.0);
	Shader.SetUniformVec4("Color1", Color1);

	// ����� �������
	glBindVertexArray(VAO_Index);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// ����� ��������� � ������� �������
	glutSwapBuffers();
};

// �������, ���������� ��� ��������� �������� ����
void Reshape(int w, int h)
{
	// ���������� ����� ������� ���������, ������ ���� ������� ����
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
};

// ������� ���������� ����� ��������� �����������, �.�. ����������� �����
void Simulation(void)
{
	//	������������ ����
	glutPostRedisplay();
};

void InitShaders()
{
	// �������� �������
	Shader.LoadVertexShader("SHADER\\Example.vsh");
	Shader.LoadFragmentShader("SHADER\\Example.fsh");
	Shader.Link();
}

void main(int argc, char **argv)
{
	// ������������� ���������� GLUT
	glutInit(&argc, argv);
	// ������������� ������� (������ ������)
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
	// ���������� � ������ OpenGL (������ 3.3 ��� ��������� �������� �������������)
	glutInitContextVersion(4,4);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	// ������������� ������� ����� ���� ����
	glutInitWindowPosition(300, 100);
	// ������������� ������ ����
	glutInitWindowSize(800, 600);
	// �������� ����
	glutCreateWindow("laba_01");

	//	������������� GLEW 
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Glew error: %s\n", glewGetErrorString(err));
		return;
	}
	// ����������� ������� ������ OpenGL
	printf("OpenGL Version = %s\n\n", glGetString(GL_VERSION));

	//������������� ��������
	InitShaders();
	
	// ������������� ������� ��� ������
	InitObject();

	// ������������� �������, ������� ����� ���������� ��� ����������� ����
	glutDisplayFunc(Display);
	// ������������� �������, ������� ����� ���������� ��� ��������� �������� ����
	glutReshapeFunc(Reshape);
	// ������������� ������� ������� ���������� ������ ���, ����� ��������� �����������
	glutIdleFunc(Simulation);
	// �������� ���� ��������� ��������� ��
	glutMainLoop();
	return;
};

