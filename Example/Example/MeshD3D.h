#pragma once
#include "Config.h"
#ifdef USING_D3D11
#include "TextureD3D.h"
#include <D3Dcompiler.h>
#include "PrimitiveBase.h"
#include "UtilsGL.h"

#include "MeshParser.h"
#include "Matrix4D.h"

#include <vector>


class MeshD3D : public PrimitiveBase
{
	struct ConstBuffer {
		Matrix4D WVP;
		Matrix4D World;
		Vector4D lightDir;
	};
private:
	struct SubSetInfo {
		ComPtr<ID3D11Buffer> IB;
		Texture*			 difuseText;
	};
	struct MeshInfo {
		ComPtr<ID3D11VertexShader>  pVS;
		ComPtr<ID3D11PixelShader>   pFS;
		ComPtr<ID3DBlob>            VS_blob;
		ComPtr<ID3DBlob>            FS_blob;
		ComPtr<ID3D11InputLayout>   Layout;
		ComPtr<ID3D11Buffer>        ConstantBuffer;

		std::vector<D3D11_INPUT_ELEMENT_DESC>	VertexDecl;
		MeshD3D::ConstBuffer					CnstBuffer;
		std::vector<SubSetInfo>					m_subSets;
	};

	ComPtr<ID3D11Buffer>  m_VB;
	std::string			  m_fileName;
	std::vector<Texture*> m_Textures;
	Matrix4D			  m_transform;

public:
	void SetFileName(char* fileName);
	void Create() override;
	void Transform(float *t) override;
	void Draw(float *t, float *vp, float *l) override;
	void Destroy() override;
	MeshParser m_parser;
	std::vector<MeshInfo> m_meshInfo;
	MeshD3D() {};
	~MeshD3D() {};
};
#endif

