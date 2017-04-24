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
#include "Plane.h"

#ifdef USING_D3D11
extern ComPtr<ID3D11Device>            D3D11Device;
extern ComPtr<ID3D11DeviceContext>     D3D11DeviceContext;
#endif

void Plane::Create() {
#ifdef USING_GL_COMMON
	TexId = Tools::textureChekerID;
#elif defined(USING_D3D11)
	tex = Tools::textureCheker;
#endif


#ifdef USING_GL_COMMON
	shaderID = glCreateProgram();

	char *vsSourceP = file2string("Shaders/VS_Water.glsl");
	char *fsSourceP = file2string("Shaders/FS_Water.glsl");

	std::string vsrc = std::string(vsSourceP);
	std::string fsrc = std::string(fsSourceP);

	free(vsSourceP);
	free(fsSourceP);

	std::string Defines;
#ifdef USING_OPENGL
	Defines += "#define lowp\n\n";
	Defines += "#define mediump\n\n";
	Defines += "#define highp\n\n";
#endif // USING_OPENGL
	vsrc = Defines + vsrc;
	fsrc = Defines + fsrc;

	GLuint vshader_id = createShader(GL_VERTEX_SHADER, (char*)vsrc.c_str());
	GLuint fshader_id = createShader(GL_FRAGMENT_SHADER, (char*)fsrc.c_str());

	glAttachShader(shaderID, vshader_id);
	glAttachShader(shaderID, fshader_id);

	glLinkProgram(shaderID);
	glUseProgram(shaderID);

	vertexAttribLoc = glGetAttribLocation(shaderID, "Vertex");

	matWorldViewProjUniformLoc = glGetUniformLocation(shaderID, "WVP");

	diffuseLoc = glGetUniformLocation(shaderID, "diffuse");
#elif defined(USING_D3D11)
	char *vsSourceP = file2string("Shaders/VS_Water.hlsl");
	char *fsSourceP = file2string("Shaders/FS_Water.hlsl");

	if (!vsSourceP || !fsSourceP)
		exit(32);

	HRESULT hr;
	{
		VS_blob = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;
		hr = D3DCompile(vsSourceP, (UINT)strlen(vsSourceP), 0, 0, 0, "VS", "vs_5_0", 0, 0, &VS_blob, &errorBlob);
		if (hr != S_OK) {

			if (errorBlob) {
				printf("errorBlob shader[%s]", (char*)errorBlob->GetBufferPointer());
				return;
			}

			if (VS_blob) {
				return;
			}
		}

		hr = D3D11Device->CreateVertexShader(VS_blob->GetBufferPointer(), VS_blob->GetBufferSize(), 0, &pVS);
		if (hr != S_OK) {
			printf("Error Creating Vertex Shader\n");
			return;
		}
	}
	{
		FS_blob = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;
		hr = D3DCompile(fsSourceP, (UINT)strlen(fsSourceP), 0, 0, 0, "FS", "ps_5_0", 0, 0, &FS_blob, &errorBlob);
		if (hr != S_OK) {
			if (errorBlob) {
				printf("errorBlob shader[%s]", (char*)errorBlob->GetBufferPointer());
				return;
			}

			if (FS_blob) {
				return;
			}
		}

		hr = D3D11Device->CreatePixelShader(FS_blob->GetBufferPointer(), FS_blob->GetBufferSize(), 0, &pFS);
		if (hr != S_OK) {
			printf("Error Creating Pixel Shader\n");
			return;
		}
	}

	free(vsSourceP);
	free(fsSourceP);
#endif

	vertices[0] = { 0.0f,  1.0f, 0.0f , 1.0f,  0.0f, 1.0f, 0.0f, 1.0f,  0.0f, 0.0f };
	vertices[1] = { 0.0f,  0.0f, 0.0f , 1.0f,  0.0f, 1.0f, 0.0f, 1.0f,  0.0f, 1.0f };
	vertices[2] = { 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 1.0f };
	vertices[3] = { 1.0f,  1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f };

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
	D3D11DeviceContext->VSSetShader(pVS.Get(), 0, 0);
	D3D11DeviceContext->PSSetShader(pFS.Get(), 0, 0);

	D3D11_INPUT_ELEMENT_DESC vertexDeclaration[] = {
		{ "POSITION" , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	hr = D3D11Device->CreateInputLayout(vertexDeclaration, ARRAYSIZE(vertexDeclaration), VS_blob->GetBufferPointer(), VS_blob->GetBufferSize(), &Layout);
	if (hr != S_OK) {
		printf("Error Creating Input Layout\n");
		return;
	}
	D3D11DeviceContext->IASetInputLayout(Layout.Get());

	D3D11_BUFFER_DESC bdesc = { 0 };
	bdesc.Usage = D3D11_USAGE_DEFAULT;
	bdesc.ByteWidth = sizeof(Plane::CBuffer);
	bdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	hr = D3D11Device->CreateBuffer(&bdesc, 0, pd3dConstantBuffer.GetAddressOf());
	if (hr != S_OK) {
		printf("Error Creating Buffer Layout\n");
		return;
	}

	D3D11DeviceContext->VSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());
	D3D11DeviceContext->PSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());


	TextureD3D *texd3d = dynamic_cast<TextureD3D*>(tex);
	D3D11DeviceContext->PSSetShaderResources(0, 1, texd3d->pSRVTex.GetAddressOf());
	D3D11DeviceContext->PSSetSamplers(0, 1, texd3d->pSampler.GetAddressOf());


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
}

void Plane::Transform(float *t) {
	transform = t;
}

void Plane::Draw(float *t) {
#ifdef USING_GL_COMMON
	glUseProgram(shaderID);

	if (t)
		transform = t;

	Matrix4D VP = Matrix4D(pScProp->pCameras[0]->VP);
	Matrix4D WVP = transform*VP;

	glUniformMatrix4fv(matWorldViewProjUniformLoc, 1, GL_FALSE, &WVP.m[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	glEnableVertexAttribArray(vertexAttribLoc);

	glVertexAttribPointer(vertexAttribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(0));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex->id);
	glUniform1i(diffuseLoc, 0);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(vertexAttribLoc);


	glUseProgram(0);
#elif defined(USING_D3D11)
	if (t)
		transform = t;

	Matrix4D VP = Matrix4D(pScProp->pCameras[0]->VP);
	Matrix4D WVP = transform*VP;
	CnstBuffer.WVP = WVP;

	UINT stride = sizeof(CVertex);
	UINT offset = 0;
	D3D11DeviceContext->VSSetShader(pVS.Get(), 0, 0);
	D3D11DeviceContext->PSSetShader(pFS.Get(), 0, 0);

	D3D11DeviceContext->IASetInputLayout(Layout.Get());

	D3D11DeviceContext->UpdateSubresource(pd3dConstantBuffer.Get(), 0, 0, &CnstBuffer.WVP.m[0][0], 0, 0);

	TextureD3D *texd3d = dynamic_cast<TextureD3D*>(tex);
	D3D11DeviceContext->PSSetShaderResources(0, 1, texd3d->pSRVTex.GetAddressOf());
	D3D11DeviceContext->PSSetSamplers(0, 1, texd3d->pSampler.GetAddressOf());

	D3D11DeviceContext->VSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());
	D3D11DeviceContext->PSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());

	D3D11DeviceContext->IASetVertexBuffers(0, 1, VB.GetAddressOf(), &stride, &offset);
	D3D11DeviceContext->IASetIndexBuffer(IB.Get(), DXGI_FORMAT_R16_UINT, 0);

	D3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D11DeviceContext->DrawIndexed(6, 0, 0);
#endif
}

void Plane::Destroy() {
#ifdef USING_GL_COMMON
	glDeleteProgram(shaderID);
#elif defined(USING_D3D11)
#endif
}