#include "TextMeshD3D.h"
#ifdef USING_D3D11
#include <iostream>
extern ComPtr<ID3D11Device>            D3D11Device;
extern ComPtr<ID3D11DeviceContext>     D3D11DeviceContext;

void TextMeshD3D::Create()
{
	bool errorShader = false;
	transform = Identity();
	Scale = Identity();
	Position = Identity();
	m_text = "Inserte Texto 'XD";

	char *vsSource = file2string("Shaders/VS_Font.hlsl");
	char *fsSource = file2string("Shaders/FS_Font.hlsl");
	std::string vstr;
	std::string fstr;
	if (!vsSource || !fsSource)
		errorShader = true;
	else {
		vstr = std::string(vsSource);
		fstr = std::string(fsSource);
	}
	delete[] vsSource;
	delete[] fsSource;
	if (!errorShader)
	{
		//==================== compile VS =====================
		VS_blob = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;
		if (D3DCompile(vstr.c_str(), vstr.size(), 0, 0, 0, "VS", "vs_5_0", 0, 0, &VS_blob, &errorBlob) != S_OK) {
			if (errorBlob) {
				std::cout << "ErrorBlob shader" << (char*)errorBlob->GetBufferPointer();
			}
			errorShader = true;
		}
		//=========== Create VS ============
		if (D3D11Device->CreateVertexShader(VS_blob->GetBufferPointer(), VS_blob->GetBufferSize(), 0, &pVS) != S_OK) {
			std::cout << "Error Creatong Vertex Shader" << std::endl;
			errorShader = true;
		}
		//==================== compile PS =====================
		FS_blob = nullptr;
		errorBlob.Reset();
		if (D3DCompile(fstr.c_str(), fstr.size(), 0, 0, 0, "FS", "ps_5_0", 0, 0, &FS_blob, &errorBlob) != S_OK) {
			if (errorBlob) {
				std::cout << "ErrorBlob shader" << (char*)errorBlob->GetBufferPointer();
			}
			errorShader = true;
		}
		//=========== Create PS ==============
		if (D3D11Device->CreatePixelShader(FS_blob->GetBufferPointer(), FS_blob->GetBufferSize(), 0, &pFS) != S_OK) {
			std::cout << "Error Creating Pixel Shader" << std::endl;
			errorShader = true;
		}
	}
	if (errorShader)
	{
		VS_blob = Tools::DefaultVS_blob;
		pVS = Tools::pDefaultVS;
		FS_blob = Tools::DefaultFS_blob;
		pFS = Tools::pDefaultFS;
	}
	else
	{
		//Cargar propiedades de fuente
		font.LoadFile("Fonts/ArialFont.fnt");
		//=========================== Create Textures ===============================
		int textureID = Tools::LoadTexture("ArialFont_t0.tga");
		difuseText = Tools::GetTexture(textureID);
		textureWidth = difuseText->x;
	}



	//==================== Create Decl Data =====================
	D3D11_INPUT_ELEMENT_DESC elementDesc;
	elementDesc.SemanticName = "POSITION";
	elementDesc.SemanticIndex = 0;
	elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elementDesc.InputSlot = 0;
	elementDesc.AlignedByteOffset = 0;
	elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elementDesc.InstanceDataStepRate = 0;
	VertexDecl.push_back(elementDesc);

	elementDesc.SemanticName = "TEXCOORD";
	elementDesc.SemanticIndex = 0;
	elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
	elementDesc.InputSlot = 0;
	elementDesc.AlignedByteOffset = 16;
	elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elementDesc.InstanceDataStepRate = 0;
	VertexDecl.push_back(elementDesc);
	


	//==================== Create Input Layout =====================
	if (D3D11Device->CreateInputLayout(&VertexDecl[0], VertexDecl.size(), VS_blob->GetBufferPointer(), VS_blob->GetBufferSize(), &Layout) != S_OK) {
		std::cout << "Error Creating Input Layout" << std::endl;
		return;
	}
	D3D11DeviceContext->IASetInputLayout(Layout.Get());

	//==================== Create Buffer Layout =====================
	D3D11_BUFFER_DESC bdesc = { 0 };
	bdesc.Usage = D3D11_USAGE_DEFAULT;
	bdesc.ByteWidth = errorShader ? sizeof(Matrix4D) : sizeof(TextMeshD3D::ConstBuffer);
	bdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	if (D3D11Device->CreateBuffer(&bdesc, 0, ConstantBuffer.GetAddressOf()) != S_OK) {
		std::cout << "Error Creating Buffer Layout" << std::endl;
		return;
	}

	//==================== Create Index Buffer =====================

	m_indexBuffer[0] = 2;
	m_indexBuffer[1] = 1;
	m_indexBuffer[2] = 0;
	m_indexBuffer[3] = 3;
	m_indexBuffer[4] = 2;
	m_indexBuffer[5] = 0;

	bdesc = { 0 };
	bdesc.ByteWidth = 6 * sizeof(USHORT);
	bdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	D3D11_SUBRESOURCE_DATA subData = { &m_indexBuffer[0], 0, 0 };

	if (D3D11Device->CreateBuffer(&bdesc, &subData, &IB) != S_OK) {
		std::cout << "Error Creating IB" << std::endl;
		return;
	}



	//==================== Create Vertex Buffer =====================

	m_VBO[0] = { 0.0f,  1.0f, 0.0f ,1.0f,  0.0f, 0.0f };//Left Top
	m_VBO[1] = { 0.0f,  0.0f, 0.0f ,1.0f,  0.0f, 1.0f };//Left Bot
	m_VBO[2] = { 1.0f,  0.0f, 0.0f ,1.0f,  1.0f, 1.0f };//Right Bot
	m_VBO[3] = { 1.0f,  1.0f, 0.0f ,1.0f,  1.0f, 0.0f };//Right Top

	bdesc = { 0 };
	bdesc.ByteWidth = 4 * sizeof(fontVertex);
	bdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bdesc.Usage = D3D11_USAGE_DYNAMIC;
	subData = { &m_VBO[0], 0, 0 };

	if (D3D11Device->CreateBuffer(&bdesc, &subData, &m_VB) != S_OK) {
		std::cout << "Error Creating VB" << std::endl;
		return;
	}
}

void TextMeshD3D::Draw()
{
	float xSeparation = 0;
	for (size_t i = 0; i < m_text.size(); i++)
	{

		int x = m_text[i] - 32;
		//============= Update VBO =================
		float height = font.m_charInfo[x].height / textureWidth;
		float width = font.m_charInfo[x].width / textureWidth;
		fontVertex newVBO[4];
		newVBO[0] = { 0.0f, height, 0.0f, 1.0f, 0.0f,   0.0f };//Left Top
		newVBO[1] = { 0.0f,0.0f,0.0f ,1.0f,0.0f, height };//Left Bot
		newVBO[2] = { width ,  0.0f,  0.0f ,1.0f,	width, height };//Right Bot
		newVBO[3] = { width,  height, 0.0f ,1.0f,   width, 0.0f };//Right Top

		D3D11_MAPPED_SUBRESOURCE resource;
		ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));
		D3D11DeviceContext->Map(m_VB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, newVBO, sizeof(newVBO));
		D3D11DeviceContext->Unmap(m_VB.Get(), 0);

	

		
		xSeparation += font.m_charInfo[x].xoffset / textureWidth;
		float ySeparation = 0;//TODO:
	//==================== Set Constant Buffer info =====================
		CnstBuffer.matWorld = transform;
		CnstBuffer.xOffset = font.m_charInfo[x].x / textureWidth;
		CnstBuffer.yOffset = font.m_charInfo[x].y / textureWidth;
		CnstBuffer.xSeparation = xSeparation;
		CnstBuffer.ySeparation = ySeparation;
		//==================== Set Shaders =====================
		D3D11DeviceContext->VSSetShader(pVS.Get(), 0, 0);
		D3D11DeviceContext->PSSetShader(pFS.Get(), 0, 0);
		//==================== Set Input Layout (describe the input-buffer data) =====================
		D3D11DeviceContext->IASetInputLayout(Layout.Get());
		//==================== Update Constant Buffers =====================
		D3D11DeviceContext->UpdateSubresource(ConstantBuffer.Get(), 0, 0, &CnstBuffer, 0, 0);
		D3D11DeviceContext->VSSetConstantBuffers(0, 1, ConstantBuffer.GetAddressOf());
		D3D11DeviceContext->PSSetConstantBuffers(0, 1, ConstantBuffer.GetAddressOf());
		//=================== Set VB ===================
		UINT stride = sizeof(fontVertex);
		UINT offset = 0;
		D3D11DeviceContext->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &stride, &offset);
		//==================== Set IB =====================
		D3D11DeviceContext->IASetIndexBuffer(IB.Get(), DXGI_FORMAT_R16_UINT, 0);
		//==================== Set Texture =====================

		TextureD3D *texd3d = dynamic_cast<TextureD3D*>(difuseText);
		D3D11DeviceContext->PSSetShaderResources(0, 1, texd3d->pSRVTex.GetAddressOf());
		D3D11DeviceContext->PSSetSamplers(0, 1, texd3d->pSampler.GetAddressOf());
		//==================== Draw =====================
		D3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		D3D11DeviceContext->DrawIndexed(6, 0, 0);

		xSeparation += font.m_charInfo[x].xadvance / textureWidth;
	}
}

void TextMeshD3D::Destroy()
{
}

TextMeshD3D::TextMeshD3D()
{
}


TextMeshD3D::~TextMeshD3D()
{
}
#endif
