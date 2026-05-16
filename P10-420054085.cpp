/*
Animaci�n por Keyframes
*/
#define _CRT_SECURE_NO_WARNINGS
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <windows.h>
#include <Xinput.h>
#pragma comment(lib, "Xinput.lib")

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

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animaci�n
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
float anguloAla = 0.0f;
bool abreAla = false;
bool grabando = false;
FILE* archivoKF = nullptr;

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



Model Kitt_M;
Model Llanta_M;
Model Pista_M;
Model Nave_base_M;
Model Ala_M;
Model Ala2_M;
Model Aeolipile_base_M;
Model Aeolipile_M;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

//INPUT DE KEYFRAMES
void inputKeyframes(bool* keys);
void inputKeyframesGamepad(XINPUT_STATE& state);
float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;
float ciclo3, ciclo4, ciclo5, ciclo6, ciclo7, ciclo8, cicloXn, cicloXnE = 0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";



//c�lculo del promedio de las normales para sombreado de Phong
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
	meshList.push_back(obj6);

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



///////////////////////////////KEYFRAMES/////////////////////


bool animacion = false;

WORD prevDpadButtons = 0; // para deteccion de borde del D-pad


//NEW// Keyframes
float posXavion = 0.0f, posYavion = 0.0f, posZavion = 0.0f;
float	movAvion_x = 0.0f, movAvion_y = 0.0f;
float giroAvion = 0;

#define MAX_FRAMES 100
int i_max_steps = 60;
int i_curr_steps = 6;//5 cuadros guardados
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movAvion_x;		//Variable para PosicionX
	float movAvion_y;		//Variable para PosicionY
	float movAvion_xInc;		//Variable para IncrementoX
	float movAvion_yInc;		//Variable para IncrementoY
	float giroAvion;
	float giroAvionInc;		//INC guarda valores de interpolacion
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 1;			// el frame 0 esta hardcodeado, los demas se crean con teclado
bool play = false;
int playIndex = 0;

void saveFrame(void) //tecla L //como si le tomara una foto
{
	KeyFrame[FrameIndex].movAvion_x = movAvion_x;
	KeyFrame[FrameIndex].movAvion_y = movAvion_y;
	KeyFrame[FrameIndex].giroAvion = giroAvion;
	printf(">> Frame [%d] guardado: x=%.1f  y=%.1f  giro=%.1f\n", FrameIndex, movAvion_x, movAvion_y, giroAvion);
	if (grabando && archivoKF) {
		fprintf(archivoKF, "%.1f %.1f %.1f\n", movAvion_x, movAvion_y, giroAvion);
		fflush(archivoKF);
	}
	FrameIndex++;
}

void resetElements(void) //Tecla 0
{

	movAvion_x = KeyFrame[0].movAvion_x;
	movAvion_y = KeyFrame[0].movAvion_y;
	giroAvion = KeyFrame[0].giroAvion;
}

void interpolation(void)
{
	KeyFrame[playIndex].movAvion_xInc = (KeyFrame[playIndex + 1].movAvion_x - KeyFrame[playIndex].movAvion_x) / i_max_steps;
	KeyFrame[playIndex].movAvion_yInc = (KeyFrame[playIndex + 1].movAvion_y - KeyFrame[playIndex].movAvion_y) / i_max_steps;
	KeyFrame[playIndex].giroAvionInc = (KeyFrame[playIndex + 1].giroAvion - KeyFrame[playIndex].giroAvion) / i_max_steps;

}


void animate(void)
{
	//Movimiento del objeto // barra espaciadora
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				//printf("entro aqu�\n");
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//printf("se qued� aqui\n");
			//printf("max steps: %f", i_max_steps);
			//Draw animation
			movAvion_x += KeyFrame[playIndex].movAvion_xInc;
			movAvion_y += KeyFrame[playIndex].movAvion_yInc;
			giroAvion += KeyFrame[playIndex].giroAvionInc;
			i_curr_steps++;
		}

	}
}

///////////////* FIN KEYFRAMES*////////////////////////////




int main()
{
	int screenW = GetSystemMetrics(SM_CXSCREEN);
	int screenH = GetSystemMetrics(SM_CYSCREEN);
	int halfW   = screenW / 2;

	mainWindow = Window(halfW, screenH);
	mainWindow.Initialise();

	// Ventana OpenGL: mitad derecha
	glfwSetWindowPos(mainWindow.getGLFWwindow(), halfW, 0);

	// Consola: mitad izquierda
	HWND consoleWnd = GetConsoleWindow();
	MoveWindow(consoleWnd, 0, 0, halfW, screenH, TRUE);

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(3.0f, 22.0f, 38.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -25.0f, 0.5f, 0.5f);

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


	//Kitt_M = Model();
	//Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	//Llanta_M = Model();
	//Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	//Pista_M = Model();
	//Pista_M.LoadModel("Models/pista.obj");
	Nave_base_M = Model();
	Nave_base_M.LoadModel("Models/nave_base.obj");
	Ala_M = Model();
	Ala_M.LoadModel("Models/ala.obj");
	Ala2_M = Model();
	Ala2_M.LoadModel("Models/ala2.obj");
	//Aeolipile_base_M = Model();
	//Aeolipile_base_M.LoadModel("Models/Aeolipile_base.obj");
	//Aeolipile_M = Model();
	//Aeolipile_M.LoadModel("Models/Aeolipile.obj");

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



	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.4f, 0.8f,
		0.0f, -1.0f, 0.0f);
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



	GLint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	movCoche = 0.0f;
	movOffset = 0.01f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;

	glm::vec3 lowerLight(0.0f, 0.0f, 0.0f);
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color (1.0f, 1.0f, 1.0f);
	glm::vec2 toffset (0.0f, 0.0f);


	//Keyframes
	glm::vec3 posblackhawk = glm::vec3(2.0f, 0.0f, 0.0f);

	// Frame 0: posicion inicial (hardcodeado, el resto se crean con teclado)
	KeyFrame[0].movAvion_x = 0.0f;
	KeyFrame[0].movAvion_y = 0.0f;
	KeyFrame[0].giroAvion  = 180.0f;
	giroAvion = 180.0f;

	printf("\n=== MODO CREACION DE KEYFRAMES ===\n");
	printf("Posicion actual: x=%.1f  y=%.1f  giro=%.1f\n\n", movAvion_x, movAvion_y, giroAvion);
	printf("--- Control Xbox ---\n");
	printf("  D-pad IZQ  -> mover nave en X+\n");
	printf("  D-pad DER  -> mover nave en X-\n");
	printf("  D-pad ARR  -> mover nave en Y+\n");
	printf("  D-pad ABA  -> mover nave en Y-\n");
	printf("  Y          -> girar nave 180 grados\n");
	printf("  A          -> guardar frame (tambien escribe al .txt si grabacion activa)\n");
	printf("  X          -> reproducir / detener animacion en memoria\n");
	printf("  B          -> reiniciar animacion desde cero\n\n");
	printf("--- Teclado ---\n");
	printf("  1          -> iniciar grabacion en keyframes.txt\n");
	printf("  2          -> detener grabacion\n");
	printf("  3          -> reproducir animacion desde keyframes.txt\n");
	printf("  4          -> limpiar keyframes.txt\n");
	printf("  ESPACIO    -> reproducir / detener animacion en memoria\n");
	printf("  7 / 8      -> girar nave +45 grados / desbloquear giro\n");
	printf("  Q / E      -> mover nave en X- / desbloquear\n");
	printf("  5 / 6      -> mover nave en Y- / desbloquear\n\n");
	printf("[Frame 0 guardado en origen (x=0, y=0, giro=180)]\n");
	printf("===========================================\n\n");

	
	
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = (GLfloat)glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / (GLfloat)limitFPS;
		lastTime = now;

		angulovaria += 0.5f*deltaTime;

			//�C�mo haces para que el coche no se salga del piso?
			movCoche -= movOffset * deltaTime;
			rotllanta += rotllantaOffset * deltaTime;
	


		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Control Xbox (jugador 0)
		XINPUT_STATE xState;
		if (XInputGetState(0, &xState) == ERROR_SUCCESS)
		{
			float leftX  =  xState.Gamepad.sThumbLX / 32767.0f;
			float leftY  =  xState.Gamepad.sThumbLY / 32767.0f;
			float rightX =  xState.Gamepad.sThumbRX / 32767.0f;
			float rightY =  xState.Gamepad.sThumbRY / 32767.0f;
			camera.gamepadControl(leftX, leftY, rightX, rightY, deltaTime);
			inputKeyframesGamepad(xState);
		}

		//para keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();

		// Aleteo: oscila 0-45 grados solo mientras play==true
		if (play) {
			if (abreAla) {
				if (anguloAla < 45.0f) anguloAla += deltaTime * 4.0f;
				else abreAla = false;
			} else {
				if (anguloAla >= 0.0f) anguloAla -= deltaTime * 4.0f;
				else abreAla = true;
			}
		} else {
			anguloAla = 0.0f;
			abreAla = false;
		}

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
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la c�mara de tipo flash
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//informaci�n al shader de fuentes de iluminaci�n
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		//reiniciar variables antes de que sean enviadas al shader
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);

		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//Pista
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.1f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Pista_M.RenderModel();

		/*//Instancia del coche
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche-50.0f, 0.5f, -2.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();*/


		//Aqu� va la nave con jerarqu�a de modelos, completar
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		toffset = glm::vec2(0.0f, 0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

		model = glm::mat4(1.0);
		posblackhawk = glm::vec3(3.0f + movAvion_x, 13.0f + movAvion_y, 20.0f);
		model = glm::translate(model, posblackhawk);
		modelaux = model; // antes de escala, para jerarquia de luz
		model = glm::rotate(model, 180*toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::rotate(model, (giroAvion) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 modelNave = model; // base compartida para todas las piezas
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelNave));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Nave_base_M.RenderModel();

		// Ala izquierda
		model = modelNave;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.3f));
		model = glm::rotate(model, anguloAla * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ala_M.RenderModel();

		// Ala derecha (gira en sentido contrario para movimiento simetrico)
		model = modelNave;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.3f));
		model = glm::rotate(model, -anguloAla * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ala2_M.RenderModel();

		

		/*//AEOLIPILE
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, -0.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Aeolipile_base_M.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Aeolipile_M.RenderModel();*/



		//Modelos con blending al final para que no afecten a los dem�s objetos, aunque tambi�n se pueden renderizar al inicio pero con blending  activado y desactivado


		/*//Agave �qu� sucede si lo renderizan antes del coche y de la pista?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();*/
		
		/*//textura con movimiento
		toffsetflechau += 0.001;
		toffsetflechav = 0.000;
		//para que no se desborde la variable
		if (toffsetflechau > 1.0)
			toffsetflechau = 0.0;
		//if (toffsetv > 1.0)
		//	toffsetv = 0;
		//pasar a la variable uniform el valor actualizado
		toffset = glm::vec2(toffsetflechau, toffsetflechav);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		FlechaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		//plano con todos los n�meros
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//n�mero 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		for (int i = 1; i<4; i++)
		{
			//n�meros 2-4
			toffsetnumerou += 0.25;
			toffsetnumerov = 0.0;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-10.0f - (i * 3.0), 2.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();

		 }

		for (int j = 1; j < 5; j++)
		{
			//n�meros 5-8
			toffsetnumerou += 0.25;
			toffsetnumerov = -0.33;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-7.0f - (j * 3.0), 5.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();
		}
 

		//n�mero cambiante
		static float timerNumeroC = 0.0f;
		timerNumeroC += deltaTime;
		if (timerNumeroC >= 0.5f) {
			timerNumeroC = 0.0f;
			toffsetnumerocambiau += 0.25f;
			if (toffsetnumerocambiau >= 1.0f)
				toffsetnumerocambiau = 0.0f;
		}
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerocambiau, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		//cambiar autom�ticamente entre textura n�mero 1 y n�mero 2
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Numero1Texture.UseTexture();
		//if
		//Numero1Texture.UseTexture();
		//Numero2Texture.UseTexture();
		
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();*/


		glDisable(GL_BLEND);
		
		



		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

void inputKeyframesGamepad(XINPUT_STATE& state)
{
	WORD curr = state.Gamepad.wButtons;
	WORD pressed = curr & ~prevDpadButtons; // botones que se presionaron este frame

	if (pressed & XINPUT_GAMEPAD_DPAD_LEFT)
	{
		movAvion_x -= 1.0f;
		printf(">> D-pad IZQ  | Pos -> x=%.1f  y=%.1f  giro=%.1f\n", movAvion_x, movAvion_y, giroAvion);
	}
	if (pressed & XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		movAvion_x += 1.0f;
		printf(">> D-pad DER  | Pos -> x=%.1f  y=%.1f  giro=%.1f\n", movAvion_x, movAvion_y, giroAvion);
	}
	if (pressed & XINPUT_GAMEPAD_DPAD_UP)
	{
		movAvion_y += 1.0f;
		printf(">> D-pad ARR  | Pos -> x=%.1f  y=%.1f  giro=%.1f\n", movAvion_x, movAvion_y, giroAvion);
	}
	if (pressed & XINPUT_GAMEPAD_DPAD_DOWN)
	{
		movAvion_y -= 1.0f;
		printf(">> D-pad ABA  | Pos -> x=%.1f  y=%.1f  giro=%.1f\n", movAvion_x, movAvion_y, giroAvion);
	}

	// A -> guardar frame
	if (pressed & XINPUT_GAMEPAD_A)
	{
		saveFrame();
	}

	// B -> reiniciar animacion desde cero
	if (pressed & XINPUT_GAMEPAD_B)
	{
		play = false;
		playIndex = 0;
		i_curr_steps = 0;
		FrameIndex = 1;
		resetElements();
		printf(">> B (reset)  | Animacion reiniciada. Frame 0 restaurado.\n");
	}

	// X -> reproducir animacion
	if (pressed & XINPUT_GAMEPAD_X)
	{
		if (play == false && FrameIndex > 1)
		{
			resetElements();
			interpolation();
			play = true;
			playIndex = 0;
			i_curr_steps = 0;
			printf(">> X (play)   | Reproduciendo animacion...\n");
		}
		else
		{
			play = false;
			printf(">> X (stop)   | Animacion detenida\n");
		}
	}

	// Y -> girar nave 180 grados en Z
	if (pressed & XINPUT_GAMEPAD_Y)
	{
		giroAvion += 180.0f;
		if (giroAvion >= 360.0f) giroAvion -= 360.0f;
		printf(">> Y (giro)   | Pos -> x=%.1f  y=%.1f  giro=%.1f\n", movAvion_x, movAvion_y, giroAvion);
	}

	prevDpadButtons = curr;
}

void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("presiona 0 para habilitar reproducir de nuevo la animaci�n'\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;

			}
		}
	}
	if (keys[GLFW_KEY_0])//mejor para lo de la latencia.
	{
		if (habilitaranimacion < 1)
		{
			reproduciranimacion = 0;
			habilitaranimacion = 1;
			printf("Ya puedes reproducir de nuevo la animaci�n con la tecla de barra espaciadora'\n");
		}
	}

	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			saveFrame();
			//printf("movAvion_x es: %f\n", movAvion_x);
			//printf("movAvion_y es: %f\n", movAvion_y);
			printf("presiona P para habilitar guardar otro frame'\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}
	if (keys[GLFW_KEY_P])
	{
		if (reinicioFrame < 1)
		{
			guardoFrame = 0;
			printf("Ya puedes guardar otro frame presionando la tecla L'\n");
		}
	}


	// 4 - limpiar keyframes.txt
	{
		static bool k4Prev = false;
		bool k4Now = keys[GLFW_KEY_4];
		if (k4Now && !k4Prev) {
			if (grabando) {
				printf(">> [4] Detén la grabacion primero (tecla 2)\n");
			} else {
				// Backup: copiar keyframes.txt -> keyframes_backup.txt antes de limpiar
				FILE* src = fopen("keyframes.txt", "r");
				if (src) {
					FILE* dst = fopen("keyframes_backup.txt", "w");
					if (dst) {
						char buf[256];
						while (fgets(buf, sizeof(buf), src))
							fputs(buf, dst);
						fclose(dst);
						printf(">> [4] Backup guardado en keyframes_backup.txt\n");
					}
					fclose(src);
				}
				// Limpiar keyframes.txt
				FILE* f = fopen("keyframes.txt", "w");
				if (f) {
					fclose(f);
					printf(">> [4] keyframes.txt limpiado\n");
				} else {
					printf(">> [4] ERROR: no se pudo limpiar keyframes.txt\n");
				}
			}
			fflush(stdout);
		}
		k4Prev = k4Now;
	}

	// --- Y- (baja) ---
	if (keys[GLFW_KEY_5])
	{
		if (ciclo5 < 1)
		{
			movAvion_y -= 1.0f;
			printf("Pos actual -> x=%.1f  y=%.1f  giro=%.1f   (presiona 6 para volver a mover en Y-)\n", movAvion_x, movAvion_y, giroAvion);
			ciclo5++;
			ciclo6 = 0;
		}
	}
	if (keys[GLFW_KEY_6])
	{
		if (ciclo6 < 1)
		{
			ciclo5 = 0;
			ciclo6 = 1;
		}
	}

	// --- Giro +45 grados ---
	if (keys[GLFW_KEY_7])
	{
		if (ciclo7 < 1)
		{
			giroAvion += 45.0f;
			if (giroAvion >= 360.0f) giroAvion -= 360.0f;
			printf("Pos actual -> x=%.1f  y=%.1f  giro=%.1f   (presiona 8 para volver a girar)\n", movAvion_x, movAvion_y, giroAvion);
			ciclo7++;
			ciclo8 = 0;
		}
	}
	if (keys[GLFW_KEY_8])
	{
		if (ciclo8 < 1)
		{
			ciclo7 = 0;
			ciclo8 = 1;
		}
	}

	// --- X- (regreso) ---
	if (keys[GLFW_KEY_Q])
	{
		if (cicloXn < 1)
		{
			movAvion_x -= 1.0f;
			printf("Pos actual -> x=%.1f  y=%.1f  giro=%.1f   (presiona E para volver a mover en X-)\n", movAvion_x, movAvion_y, giroAvion);
			cicloXn++;
			cicloXnE = 0;
		}
	}
	if (keys[GLFW_KEY_E])
	{
		if (cicloXnE < 1)
		{
			cicloXn = 0;
			cicloXnE = 1;
		}
	}

	// 1 - iniciar grabacion en keyframes.txt
	{
		static bool k1Prev = false;
		bool k1Now = keys[GLFW_KEY_1];
		if (k1Now && !k1Prev) {
			if (grabando) {
				printf(">> [1] Ya hay una grabacion activa (presiona 2 para terminarla)\n");
			} else {
				printf(">> [1] Tecla detectada - abriendo keyframes.txt...\n");
				fflush(stdout);
				archivoKF = fopen("keyframes.txt", "w");
				if (archivoKF) {
					grabando = true;
					fprintf(archivoKF, "%.1f %.1f %.1f\n",
						KeyFrame[0].movAvion_x, KeyFrame[0].movAvion_y, KeyFrame[0].giroAvion);
					fflush(archivoKF);
					printf(">> [1] GRABACION INICIADA - keyframes.txt abierto correctamente\n");
					printf("       Usa A del control para guardar frames, tecla 2 para terminar\n");
				} else {
					printf(">> [1] ERROR: no se pudo crear keyframes.txt\n");
				}
			}
			fflush(stdout);
		}
		k1Prev = k1Now;
	}

	// 2 - detener grabacion
	{
		static bool k2Prev = false;
		bool k2Now = keys[GLFW_KEY_2];
		if (k2Now && !k2Prev) {
			if (!grabando) {
				printf(">> [2] No hay grabacion activa (inicia con tecla 1)\n");
			} else {
				fclose(archivoKF);
				archivoKF = nullptr;
				grabando = false;
				printf(">> [2] GRABACION TERMINADA - keyframes.txt cerrado (%d frames guardados)\n", FrameIndex - 1);
			}
			fflush(stdout);
		}
		k2Prev = k2Now;
	}

	// 3 - reproducir animacion desde keyframes.txt
	{
		static bool k3Prev = false;
		bool k3Now = keys[GLFW_KEY_3];
		if (k3Now && !k3Prev) {
			printf(">> [3] Tecla detectada - buscando keyframes.txt...\n");
			fflush(stdout);
			FILE* f = fopen("keyframes.txt", "r");
			if (f) {
				FrameIndex = 0;
				float x, y, g;
				while (fscanf(f, "%f %f %f", &x, &y, &g) == 3 && FrameIndex < MAX_FRAMES) {
					KeyFrame[FrameIndex].movAvion_x = x;
					KeyFrame[FrameIndex].movAvion_y = y;
					KeyFrame[FrameIndex].giroAvion  = g;
					FrameIndex++;
				}
				fclose(f);
				if (FrameIndex > 1) {
					resetElements();
					interpolation();
					play = true;
					playIndex = 0;
					i_curr_steps = 0;
					printf(">> [3] Reproduciendo animacion de archivo keyframes.txt (%d frames)\n", FrameIndex);
				} else {
					printf(">> [3] keyframes.txt vacio o sin frames suficientes\n");
				}
			} else {
				printf(">> [3] No se encontro keyframes.txt - graba primero con tecla 1\n");
			}
			fflush(stdout);
		}
		k3Prev = k3Now;
	}

}
