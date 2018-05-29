#include "NormalShader.h"
#include "VAO.h"

const char* NormalShader::vertShaderSrc = 
	"#version 120\n										"\
	"attribute vec4 position;\n							"\
	"attribute vec2 inUV;\n								"\
	"attribute vec3 inNormal;\n							"\
	"varying vec3 normalV;\n							"\
	"varying vec2 uvOut;\n								"\
	"void main(){\n										"\
	"	normalV = normalize(gl_NormalMatrix * inNormal);\n								"\
	"	gl_Position = gl_ModelViewProjectionMatrix * position;\n						"\
	"	uvOut = inUV;\n									"\
	"}\0												";

const char* NormalShader::fragShaderSrc = 
	"#version 120\n										"\
	"varying vec3 normalV;\n							"\
	"varying vec2 uvOut;\n								"\
	"uniform sampler2D tex;\n							"\
	"const vec3 lightV = vec3(-1.0, -1.0, -1.0);\n		"\
	"void main(){\n										"\
	"	vec3 diffuseColor = vec3(1.0, 1.0, 1.0);\n		"\
	"	vec3 unit_normal = normalize(normalV);\n		"\
	"	vec3 unit_light = normalize(lightV);\n		"\
	"	float diffuseLight = max(dot(-unit_light, unit_normal), 0.0) + 0.2;\n						"\
	"	gl_FragColor = vec4( diffuseColor * diffuseLight, 1.0);\n						"\
	"	//gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n						"\
	"}\0												";

NormalShader::NormalShader() {
	m_textureUniform = 0;
	m_program = 0;
}

NormalShader::~NormalShader() {

}

bool NormalShader::LoadShaders() {
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
	glBindAttribLocation(m_program, IVertexArrayObject::VertAttrib::ATTRIB_UV, "inUV");
	glBindAttribLocation(m_program, IVertexArrayObject::VertAttrib::ATTRIB_NORMAL, "inNormal");
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

void NormalShader::Activate() {
	glUseProgram(m_program);
}

void NormalShader::Deactivate() {
	glUseProgram(0);
}
