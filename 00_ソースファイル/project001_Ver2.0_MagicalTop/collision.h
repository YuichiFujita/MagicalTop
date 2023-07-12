//============================================================
//
//	判定関数ヘッダー [collision.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _COLLISION_H_
#define _COLLISION_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	名前空間宣言
//************************************************************
// 判定空間
namespace collision
{
	bool Box3D	// 3軸の矩形の当たり判定
	( // 引数
		D3DXVECTOR3 centerPos,		// 判定位置
		D3DXVECTOR3 targetPos,		// 判定目標位置
		D3DXVECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
		D3DXVECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
		D3DXVECTOR3 targetSizeUp,	// 判定目標サイズ(右・上・後)
		D3DXVECTOR3 targetSizeDown	// 判定目標サイズ(左・下・前)
	);
	bool Circle2D	// XZ平面の円の当たり判定
	( // 引数
		D3DXVECTOR3 centerPos,	// 判定位置
		D3DXVECTOR3 targetPos,	// 判定目標位置
		float fCenterRadius,	// 判定半径
		float fTargetRadius,	// 判定目標半径
		float *pLength = NULL	// 判定目標との距離
	);
	bool Circle3D	// 3軸の円の当たり判定
	( // 引数
		D3DXVECTOR3 centerPos,	// 判定位置
		D3DXVECTOR3 targetPos,	// 判定目標位置
		float fCenterRadius,	// 判定半径
		float fTargetRadius		// 判定目標半径
	);
	bool Sector	// XZ平面の扇形の当たり判定
	( // 引数
		D3DXVECTOR3 centerPos,	// 判定位置
		D3DXVECTOR3 targetPos,	// 判定目標位置
		float fCenterRot,		// 判定向き
		float fRadius,			// 視界範囲
		float fAngle,			// 視野角
		float *pLength = NULL	// 判定目標との距離
	);

	bool BoxPillar	// 角柱の衝突判定
	( // 引数
		D3DXVECTOR3& rCenterPos,	// 判定位置
		D3DXVECTOR3& rCenterPosOld,	// 判定過去位置
		D3DXVECTOR3 targetPos,		// 判定目標位置
		D3DXVECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
		D3DXVECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
		D3DXVECTOR3 targetSizeUp,	// 判定目標サイズ(右・上・後)
		D3DXVECTOR3 targetSizeDown	// 判定目標サイズ(左・下・前)
	);
	bool CirclePillar	// 円柱の衝突判定
	( // 引数
		D3DXVECTOR3& rCenterPos,	// 判定位置
		D3DXVECTOR3 targetPos,		// 判定目標位置
		float fCenterRadius,		// 判定半径
		float fTargetRadius			// 判定目標半径
	);

	float LineOuterProduct	// 外積の左右判定
	( // 引数
		D3DXVECTOR3 posLeft,	// 境界線左座標
		D3DXVECTOR3 posRight,	// 境界線右座標
		D3DXVECTOR3 pos			// 判定位置
	);
	bool TrianglePillar	// 三角柱の当たり判定
	( // 引数
		D3DXVECTOR3 pos0,	// 三角の各頂点
		D3DXVECTOR3 pos1,	// 三角の各頂点
		D3DXVECTOR3 pos2,	// 三角の各頂点
		D3DXVECTOR3 pos		// 判定位置
	);
}

#endif	// _COLLISION_H_
