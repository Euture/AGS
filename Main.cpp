#include <windows.h>
#include "stdio.h"

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CShader.h"

using namespace glm;

// используемый шейдер (пока только один)
CShader		Shader;

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


// функция вызывается при перерисовке окна
// в том числе и принудительно, по командам glutPostRedisplay
void Display(void)
{
	// отчищаем буфер цвета
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// активируем шейдер
	Shader.Activate();

	// установка uniform-переменных
	vec4 Offset = vec4(0.5, 0.5, 0.0, 0.0);
	Shader.SetUniformVec4("Offset", Offset);

	vec4 Color = vec4(1.0, 1.0, 0.0, 1.0);
	Shader.SetUniformVec4("Color", Color);

	vec4 Color1 = vec4(1.0, 0.0, 0.0, 1.0);
	Shader.SetUniformVec4("Color1", Color1);

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
};

// функция вызывается когда процессор простаивает, т.е. максимально часто
void Simulation(void)
{
	//	ПЕРЕРИСОВАТЬ ОКНО
	glutPostRedisplay();
};

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
	glutCreateWindow("laba_01");

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
	InitObject();

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

