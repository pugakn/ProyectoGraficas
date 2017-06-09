#include "SceneProp.h"


void	SceneProps::AddLight(Vector3D Pos, Vector3D Color, bool enabled){
	Light l;
	l.Position=Pos;
	l.Color=Color;
	l.Enabled=(int)enabled;
	Lights.push_back(l);
}

void SceneProps::AddLightWShadow(Vector3D Pos, Vector3D Color, bool enabled, Matrix4D VP) {
	LightWShadow l;
	l.Position = Pos;
	l.Color = Color;
	l.Enabled = (int)enabled;
	//Matrix4D tmp = LookAtRH(Pos, target, up);
	l.VP = VP;
	LightsWShadow.push_back(l);
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