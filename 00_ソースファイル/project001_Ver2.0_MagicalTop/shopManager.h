//============================================================
//
//	ショップマネージャーヘッダー [shopManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SHOPMANAGER_H_
#define _SHOPMANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;	// オブジェクト2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// ショップマネージャークラス
class CShopManager
{
public:
	// テクスチャ列挙
	enum TEXTURE
	{
		TEXTURE_TARGET = 0,	// ターゲットアイコンテクスチャ
		TEXTURE_EXP,		// 経験値アイコンテクスチャ
		TEXTURE_MAX,		// この列挙型の総数
	};

	// コンストラクタ
	CShopManager();

	// デストラクタ
	~CShopManager();

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void SetEnableDraw(const bool bDraw);	// 描画状況設定

	// 静的メンバ関数
	static CShopManager *Create(void);	// 生成
	static HRESULT Release(CShopManager *&prShopManager);	// 破棄

private:
	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数

	// メンバ変数
	CObject2D *m_pBg;			// 背景情報
	CObject2D *m_pIconTarget;	// ターゲットアイコン情報
	CObject2D *m_pIconExp;		// 経験値アイコン情報
};

#endif	// _SHOPMANAGER_H_
