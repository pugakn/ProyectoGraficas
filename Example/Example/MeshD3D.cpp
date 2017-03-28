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

	char *vsSourceP = file2string("Shaders/VS_MeshPL.hlsl");
	char *fsSourceP = file2string("Shaders/FS_MeshPL.hlsl");

	std::string vstr = std::string(vsSourceP);
	std::string fstr = std::string(fsSourceP);

	m_meshInfo.resize(m_parser.m_meshes.size());
	int meshInfoIndex = 0;
	for (auto &meshIt : m_parser.m_meshes)
	{


		size_t subsetInfoIndex = 0;
		m_meshInfo[meshInfoIndex].m_subSets.resize(m_parser.m_meshes[meshInfoIndex].m_subsets.size()); //Resize subetInfo
		for (auto &subsetIt : meshIt.m_subsets)
		{
			SubSetInfo *it_subsetinfo =  &m_meshInfo[meshInfoIndex].m_subSets[subsetInfoIndex];


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

			if (subsetIt.m_effects.m_specularMap != "")
				Defines += "#define USE_SPEC_MAP\n\n";
			if (subsetIt.m_effects.m_glossMap != "")
				Defines += "#define USE_GLOSS_MAP\n\n";
			if (subsetIt.m_effects.m_normalMap != "")
				Defines += "#define USE_NORMAL_MAP\n\n";

			Defines += "#define USE_PIXELLIGHTING \n\n";
			Defines += "#define USING_ATENUATION \n\n";
			Defines += "#define USE_SPECULAR \n\n";

			vstr = Defines + vstr;
			fstr = Defines + fstr;
			if (!vsSourceP || !fsSourceP)
				return;
			//==================== compile VS =====================
			it_subsetinfo->VS_blob = nullptr;
			ComPtr<ID3DBlob> errorBlob = nullptr;
			if (D3DCompile(vstr.c_str(), vstr.size(), 0, 0, 0, "VS", "vs_5_0", 0, 0, &it_subsetinfo->VS_blob, &errorBlob) != S_OK) {
				if (errorBlob) {
					std::cout << "ErrorBlob shader" << (char*)errorBlob->GetBufferPointer();
					return;
				}
				if (it_subsetinfo->VS_blob) {
					return;
				}
			}
			//=========== Create VS ============
			if (D3D11Device->CreateVertexShader(it_subsetinfo->VS_blob->GetBufferPointer(), it_subsetinfo->VS_blob->GetBufferSize(), 0, &it_subsetinfo->pVS) != S_OK) {
				std::cout << "Error Creatong Vertex Shader" << std::endl;

				return;
			}
			//==================== compile PS =====================
			it_subsetinfo->FS_blob = nullptr;
			errorBlob.Reset();
			if (D3DCompile(fstr.c_str(), fstr.size(), 0, 0, 0, "FS", "ps_5_0", 0, 0, &it_subsetinfo->FS_blob, &errorBlob) != S_OK) {
				if (errorBlob) {
					std::cout << "ErrorBlob shader" << (char*)errorBlob->GetBufferPointer();
					return;
				}

				if (it_subsetinfo->FS_blob) {
					return;
				}
			}
			//=========== Create PS ==============
			if (D3D11Device->CreatePixelShader(it_subsetinfo->FS_blob->GetBufferPointer(), it_subsetinfo->FS_blob->GetBufferSize(), 0, &it_subsetinfo->pFS) != S_OK) {
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
			it_subsetinfo->VertexDecl.push_back(elementDesc);

			if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_NORMAL) {
				elementDesc.SemanticName = "NORMAL";
				elementDesc.SemanticIndex = 0;
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				elementDesc.InputSlot = 0;
				elementDesc.AlignedByteOffset = offset;
				elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				elementDesc.InstanceDataStepRate = 0;
				offset += 16;
				it_subsetinfo->VertexDecl.push_back(elementDesc);
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
				it_subsetinfo->VertexDecl.push_back(elementDesc);
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
				it_subsetinfo->VertexDecl.push_back(elementDesc);
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
				it_subsetinfo->VertexDecl.push_back(elementDesc);
			}

			//==================== Create Input Layout =====================
			if (D3D11Device->CreateInputLayout(&it_subsetinfo->VertexDecl[0], it_subsetinfo->VertexDecl.size(), it_subsetinfo->VS_blob->GetBufferPointer(), it_subsetinfo->VS_blob->GetBufferSize(), &it_subsetinfo->Layout) != S_OK) {
				std::cout << "Error Creating Input Layout" << std::endl;
				return;
			}
			D3D11DeviceContext->IASetInputLayout(it_subsetinfo->Layout.Get());

			//==================== Create Buffer Layout =====================
			D3D11_BUFFER_DESC bdesc = { 0 };
			bdesc.Usage = D3D11_USAGE_DEFAULT;
			bdesc.ByteWidth = sizeof(MeshD3D::ConstBuffer);
			bdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

			if (D3D11Device->CreateBuffer(&bdesc, 0, it_subsetinfo->ConstantBuffer.GetAddressOf()) != S_OK) {
				std::cout << "Error Creating Buffer Layout" << std::endl;
				return;
			}

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


			if (subsetIt.m_effects.m_specularMap != "") {
				bool SpecFound = false;
				for (std::size_t f = 0; f < m_Textures.size(); f++) {
					Texture *ttex = m_Textures[f];
					std::string ttstr = std::string(ttex->optname);
					if (ttstr == subsetIt.m_effects.m_specularMap.c_str()) {
						it_subsetinfo->specularText = ttex;
						SpecFound = true;
						break;
					}
				}
				if (!SpecFound) {
					Texture *tex = new TextureD3D;
					int textureID = tex->LoadTexture(const_cast<char*>(subsetIt.m_effects.m_specularMap.c_str()));
					if (textureID != -1) {
						m_Textures.push_back(tex);
						it_subsetinfo->specularText = tex;
					}
					else {
						std::cout << "Texture not Found" << std::endl;
						delete tex;
					}
				}
			}
			if (subsetIt.m_effects.m_glossMap != "") {
				bool GlossFound = false;
				for (std::size_t f = 0; f < m_Textures.size(); f++) {
					Texture *ttex = m_Textures[f];
					std::string ttstr = std::string(ttex->optname);
					if (ttstr == subsetIt.m_effects.m_glossMap.c_str()) {
						it_subsetinfo->glossText = ttex;
						GlossFound = true;
						break;
					}
				}
				if (!GlossFound) {
					Texture *tex = new TextureD3D;
					int textureID = tex->LoadTexture(const_cast<char*>(subsetIt.m_effects.m_glossMap.c_str()));
					if (textureID != -1) {
						m_Textures.push_back(tex);
						it_subsetinfo->glossText = tex;
					}
					else {
						std::cout << "Texture not Found" << std::endl;
						delete tex;
					}
				}
			}
			if (subsetIt.m_effects.m_normalMap != "") {
				bool NormalFound = false;
				for (std::size_t f = 0; f < m_Textures.size(); f++) {
					Texture *ttex = m_Textures[f];
					std::string ttstr = std::string(ttex->optname);
					if (ttstr == subsetIt.m_effects.m_normalMap.c_str()) {
						it_subsetinfo->normalText = ttex;
						NormalFound = true;
						break;
					}
				}
				if (!NormalFound) {
					Texture *tex = new TextureD3D;
					int textureID = tex->LoadTexture(const_cast<char*>(subsetIt.m_effects.m_normalMap.c_str()));
					if (textureID != -1) {
						m_Textures.push_back(tex);
						it_subsetinfo->normalText = tex;
					}
					else {
						std::cout << "Texture not Found" << std::endl;
						delete tex;
					}
				}
			}


			//==================== Create Index Buffer =====================
			bdesc = { 0 };
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

void MeshD3D::Draw(float * t)
{
	if (t)
		m_transform = t;
	Matrix4D VP = Matrix4D(pScProp->pCameras[0]->VP);
	Matrix4D WVP = m_transform*VP;
	Vector4D lightDir = Vector4D(Vector3D(pScProp->Lights[0].Position),0);
	Vector4D lightCol = Vector4D(Vector3D(pScProp->Lights[0].Color), 1);
	Vector4D camPos = Vector4D(Vector3D(pScProp->pCameras[0]->m_pos), 0);
	//==================== Set VB =====================
	UINT stride = sizeof(vertexStruct);
	UINT offset = 0;
	D3D11DeviceContext->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &stride, &offset);

	for (size_t i = 0; i < m_parser.m_meshes.size(); i++)
	{
		for (std::size_t k = 0; k < m_meshInfo[i].m_subSets.size(); k++) {
			SubSetInfo *it_subsetinfo = &m_meshInfo[i].m_subSets[k];

			//==================== Set Constant Buffer info =====================
			it_subsetinfo->CnstBuffer.WVP = WVP;
			it_subsetinfo->CnstBuffer.World = m_transform;
			it_subsetinfo->CnstBuffer.lightDir = lightDir;
			it_subsetinfo->CnstBuffer.lightColor = lightCol;
			it_subsetinfo->CnstBuffer.camPos = camPos;
			//==================== Set Shaders =====================
			D3D11DeviceContext->VSSetShader(it_subsetinfo->pVS.Get(), 0, 0);
			D3D11DeviceContext->PSSetShader(it_subsetinfo->pFS.Get(), 0, 0);
			//==================== Set Input Layout (describe the input-buffer data) =====================
			D3D11DeviceContext->IASetInputLayout(it_subsetinfo->Layout.Get());
			//==================== Update Constant Buffers =====================
			D3D11DeviceContext->UpdateSubresource(it_subsetinfo->ConstantBuffer.Get(), 0, 0, &it_subsetinfo->CnstBuffer, 0, 0);
			D3D11DeviceContext->VSSetConstantBuffers(0, 1, it_subsetinfo->ConstantBuffer.GetAddressOf());
			D3D11DeviceContext->PSSetConstantBuffers(0, 1, it_subsetinfo->ConstantBuffer.GetAddressOf());


			//==================== Set Texture =====================
			//size_t textNum = 0;

			TextureD3D *texd3d = dynamic_cast<TextureD3D*>(it_subsetinfo->difuseText);
			D3D11DeviceContext->PSSetShaderResources(0, 1, texd3d->pSRVTex.GetAddressOf());
			D3D11DeviceContext->PSSetSamplers(0, 1, texd3d->pSampler.GetAddressOf());
			if (m_parser.m_meshes[i].m_subsets[k].m_effects.m_specularMap != "") {
				texd3d = dynamic_cast<TextureD3D*>(it_subsetinfo->specularText);
				D3D11DeviceContext->PSSetShaderResources(1, 1, texd3d->pSRVTex.GetAddressOf());
				D3D11DeviceContext->PSSetSamplers(1, 1, texd3d->pSampler.GetAddressOf());
			}
			if (m_parser.m_meshes[i].m_subsets[k].m_effects.m_glossMap != "") {
				texd3d = dynamic_cast<TextureD3D*>(it_subsetinfo->glossText);
				D3D11DeviceContext->PSSetShaderResources(2, 1, texd3d->pSRVTex.GetAddressOf());
				D3D11DeviceContext->PSSetSamplers(2, 1, texd3d->pSampler.GetAddressOf());
			}
			if (m_parser.m_meshes[i].m_subsets[k].m_effects.m_normalMap != "") {
				texd3d = dynamic_cast<TextureD3D*>(it_subsetinfo->normalText);
				D3D11DeviceContext->PSSetShaderResources(3, 1, texd3d->pSRVTex.GetAddressOf());
				D3D11DeviceContext->PSSetSamplers(3, 1, texd3d->pSampler.GetAddressOf());
			}


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


