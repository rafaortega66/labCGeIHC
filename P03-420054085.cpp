// P03-420054085.cpp - Rafael Ortega de la Paz - 420054085
// Pr塶tica 3: Modelado Geom彋rico - Pyraminx
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"

using std::vector;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader> shaderList;

static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";

void CrearPiramideTriangular()
{
	unsigned int indices[] = { 0, 1, 2, 1, 3, 2, 3, 0, 2, 1, 0, 3 };
	GLfloat vertices[] = {
		-0.5f, -0.25f,  0.28f,
		 0.5f, -0.25f,  0.28f,
		 0.0f, -0.25f, -0.57f,
		 0.0f,  0.55f,  0.0f
	};
	Mesh* obj = new Mesh();
	obj->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(1366, 768);
	mainWindow.Initialise();
	CrearPiramideTriangular();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 1.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -15.0f, 10.0f, 0.5f);

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;

	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		// --- L笉ICA DE ROTACI粍 ---
		float autoAngle = now * 100.0f; // Velocidad de rotacion lenta (20 grados por segundo)
		glm::mat4 autoRot = glm::mat4(1.0f);
		autoRot = glm::rotate(autoRot, glm::radians(autoAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotacion en Y
		autoRot = glm::rotate(autoRot, glm::radians(autoAngle * 0.5f), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotacion en X (mitad de velocidad)

		// Aplicamos la rotacion acumulada a la matriz de vista de la camara
		glm::mat4 viewMatrix = camera.calculateViewMatrix() * autoRot;
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		// --------------------------------
	
		// --- 1. BASE NEGRA (CORE) ---
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(uniformColor, 0.0f, 0.0f, 0.0f);
		meshList[0]->RenderMesh();

		// --- 2. CARA FRONTAL (ROSA) ---
		glUniform3f(uniformColor, 1.0f, 0.5f, 0.7f);

		// Pieza 1: 聯ice
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.10f, 0.05f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 2: Fila Media Izq
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.5f, 0.30f, 0.34f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 3: Fila Media Der
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.5f, 0.30f, 0.34f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 4: Invertida Central
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.49f, 0.263));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.33f, 0.944f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 5: Inferior Izquierda
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, -0.50f, 0.61f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 6: Inferior Centro
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.50f, 0.61f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 7: Inferior Derecha
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, -0.50f, 0.61f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 8: Invertida Inf Izquierda
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.5f, -0.3f, 0.54f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.33f, 0.944f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 9: Invertida Inf Derecha
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.5f, -0.3f, 0.54f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.33f, 0.944f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();


		// --- 3. CARA LATERAL DERECHA (VERDE) ---
		glUniform3f(uniformColor, 0.0f, 1.0f, 0.5f);

		// Pieza 1: 聯ice
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 1.10f, 0.05f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 2: Fila Media Izq
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.5f, 0.30f, 0.34f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 3: Fila Media Der
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.5f, 0.30f, 0.34f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 4: Invertida Central
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.49f, 0.293f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.33f, 0.944f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 5: Inferior Izquierda
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-1.0f, -0.50f, 0.63f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 6: Inferior Centro
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.50f, 0.63f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 7: Inferior Derecha
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.0f, -0.50f, 0.63f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 8: Invertida Inf Izquierda
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.5f, -0.3f, 0.57f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.33f, 0.944f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 9: Invertida Inf Derecha
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.5f, -0.3f, 0.57f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.33f, 0.944f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// --- 4. CARA LATERAL IZQUIERDA (AMARILLA) ---
		glUniform3f(uniformColor, 1.0f, 1.0f, 0.4f);

		// Pieza 1: 聯ice
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 1.10f, 0.05f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 2: Fila Media Izq
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.5f, 0.30f, 0.34f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 3: Fila Media Der
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.5f, 0.30f, 0.34f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 4: Invertida Central
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.49f, 0.293f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.33f, 0.944f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 5: Inferior Izquierda
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-1.0f, -0.50f, 0.63f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 6: Inferior Centro
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.50f, 0.63f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 7: Inferior Derecha
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.0f, -0.50f, 0.63f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 8: Invertida Inf Izquierda
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.5f, -0.3f, 0.57f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.33f, 0.944f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 9: Invertida Inf Derecha
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.5f, -0.3f, 0.57f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.33f, 0.944f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// --- 5. CARA INFERIOR (NARANJA) ---
		glUniform3f(uniformColor, 1.0f, 0.35f, 0.0f);

		// Pieza 1: 聯ice
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.57f, -1.1f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 2: Fila Media Izq
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.5f, -0.57f, -0.3f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 3: Fila Media Der
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.5f, -0.57f, -0.3f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 4: Invertida Central
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.57f, -0.56f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 5: Inferior Izquierda
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, -0.57f, 0.56f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 6: Inferior Centro
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.57f, 0.56f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 7: Inferior Derecha
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, -0.57f, 0.56f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 8: Invertida Inf Izquierda
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.5f, -0.57f, 0.26f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Pieza 9: Invertida Inf Derecha
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.5f, -0.57f, 0.26f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}