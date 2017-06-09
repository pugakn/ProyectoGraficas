#include "D3DShader.h"
#ifdef USING_D3D11
#include <iostream>
extern ComPtr<ID3D11Device>            D3D11Device;

void D3DShader::LoadAPI()
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

D3DShader::D3DShader()
{
}


D3DShader::~D3DShader()
{
}
#endif
