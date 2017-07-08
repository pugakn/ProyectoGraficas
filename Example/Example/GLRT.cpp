#include "GLRT.h"
#include "TextureGL.h"


bool GLRT::Load(int numRt, int colorf, int depthf, int w, int h)
{
	m_numRT = numRt;
	GLint depth_fmt = GL_DEPTH_COMPONENT;
	GLint color_fmt = GL_RGB;
	GLuint fbo;
	GLuint dtex;

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &dtex);
	glBindTexture(GL_TEXTURE_2D, dtex);
	glTexImage2D(GL_TEXTURE_2D, 0, depth_fmt, w, h, 0, depth_fmt, GL_UNSIGNED_INT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dtex, 0);

	TextureGL *pTextureDepth = new TextureGL;
	pTextureDepth->id = dtex;
	pTextureDepth->x = w;
	pTextureDepth->y = h;
	this->pDepthTexture = pTextureDepth;
	DepthTexture = dtex;
	for (int i = 0; i < numRt; i++) {

		GLuint ctex;
		glGenTextures(1, &ctex);
		glBindTexture(GL_TEXTURE_2D, ctex);

		glTexImage2D(GL_TEXTURE_2D, 0, color_fmt, w, h, 0, color_fmt, GL_UNSIGNED_BYTE, 0);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		TextureGL *pTextureColor = new TextureGL;
#if defined(USING_OPENGL_ES20)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ctex, 0);
		for (int i = 0; i < numRt; i++) {
			pTextureColor->id = ctex;
			vColorTextures.push_back(pTextureColor);
			vFrameBuffers.push_back(fbo);
			vGLColorTex.push_back(ctex);
		}
		break;
#else
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, ctex, 0);
#endif	
		pTextureColor->id = ctex;
		pTextureColor->x = w;
		pTextureColor->y = h;
		vColorTextures.push_back(pTextureColor);
		vFrameBuffers.push_back(fbo);
		vGLColorTex.push_back(ctex);
	}


	return true;
}
