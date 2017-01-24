
#include "TriangleGL.h"

#include <d3dx9math.h>

#include <stdio.h>

void checkcompilederrors(GLuint shader, GLenum type) {
	GLint bShaderCompiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &bShaderCompiled);
	if (!bShaderCompiled) {
		int i32InfoLogLength, i32CharsWritten;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &i32InfoLogLength);

		char* pszInfoLog = new char[i32InfoLogLength];
		glGetShaderInfoLog(shader, i32InfoLogLength, &i32CharsWritten, pszInfoLog);
		char* pszMsg = new char[i32InfoLogLength + 256];
		if (type == GL_FRAGMENT_SHADER){
			sprintf(pszMsg, "Failed to compile pixel shader: %s", pszInfoLog);
		}
		else if (type == GL_VERTEX_SHADER){ 
			sprintf(pszMsg, "Failed to compile vertex shader: %s", pszInfoLog);
		}
		else{
			sprintf(pszMsg, "Failed to compile wtf shader: %s", pszInfoLog);
		}
		printf("%s", pszMsg);
		delete[] pszMsg;
		delete[] pszInfoLog;
	}
}

GLuint createShader(GLenum type, char* pSource) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, (const char**)&pSource, NULL);
	glCompileShader(shader);
	checkcompilederrors(shader, type);
	return shader;
}

char *file2string(const char *path){
	FILE *fd;
	long len, r;
	char *str;
	if (!(fd = fopen(path, "r"))){
		fprintf(stderr, "Can't open file '%s'\n", path);
		return NULL;
	}
	fseek(fd, 0, SEEK_END);
	len = ftell(fd);
	fseek(fd, 0, SEEK_SET);
	str = (char*)malloc(len * sizeof(char));
	r = fread(str, sizeof(char), len, fd);
	str[r] = '\0';
	fclose(fd);
	return str;
}



void TrangleGL::Create() {
	shaderID = glCreateProgram();

	char *vsSourceP = file2string("VS.glsl");
	char *fsSourceP = file2string("FS.glsl");

	GLuint vshader_id = createShader(GL_VERTEX_SHADER, vsSourceP);
	GLuint fshader_id = createShader(GL_FRAGMENT_SHADER, fsSourceP);

	glAttachShader(shaderID, vshader_id);
	glAttachShader(shaderID, fshader_id);

	glLinkProgram(shaderID);
	glUseProgram(shaderID);

	vertexAttribLoc = glGetAttribLocation(shaderID, "MyVertex");
	MatrixID = glGetUniformLocation(shaderID, "tr");

	vertices[0] = {  0.0f,  0.5f, 0.0f };
	vertices[1] = { -0.5f, -0.5f, 0.0f };
	vertices[2] = {  0.5f, -0.5f, 0.0f };	
	transformMatrix = Identity();

}

void TrangleGL::Transform(float *t) {
	//for (int i = 0; i < 16;i++) {
	//	transformMatrix.v[i] += t[i];
	//}
	/*transformMatrix.v[3] += t[3];
	transformMatrix.v[7] += t[7];
	transformMatrix.v[11] += t[11];*/
	transformMatrix = transformMatrix * *reinterpret_cast<Matrix4D*>(t);


}

void TrangleGL::Draw() {
	glEnableVertexAttribArray(vertexAttribLoc);
	glVertexAttribPointer(vertexAttribLoc, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, transformMatrix.v);
	glDrawArrays(GL_TRIANGLES,0,3);
}

void TrangleGL::Destroy() {
	glDeleteProgram(shaderID);
}