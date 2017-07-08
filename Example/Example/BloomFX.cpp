#include "BloomFX.h"
#include "Quad.h"
#include "GLRT.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))


void BloomFX::ApplyFX(Texture * deferredTexture)
{
	GLShader * actualSH;
	Matrix4D W = Identity();
	actualSH = illuminanceShader;
	Tools::UseRT(Quad::deferredRT_2);
	glUseProgram(actualSH->ShaderID);
	if (actualSH->m_locs.matWorldUniformLoc != -1)
		glUniformMatrix4fv(actualSH->m_locs.matWorldUniformLoc, 1, GL_FALSE, &W.m[0][0]);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glEnableVertexAttribArray(actualSH->m_locs.vertexAttribLocs);
	glEnableVertexAttribArray(actualSH->m_locs.uvAttribLocs);
	glVertexAttribPointer(actualSH->m_locs.vertexAttribLocs, 4, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(0));
	if (actualSH->m_locs.uvAttribLocs != -1)
		glVertexAttribPointer(actualSH->m_locs.uvAttribLocs, 2, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(16));
	if (actualSH->ShaderID != Tools::DefaultShaderID)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, deferredTexture->id);
		glUniform1i(actualSH->m_locs.textureLoc01, 0);
	}
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	if (actualSH->m_locs.uvAttribLocs != -1)
		glDisableVertexAttribArray(actualSH->m_locs.uvAttribLocs);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(actualSH->m_locs.vertexAttribLocs);
	glUseProgram(0);



	actualSH = HorizontalBlurShader;
	Tools::UseRT(Quad::deferredRT_1);
	glUseProgram(actualSH->ShaderID);
	if (actualSH->m_locs.matWorldUniformLoc != -1)
		glUniformMatrix4fv(actualSH->m_locs.matWorldUniformLoc, 1, GL_FALSE, &W.m[0][0]);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glEnableVertexAttribArray(actualSH->m_locs.vertexAttribLocs);
	glEnableVertexAttribArray(actualSH->m_locs.uvAttribLocs);
	glVertexAttribPointer(actualSH->m_locs.vertexAttribLocs, 4, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(0));
	if (actualSH->m_locs.uvAttribLocs != -1)
		glVertexAttribPointer(actualSH->m_locs.uvAttribLocs, 2, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(16));
	if (actualSH->ShaderID != Tools::DefaultShaderID)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Tools::RTs[Quad::deferredRT_2]->vColorTextures[0]->id);
		glUniform1i(actualSH->m_locs.textureLoc01, 0);
		float smsize[] = { static_cast<float>(Tools::RTs[Quad::deferredRT_2]->vColorTextures[0]->x) , static_cast<float>(Tools::RTs[Quad::deferredRT_2]->vColorTextures[0]->y) };
		glUniform2fv(actualSH->m_locs.ShadowMapSize, 1, &smsize[0]);
	}
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	if (actualSH->m_locs.uvAttribLocs != -1)
		glDisableVertexAttribArray(actualSH->m_locs.uvAttribLocs);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(actualSH->m_locs.vertexAttribLocs);
	glUseProgram(0);

	actualSH = VerticalBlurShader;
	Tools::UseRT(Quad::deferredRT_2);
	glUseProgram(actualSH->ShaderID);
	if (actualSH->m_locs.matWorldUniformLoc != -1)
		glUniformMatrix4fv(actualSH->m_locs.matWorldUniformLoc, 1, GL_FALSE, &W.m[0][0]);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glEnableVertexAttribArray(actualSH->m_locs.vertexAttribLocs);
	glEnableVertexAttribArray(actualSH->m_locs.uvAttribLocs);
	glVertexAttribPointer(actualSH->m_locs.vertexAttribLocs, 4, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(0));
	if (actualSH->m_locs.uvAttribLocs != -1)
		glVertexAttribPointer(actualSH->m_locs.uvAttribLocs, 2, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(16));
	if (actualSH->ShaderID != Tools::DefaultShaderID)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Tools::RTs[Quad::deferredRT_1]->vColorTextures[0]->id);
		glUniform1i(actualSH->m_locs.textureLoc01, 0);
		float smsize[] = { static_cast<float>(Tools::RTs[Quad::deferredRT_1]->vColorTextures[0]->x) , static_cast<float>(Tools::RTs[Quad::deferredRT_1]->vColorTextures[0]->y) };
		glUniform2fv(actualSH->m_locs.ShadowMapSize, 1, &smsize[0]);
	}
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	if (actualSH->m_locs.uvAttribLocs != -1)
		glDisableVertexAttribArray(actualSH->m_locs.uvAttribLocs);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(actualSH->m_locs.vertexAttribLocs);
	glUseProgram(0);


	actualSH = SumShader;
	Tools::UseRT(Quad::deferredRT_1);
	glUseProgram(actualSH->ShaderID);
	if (actualSH->m_locs.matWorldUniformLoc != -1)
		glUniformMatrix4fv(actualSH->m_locs.matWorldUniformLoc, 1, GL_FALSE, &W.m[0][0]);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glEnableVertexAttribArray(actualSH->m_locs.vertexAttribLocs);
	glEnableVertexAttribArray(actualSH->m_locs.uvAttribLocs);
	glVertexAttribPointer(actualSH->m_locs.vertexAttribLocs, 4, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(0));
	if (actualSH->m_locs.uvAttribLocs != -1)
		glVertexAttribPointer(actualSH->m_locs.uvAttribLocs, 2, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(16));
	if (actualSH->ShaderID != Tools::DefaultShaderID)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, deferredTexture->id);
		glUniform1i(actualSH->m_locs.textureLoc01, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Tools::RTs[Quad::deferredRT_2]->vColorTextures[0]->id);
		glUniform1i(actualSH->m_locs.textureLoc02, 1);

	}
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	if (actualSH->m_locs.uvAttribLocs != -1)
		glDisableVertexAttribArray(actualSH->m_locs.uvAttribLocs);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(actualSH->m_locs.vertexAttribLocs);
	glUseProgram(0);


	Tools::UseOriginalFBO();
}

void BloomFX::InitAPI()
{
	illuminanceShader = new GLShader();
	HorizontalBlurShader = new GLShader();
	VerticalBlurShader = new GLShader();
	SumShader = new GLShader();
	illuminanceShader->Load("Shaders/FX/VS_illuminance.glsl", "Shaders/FX/FS_illuminance.glsl", 0, (Shader::TYPE)0);
	HorizontalBlurShader->Load("Shaders/FX/VS_hBlur.glsl", "Shaders/FX/FS_hBlur.glsl", 0, (Shader::TYPE)0);
	VerticalBlurShader->Load("Shaders/FX/VS_vBlur.glsl", "Shaders/FX/FS_vBlur.glsl", 0, (Shader::TYPE)0);
	SumShader->Load("Shaders/FX/VS_BlurSum.glsl", "Shaders/FX/FS_BlurSum.glsl", 0, (Shader::TYPE)0);
	
}

BloomFX::BloomFX()
{
}


BloomFX::~BloomFX()
{
}
