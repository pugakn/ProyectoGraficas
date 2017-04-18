#pragma once
#include <vector>
class Texture;
class BaseRT {
public:
	enum ATTACHMENTS {
		COLOR0_ATTACHMENT = 1,
		COLOR1_ATTACHMENT = 2,
		COLOR2_ATTACHMENT = 4,
		COLOR3_ATTACHMENT = 8,
		DEPTH_ATTACHMENT = 16
	};

	enum FORMAT {
		FD16 = 0,
		F32,
		RGB8,
		RGBA8,
		RGBA32F,
		BGR8,
		BGRA8,
		BGRA32
	};
	bool			Load(int nrt, int cf, int df, int w, int h);
	std::vector<Texture*>	vColorTextures;
	Texture*				pDepthTexture;

	virtual bool	LoadAPIRT() = 0;

	int w;
	int h;
	int number_RT;
	int color_format;
	int depth_format;
};


