#include "SpriteD3D.h"
#ifdef USING_D3D11
#include <iostream>
extern ComPtr<ID3D11Device>            D3D11Device;
extern ComPtr<ID3D11DeviceContext>     D3D11DeviceContext;

void SpriteD3D::Create(Matrix4D & VP)
{
	bool errorShader = false;
	m_VP = VP;
	transform = Identity();
	Scale = Identity();
	Position = Identity();

	char *vsSource = file2string("Shaders/VS_Sprite.hlsl");
	char *fsSource = file2string("Shaders/FS_Sprite.hlsl");
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
	//==================== compile VS =====================
	if (!errorShader)
	{
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
	else {
		//=========================== Create Textures ===============================
		int textureID = Tools::LoadTexture(m_str);
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
	bdesc.ByteWidth = errorShader ? sizeof(Matrix4D) : sizeof(SpriteD3D::ConstBuffer);
	bdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	if (D3D11Device->CreateBuffer(&bdesc, 0, ConstantBuffer.GetAddressOf()) != S_OK) {
		std::cout << "Error Creating Buffer Layout" << std::endl;
		return;
	}

	D3D11DeviceContext->VSSetConstantBuffers(0, 1, ConstantBuffer.GetAddressOf());
	D3D11DeviceContext->PSSetConstantBuffers(0, 1, ConstantBuffer.GetAddressOf());

	//==================== Create Vertex Buffer =====================

	m_VBO[0] = { 0.0f,  1.0f, 0.0f ,1.0f,  0.0f, 0.0f };//Left Top
	m_VBO[1] = { 0.0f,  0.0f, 0.0f ,1.0f,  0.0f, 1.0f };//Left Bot
	m_VBO[2] = { 1.0f,  0.0f, 0.0f ,1.0f,  1.0f, 1.0f };//Right Bot
	m_VBO[3] = { 1.0f,  1.0f, 0.0f ,1.0f,  1.0f, 0.0f };//Right Top

	bdesc = { 0 };
	bdesc.ByteWidth = 4 * sizeof(spriteVertex);
	bdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA subData = { m_VBO, 0, 0 };

	if (D3D11Device->CreateBuffer(&bdesc, &subData, &m_VB) != S_OK) {
		std::cout << "Error Creating VB" << std::endl;
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
	subData = { m_indexBuffer, 0, 0 };

	if (D3D11Device->CreateBuffer(&bdesc, &subData, &IB) != S_OK) {
		std::cout << "Error Creating IB" << std::endl;
		return;
	}
}

void SpriteD3D::Draw()
{

	//==================== Set Constant Buffer info =====================
	CnstBuffer.matWorld = transform*m_VP;
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
	UINT stride = sizeof(spriteVertex);
	UINT offset = 0;
	D3D11DeviceContext->IASetVertexBuffers(0,1, m_VB.GetAddressOf(),&stride,&offset);
	//==================== Set IB =====================
	D3D11DeviceContext->IASetIndexBuffer(IB.Get(), DXGI_FORMAT_R16_UINT, 0);
	//==================== Set Texture =====================

	TextureD3D *texd3d = dynamic_cast<TextureD3D*>(difuseText);
	D3D11DeviceContext->PSSetShaderResources(0, 1, texd3d->pSRVTex.GetAddressOf());
	D3D11DeviceContext->PSSetSamplers(0, 1, texd3d->pSampler.GetAddressOf());
	//==================== Draw =====================
	D3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D11DeviceContext->DrawIndexed(6, 0, 0);
	
}

void SpriteD3D::Destroy()
{
}

SpriteD3D::SpriteD3D()
{
}


SpriteD3D::~SpriteD3D()
{
}
#endif
