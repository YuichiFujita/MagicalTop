//============================================================
//
//	便利関数処理 [useful.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "useful.h"

//************************************************************
//	通常関数
//************************************************************
//============================================================
//	数値の桁数ごとの分解
//============================================================
//	スコア表示の際などに使用
//============================================================
void useful::DivideDigitNum
(
	int *pNumDivide,	// 分解結果の格納配列
	const int nNum,		// 分解する数値
	const int nMaxDigit	// 分解する数字の桁数
)
{
	// 変数を宣言
	int nDigit = 1;	// 各桁の数値の計算用

	for (int nCntDigit = 0; nCntDigit < nMaxDigit; nCntDigit++)
	{ // 桁の最大数分繰り返す

		// 桁数分乗算する
		nDigit *= 10;
	}

	for (int nCntTex = nMaxDigit - 1; nCntTex >= 0; nCntTex--, pNumDivide++)
	{ // 桁数の最大から桁数の最小まで繰り返す

		// 桁数ごとの数値を求める
		*pNumDivide = (nNum % nDigit) / (nDigit / 10);

		// 除算して次の桁数に合わせる
		nDigit /= 10;
	}
}

//============================================================
//	法線の正規化
//============================================================
void useful::NormalizeNormal
(
	const D3DXVECTOR3& rPosLeft,	// 左位置
	const D3DXVECTOR3& rPosCenter,	// 中心位置
	const D3DXVECTOR3& rPosRight,	// 右位置
	D3DXVECTOR3& rNor				// 法線
)
{
	// 変数を宣言
	D3DXVECTOR3 vecLine[2];	// 辺ベクトル

	// 辺ベクトルを求める
	vecLine[0] = rPosLeft - rPosCenter;
	vecLine[1] = rPosRight - rPosCenter;

	// 法線を計算
	rNor.x = (vecLine[0].y * vecLine[1].z) - (vecLine[0].z * vecLine[1].y);
	rNor.y = (vecLine[0].z * vecLine[1].x) - (vecLine[0].x * vecLine[1].z);
	rNor.z = (vecLine[0].x * vecLine[1].y) - (vecLine[0].y * vecLine[1].x);

	// 法線を正規化
	D3DXVec3Normalize(&rNor, &rNor);
}

//============================================================
//	向きの正規化
//============================================================
void useful::NormalizeRot(float& rRot)
{
	if (rRot > D3DX_PI)
	{ // 向きが 3.14 を超えた場合

		// 向きの正規化
		rRot -= D3DX_PI * 2;
	}
	else if (rRot < -D3DX_PI)
	{ // 向きが -3.14 を超えた場合

		// 向きの正規化
		rRot += D3DX_PI * 2;
	}
}
