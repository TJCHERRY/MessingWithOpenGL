#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include <SOIL.h>

// Other includes
#include "sHader.h"
#include "lightShader.h"


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void do_movement();

GLfloat texden = 0.5f;

GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame
// Window dimensions
const GLuint WIDTH = 1366, HEIGHT = 786;
// Light attributes
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.5f, 0.0f);

GLfloat yaw = -90.0;
GLfloat pitch = 0.0f;

GLfloat lastX = WIDTH / 2;
GLfloat  lastY = HEIGHT / 2;
bool keys[1024];
// The MAIN function, from here we start the application and run the game loop
int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Specialist Practise II", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	//glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	int widths, heights;
	glfwGetFramebufferSize(window, &widths, &heights);
	glViewport(0, 0, widths, heights);
	glEnable(GL_DEPTH_TEST);



	// Build and compile our shader program
	//Shader ourShader("path/to/shaders/textures.vs", "path/to/shaders/textures.frag");
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	
	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--------------------------------------------------------------------------------------
	GLfloat quadVerts[] = {
		0.5f,  0.5f, 0.0f, texden, texden,   // Top Right
		0.5f, -0.5f, 0.0f, texden, 0.0f,   // Bottom Right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // Bottom Left
		-0.5f,  0.5f, 0.0f, 0.0f, texden    // Top Left 
};



	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};
	

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat cubeVertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	//=============================================

	GLfloat texCubeVerts[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.5f, 0.5f,
		0.5f,  0.5f, -0.5f,  0.5f, 0.5f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.5f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.5f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.5f, 0.5f,
		0.5f,  0.5f,  0.5f,  0.5f, 0.5f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.5f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f,  -0.5f,  0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.5f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.5f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.5f, 0.0f,

		0.5f,  0.5f,  0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,  0.0f, 0.5f,
		0.5f, -0.5f, -0.5f,  0.0f, 0.5f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.5f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 0.5f,
		0.5f, -0.5f, -0.5f,  0.5f, 0.5f,
		0.5f, -0.5f,  0.5f,  0.5f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.5f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.5f,

		-0.5f,  0.5f, -0.5f,  0.0f, 0.5f,
		0.5f,  0.5f, -0.5f,  0.5f, 0.5f,
		0.5f,  0.5f,  0.5f,  0.5f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.5f

	};
	//=======================================
	GLfloat WALLCubeVerts[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	//==============================
	//Doorpositions
	//world space coordinates
	glm::vec3 cubePositions[] = {
		//pillar1
		glm::vec3(2.3f, -1.37f, 4.0f),
		glm::vec3(2.3f,  -0.37f, 4.0f),
		glm::vec3(2.3f,  0.63f, 4.0f),
		glm::vec3(2.3f,  1.63f, 4.0f),
		glm::vec3(2.3f,  2.63f, 4.0f),

		//pillar2
		glm::vec3(2.3f, -1.37f, 0.0f),
		glm::vec3(2.3f,  -0.37f, 0.0f),
		glm::vec3(2.3f,  0.63f, 0.0f),
		glm::vec3(2.3f,  1.63f, 0.0f),
		glm::vec3(2.3f,  2.63f, 0.0f),
		//pillar3
		glm::vec3(-2.7f, -1.37f, 0.0f),
		glm::vec3(-2.7f,  -0.37f, 0.0f),
		glm::vec3(-2.7f,  0.63f, 0.0f),
		glm::vec3(-2.7f,  1.63f, 0.0f),
		glm::vec3(-2.7f,  2.63f, 0.0f),
		//pillar4
		glm::vec3(-2.7f, -1.37f, 4.0f),
		glm::vec3(-2.7f,  -0.37f,4.0f),
		glm::vec3(-2.7f,  0.63f, 4.0f),
		glm::vec3(-2.7f,  1.63f, 4.0f),
		glm::vec3(-2.7f,  2.63f, 4.0f),
		//pillar5
		glm::vec3(-4.7f, -1.37f, -3.7f),
		glm::vec3(-4.7f,  -0.37f,-3.7f),
		glm::vec3(-4.7f,  0.63f, -3.7f),
		glm::vec3(-4.7f,  1.63f, -3.7f),
		glm::vec3(-4.7f,  2.63f, -3.7f),
		glm::vec3(-4.7f,  3.63f, -3.7f),
		//pillar6
		glm::vec3(-8.7f, -1.37f, -3.7f),
		glm::vec3(-8.7f,  -0.37f,-3.7f),
		glm::vec3(-8.7f,  0.63f, -3.7f),
		glm::vec3(-8.7f,  1.63f, -3.7f),
		glm::vec3(-8.7f,  2.63f, -3.7f),
		glm::vec3(-8.7f,  3.63f, -3.7f),

		glm::vec3(-12.7f, -1.37f, -3.7f),
		glm::vec3(-12.7f,  -0.37f,-3.7f),
		glm::vec3(-12.7f,  0.63f, -3.7f),
		glm::vec3(-12.7f,  1.63f, -3.7f),
		glm::vec3(-12.7f,  2.63f, -3.7f),
		glm::vec3(-12.7f,  3.63f, -3.7f),
		//-------------------------------------------
		glm::vec3(-4.7f, -1.37f, -8.7f),
		glm::vec3(-4.7f,  -0.37f,-8.7f),
		glm::vec3(-4.7f,  0.63f, -8.7f),
		glm::vec3(-4.7f,  1.63f, -8.7f),
		glm::vec3(-4.7f,  2.63f, -8.7f),
		glm::vec3(-4.7f,  3.63f, -8.7f),

		glm::vec3(-8.7f, -1.37f, -8.7f),
		glm::vec3(-8.7f,  -0.37f,-8.7f),
		glm::vec3(-8.7f,  0.63f, -8.7f),
		glm::vec3(-8.7f,  1.63f, -8.7f),
		glm::vec3(-8.7f,  2.63f, -8.7f),
		glm::vec3(-8.7f,  3.63f, -8.7f),

		glm::vec3(-12.7f, -1.37f, -8.7f),
		glm::vec3(-12.7f,  -0.37f,-8.7f),
		glm::vec3(-12.7f,  0.63f, -8.7f),
		glm::vec3(-12.7f,  1.63f, -8.7f),
		glm::vec3(-12.7f,  2.63f, -8.7f),
		glm::vec3(-12.7f,  3.63f, -8.7f),
		//======================================================
		glm::vec3(-4.7f, -1.37f, -3.7f),
		glm::vec3(-4.7f,  -0.37f,-3.7f),
		glm::vec3(-4.7f,  0.63f, -3.7f),
		glm::vec3(-4.7f,  1.63f, -3.7f),
		glm::vec3(-4.7f,  2.63f, -3.7f),
		glm::vec3(-4.7f,  3.63f, -3.7f),
		//pillar6
		glm::vec3(-8.7f, -1.37f, -3.7f),
		glm::vec3(-8.7f,  -0.37f,-3.7f),
		glm::vec3(-8.7f,  0.63f, -3.7f),
		glm::vec3(-8.7f,  1.63f, -3.7f),
		glm::vec3(-8.7f,  2.63f, -3.7f),
		glm::vec3(-8.7f,  3.63f, -3.7f),

		glm::vec3(-12.7f, -1.37f, -3.7f),
		glm::vec3(-12.7f,  -0.37f,-3.7f),
		glm::vec3(-12.7f,  0.63f, -3.7f),
		glm::vec3(-12.7f,  1.63f, -3.7f),
		glm::vec3(-12.7f,  2.63f, -3.7f),
		glm::vec3(-12.7f,  3.63f, -3.7f),
		//-------------------------------------------
		glm::vec3(4.3f, -1.37f, -8.7f),
		glm::vec3(4.3f,  -0.37f,-8.7f),
		glm::vec3(4.3f,  0.63f, -8.7f),
		glm::vec3(4.3f,  1.63f, -8.7f),
		glm::vec3(4.3f,  2.63f, -8.7f),
		glm::vec3(4.3f,  3.63f, -8.7f),

		glm::vec3(8.3f, -1.37f, -8.7f),
		glm::vec3(8.3f,  -0.37f,-8.7f),
		glm::vec3(8.3f,  0.63f, -8.7f),
		glm::vec3(8.3f,  1.63f, -8.7f),
		glm::vec3(8.3f,  2.63f, -8.7f),
		glm::vec3(8.3f,  3.63f, -8.7f),

		glm::vec3(12.3f, -1.37f, -8.7f),
		glm::vec3(12.3f,  -0.37f,-8.7f),
		glm::vec3(12.3f,  0.63f, -8.7f),
		glm::vec3(12.3f,  1.63f, -8.7f),
		glm::vec3(12.3f,  2.63f, -8.7f),
		glm::vec3(12.3f,  3.63f, -8.7f),
			//===================================
			glm::vec3(4.3f, -1.37f, -3.7f),
			glm::vec3(4.3f, -0.37f, -3.7f),
			glm::vec3(4.3f, 0.63f, -3.7f),
			glm::vec3(4.3f, 1.63f, -3.7f),
			glm::vec3(4.3f, 2.63f, -3.7f),
			glm::vec3(4.3f, 3.63f, -3.7f),
			//pillar6
			glm::vec3(8.3f, -1.37f, -3.7f),
			glm::vec3(8.3f, -0.37f, -3.7f),
			glm::vec3(8.3f, 0.63f, -3.7f),
			glm::vec3(8.3f, 1.63f, -3.7f),
			glm::vec3(8.3f, 2.63f, -3.7f),
			glm::vec3(8.3f, 3.63f, -3.7f),

			glm::vec3(12.3f, -1.37f, -3.7f),
			glm::vec3(12.3f, -0.37f, -3.7f),
			glm::vec3(12.3f, 0.63f, -3.7f),
			glm::vec3(12.3f, 1.63f, -3.7f),
			glm::vec3(12.3f, 2.63f, -3.7f),
			glm::vec3(12.3f, 3.63f, -3.7f),
		//glm::vec3(-2.0f, -1.2f, -9.3f),
		//glm::vec3(-2.0f,  -0.5f, -9.3f),
		//glm::vec3(-2.0f,  0.2f, -9.3f),
		//glm::vec3(-2.0f,  0.9f, -9.3f)
		//glm::vec3(0.0f,  0.0f,  1.0f),
		//glm::vec3(0.0f,  0.0f, 0.0f),
		//glm::vec3(-1.5f, -2.2f, -2.5f),
		//glm::vec3(-3.8f, -2.0f, -12.3f),
		//glm::vec3(-1.5f, 1.3f, -3.0f),
		//glm::vec3(-1.5f,  0.2f, -3.0f),

		//center beam

		glm::vec3(-2.85f,  3.71f, -3.7f),
		glm::vec3(-2.85f,  3.71f, -4.7f),
		glm::vec3(-2.85f,  3.71f, -5.7f),
		glm::vec3(-2.85f,  3.71f, -6.7f),
		glm::vec3(-2.85f,  3.71f, -7.7f),
		glm::vec3(-2.85f,  3.71f, -8.7f),

		glm::vec3(2.45f,  3.71f, -3.7f),
		glm::vec3(1.45f,  3.71f, -3.7f),
		glm::vec3(0.45f,  3.71f, -3.7f),
		glm::vec3(-0.55f,  3.71f, -3.7f),
		glm::vec3(-1.55f,  3.71f, -3.7f),
		glm::vec3(-2.55f,  3.71f, -3.7f),

		glm::vec3(2.45f,  3.71f, -8.7f),
		glm::vec3(1.45f,  3.71f, -8.7f),
		glm::vec3(0.45f,  3.71f, -8.7f),
		glm::vec3(-0.55f,  3.71f, -8.7f),
		glm::vec3(-1.55f,  3.71f, -8.7f),
		glm::vec3(-2.55f,  3.71f, -8.7f),
		
		glm::vec3(2.45f,  3.71f, -4.7f),
		glm::vec3(2.45f,  3.71f, -5.7f),
		glm::vec3(2.45f,  3.71f, -6.7f),
		glm::vec3(2.45f,  3.71f, -7.7f),
		glm::vec3(2.45f,  3.71f, -8.7f),
		
	};

	glm::vec3 texPos[] = {
		glm::vec3(-2.7f,-1.8f,-2.85f),
		glm::vec3(-2.7f,-1.8f,-2.0f),
		glm::vec3(-2.7f,-1.8f,-1.15f),
		glm::vec3(-2.7f,-1.8f,-0.3f),
		glm::vec3(-2.7f, -1.8f, 0.15f),
		glm::vec3(-2.7f, -1.8f, 1.0f),
		glm::vec3(-2.7f, -1.8f, 1.85f),
		glm::vec3(-2.7f, -1.8f, 2.7f),
		glm::vec3(-2.7f, -1.8f, 3.55f),
		glm::vec3(-2.7f, -1.8f, 4.4f),

		glm::vec3(2.3f,-1.8f,-2.85f),
		glm::vec3(2.3f,-1.8f,-2.0f),
		glm::vec3(2.3f,-1.8f,-1.15f),
		glm::vec3(2.3f,-1.8f,-0.3f),
		glm::vec3(2.3f, -1.8f, 0.15f),
		glm::vec3(2.3f, -1.8f, 1.0f),
		glm::vec3(2.3f, -1.8f, 1.85f),
		glm::vec3(2.3f, -1.8f, 2.7f),
		glm::vec3(2.3f, -1.8f, 3.55f),
		glm::vec3(2.3f, -1.8f, 4.4f),

		//column1
		glm::vec3(-0.7f,-1.8f,-2.85f),
		glm::vec3(-0.7f,-1.8f,-2.0f),
		glm::vec3(-0.7f,-1.8f,-1.15f),
		glm::vec3(-0.7f,-1.8f,-0.3f),
		glm::vec3(-0.7f, -1.8f, 0.15f),
		glm::vec3(-0.7f, -1.8f, 1.0f),
		glm::vec3(-0.7f, -1.8f, 1.85f),
		glm::vec3(-0.7f, -1.8f, 2.7f),
		glm::vec3(-0.7f, -1.8f, 3.55f),
		glm::vec3(-0.7f, -1.8f, 4.4f),
		//column2
		glm::vec3(0.3f,-1.8f,-2.85f),
		glm::vec3(0.3f,-1.8f,-2.0f),
		glm::vec3(0.3f,-1.8f,-1.15f),
		glm::vec3(0.3f,-1.8f,-0.3f),
		glm::vec3(0.3f, -1.8f, 0.15f),
		glm::vec3(0.3f, -1.8f, 1.0f),
		glm::vec3(0.3f, -1.8f, 1.85f),
		glm::vec3(0.3f, -1.8f, 2.7f),
		glm::vec3(0.3f, -1.8f, 3.55f),
		glm::vec3(0.3f, -1.8f, 4.4f),
		//column3
		glm::vec3(1.3f,-1.8f,-2.85f),
		glm::vec3(1.3f,-1.8f,-2.0f),
		glm::vec3(1.3f,-1.8f,-1.15f),
		glm::vec3(1.3f,-1.8f,-0.3f),
		glm::vec3(1.3f, -1.8f, 0.15f),
		glm::vec3(1.3f, -1.8f, 1.0f),
		glm::vec3(1.3f, -1.8f, 1.85f),
		glm::vec3(1.3f, -1.8f, 2.7f),
		glm::vec3(1.3f, -1.8f, 3.55f),
		glm::vec3(1.3f, -1.8f, 4.4f),
		//column4
		//column3
		glm::vec3(-1.7f,-1.8f,-2.85f),
		glm::vec3(-1.7f,-1.8f,-2.0f),
		glm::vec3(-1.7f,-1.8f,-1.15f),
		glm::vec3(-1.7f,-1.8f,-0.3f),
		glm::vec3(-1.7f, -1.8f, 0.15f),
		glm::vec3(-1.7f, -1.8f, 1.0f),
		glm::vec3(-1.7f, -1.8f, 1.85f),
		glm::vec3(-1.7f, -1.8f, 2.7f),
		glm::vec3(-1.7f, -1.8f, 3.55f),
		glm::vec3(-1.7f, -1.8f, 4.4f),

		//nextfloor
		glm::vec3(-1.7f,-1.8f, -3.83f),
		glm::vec3(-1.7f,-1.8f, -4.82f),
		glm::vec3(-1.7f,-1.8f, -5.81f),
		glm::vec3(-1.7f,-1.8f, -6.80f),
		glm::vec3(-1.7f,-1.8f, -7.79f),
		glm::vec3(-1.7f,-1.8f, -8.78f),

		glm::vec3(-2.7f,-1.8f, -3.83f),
		glm::vec3(-2.7f,-1.8f, -4.82f),
		glm::vec3(-2.7f,-1.8f, -5.81f),
		glm::vec3(-2.7f,-1.8f, -6.80f),
		glm::vec3(-2.7f,-1.8f, -7.79f),
		glm::vec3(-2.7f,-1.8f, -8.78f),

		glm::vec3(-0.7f,-1.8f, -3.83f),
		glm::vec3(-0.7f,-1.8f, -4.82f),
		glm::vec3(-0.7f,-1.8f, -5.81f),
		glm::vec3(-0.7f,-1.8f, -6.80f),
		glm::vec3(-0.7f,-1.8f, -7.79f),
		glm::vec3(-0.7f,-1.8f, -8.78f),

		glm::vec3(0.3f,-1.8f, -3.83f),
		glm::vec3(0.3f,-1.8f, -4.82f),
		glm::vec3(0.3f,-1.8f, -5.81f),
		glm::vec3(0.3f,-1.8f, -6.80f),
		glm::vec3(0.3f,-1.8f, -7.79f),
		glm::vec3(0.3f,-1.8f, -8.78f),

		glm::vec3(1.3f,-1.8f, -3.83f),
		glm::vec3(1.3f,-1.8f, -4.82f),
		glm::vec3(1.3f,-1.8f, -5.81f),
		glm::vec3(1.3f,-1.8f, -6.80f),
		glm::vec3(1.3f,-1.8f, -7.79f),
		glm::vec3(1.3f,-1.8f, -8.78f),

			glm::vec3(2.3f, -1.8f, -3.83f),
			glm::vec3(2.3f, -1.8f, -4.82f),
			glm::vec3(2.3f, -1.8f, -5.81f),
			glm::vec3(2.3f, -1.8f, -6.80f),
			glm::vec3(2.3f, -1.8f, -7.79f),
			glm::vec3(2.3f, -1.8f, -8.78f),

			//leftside

			glm::vec3(-3.7f, -1.8f, -3.70f),
			glm::vec3(-3.7f, -1.8f, -4.69f),
			glm::vec3(-3.7f, -1.8f, -5.68f),
			glm::vec3(-3.7f, -1.8f, -6.67f),
			glm::vec3(-3.7f, -1.8f, -7.66f),
			glm::vec3(-3.7f, -1.8f, -8.65f),

			glm::vec3(-4.7f, -1.8f, -3.70f),
			glm::vec3(-4.7f, -1.8f, -4.69f),
			glm::vec3(-4.7f, -1.8f, -5.68f),
			glm::vec3(-4.7f, -1.8f, -6.67f),
			glm::vec3(-4.7f, -1.8f, -7.66f),
			glm::vec3(-4.7f, -1.8f, -8.65f),

			glm::vec3(-5.7f, -1.8f, -3.70f),
			glm::vec3(-5.7f, -1.8f, -4.69f),
			glm::vec3(-5.7f, -1.8f, -5.68f),
			glm::vec3(-5.7f, -1.8f, -6.67f),
			glm::vec3(-5.7f, -1.8f, -7.66f),
			glm::vec3(-5.7f, -1.8f, -8.65f),

			glm::vec3(-6.7f, -1.8f, -3.70f),
			glm::vec3(-6.7f, -1.8f, -4.69f),
			glm::vec3(-6.7f, -1.8f, -5.68f),
			glm::vec3(-6.7f, -1.8f, -6.67f),
			glm::vec3(-6.7f, -1.8f, -7.66f),
			glm::vec3(-6.7f, -1.8f, -8.65f),

			glm::vec3(-7.7f, -1.8f, -3.70f),
			glm::vec3(-7.7f, -1.8f, -4.69f),
			glm::vec3(-7.7f, -1.8f, -5.68f),
			glm::vec3(-7.7f, -1.8f, -6.67f),
			glm::vec3(-7.7f, -1.8f, -7.66f),
			glm::vec3(-7.7f, -1.8f, -8.65f),

			glm::vec3(-8.7f, -1.8f, -3.70f),
			glm::vec3(-8.7f, -1.8f, -4.69f),
			glm::vec3(-8.7f, -1.8f, -5.68f),
			glm::vec3(-8.7f, -1.8f, -6.67f),
			glm::vec3(-8.7f, -1.8f, -7.66f),
			glm::vec3(-8.7f, -1.8f, -8.65f),

			glm::vec3(-9.7f, -1.8f, -3.70f),
			glm::vec3(-9.7f, -1.8f, -4.69f),
			glm::vec3(-9.7f, -1.8f, -5.68f),
			glm::vec3(-9.7f, -1.8f, -6.67f),
			glm::vec3(-9.7f, -1.8f, -7.66f),
			glm::vec3(-9.7f, -1.8f, -8.65f),

			glm::vec3(-10.7f, -1.8f, -3.70f),
			glm::vec3(-10.7f, -1.8f, -4.69f),
			glm::vec3(-10.7f, -1.8f, -5.68f),
			glm::vec3(-10.7f, -1.8f, -6.67f),
			glm::vec3(-10.7f, -1.8f, -7.66f),
			glm::vec3(-10.7f, -1.8f, -8.65f),

			glm::vec3(-11.7f, -1.8f, -3.70f),
			glm::vec3(-11.7f, -1.8f, -4.69f),
			glm::vec3(-11.7f, -1.8f, -5.68f),
			glm::vec3(-11.7f, -1.8f, -6.67f),
			glm::vec3(-11.7f, -1.8f, -7.66f),
			glm::vec3(-11.7f, -1.8f, -8.65f),

			glm::vec3(-12.7f, -1.8f, -3.70f),
			glm::vec3(-12.7f, -1.8f, -4.69f),
			glm::vec3(-12.7f, -1.8f, -5.68f),
			glm::vec3(-12.7f, -1.8f, -6.67f),
			glm::vec3(-12.7f, -1.8f, -7.66f),
			glm::vec3(-12.7f, -1.8f, -8.65f),

			glm::vec3(-13.7f, -1.8f, -3.70f),
			glm::vec3(-13.7f, -1.8f, -4.69f),
			glm::vec3(-13.7f, -1.8f, -5.68f),
			glm::vec3(-13.7f, -1.8f, -6.67f),
			glm::vec3(-13.7f, -1.8f, -7.66f),
			glm::vec3(-13.7f, -1.8f, -8.65f),

			//leftfloor
			glm::vec3(3.3f, -1.8f, -3.70f),
			glm::vec3(3.3f, -1.8f, -4.69f),
			glm::vec3(3.3f, -1.8f, -5.68f),
			glm::vec3(3.3f, -1.8f, -6.67f),
			glm::vec3(3.3f, -1.8f, -7.66f),
			glm::vec3(3.3f, -1.8f, -8.65f),

			glm::vec3(4.3f, -1.8f, -3.70f),
			glm::vec3(4.3f, -1.8f, -4.69f),
			glm::vec3(4.3f, -1.8f, -5.68f),
			glm::vec3(4.3f, -1.8f, -6.67f),
			glm::vec3(4.3f, -1.8f, -7.66f),
			glm::vec3(4.3f, -1.8f, -8.65f),

				glm::vec3(5.3f, -1.8f, -3.70f),
				glm::vec3(5.3f, -1.8f, -4.69f),
				glm::vec3(5.3f, -1.8f, -5.68f),
				glm::vec3(5.3f, -1.8f, -6.67f),
				glm::vec3(5.3f, -1.8f, -7.66f),
				glm::vec3(5.3f, -1.8f, -8.65f),

				glm::vec3(6.3f, -1.8f, -3.70f),
				glm::vec3(6.3f, -1.8f, -4.69f),
				glm::vec3(6.3f, -1.8f, -5.68f),
				glm::vec3(6.3f, -1.8f, -6.67f),
				glm::vec3(6.3f, -1.8f, -7.66f),
				glm::vec3(6.3f, -1.8f, -8.65f),

				glm::vec3(7.3f, -1.8f, -3.70f),
				glm::vec3(7.3f, -1.8f, -4.69f),
				glm::vec3(7.3f, -1.8f, -5.68f),
				glm::vec3(7.3f, -1.8f, -6.67f),
				glm::vec3(7.3f, -1.8f, -7.66f),
				glm::vec3(7.3f, -1.8f, -8.65f),

				glm::vec3(8.3f, -1.8f, -3.70f),
				glm::vec3(8.3f, -1.8f, -4.69f),
				glm::vec3(8.3f, -1.8f, -5.68f),
				glm::vec3(8.3f, -1.8f, -6.67f),
				glm::vec3(8.3f, -1.8f, -7.66f),
				glm::vec3(8.3f, -1.8f, -8.65f),

				glm::vec3(9.3f, -1.8f, -3.70f),
				glm::vec3(9.3f, -1.8f, -4.69f),
				glm::vec3(9.3f, -1.8f, -5.68f),
				glm::vec3(9.3f, -1.8f, -6.67f),
				glm::vec3(9.3f, -1.8f, -7.66f),
				glm::vec3(9.3f, -1.8f, -8.65f),

				glm::vec3(10.3f, -1.8f, -3.70f),
				glm::vec3(10.3f, -1.8f, -4.69f),
				glm::vec3(10.3f, -1.8f, -5.68f),
				glm::vec3(10.3f, -1.8f, -6.67f),
				glm::vec3(10.3f, -1.8f, -7.66f),
				glm::vec3(10.3f, -1.8f, -8.65f),

				glm::vec3(11.3f, -1.8f, -3.70f),
				glm::vec3(11.3f, -1.8f, -4.69f),
				glm::vec3(11.3f, -1.8f, -5.68f),
				glm::vec3(11.3f, -1.8f, -6.67f),
				glm::vec3(11.3f, -1.8f, -7.66f),
				glm::vec3(11.3f, -1.8f, -8.65f),

				glm::vec3(12.3f, -1.8f, -3.70f),
				glm::vec3(12.3f, -1.8f, -4.69f),
				glm::vec3(12.3f, -1.8f, -5.68f),
				glm::vec3(12.3f, -1.8f, -6.67f),
				glm::vec3(12.3f, -1.8f, -7.66f),
				glm::vec3(12.3f, -1.8f, -8.65f),
	};

	glm::vec3 wallPos[] = {
		//leftwall
		glm::vec3(-3.28f,-1.3f,-2.7f),
		glm::vec3(-3.28f,-1.3f,-1.7f),
		glm::vec3(-3.28f,-1.3f,-0.7f),
		glm::vec3(-3.28f,-1.3f,0.3f),
		glm::vec3(-3.28f,-1.3f,1.3f),
		glm::vec3(-3.28f,-1.3f,2.3f),
		glm::vec3(-3.28f,-1.3f,3.3f),
		glm::vec3(-3.28f,-1.3f,4.3f),


	glm::vec3(-3.28f,-0.3f,-2.7f),
		glm::vec3(-3.28f,-0.3f,-1.7f),
		glm::vec3(-3.28f,-0.3f,-0.7f),
		glm::vec3(-3.28f,-0.3f,0.3f),
		glm::vec3(-3.28f,-0.3f,1.3f),
		glm::vec3(-3.28f,-0.3f,2.3f),
		glm::vec3(-3.28f,-0.3f,3.3f),
		glm::vec3(-3.28f,-0.3f,4.3f),

		glm::vec3(-3.28f,0.7f,-2.7f),
		glm::vec3(-3.28f,0.7f,-1.7f),
		glm::vec3(-3.28f,0.7f,-0.7f),
		glm::vec3(-3.28f,0.7f,0.3f),
		glm::vec3(-3.28f,0.7f,1.3f),
		glm::vec3(-3.28f,0.7,2.3f),
		glm::vec3(-3.28f,0.7f,3.3f),
		glm::vec3(-3.28f,0.7f,4.3f),


		glm::vec3(-3.28f,1.7f,-2.7f),
		glm::vec3(-3.28f,1.7f,-1.7f),
		glm::vec3(-3.28f,1.7f,-0.7f),
		glm::vec3(-3.28f,1.7f,0.3f),
		glm::vec3(-3.28f,1.7f,1.3f),
		glm::vec3(-3.28f,1.7,2.3f),
		glm::vec3(-3.28f,1.7f,3.3f),
		glm::vec3(-3.28f,1.7f,4.3f),



		glm::vec3(-3.28f,2.7f,-2.7f),
		glm::vec3(-3.28f,2.7f,-1.7f),
		glm::vec3(-3.28f,2.7f,-0.7f),
		glm::vec3(-3.28f,2.7f,0.3f),
		glm::vec3(-3.28f,2.7f,1.3f),
		glm::vec3(-3.28f,2.7f,2.3f),
		glm::vec3(-3.28f,2.7f,3.3f),
		glm::vec3(-3.28f,2.7f,4.3f),

		//rightwall
		glm::vec3(2.87f,-1.3f,-2.7f),
		glm::vec3(2.87f,-1.3f,-1.7f),
		glm::vec3(2.87f,-1.3f,-0.7f),
		glm::vec3(2.87f,-1.3f,0.3f),
		glm::vec3(2.87f,-1.3f,1.3f),
		glm::vec3(2.87f,-1.3f,2.3f),
		glm::vec3(2.87f,-1.3f,3.3f),
		glm::vec3(2.87f,-1.3f,4.3f),


		glm::vec3(2.87f,-0.3f,-2.7f),
		glm::vec3(2.87f,-0.3f,-1.7f),
		glm::vec3(2.87f,-0.3f,-0.7f),
		glm::vec3(2.87f,-0.3f,0.3f),
		glm::vec3(2.87f,-0.3f,1.3f),
		glm::vec3(2.87f,-0.3f,2.3f),
		glm::vec3(2.87f,-0.3f,3.3f),
		glm::vec3(2.87f,-0.3f,4.3f),

		glm::vec3(2.87f,0.7f,-2.7f),
		glm::vec3(2.87f,0.7f,-1.7f),
		glm::vec3(2.87f,0.7f,-0.7f),
		glm::vec3(2.87f,0.7f,0.3f),
		glm::vec3(2.87f,0.7f,1.3f),
		glm::vec3(2.87f,0.7,2.3f),
		glm::vec3(2.87f,0.7f,3.3f),
		glm::vec3(2.87f,0.7f,4.3f),


		glm::vec3(2.87f,1.7f,-2.7f),
		glm::vec3(2.87f,1.7f,-1.7f),
		glm::vec3(2.87f,1.7f,-0.7f),
		glm::vec3(2.87f,1.7f,0.3f),
		glm::vec3(2.87f,1.7f,1.3f),
		glm::vec3(2.87f,1.7,2.3f),
		glm::vec3(2.87f,1.7f,3.3f),
		glm::vec3(2.87f,1.7f,4.3f),



		glm::vec3(2.87f,2.7f,-2.7f),
		glm::vec3(2.87f,2.7f,-1.7f),
		glm::vec3(2.87f,2.7f,-0.7f),
		glm::vec3(2.87f,2.7f,0.3f),
		glm::vec3(2.87f,2.7f,1.3f),
		glm::vec3(2.87f,2.7f,2.3f),
		glm::vec3(2.87f,2.7f,3.3f),
		glm::vec3(2.87f,2.7f,4.3f),
	

};
//=============================================
glm::vec3 wallPos2[] = {
	//left sector
	//leftwall
	glm::vec3(-4.38f,-0.8f,-3.1f),
	glm::vec3(-4.38f,1.2f,-3.1f),
	glm::vec3(-4.38f,3.2f,-3.1f),

	glm::vec3(-6.38f,-0.8f,-3.1f),
	glm::vec3(-6.38f,1.2f,-3.1f),
	glm::vec3(-6.38f,3.2f,-3.1f),

	glm::vec3(-8.38f,-0.8f,-3.1f),
	glm::vec3(-8.38f,1.2f,-3.1f),
	glm::vec3(-8.38f,3.2f,-3.1f),

	glm::vec3(-10.38f,-0.8f,-3.1f),
	glm::vec3(-10.38f,1.2f,-3.1f),
	glm::vec3(-10.38f,3.2f,-3.1f),

	glm::vec3(-12.38f,-0.8f,-3.1f),
	glm::vec3(-12.38f,1.2f,-3.1f),
	glm::vec3(-12.38f,3.2f,-3.1f),

	//rightwall
	glm::vec3(-2.38f,-0.8f,-9.1f),
	glm::vec3(-2.38f,1.2f,-9.1f),
	glm::vec3(-2.38f,3.2f,-9.1f),

	glm::vec3(-4.38f,-0.8f,-9.1f),
	glm::vec3(-4.38f,1.2f,-9.1f),
	glm::vec3(-4.38f,3.2f,-9.1f),

	glm::vec3(-6.38f,-0.8f,-9.1f),
	glm::vec3(-6.38f,1.2f,-9.1f),
	glm::vec3(-6.38f,3.2f,-9.1f),

	glm::vec3(-8.38f,-0.8f,-9.1f),
	glm::vec3(-8.38f,1.2f,-9.1f),
	glm::vec3(-8.38f,3.2f,-9.1f),

	glm::vec3(-10.38f,-0.8f,-9.1f),
	glm::vec3(-10.38f,1.2f,-9.1f),
	glm::vec3(-10.38f,3.2f,-9.1f),

	glm::vec3(-12.38f,-0.8f,-9.1f),
	glm::vec3(-12.38f,1.2f,-9.1f),
	glm::vec3(-12.38f,3.2f,-9.1f),


	//right Sector
	//rightwall
	glm::vec3(3.97f,-0.8f,-3.1f),
	glm::vec3(3.97f,1.2f,-3.1f),
	glm::vec3(3.97f,3.2f,-3.1f),

	glm::vec3(4.97f,-0.8f,-3.1f),
	glm::vec3(4.97,1.2f,-3.1f),
	glm::vec3(4.97,3.2f,-3.1f),

	glm::vec3(5.97f,-0.8f,-3.1f),
	glm::vec3(5.97f,1.2f,-3.1f),
	glm::vec3(5.97f,3.2f,-3.1f),

	glm::vec3(6.97f,-0.8f,-3.1f),
	glm::vec3(6.97f,1.2f,-3.1f),
	glm::vec3(6.97f,3.2f,-3.1f),

	glm::vec3(7.97f,-0.8f,-3.1f),
	glm::vec3(7.97f,1.2f,-3.1f),
	glm::vec3(7.97f,3.2f,-3.1f),

	glm::vec3(8.97f,-0.8f,-3.1f),
	glm::vec3(8.97f,1.2f,-3.1f),
	glm::vec3(8.97f,3.2f,-3.1f),

	glm::vec3(9.97f,-0.8f,-3.1f),
	glm::vec3(9.97f,1.2f,-3.1f),
	glm::vec3(9.97f,3.2f,-3.1f),

	glm::vec3(10.97f,-0.8f,-3.1f),
	glm::vec3(10.97f,1.2f,-3.1f),
	glm::vec3(10.97f,3.2f,-3.1f),

	//right Sector
	//leftwall
	glm::vec3(2.97f,-0.8f,-9.1f),
	glm::vec3(2.97f,1.2f,-9.1f),
	glm::vec3(2.97f,3.2f,-9.1f),

	glm::vec3(3.97f,-0.8f,-9.1f),
	glm::vec3(3.97f,1.2f,-9.1f),
	glm::vec3(3.97f,3.2f,-9.1f),

	glm::vec3(4.97f,-0.8f,-9.1f),
	glm::vec3(4.97,1.2f,-9.1f),
	glm::vec3(4.97,3.2f,-9.1f),

	glm::vec3(5.97f,-0.8f,-9.1f),
	glm::vec3(5.97f,1.2f,-9.1f),
	glm::vec3(5.97f,3.2f,-9.1f),

	glm::vec3(6.97f,-0.8f,-9.1f),
	glm::vec3(6.97f,1.2f,-9.1f),
	glm::vec3(6.97f,3.2f,-9.1f),

	glm::vec3(7.97f,-0.8f,-9.1f),
	glm::vec3(7.97f,1.2f,-9.1f),
	glm::vec3(7.97f,3.2f,-9.1f),

	glm::vec3(8.97f,-0.8f,-9.1f),
	glm::vec3(8.97f,1.2f,-9.1f),
	glm::vec3(8.97f,3.2f,-9.1f),

	glm::vec3(9.97f,-0.8f,-9.1f),
	glm::vec3(9.97f,1.2f,-9.1f),
	glm::vec3(9.97f,3.2f,-9.1f),

	glm::vec3(10.97f,-0.8f,-9.1f),
	glm::vec3(10.97f,1.2f,-9.1f),
	glm::vec3(10.97f,3.2f,-9.1f),


};
//============================================

	glm::vec3 roofPos[] = {
		glm::vec3(-2.73f,3.3f,-2.7f),
		glm::vec3(-2.73f,3.3f,-1.7f),
		glm::vec3(-2.73f,3.3f,-0.7f),
		glm::vec3(-2.73f,3.3f,0.3f),
		glm::vec3(-2.73f,3.3f,1.3f),
		glm::vec3(-2.73f,3.3f,2.3f),
		glm::vec3(-2.73f,3.3f,3.3f),
		glm::vec3(-2.73f,3.3f,4.3f),
		//-----
		glm::vec3(-1.73f,3.3f,-2.7f),
		glm::vec3(-1.73f,3.3f,-1.7f),
		glm::vec3(-1.73f,3.3f,-0.7f),
		glm::vec3(-1.73f,3.3f,0.3f),
		glm::vec3(-1.73f,3.3f,1.3f),
		glm::vec3(-1.73f,3.3f,2.3f),
		glm::vec3(-1.73f,3.3f,3.3f),
		glm::vec3(-1.73f,3.3f,4.3f),
        //-----------
		glm::vec3(-0.73f,3.3f,-2.7f),
		glm::vec3(-0.73f,3.3f,-1.7f),
		glm::vec3(-0.73f,3.3f,-0.7f),
		glm::vec3(-0.73f,3.3f,0.3f),
		glm::vec3(-0.73f,3.3f,1.3f),
		glm::vec3(-0.73f,3.3f,2.3f),
		glm::vec3(-0.73f,3.3f,3.3f),
		glm::vec3(-0.73f,3.3f,4.3f),
		//-------
		glm::vec3(0.27f,3.3f,-2.7f),
		glm::vec3(0.27f,3.3f,-1.7f),
		glm::vec3(0.27f,3.3f,-0.7f),
		glm::vec3(0.27f,3.3f,0.3f),
		glm::vec3(0.27f,3.3f,1.3f),
		glm::vec3(0.27f,3.3f,2.3f),
		glm::vec3(0.27f,3.3f,3.3f),
		glm::vec3(0.27f,3.3f,4.3f),
        //-----
		glm::vec3(1.27f,3.3f,-2.7f),
		glm::vec3(1.27f,3.3f,-1.7f),
		glm::vec3(1.27f,3.3f,-0.7f),
		glm::vec3(1.27f,3.3f,0.3f),
		glm::vec3(1.27f,3.3f,1.3f),
		glm::vec3(1.27f,3.3f,2.3f),
		glm::vec3(1.27f,3.3f,3.3f),
		glm::vec3(1.27f,3.3f,4.3f),
		//
		glm::vec3(2.27f,3.3f,-2.7f),
		glm::vec3(2.27f,3.3f,-1.7f),
		glm::vec3(2.27f,3.3f,-0.7f),
		glm::vec3(2.27f,3.3f,0.3f),
		glm::vec3(2.27f,3.3f,1.3f),
		glm::vec3(2.27f,3.3f,2.3f),
		glm::vec3(2.27f,3.3f,3.3f),
		glm::vec3(2.27f,3.3f,4.3f),
		//leftroof

		glm::vec3(-3.88f,4.3f,-3.7f),
		glm::vec3(-3.88f,4.3f,-4.7f),
		glm::vec3(-3.88f,4.3f,-5.7f),
		glm::vec3(-3.88f,4.3f,-6.7f),
		glm::vec3(-3.88f,4.3f,-7.7f),
		glm::vec3(-3.88f,4.3f,-8.7f),

		glm::vec3(-4.88f,4.3f,-3.7f),
		glm::vec3(-4.88f,4.3f,-4.7f),
		glm::vec3(-4.88f,4.3f,-5.7f),
		glm::vec3(-4.88f,4.3f,-6.7f),
		glm::vec3(-4.88f,4.3f,-7.7f),
		glm::vec3(-4.88f,4.3f,-8.7f),

		
		glm::vec3(-6.88f,4.3f,-3.7f),
		glm::vec3(-6.88f,4.3f,-4.7f),
		glm::vec3(-6.88f,4.3f,-5.7f),
		glm::vec3(-6.88f,4.3f,-6.7f),
		glm::vec3(-6.88f,4.3f,-7.7f),
		glm::vec3(-6.88f,4.3f,-8.7f),

		glm::vec3(-5.88f,4.3f,-3.7f),
		glm::vec3(-5.88f,4.3f,-4.7f),
		glm::vec3(-5.88f,4.3f,-5.7f),
		glm::vec3(-5.88f,4.3f,-6.7f),
		glm::vec3(-5.88f,4.3f,-7.7f),
		glm::vec3(-5.88f,4.3f,-8.7f),

		glm::vec3(-7.88f,4.3f,-3.7f),
		glm::vec3(-7.88f,4.3f,-4.7f),
		glm::vec3(-7.88f,4.3f,-5.7f),
		glm::vec3(-7.88f,4.3f,-6.7f),
		glm::vec3(-7.88f,4.3f,-7.7f),
		glm::vec3(-7.88f,4.3f,-8.7f),

		glm::vec3(-8.88f,4.3f,-3.7f),
		glm::vec3(-8.88f,4.3f,-4.7f),
		glm::vec3(-8.88f,4.3f,-5.7f),
		glm::vec3(-8.88f,4.3f,-6.7f),
		glm::vec3(-8.88f,4.3f,-7.7f),
		glm::vec3(-8.88f,4.3f,-8.7f),

		glm::vec3(-9.88f,4.3f,-3.7f),
		glm::vec3(-9.88f,4.3f,-4.7f),
		glm::vec3(-9.88f,4.3f,-5.7f),
		glm::vec3(-9.88f,4.3f,-6.7f),
		glm::vec3(-9.88f,4.3f,-7.7f),
		glm::vec3(-9.88f,4.3f,-8.7f),

			glm::vec3(-10.88f, 4.3f, -3.7f),
			glm::vec3(-10.88f, 4.3f, -4.7f),
			glm::vec3(-10.88f, 4.3f, -5.7f),
			glm::vec3(-10.88f, 4.3f, -6.7f),
			glm::vec3(-10.88f, 4.3f, -7.7f),
			glm::vec3(-10.88f, 4.3f, -8.7f),

			glm::vec3(-11.88f, 4.3f, -3.7f),
			glm::vec3(-11.88f, 4.3f, -4.7f),
			glm::vec3(-11.88f, 4.3f, -5.7f),
			glm::vec3(-11.88f, 4.3f, -6.7f),
			glm::vec3(-11.88f, 4.3f, -7.7f),
			glm::vec3(-11.88f, 4.3f, -8.7f),

			glm::vec3(-12.88f, 4.3f, -3.7f),
			glm::vec3(-12.88f, 4.3f, -4.7f),
			glm::vec3(-12.88f, 4.3f, -5.7f),
			glm::vec3(-12.88f, 4.3f, -6.7f),
			glm::vec3(-12.88f, 4.3f, -7.7f),
			glm::vec3(-12.88f, 4.3f, -8.7f),

			//rigtroof
			glm::vec3(3.47f, 4.3f, -3.7f),
			glm::vec3(3.47f, 4.3f, -4.7f),
			glm::vec3(3.47f, 4.3f, -5.7f),
			glm::vec3(3.47f, 4.3f, -6.7f),
			glm::vec3(3.47f, 4.3f, -7.7f),
			glm::vec3(3.47f, 4.3f, -8.7f),

			glm::vec3(4.47f, 4.3f, -3.7f),
			glm::vec3(4.47f, 4.3f, -4.7f),
			glm::vec3(4.47f, 4.3f, -5.7f),
			glm::vec3(4.47f, 4.3f, -6.7f),
			glm::vec3(4.47f, 4.3f, -7.7f),
			glm::vec3(4.47f, 4.3f, -8.7f),


			glm::vec3(5.47f, 4.3f, -3.7f),
			glm::vec3(5.47f, 4.3f, -4.7f),
			glm::vec3(5.47f, 4.3f, -5.7f),
			glm::vec3(5.47f, 4.3f, -6.7f),
			glm::vec3(5.47f, 4.3f, -7.7f),
			glm::vec3(5.47f, 4.3f, -8.7f),

			glm::vec3(6.47f, 4.3f, -3.7f),
			glm::vec3(6.47f, 4.3f, -4.7f),
			glm::vec3(6.47f, 4.3f, -5.7f),
			glm::vec3(6.47f, 4.3f, -6.7f),
			glm::vec3(6.47f, 4.3f, -7.7f),
			glm::vec3(6.47f, 4.3f, -8.7f),

			glm::vec3(7.47f, 4.3f, -3.7f),
			glm::vec3(7.47f, 4.3f, -4.7f),
			glm::vec3(7.47f, 4.3f, -5.7f),
			glm::vec3(7.47f, 4.3f, -6.7f),
			glm::vec3(7.47f, 4.3f, -7.7f),
			glm::vec3(7.47f, 4.3f, -8.7f),

			glm::vec3(8.47f, 4.3f, -3.7f),
			glm::vec3(8.47f, 4.3f, -4.7f),
			glm::vec3(8.47f, 4.3f, -5.7f),
			glm::vec3(8.47f, 4.3f, -6.7f),
			glm::vec3(8.47f, 4.3f, -7.7f),
			glm::vec3(8.47f, 4.3f, -8.7f),

			glm::vec3(9.47f, 4.3f, -3.7f),
			glm::vec3(9.47f, 4.3f, -4.7f),
			glm::vec3(9.47f, 4.3f, -5.7f),
			glm::vec3(9.47f, 4.3f, -6.7f),
			glm::vec3(9.47f, 4.3f, -7.7f),
			glm::vec3(9.47f, 4.3f, -8.7f),

			glm::vec3(10.47f, 4.3f, -3.7f),
			glm::vec3(10.47f, 4.3f, -4.7f),
			glm::vec3(10.47f, 4.3f, -5.7f),
			glm::vec3(10.47f, 4.3f, -6.7f),
			glm::vec3(10.47f, 4.3f, -7.7f),
			glm::vec3(10.47f, 4.3f, -8.7f),

			glm::vec3(11.47f, 4.3f, -3.7f),
			glm::vec3(11.47f, 4.3f, -4.7f),
			glm::vec3(11.47f, 4.3f, -5.7f),
			glm::vec3(11.47f, 4.3f, -6.7f),
			glm::vec3(11.47f, 4.3f, -7.7f),
			glm::vec3(11.47f, 4.3f, -8.7f),

			glm::vec3(12.47f, 4.3f, -3.7f),
			glm::vec3(12.47f, 4.3f, -4.7f),
			glm::vec3(12.47f, 4.3f, -5.7f),
			glm::vec3(12.47f, 4.3f, -6.7f),
			glm::vec3(12.47f, 4.3f, -7.7f),
			glm::vec3(12.47f, 4.3f, -8.7f),

		

};

glm::vec3 doorPos[]={

	glm::vec3(0.3f, 0.8f, -9.8f),
	glm::vec3(10.97f,1.2f,-9.1f)
};



	
	
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO
	//-----------------------------------------------------------------
	GLuint VAO2, VBO2, EBO2;
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO


	GLuint VAO3, VBO3;
	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	//glGenBuffers(1, &EBO3);

	glBindVertexArray(VAO3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCubeVerts), texCubeVerts, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO
	//--------------------------------------------------------------------

	GLuint VAO4, VBO4;
	glGenVertexArrays(1, &VAO4);
	glGenBuffers(1, &VBO4);
	//glGenBuffers(1, &EBO3);

	glBindVertexArray(VAO4);
	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCubeVerts), texCubeVerts, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO
	//---------------------------------------------------
	GLuint VAO5, VBO5;
	glGenVertexArrays(1, &VAO5);
	glGenBuffers(1, &VBO5);
	//glGenBuffers(1, &EBO3);

	glBindVertexArray(VAO5);
	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(WALLCubeVerts), WALLCubeVerts, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO
//============================================================
	GLuint VAO6, VBO6;
	glGenVertexArrays(1, &VAO6);
	glGenBuffers(1, &VBO6);
	//glGenBuffers(1, &EBO3);

	glBindVertexArray(VAO6);
	glBindBuffer(GL_ARRAY_BUFFER, VBO6);
	glBufferData(GL_ARRAY_BUFFER, sizeof(WALLCubeVerts), WALLCubeVerts, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

						  // Load and create a texture 
	GLuint texture;
	GLuint texture2;
	GLuint texture3;
	GLuint texture4;
	GLuint texture5;

    glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image("floor2.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	//------------------------------------------------------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps
	int widthb, heightb;
	 unsigned char*images = SOIL_load_image("design.jpg", &widthb, &heightb, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthb, heightb, 0, GL_RGB, GL_UNSIGNED_BYTE, images);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(images);
	glBindTexture(GL_TEXTURE_2D, 0);
	//------------------------------------------------------
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps
	int widthc, heightc;
	unsigned char* imager = SOIL_load_image("ironwal.png", &widthc, &heightc, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthc, heightc, 0, GL_RGB, GL_UNSIGNED_BYTE, imager);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(imager);
	glBindTexture(GL_TEXTURE_2D, 0);
	//----------------------------------------------------------
	glGenTextures(1, &texture4);
	glBindTexture(GL_TEXTURE_2D, texture4); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps
	int widthd, heightd;
	unsigned char*imager2 = SOIL_load_image("ironwal.png", &widthd, &heightd, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthd, heightd, 0, GL_RGB, GL_UNSIGNED_BYTE, imager2);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(imager2);
	glBindTexture(GL_TEXTURE_2D, 0);
	//----------------------------------------------------------
	glGenTextures(1, &texture5);
	glBindTexture(GL_TEXTURE_2D, texture5); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps
	int widthe, heighte;
	unsigned char*imager3 = SOIL_load_image("DOOR2.png", &widthe, &heighte, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthe, heighte, 0, GL_RGB, GL_UNSIGNED_BYTE, imager3);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(imager3);
	glBindTexture(GL_TEXTURE_2D, 0);

									 // ===================GAME LOOP===============================================//
	while (!glfwWindowShouldClose(window))
	{
		glfwSetKeyCallback(window, key_callback);
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		glfwPollEvents();
		//glfwSetKeyCallback(window,key_callback);
		do_movement();
		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Bind Texture
	
		     

				
              // glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture3"), 0);


			// Activate shader
		glUseProgram(shaderProgram);
		/*GLfloat radius = -10.0f;
		GLfloat camX = sin(glfwGetTime()) * radius;
		GLfloat camZ = cos(glfwGetTime()) * radius;
		glm::mat4 view;
		view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));*/
		//glm::mat4 model;
        glm::mat4 view;
		glm::mat4 projection;
		//model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f,-10.0f));
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projection = glm::perspective( glm::radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		// Get their uniform location
		GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
		GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
		GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
		//pass them to the shaders
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		

		// Draw container
		glBindVertexArray(VAO);
		
		for (GLuint i = 0; i <134; i++)
		{
			
			// Calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			//model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
			//if (i == 3) {
				
				//model = glm::scale(model, glm::vec3(1.2f,1.2f, 1.2f));
			//}
		//	GLfloat angle = 20.0f + i;
			//model = glm::rotate(model, angle*(GLfloat)glfwGetTime()*0.05f, glm::vec3(1.0f*i, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			//glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, texture);
			//glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture1"), 0);
			
			//glDrawArrays(GL_TRIANGLES, 0, 36);
			
			//glBindTexture(GL_TEXTURE_2D, 0);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture2);
			glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture2"), 0);

			glDrawArrays(GL_TRIANGLES, 0, 36);

			glBindTexture(GL_TEXTURE_2D, 0);



		}
		glBindVertexArray(0);
		//======================================================
		glBindVertexArray(VAO6);

		for (GLuint i = 0; i <1; i++)
		{

			// Calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model2;
			model2 = glm::translate(model2, doorPos[i]);
			//model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
			//if (i == 3) {

			model2 = glm::scale(model2, glm::vec3(3.5f,5.3f, 1.2f));
			//}
			
			//	GLfloat angle = 20.0f + i;
			//model = glm::rotate(model, angle*(GLfloat)glfwGetTime()*0.05f, glm::vec3(1.0f*i, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));

			//glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, texture);
			//glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture1"), 0);

			//glDrawArrays(GL_TRIANGLES, 0, 36);

			//glBindTexture(GL_TEXTURE_2D, 0);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture5);
			glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture5"), 0);

			glDrawArrays(GL_TRIANGLES, 0, 36);

			glBindTexture(GL_TEXTURE_2D, 0);
          }
		glBindVertexArray(0);
		//======================================================

		glBindVertexArray(VAO2);
		for (GLuint i = 0; i <223; i++) {
			glm::mat4 models;
			models = glm::translate(models, texPos[i]);
			models = glm::scale(models, glm::vec3(1.0f, 0.0f, 1.0f));
			models = glm::rotate(models,-55.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			//if (i == 60){
				
				//models = glm::scale(models, glm::vec3(2.0f, 2.0f, 2.0f));
			//}
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(models));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);
			glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture1"), 0);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
          glBindVertexArray(0);
		  //========================================================================================

		  glBindVertexArray(VAO3);
		 for (GLuint i = 0; i <81; i++) {
			  glm::mat4 models;
			  models = glm::translate(models, wallPos[i]);
			  models = glm::scale(models, glm::vec3(0.2f, 1.0f, 1.0f));
			 // models = glm::rotate(models, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
			  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(models));
			  glActiveTexture(GL_TEXTURE0);
			  glBindTexture(GL_TEXTURE_2D, texture3);
			  glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture3"), 0);

			  glDrawArrays(GL_TRIANGLES, 0, 36);

			  glBindTexture(GL_TEXTURE_2D, 0);
		  }
		  glBindVertexArray(0);
		


		//---------------------------------------------------------------------------------------------------------------
		
		  glBindVertexArray(VAO4);
		  for (GLuint i = 0; i <162; i++) {
			  glm::mat4 models;
			  models = glm::translate(models, roofPos[i]);
			  models = glm::scale(models, glm::vec3(1.0f, 0.2f, 1.0f));
			  //if (i > 39 && i <= 48) {
				//  models = glm::scale(models, glm::vec3(1.7f, 1.0f, 1.0f));
			  //}
			  // models = glm::rotate(models, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
			  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(models));
			  glActiveTexture(GL_TEXTURE0);
			  glBindTexture(GL_TEXTURE_2D, texture4);
			  glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture4"), 0);

			  glDrawArrays(GL_TRIANGLES, 0, 36);

			  glBindTexture(GL_TEXTURE_2D, 0);
		  }
		  glBindVertexArray(0);

      //----------------------------------------------------------------------------------------------
		  glBindVertexArray(VAO5);
		  for (GLuint i = 0; i <84; i++) {
			  glm::mat4 models;
			  models = glm::translate(models, wallPos2[i]);
			  models = glm::scale(models, glm::vec3(2.0f, 2.0f, 0.2f));
			 // if (i > 39 && i <= 48) {
				 // models = glm::scale(models, glm::vec3(1.7f, 1.0f, 1.0f));
			 // }
			  // models = glm::rotate(models, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
			  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(models));
			  glActiveTexture(GL_TEXTURE0);
			  glBindTexture(GL_TEXTURE_2D, texture4);
			  glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture4"), 0);

			  glDrawArrays(GL_TRIANGLES, 0, 36);

			  glBindTexture(GL_TEXTURE_2D, 0);
		  }
		  glBindVertexArray(0);


	// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteVertexArrays(1, &VAO3);
	glDeleteBuffers(1, &VBO3);
	glDeleteVertexArrays(1, &VAO4);
	glDeleteBuffers(1, &VBO4);
	glDeleteBuffers(1, &EBO2);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);


	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void do_movement() {

	GLfloat cameraSpeed = 5.0f*deltaTime;
	if (keys[GLFW_KEY_W])
		cameraPos += cameraSpeed*cameraFront;

	if (keys[GLFW_KEY_S])
		cameraPos -= cameraSpeed*cameraFront;

	if (keys[GLFW_KEY_A])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp))*cameraSpeed;

	if (keys[GLFW_KEY_D])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp))*cameraSpeed;
	if (keys[GLFW_KEY_SPACE])
		cameraPos += cameraSpeed*cameraUp;
	if(keys[GLFW_KEY_B])
		cameraPos -= cameraSpeed*cameraUp;


		
}
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.5f;	// Change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	//if (pitch > 89.0f)
	//	pitch = 89.0f;
	//if (pitch < -89.0f)
		//pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}