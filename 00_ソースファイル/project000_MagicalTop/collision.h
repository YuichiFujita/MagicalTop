//============================================================
//
//	»θΦwb_[ [collision.h]
//	AuthorF‘cEκ
//
//============================================================
//************************************************************
//	ρdCN[hh~
//************************************************************
#ifndef _COLLISION_H_
#define _COLLISION_H_

//************************************************************
//	CN[ht@C
//************************************************************
#include "main.h"

//************************************************************
//	ΌOσΤιΎ
//************************************************************
// »θσΤ
namespace collision
{
	bool Box3D	// 3²Μι`Μ½θ»θ
	( // ψ
		D3DXVECTOR3 centerPos,		// »θΚu
		D3DXVECTOR3 targetPos,		// »θΪWΚu
		D3DXVECTOR3 centerSizeUp,	// »θTCY(EEγEγ)
		D3DXVECTOR3 centerSizeDown,	// »θTCY(ΆEΊEO)
		D3DXVECTOR3 targetSizeUp,	// »θΪWTCY(EEγEγ)
		D3DXVECTOR3 targetSizeDown	// »θΪWTCY(ΆEΊEO)
	);
	bool Circle2D	// XZ½ΚΜ~Μ½θ»θ
	( // ψ
		D3DXVECTOR3 centerPos,	// »θΚu
		D3DXVECTOR3 targetPos,	// »θΪWΚu
		float fCenterRadius,	// »θΌa
		float fTargetRadius		// »θΪWΌa
	);
	bool Circle3D	// 3²Μ~Μ½θ»θ
	( // ψ
		D3DXVECTOR3 centerPos,	// »θΚu
		D3DXVECTOR3 targetPos,	// »θΪWΚu
		float fCenterRadius,	// »θΌa
		float fTargetRadius		// »θΪWΌa
	);
	bool Sector	// XZ½ΚΜξ`Μ½θ»θ
	( // ψ
		D3DXVECTOR3 centerPos,	// »θΚu
		D3DXVECTOR3 targetPos,	// »θΪWΚu
		float fCenterRot,		// »θό«
		float fRadius,			// EΝΝ
		float fAngle			// μp
	);

	bool BoxPillar	// pΜΥΛ»θ
	( // ψ
		D3DXVECTOR3& rCenterPos,	// »θΚu
		D3DXVECTOR3& rCenterPosOld,	// »θίΚu
		D3DXVECTOR3 targetPos,		// »θΪWΚu
		D3DXVECTOR3 centerSizeUp,	// »θTCY(EEγEγ)
		D3DXVECTOR3 centerSizeDown,	// »θTCY(ΆEΊEO)
		D3DXVECTOR3 targetSizeUp,	// »θΪWTCY(EEγEγ)
		D3DXVECTOR3 targetSizeDown	// »θΪWTCY(ΆEΊEO)
	);
	bool CirclePillar	// ~ΜΥΛ»θ
	( // ψ
		D3DXVECTOR3& rCenterPos,	// »θΚu
		D3DXVECTOR3 targetPos,		// »θΪWΚu
		float fCenterRadius,		// »θΌa
		float fTargetRadius			// »θΪWΌa
	);

	float LineOuterProduct	// OΟΜΆE»θ
	( // ψ
		D3DXVECTOR3 posLeft,	// «EόΆΐW
		D3DXVECTOR3 posRight,	// «EόEΐW
		D3DXVECTOR3 pos			// »θΚu
	);
}

#endif	// _COLLISION_H_
