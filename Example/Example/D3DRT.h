#pragma once
#include "Config.h"
#include "BaseRT.h"
#include "TextureD3D.h"


#include <d3d11.h>
#include <dxgi.h>
#include <vector>
#include <wrl.h>
#include <wrl/client.h>
using namespace Microsoft::WRL;


class D3DRT : public BaseRT {
public:
	bool			LoadAPIRT();

	std::vector<ComPtr<ID3D11RenderTargetView>>		vD3D11RenderTargetView;
	std::vector<ComPtr<ID3D11Texture2D>>			vD3D11ColorTex;
	ComPtr<ID3D11Texture2D>							D3D11DepthTex;
	ComPtr<ID3D11DepthStencilView>					D3D11DepthStencilTargetView;
};

