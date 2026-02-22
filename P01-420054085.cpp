#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>
#include <stdlib.h>
#include <time.h>

float intervaloCambio = 0.5f; // El tiempo que dura cada color (2 segundos)
double tiempoAnterior = 0.0; // Aqui guardamos la última vez que cambió el color

//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 800;
GLuint VAO, VBO, shader;//variables de OpenGL//VAO(arregloDeVertice)//VBO(objetoDeBufferDeLosVertices)//shader(programaDeShader(VertexYFragment))
GLint colorLoc;

//LENGUAJE DE SHADER (SOMBRAS) GLSL //siempre tiene IN
//Vertex Shader
//recibir color, salida Vcolor
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";

//Fragment Shader //siempre tiene OUT
//recibir Vcolor y dar de salida color
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
uniform vec4 colorFigura;										\n\
void main()											\n\
{													\n\
	color = colorFigura;	 			\n\
}";



void CrearFiguras()
{
	float vertices[] = {
		//R
		// Línea vertical izquierda
		-0.9f,-0.8f,0.0f,  -0.8f,-0.8f,0.0f,  -0.8f,-0.2f,0.0f,
		-0.9f,-0.8f,0.0f,  -0.8f,-0.2f,0.0f,  -0.9f,-0.2f,0.0f,

		// Línea superior
		-0.8f,-0.2f,0.0f,  -0.4f,-0.2f,0.0f,  -0.4f,-0.3f,0.0f,
		-0.8f,-0.2f,0.0f,  -0.4f,-0.3f,0.0f,  -0.8f,-0.3f,0.0f,

		// Línea vertical derecha
		-0.5f,-0.3f,0.0f,  -0.4f,-0.3f,0.0f,  -0.4f,-0.6f,0.0f,
		-0.5f,-0.3f,0.0f,  -0.4f,-0.6f,0.0f,  -0.5f,-0.6f,0.0f,

		// Línea media horizontal
		-0.8f,-0.5f,0.0f,  -0.5f,-0.5f,0.0f,  -0.5f,-0.6f,0.0f,
		-0.8f,-0.5f,0.0f,  -0.5f,-0.6f,0.0f,  -0.8f,-0.6f,0.0f,

		// Pierna diagonal (Iniciando desde el poste vertical izquierdo)
		-0.90f,-0.5f,0.0f, -0.8f,-0.5f,0.0f, -0.4f,-0.8f,0.0f,
		-0.90f,-0.5f,0.0f, -0.4f,-0.8f,0.0f, -0.6f,-0.8f,0.0f,

		//O
		// Linea vertical izquierda
		-0.3f,-0.1f,0.0f, -0.2f,-0.1f,0.0f, -0.2f,0.3f,0.0f,
		-0.3f,-0.1f,0.0f, -0.2f,0.3f,0.0f, -0.3f,0.3f,0.0f,

		// Linea vertical derecha
		0.0f,-0.1f,0.0f, 0.1f,-0.1f,0.0f, 0.1f,0.3f,0.0f,
		0.0f,-0.1f,0.0f, 0.1f,0.3f,0.0f, 0.0f,0.3f,0.0f,

		// Linea superior horizontal
		-0.2f,0.3f,0.0f, 0.0f,0.3f,0.0f, 0.0f,0.4f,0.0f,
		-0.2f,0.3f,0.0f, 0.0f,0.4f,0.0f, -0.2f,0.4f,0.0f,

		// Linea inferior horizontal
		-0.2f,-0.2f,0.0f, 0.0f,-0.2f,0.0f, 0.0f,-0.1f,0.0f,
		-0.2f,-0.2f,0.0f, 0.0f,-0.1f,0.0f, -0.2f,-0.1f,0.0f,

		// Diagonal superior izquierda
		-0.3f,0.3f,0.0f, -0.2f,0.4f,0.0f, -0.2f,0.3f,0.0f,

		// Diagonal superior derecha
		0.1f,0.3f,0.0f, 0.0f,0.3f,0.0f, 0.0f,0.4f,0.0f,

		// Diagonal inferior izquierda
		-0.3f,-0.1f,0.0f, -0.2f,-0.1f,0.0f, -0.2f,-0.2f,0.0f,

		// Diagonal inferior derecha
		0.1f,-0.1f,0.0f, 0.0f,-0.1f,0.0f, 0.0f,-0.2f,0.0f,

		// --- P ---
		// Línea vertical larga izquierda (X: 0.2 a 0.3 | Y: 0.4 a 1.0)
		0.2f,0.4f,0.0f, 0.3f,0.4f,0.0f, 0.3f,1.0f,0.0f,
		0.2f,0.4f,0.0f, 0.3f,1.0f,0.0f, 0.2f,1.0f,0.0f,

		// Línea superior horizontal (X: 0.3 a 0.6 | Y: 0.9 a 1.0)
		0.3f,0.9f,0.0f, 0.6f,0.9f,0.0f, 0.6f,1.0f,0.0f,
		0.3f,0.9f,0.0f, 0.6f,1.0f,0.0f, 0.3f,1.0f,0.0f,

		// Línea inferior horizontal (X: 0.3 a 0.6 | Y: 0.7 a 0.8)
		0.3f,0.7f,0.0f, 0.6f,0.7f,0.0f, 0.6f,0.8f,0.0f,
		0.3f,0.7f,0.0f, 0.6f,0.8f,0.0f, 0.3f,0.8f,0.0f,

		// Línea pequeña vertical derecha (X: 0.5 a 0.6 | Y: 0.8 a 0.9)
		0.5f,0.8f,0.0f, 0.6f,0.8f,0.0f, 0.6f,0.9f,0.0f,
		0.5f,0.8f,0.0f, 0.6f,0.9f,0.0f, 0.5f,0.9f,0.0f
	};
	glGenVertexArrays(1, &VAO); //generar 1 VAO//ahi van mis nueve datos flotantes
	glBindVertexArray(VAO);//asignar VAO//ligo el VAO

	glGenBuffers(1, &VBO);//genero VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//loMandoAlBuffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamano, los datos y en este caso es estático pues no se modificarán los valores

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);//Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
		glEnableVertexAttribArray(0);//datos que agrupe de 3 en 3 se los mando a in vec3 pos//se convierten en el main a 3 vectores de 3
		//agregar valores a vèrtices y luego declarar un nuevo vertexAttribPointer
		glBindBuffer(GL_ARRAY_BUFFER, 0);//ligar datos de bindBuffer
		glBindVertexArray(0);

}
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) //Función para agregar los shaders a la tarjeta gráfica

//the Program recibe los datos de theShader

{
	GLuint theShader = glCreateShader(shaderType);//theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
	const GLchar* theCode[1];
	theCode[0] = shaderCode;//shaderCode es el texto que se le pasa a theCode
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);//longitud del texto
	glShaderSource(theShader,1, theCode, codeLength);//Se le asigna al shader el código
	glCompileShader(theShader);//Se comila el shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	//verificaciones y prevención de errores
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n",shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);//Si no hubo problemas se asigna el shader a theProgram el cual asigna el código a la tarjeta gráfica
}

void CompileShaders() {
	shader= glCreateProgram(); //se crea un programa
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader
	//Para terminar de linkear el programa y ver que no tengamos errores
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);//se linkean los shaders a la tarjeta gráfica
	 //verificaciones y prevención de errores
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}



}
int main()
{
	srand(time(NULL));
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}

	//****  LAS SIGUIENTES 4 LÍNEAS SE COMENTAN EN DADO CASO DE QUE AL USUARIO NO LE FUNCIONE LA VENTANA Y PUEDA CONOCER LA VERSIÓN DE OPENGL QUE TIENE ****/

	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Primer ventana", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);

 //Llamada a las funciones creadas antes del main //LO NUEVO
	CrearFiguras();
	CompileShaders();
	// Color inicial (NO negro)
	float r = (float)rand() / RAND_MAX;
	float g = (float)rand() / RAND_MAX;
	float b = (float)rand() / RAND_MAX;

	glClearColor(r, g, b, 1.0f);
	tiempoAnterior = glfwGetTime();

	colorLoc = glGetUniformLocation(shader, "colorFigura");
	if (colorLoc == -1) {
		printf("No se encontró el uniform colorFigura\n");
	}



	//Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow))
	{
		//Recibir eventos del usuario
		glfwPollEvents();

		// 1. Le preguntamos a GLFW qué hora es
		double tiempoActual = glfwGetTime();

		// 2. Restamos: "Tiempo de ahora" menos "Tiempo del último cambio"
		// Si esa resta es mayor o igual a nuestro intervalo (2.0s), entramos al IF
		if (tiempoActual - tiempoAnterior >= intervaloCambio)
		{
			float r = (float)rand() / RAND_MAX;
			float g = (float)rand() / RAND_MAX;
			float b = (float)rand() / RAND_MAX;

			glClearColor(r, g, b, 1.0f);

			tiempoAnterior = tiempoActual;
		}

		glClear(GL_COLOR_BUFFER_BIT);//MUESTRA EL COLOR

		glUseProgram(shader);//indice, me regresa 0

		glBindVertexArray(VAO);//que vao voy a utilizar

		glUniform4f(colorLoc, 1.0f, 1.0f, 0.0f, 1.0f);
		glDrawArrays(GL_TRIANGLES,0,90);//voy a dibujar triangulos, desde mi primer dato, leyendo 3 datos
		glBindVertexArray(0);//libero para no seguir utilizando en memoria

		glUseProgram(0);//libero el programa

		glfwSwapBuffers(mainWindow);
		 
		//NO ESCRIBIR NINGUNA LÍNEA DESPUÉS DE glfwSwapBuffers(mainWindow); 
	}


	return 0;
}