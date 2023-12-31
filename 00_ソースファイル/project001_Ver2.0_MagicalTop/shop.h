//============================================================
//
//	ショップヘッダー [shop.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SHOP_H_
#define _SHOP_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	前方宣言
//************************************************************
class CAnim2D;	// アニメーション2Dクラス
class CValueUI;	// 数字UIクラス

//************************************************************
//	クラス定義
//************************************************************
// ショップクラス
class CShop : public CObject
{
public:
	// テクスチャ列挙
	enum TEXTURE
	{
		TEXTURE_LIFE = 0,	// 体力テクスチャ
		TEXTURE_LV,			// レベルテクスチャ
		TEXTURE_MAX,		// この列挙型の総数
	};

	// 購入列挙
	enum BUY
	{
		BUY_LV1_FIRE = 0,	// LV1炎魔法
		BUY_LV1_WATER,		// LV1水魔法
		BUY_LV1_WIND,		// LV1風魔法
		BUY_LV2_FIRE,		// LV2炎魔法
		BUY_LV2_WATER,		// LV2水魔法
		BUY_LV2_WIND,		// LV2風魔法
		BUY_MAX				// この列挙型の総数
	};

	// コンストラクタ
	CShop();

	// デストラクタ
	~CShop();

	// オーバーライド関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	void SetEnableDraw(const bool bDraw);	// 描画状況設定
	void SetPriority(const int nPriority);	// 優先順位設定

	// オーバーロードしたオーバーライド関数
	HRESULT Init(const BUY buy);	// 初期化

	// 静的メンバ関数
	static CShop *Create	// 生成
	( // 引数
		const BUY buy,			// 購入品
		const D3DXVECTOR3& rPos	// 位置
	);

	// メンバ関数
	void Set(const BUY buy);	// 設定
	BUY  Get(void);				// 取得
	void SetPosition(const D3DXVECTOR3& rPos);	// 位置設定
	void SetColor(const D3DXCOLOR& rCol);		// 色設定
	D3DXVECTOR3 GetPosition(void) const;		// 位置取得
	D3DXCOLOR GetColor(void) const;				// 色取得

private:
	// メンバ関数
	void SetPositionRelative(void);	// 相対位置設定

	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数

	// メンバ変数
	CAnim2D *m_pIcon;	// 購入品アイコン
	CValueUI *m_pLife;	// 必要体力
	CValueUI *m_pLv;	// 必要レベル
	D3DXVECTOR3 m_pos;	// 位置
	D3DXCOLOR m_col;	// 色
	BUY m_buy;			// 購入品
};

#endif	// _SHOP_H_
