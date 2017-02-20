#ifndef UAD_INPUT_MANAGER_H
#define UAD_INPUT_MANAGER_H

#include <SDL/SDL_keysym.h>
#include "Vector3D.h"

#define MAXKEYS 512
#define MAXMOUSEBUTTONS 5


class InputManager {
public:
	InputManager() ;
	bool	PressedOnceKey(int key);
	bool	PressedOnceMouseButton(int mb);

	bool	PressedKey(int key);
	bool	PressedMouseButton(int mb);

	void setMouseMotion(float xM, float yM);
	
	bool	KeyStates[2][MAXKEYS];
	bool	MouseButtonStates[2][MAXMOUSEBUTTONS];

	Vector3D m_normalizedMousePos;

};

#endif