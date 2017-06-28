#include "MeshD3D.h"
#ifdef USING_D3D11
#include "Timer.h"
#include <iostream>
#include "D3DShader.h"
#include "ShaderManager.h"
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

	m_meshInfo.resize(m_parser.m_meshes.size());
	int meshInfoIndex = 0;
	for (auto &meshIt : m_parser.m_meshes)
	{
		size_t subsetInfoIndex = 0;
		m_meshInfo[meshInfoIndex].m_subSets.resize(m_parser.m_meshes[meshInfoIndex].m_subsets.size()); //Resize subetInfo
		for (auto &subsetIt : meshIt.m_subsets)
		{
			SubSetInfo *it_subsetinfo =  &m_meshInfo[meshInfoIndex].m_subSets[subsetInfoIndex];

			if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_NORMAL)
				it_subsetinfo->sig |= Shader::HAS_NORMALS;
			if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_TEXCOORD0)
				it_subsetinfo->sig |= Shader::HAS_TEXCOORD0;
			if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_TEXCOORD1)
				it_subsetinfo->sig |= Shader::HAS_TEXCOORD1;
			if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_TANGENT)
				it_subsetinfo->sig |= Shader::HAS_TANGENTS;
			if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_BINORMAL)
				it_subsetinfo->sig |= Shader::HAS_BINORMALS;
			//if (subsetIt.m_effects.m_glossMap != "")
			//	Defines += "#define USE_GLOSS_MAP\n\n";
			if (subsetIt.m_effects.m_normalMap != "")
				it_subsetinfo->sig |= Shader::USE_NORMAL_MAP;
			if (subsetIt.m_effects.m_specularMap != "")
				it_subsetinfo->sig |= Shader::USE_SPEC_MAP;
			if (!useLight)
				it_subsetinfo->sig |= Shader::NOT_LIGHT;

			//it_subsetinfo->m_shader  = (D3DShader*)ShaderManager::GetShaderBySignature(it_subsetinfo->sig);
			//it_subsetinfo->VS_blob = it_subsetinfo->m_shader->VS_blob;
			//it_subsetinfo->FS_blob = it_subsetinfo->m_shader->FS_blob;
			//it_subsetinfo->pVS = it_subsetinfo->m_shader->pVS;
			//it_subsetinfo->pFS = it_subsetinfo->m_shader->pFS;



			auto  set = ShaderManager::GetShaderSetBySignature(it_subsetinfo->sig);
			it_subsetinfo->m_shaderSet = set;
			m_shaderType = Shader::TYPE::G_FORWARD_PASS;
			it_subsetinfo->m_shader = (D3DShader*)(it_subsetinfo->m_shaderSet)[m_shaderType];

			if (it_subsetinfo->m_shader->errorShader)
			{
				meshIt.m_vertexAttributes = 0;
				//it_subsetinfo->VS_blob = Tools::DefaultVS_blob; //TODO: Add default shader
				//it_subsetinfo->pVS = Tools::pDefaultVS;
				//it_subsetinfo->FS_blob = Tools::DefaultFS_blob;
				//it_subsetinfo->pFS = Tools::pDefaultFS;
			}
			else
			{
				//=========================== Create Textures ===============================
				int textureID = Tools::LoadTexture(subsetIt.m_effects.m_difusePath.c_str());
				it_subsetinfo->difuseText = Tools::GetTexture(textureID);
				if (subsetIt.m_effects.m_specularMap != "")
				{
					textureID = Tools::LoadTexture(subsetIt.m_effects.m_specularMap.c_str());
					it_subsetinfo->specularText = Tools::GetTexture(textureID);
				}
				if (subsetIt.m_effects.m_glossMap != "")
				{
					textureID = Tools::LoadTexture(subsetIt.m_effects.m_glossMap.c_str());
					it_subsetinfo->glossText = Tools::GetTexture(textureID);
				}
				if (subsetIt.m_effects.m_normalMap != "")
				{
					textureID = Tools::LoadTexture(subsetIt.m_effects.m_normalMap.c_str());
					it_subsetinfo->normalText = Tools::GetTexture(textureID);
				}
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
			if (D3D11Device->CreateInputLayout(&it_subsetinfo->VertexDecl[0], it_subsetinfo->VertexDecl.size(), it_subsetinfo->m_shader->VS_blob->GetBufferPointer(), it_subsetinfo->m_shader->VS_blob->GetBufferSize(), &it_subsetinfo->Layout) != S_OK) {
				std::cout << "Error Creating Input Layout" << std::endl;
				return;
			}
			D3D11DeviceContext->IASetInputLayout(it_subsetinfo->Layout.Get());


			//==================== Create Buffer Layout =====================
			D3D11_BUFFER_DESC bdesc = { 0 };
			bdesc.Usage = D3D11_USAGE_DEFAULT;
			bdesc.ByteWidth = it_subsetinfo->m_shader->errorShader ? sizeof(Matrix4D) : sizeof(MeshD3D::ConstBuffer);
			bdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

			if (D3D11Device->CreateBuffer(&bdesc, 0, it_subsetinfo->ConstantBuffer.GetAddressOf()) != S_OK) {
				std::cout << "Error Creating Buffer Layout" << std::endl;
				return;
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
	//delete[] vsSourceP;
	//delete[] fsSourceP;

	// ================================================== Wireframe =============================================
	wire.pVS = Tools::pDefaultVS;
	wire.pFS = Tools::pDefaultFS;
	wire.VS_blob = Tools::DefaultVS_blob;
	wire.FS_blob = Tools::DefaultFS_blob;

	//==================== Create Decl Data =====================
	D3D11_INPUT_ELEMENT_DESC elementDesc;
	elementDesc.SemanticName = "POSITION";
	elementDesc.SemanticIndex = 0;
	elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elementDesc.InputSlot = 0;
	elementDesc.AlignedByteOffset = 0;
	elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elementDesc.InstanceDataStepRate = 0;
	wire.VertexDecl.push_back(elementDesc);


	//==================== Create Input Layout =====================
	if (D3D11Device->CreateInputLayout(&wire.VertexDecl[0], wire.VertexDecl.size(), wire.VS_blob->GetBufferPointer(), wire.VS_blob->GetBufferSize(), &wire.Layout) != S_OK) {
		std::cout << "Error Creating Input Layout" << std::endl;
		return;
	}
	D3D11DeviceContext->IASetInputLayout(wire.Layout.Get());

	//==================== Create Buffer Layout =====================
	bdesc = { 0 };
	bdesc.Usage = D3D11_USAGE_DEFAULT;
	bdesc.ByteWidth = sizeof(Matrix4D);
	bdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	if (D3D11Device->CreateBuffer(&bdesc, 0, wire.ConstantBuffer.GetAddressOf()) != S_OK) {
		std::cout << "Error Creating Buffer Layout" << std::endl;
		return;
	}

	//==================== Create Index Buffer =====================
	std::vector<unsigned short> fullIndex;
	for (auto &it : m_parser.m_vbo, m_parser.m_meshes)
	{
		fullIndex.insert(fullIndex.end(), it.m_indexBuffer.begin(), it.m_indexBuffer.end());
	}
	wireframe.CreateWireframe(fullIndex);
	bdesc = { 0 };
	bdesc.ByteWidth = wireframe.m_indexBuffer.size() * sizeof(USHORT);
	bdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	subData = { &wireframe.m_indexBuffer[0], 0, 0 };

	if (D3D11Device->CreateBuffer(&bdesc, &subData, &wire.IB) != S_OK) {
		std::cout << "Error Creating IB" << std::endl;
		return;
	}



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

	//==================== Set VB =====================
	UINT stride = sizeof(vertexStruct);
	UINT offset = 0;
	D3D11DeviceContext->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &stride, &offset);
	switch (pScProp->renderMode)
	{
	case RM::RenderMode::SOLID:
	{
		DrawMeshes(VP, WVP);
		break;
	}
	case RM::RenderMode::WIREFRAME:
		DrawWireframe(VP, WVP);
		break;
	case RM::RenderMode::SOLID_WIREFRAME:
		DrawMeshes(VP, WVP);
		DrawWireframe(VP, WVP);
		break;
	}
}


inline void MeshD3D::DrawMeshes(const Matrix4D& VP, const Matrix4D & WVP)
{
	Vector4D lightDir = Vector4D(Vector3D(pScProp->Lights[0].Position), 0);
	Vector4D lightCol = Vector4D(Vector3D(pScProp->Lights[0].Color), 1);
	Vector4D camPos = Vector4D(Vector3D(pScProp->pCameras[0]->m_pos), 0);
	for (size_t i = 0; i < m_parser.m_meshes.size(); i++)
	{
		for (std::size_t k = 0; k < m_meshInfo[i].m_subSets.size(); k++) {
			SubSetInfo *it_subsetinfo = &m_meshInfo[i].m_subSets[k];

			//==================== Set Constant Buffer info =====================
			it_subsetinfo->CnstBuffer.WV = m_transform*pScProp->pCameras[0]->m_view;
			it_subsetinfo->CnstBuffer.WVP = WVP;
			it_subsetinfo->CnstBuffer.World = m_transform;
			it_subsetinfo->CnstBuffer.lightDir = lightDir;
			it_subsetinfo->CnstBuffer.lightColor = lightCol;
			it_subsetinfo->CnstBuffer.camPos = camPos;
			//==================== Set Shaders =====================
			D3D11DeviceContext->VSSetShader(it_subsetinfo->m_shader->pVS.Get(), 0, 0);
			D3D11DeviceContext->PSSetShader(it_subsetinfo->m_shader->pFS.Get(), 0, 0);
			//==================== Set Input Layout (describe the input-buffer data) =====================
			D3D11DeviceContext->IASetInputLayout(it_subsetinfo->Layout.Get());
			//==================== Update Constant Buffers =====================
			D3D11DeviceContext->UpdateSubresource(it_subsetinfo->ConstantBuffer.Get(), 0, 0, &it_subsetinfo->CnstBuffer, 0, 0);
			D3D11DeviceContext->VSSetConstantBuffers(0, 1, it_subsetinfo->ConstantBuffer.GetAddressOf());
			D3D11DeviceContext->PSSetConstantBuffers(0, 1, it_subsetinfo->ConstantBuffer.GetAddressOf());


			//==================== Set Texture =====================
			//size_t textNum = 0;
			TextureD3D *texd3d = dynamic_cast<TextureD3D*>(it_subsetinfo->difuseText);
			if (texd3d != nullptr)
			{
				D3D11DeviceContext->PSSetShaderResources(0, 1, texd3d->pSRVTex.GetAddressOf());
				D3D11DeviceContext->PSSetSamplers(0, 1, texd3d->pSampler.GetAddressOf());
			}
			if (m_parser.m_meshes[i].m_subsets[k].m_effects.m_specularMap != "") {
				texd3d = dynamic_cast<TextureD3D*>(it_subsetinfo->specularText);
				if (texd3d != nullptr)
				{
					D3D11DeviceContext->PSSetShaderResources(1, 1, texd3d->pSRVTex.GetAddressOf());
					D3D11DeviceContext->PSSetSamplers(1, 1, texd3d->pSampler.GetAddressOf());
				}
			}
			if (m_parser.m_meshes[i].m_subsets[k].m_effects.m_glossMap != "") {
				texd3d = dynamic_cast<TextureD3D*>(it_subsetinfo->glossText);
				if (texd3d != nullptr)
				{
					D3D11DeviceContext->PSSetShaderResources(2, 1, texd3d->pSRVTex.GetAddressOf());
					D3D11DeviceContext->PSSetSamplers(2, 1, texd3d->pSampler.GetAddressOf());
				}
			}
			if (m_parser.m_meshes[i].m_subsets[k].m_effects.m_normalMap != "") {
				texd3d = dynamic_cast<TextureD3D*>(it_subsetinfo->normalText);
				if (texd3d != nullptr)
				{
					D3D11DeviceContext->PSSetShaderResources(3, 1, texd3d->pSRVTex.GetAddressOf());
					D3D11DeviceContext->PSSetSamplers(3, 1, texd3d->pSampler.GetAddressOf());
				}
			}
			//==================== Set IB =====================
			D3D11DeviceContext->IASetIndexBuffer(it_subsetinfo->IB.Get(), DXGI_FORMAT_R16_UINT, 0);

			//==================== Draw =====================
			D3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			D3D11DeviceContext->DrawIndexed(m_parser.m_meshes[i].m_subsets[k].m_indexBuffer.size(), 0, 0);
		}
	}
}

inline void MeshD3D::DrawWireframe(const Matrix4D& VP, const Matrix4D & WVP)
{
	//==================== Set Constant Buffer info =====================
	wire.WVP = WVP;
	//==================== Set Shaders =====================
	D3D11DeviceContext->VSSetShader(wire.pVS.Get(), 0, 0);
	D3D11DeviceContext->PSSetShader(wire.pFS.Get(), 0, 0);
	//==================== Set Input Layout (describe the input-buffer data) =====================
	D3D11DeviceContext->IASetInputLayout(wire.Layout.Get());
	//==================== Update Constant Buffers =====================
	D3D11DeviceContext->UpdateSubresource(wire.ConstantBuffer.Get(), 0, 0, &wire.WVP, 0, 0);
	D3D11DeviceContext->VSSetConstantBuffers(0, 1, wire.ConstantBuffer.GetAddressOf());
	D3D11DeviceContext->PSSetConstantBuffers(0, 1, wire.ConstantBuffer.GetAddressOf());

	//==================== Set IB =====================
	D3D11DeviceContext->IASetIndexBuffer(wire.IB.Get(), DXGI_FORMAT_R16_UINT, 0);

	//==================== Draw =====================
	D3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	D3D11DeviceContext->DrawIndexed(wireframe.m_indexBuffer.size(), 0, 0);
}
void MeshD3D::Destroy()
{

}
void MeshD3D::SetShaderType(Shader::TYPE type)
{
	if (m_shaderType != type)
	{
		m_shaderType = type;
		for (auto &meshIt : m_meshInfo)
		{
			for (auto &subsetIt : meshIt.m_subSets)
			{
				subsetIt.m_shader = (D3DShader*)(subsetIt.m_shaderSet)[type];
			}
		}
	}
}
#endif // USING_D3D11


