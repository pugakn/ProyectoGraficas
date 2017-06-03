#ifndef UAD_PRIMITIVE_INSTANCE_H
#define UAD_PRIMITIVE_INSTANCE_H

#include "PrimitiveBase.h"
#include "Matrix4D.h"


class PrimitiveInst {
	public:
		void	CreateInstance(PrimitiveBase *pPrim) {
			pBase = pPrim;
			//pViewProj = pVP;	
			//lightDir = l;

			Position = Identity();
			Scale = Identity();
			m_RotationX = Identity();
			m_RotationY = Identity();
			m_RotationZ = Identity();
			Final = Identity();

		}

		void	TranslateAbsolute(float x, float y, float z);
		void	RotateXAbsolute(float ang);
		void	RotateYAbsolute(float ang);
		void	RotateZAbsolute(float ang);
		void	ScaleAbsolute(float sc);

		void	TranslateRelative(float x, float y, float z);
		void	RotateXRelative(float ang);
		void	RotateYRelative(float ang);
		void	RotateZRelative(float ang);
		void	ScaleRelative(float sc);

		void	Update();
		void	Draw();
		void SetShaderByGlobalSignature(unsigned long gsig);
		//void SetShaderLocalSignature();

		Matrix4D		Position;
		Matrix4D		Scale;
		Matrix4D		m_RotationX;
		Matrix4D		m_RotationY;
		Matrix4D		m_RotationZ;
		Matrix4D		Final;

		Matrix4D		*pViewProj;
		Vector3D		*lightDir;

		PrimitiveBase	*pBase;
};


#endif