#pragma once
#include "Config.h"
#ifdef USING_D3D11
#include "TextMesh.h"
#include "TextureD3D.h"
#include <D3Dcompiler.h>
#include "PrimitiveBase.h"

#include <vector>
class TextMeshD3D :
	public TextMesh
{
private:
	struct ConstBuffer {
		Matrix4D matWorld;
		float xOffset;
		float yOffset;
		float xSeparation;
		float ySeparation;
	};
	ComPtr<ID3D11Buffer>  m_VB;
	ComPtr<ID3D11Buffer> IB;
	Texture*			 difuseText;

	ComPtr<ID3D11VertexShader>  pVS;
	ComPtr<ID3D11PixelShader>   pFS;
	ComPtr<ID3DBlob>            VS_blob;
	ComPtr<ID3DBlob>            FS_blob;
	ComPtr<ID3D11InputLayout>   Layout;

	std::vector<D3D11_INPUT_ELEMENT_DESC>	VertexDecl;
	ComPtr<ID3D11Buffer>        ConstantBuffer;
	TextMeshD3D::ConstBuffer		CnstBuffer;
	

public:
	void Create() override;
	void Draw() override;
	void Destroy() override;
	TextMeshD3D();
	~TextMeshD3D();
};
#endif

