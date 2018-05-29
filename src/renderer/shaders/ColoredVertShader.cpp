#include "ColoredVertShader.h"
#include "VAO.h"

const char* ColoredVertShader::vertShaderSrc = 
	"#version 120\n										"\
	"attribute vec4 position;\n							"\
	"attribute vec4 inColor;\n							"\
	"attribute vec2 inUV;\n								"\
	"varying vec4 colorV;\n								"\
	"varying vec2 uvOut;\n								"\
	"void main(){\n										"\
	"	colorV = inColor;\n								"\
	"	gl_Position = gl_ModelViewProjectionMatrix * position;\n						"\
	"	uvOut = inUV;\n									"\
	"}\0												";

const char* ColoredVertShader::fragShaderSrc = 
	"#version 120\n										"\
	"varying vec4 colorV;\n								"\
	"varying vec2 uvOut;\n								"\
	"uniform sampler2D tex;\n							"\
	"void main(){\n										"\
	"	gl_FragColor = colorV;\n						"\
	"}\0												";

ColoredVertShader::ColoredVertShader() {
	m_textureUniform = 0;
	m_program = 0;
}

ColoredVertShader::~ColoredVertShader() {

}

bool ColoredVertShader::LoadShaders() {
    bool success = true;
	char buf[1024];
	int length = 0;
	GLuint vertShader, fragShader;
	m_program = glCreateProgram();
	vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
	glCompileShader(vertShader);
	GLint status;
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &status);
	if (!status) {
		glGetShaderInfoLog(vertShader, 1024, &length, buf);
		success = false;
	}
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
	if (!status) {
		glGetShaderInfoLog(fragShader, 1024, &length, buf);
		success = false;
	}
	glAttachShader(m_program, vertShader);
	glAttachShader(m_program, fragShader);
	glBindAttribLocation(m_program, IVertexArrayObject::VertAttrib::ATTRIB_VERTEX, "position");
	glBindAttribLocation(m_program, IVertexArrayObject::VertAttrib::ATTRIB_COLOR, "inColor");
	glBindAttribLocation(m_program, IVertexArrayObject::VertAttrib::ATTRIB_UV, "inUV");
	glLinkProgram(m_program);
	GLint linkSuccess;
	glGetProgramiv(m_program, GL_LINK_STATUS, &linkSuccess);
	if (!linkSuccess) {
		//error
		success = false;
	}
	m_textureUniform = glGetUniformLocation(m_program, "tex");
	//if (vertShader) {
	//	glDetachShader(m_glDefaultProgram, vertShader);
	//	glDeleteShader(vertShader);
	//}
	//if (fragShader) {
	//	glDetachShader(m_glDefaultProgram, fragShader);
	//	glDeleteShader(fragShader);
	//}
	return success;
}

void ColoredVertShader::Activate() {
	glUseProgram(m_program);
}

void ColoredVertShader::Deactivate() {
	glUseProgram(0);
}