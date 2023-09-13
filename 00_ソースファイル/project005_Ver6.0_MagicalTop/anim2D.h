//============================================================
//
//	アニメーション2Dヘッダー [anim2D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ANIM2D_H_
#define _ANIM2D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object2D.h"

//************************************************************
//	クラス定義
//************************************************************
// アニメーション2Dクラス
class CAnim2D : public CObject2D
{
public:
	// コンストラクタ
	CAnim2D();
	CAnim2D(const CObject::LABEL label, const int nPriority = DEFAULT_PRIO);

	// デストラクタ
	~CAnim2D();

	// オーバーライド関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	// オーバーロードしたオーバーライド関数
	HRESULT Init	// 初期化
	( // 引数
		const int nWidthPtrn,	// テクスチャの横の分割数
		const int nHeightPtrn	// テクスチャの縦の分割数
	);

	// 静的メンバ関数
	static CAnim2D *Create	// 生成
	( // 引数
		const int nWidthPtrn,		// テクスチャの横の分割数
		const int nHeightPtrn,		// テクスチャの縦の分割数
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rSize = VEC3_ONE,	// 大きさ
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// 向き
		const D3DXCOLOR& rCol = XCOL_WHITE		// 色
	);

	// メンバ関数
	void SetPosition(const D3DXVECTOR3& rPos);		// 位置設定
	void SetRotation(const D3DXVECTOR3& rRot);		// 向き設定
	void SetScaling(const D3DXVECTOR3& rSize);		// 大きさ設定
	void SetColor(const D3DXCOLOR& rCol);			// 色設定
	void SetPattern(const int nPattern);			// パターンの設定
	void SetMaxPattern(const int nMaxPtrn);			// パターンの総数の設定
	void SetCounter(const int nCntChange);			// カウンターの設定
	void SetEnableStop(const bool bStop);			// 停止状況の設定
	void SetEnablePlayBack(const bool bPlayBack);	// 逆再生状況の設定
	int  GetLoopAnimation(void) const;				// パターン繰り返し数の取得

private:
	// メンバ変数
	int m_nCounter;		// アニメーションカウンター
	int m_nCntChange;	// パターン変更カウント
	int m_nPattern;		// アニメーションパターン
	int m_nMaxPtrn;		// パターンの総数
	int m_nWidthPtrn;	// テクスチャの横の分割数
	int m_nHeightPtrn;	// テクスチャの縦の分割数
	int m_nNumLoop;		// パターン繰り返し数
	bool m_bStop;		// 停止状況
	bool m_bPlayBack;	// 逆再生状況
};

#endif	// _ANIM2D_H_
