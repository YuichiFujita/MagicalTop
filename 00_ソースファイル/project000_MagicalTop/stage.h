//============================================================
//
//	ステージヘッダー [stage.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _STAGE_H_
#define _STAGE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	マクロ定義
//************************************************************
#define MAX_GRIDX	(128)	// 方眼座標Xの最大数
#define MAX_GRIDY	(128)	// 方眼座標Yの最大数
#define MAX_GRIDZ	(128)	// 方眼座標Zの最大数

//************************************************************
//	前方宣言
//************************************************************
class CBlock;	// ブロッククラス

//************************************************************
//	クラス定義
//************************************************************
// ステージクラス
class CStage
{
public:
	// コンストラクタ
	CStage();

	// デストラクタ
	~CStage();

	// 方眼情報 Z構造体
	typedef struct
	{
		CBlock *pBlock;	// ブロックのポインタ
	}GridZ;

	// 方眼情報 Y構造体
	typedef struct
	{
		GridZ aZ[MAX_GRIDZ];	// 方眼座標Z
	}GridY;

	// 方眼情報 X構造体
	typedef struct
	{
		GridY aY[MAX_GRIDY];	// 方眼座標Y
	}GridX;

	// ステージ方眼座標構造体
	typedef struct
	{
		// メンバ関数
		void In(const POSGRID3& rGrid, CBlock *pBlock)
		{
			aX[rGrid.x].aY[rGrid.y].aZ[rGrid.z].pBlock = pBlock;
		}
		GridZ Out(const POSGRID3& rGrid)
		{
			return aX[rGrid.x].aY[rGrid.y].aZ[rGrid.z];
		}

		// メンバ変数
		GridX aX[MAX_GRIDX];	// 方眼座標X
		D3DXVECTOR3 centerPos;	// 方眼座標の中心座標
		D3DXVECTOR3 revPos;		// 座標変換の補正値
		POSGRID3 maxGrid;		// 方眼座標数
	}StageGrid;

	// ステージ範囲構造体
	typedef struct
	{
		float fNear;	// 制限位置 (前)
		float fFar;		// 制限位置 (後)
		float fRight;	// 制限位置 (右)
		float fLeft;	// 制限位置 (左)
		float fField;	// 制限位置 (地面)
	}StageLimit;

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void LimitPosition(D3DXVECTOR3& rPos, const float fRadius);						// 位置補正
	bool LandPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove, const float fHeight);	// 範囲外着地
	D3DXVECTOR3 ChangeGridPosition(const POSGRID3& rGrid);							// 方眼座標の座標変換

	void SetStageLimit(const StageLimit& rLimit);			// ステージ範囲設定
	void SetStageGridCenter(const D3DXVECTOR3& rPos);		// ステージ方眼座標の中心位置設定
	void SetStageGridRevision(const D3DXVECTOR3& rRev);		// ステージ方眼座標の位置補正量設定
	void SetStageGridMax(const POSGRID3& rGrid);			// ステージ方眼座標の最大方眼数設定
	void SetInGrid(const POSGRID3& rGrid, CBlock *pBlock);	// ステージ方眼座標内設定

	StageLimit GetStageLimit(void) const;	// ステージ範囲取得
	StageGrid GetStageGrid(void) const;		// ステージ方眼座標取得
	GridZ GetInGrid(const POSGRID3& rGrid);	// ステージ方眼座標内取得

	// 静的メンバ関数
	static CStage *Create(void);				// 生成
	static HRESULT Release(CStage *&prStage);	// 破棄

private:
	// 静的メンバ関数
	static void LoadSetup(CStage *pStage);	// セットアップ

	// メンバ変数
	StageLimit m_stageLimit;	// ステージ範囲
	StageGrid m_stageGrid;		// ステージ方眼座標
};

#endif	// _STAGE_H_
