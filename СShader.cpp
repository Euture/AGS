#include "CShader.h"
#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <vector>

char ** CShader::ReadF(char *FileName)
{
	vector<string> temp;
	ifstream myfile(FileName);
	int i = 0;
	if (myfile.is_open())
	{
		while (!myfile.eof())
		{
			getline(myfile, temp.emplace_back());
			temp[i] += "\n\0";
			i++;
		}
		myfile.close();
	}
	else
		cout << "Unable to open file "<< FileName;

	int count = 0;
	char **converted = new char *[temp.size()];
	for (int i = 0; i < temp.size(); i++)
	{
		converted[i] = new char[temp[i].size() + 1];
		strcpy(converted[i], temp[i].c_str());
		count++;
		cout << converted[i];
	}

	if (FileName=="SHADER\\Example.vsh")
		VshStringCount = count;
	if (FileName == "SHADER\\Example.fsh")
		FshStringCount = count;
	return converted;
}

int CShader::LoadVertexShader(char *VertexFileName, bool DebugOutput)
{
	Vsh = glCreateShader(GL_VERTEX_SHADER);

	char ** TextShader = CShader::ReadF(VertexFileName);
	glShaderSource(Vsh, VshStringCount, TextShader, NULL);
	if (DebugOutput)
	{
		GLint CompileStatus;
		glCompileShader(Vsh);
		glGetShaderiv(Vsh, GL_COMPILE_STATUS, &CompileStatus);
		if (CompileStatus == GL_FALSE)
		{
			char *InfoLog = new char[100];
			GLsizei log_length = 0;
			glGetShaderInfoLog(Vsh, 100, &log_length, InfoLog);
			cout << "Error Compile Vertex shader" << endl;
			cout << InfoLog << endl << endl;
		}
		else cout << "Compile Vertex shader successful" << endl << endl;
	}

	return Vsh;
}

int CShader::LoadFragmentShader(char *FragmentFileName, bool DebugOutput)
{
	Fsh = glCreateShader(GL_FRAGMENT_SHADER);

	char ** TextShader = CShader::ReadF(FragmentFileName);
	glShaderSource(Fsh, FshStringCount, TextShader, NULL);

	if (DebugOutput)
	{
		GLint CompileStatus;
		glCompileShader(Fsh);
		glGetShaderiv(Fsh, GL_COMPILE_STATUS, &CompileStatus);
		if (CompileStatus == GL_FALSE)
		{
			char * InfoLog = new char[100];
			glGetShaderInfoLog(Fsh, 100, (int*)100, InfoLog);
			cout << "Error Compile  Fragment shader" << endl;
			cout << InfoLog << endl << endl;
		}
		else cout << "Compile Fragment shader successful" << endl << endl;
	}

	return Fsh;
}

int CShader::Link(bool DebugOutput)
{
	Program = glCreateProgram();
	glAttachShader(Program, Vsh);
	glAttachShader(Program, Fsh);
	glLinkProgram(Program);
	if (DebugOutput)
	{
		GLint LinkStatus;
		glCompileShader(Fsh);
		glGetShaderiv(Fsh, GL_COMPILE_STATUS, &LinkStatus);
		if (LinkStatus == GL_FALSE)
		{
			char * InfoLog = new char[100];
			glGetProgramInfoLog(Program, 100, (int*)100, InfoLog);
			cout << "Error Link program" << endl;
			cout << InfoLog << endl << endl;
		}
		else cout << "Link program successful" << endl << endl;
	}
	return Program;
}

void CShader::Activate()
{
	glUseProgram(Program);
}

void CShader::Deactivate()
{
	glUseProgram(0);
}

void CShader::SetUniformVec4(string str, vec4 &value)
{
	int	k = glGetUniformLocation(Program, str.c_str());
	if (k < 0) return;
	glUseProgram(Program);
	glUniform4fv(k, 1, value_ptr(value));
	UniformId.insert(pair<string, int>(str, k));
}

void CShader::SetUniformMat4(string str, mat4 &value)
{
	int	k = glGetUniformLocation(Program, str.c_str());
	if (k < 0) return;
	glUseProgram(Program);
	glUniformMatrix4fv(k, 1, GL_FALSE, value_ptr(value));
	UniformId.insert(pair<string, int>(str, k));
}

int CShader::GetUniformId(const string &name)
{
	auto fi = UniformId.find(name);
	if (fi != UniformId.end())
	{
		cout << "Ёлемент найден" << endl;
		cout << "«начение равно = " << fi->second << endl;
	}
	else
	{
		cout << "Ёлемент не найден" << endl;
	}
	return fi->second;
}