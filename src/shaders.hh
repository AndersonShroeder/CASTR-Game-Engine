// Vertex Shader source code
const char *vertexShaderSource = "#version 330 core\n"
								 "layout (location = 0) in vec3 aPos;\n"
								 "layout (location = 1) in vec3 aColor;\n"
								 "out vec3 vertexColor;\n"
								 "void main()\n"
								 "{\n"
								 "   gl_Position = vec4(aPos, 1.0);\n"
								 "	vertexColor = aColor;\n"
								 "}\0";
// Fragment Shader source code
const char *fragmentShaderSource = "#version 330 core\n"
								   "in vec3 vertexColor;\n"
								   "out vec4 FragColor;\n"
								   "void main()\n"
								   "{\n"
								   "   FragColor = vec4(vertexColor, 1.0);\n"
								   "}\n\0";