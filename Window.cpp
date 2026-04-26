//420054085 Ortega de la Paz Rafael

#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 2.0f;
	mueveCoche = 0.0f;     // Inicializado
	mueveCofre = 0.0f;  // Inicializado
	mueveLlanta = 0.0f;  // Inicializado
	mueveHelicoptero = 0.0f;//Inicializando helicptero
	muevePez = 0.0f;
	mueveFocoX = 0.0f;
	mueveFocoY = 0.0f;
	mueveFocoZ = 0.0f;
	lampSw = true;
	contador = 0;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "420054085 / Practica07: Iluminacion 1", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
	// Asignar valores de la ventana y coordenadas

	//Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_Y)
	{
		theWindow->muevex += 1.0;
	}
	if (key == GLFW_KEY_U)
	{
		theWindow->muevex -= 1.0;
	}



	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}

	//mover coche Y LUZ
	if (key == GLFW_KEY_U) {
		theWindow->mueveCoche += 0.5f;
		theWindow->posicionLuz += 0.5f;
	}
	if (key == GLFW_KEY_J) {
		theWindow->mueveCoche -= 0.5f;
		theWindow->posicionLuz -= 0.5f;
	}

	//moverHelicoptero
	if (key == GLFW_KEY_Y) {
		theWindow->mueveHelicoptero += 0.5f;
	
	}
	if (key == GLFW_KEY_H) {
		theWindow->mueveHelicoptero -= 0.5f;;
	}

	// Abrir/Cerrar cofre
	if (key == GLFW_KEY_I) { theWindow->mueveCofre += 5.0f; }
	if (key == GLFW_KEY_K) { theWindow->mueveCofre -= 5.0f; }

	//Mover Llantas
	if (key == GLFW_KEY_O) { theWindow->mueveLlanta += 5.0f; }
	if (key == GLFW_KEY_L) { theWindow->mueveLlanta -= 5.0f; }

	//Switch lámpara
	if (key == GLFW_KEY_T && action == GLFW_PRESS) {theWindow->lampSw = !theWindow->lampSw; }

	//Colores faro
	if (key == GLFW_KEY_G && action == GLFW_PRESS) { 
		theWindow->contador = theWindow->contador+1;
		if (theWindow->contador >= 6) { theWindow->contador = 0; }
	}

	//MoverPez
	if (key == GLFW_KEY_R) { theWindow->muevePez += 1.0f; }
	if (key == GLFW_KEY_F) { theWindow->muevePez -= 1.0f; }

	//Switch pez
	if (key == GLFW_KEY_E && action == GLFW_PRESS) { theWindow->pezSw = !theWindow->pezSw; }

	//Mueve Foco Pez
		//moverHelicoptero
	if (key == GLFW_KEY_B) {
		theWindow->mueveFocoX += 10.0f;

	}
	if (key == GLFW_KEY_N) {
		theWindow->mueveFocoY += 10.0f;

	}
	if (key == GLFW_KEY_M) {
		theWindow->mueveFocoZ += 10.0f;

	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
