#define GLEW_STATIC
#include<gl\glew.h>
#include<GLFW\glfw3.h>
#include<SOIL.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

const GLchar* vertexShaderSource2 = "#version 330 core\n"
"layout(location = 0) in vec3 position;\n"
//"layout(location = 1) in vec3 color;\n"
"layout(location = 2) in vec2 texCoord;\n"

//"out vec3 ourColor;\n"
"out vec2 TexCoord;\n"
"uniform mat4 model; \n"
"uniform mat4 view; \n"
"uniform mat4 projection; \n"

"void main()\n"
"{\n"
"gl_Position= projection*view*model*vec4(position, 1.0f); \n"
//"ourColor = color; \n"
// We swap the y-axis by substracing our coordinates from 1. This is done because most images have the top y-axis inversed with OpenGL's top y-axis.
// TexCoord = texCoord;
"TexCoord = vec2(texCoord.x, 1.0 - texCoord.y); \n"
"}\0";
const GLchar* fragmentShaderSource2 = "#version 330 core\n"

//"in vec3 ourColor; \n"
"in vec2 TexCoord; \n"
"out vec4 colorer; \n"

"out vec4 color; \n"

// Texture samplers
//"uniform sampler2D ourTexture1; \n"
"uniform sampler2D ourTexture2; \n"
//"uniform sampler2D ourTexture3; \n"
"uniform vec3 objectColor; \n"
"uniform vec3 lightColor; \n"

"void main()\n"
"{\n"

// Linearly interpolate between both textures (second texture is only slightly combined)
"color = texture(ourTexture2, TexCoord);\n"
//"color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.5);\n"
//"colorer = texture(ourTexture2, TexCoord); \n"
//"color= vec4(lightColor*objectColor, 1.0f); \n"

"}\n\0";

