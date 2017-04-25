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

#ifndef UAD_CONFIG_H
#define UAD_CONFIG_H

#define D3DX 1
#define OGLES20 2
#define OGLES30 3
#define OGL 4

#define DRIVER_SELECTED OGL

#if   DRIVER_SELECTED == OGLES20
#define USING_OPENGL_ES20
#elif DRIVER_SELECTED == OGLES30
#define USING_OPENGL_ES30
#elif DRIVER_SELECTED == D3DX
#define USING_D3D11
#elif DRIVER_SELECTED == OGL
#define USING_OPENGL
#else
#define USING_OPENGL // Default GL
#endif

#if defined(USING_OPENGL_ES20) || defined(USING_OPENGL_ES30) || defined(USING_OPENGL)
#define USING_GL_COMMON
#endif

#define USING_32BIT_IB 0

#endif