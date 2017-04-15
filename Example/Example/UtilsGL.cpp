/*********************************************************
* Copyright (C) 2017 Daniel Enriquez (camus_mm@hotmail.com)
* All Rights Reserved
*
* You may use, distribute and modify this code under the
* following terms:
* ** Do not claim that you wrote this software
* ** A mention would be appreciated but not needed
* ** I do not and will not provide support, this software is "as is"
* ** Enjoy, learn and share.
*********************************************************/

#include "UtilsGL.h"
#include <stdio.h>    
#include <stdlib.h>
#include <string>
#include <iostream>

#ifdef USING_OPENGL_ES
bool checkcompilederrors(GLuint shader, GLenum type) {
	GLint bShaderCompiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &bShaderCompiled);
	if (!bShaderCompiled) {
		int i32InfoLogLength, i32CharsWritten;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &i32InfoLogLength);

		char* pszInfoLog = new char[i32InfoLogLength];
		glGetShaderInfoLog(shader, i32InfoLogLength, &i32CharsWritten, pszInfoLog);
		char* pszMsg = new char[i32InfoLogLength + 256];
		if (type == GL_FRAGMENT_SHADER) {
			sprintf(pszMsg, "Failed to compile pixel shader: %s", pszInfoLog);
		}
		else if (type == GL_VERTEX_SHADER) {
			sprintf(pszMsg, "Failed to compile vertex shader: %s", pszInfoLog);
		}
		else {
			sprintf(pszMsg, "Failed to compile wtf shader: %s", pszInfoLog);
		}
		printf("%s", pszMsg);
		delete[] pszMsg;
		delete[] pszInfoLog;
		return false;
	}
	return true;
}

GLuint createShader(GLenum type, char* pSource) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, (const char**)&pSource, NULL);
	glCompileShader(shader);
	if (!checkcompilederrors(shader, type))
		return 0;
	return shader;
}
#endif

char *file2string(const char *path) {
	FILE *fd;
	long len, r;
	char *str;
	if (!(fd = fopen(path, "r"))) {
		fprintf(stderr, "Can't open file '%s'\n", path);
		return NULL;
	}
	fseek(fd, 0, SEEK_END);
	len = ftell(fd);
	fseek(fd, 0, SEEK_SET);
	str = (char*)malloc(1 + len * sizeof(char));
	r = fread(str, sizeof(char), len, fd);
	str[r] = '\0';
	fclose(fd);
	return str;
}



Texture* Utils::textureCheker = nullptr;
#ifdef USING_OPENGL_ES
GLuint Utils::DefaultShaderID = 0;
int Utils::textureChekerID = -1;

void Utils::Init()
{
	char *vsSourceWire = "attribute highp vec4 Vertex;uniform highp mat4 WVP;void main() {gl_Position = WVP*Vertex;}";
	char *fsSourceWire = "void main() {gl_FragColor = vec4(1, 0, 1, 1);}";
	GLuint vshaderWire_id = createShader(GL_VERTEX_SHADER, const_cast<char*>(vsSourceWire));
	GLuint fshaderWire_id = createShader(GL_FRAGMENT_SHADER, const_cast<char*>(fsSourceWire));
	DefaultShaderID = glCreateProgram();
	glAttachShader(DefaultShaderID, vshaderWire_id);
	glAttachShader(DefaultShaderID, fshaderWire_id);

	textureCheker = new TextureGL;
	textureChekerID = textureCheker->LoadDefaultTxture();
}

#elif defined USING_D3D11
extern ComPtr<ID3D11Device>            D3D11Device;
extern ComPtr<ID3D11DeviceContext>     D3D11DeviceContext;

ComPtr<ID3D11VertexShader> Utils::pDefaultVS;
ComPtr<ID3D11PixelShader>  Utils::pDefaultFS;
ComPtr<ID3DBlob>  Utils::DefaultVS_blob = nullptr;
ComPtr<ID3DBlob> Utils::DefaultFS_blob = nullptr;
void Utils::Init()
{
	textureCheker = new TextureD3D;
	textureCheker->LoadDefaultTxture();
	char *vsSourceWire = "cbuffer ConstantBuffer{float4x4 WVP;}struct VS_INPUT {float4 position : POSITION;};struct VS_OUTPUT {float4 hposition : SV_POSITION;};VS_OUTPUT VS(VS_INPUT input) {VS_OUTPUT OUT;OUT.hposition = mul(WVP, input.position);return OUT;}";
	char *fsSourceWire = "cbuffer ConstantBuffer{float4x4 WVP;}struct VS_OUTPUT {float4 hposition : SV_POSITION;};float4 FS(VS_OUTPUT input) : SV_TARGET{return float4(1,0,1,1);}";
	std::string vstrWire = std::string(vsSourceWire);
	std::string fstrWire = std::string(fsSourceWire);

	if (!vsSourceWire || !fsSourceWire)
		return;
	//==================== compile VS =====================
	ComPtr<ID3DBlob> errorBlob = nullptr;
	if (D3DCompile(vstrWire.c_str(), vstrWire.size(), 0, 0, 0, "VS", "vs_5_0", 0, 0, &DefaultVS_blob, &errorBlob) != S_OK) {
		if (errorBlob) {
			std::cout << "ErrorBlob shader" << (char*)errorBlob->GetBufferPointer();
			return;
		}
		if (DefaultVS_blob) {
			return;
		}
	}
	//=========== Create VS ============
	if (D3D11Device->CreateVertexShader(DefaultVS_blob->GetBufferPointer(), DefaultVS_blob->GetBufferSize(), 0, &pDefaultVS) != S_OK) {
		std::cout << "Error Creatong Vertex Shader" << std::endl;

		return;
	}
	//==================== compile PS =====================
	errorBlob.Reset();
	if (D3DCompile(fstrWire.c_str(), fstrWire.size(), 0, 0, 0, "FS", "ps_5_0", 0, 0, &DefaultFS_blob, &errorBlob) != S_OK) {
		if (errorBlob) {
			std::cout << "ErrorBlob shader" << (char*)errorBlob->GetBufferPointer();
			return;
		}

		if (DefaultFS_blob) {
			return;
		}
	}
	//=========== Create PS ==============
	if (D3D11Device->CreatePixelShader(DefaultFS_blob->GetBufferPointer(), DefaultFS_blob->GetBufferSize(), 0, &pDefaultFS) != S_OK) {
		std::cout << "Error Creating Pixel Shader" << std::endl;
		return;
	}
}


#endif