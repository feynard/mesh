#include "graphics.hpp"

using namespace std;

// Read file into a string
static char* ReadFile(const char* file)
{
    FILE* fp = fopen(file, "r");

    if (fp == 0)
        return 0;

    fseek(fp, 0L, SEEK_END);

    long size = ftell(fp);
    char* buf = new char[size + 1];

    fseek(fp, 0L, SEEK_SET);
    fread(buf, 1, size, fp);

    buf[size] = '\0';

    fclose(fp);

    return buf;
}


// Create a GLSL program object from vertex and fragment shader files
GLuint ShaderInit(const char* vertex_shader, const char* fragment_shader)
{
    struct Shader {
        const char* filename;
	    GLenum type;
        GLchar* source;
    } shaders[2] = {
        {vertex_shader, GL_VERTEX_SHADER, NULL},
	    {fragment_shader, GL_FRAGMENT_SHADER, NULL}
    };

    GLuint program = glCreateProgram();

    for (int i = 0; i < 2; ++i ) {
        Shader& s = shaders[i];
        s.source = ReadFile(s.filename);

        if (shaders[i].source == NULL) {
            cerr << "Failed to read: " << s.filename << endl;
            exit(EXIT_FAILURE);
        }

    	GLuint shader = glCreateShader(s.type);
    	glShaderSource(shader, 1, (const GLchar**) &s.source, NULL);
    	glCompileShader(shader);

    	GLint compiled;
    	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

        if (!compiled) {
            cerr << "Failed to compile: " << s.filename << endl;
            GLint logSize;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
            char* logMsg = new char[logSize];
            glGetShaderInfoLog(shader, logSize, NULL, logMsg);
            cerr << logMsg << endl;
            delete [] logMsg;
	        exit(EXIT_FAILURE);
        }

	    delete[] s.source;

	    glAttachShader(program, shader);
    }

    glLinkProgram(program);

    // Link error check

    GLint  linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
    	cerr << "Shader program failed to link" << endl;
    	GLint logSize;
    	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
    	char* logMsg = new char[logSize];
    	glGetProgramInfoLog(program, logSize, NULL, logMsg);
    	cerr << logMsg << endl;
    	delete [] logMsg;

    	exit(EXIT_FAILURE);
    }

    // Use program object
    glUseProgram(program);

    return program;
}
