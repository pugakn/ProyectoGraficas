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
#include <algorithm>

#ifdef USING_GL_COMMON
bool checkcompilederrors(GLuint shader, GLenum type) {
	GLint bShaderCompiled = 0;
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
	if (!strcmp(pSource, ""))
		return 0;
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



Texture* Tools::textureCheker = nullptr;
std::vector<Texture*> Tools::m_textures;
std::vector<BaseRT*> Tools::RTs;
int Tools::textureChekerID = -1;
int Tools::LoadTexture(const char * path)
{
	for (auto &it : m_textures) {
		std::string ttstr = std::string(it->optname);
		if (ttstr == path) {
			return it->id;
		}
	}
#ifdef USING_GL_COMMON
	Texture *tex = new TextureGL;
#elif defined USING_D3D11
	Texture *tex = new TextureD3D;
#endif
	int textureID = tex->LoadTexture(const_cast<char*>(path));
	if (textureID != -1) {
		m_textures.push_back(tex);
		return textureID;
	}
	std::cout << "File not Found" << std::endl;
	delete tex;
	return textureChekerID;
}
Texture * Tools::GetTexture(int id)
{
	if (id == textureChekerID)
	{
		return textureCheker;
	}
	return *std::find_if(m_textures.begin(), m_textures.end(), [&id](const Texture* p) { return p->id == id; });;
}
#ifdef USING_GL_COMMON
#include "GLRT.h"
#include "GLDriver.h"
GLuint Tools::DefaultShaderID = 0;
GLDriver* Tools::pVideoDriver;
//===================== Render Targets ================================
int Tools::CreateRT(int numRT, int colorf, int depthf, int w, int h)
{
	GLRT	*pRT = new GLRT;
	if (w == 0)
		w = pVideoDriver->width;
	if (h == 0)
		h = pVideoDriver->height;
	if (pRT->Load(numRT, colorf, depthf, w, h)) {
		RTs.push_back(pRT);
		glBindFramebuffer(GL_FRAMEBUFFER, pVideoDriver->originalFBO);
		return (RTs.size() - 1);
	}
	else {
		delete pRT;
	}
	return -1;
}
int Tools::CreateRT(int numRT)
{
	GLRT	*pRT = new GLRT;
	if (pRT->Load(numRT, 0, 0, pVideoDriver->width, pVideoDriver->height)) {
		RTs.push_back(pRT);
		glBindFramebuffer(GL_FRAMEBUFFER, pVideoDriver->originalFBO);
		return (RTs.size() - 1);
	}
	else {
		delete pRT;
	}
	return -1;
}

void Tools::UseRT(int id) {
	if (id < 0 || id >= (int)RTs.size())
		return;

	GLRT *pRT = dynamic_cast<GLRT*>(RTs[id]);

	glBindFramebuffer(GL_FRAMEBUFFER, pRT->vFrameBuffers[0]);

#if defined(USING_OPENGL) || defined(USING_OPENGL_ES30)
	glDrawBuffers(pRT->m_numRT, pVideoDriver->DrawBuffers);
#endif

	glClearColor(0.5, 0.5, 0.5, 1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Tools::UseOriginalFBO() {
	glBindFramebuffer(GL_FRAMEBUFFER, pVideoDriver->originalFBO);
}

void Tools::DestroyRTs() {
	for (auto &it : RTs)
		delete it;
}
void Tools::Init(BaseDriver* driver)
{
	pVideoDriver = dynamic_cast<GLDriver*>(driver);
#ifdef USING_OPENGL
	char *vsSourceWire = "attribute vec4 Vertex;uniform mat4 WVP;void main() {gl_Position = WVP*Vertex;}";
	char *fsSourceWire = "void main() {gl_FragColor = vec4(1, 0, 1, 1);}";
#else
	char *vsSourceWire = "attribute highp vec4 Vertex;uniform highp mat4 WVP;void main() {gl_Position = WVP*Vertex;}";
	char *fsSourceWire = "void main() {gl_FragColor = vec4(1, 0, 1, 1);}";
#endif // USING_OPENGL

	GLuint vshaderWire_id = createShader(GL_VERTEX_SHADER, const_cast<char*>(vsSourceWire));
	GLuint fshaderWire_id = createShader(GL_FRAGMENT_SHADER, const_cast<char*>(fsSourceWire));
	DefaultShaderID = glCreateProgram();
	glAttachShader(DefaultShaderID, vshaderWire_id);
	glAttachShader(DefaultShaderID, fshaderWire_id);

	textureCheker = new TextureGL;
	textureChekerID = textureCheker->LoadDefaultTxture();
}

#elif defined USING_D3D11
#include "D3DXDriver.h"
#include "D3DRT.h"
extern ComPtr<ID3D11Device>            D3D11Device;
extern ComPtr<ID3D11DeviceContext>     D3D11DeviceContext;

extern ComPtr<ID3D11RenderTargetView>  D3D11RenderTargetView;  
extern ComPtr<ID3D11DepthStencilView>  D3D11DepthStencilTargetView;

ComPtr<ID3D11VertexShader> Tools::pDefaultVS;
ComPtr<ID3D11PixelShader>  Tools::pDefaultFS;
ComPtr<ID3DBlob>  Tools::DefaultVS_blob = nullptr;
ComPtr<ID3DBlob> Tools::DefaultFS_blob = nullptr;
D3DXDriver* Tools::pVideoDriver;

//===================== Render Targets ================================
int Tools::CreateRT(int numRT, int colorf, int depthf, int w, int h)
{
	D3DRT	*pRT = new D3DRT;
	if (w == 0)
		w = pVideoDriver->Width;
	if (h == 0)
		h = pVideoDriver->Height;
	if (pRT->Load(numRT, colorf, depthf, w, h)) {
		RTs.push_back(pRT);
		D3D11DeviceContext->OMSetRenderTargets(1, D3D11RenderTargetView.GetAddressOf(), D3D11DepthStencilTargetView.Get());
		return (RTs.size() - 1);
	}
	else {
		delete pRT;
	}
	return -1;
}
int Tools::CreateRT(int numRT)
{
	D3DRT	*pRT = new D3DRT;
	if (pRT->Load(numRT, 0, 0, pVideoDriver->Width, pVideoDriver->Height)) {
		RTs.push_back(pRT);
		D3D11DeviceContext->OMSetRenderTargets(1, D3D11RenderTargetView.GetAddressOf(), D3D11DepthStencilTargetView.Get());
		return (RTs.size() - 1);
	}
	else {
		delete pRT;
	}
	return -1;
}

void Tools::UseRT(int id) {
	if (id < 0 || id >= (int)RTs.size())
		return;

	D3DRT *pRT = dynamic_cast<D3DRT*>(RTs[id]);

	std::vector<ID3D11RenderTargetView**> RTVA;
	for (int i = 0; i < pRT->m_numRT; i++) {
		RTVA.push_back(pRT->vD3D11RenderTargetView[i].GetAddressOf());
	}

	D3D11DeviceContext->OMSetRenderTargets(pRT->m_numRT, &RTVA[0][0], pRT->D3D11DepthStencilTargetView.Get());

	float rgba[4];
	rgba[0] = 0.5f;
	rgba[1] = 0.5f;
	rgba[2] = 0.5f;
	rgba[3] = 1.0f;

	for (int i = 0; i < pRT->m_numRT; i++) {
		D3D11DeviceContext->ClearRenderTargetView(pRT->vD3D11RenderTargetView[i].Get(), rgba);
	}

	D3D11DeviceContext->ClearDepthStencilView(pRT->D3D11DepthStencilTargetView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Tools::UseOriginalFBO() {
	D3D11DeviceContext->OMSetRenderTargets(1, D3D11RenderTargetView.GetAddressOf(), D3D11DepthStencilTargetView.Get());
}

void Tools::DestroyRTs() {
	for (auto &it : RTs)
		delete it;
}
void Tools::Init(BaseDriver* driver)
{
	pVideoDriver = dynamic_cast<D3DXDriver*>(driver);
	textureCheker = new TextureD3D;
	textureChekerID = textureCheker->LoadDefaultTxture();
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