/*Práctica 4: Modelado Jerárquico.
Se implementa el uso de matrices adicionales para almacenar información de transformaciones geométricas que se quiere
heredar entre diversas instancias para que estén unidas
Teclas de la F a la K para rotaciones de articulaciones
*/
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"

using std::vector;

//Dimensiones de la ventana y constantes
const float toRadians = 3.14159265f / 180.0;
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;

//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
Sphere sp = Sphere(1.0, 20, 20);

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		0, 1, 2, 2, 3, 0, // front
		1, 5, 6, 6, 2, 1, // right
		7, 6, 5, 5, 4, 7, // back
		4, 0, 3, 3, 7, 4, // left
		4, 5, 1, 1, 0, 4, // bottom
		3, 2, 6, 6, 7, 3  // top
	};

	GLfloat cubo_vertices[] = {
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2, 1,3,2, 3,0,2, 1,0,3
	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		0.0f,0.5f, -0.25f,
		0.0f,-0.5f,-0.5f,
	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);
}

void CrearCilindro(int res, float R) {
	int n, i;
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		else {
			x = R * cos((0) * dt);
			z = R * sin((0) * dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0: vertices.push_back(x); break;
			case 1: vertices.push_back(y); break;
			case 2: vertices.push_back(z); break;
			case 3: vertices.push_back(x); break;
			case 4: vertices.push_back(0.5); break;
			case 5: vertices.push_back(z); break;
			}
		}
	}
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt); z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0: vertices.push_back(x); break;
			case 1: vertices.push_back(-0.5f); break;
			case 2: vertices.push_back(z); break;
			}
		}
	}
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt); z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0: vertices.push_back(x); break;
			case 1: vertices.push_back(0.5); break;
			case 2: vertices.push_back(z); break;
			}
		}
	}
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);
	Mesh* cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

void CrearCono(int res, float R) {
	int n, i;
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	vertices.push_back(0.0); vertices.push_back(0.5); vertices.push_back(0.0);
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt); z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0: vertices.push_back(x); break;
			case 1: vertices.push_back(y); break;
			case 2: vertices.push_back(z); break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt); vertices.push_back(-0.5); vertices.push_back(R * sin(0) * dt);
	for (i = 0; i < res + 2; i++) indices.push_back(i);
	Mesh* cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = { 0,3,4, 3,2,4, 2,1,4, 1,0,4, 0,1,2, 0,2,4 };
	vector<GLfloat> piramidecuadrangular_vertices = { 0.5f,-0.5f,0.5f, 0.5f,-0.5f,-0.5f, -0.5f,-0.5f,-0.5f, -0.5f,-0.5f,0.5f, 0.0f,0.5f,0.0f };
	Mesh* piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(1920, 1080);
	mainWindow.Initialise();

	CrearCubo(); CrearPiramideTriangular(); CrearCilindro(20, 1.0f); CrearCono(25, 2.0f); CrearPiramideCuadrangular();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.2f, 0.2f);
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	sp.init();
	sp.load();

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		// ==========================================
		// SECCIÓN: DIBUJAR LA CABINA (NODO PADRE)
		// ==========================================
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 6.0f, -4.0f));
		modelaux = model; // Guardamos la transformación de la cabina para los hijos
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.0f)));
		model = glm::scale(model, glm::vec3(8.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[0]->RenderMesh();
		model = modelaux; // Recuperamos la matriz sin la escala de la cabina

		// ==========================================
		// SECCIÓN: ARTICULACIÓN 1 (BASE DEL BRAZO)
		// ==========================================
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		// ==========================================
		// SECCIÓN: BRAZO 1
		// ==========================================
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		modelaux = model; // Guardamos traslación para la siguiente articulación
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 1.0f)));
		meshList[0]->RenderMesh();

		model = modelaux; // Recuperamos matriz sin escala del brazo 1

		// ==========================================
		// SECCIÓN: ARTICULACIÓN 2
		// ==========================================
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		// Dibujamos esfera de articulación
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux; // Recuperamos matriz sin escala de esfera

		// ==========================================
		// SECCIÓN: BRAZO 2
		// ==========================================
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		model = modelaux; // Recuperamos matriz sin escala del brazo 2

		// ==========================================
		// SECCIÓN: ARTICULACIÓN 3
		// ==========================================
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		// Dibujamos esfera de articulación
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux; // Recuperamos matriz sin escala de esfera

		// ==========================================
		// SECCIÓN: BRAZO 3
		// ==========================================
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 1.0f)));
		meshList[0]->RenderMesh();
		model = modelaux; // Recuperamos matriz sin escala del brazo 3

		// ==========================================
		// SECCIÓN: ARTICULACIÓN 4
		// ==========================================
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		model = modelaux; // Recuperamos matriz sin escala de esfera

		// ==========================================
	// SECCIÓN: CANASTA (EXTREMO FINAL)
	// ==========================================

	// 1. Extraemos la posición del brazo (esto no cambia)
		glm::vec3 posBrazo = glm::vec3(model[3]);

		// 2. Limpiamos matriz
		model = glm::mat4(1.0f);

		// 3. Trasladamos a la punta del brazo
		model = glm::translate(model, posBrazo);

		// 4. CAMBIO CLAVE: Aplicamos la rotación de 180 en Z y la de articulacion5 en Y
		// Lo hacemos en una sola línea para evitar que se peleen los ejes
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 1.0f, 0.0f));

		// 5. Ajuste de altura (prueba con -1.0f si sale volando o +1.0f si queda enterrada)
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.0f)));
		meshList[0]->RenderMesh();
		// ==========================================
		// SECCIÓN: REESTABLECER ORIGEN (RAÍZ)
		// ==========================================
		// Reiniciamos la matriz para "olvidar" los brazos y volver a la cabina
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 6.0f, -4.0f));
		modelaux = model; // Ahora modelaux vuelve a ser la referencia de la cabina

		// ==========================================
		// SECCIÓN: PIRÁMIDE CUADRANGULAR (BASE)
		// ==========================================
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		glm::mat4 modelaux_base = model;
		model = glm::scale(model, glm::vec3(6.0f, 2.0f, 6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f); // Azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[4]->RenderMesh();
		model = modelaux_base;

		// ==========================================
		// ARTICULACIÓN 5 (Esquina Frontal Derecha)
		// ==========================================
		model = modelaux_base;
		model = glm::translate(model, glm::vec3(3.0f, -1.0f, 3.25f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
		sp.render();

		// ==========================================
		// LLANTA 1 (Esquina Frontal Derecha)
		// ==========================================
		model = modelaux;
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.0f)));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // 2. Se aplica después: la acuesta
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 1.0f, 0.0f)); // 1. Se aplica primero: gira como disco
		model = glm::scale(model, glm::vec3(1.0f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMeshGeometry();
		model = modelaux_base;

		// ==========================================
		// ARTICULACIÓN 6 (Esquina Frontal Izquierda)
		// ==========================================
		model = modelaux_base;
		model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 3.25f)); // X negativa para la izquierda
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
		sp.render();

		// ==========================================
		// LLANTA 2 (Esquina Frontal Izquierda)
		// ==========================================
		model = modelaux;
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.0f)));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 1.0f, 0.0f)); // CAMBIADO A 5 PARA GIRAR CON K
		model = glm::scale(model, glm::vec3(1.0f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMeshGeometry();
		model = modelaux_base;

		// ==========================================
		// ARTICULACIÓN 7 (Esquina Trasera Derecha)
		// ==========================================
		model = modelaux_base;
		model = glm::translate(model, glm::vec3(3.0f, -1.0f, -3.25f)); // Z negativa para ir atrás
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
		sp.render();

		// ==========================================
		// LLANTA 3 (Esquina Trasera Derecha)
		// ==========================================
		model = modelaux;
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.0f)));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 1.0f, 0.0f)); // Gira con K
		model = glm::scale(model, glm::vec3(1.0f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMeshGeometry();
		model = modelaux_base;

		// ==========================================
		// ARTICULACIÓN 8 (Esquina Trasera Izquierda)
		// ==========================================
		model = modelaux_base;
		model = glm::translate(model, glm::vec3(-3.0f, -1.0f, -3.25f)); // X y Z negativas
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
		sp.render();

		// ==========================================
		// LLANTA 4 (Esquina Trasera Izquierda)
		// ==========================================
		model = modelaux;
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.0f)));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(0.0f, 1.0f, 0.0f)); // Gira con K
		model = glm::scale(model, glm::vec3(1.0f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMeshGeometry();
		model = modelaux_base;
		

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}