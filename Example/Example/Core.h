#ifndef UAD_CORE_H
#define UAD_CORE_H

#define K_A 1
#define K_S 2
#define K_D 3
#define K_W 4

#include "BaseDriver.h"

class RootFramework;

class AppBase {
public:
	AppBase() : bInited(false), bPaused(false), pFramework(0) {}
	virtual void CreateAssets() = 0;
	virtual void DestroyAssets() = 0;
	virtual void OnUpdate(unsigned int dt) = 0;
	virtual void OnDraw() = 0;
	virtual void OnInput() = 0;
	virtual void OnPause() = 0;
	virtual void OnResume() = 0;
	virtual void OnReset() = 0;
	void	SetParentFramework(RootFramework* pParentFramework) {
		pFramework = pParentFramework;
	}
	bool			bInited;
	bool			bPaused;
	RootFramework	*pFramework;
	int keyPressed;
};

class RootFramework {
public:
	RootFramework(AppBase *pApp) : pBaseApp(pApp) {}
	virtual void InitGlobalVars() = 0;
	virtual void OnCreateApplication() = 0;
	virtual void OnDestroyApplication() = 0;
	virtual void OnInterruptApplication() = 0;
	virtual void OnResumeApplication() = 0;
	virtual void UpdateApplication() = 0;
	virtual void ProcessInput() = 0;
	virtual void ResetApplication() = 0;

	BaseDriver		*pVideoDriver;
	AppBase			*pBaseApp;
};

#endif