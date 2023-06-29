//============================================================
//
//	判定関数処理 [collision.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "collision.h"

//============================================================
//	3軸矩形の当たり判定
//============================================================
bool collision::Box3D
(
	D3DXVECTOR3 centerPos,		// 判定位置
	D3DXVECTOR3 targetPos,		// 判定目標位置
	D3DXVECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
	D3DXVECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
	D3DXVECTOR3 targetSizeUp,	// 判定目標サイズ(右・上・後)
	D3DXVECTOR3 targetSizeDown	// 判定目標サイズ(左・下・前)
)
{
	if (centerPos.x + centerSizeUp.x   > targetPos.x - targetSizeDown.x
	&&  centerPos.y + centerSizeUp.y   > targetPos.y - targetSizeDown.y
	&&  centerPos.z + centerSizeUp.z   > targetPos.z - targetSizeDown.z
	&&  centerPos.x - centerSizeDown.x < targetPos.x + targetSizeUp.x
	&&  centerPos.y - centerSizeDown.y < targetPos.y + targetSizeUp.y
	&&  centerPos.z - centerSizeDown.z < targetPos.z + targetSizeUp.z)
	{ // 判定内の場合

		// 真を返す
		return true;
	}

	// 偽を返す
	return false;
}

//============================================================
//	柱の衝突判定
//============================================================
bool collision::Pillar
(
	D3DXVECTOR3& rCenterPos,	// 判定位置
	D3DXVECTOR3& rCenterPosOld,	// 判定過去位置
	D3DXVECTOR3 targetPos,		// 判定目標位置
	D3DXVECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
	D3DXVECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
	D3DXVECTOR3 targetSizeUp,	// 判定目標サイズ(右・上・後)
	D3DXVECTOR3 targetSizeDown	// 判定目標サイズ(左・下・前)
)
{
	// 変数を宣言
	bool bHit = false;	// 衝突判定結果

	// 前後の当たり判定
	if (rCenterPos.x + centerSizeUp.x > targetPos.x - targetSizeDown.x
	&&  rCenterPos.x - centerSizeDown.x < targetPos.x + targetSizeUp.x)
	{ // 左右の範囲内の場合

		if (rCenterPos.z    + centerSizeUp.z >  targetPos.z - targetSizeDown.z
		&&  rCenterPosOld.z + centerSizeUp.z <= targetPos.z - targetSizeDown.z)
		{ // 前からの当たり判定

			// 位置を補正
			rCenterPos.z = targetPos.z - targetSizeDown.z - centerSizeUp.z;

			// 衝突状態にする
			bHit = true;
		}
		else if (rCenterPos.z    - centerSizeDown.z <  targetPos.z + targetSizeUp.z
		     &&  rCenterPosOld.z - centerSizeDown.z >= targetPos.z + targetSizeUp.z)
		{ // 後からの当たり判定

			// 位置を補正
			rCenterPos.z = targetPos.z + targetSizeUp.z + centerSizeDown.z;

			// 衝突状態にする
			bHit = true;
		}
	}

	// 左右の当たり判定
	if (rCenterPos.z + centerSizeUp.z > targetPos.z - targetSizeDown.z
	&&  rCenterPos.z - centerSizeDown.z < targetPos.z + targetSizeUp.z)
	{ // 前後の範囲内の場合

		if (rCenterPos.x    + centerSizeUp.x >  targetPos.x - targetSizeDown.x
		&&  rCenterPosOld.x + centerSizeUp.x <= targetPos.x - targetSizeDown.x)
		{ // 左からの当たり判定

			// 位置を補正
			rCenterPos.x = targetPos.x - targetSizeDown.x - centerSizeUp.x;

			// 衝突状態にする
			bHit = true;
		}
		else if (rCenterPos.x    - centerSizeDown.x <  targetPos.x + targetSizeUp.x
		     &&  rCenterPosOld.x - centerSizeDown.x >= targetPos.x + targetSizeUp.x)
		{ // 右からの当たり判定
			
			// 位置を補正
			rCenterPos.x = targetPos.x + targetSizeUp.x + centerSizeDown.x;

			// 衝突状態にする
			bHit = true;
		}
	}

	// 衝突判定を返す
	return bHit;
}

//============================================================
//	円の当たり判定
//============================================================
bool collision::Circle
(
	D3DXVECTOR3 centerPos,	// 判定位置
	D3DXVECTOR3 targetPos,	// 判定目標位置
	float fCenterSize,		// 判定サイズ
	float fTargetSize		// 判定目標サイズ
)
{
	// 変数を宣言
	float fLength  = 0.0f;	// 判定位置と判定目標位置の間の距離

	// 判定位置と判定目標位置の距離を求める
	fLength = (centerPos.x - targetPos.x) * (centerPos.x - targetPos.x)
			+ (centerPos.z - targetPos.z) * (centerPos.z - targetPos.z);

	if (fLength < ((fCenterSize + fTargetSize) * (fCenterSize + fTargetSize)))
	{ // 判定内の場合

		// 真を返す
		return true;
	}

	// 偽を返す
	return false;
}

//============================================================
//	扇形の当たり判定
//============================================================
bool collision::Sector
(
	D3DXVECTOR3 centerPos,	// 判定位置
	D3DXVECTOR3 targetPos,	// 判定目標位置
	float fCenterRot,		// 判定向き
	float fRadius,			// 視界範囲
	float fAngle			// 視野角
)
{
	// 変数を宣言
	float       fLength;		// 中心位置と目標位置の距離
	float       fHalfAngle;		// 引数の角度の半分の値の代入用
	bool        bHit = false;	// 当たり判定の結果
	D3DXVECTOR3 vecToPos;		// 左端と位置のベクトル

	// 変数配列を宣言
	float       fRotEdge[2];	// 扇形の縁の角度     [※] 0：左 1：右
	D3DXVECTOR3 posEdge[2];		// 扇形の縁の先端位置 [※] 0：左 1：右
	D3DXVECTOR3 vecEdge[2];		// 扇形の縁ベクトル   [※] 0：左 1：右

	// 中心位置と目標位置の距離求める
	fLength = (centerPos.x - targetPos.x) * (centerPos.x - targetPos.x)
			+ (centerPos.z - targetPos.z) * (centerPos.z - targetPos.z);

	if (fLength < fRadius * fRadius)
	{ // 円の範囲内の場合

		// 引数の角度の半分の値を求める
		fHalfAngle = fAngle * 0.5f;

		// 扇形の左縁の角度を求める
		fRotEdge[0] = fCenterRot + fHalfAngle;	// 角度を左に傾ける
		useful::NormalizeRot(fRotEdge[0]);		// 向きを正規化

		// 扇形の右縁の角度を求める
		fRotEdge[1] = fCenterRot - fHalfAngle;	// 角度を右に傾ける
		useful::NormalizeRot(fRotEdge[1]);		// 向きを正規化

		// 扇形の左縁の先端位置を求める
		posEdge[0].x = centerPos.x + sinf(fRotEdge[0]) * 1.0f;
		posEdge[0].y = 0.0f;
		posEdge[0].z = centerPos.z + cosf(fRotEdge[0]) * 1.0f;

		// 扇形の右縁の先端位置を求める
		posEdge[1].x = centerPos.x + sinf(fRotEdge[1]) * 1.0f;
		posEdge[1].y = 0.0f;
		posEdge[1].z = centerPos.z + cosf(fRotEdge[1]) * 1.0f;

		// 扇形の左縁のベクトルを求める
		vecEdge[0] = posEdge[0] - centerPos;
		vecEdge[0].y = 0.0f;

		// 扇形の右縁のベクトルを求める
		vecEdge[1] = posEdge[1] - centerPos;
		vecEdge[1].y = 0.0f;

		// 左端と位置のベクトルを求める
		vecToPos = targetPos - centerPos;

		if ((vecEdge[0].z * vecToPos.x) - (vecEdge[0].x * vecToPos.z) < 0
		&&  (vecEdge[1].z * vecToPos.x) - (vecEdge[1].x * vecToPos.z) > 0)
		{ // 扇形の両縁の範囲内の場合

			// 当たっている状態にする
			bHit = true;
		}
	}

	// 当たり判定の結果を返す
	return bHit;
}

//============================================================
//	外積の左右判定
//============================================================
//	境界線から見て左右どちらにいるかの判定に使用
//============================================================
float collision::LineOuterProduct
(
	D3DXVECTOR3 posLeft,	// 境界線左座標
	D3DXVECTOR3 posRight,	// 境界線右座標
	D3DXVECTOR3 pos			// 判定位置
)
{
	// 変数を宣言
	D3DXVECTOR3 vecLine;	// 境界線ベクトル
	D3DXVECTOR3 vecToPos;	// 左端と位置のベクトル

	// 境界線ベクトルを求める
	vecLine = posRight - posLeft;

	// 左端と位置のベクトルを求める
	vecToPos = pos - posLeft;

	// 外積の計算結果を返す
	return (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);
}
