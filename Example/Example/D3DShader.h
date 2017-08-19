#pragma once
#include "Config.h"
#ifdef USING_D3D11
#include "BaseShader.h"
class D3DShader :
	public Shader
{
public:
	//struct ConstBuffer {
	//	Matrix4D WV;
	//	Matrix4D WVP;
	//	Matrix4D World;
	//	Vector4D lightDir;
	//	Vector4D lightColor;
	//	Vector4D camPos;
	//};

	//ConstBuffer		CnstBuffer;

	void LoadAPI()override;
	ComPtr<ID3D11VertexShader>  pVS;
	ComPtr<ID3D11PixelShader>   pFS;
	ComPtr<ID3DBlob>            VS_blob;
	ComPtr<ID3DBlob>            FS_blob;
	D3DShader();
	~D3DShader();
};
#endif

