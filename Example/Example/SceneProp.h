/*********************************************************
* Copyright (C) 2017 Daniel Enriquez (camus_mm@hotmail.com)
* All Rights Reserved
*
* You may use, distribute and modify this code under the
* following terms:
* ** Do not claim that you wrote this software
* ** A mention would be appreciated but not needed
* ** I do not and will not provide support, this software is "as is"
* ** Enjoy, learn and share.
*********************************************************/

#ifndef UAD_SCENEPROPS_H
#define UAD_SCENEPROPS_H

#include "Vector3D.h"
#include "FPCamera.h"
#include <vector>

namespace RM {
	enum RenderMode
	{
		WIREFRAME,
		SOLID,
		SOLID_WIREFRAME
	};
}

struct Light{
	Vector3D Position;
	Vector3D Color;
	int		 Type;
	int		 Enabled;
};

struct LightWShadow {
	Vector3D Position;
	Vector3D Color;
	int		 Type;
	int		 Enabled;
	Matrix4D VP;
};

struct SceneProps{
	SceneProps() : ActiveCamera(0) , ActiveLights(0), renderMode(RM::SOLID) {}

	void	AddLight(Vector3D Pos, Vector3D Color,bool enabled);
	void	AddLightWShadow(Vector3D Pos, Vector3D Color, bool enabled, Matrix4D VP);
	void	RemoveLight(unsigned int index);
	void	SetLightPos(unsigned int index, Vector3D);

	void	AddCamera(FPCamera*);
	void	RemoveCamera(unsigned int index);
	

	std::vector<Light>	        Lights;
	std::vector<LightWShadow>	LightsWShadow;
	std::vector<FPCamera*> pCameras;

	Vector3D			AmbientColor;

	int	ActiveLights;
	int ActiveCamera;

	float specExp;
	float attMax;

	RM::RenderMode renderMode;
};

#endif