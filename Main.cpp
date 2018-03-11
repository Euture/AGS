#include <windows.h>
#include "stdio.h"
#include <string>

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glfw3.h"

#include "CShader.h"
#include "CCamera.h"

using namespace glm;
using namespace std;
// время между вызовами simulation
static GLfloat Simulation_Time_Passed = 0;
LARGE_INTEGER OldValue, NewValue, Time;
GLint FPSCounter = 0;
GLint FPS = 0;
GLdouble oneSecond = 0;
string Title;

// движение 
POINT Last , Now;

// используемый шейдер (пока только один)
CShader		Shader;

// камера
CCamera Camera;

// переменные для вывода объекта (прямоугольника из двух треугольников)
GLuint	VAO_Index = 0;		// индекс VAO-буфера
GLuint	VBO_Index = 0;		// индекс VBO-буфера
int		VertexCount = 0;	// количество вершин

							// инициализация переменных для вывода объекта
void	InitObject(void) {

	// создание и заполнение VBO
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

	// создание VAO
	glGenVertexArrays(1, &VAO_Index);
	glBindVertexArray(VAO_Index);
	// заполнение VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
	int		k = Shader.GetAttribLocation("vPosition");
	glVertexAttribPointer(k, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(k);
	// "отвязка" буфера VAO, чтоб случайно не испортить
	glBindVertexArray(0);

	// указание количество вершин
	VertexCount = 6;
	return;
}

void DrawCube(CShader &shader)
{
	// переменные для вывода объекта (прямоугольника из двух треугольников) 
	static GLuint VAO_Index = 0; // индекс VAO‐буфера 
	static GLuint VBO_Index = 0; // индекс VBO‐буфера 
	static int VertexCount = 0; // количество вершин 
	static bool init = true;
	if (init)
	{
		// создание и заполнение VBO 
		glGenBuffers(1, &VBO_Index);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
		GLfloat Verteces[] =
		{
			// передняя грань 
			-0.5, +0.5, +0.5,
			-0.5, -0.5, +0.5,
			+0.5, +0.5, +0.5,
			+0.5, +0.5, +0.5,
			-0.5, -0.5, +0.5,
			+0.5, -0.5, +0.5,
			// задняя грань 
			+0.5, +0.5, -0.5,
			+0.5, -0.5, -0.5,
			-0.5, +0.5, -0.5,
			-0.5, +0.5, -0.5,
			+0.5, -0.5, -0.5,
			-0.5, -0.5, -0.5,
			// правая грань 
			+0.5, -0.5, +0.5,
			+0.5, -0.5, -0.5,
			+0.5, +0.5, +0.5,
			+0.5, +0.5, +0.5,
			+0.5, -0.5, -0.5,
			+0.5, +0.5, -0.5,
			// левая грань 
			-0.5, +0.5, +0.5,
			-0.5, +0.5, -0.5,
			-0.5, -0.5, +0.5,
			-0.5, -0.5, +0.5,
			-0.5, +0.5, -0.5,
			-0.5, -0.5, -0.5,
			// верхняя грань 
			-0.5, +0.5, -0.5,
			-0.5, +0.5, +0.5,
			+0.5, +0.5, -0.5,
			+0.5, +0.5, -0.5,
			-0.5, +0.5, +0.5,
			+0.5, +0.5, +0.5,
			// нижняя грань 
			-0.5, -0.5, +0.5,
			-0.5, -0.5, -0.5,
			+0.5, -0.5, +0.5,
			+0.5, -0.5, +0.5,
			-0.5, -0.5, -0.5,
			+0.5, -0.5, -0.5
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(Verteces), Verteces, GL_STATIC_DRAW);
		// создание VAO 
		glGenVertexArrays(1, &VAO_Index);
		glBindVertexArray(VAO_Index);
		// заполнение VAO 
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
		int k = shader.GetAttribLocation("vPosition");
		glVertexAttribPointer(k, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(k);
		// "отвязка" буфера VAO, чтоб случайно не испортить 
		glBindVertexArray(0);
		// указание количество вершин 
		VertexCount = 36;
		init = false;
	}
	glBindVertexArray(VAO_Index);
	glDrawArrays(GL_TRIANGLES, 0, VertexCount);
}

// функция вызывается при перерисовке окна
// в том числе и принудительно, по командам glutPostRedisplay
void Display(void)
{
	// отчищаем буфер цвета
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// активируем шейдер
	Shader.Activate();

	// получаем матрицу проекции
	mat4 ProjectionMatrix = Camera.GetProjectionMatrix();
	// устанавливаем матрицу проекции
	Shader.SetUniformMat4("ProjectionMatrix", ProjectionMatrix);
	
	// получаем матрицу наблюдения
	mat4 ViewMatrix = Camera.GetViewMatrix();
	
	// выводим 1 модель
	mat4 ModelMatrix1 = mat4(
		vec4(1, 0, 0, 0), // направление ox
		vec4(0, 1, 0, 0), // направление oy
		vec4(0, 0, 1, 0), // направление oz
		vec4(3, 0, 0, 1)  // позицияя объекта
	);
	// устанавливаем матрицу наблюдения модели
	mat4 ModelViewMatrix1 = ViewMatrix * ModelMatrix1;
	Shader.SetUniformMat4("ModelViewMatrix", ModelViewMatrix1);
	
	// установка uniform-переменных
	vec4 Color1 = vec4(1.0, 0.0, 0.0, 1.0);
	Shader.SetUniformVec4("Color", Color1);

	// вывод объекта 
	DrawCube(Shader);

	// выводим 2 модель
	mat4 ModelMatrix2 = mat4(
		vec4(1, 0, 0, 0), // направление ox
		vec4(0, 1, 0, 0), // направление oy
		vec4(0, 0, 1, 0), // направление oz
		vec4(-3, 0, 0, 1)  // позицияя объекта
	);
	// устанавливаем матрицу наблюдения модели
	mat4 ModelViewMatrix2 = ViewMatrix * ModelMatrix2;
	Shader.SetUniformMat4("ModelViewMatrix", ModelViewMatrix2);

	// установка uniform-переменных
	vec4 Color2 = vec4(0.0, 0.0, 1.0, 1.0);
	Shader.SetUniformVec4("Color", Color2);

	// вывод объекта 
	DrawCube(Shader);

	// вывод объекта
	glBindVertexArray(VAO_Index);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// смена переднего и заднего буферов
	glutSwapBuffers();
};

// функция, вызываемая при изменении размеров окна
void Reshape(int w, int h)
{
	// установить новую область просмотра, равную всей области окна
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	// установить матрицу проекции камеры
	Camera.SetProjectionMatrix(45.0, float(w) / h, 1, 100);
	
}

// фпс
void ShowFPS(GLfloat time)
{
	oneSecond += time;
	if (oneSecond < 1)FPSCounter++;
	else
	{
		FPS = FPSCounter;
		FPSCounter = 0;
		oneSecond = 0;
	}
}


// функция вызывается когда процессор простаивает, т.е. максимально часто
void Simulation(void)
{
	
	// подсчет фпс + title окна
	OldValue = NewValue;
	QueryPerformanceCounter(&NewValue);
	QueryPerformanceFrequency(&Time);
	GLdouble Simulation_Time_Passed = ((GLdouble)NewValue.QuadPart / Time.QuadPart) - ((GLdouble)OldValue.QuadPart / Time.QuadPart);
	ShowFPS(Simulation_Time_Passed);
	Title = "Lab #2: [" + to_string(FPS) + "]";
	glutSetWindowTitle(Title.c_str());

	// определяем необходимость передвижения камеры
	bool CameraLeft = GetAsyncKeyState(VK_LEFT);
	bool CameraRight = GetAsyncKeyState(VK_RIGHT);
	bool CameraForward = GetAsyncKeyState(VK_DOWN);
	bool CameraBackward = GetAsyncKeyState(VK_UP);

	if (CameraLeft || CameraRight || CameraForward || CameraBackward)
	{
		Camera.MoveOXZ(CameraLeft, CameraRight, CameraForward, CameraBackward, Simulation_Time_Passed);
	}

	// определяем необходимость вращения
	bool Mouse = GetAsyncKeyState(VK_RBUTTON);
	//	ПЕРЕРИСОВАТЬ ОКНО
	glutPostRedisplay();
}

void InitShaders()
{
	// загрузка шейдера
	Shader.LoadVertexShader("SHADER\\Example.vsh");
	Shader.LoadFragmentShader("SHADER\\Example.fsh");
	Shader.Link();
}

void main(int argc, char **argv)
{
	// инициализация библиотеки GLUT
	glutInit(&argc, argv);
	// инициализация дисплея (формат вывода)
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
	// требования к версии OpenGL (версия 3.3 без поддержки обратной совместимости)
	glutInitContextVersion(4,4);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	// устанавливаем верхний левый угол окна
	glutInitWindowPosition(300, 100);
	// устанавливаем размер окна
	glutInitWindowSize(800, 600);
	// создание окна
	glutCreateWindow("Lab #2");

	//	инициализация GLEW 
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Glew error: %s\n", glewGetErrorString(err));
		return;
	}
	// определение текущей версии OpenGL
	printf("OpenGL Version = %s\n\n", glGetString(GL_VERSION));

	//инициализация шейдеров
	InitShaders();
	
	// инициализация объекта для вывода
	//InitObject();

	// устанавливаем функцию, которая будет вызываться для перерисовки окна
	glutDisplayFunc(Display);
	// устанавливаем функцию, которая будет вызываться при изменении размеров окна
	glutReshapeFunc(Reshape);
	// устанавливаем функцию которая вызывается всякий раз, когда процессор простаивает
	glutIdleFunc(Simulation);
	// основной цикл обработки сообщений ОС
	glutMainLoop();
	return;
};
