#include "Application.h"
#include <math.h>
#define TRIANGLES_SIZE 100
#define MAX_TIME 10
#define OFFSET 0.1
void TestApp::InitVars() {
	DtTimer.Init();
	Position	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Orientation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Scaling		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

void TestApp::CreateAssets() {	
	for (int i = 0; i < TRIANGLES_SIZE; i++)
	{
		PrimitiveMgr.CreateTriangle();
		times[i] = i*OFFSET;
	}


}

void TestApp::DestroyAssets() {
	PrimitiveMgr.DestroyPrimitives();
}

void TestApp::MoveSpiral() {
	for (int i = 0; i < TRIANGLES_SIZE; i++)
	{
		times[i] += DtTimer.GetDTSecs();
		if (times[i] >= MAX_TIME) {
			times[i] = 1;
		}
		
		D3DXVECTOR3 Pos((pow(.89, times[i])*sinf(times[i])), (pow(.89, times[i])*cosf(times[i])), 0);
		Scaling = D3DXVECTOR3(1 - OFFSET*times[i], 1 - OFFSET*times[i], 1 - OFFSET*times[i]);
		D3DXMATRIX WorldTranslate, WorldScale;
		D3DXMatrixTranslation(&WorldTranslate, Pos.x, Pos.y, Pos.z);
		D3DXMatrixScaling(&WorldScale, Scaling.x, Scaling.y, Scaling.z);
		WorldTransform = WorldScale*WorldTranslate;


		PrimitiveMgr.TransformPrimitive(i, &WorldTransform.m[0][0]);
	}
}
void TestApp::OnUpdate() {
	DtTimer.Update();
	OnInput();
	MoveSpiral();
	OnDraw();
}

void TestApp::OnDraw() {
	pFramework->pVideoDriver->Clear();
	PrimitiveMgr.DrawPrimitives();
	pFramework->pVideoDriver->SwapBuffers();
}

void TestApp::OnInput() {

	
}

void TestApp::OnPause() {

}

void TestApp::OnResume() {

}

void TestApp::OnReset() {

}