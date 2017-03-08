#include "MeshD3D.h"
#ifdef USING_D3D11
#include "Timer.h"
#include <iostream>

extern ComPtr<ID3D11Device>            D3D11Device;
extern ComPtr<ID3D11DeviceContext>     D3D11DeviceContext;

void MeshD3D::SetFileName(char * fileName)
{
	m_fileName = std::string(fileName);
}

void MeshD3D::Create()
{
	//------------------------------------------------------------------------//
	//Leer Archivo .X
	Timer timer;
	timer.Init();
	if (!m_parser.LoadFile(m_fileName.c_str()))
	{
		std::cout << "Error: No se pudo cargar el modelo" << std::endl;
		return;
	}
	timer.Update();
	std::cout << "Archivo cargado en: " << timer.GetDTSecs() << " segundos..." << std::endl;
	//-------------------------------------------------------------------------//

	char *vsSourceP = file2string("Shaders/VS_Mesh.hlsl");
	char *fsSourceP = file2string("Shaders/FS_Mesh.hlsl");

	std::string vstr = std::string(vsSourceP);
	std::string fstr = std::string(fsSourceP);

	m_meshInfo.resize(m_parser.m_meshes.size());
	int meshInfoIndex = 0;
	for (auto &meshIt : m_parser.m_meshes)
	{
		std::string Defines = "";
		if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_NORMAL)
			Defines += "#define USE_NORMALS\n\n";
		if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_TEXCOORD0)
			Defines += "#define USE_TEXCOORD0\n\n";
		if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_TEXCOORD1)
			Defines += "#define USE_TEXCOORD1\n\n";
		if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_TANGENT)
			Defines += "#define USE_TANGENTS\n\n";
		if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_BINORMAL)
			Defines += "#define USE_BINORMALS\n\n";

		vstr = Defines + vstr;
		fstr = Defines + fstr;
		if (!vsSourceP || !fsSourceP)
			return;
		//==================== compile VS =====================
		m_meshInfo[meshInfoIndex].VS_blob = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;
		if (D3DCompile(vstr.c_str(), vstr.size(), 0, 0, 0, "VS", "vs_5_0", 0, 0, &m_meshInfo[meshInfoIndex].VS_blob, &errorBlob) != S_OK) {
			if (errorBlob) {
				std::cout << "ErrorBlob shader" << (char*)errorBlob->GetBufferPointer();
				return;
			}
			if (m_meshInfo[meshInfoIndex].VS_blob) {
				return;
			}
		}
		//=========== Create VS ============
		if (D3D11Device->CreateVertexShader(m_meshInfo[meshInfoIndex].VS_blob->GetBufferPointer(), m_meshInfo[meshInfoIndex].VS_blob->GetBufferSize(), 0, &m_meshInfo[meshInfoIndex].pVS) != S_OK) {
			std::cout << "Error Creatong Vertex Shader" << std::endl;

			return;
		}
		//==================== compile PS =====================
		m_meshInfo[meshInfoIndex].FS_blob = nullptr;
		errorBlob.Reset();
		if (D3DCompile(fstr.c_str(), fstr.size(), 0, 0, 0, "FS", "ps_5_0", 0, 0, &m_meshInfo[meshInfoIndex].FS_blob, &errorBlob) != S_OK) {
			if (errorBlob) {
				std::cout << "ErrorBlob shader" << (char*)errorBlob->GetBufferPointer();
				return;
			}

			if (m_meshInfo[meshInfoIndex].FS_blob) {
				return;
			}
		}
		//=========== Create PS ==============
		if (D3D11Device->CreatePixelShader(m_meshInfo[meshInfoIndex].FS_blob->GetBufferPointer(), m_meshInfo[meshInfoIndex].FS_blob->GetBufferSize(), 0, &m_meshInfo[meshInfoIndex].pFS) != S_OK) {
			std::cout << "Error Creating Pixel Shader" << std::endl;
			return;
		}

		//==================== Create Decl Data =====================
		int offset = 0;
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = "POSITION";
		elementDesc.SemanticIndex = 0;
		elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = offset;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;
		offset += 16;
		m_meshInfo[meshInfoIndex].VertexDecl.push_back(elementDesc);

		if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_NORMAL) {
			elementDesc.SemanticName = "NORMAL";
			elementDesc.SemanticIndex = 0;
			elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = offset;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;
			offset += 16;
			m_meshInfo[meshInfoIndex].VertexDecl.push_back(elementDesc);
		}

		if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_TANGENT) {
			elementDesc.SemanticName = "TANGENT";
			elementDesc.SemanticIndex = 0;
			elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = offset;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;
			offset += 16;
			m_meshInfo[meshInfoIndex].VertexDecl.push_back(elementDesc);
		}

		if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_BINORMAL) {
			elementDesc.SemanticName = "BINORMAL";
			elementDesc.SemanticIndex = 0;
			elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = offset;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;
			offset += 16;
			m_meshInfo[meshInfoIndex].VertexDecl.push_back(elementDesc);
		}

		if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_TEXCOORD0) {
			elementDesc.SemanticName = "TEXCOORD";
			elementDesc.SemanticIndex = 0;
			elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = offset;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;
			offset += 8;
			m_meshInfo[meshInfoIndex].VertexDecl.push_back(elementDesc);
		}

		//==================== Create Input Layout =====================
		if (D3D11Device->CreateInputLayout(&m_meshInfo[meshInfoIndex].VertexDecl[0], m_meshInfo[meshInfoIndex].VertexDecl.size(), m_meshInfo[meshInfoIndex].VS_blob->GetBufferPointer(), m_meshInfo[meshInfoIndex].VS_blob->GetBufferSize(), &m_meshInfo[meshInfoIndex].Layout) != S_OK) {
			std::cout << "Error Creating Input Layout" << std::endl;
			return;
		}
		D3D11DeviceContext->IASetInputLayout(m_meshInfo[meshInfoIndex].Layout.Get());

		//==================== Create Buffer Layout =====================
		D3D11_BUFFER_DESC bdesc = { 0 };
		bdesc.Usage = D3D11_USAGE_DEFAULT;
		bdesc.ByteWidth = sizeof(MeshD3D::ConstBuffer);
		bdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		if (D3D11Device->CreateBuffer(&bdesc, 0, m_meshInfo[meshInfoIndex].ConstantBuffer.GetAddressOf()) != S_OK) {
			std::cout << "Error Creating Buffer Layout" << std::endl;
			return;
		}


		size_t subsetInfoIndex = 0;
		m_meshInfo[meshInfoIndex].m_subSets.resize(m_parser.m_meshes[meshInfoIndex].m_subsets.size()); //Resize subetInfo
		for (auto &subsetIt : meshIt.m_subsets)
		{
			SubSetInfo *it_subsetinfo =  &m_meshInfo[meshInfoIndex].m_subSets[subsetInfoIndex];
			//=========================== Create Textures ===============================
			bool found = false;
			for (std::size_t f = 0; f < m_Textures.size(); f++) {
				Texture *ttex = m_Textures[f];
				std::string ttstr = std::string(ttex->optname);
				if (ttstr == subsetIt.m_effects.m_difusePath.c_str()) {
					it_subsetinfo->difuseText = ttex;
					found = true;
					break;
				}
			}
			if (!found) {
				Texture *tex = new TextureD3D;
				int textureID = tex->LoadTexture(const_cast<char*>(subsetIt.m_effects.m_difusePath.c_str()));
				if (textureID != -1) {
					m_Textures.push_back(tex);
					it_subsetinfo->difuseText = tex;
				}
				else {
					std::cout << "Texture not Found" << std::endl;
					delete tex;
				}
			}
			//==================== Create Index Buffer =====================
			D3D11_BUFFER_DESC bdesc = { 0 };
			bdesc.ByteWidth = subsetIt.m_indexBuffer.size() * sizeof(USHORT);
			bdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			D3D11_SUBRESOURCE_DATA subData = { &subsetIt.m_indexBuffer[0], 0, 0 };

			if (D3D11Device->CreateBuffer(&bdesc, &subData, &it_subsetinfo->IB) != S_OK) {
				std::cout << "Error Creating IB" << std::endl;
				return;
			}
			++subsetInfoIndex;
		}//End Subsets
		++meshInfoIndex;
	}//End Meshes
	 //==================== Create Vertex Buffer =====================
	D3D11_BUFFER_DESC bdesc = { 0 };
	bdesc.ByteWidth = m_parser.m_vertexSize * sizeof(vertexStruct);
	bdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA subData = { &m_parser.m_vbo[0], 0, 0 };

	if (D3D11Device->CreateBuffer(&bdesc, &subData, &m_VB) != S_OK) {
		std::cout << "Error Creating VB" << std::endl;
		return;
	}
	delete[] vsSourceP;
	delete[] fsSourceP;
}

void MeshD3D::Transform(float * t)
{
	m_transform = t;
}

void MeshD3D::Draw(float * t, float * vp)
{
	if (t)
		m_transform = t;
	Matrix4D VP = Matrix4D(vp);
	Matrix4D WVP = m_transform*VP;
	//==================== Set VB =====================
	UINT stride = sizeof(vertexStruct);
	UINT offset = 0;
	D3D11DeviceContext->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &stride, &offset);

	for (size_t i = 0; i < m_parser.m_meshes.size(); i++)
	{
		//==================== Set Constant Buffer info =====================
		m_meshInfo[i].CnstBuffer.WVP = WVP;
		m_meshInfo[i].CnstBuffer.World = m_transform;
		//==================== Set Shaders =====================
		D3D11DeviceContext->VSSetShader(m_meshInfo[i].pVS.Get(), 0, 0);
		D3D11DeviceContext->PSSetShader(m_meshInfo[i].pFS.Get(), 0, 0);
		//==================== Set Input Layout (describe the input-buffer data) =====================
		D3D11DeviceContext->IASetInputLayout(m_meshInfo[i].Layout.Get());
		//==================== Update Constant Buffers =====================
		D3D11DeviceContext->UpdateSubresource(m_meshInfo[i].ConstantBuffer.Get(), 0, 0, &m_meshInfo[i].CnstBuffer, 0, 0);
		D3D11DeviceContext->VSSetConstantBuffers(0, 1, m_meshInfo[i].ConstantBuffer.GetAddressOf());
		D3D11DeviceContext->PSSetConstantBuffers(0, 1, m_meshInfo[i].ConstantBuffer.GetAddressOf());

		for (std::size_t k = 0; k < m_meshInfo[i].m_subSets.size(); k++) {
			SubSetInfo *it_subsetinfo = &m_meshInfo[i].m_subSets[k];
			//==================== Set Texture =====================
			TextureD3D *texd3d = dynamic_cast<TextureD3D*>(it_subsetinfo->difuseText);
			D3D11DeviceContext->PSSetShaderResources(0, 1, texd3d->pSRVTex.GetAddressOf());
			D3D11DeviceContext->PSSetSamplers(0, 1, texd3d->pSampler.GetAddressOf());
			//==================== Set IB =====================
			D3D11DeviceContext->IASetIndexBuffer(it_subsetinfo->IB.Get(), DXGI_FORMAT_R16_UINT, 0);

			//==================== Draw =====================
			D3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			D3D11DeviceContext->DrawIndexed(m_parser.m_meshes[i].m_subsets[k].m_indexBuffer.size(), 0, 0);
		}
	}
}

void MeshD3D::Destroy()
{

}
#endif // USING_D3D11


