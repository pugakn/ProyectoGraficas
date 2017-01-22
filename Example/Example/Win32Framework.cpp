#include "Win32Framework.h"
#include "GLDriver.h"
// SDL
#include <SDL/SDL.h>
// Windows 
#include <windows.h>
#include <mmsystem.h>

void Win32Framework::InitGlobalVars() {

	
}

void Win32Framework::OnCreateApplication(){
	SDL_Init(SDL_INIT_VIDEO);
	SDL_WM_SetCaption("UAD Framework Daniel", 0);
	int flags = SDL_HWSURFACE;
	//flags |= SDL_FULLSCREEN;
	//flags |= SDL_RESIZABLE;
	int width = 1280;
	int height = 720;
	SDL_SetVideoMode(width, height, 32, flags);

	pVideoDriver = new GLDriver;
	pVideoDriver->SetWindow(0);
	pVideoDriver->InitDriver();

	pBaseApp->CreateAssets();
}
void Win32Framework::OnDestroyApplication() {
	pBaseApp->DestroyAssets();
	pVideoDriver->DestroyDriver();
	delete pVideoDriver;
	SDL_Quit();
}
void Win32Framework::OnInterruptApplication() {
}
void Win32Framework::OnResumeApplication() {
}
void Win32Framework::UpdateApplication() {
	while (m_alive) {
		ProcessInput();

		pBaseApp->OnInput();
		pBaseApp->OnUpdate(0);
		pBaseApp->OnDraw();

	}
}
void Win32Framework::ProcessInput() {
	SDL_Event       evento;
	while (SDL_PollEvent(&evento)) {
		switch (evento.type) {
			case SDL_KEYDOWN: {
				if (evento.key.keysym.sym == SDLK_q) {
					m_alive = false;
				}
				switch (evento.key.keysym.sym)
				{
				case SDLK_a:
					pBaseApp->keyPressed = K_A;
					break;
				case SDLK_s:
					pBaseApp->keyPressed = K_S;
					break;
				case SDLK_d:
					pBaseApp->keyPressed = K_D;
					break;
				case SDLK_w:
					pBaseApp->keyPressed = K_W;
					break;
				default:
					break;
				}
			}break;
			case SDL_KEYUP: {
				pBaseApp->keyPressed = NULL;
			}break;
		}
	}
}
void Win32Framework::ResetApplication() {
}