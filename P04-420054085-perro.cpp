/*Práctica 5: Perro Robot - Modelado Jerárquico */
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

const float toRadians = 3.14159265f / 180.0;
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;

static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
Sphere sp = Sphere(1.0, 20, 20);

// --- FUNCIONES DE CREACIÓN DE GEOMETRÍA SE MANTIENEN ---
void CrearCubo() {
	unsigned int cubo_indices[] = {
		0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1, 7, 6, 5, 5, 4, 7, 4, 0, 3, 3, 7, 4, 4, 5, 1, 1, 0, 4, 3, 2, 6, 6, 7, 3
	};
	GLfloat cubo_vertices[] = {
		-0.5f, -0.5f,  0.5f, 0.5f, -0.5f,  0.5f, 0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}
void CrearPiramideTriangular() {
	unsigned int indices_piramide_triangular[] = { 0,1,2, 1,3,2, 3,0,2, 1,0,3 };
	GLfloat vertices_piramide_triangular[] = { -0.5f, -0.5f,0.0f, 0.5f,-0.5f,0.0f, 0.0f,0.5f, -0.25f, 0.0f,-0.5f,-0.5f };
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);
}
void CrearCilindro(int res, float R) {
	int n, i;
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	vector<GLfloat> vertices; vector<unsigned int> indices;
	for (n = 0; n <= (res); n++) {
		x = (n != res) ? R * cos((n)*dt) : R * cos((0) * dt);
		z = (n != res) ? R * sin((n)*dt) : R * sin((0) * dt);
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0: vertices.push_back(x); break; case 1: vertices.push_back(y); break; case 2: vertices.push_back(z); break;
			case 3: vertices.push_back(x); break; case 4: vertices.push_back(0.5); break; case 5: vertices.push_back(z); break;
			}
		}
	}
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt); z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) { case 0: vertices.push_back(x); break; case 1: vertices.push_back(-0.5f); break; case 2: vertices.push_back(z); break; }
		}
	}
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt); z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) { case 0: vertices.push_back(x); break; case 1: vertices.push_back(0.5); break; case 2: vertices.push_back(z); break; }
		}
	}
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);
	Mesh* cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}
void CrearCono(int res, float R) {
	int n, i; GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	vector<GLfloat> vertices; vector<unsigned int> indices;
	vertices.push_back(0.0); vertices.push_back(0.5); vertices.push_back(0.0);
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt); z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) { case 0: vertices.push_back(x); break; case 1: vertices.push_back(y); break; case 2: vertices.push_back(z); break; }
		}
	}
	vertices.push_back(R * cos(0) * dt); vertices.push_back(-0.5); vertices.push_back(R * sin(0) * dt);
	for (i = 0; i < res + 2; i++) indices.push_back(i);
	Mesh* cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}
void CrearPiramideCuadrangular() {
	vector<unsigned int> piramidecuadrangular_indices = { 0,3,4, 3,2,4, 2,1,4, 1,0,4, 0,1,2, 0,2,4 };
	vector<GLfloat> piramidecuadrangular_vertices = { 0.5f,-0.5f,0.5f, 0.5f,-0.5f,-0.5f, -0.5f,-0.5f,-0.5f, -0.5f,-0.5f,0.5f, 0.0f,0.5f,0.0f };
	Mesh* piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}
void CreateShaders() {
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main() {
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

	while (!mainWindow.getShouldClose()) {
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

		// Mandar matrices de Proyección y Vista una vez por frame
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		// ==========================================
		// NODO PADRE: CUERPO
		// ==========================================
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f)); // Altura base
		modelaux = model;
		model = glm::scale(model, glm::vec3(4.0f, 2.0f, 6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
		meshList[0]->RenderMesh();
		model = modelaux;

		// ==========================================
		// SECCIÓN: CUELLO (Independiente del Cuerpo)
		// ==========================================
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 2.5f));
		model = glm::translate(model, glm::vec3(0.0f, 0.75f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.8f, 1.5f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.4f, 0.4f, 0.4f)));
		meshList[0]->RenderMesh();

		// ==========================================
		// SECCIÓN: CABEZA (Hija del Cuello)
		// ==========================================
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.75f, 0.5f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(2.5f, 2.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.7f, 0.7f, 0.7f)));
		meshList[0]->RenderMesh();

		// --- NARIZ (Hija de la Cabeza) ---
		model = modelaux;
		// Z: 1.5f para que esté al frente.
		// Y: 0.7f para que el borde superior (escala 0.6 / 2 = 0.3) llegue al 1.0 de la cabeza.
		model = glm::translate(model, glm::vec3(0.0f, 0.7f, 1.5f));
		model = glm::scale(model, glm::vec3(0.8f, 0.6f, 0.4f)); // Un prisma pequeño
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.15f, 0.15f, 0.15f))); // Gris casi negro
		meshList[0]->RenderMesh();
		
		// OJOS (Hijo de la Cabeza)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.2f, -0.5f));
		modelaux = model;

		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.7f, 0.7f, 0.7f)));
		meshList[0]->RenderMesh();

		// OREJA IZQUIERDA
		model = modelaux;
		// X: 0.6f (separación), Y: 0.8f (más abajo), Z: 1.2f (pegadas a los ojos)
		model = glm::translate(model, glm::vec3(0.6f, 0.6f, 0.0f));
		model = glm::scale(model, glm::vec3(0.20f, 0.8f, 0.20f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.3f, 0.3f, 0.3f)));
		meshList[3]->RenderMeshGeometry();

		// OREJA DERECHA
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.6f, 0.6f, 0.0f));
		model = glm::scale(model, glm::vec3(0.20f, 0.8f, 0.20f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.3f, 0.3f, 0.3f)));
		meshList[3]->RenderMeshGeometry();

		// BLOQUE VERDE INCRUSTADO
		model = modelaux;
		// Lo adelantamos en Z (0.2f) para que salga del bloque gris y sea visible
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.2f));
		// Lo hacemos un poco más pequeño que el gris para que parezca un marco
		model = glm::scale(model, glm::vec3(1.8f, 0.8f, 1.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
		meshList[0]->RenderMesh();

		// ==========================================
		// SECCIÓN: PATA FRONTAL IZQUIERDA
		// ==========================================
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
		model = glm::translate(model, glm::vec3(2.0f, -0.5f, 2.5f));
		modelaux = model;

		// ARTICULACIÓN 1 (Hombro)
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
		sp.render();

		// BRAZO 1
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.2f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(45.0f + mainWindow.getarticulacion1()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.4f, 1.5f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.7f, 0.7f, 0.7f)));
		meshList[0]->RenderMesh();

		// ARTICULACIÓN 2 (Codo)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -0.75f, 0.0f));
		modelaux = model;
		model = glm::rotate(model, glm::radians(-90.0f + mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
		sp.render();

		// ANTEBRAZO 1
		model = modelaux; // Este modelaux viene de la Articulación 2 (Codo)
		model = glm::rotate(model, glm::radians(-90.0f + mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::translate(model, glm::vec3(0.0f, -0.75f, 0.0f)); // Bajamos para centrar el prisma
		model = glm::scale(model, glm::vec3(0.4f, 1.5f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.7f, 0.7f, 0.7f)));
		meshList[0]->RenderMesh();

		// PIE 1
		model = modelaux; // Ahora este modelaux YA incluye la rotación de la rodilla
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.2f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.3f, 0.3f, 0.3f)));
		meshList[0]->RenderMesh();

		// ==========================================
		// SECCIÓN: PATA FRONTAL DERECHA
		// ==========================================
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
		// X negativa para el lado derecho, misma altura Y y posición Z
		model = glm::translate(model, glm::vec3(-2.0f, -0.5f, 2.5f));
		modelaux = model;

		// ARTICULACIÓN 3 (Hombro Derecho)
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
		sp.render();

		// BRAZO 2
		model = modelaux;
		// X negativa (-0.2f) para que el brazo se pegue al lado izquierdo del hombro (hacia el cuerpo)
		model = glm::translate(model, glm::vec3(-0.2f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(45.0f + mainWindow.getarticulacion3()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
		modelaux = model;

		model = glm::scale(model, glm::vec3(0.4f, 1.5f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.7f, 0.7f, 0.7f)));
		meshList[0]->RenderMesh();

		// ARTICULACIÓN 4 (Codo Derecho)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -0.75f, 0.0f));
		modelaux = model;

		model = glm::rotate(model, glm::radians(-90.0f + mainWindow.getarticulacion4()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
		sp.render();

		// ANTEBRAZO 2
		model = modelaux;
		model = glm::rotate(model, glm::radians(-90.0f + mainWindow.getarticulacion4()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model; // Guardamos para el pie

		model = glm::translate(model, glm::vec3(0.0f, -0.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 1.5f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.7f, 0.7f, 0.7f)));
		meshList[0]->RenderMesh();

		// PIE 2
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.2f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.3f, 0.3f, 0.3f)));
		meshList[0]->RenderMesh();

		// ==========================================
		// SECCIÓN: PATA TRASERA IZQUIERDA
		// ==========================================
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
		// Posición: Izquierda (X=2.0), Abajo (Y=-0.5), Atrás (Z=-2.5)
		model = glm::translate(model, glm::vec3(2.0f, -0.5f, -2.5f));
		modelaux = model;

		// ARTICULACIÓN 5 (Cadera Trasera Izquierda)
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
		sp.render();

		// BRAZO (Muslo)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.2f, 0.0f, 0.0f)); // Pegado al cuerpo
		model = glm::rotate(model, glm::radians(45.0f + mainWindow.getarticulacion5()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
		modelaux = model;

		model = glm::scale(model, glm::vec3(0.4f, 1.5f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.7f, 0.7f, 0.7f)));
		meshList[0]->RenderMesh();

		// ARTICULACIÓN 6 (Rodilla Trasera Izquierda)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -0.75f, 0.0f));
		modelaux = model;

		model = glm::rotate(model, glm::radians(-90.0f + mainWindow.getarticulacion6()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
		sp.render();

		// ANTEBRAZO (Pantorrilla)
		model = modelaux;
		model = glm::rotate(model, glm::radians(-90.0f + mainWindow.getarticulacion6()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;

		model = glm::translate(model, glm::vec3(0.0f, -0.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 1.5f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.7f, 0.7f, 0.7f)));
		meshList[0]->RenderMesh();

		// PIE TRASERO
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.2f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.3f, 0.3f, 0.3f)));
		meshList[0]->RenderMesh();

		// ==========================================
		// SECCIÓN: PATA TRASERA DERECHA
		// ==========================================
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
		// Espejo: X negativa, mismo nivel que la trasera izquierda
		model = glm::translate(model, glm::vec3(-2.0f, -0.5f, -2.5f));
		modelaux = model;

		// ARTICULACIÓN 7 (Cadera Trasera Derecha)
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
		sp.render();

		// BRAZO (Muslo Derecho)
		model = modelaux;
		// X negativa (-0.2f) para que pegue al cuerpo desde la derecha
		model = glm::translate(model, glm::vec3(-0.2f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(45.0f + mainWindow.getarticulacion7()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
		modelaux = model;

		model = glm::scale(model, glm::vec3(0.4f, 1.5f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.7f, 0.7f, 0.7f)));
		meshList[0]->RenderMesh();

		// ARTICULACIÓN 8 (Rodilla Trasera Derecha)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -0.75f, 0.0f));
		modelaux = model;

		model = glm::rotate(model, glm::radians(-90.0f + mainWindow.getarticulacion8()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
		sp.render();

		// ANTEBRAZO (Pantorrilla Derecha)
		model = modelaux;
		model = glm::rotate(model, glm::radians(-90.0f + mainWindow.getarticulacion8()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;

		model = glm::translate(model, glm::vec3(0.0f, -0.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 1.5f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.7f, 0.7f, 0.7f)));
		meshList[0]->RenderMesh();

		// PIE TRASERO DERECHO
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.2f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.3f, 0.3f, 0.3f)));
		meshList[0]->RenderMesh();

		// ==========================================
		// SECCIÓN: COLA ARTICULADA (Postura Inicial Curva)
		// ==========================================
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -3.0f));
		modelaux = model;

		// --- ARTICULACIÓN 9 (Base - Levantada 45 grados) ---
		// Agregamos 45.0f fijos para que la cola nazca hacia arriba
		model = glm::rotate(model, glm::radians(45.0f + mainWindow.getarticulacion9()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
		sp.render();

		// --- COLA SECCIÓN 1 ---
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.6f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.4f, 0.4f, 0.4f)));
		meshList[0]->RenderMesh();

		// --- ARTICULACIÓN 10 (Mitad - Curva adicional de 30 grados) ---
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.6f));
		// Agregamos 30.0f fijos para acentuar la curva de la punta
		model = glm::rotate(model, glm::radians(30.0f + mainWindow.getarticulacion10()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
		sp.render();

		// --- COLA SECCIÓN 2 ---
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.6f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.4f, 0.4f, 0.4f)));
		meshList[0]->RenderMesh();


		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}