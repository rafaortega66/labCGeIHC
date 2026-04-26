/*
Práctica 7: Iluminación 1
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
Model Coche_M;
Model Cofre_M;
Model RinIzq_M;
Model RinDer_M;
Model Caucho_M;
Model Parrilla_M;
Model Lampara_M;
Model Nave_M;
Model PezCuerpo_M;
Model PezAntena_M;
Model PezFoco_M;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture rinTexture;
Texture cauchoTexture;
Texture lamparaTexture;
Texture dadoOchoTexture;
Texture naveTexture;
Texture aguaTexture;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};

	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.26f,  0.34f,		0.0f,	0.0f,	1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.49f,	0.34f,		0.0f,	0.0f,	1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.49f,	0.66f,		0.0f,	0.0f,	1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.26f,	0.66f,		0.0f,	0.0f,	1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.0f,  0.0f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	1.0f,	0.0f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	1.0f,	1.0f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.0f,	1.0f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		1.0f,	0.0f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		1.0f,	1.0f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,	1.0f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,  0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	1.0f,	0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f,	1.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	1.0f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	1.0f,	0.0f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,	0.0f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}

void CrearDecaedro()//funcion que crea el dado de ocho caras
{
	unsigned int cubo_indices[] = {
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		12, 13, 14,
		15, 16, 17,
		18, 19, 20,
		21, 22, 23,
	};

	GLfloat cubo_vertices[] = {
		// front // alegria
		//x		y		z			S		T			NX		NY		NZ
		-0.5f,	0.0f,	0.5f,		0.01f,	0.38f,		0.0f, 0.0f, 0.0f,// C A F
		0.5f,	0.0f,	0.5f,		0.02f,	0.02f,		0.0f, 0.0f, 0.0f,
		0.0f,	1.0f,	0.0f,		0.32f,	0.20f,		0.0f, 0.0f, 0.0f,

		0.5f,	0.0f,	0.5f,		0.33f,	0.59f,		0.0f, 0.0f, 0.0f,// E C F
		0.5f,	0.0f,	-0.5f,		0.01f,	0.40f,		0.0f, 0.0f, 0.0f,
		0.0f,	1.0f,	0.0f,		0.33f,	0.21f,		0.0f, 0.0f, 0.0f,

		0.5f,	0.0f,	-0.5f,		0.66f,	0.40f,		0.0f, 0.0f, 0.0f,// H E F
		-0.5f,	0.0f,	-0.5f,		0.34f,	0.58f,		0.0f, 0.0f, 0.0f,
		0.0f,	1.0f,	0.0f,		0.34f,	0.21f,		0.0f, 0.0f, 0.0f,

		-0.5f,	0.0f,	-0.5f,		0.66f,	0.01f,		0.0f, 0.0f, 0.0f,// I H F
		-0.5f,	0.0f,	0.5f,		0.66f,	0.39f,		0.0f, 0.0f, 0.0f,
		0.0f,	1.0f,	0.0f,		0.34f,	0.20f,		0.0f, 0.0f, 0.0f,

		-0.5f,	0.0f,	0.5f,		0.34f,	0.61f,		0.0f, 0.0f, 0.0f,// E D G
		0.5f,	0.0f,	0.5f,		0.34f,	0.99f,		0.0f, 0.0f, 0.0f,
		0.0f,	-1.0f,	0.0f,		0.66f,	0.80f,		0.0f, 0.0f, 0.0f,

		0.5f,	0.0f,	0.5f,		0.66f,	0.41f,		0.0f, 0.0f, 0.0f,// H E G 
		0.5f,	0.0f,	-0.5f,		0.35f,	0.60f,		0.0f, 0.0f, 0.0f,
		0.0f,	-1.0f,	0.0f,		0.66f,	0.78f,		0.0f, 0.0f, 0.0f,

		0.5f,	0.0f,	-0.5f,		0.98f,	0.60f,		0.0f, 0.0f, 0.0f,//K H G
		-0.5f,	0.0f,	-0.5f,		0.68f,	0.42f,		0.0f, 0.0f, 0.0f,
		0.0f,	-1.0f,	0.0f,		0.68f,	0.78f,		0.0f, 0.0f, 0.0f,

		-0.5f,	0.0f,	-0.5f,		0.99f,	0.98f,		0.0f, 0.0f, 0.0f,//J K G
		-0.5f,	0.0f,	0.5f,		0.99f,	0.62f,		0.0f, 0.0f, 0.0f,
		0.0f,	-1.0f,	0.0f,		0.68f,	0.80f,		0.0f, 0.0f, 0.0f,

	};

	calcAverageNormals(cubo_indices, 24, cubo_vertices, 192, 8, 5);

	// Invertir normales de la pirámide superior para que apunten afuera
	for (int i = 0; i < 12; i++) {
		unsigned int nOffset = i * 8 + 5; // 8 datos por fila, normal empieza en 5
		cubo_vertices[nOffset] *= -1.0f; // NX
		cubo_vertices[nOffset + 1] *= -1.0f; // NY
		cubo_vertices[nOffset + 2] *= -1.0f; // NZ
	}
	


	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 24);
	meshList.push_back(dado);

}

void CreaPecera()
{
	unsigned int indicesLaterales[] = {
		 0,  1,  2,   2,  3,  0,
		 4,  5,  6,   6,  7,  4,
		 8,  9, 10,  10, 11,  8,
		12, 13, 14,  14, 15, 12,
	};

	GLfloat datosLaterales[] = {
		-1.0f, -1.0f,  1.0f,   0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,   1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,   1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,   0.0f, 1.0f,  0.0f,  0.0f, -1.0f,

		 1.0f, -1.0f, -1.0f,   0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,   1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,   1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,   0.0f, 1.0f,  0.0f,  0.0f,  1.0f,

		 1.0f, -1.0f,  1.0f,   0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		 1.0f, -1.0f, -1.0f,   1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		 1.0f,  1.0f, -1.0f,   1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		 1.0f,  1.0f,  1.0f,   0.0f, 1.0f, -1.0f,  0.0f,  0.0f,

		-1.0f, -1.0f, -1.0f,   0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f,  1.0f,   1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		-1.0f,  1.0f,  1.0f,   1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		-1.0f,  1.0f, -1.0f,   0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	};

	unsigned int idxBase[] = { 0, 1, 2,  2, 3, 0 };
	GLfloat vtxBase[] = {
		-1.0f, -1.0f, -1.0f,   0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		 1.0f, -1.0f, -1.0f,   1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		 1.0f, -1.0f,  1.0f,   1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		-1.0f, -1.0f,  1.0f,   0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
	};

	unsigned int idxCapa[] = { 0, 1, 2,  2, 3, 0 };
	GLfloat vtxCapa[] = {
		-1.0f,  1.0f, -1.0f,   0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		-1.0f,  1.0f,  1.0f,   0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		 1.0f,  1.0f,  1.0f,   1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		 1.0f,  1.0f, -1.0f,   1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	};

	Mesh* estructuraLados = new Mesh();
	estructuraLados->CreateMesh(datosLaterales, indicesLaterales, 128, 24);
	meshList.push_back(estructuraLados);

	Mesh* mallaSuelo = new Mesh();
	mallaSuelo->CreateMesh(vtxBase, idxBase, 32, 6);
	meshList.push_back(mallaSuelo);

	Mesh* mallaTecho = new Mesh();
	mallaTecho->CreateMesh(vtxCapa, idxCapa, 32, 6);
	meshList.push_back(mallaTecho);
}




int main()
{
	//mainWindow = Window(512, 512); // 1280, 1024 or 1024, 768
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CrearDado();
	CreateShaders();
	CrearDecaedro();
	CreaPecera();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	Coche_M = Model();
	Coche_M.LoadModel("Models/coche.fbx");
	Cofre_M = Model();
	Cofre_M.LoadModel("Models/cofre.fbx");
	RinIzq_M = Model();
	RinIzq_M.LoadModel("Models/rinIzq.obj");
	RinDer_M = Model();
	RinDer_M.LoadModel("Models/rinDer.obj");
	Caucho_M = Model();
	Caucho_M.LoadModel("Models/caucho.fbx");
	Parrilla_M = Model();
	Parrilla_M.LoadModel("Models/parrilla.obj");
	rinTexture = Texture("Textures/rin.tga");
	rinTexture.LoadTextureA();
	cauchoTexture = Texture("Textures/caucho.tga");
	cauchoTexture.LoadTextureA();
	Lampara_M = Model();
	Lampara_M.LoadModel("Models/lampara.obj");

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	//lamparaTexture = Texture("Textures/Lamp_Base_color.png");
	//lamparaTexture.LoadTextureA();
	dadoOchoTexture = Texture("Textures/dadoOcho.tga");
	dadoOchoTexture.LoadTextureA();
	//AGUA PARA PECERA
	aguaTexture = Texture("Textures/Agua.jpg");
	aguaTexture.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	//Blackhawk_M = Model();
	//Blackhawk_M.LoadModel("Models/uh60.obj");
	
	//NAVE
	Nave_M = Model();
	Nave_M.LoadModel("Models/nave.obj");
	naveTexture = Texture("Textures/Image_0.jpeg");
	naveTexture.LoadTextureA();

	//Pez
	PezCuerpo_M = Model();
	PezCuerpo_M.LoadModel("Models/cuerpo_pez.obj");
	PezAntena_M = Model();
	PezAntena_M.LoadModel("Models/antena_pez.obj");
	PezFoco_M = Model();
	PezFoco_M.LoadModel("Models/foco_pez.obj");



	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	//luz Fija para la lampara
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
		3.5f, 3.0f,			//luz ambiental y luz difusa
		12.0f, 1.0f, 3.0f,//los de posicion
		1.0f, 0.9f, 0.032f);//factores constante, lineal y exponencial
	//1.0f, 2.0f, 10.0f);previo
	pointLightCount++;

	//luz  para el pez
	pointLights[2] = PointLight(0.0f, 0.0f, 1.0f,
		3.5f, 3.0f,			//luz ambiental y luz difusa
		-15.0f, 0.0f, -6.0f,//los de posicion
		1.0f, 0.9f, 0.032f);//factores constante, lineal y exponencial
	//1.0f, 2.0f, 10.0f);previo
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//LUZ AZUL
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f, // rgb LUZ DE FARO
		1.0f, 10.0f,
		// aLight dLight 
		1.41f, 0.14f, 4.9f,  // posicion 
		0.0f, -0.10f, 1.0f, // dir 
		1.0f, 0.0f, 0.0f,  // con lin exp LUZ INFINITA
		//1.0f, 0.05f, 0.01f,  // con lin exp 
		25.0f);
	// edge location 
	spotLightCount++;

	//LUZ AMARILLA
	spotLights[2] = SpotLight(1.0f, 1.0f, 0.0f, // rgb LUZ DE HELICOPTERO
		1.0f, 10.0f,
		// aLight dLight 
		0.0f, 5.0f, 6.0f,  // posicion 
		0.0f, -1.0f, 0.0f, // dir 
		1.0f, 0.0f, 0.0f,  // con lin exp 
		25.0f);
	// edge location 
	spotLightCount++;

	//SPOTLIGHt PEZ
	spotLights[3] = SpotLight(0.0f, 1.0f, 0.0f, // rgb LUZ DE PEZ
		1.0f, 10.0f,
		// aLight dLight 
		-15.0f, 2.5f, -4.1f,  // posicion 
		0.0f, 0.0f, 1.0f, // dir 
		1.0f, 0.0f, 0.0f,  // con lin exp 
		25.0f);
	// edge location 
	spotLightCount++;

	/*//luz fija
	spotLights[2] = SpotLightSpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;*/

	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0, uniformAlpha = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	// Declaraciones (Se reserva memoria una sola vez)
	// --- SECCIÓN DE DECLARACIONES (FUERA DEL WHILE) ---
	glm::mat4 model(1.0f);
	glm::mat4 modelaux(1.0f);
	glm::mat4 modelaux2(1.0f);
	glm::mat4 matrizFaro(1.0f);      // Para jerarquía del faro azul
	glm::mat4 matrizAmarilla(1.0f);   // Para jerarquía del helicóptero
	glm::vec3 color(1.0f);
	glm::vec3 spotLightPos(0.0f);     // Posición genérica para cálculos
	glm::vec3 lowerLight(0.0f);       // Para la linterna de la cámara
	glm::vec3 posicionFaro(0.0f);     // Coordenadas mundiales del faro
	glm::vec3 direccionFaro(0.0f);    // Vector de dirección del faro
	glm::vec3 posicionAmarilla(0.0f); // Coordenadas mundiales de luz helicóptero
	glm::vec3 direccionAmarilla(0.0f);// Vector de dirección de luz helicóptero
	glm::vec3 posLuzLampara(0.0f);    // Para la jerarquía de la lámpara fija
	GLfloat now(0.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformAlpha = shaderList[0].GetAlphaLocation();


		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		//spotLights[1].SetPos(poscoche + glm::vec(x, y, cofre));

		shaderList[0].SetDirectionalLight(&mainLight);
		//cndicion para prender luz 
		if (mainWindow.getLampSw()) {
			pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
				3.5f, 3.0f,			//luz ambiental y luz difusa
				12.0f, 1.0f, 3.0f,//los de posicion
				1.0f, 0.9f, 0.032f);//factores constante, lineal y exponencial
		}
		else {
			//luz Fija para la lampara
			pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
				0.0f, 0.0f,			//luz ambiental y luz difusa
				12.0f, 1.0f, 3.0f,//los de posicion
				1.0f, 0.9f, 0.032f);//factores constante, lineal y exponencial
		}
		if (mainWindow.getPezSw()) {
			//luz  para el pez
			pointLights[2] = PointLight(0.0f, 0.0f, 1.0f,
				3.5f, 3.0f,			//luz ambiental y luz difusa
				-15.0f, 0.0f, -6.0f,//los de posicion
				1.0f, 0.9f, 0.032f);//factores constante, lineal y exponencial
			//1.0f, 2.0f, 10.0f);previo
		}
		else {
			//luz  para el pez
			pointLights[2] = PointLight(0.0f, 0.0f, 1.0f,
				0.0f, 0.0f,			//luz ambiental y luz difusa
				-15.0f, 0.0f, -6.0f,//los de posicion
				1.0f, 0.9f, 0.032f);//factores constante, lineal y exponencial
			//1.0f, 2.0f, 10.0f);previo
		}
		
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		// DENTRO DEL WHILE
		model = glm::mat4(1.0f);
		modelaux = glm::mat4(1.0f);
		modelaux2 = glm::mat4(1.0f);
		color = glm::vec3(1.0f, 1.0f, 1.0f);


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		// colores del faro
		switch (mainWindow.getContador()) {
		case 0: // rojo
			spotLights[1] = SpotLight(1.0f, 0.0f, 0.0f,
				10.0f, 10.0f, 0.95f, 0.8f, 2.4f, 0.0f, -0.10f, 1.0f, 1.0f, 0.09f, 0.032f, 25.0f);
			break;
		case 1: // amarillo
			spotLights[1] = SpotLight(1.0f, 1.0f, 0.0f,
				10.0f, 10.0f, 0.95f, 0.8f, 2.4f, 0.0f, -0.10f, 1.0f, 1.0f, 0.09f, 0.032f, 25.0f);
			break;
		case 2: // verde
			spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
				10.0f, 10.0f, 0.95f, 0.8f, 2.4f, 0.0f, -0.10f, 1.0f, 1.0f, 0.09f, 0.032f, 25.0f);
			break;
		case 3: // cyan
			spotLights[1] = SpotLight(0.0f, 1.0f, 1.0f,
				10.0f, 10.0f, 0.95f, 0.8f, 2.4f, 0.0f, -0.10f, 1.0f, 1.0f, 0.09f, 0.032f, 25.0f);
			break;
		case 4: // azul
			spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
				10.0f, 10.0f, 0.95f, 0.8f, 2.4f, 0.0f, -0.10f, 1.0f, 1.0f, 0.09f, 0.032f, 25.0f);
			break;
		case 5: // magenta
			spotLights[1] = SpotLight(1.0f, 0.0f, 1.0f,
				10.0f, 10.0f, 0.95f, 0.8f, 2.4f, 0.0f, -0.10f, 1.0f, 1.0f, 0.09f, 0.032f, 25.0f);
			break;
		default:
			// Opcional: manejar casos fuera de 0-5
			break;
		}

		//Instancia del coche 

		//Carro
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.4f, mainWindow.getmueveCoche()));
		modelaux = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Coche_M.RenderModel();

		//FARO luz del carro azul
		matrizFaro = model;
		matrizFaro = glm::translate(matrizFaro, glm::vec3(1.4f, -0.3f, 4.8f));

		posicionFaro = glm::vec3(matrizFaro[3]);
		direccionFaro = glm::vec3(matrizFaro * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));//tener en cuenta para animacion
		spotLights[1].SetFlash(posicionFaro, glm::normalize(direccionFaro));

		//Cofre
		//color = glm::vec3(0.0f, 1.0f, 0.0f);
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.35f, 2.05f));
		model = glm::rotate(model, -glm::radians(mainWindow.getmueveCofre()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cofre_M.RenderModel();

		//RinIzquierdo1
		//color = glm::vec3(0.0f, 0.0f, 0.0f);
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.5f, -0.8f, 3.85f));
		model = glm::rotate(model, -glm::radians(mainWindow.getmueveLlanta()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux2 = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rinTexture.UseTexture();
		RinIzq_M.RenderModel();

		//Caucho1
		//color = glm::vec3(1.0f, 1.0f, 0.0f);
		model = (modelaux2);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cauchoTexture.UseTexture();
		Caucho_M.RenderModel();

		//RinIzquierdo2
		//color = glm::vec3(0.0f, 0.0f, 0.0f);
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.5f, -0.8f, -1.9f));
		model = glm::rotate(model, -glm::radians(mainWindow.getmueveLlanta()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux2 = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rinTexture.UseTexture();
		RinIzq_M.RenderModel();

		//Caucho
		//color = glm::vec3(1.0f, 1.0f, 0.0f);
		model = (modelaux2);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cauchoTexture.UseTexture();
		Caucho_M.RenderModel();

		//RinDerecho1
		//color = glm::vec3(0.0f, 0.0f, 0.0f);
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.5f, -0.8f, 3.85f));
		model = glm::rotate(model, -glm::radians(mainWindow.getmueveLlanta()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux2 = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rinTexture.UseTexture();
		RinDer_M.RenderModel();

		//Caucho3
		//color = glm::vec3(1.0f, 1.0f, 0.0f);
		model = (modelaux2);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cauchoTexture.UseTexture();
		Caucho_M.RenderModel();

		//RinDerecho2
		//color = glm::vec3(0.0f, 0.0f, 0.0f);
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.5f, -0.8f, -1.9f));
		//model = glm::rotate(model, 90*toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -glm::radians(mainWindow.getmueveLlanta()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux2 = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rinTexture.UseTexture();
		RinDer_M.RenderModel();

		//Caucho4
		//color = glm::vec3(1.0f, 1.0f, 0.0f);
		model = (modelaux2);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cauchoTexture.UseTexture();
		Caucho_M.RenderModel();

		//Parrilla
		//color = glm::vec3(1.0f, 1.0f, 0.0f);
		model = (modelaux);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//cauchoTexture.UseTexture();
		Parrilla_M.RenderModel();

		//NAVE
		model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 5.0f, 6.0)); se cambio esta linea
		//model = glm::translate(model, glm::vec3(mainWindow.getmueveHelicoptero(), 0.4f, 0.0f));
		model = glm::translate(model, glm::vec3(mainWindow.getmueveHelicoptero(), 5.0f, 6.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		naveTexture.UseTexture();
		Nave_M.RenderModel();

		//SPOTLIGHT amarillo jerarquia
		matrizAmarilla = modelaux;
		matrizAmarilla = glm::translate(matrizAmarilla, glm::vec3(1.0f, 0.0f, 0.4f));

		posicionAmarilla = glm::vec3(matrizAmarilla[3]);
		
		float anguloLuz = 0.0f;
		if (mainWindow.getsKeys()[GLFW_KEY_H]){
			anguloLuz = -0.5F;
		}
		if (mainWindow.getsKeys()[GLFW_KEY_Y]) {
			anguloLuz = 0.5F;
		}
		direccionAmarilla = glm::vec3(matrizAmarilla * glm::vec4(anguloLuz, -1.0f, 0.0f, 0.0f));//tener en cuenta para animacion
		spotLights[2].SetFlash(posicionAmarilla, glm::normalize(direccionAmarilla));

		/*//Lampara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(12.0f, 0.0f, 3.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.00f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//lamparaTexture.UseTexture();
		Lampara_M.RenderModel();*/

		//Lampara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(12.0f, 0.0f, 3.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.00f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara_M.RenderModel();
		modelaux = model;
		posLuzLampara = glm::vec3(modelaux * glm::vec4(0.0f, 0.85f, 0.0f, 1.0f));
		pointLights[1].SetPos(posLuzLampara);

		//cubo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 5.0f, 6.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//meshList[4]->RenderMesh();

		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -8.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		//Dado De Ocho Caras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f, 3.0f, -2.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoOchoTexture.UseTexture();
		meshList[5]->RenderMesh();

		//Pez Cuerpo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 0.0f + mainWindow.getMuevePez(), -6.0f + mainWindow.getMuevePez()));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PezCuerpo_M.RenderModel();
		//Pez Antena
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));

		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.00f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PezAntena_M.RenderModel();
		//Pez Foco
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 10.0f, 6.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getMueveFocoX()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getMueveFocoY()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getMueveFocoZ()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PezFoco_M.RenderModel();
		spotLights[3].SetFlash(model[3], glm::normalize(glm::vec3(model * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f))));

		// Pecera
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 2.5f, -2.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		color = glm::vec3(0.0f, 0.0f, 0.0f); 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform1f(uniformAlpha, 1.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		plainTexture.UseTexture();
		meshList[7]->RenderMesh();

		// Tapa
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform1f(uniformAlpha, 0.5f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		aguaTexture.UseTexture();
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		meshList[8]->RenderMesh();
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glUniform1f(uniformAlpha, 1.0f);

		// Paredes
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform1f(uniformAlpha, 0.10f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		meshList[6]->RenderMesh(); 
		glDisable(GL_BLEND);
		glUniform1f(uniformAlpha, 1.0f);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshList[3]->RenderMesh();
		glDisable(GL_BLEND);
		
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
