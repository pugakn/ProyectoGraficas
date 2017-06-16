#include "SceneProp.h"


void	SceneProps::AddLight(Vector3D Pos, Vector3D Color, bool enabled){
	Light l;
	l.Position=Pos;
	l.Color=Color;
	l.Enabled=(int)enabled;
	Lights.push_back(l);
}

void SceneProps::AddLightWShadow(Vector3D Pos, Vector3D Color, bool enabled, Vector3D target) {
	LightWShadow l;
	l.Position = Pos;
	l.Color = Color;
	l.Enabled = (int)enabled;
	Matrix4D shadowLightVP = LookAtRH(Pos, target, Vector3D(0, 1, 0));
	shadowLightVP = shadowLightVP* shadowLightProj;
	l.VP = shadowLightVP;
	l.dir = Normalize(Pos - target);
	LightsWShadow.push_back(l);
}
void SceneProps::ModifyLightWShadow(int index, Vector3D Pos, Vector3D Color, bool enabled, Vector3D target) {
	LightWShadow l;
	l.Position = Pos;
	l.Color = Color;
	l.Enabled = (int)enabled;
	Matrix4D shadowLightVP = LookAtRH(Pos, target, Vector3D(0, 1, 0));
	shadowLightVP = shadowLightVP*shadowLightProj;
	l.VP = shadowLightVP;
	l.dir = Normalize(Pos - target);
	LightsWShadow[index] = (l);
}

void	SceneProps::RemoveLight(unsigned int index){
	if(index < 0 || index >= Lights.size())
		return;

	Lights.erase(Lights.begin() + index);
}

void	SceneProps::SetLightPos(unsigned int index, Vector3D pos){
	if (index < 0 || index >= Lights.size())
		return;

	Lights[index].Position = pos;
}

void	SceneProps::AddCamera(FPCamera* cam){
	pCameras.push_back(cam);
}

void	SceneProps::RemoveCamera(unsigned int index){
	if (index < 0 || index >= pCameras.size())
		return;

	pCameras.erase(pCameras.begin() + index);
}