//420054085 Ortega de la Paz Rafael

#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	GLfloat getmueveCoche() { return mueveCoche; }
	GLfloat getmueveCofre() { return mueveCofre; }
	GLfloat getmueveLlanta() { return mueveLlanta; }

	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);
	}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }

	~Window();

private:
	GLFWwindow* mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX, lastY, xChange, yChange;
	bool mouseFirstMoved;

	GLfloat muevex;
	GLfloat mueveCoche;
	GLfloat mueveCofre;
	GLfloat mueveLlanta;

	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};