#pragma once
#include "Config.h"
#ifdef USING_D3D11
#include "TextureD3D.h"
#include <D3Dcompiler.h>
#include "PrimitiveBase.h"
#include "UtilsGL.h"

#include "MeshParser.h"
#include "Matrix4D.h"
#include "Wireframe.h"

#include <vector>
#include "D3DShader.h"


class MeshD3D : public PrimitiveBase
{
	struct ConstBuffer {
		Matrix4D WV;
		Matrix4D WVP;
		Matrix4D World;
		Vector4D lightDir;
		Vector4D lightColor;
		Vector4D camPos;
	};
private:
	struct SubSetInfo {
		ComPtr<ID3D11Buffer> IB;
		Texture*			 difuseText;
		Texture*			 specularText;
		Texture*			 glossText;
		Texture*			 normalText;

		//ComPtr<ID3D11VertexShader>  pVS;
		//ComPtr<ID3D11PixelShader>   pFS;
		//ComPtr<ID3DBlob>            VS_blob;
		//ComPtr<ID3DBlob>            FS_blob;
		ComPtr<ID3D11InputLayout>   Layout;

		std::vector<D3D11_INPUT_ELEMENT_DESC>	VertexDecl;
		ComPtr<ID3D11Buffer>        ConstantBuffer;
		MeshD3D::ConstBuffer		CnstBuffer;

		unsigned long sig;
		D3DShader* m_shader;
		std::vector<Shader*> m_shaderSet;
	};
	struct MeshInfo {
		std::vector<SubSetInfo>					m_subSets;
		ComPtr<ID3D11Buffer>  m_VB;
	};

	struct WireframeInfo
	{
		ComPtr<ID3D11Buffer> IB;
		ComPtr<ID3D11VertexShader>  pVS;
		ComPtr<ID3D11PixelShader>   pFS;
		ComPtr<ID3DBlob>            VS_blob;
		ComPtr<ID3DBlob>            FS_blob;
		ComPtr<ID3D11InputLayout>   Layout;
		std::vector<D3D11_INPUT_ELEMENT_DESC>	VertexDecl;
		ComPtr<ID3D11Buffer>        ConstantBuffer;
		Matrix4D WVP;
	};

	std::string			  m_fileName;
	std::vector<Texture*> m_Textures;
	Matrix4D			  m_transform;

	Wireframe wireframe;
	WireframeInfo wire;

	inline void DrawMeshes(const Matrix4D& VP, const Matrix4D& WVP);
	inline void DrawWireframe(const Matrix4D& VP, const Matrix4D& WVP);

public:
	void SetFileName(char* fileName);
	void Create() override;
	void Transform(float *t) override;
	void Draw(float *t) override;
	void Destroy() override;
	void SetShaderType(Shader::TYPE type) override;


	void TransformBone(int index, Matrix4D t);
	void CalcCombinedMatrix(int index, Matrix4D t);
	MeshParser m_parser;
	std::vector<MeshInfo> m_meshInfo;
	MeshD3D() { useLight = true; };
	~MeshD3D() {};
};
#endif

