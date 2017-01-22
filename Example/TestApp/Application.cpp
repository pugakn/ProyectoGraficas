#include "Application.h"
#include <Matrix4D.h>


void TestApp::CreateAssets() {
	keyPressed = 0;
	PrimitiveMgr.CreateTriangle();
}

void TestApp::DestroyAssets() {
	PrimitiveMgr.DestroyPrimitives();
}

void TestApp::OnUpdate(unsigned int dt) {

}

void TestApp::OnDraw() {
	pFramework->pVideoDriver->Clear();
	PrimitiveMgr.DrawPrimitives();
	pFramework->pVideoDriver->SwapBuffers();
}

void TestApp::OnInput() {
	if (keyPressed != NULL) {
		Matrix4D t;
		switch (keyPressed)
		{
		case K_A:
			t = Translation(-0.01, 0, 0);
			break;
		case K_S:
			t = Translation(0, -0.01, 0);
			break;
		case K_D:
			t = Translation(0.01, 0, 0);
			break;
		case K_W:
			t = Translation(0, 0.01, 0);
			break;
		}
		
		PrimitiveMgr.TransformPrimitive(0, t.v);
	}

}

void TestApp::OnPause() {

}

void TestApp::OnResume() {

}

void TestApp::OnReset() {

}