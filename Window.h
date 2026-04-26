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
	GLfloat getPosicionLuz() { return posicionLuz; }
	GLfloat getmueveHelicoptero() { return mueveHelicoptero; }
	bool	getLampSw() { return lampSw;}
	int		getContador() { return contador; }
	GLfloat getMuevePez() { return muevePez; }
	bool	getPezSw() { return pezSw; }
	GLfloat getMueveFocoX() { return mueveFocoX; }
	GLfloat getMueveFocoY() { return mueveFocoY; }
	GLfloat getMueveFocoZ() { return mueveFocoZ; }



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
	GLfloat posicionLuz;
	GLfloat mueveHelicoptero;
	bool	lampSw;
	int		contador;
	GLfloat muevePez;
	bool	pezSw;
	GLfloat mueveFocoX;
	GLfloat mueveFocoY;
	GLfloat mueveFocoZ;



	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};