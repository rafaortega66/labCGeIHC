/*
ORTEGA DE LA PAZ RAFAEL
420054085
Practica 9 ANIMIACION !:

- Simple o basica: Por banderas y condicionales (mas de 1 transformacion geometrica se ve modificada)
-Compleja: Por medio de funciones y algoritmos. 
-Textura Animada
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

//para iluminacion
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animacion
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;
// --- animacion pista ---
bool  animando   = false;
float car_pitch   = 0.0f;  // inclinacion en rampa (grados)
float car_heading = 0.0f;  // direccion en curvas  (grados)
float car_roll    = 0.0f;  // balanceo lateral en curvas (grados)
//_____________________________


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;
//
Texture rinTexture;
Texture cauchoTexture;
//Texture naveTexture;

//-----------------------------------------------------------
// Variables para el ciclo de 50 unidades
enum Estado { AVANZANDO, REGRESANDO };
Estado carro_estado = AVANZANDO;
float carro_x = 0.0f;       // Controla el desplazamiento
float carro_y = 0.0f;       // Controla el desplazamiento
float carro_z = 0.0f;       // Controla el desplazamiento
float angulo_x = 0.0f;       // Controla el desplazamiento
float angulo_y = 0.0f;       // Controla el desplazamiento
float angulo_z = 0.0f;       // Controla el desplazamiento
float llanta_rotacion = 0.0f; // Controla el giro de rines y cauchos
float speed = 0.1f;         // Velocidad constante
//-----------------------------------------------------------

//-----------------------------------------------------------
// Variables para el ciclo de 50 unidades
Estado nave_estado = AVANZANDO;
float nave_z       = -190.0f; // inicia en extremo: World X = -190
float nave_y       =  32.7f;  // Y pista en extremo (23.7) + 9 de altura de vuelo
float nave_world_z = -18.0f;  // World Z del extremo de la pista
bool subeNave = false;
bool abreAla = false;
float anguloAla = 0.0f;
bool animandoNave = true;     // no se puede repetir
float contadorNave = 0.0f;
float helice_rotacion = 0.0f;
//-----------------------------------------------------------//1.-despues de instanciar el modelo importar hago esto

Model Kitt_M;
Model Llanta_M;
Model Pista_M;
Model Nave_M;
Model Nave_base_M;
Model Ala_M;
Model Ala2_M;
Model Helice_M;
Model Aeolipile_base_M;
Model Aeolipile_M;
//
Model Coche_M;
Model Cofre_M;
Model RinIzq_M;
Model RinDer_M;
Model Caucho_M;
Model Parrilla_M;

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



//calculo del promedio de las normales para sombreado de Phong
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
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6); // todos los numeros

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7); // solo un numero

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	// Pos (10, 3, 3): detras del inicio; yaw=180 mira hacia -X donde va el carro
	camera = Camera(glm::vec3(10.0f, 3.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, -10.0f, 0.5f, 0.5f);

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
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA(); 
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();
	//
	rinTexture = Texture("Textures/rin.tga");
	rinTexture.LoadTextureA();
	cauchoTexture = Texture("Textures/caucho.tga");
	cauchoTexture.LoadTextureA();
	//NAVE

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Pista_M = Model();
	Pista_M.LoadModel("Models/pista.obj");
	Nave_base_M = Model();
	Nave_base_M.LoadModel("Models/nave_base.obj");
	Ala_M = Model();
	Ala_M.LoadModel("Models/ala.obj");
	Ala2_M = Model();
	Ala2_M.LoadModel("Models/ala2.obj");
	Helice_M = Model();
	Helice_M.LoadModel("Models/helice.obj");
	Aeolipile_base_M = Model();
	Aeolipile_base_M.LoadModel("Models/Aeolipile_base.obj");
	Aeolipile_M = Model();
	Aeolipile_M.LoadModel("Models/Aeolipile.obj");
	//
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


	//luz direccional, solo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.5f, 0.5f,
		0.0f, -1.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaraci�n de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
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

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
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




	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	movCoche = 0.0f;
	movOffset = 0.01f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;
	animando = true;

	glm::vec3 lowerLight(0.0f,0.0f,0.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);


	// Declaraciones (Se reserva memoria una sola vez)
	// --- SECCIoN DE DECLARACIONES (FUERA DEL WHILE) ---
	//glm::mat4 model(1.0f);
	//glm::mat4 modelaux(1.0f);
	glm::mat4 modelaux2(1.0f);
	glm::mat4 matrizFaro(1.0f);      // Para jerarqu�a del faro azul
	glm::mat4 matrizAmarilla(1.0f);   // Para jerarqu�a del helic�ptero
	glm::mat4 modelNave(1.0f);        // Para jerarqu�a interna de la nave
	//glm::vec3 color(1.0f);
	glm::vec3 spotLightPos(0.0f);     // Posici�n gen�rica para c�lculos
	//glm::vec3 lowerLight(0.0f);       // Para la linterna de la c�mara
	glm::vec3 posicionFaro(0.0f);     // Coordenadas mundiales del faro
	glm::vec3 direccionFaro(0.0f);    // Vector de direcci�n del faro
	glm::vec3 posicionAmarilla(0.0f); // Coordenadas mundiales de luz helic�ptero
	glm::vec3 direccionAmarilla(0.0f);// Vector de direcci�n de luz helic�ptero
	glm::vec3 posLuzLampara(0.0f);    // Para la jerarqu�a de la l�mpara fija
	GLfloat now(0.0f);
	//_____________________________________________________________________________
	float contador = 0.0f;


	////Loop mientras no se cierra la ventana
	lastTime = glfwGetTime();
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		contador += deltaTime;
		
		// --- Tecla P: reiniciar animaci�n ---
		{
			static bool pPrev = false;
			bool pNow = mainWindow.getsKeys()[GLFW_KEY_P];
			if (pNow && !pPrev) {
				// reinicia carro
				carro_x = 0.0f; carro_y = 0.0f; carro_z = 0.0f;
				car_pitch = 0.0f; car_heading = 0.0f; car_roll = 0.0f;
				llanta_rotacion = 0.0f;
				contador = 0.0f;
				animando = true;
				// reinicia nave
				/*nave_z = -190.0f;
				nave_y       =  32.7f;
				nave_world_z = -18.0f;
				contadorNave = 0.0f;
				animandoNave = true;
				anguloAla    = 0.0f;
				abreAla      = false;
				helice_rotacion = 0.0f;*/
			}
			pPrev = pNow;
		}

		// --- Animacion simple por fases ---
		if (animando) {
			llanta_rotacion += 5.0f * deltaTime;

			// F1 (0-50): inicio, posicion sobre pista
			if (contador < 50) {
				carro_z     += 0.15f   * deltaTime;
				carro_x     += 0.0058f * deltaTime;
				car_heading += 0.0530f * deltaTime;
				car_roll    += 0.0145f * deltaTime;
			}
			// F2 (50-113): tramo recto, leve curva derecha
			else if (contador < 113) {
				carro_z     += 0.15f   * deltaTime;
				carro_x     += 0.0120f * deltaTime;
				car_heading += 0.0377f * deltaTime;
				car_pitch   += 0.0021f * deltaTime;
				car_roll    -= 0.0029f * deltaTime;
			}
			// F3 (113-175): curva derecha, Z sube a 1.87
			else if (contador < 175) {
				carro_z     += 0.15f   * deltaTime;
				carro_x     += 0.0279f * deltaTime;
				carro_y     += 0.0008f * deltaTime;
				car_heading += 0.0955f * deltaTime;
				car_pitch   += 0.0046f * deltaTime;
				car_roll    += 0.0108f * deltaTime;
			}
			// F4 (175-259): sube al apex real, sin pasar por buckets con ruido
			else if (contador < 259) {
				carro_z     += 0.15f   * deltaTime;
				carro_x     += 0.0547f * deltaTime;
				carro_y     += 0.0034f * deltaTime;
				car_heading += 0.1101f * deltaTime;
				car_pitch   += 0.0086f * deltaTime;
				car_roll    += 0.0010f * deltaTime;
			}
			// F5 (259-322): apex, heading inicia reversion de S-curva
			else if (contador < 322) {
				carro_z     += 0.15f   * deltaTime;
				carro_x     -= 0.0211f * deltaTime;
				carro_y     += 0.0124f * deltaTime;
				car_heading -= 0.4476f * deltaTime;
				car_pitch   += 0.0570f * deltaTime;
				car_roll    -= 0.1044f * deltaTime;
			}
			// F6 (322-384): S-curva izquierda, Z cae rapidamente
			else if (contador < 384) {
				carro_z     += 0.15f   * deltaTime;
				carro_x     -= 0.0974f * deltaTime;
				carro_y     += 0.0312f * deltaTime;
				car_heading -= 0.3984f * deltaTime;
				car_pitch   += 0.1115f * deltaTime;
				car_roll    += 0.0094f * deltaTime;
			}
			// F7 (384-447): S-curva continua, carro cruza al lado izquierdo
			else if (contador < 447) {
				carro_z     += 0.15f   * deltaTime;
				carro_x     -= 0.1382f * deltaTime;
				carro_y     += 0.0404f * deltaTime;
				car_heading -= 0.1603f * deltaTime;
				car_pitch   += 0.0557f * deltaTime;
				car_roll    += 0.0446f * deltaTime;
			}
			// F8 (447-510): carro se endereza, entra a rampa
			else if (contador < 510) {
				carro_z     += 0.15f   * deltaTime;
				carro_x     -= 0.0286f * deltaTime;
				carro_y     += 0.0535f * deltaTime;
				car_heading += 0.5079f * deltaTime;
				car_pitch   += 0.0727f * deltaTime;
				car_roll    += 0.1252f * deltaTime;
			}
			// F9 (510-572): rampa, sigue girando a derecha
			else if (contador < 572) {
				carro_z     += 0.15f   * deltaTime;
				carro_x     += 0.0400f * deltaTime;
				carro_y     += 0.0637f * deltaTime;
				car_heading += 0.4121f * deltaTime;
				car_pitch   += 0.0484f * deltaTime;
				car_roll    -= 0.0845f * deltaTime;
			}
			// F10 (572-614): rampa fuerte, pendiente maxima
			else if (contador < 614) {
				carro_z     += 0.15f   * deltaTime;
				carro_x     += 0.0269f * deltaTime;
				carro_y     += 0.0767f * deltaTime;
				car_heading -= 0.1060f * deltaTime;
				car_pitch   += 0.1064f * deltaTime;
			}
			// F11 (614-676): rampa alta, empieza a aplanarse
			else if (contador < 676) {
				carro_z     += 0.15f   * deltaTime;
				carro_x     += 0.0080f * deltaTime;
				carro_y     += 0.0694f * deltaTime;
				car_heading -= 0.1836f * deltaTime;
				car_pitch   -= 0.0189f * deltaTime;
				car_roll    -= 0.0797f * deltaTime;
			}
			// F12 (676-739): zona alta, pendiente baja moderadamente
			else if (contador < 739) {
				carro_z     += 0.15f   * deltaTime;
				carro_x     += 0.0108f * deltaTime;
				carro_y     += 0.0569f * deltaTime;
				car_heading += 0.0170f * deltaTime;
				car_pitch   -= 0.0649f * deltaTime;
				car_roll    += 0.0367f * deltaTime;
			}
			// F13 (739-802): plateau, pendiente cae fuerte
			else if (contador < 802) {
				carro_z     += 0.15f   * deltaTime;
				carro_x     -= 0.0297f * deltaTime;
				carro_y     += 0.0387f * deltaTime;
				car_heading -= 0.2445f * deltaTime;
				car_pitch   -= 0.1008f * deltaTime;
				car_roll    -= 0.0478f * deltaTime;
			}
			// F14 (802-864): descenso de pendiente continua
			else if (contador < 864) {
				carro_z     += 0.15f   * deltaTime;
				carro_x     -= 0.0415f * deltaTime;
				carro_y     += 0.0212f * deltaTime;
				car_heading -= 0.0681f * deltaTime;
				car_pitch   -= 0.1024f * deltaTime;
				car_roll    += 0.0327f * deltaTime;
			}
			// F15 (864-927): pendiente se acerca a cero
			else if (contador < 927) {
				carro_z     += 0.15f   * deltaTime;
				carro_x     -= 0.0569f * deltaTime;
				carro_y     += 0.0081f * deltaTime;
				car_heading -= 0.0849f * deltaTime;
				car_pitch   -= 0.0793f * deltaTime;
				car_roll    -= 0.0032f * deltaTime;
			}
			// F16 (927-990): altura maxima, pitch cruza 0
			else if (contador < 990) {
				carro_z     += 0.15f   * deltaTime;
				carro_x     -= 0.0376f * deltaTime;
				carro_y     -= 0.0020f * deltaTime;
				car_heading += 0.1069f * deltaTime;
				car_pitch   -= 0.0613f * deltaTime;
				car_roll    += 0.0351f * deltaTime;
			}
			// F17 (990-1052): descenso suave
			else if (contador < 1052) {
				carro_z     += 0.15f   * deltaTime;
				carro_x     -= 0.0507f * deltaTime;
				carro_y     -= 0.0098f * deltaTime;
				car_heading -= 0.0736f * deltaTime;
				car_pitch   -= 0.0478f * deltaTime;
				car_roll    -= 0.0335f * deltaTime;
			}
			// F18 (1052-1115): descenso continuo
			else if (contador < 1115) {
				carro_z     += 0.15f   * deltaTime;
				carro_x     -= 0.0502f * deltaTime;
				carro_y     -= 0.0169f * deltaTime;
				car_heading += 0.0029f * deltaTime;
				car_pitch   -= 0.0425f * deltaTime;
				car_roll    += 0.0140f * deltaTime;
			}
			// F19 (1115-1177): tramo final hasta punto extremo
			else if (contador < 1177) {
				carro_z     += 0.15f   * deltaTime;
				carro_x     -= 0.0138f * deltaTime;
				carro_y     -= 0.0195f * deltaTime;
				car_heading += 0.2118f * deltaTime;
				car_pitch   -= 0.0158f * deltaTime;
				car_roll    += 0.0389f * deltaTime;
			}
			else {
				animando = false;
			}
		}

		//_____________________________________________________________________________
		// Animacion nave - no se puede repetir
		// 7 fases )
		if (animandoNave) {
			helice_rotacion += 300.0f * deltaTime;
			contadorNave    += deltaTime;

			// Fase 1 (0-245): zona alta, Y sube levemente, Z hacia el centro
			// X: -190->-150  Y: 32.7->35.5  Z: -18->-10.3
			if (contadorNave < 245) {
				nave_z       += 0.163f  * deltaTime;
				nave_y       += 0.0114f * deltaTime;
				nave_world_z += 0.0314f * deltaTime;
			}
			// Fase 2 (245-490): inicio de rampa, Y empieza a bajar
			// X: -150->-110  Y: 35.5->28.4  Z: -10.3->-1.75
			else if (contadorNave < 490) {
				nave_z       += 0.163f  * deltaTime;
				nave_y       -= 0.0290f * deltaTime;
				nave_world_z += 0.0349f * deltaTime;
			}
			// Fase 3 (490-674): rampa fuerte, Y cae rapidamente, Z se desplaza
			// X: -110->-80  Y: 28.4->15.3  Z: -1.75->-6.7
			else if (contadorNave < 674) {
				nave_z       += 0.163f  * deltaTime;
				nave_y       -= 0.0712f * deltaTime;
				nave_world_z -= 0.0269f * deltaTime;
			}
			// Fase 4 (674-809): CURVA EN S - gran swing lateral +14.6 en Z
			// X: -80->-58  Y: 15.3->8.7  Z: -6.7->+7.9
			else if (contadorNave < 809) {
				nave_z       += 0.163f  * deltaTime;
				nave_y       -= 0.0489f * deltaTime;
				nave_world_z += 0.1081f * deltaTime;
			}
			// Fase 5 (809-901): Z llega al maximo lateral
			// X: -58->-43  Y: 8.7->7.4  Z: +7.9->+9.4
			else if (contadorNave < 901) {
				nave_z       += 0.163f  * deltaTime;
				nave_y       -= 0.0141f * deltaTime;
				nave_world_z += 0.0163f * deltaTime;
			}
			// Fase 6 (901-1042): curva de regreso, Z vuelve al centro
			// X: -43->-20  Y: ~7.2 plano  Z: +9.4->+3.9
			else if (contadorNave < 1042) {
				nave_z       += 0.163f  * deltaTime;
				nave_world_z -= 0.0390f * deltaTime;
			}
			// Fase 7 (1042-1165): descenso final y aterrizaje junto al punto inicial
			// X: -20->0  Y: 7.2->0  Z: +3.9->+3.3
			else if (contadorNave < 1165) {
				nave_z       += 0.163f  * deltaTime;
				nave_y       -= 0.0585f * deltaTime;
				nave_world_z -= 0.0049f * deltaTime;
			}
			else {
				animandoNave = false; // aterrizo junto al punto inicial
			}

			// Aleteo activo mientras vuela
			if (abreAla == true) {
				if (anguloAla < 45) {
					anguloAla += deltaTime * speed * 40;
				}
				else abreAla = false;
			}
			else {
				if (anguloAla >= 0.0) {
					anguloAla -= deltaTime * speed * 40;
				}
				else abreAla = true;
			}
		}
		//_____________________________________________________________________________

		angulovaria += 0.5f*deltaTime;
		//Como haces para que el carro no se salga del piso
			movCoche -= movOffset * deltaTime;
			rotllanta += rotllantaOffset * deltaTime;

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
		uniformTextureOffset = shaderList[0].getOffsetLocation(); // para la textura con movimiento

		//informacion en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la c�mara de tipo flash
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//informacion al shader de fuentes de iluminaci�n
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		//Reinicializando variables cada ciclo de reloj
		model= glm::mat4(1.0);
		modelaux= glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 2.0f));//////////////////////////////////////
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();////////////////////////////
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		//Pista
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.1f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pista_M.RenderModel();

		//Instancia del coche
		//Carro
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::mat4(1.0);
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(3.0f + carro_x, -0.4f + carro_y, 8.0f + carro_z));
		model = glm::rotate(model, car_heading * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // giro en curvas
		model = glm::rotate(model, car_roll    * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); // banqueo lateral
		model = glm::rotate(model, -car_pitch  * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); // inclinaci�n: - = nariz arriba
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
		model = glm::rotate(model, angulo_x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, angulo_y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, angulo_z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cofre_M.RenderModel();

		//RinIzquierdo1
		//color = glm::vec3(0.0f, 0.0f, 0.0f);
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.5f, -0.8f, 3.85f));
		model = glm::rotate(model, -glm::radians(mainWindow.getmueveLlanta()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, llanta_rotacion * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));//////
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
		model = glm::rotate(model, llanta_rotacion * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));//////
		modelaux2 = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rinTexture.UseTexture();
		RinIzq_M.RenderModel();

		//Caucho 2
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
		model = glm::rotate(model, llanta_rotacion * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));//////
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
		model = glm::rotate(model, llanta_rotacion * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));//////
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
		

		//NAVE - posicion base en el mundo
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(nave_z, nave_y, nave_world_z));
		modelaux = model; // guarda antes de escala para jerarquia de luz amarilla

		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		modelNave = model; // espacio compartido de todas las piezas

		// Nave base (fuselaje principal)
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelNave));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Nave_base_M.RenderModel();

		// Ala izquierda - con aleteo
		model = modelNave;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.3f));
		model = glm::rotate(model, anguloAla * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ala_M.RenderModel();

		// Ala derecha - aleteo opuesto para efecto simetrico
		model = modelNave;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.3f));
		model = glm::rotate(model, -anguloAla * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ala2_M.RenderModel();

		// Helice 1 - giro continuo
		model = modelNave;
		model = glm::translate(model, glm::vec3(0.94f, 0.0f, 0.0f));
		model = glm::rotate(model, helice_rotacion * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helice_M.RenderModel();

		// Helice 2 - desfasada 90 grados para verse como una sola pieza
		model = modelNave;
		model = glm::translate(model, glm::vec3(0.97f, 0.0f, 0.0f));
		model = glm::rotate(model, (helice_rotacion + 90.0f) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helice_M.RenderModel();

		// Spotlight amarillo - jerarquia ligada a la nave
		matrizAmarilla = modelaux;
		matrizAmarilla = glm::translate(matrizAmarilla, glm::vec3(0.0f, 0.0f, 0.0f));
		posicionAmarilla = glm::vec3(matrizAmarilla[3]);
		direccionAmarilla = glm::vec3(matrizAmarilla * glm::vec4(0.0f, -1.0f, 0.0f, 0.0f));
		spotLights[2].SetFlash(posicionAmarilla, glm::normalize(direccionAmarilla));

		
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
