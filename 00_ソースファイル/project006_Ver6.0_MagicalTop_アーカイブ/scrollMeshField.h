//============================================================
//
//	スクロールメッシュフィールドヘッダー [scrollMeshField.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SCROLL_MESHFIELD_H_
#define _SCROLL_MESHFIELD_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "objectMeshField.h"

//************************************************************
//	クラス定義
//************************************************************
// スクロールメッシュフィールドクラス
class CScrollMeshField : public CObjectMeshField
{
public:
	// コンストラクタ
	CScrollMeshField();
	CScrollMeshField(const CObject::LABEL label, const int nPriority = DEFAULT_PRIO);

	// デストラクタ
	~CScrollMeshField();

	// オーバーライド関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	// オーバーロードしたオーバーライド関数
	HRESULT Init	// 初期化
	( // 引数
		const float fMoveU,	// 横座標の移動量
		const float fMoveV	// 縦座標の移動量
	);

	// 静的メンバ関数
	static CScrollMeshField *Create	// 生成
	( // 引数
		const float fMoveU,			// 横座標の移動量
		const float fMoveV,			// 縦座標の移動量
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const D3DXVECTOR2& rSize,	// 大きさ
		const D3DXCOLOR& rCol,		// 色
		const POSGRID2& rPart,		// 分割数
		const D3DCULL cull = D3DCULL_CCW,	// カリング状況
		const bool bLight = true			// ライティング状況
	);

	// メンバ関数
	void SetPosition(const D3DXVECTOR3& rPos);			// 位置設定
	void SetRotation(const D3DXVECTOR3& rRot);			// 向き設定
	void SetScalingBiaxial(const D3DXVECTOR2& rSize);	// 大きさ設定
	void SetColor(const D3DXCOLOR& rCol);				// 色設定
	HRESULT SetPattern(const POSGRID2& rPart);			// 分割数設定

	void SetTexU(const float fTexU);	// 横座標の開始位置設定
	void SetTexV(const float fTexV);	// 縦座標の開始位置設定
	void SetMoveU(const float fMoveU);	// 横座標の移動量設定
	void SetMoveV(const float fMoveV);	// 縦座標の移動量設定

private:
	// メンバ変数
	float m_fTexU;	// テクスチャ横座標の開始位置
	float m_fTexV;	// テクスチャ縦座標の開始位置
	float m_fMoveU;	// テクスチャ横座標の移動量
	float m_fMoveV;	// テクスチャ縦座標の移動量
};

#endif	// _SCROLL_MESHFIELD_H_
