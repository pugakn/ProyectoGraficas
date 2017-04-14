#pragma once
#include "Config.h"
#ifdef USING_D3D11
#include "TextureD3D.h"
#include <D3Dcompiler.h>

#include <vector>
#include "Sprite.h"
class SpriteD3D : public Sprite
{
	struct ConstBuffer {
		Matrix4D matWorld;
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
	SpriteD3D::ConstBuffer		CnstBuffer;
public:
	void Create(Matrix4D &VP) override;
	void Draw() override;
	void Destroy() override;
	SpriteD3D();
	~SpriteD3D();
};
#endif

