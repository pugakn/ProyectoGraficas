#include "Quad.h"
#include "GLShader.h"
#include "D3DShader.h"
#include "ShaderManager.h"
#include "GLRT.h"
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
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#ifdef USING_D3D11
extern ComPtr<ID3D11Device>            D3D11Device;
extern ComPtr<ID3D11DeviceContext>     D3D11DeviceContext;
#endif
short Quad::deferredRT = -1;
short Quad::deferredRT_2 = -1;
short Quad::deferredRT_1 = -1;
void Quad::Create() {
	difTextId = Tools::textureChekerID;
	difTex = Tools::textureCheker;
#ifdef USING_GL_COMMON
		auto  set = ShaderManager::GetShaderSetBySignature(sig, "Shaders/VS_Quad.glsl", "Shaders/FS_Quad.glsl");
		m_shaderSet = set;
		m_shaderType = Shader::TYPE::G_DEFERRED_PASS;
		GLShader * glDefaultShader = (GLShader*)(m_shaderSet)[m_shaderType];
		shaderID = glDefaultShader->ShaderID;
		glUseProgram(shaderID);
#elif defined(USING_D3D11)
	auto  set = ShaderManager::GetShaderSetBySignature(sig, "Shaders/VS_Quad.hlsl", "Shaders/FS_Quad.hlsl");
	m_shaderSet = set;
	m_shaderType = Shader::TYPE::G_DEFERRED_PASS;
	D3DShader * glDefaultShader = (D3DShader*)(m_shaderSet)[m_shaderType];



	//bool errorShader = false;
	//char *vsSourceP = file2string("Shaders/VS_Quad.hlsl");
	//char *fsSourceP = file2string("Shaders/FS_Quad.hlsl");

	//std::string vsrc;
	//std::string fsrc;
	//if (vsSourceP && fsSourceP)
	//{
	//	vsrc = std::string(vsSourceP);
	//	fsrc = std::string(fsSourceP);
	//}


	//std::string Defines;
	////Defines += "#define LINEAR_DEPTH\n\n";
	//Defines += "#define G_BUFF_PASS\n\n";
	//vsrc = Defines + vsrc;
	//fsrc = Defines + fsrc;

	//if (!vsSourceP || !fsSourceP)
	//	errorShader = true;
	//HRESULT hr;
	//if (!errorShader)
	//{
	//	{
	//		VS_blob = nullptr;
	//		ComPtr<ID3DBlob> errorBlob = nullptr;
	//		hr = D3DCompile(vsrc.c_str(), (UINT)strlen(vsrc.c_str()), 0, 0, 0, "VS", "vs_5_0", 0, 0, &VS_blob, &errorBlob);
	//		if (hr != S_OK) {

	//			if (errorBlob) {
	//				printf("errorBlob shader[%s]", (char*)errorBlob->GetBufferPointer());
	//			}
	//			errorShader = true;
	//		}

	//		hr = D3D11Device->CreateVertexShader(VS_blob->GetBufferPointer(), VS_blob->GetBufferSize(), 0, &pVS);
	//		if (hr != S_OK) {
	//			printf("Error Creating Vertex Shader\n");
	//			errorShader = true;
	//		}
	//	}
	//	{
	//		FS_blob = nullptr;
	//		ComPtr<ID3DBlob> errorBlob = nullptr;
	//		hr = D3DCompile(fsrc.c_str(), (UINT)strlen(fsrc.c_str()), 0, 0, 0, "FS", "ps_5_0", 0, 0, &FS_blob, &errorBlob);
	//		if (hr != S_OK) {
	//			if (errorBlob) {
	//				printf("errorBlob shader[%s]", (char*)errorBlob->GetBufferPointer());
	//			}
	//			errorShader = true;
	//		}

	//		hr = D3D11Device->CreatePixelShader(FS_blob->GetBufferPointer(), FS_blob->GetBufferSize(), 0, &pFS);
	//		if (hr != S_OK) {
	//			printf("Error Creating Pixel Shader\n");
	//			errorShader = true;
	//		}
	//	}
	//}
	//if (errorShader)
	//{
	//	VS_blob = Tools::DefaultVS_blob;
	//	pVS = Tools::pDefaultVS;
	//	FS_blob = Tools::DefaultFS_blob;
	//	pFS = Tools::pDefaultFS;
	//}
	//free(vsSourceP);
	//free(fsSourceP);
#endif

	vertices[0] = { -1.0f,  1.0f,  0.9f ,1.0f,  0.0f, 0.0f };//Left Top
	vertices[1] = { -1.0f,  -1.0f, 0.9f ,1.0f,  0.0f, 1.0f };//Left Bot
	vertices[2] = { 1.0f,  -1.0f,  0.9f ,1.0f,  1.0f, 1.0f };//Right Bot
	vertices[3] = { 1.0f,  1.0f,   0.9f ,1.0f,  1.0f, 0.0f };//Right Top


	indices[0] = 2;
	indices[1] = 1;
	indices[2] = 0;
	indices[3] = 3;
	indices[4] = 2;
	indices[5] = 0;

#ifdef USING_GL_COMMON
	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(CVertex), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned short), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#elif defined(USING_D3D11)
	D3D11DeviceContext->VSSetShader(glDefaultShader->pVS.Get(), 0, 0);
	D3D11DeviceContext->PSSetShader(glDefaultShader->pFS.Get(), 0, 0);

	D3D11_INPUT_ELEMENT_DESC vertexDeclaration[] = {
		{ "POSITION" , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0,DXGI_FORMAT_R32G32_FLOAT, 0, 16,D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	HRESULT hr = D3D11Device->CreateInputLayout(vertexDeclaration, ARRAYSIZE(vertexDeclaration), glDefaultShader->VS_blob->GetBufferPointer(), glDefaultShader->VS_blob->GetBufferSize(), &Layout);
	if (hr != S_OK) {
		printf("Error Creating Input Layout\n");
		return;
	}
	D3D11DeviceContext->IASetInputLayout(Layout.Get());

	D3D11_BUFFER_DESC bdesc = { 0 };
	bdesc.Usage = D3D11_USAGE_DEFAULT;
	bdesc.ByteWidth = /*errorShader ? sizeof(Matrix4D) :*/ sizeof(Quad::CBuffer);
	bdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	hr = D3D11Device->CreateBuffer(&bdesc, 0, pd3dConstantBuffer.GetAddressOf());
	if (hr != S_OK) {
		printf("Error Creating Buffer Layout\n");
		return;
	}

	D3D11DeviceContext->VSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());
	D3D11DeviceContext->PSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());


	//int slot = 0;
	//TextureD3D *texd3d = dynamic_cast<TextureD3D*>(difTex);
	//D3D11DeviceContext->PSSetShaderResources(slot, 1, texd3d->pSRVTex.GetAddressOf());
	//D3D11DeviceContext->PSSetSamplers(slot, 1, texd3d->pSampler.GetAddressOf());
	//slot++;
	//texd3d = dynamic_cast<TextureD3D*>(normalTex);
	//D3D11DeviceContext->PSSetShaderResources(slot, 1, texd3d->pSRVTex.GetAddressOf());
	//D3D11DeviceContext->PSSetSamplers(slot, 1, texd3d->pSampler.GetAddressOf());
	//slot++;
	//texd3d = dynamic_cast<TextureD3D*>(specTex);
	//D3D11DeviceContext->PSSetShaderResources(slot, 1, texd3d->pSRVTex.GetAddressOf());
	//D3D11DeviceContext->PSSetSamplers(slot, 1, texd3d->pSampler.GetAddressOf());
	//slot++;
	//texd3d = dynamic_cast<TextureD3D*>(depthTex);
	//D3D11DeviceContext->PSSetShaderResources(slot, 1, texd3d->pSRVTex.GetAddressOf());
	//D3D11DeviceContext->PSSetSamplers(slot, 1, texd3d->pSampler.GetAddressOf());
	//slot++;
	//texd3d = dynamic_cast<TextureD3D*>(shadowMapTexture);
	//D3D11DeviceContext->PSSetShaderResources(slot, 1, texd3d->pSRVTex.GetAddressOf());
	//D3D11DeviceContext->PSSetSamplers(slot, 1, texd3d->pSampler.GetAddressOf());

	//D3D11DeviceContext->VSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());
	//D3D11DeviceContext->PSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());



	bdesc = { 0 };
	bdesc.ByteWidth = sizeof(CVertex) * 4;
	bdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA subData = { vertices, 0, 0 };
	hr = D3D11Device->CreateBuffer(&bdesc, &subData, &VB);
	if (hr != S_OK) {
		printf("Error Creating Vertex Buffer\n");
		return;
	}

	bdesc = { 0 };
	bdesc.ByteWidth = 6 * sizeof(USHORT);
	bdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	subData = { indices, 0, 0 };

	hr = D3D11Device->CreateBuffer(&bdesc, &subData, &IB);
	if (hr != S_OK) {
		printf("Error Creating Index Buffer\n");
		return;
	}
#endif

	transform = Identity();
	deferredRT = Tools::CreateRT(1);
	deferredRT_2 = Tools::CreateRT(1);
	deferredRT_1 = Tools::CreateRT(1);
	for (auto & fx : m_FX)
	{
		fx->Init();
	}
}

void Quad::Transform(float *t) {
	transform = t;
}

void Quad::Draw(float *t) {
#ifdef USING_GL_COMMON
	glUseProgram(shaderID);

	if (t)
		transform = t;

	Tools::UseRT(deferredRT);
	SetShaderType(Shader::TYPE::G_DEFERRED_PASS);//
	Matrix4D VP = Matrix4D(pScProp->pCameras[0]->VP);
	Matrix4D W = transform;
	Matrix4D WVP = W*VP;
	Matrix4D WV = pScProp->pCameras[0]->m_view;
	Matrix4D WVPI = Inverse(VP);
	std::vector<Vector4D>LightPositions;
	std::vector<Vector4D>LightColors;
	for (unsigned int i = 0; i < pScProp->Lights.size(); i++) {
		LightPositions.push_back(Vector4D(pScProp->Lights[i].Position,1.0));
		LightColors.push_back(Vector4D(pScProp->Lights[i].Color,1.0));
	}
	auto  sh = ((GLShader*)m_shaderSet[m_shaderType]);
	//if (matWorldUniformLoc != -1)
		//glUniformMatrix4fv(sh->m_locs.matWorldUniformLoc, 1, GL_FALSE, &W.m[0][0]);
	//if (WVPLoc != -1)
		//glUniformMatrix4fv(sh->m_locs.matWorldViewProjUniformLoc, 1, GL_FALSE, &WVP.m[0][0]);
	if (sh->m_locs.matWorldUniformLoc != -1)
		glUniformMatrix4fv(sh->m_locs.matWorldUniformLoc, 1, GL_FALSE, &W.m[0][0]);
	//if (WorldViewLoc!=-1)
		//glUniformMatrix4fv(sh->m_locs.matWorldViewUniformLoc, 1, GL_FALSE, &WV.m[0][0]);
	if (sh->m_locs.VPInverseLoc != -1)
		glUniformMatrix4fv(sh->m_locs.VPInverseLoc, 1, GL_FALSE, &WVPI.m[0][0]);

	if (sh->m_locs.LightColorsLoc != -1)
		glUniform4fv(sh->m_locs.LightColorsLoc, LightColors.size(), &LightColors[0].x);
	if (sh->m_locs.LightPositionsLoc != -1)
		glUniform4fv(sh->m_locs.LightPositionsLoc, LightPositions.size(), &LightPositions[0].x);

	Vector4D campos = Vector4D(pScProp->pCameras[0]->m_pos.x, pScProp->pCameras[0]->m_pos.y, pScProp->pCameras[0]->m_pos.z,1.0);
	if (sh->m_locs.camPosLoc != -1)
		glUniform4fv(sh->m_locs.camPosLoc, 1, &campos.x);
	int ligthSize = pScProp->Lights.size();
	if (sh->m_locs.NumLightsLoc != -1)
		glUniform1iv(sh->m_locs.NumLightsLoc, 1, &ligthSize);

	Matrix4D LightVP = pScProp->LightsWShadow[0].VP;
	if (sh->m_locs.CamVPLoc != -1)
		glUniformMatrix4fv(sh->m_locs.CamVPLoc, 1, GL_FALSE, &LightVP.m[0][0]);
	if (sh->m_locs.LinearLightDirLoc != -1)
		glUniform3fv(sh->m_locs.LinearLightDirLoc,1,&pScProp->LightsWShadow[0].dir.x);

	//if (uvAttribLoc != -1)
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	glEnableVertexAttribArray(sh->m_locs.vertexAttribLocs);
	//if (uvAttribLoc != -1)
		glEnableVertexAttribArray(sh->m_locs.uvAttribLocs);


	glVertexAttribPointer(sh->m_locs.vertexAttribLocs, 4, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(0));
	if (sh->m_locs.uvAttribLocs != -1)
		glVertexAttribPointer(sh->m_locs.uvAttribLocs, 2, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(16));
	if (shaderID != Tools::DefaultShaderID)
	{
		int c = 0;
		if (sh->m_locs.textureLoc01 != -1)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, difTex->id);
			glUniform1i(sh->m_locs.textureLoc01, c++);
		}
		if (sh->m_locs.textureLoc02 != -1)
		{
			glActiveTexture(GL_TEXTURE0+c);
			glBindTexture(GL_TEXTURE_2D, depthTex->id);
			glUniform1i(sh->m_locs.textureLoc02, c++);
		}
		if (sh->m_locs.textureLoc03 != -1)
		{
			glActiveTexture(GL_TEXTURE0+c);
			glBindTexture(GL_TEXTURE_2D, specTex->id);
			glUniform1i(sh->m_locs.textureLoc03, c++);
		}
		if (sh->m_locs.textureLoc04 != -1)
		{
			glActiveTexture(GL_TEXTURE0+c);
			glBindTexture(GL_TEXTURE_2D, normalTex->id);
			glUniform1i(sh->m_locs.textureLoc04, c++);
		}
		if (sh->m_locs.textureLoc05 != -1)
		{
			glActiveTexture(GL_TEXTURE0 + c);
			glBindTexture(GL_TEXTURE_2D, shadowMapTexture->id);
			glUniform1i(sh->m_locs.textureLoc05, c++);
			if (sh->m_locs.ShadowMapSize != -1){
				float smsize[] = { static_cast<float>(shadowMapTexture->x) , static_cast<float>(shadowMapTexture->y) };
				glUniform2fv(sh->m_locs.ShadowMapSize, 1, &smsize[0]);
			}
		}
	}

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	if (sh->m_locs.uvAttribLocs != -1)
		glDisableVertexAttribArray(sh->m_locs.uvAttribLocs);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(sh->m_locs.vertexAttribLocs);


	glUseProgram(0);
	Tools::UseOriginalFBO();
	FXPass();
#elif defined(USING_D3D11)

	//std::vector<Vector4D>LightPositions;
	//std::vector<Vector4D>LightColors;
	//for (unsigned int i = 0; i < pScProp->Lights.size(); i++) {
	//	LightPositions.push_back(Vector4D(pScProp->Lights[i].Position, 1.0));
	//	LightColors.push_back(Vector4D(pScProp->Lights[i].Color, 1.0));
	//}
	if (t)
		transform = t;

	//Tools::UseRT(deferredRT);
	SetShaderType(Shader::TYPE::G_DEFERRED_PASS);//
	Matrix4D VP = Matrix4D(pScProp->pCameras[0]->VP);
	Matrix4D W = transform;
	Matrix4D WVP = W*VP;
	Matrix4D WV = pScProp->pCameras[0]->m_view;
	Matrix4D WVPI = Inverse(VP);
	for (unsigned int i = 0; i < pScProp->Lights.size(); i++) {
		CnstBuffer.LightPositions[i] = (Vector4D(pScProp->Lights[i].Position, 1.0));
		CnstBuffer.LightColors[i] =  (Vector4D(pScProp->Lights[i].Color, 1.0));
	}
	//Matrix4D WVPI = Inverse(VP);
	CnstBuffer.World = transform;
	CnstBuffer.CameraPosition =  Vector4D(pScProp->pCameras[0]->m_pos.x, pScProp->pCameras[0]->m_pos.y, pScProp->pCameras[0]->m_pos.z, 1.0);
	CnstBuffer.CamVP = transform*pScProp->LightsWShadow[0].VP;
	CnstBuffer.NumLights = NUM_LIGHTS;
	CnstBuffer.ShadowTexSize[0] = static_cast<float>(shadowMapTexture->x);
	CnstBuffer.ShadowTexSize[1] = static_cast<float>(shadowMapTexture->y);
	CnstBuffer.VPInverse = WVPI;
	CnstBuffer.NumLights2 = NUM_LIGHTS;
	//memcpy(CnstBuffer.LightPositions, &LightPositions[0].x, LightPositions.size());
	//memcpy(CnstBuffer.LightColors, &LightColors[0].x, LightColors.size());

	auto  sh = ((D3DShader*)m_shaderSet[m_shaderType]);

	UINT stride = sizeof(CVertex);
	UINT offset = 0;
	D3D11DeviceContext->VSSetShader(sh->pVS.Get(), 0, 0);
	D3D11DeviceContext->PSSetShader(sh->pFS.Get(), 0, 0);

	D3D11DeviceContext->IASetInputLayout(Layout.Get());

	D3D11DeviceContext->UpdateSubresource(pd3dConstantBuffer.Get(), 0, 0, &CnstBuffer.World.m[0][0], 0, 0);
	D3D11DeviceContext->VSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());
	D3D11DeviceContext->PSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());

	int slot = 0;
	TextureD3D *texd3d = dynamic_cast<TextureD3D*>(difTex);
	D3D11DeviceContext->PSSetShaderResources(slot, 1, texd3d->pSRVTex.GetAddressOf());
	D3D11DeviceContext->PSSetSamplers(slot, 1, texd3d->pSampler.GetAddressOf());
	slot++;
	texd3d = dynamic_cast<TextureD3D*>(normalTex);
	D3D11DeviceContext->PSSetShaderResources(slot, 1, texd3d->pSRVTex.GetAddressOf());
	D3D11DeviceContext->PSSetSamplers(slot, 1, texd3d->pSampler.GetAddressOf());
	slot++;
	texd3d = dynamic_cast<TextureD3D*>(specTex);
	D3D11DeviceContext->PSSetShaderResources(slot, 1, texd3d->pSRVTex.GetAddressOf());
	D3D11DeviceContext->PSSetSamplers(slot, 1, texd3d->pSampler.GetAddressOf());
	slot++;
	texd3d = dynamic_cast<TextureD3D*>(depthTex);
	D3D11DeviceContext->PSSetShaderResources(slot, 1, texd3d->pSRVTex.GetAddressOf());
	D3D11DeviceContext->PSSetSamplers(slot, 1, texd3d->pSampler.GetAddressOf());
	slot++;
	texd3d = dynamic_cast<TextureD3D*>(shadowMapTexture);
	D3D11DeviceContext->PSSetShaderResources(slot, 1, texd3d->pSRVTex.GetAddressOf());
	D3D11DeviceContext->PSSetSamplers(slot, 1, texd3d->pSampler.GetAddressOf());


	D3D11DeviceContext->IASetVertexBuffers(0, 1, VB.GetAddressOf(), &stride, &offset);
	D3D11DeviceContext->IASetIndexBuffer(IB.Get(), DXGI_FORMAT_R16_UINT, 0);

	D3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D11DeviceContext->DrawIndexed(6, 0, 0);

	Tools::UseOriginalFBO();
#endif

}
void Quad::FXPass()
{

	for (auto & fx : m_FX)
	{
		fx->ApplyFX(Tools::RTs[deferredRT]->vColorTextures[0]);
	}

#ifdef USING_GL_COMMON
	// FINAL PASS
	SetShaderType(Shader::TYPE::G_FORWARD_PASS);//
	Matrix4D W = transform;
	auto sh = ((GLShader*)m_shaderSet[m_shaderType]);
	if (sh->m_locs.matWorldUniformLoc != -1)
		glUniformMatrix4fv(sh->m_locs.matWorldUniformLoc, 1, GL_FALSE, &W.m[0][0]);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glEnableVertexAttribArray(sh->m_locs.vertexAttribLocs);
	glEnableVertexAttribArray(sh->m_locs.uvAttribLocs);
	glVertexAttribPointer(sh->m_locs.vertexAttribLocs, 4, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(0));
	if (sh->m_locs.uvAttribLocs != -1)
		glVertexAttribPointer(sh->m_locs.uvAttribLocs, 2, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(16));
	if (shaderID != Tools::DefaultShaderID)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Tools::RTs[deferredRT_1]->vColorTextures[0]->id);
		glUniform1i(sh->m_locs.textureLoc01, 0);
	}
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	if (sh->m_locs.uvAttribLocs != -1)
		glDisableVertexAttribArray(sh->m_locs.uvAttribLocs);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(sh->m_locs.vertexAttribLocs);
	glUseProgram(0);
#else

#endif
}
void Quad::Destroy() {
#ifdef USING_GL_COMMON
	glDeleteProgram(shaderID);
#elif defined(USING_D3D11)
#endif
}

void Quad::SetShaderType(Shader::TYPE type)
{
	if (m_shaderType != type)
	{
		m_shaderType = type;
#ifdef USING_GL_COMMON
		shaderID = ((GLShader*)(m_shaderSet)[type])->ShaderID;
		glUseProgram(shaderID);
#else
		auto sh = ((D3DShader*)(m_shaderSet)[type]);
		D3D11DeviceContext->VSSetShader(sh->pVS.Get(), 0, 0);
		D3D11DeviceContext->PSSetShader(sh->pFS.Get(), 0, 0);
		
#endif
	}
}

