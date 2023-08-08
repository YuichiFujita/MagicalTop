//============================================================
//
//	ショップ処理 [shop.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "shop.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "anim2D.h"
#include "valueUI.h"
#include "multiValue.h"

//************************************************************
//	マクロ定義
//************************************************************
#define PTRN_WIDTH	(BUY_MAX)	// テクスチャの横分割数
#define PTRN_HEIGHT	(1)			// テクスチャの縦分割数

#define ICON_SIZE	(D3DXVECTOR3(120.0f, 120.0f, 0.0f))	// アイコンの大きさ
#define ICON_COL	(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f))	// アイコンの色

#define LIFE_SPACE			(D3DXVECTOR3(110.0f, -4.0f, 0.0f))	// ライフの行間
#define LIFE_TITLE_SIZE		(D3DXVECTOR3(180.0f, 50.0f, 0.0f))	// ライフのタイトル大きさ
#define LIFE_VALUE_SIZE		(D3DXVECTOR3(40.0f, 50.0f, 0.0f))	// ライフの数字大きさ
#define LIFE_VALUE_SPACE	(D3DXVECTOR3(40.0f, 0.0f, 0.0f))	// ライフの数字の行間
#define LIFE_VALUE_DIGIT	(3)	// ライフの数字の桁数

#define LV_SPACE		(D3DXVECTOR3(130.0f, -4.0f, 0.0f))	// レベルの行間
#define LV_TITLE_SIZE	(D3DXVECTOR3(180.0f, 50.0f, 0.0f))	// レベルのタイトル大きさ
#define LV_VALUE_SIZE	(D3DXVECTOR3(40.0f, 50.0f, 0.0f))	// レベルの数字大きさ
#define LV_VALUE_SPACE	(D3DXVECTOR3(40.0f, 0.0f, 0.0f))	// レベルの数字の行間
#define LV_VALUE_DIGIT	(2)	// レベルの数字の桁数

#define ADD_POS_LIFE	(D3DXVECTOR3(-50.0f, -140.0f, 0.0f))	// 体力の表示位置の相対座標への加算量
#define ADD_POS_LEVEL	(D3DXVECTOR3(-50.0f, -90.0f, 0.0f))		// レベルの表示位置の相対座標への加算量

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CShop::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\shop002.png",	// 体力テクスチャ
	"data\\TEXTURE\\shop003.png",	// レベルテクスチャ
};

//************************************************************
//	子クラス [CShop] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CShop::CShop() : CObject(CObject::LABEL_NONE, DEFAULT_PRIO)
{
	// メンバ変数をクリア
	m_pIcon = NULL;			// 購入品アイコン
	m_pLife = NULL;			// 必要体力
	m_pLv = NULL;			// 必要レベル
	m_pos = VEC3_ZERO;		// 位置
	m_col = XCOL_WHITE;		// 色
	m_buy = BUY_LV1_FIRE;	// 購入品
}

//============================================================
//	デストラクタ
//============================================================
CShop::~CShop()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CShop::Init(void)
{
	// メンバ変数を初期化
	m_pIcon = NULL;			// 購入品アイコン
	m_pLife = NULL;			// 必要体力
	m_pLv = NULL;			// 必要レベル
	m_pos = VEC3_ZERO;		// 位置
	m_col = XCOL_WHITE;		// 色
	m_buy = BUY_LV1_FIRE;	// 購入品

	// 例外処理
	assert(false);	// 使用しない初期化

	// 成功を返す
	return S_OK;
}

//============================================================
//	初期化処理 (オーバーロード)
//============================================================
HRESULT CShop::Init(const BUY buy)
{
	// ポインタを宣言
	CTexture *pTexture = CManager::GetTexture();	// テクスチャ

	// メンバ変数を初期化
	m_pIcon = NULL;		// 購入品アイコン
	m_pLife = NULL;		// 必要体力
	m_pLv = NULL;		// 必要レベル
	m_pos = VEC3_ZERO;	// 位置
	m_col = XCOL_WHITE;	// 色
	m_buy = buy;		// 購入品

	//--------------------------------------------------------
	//	購入品アイコンの生成・設定
	//--------------------------------------------------------
	// 購入品アイコンの生成
	m_pIcon = CAnim2D::Create
	( // 引数
		PTRN_WIDTH,		// 横分割数
		PTRN_HEIGHT,	// 縦分割数
		VEC3_ZERO,		// 位置
		ICON_SIZE		// 大きさ
	);
	if (UNUSED(m_pIcon))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 色を設定
	m_pIcon->SetColor(ICON_COL);

	// パターンを設定
	m_pIcon->SetPattern(m_buy);

	//--------------------------------------------------------
	//	必要体力の生成・設定
	//--------------------------------------------------------
	// 必要体力の生成
	m_pLife = CValueUI::Create
	( // 引数
		VEC3_ZERO,			// 位置
		LIFE_SPACE,			// 行間
		LIFE_TITLE_SIZE,	// タイトル大きさ
		LIFE_VALUE_SIZE,	// 数字大きさ
		pTexture->Regist(mc_apTextureFile[TEXTURE_LIFE])
	);
	if (UNUSED(m_pLife))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 数字の行間を設定
	m_pLife->GetMultiValue()->SetSpace(LIFE_VALUE_SPACE);

	// 数字の桁数を設定
	m_pLife->GetMultiValue()->SetDigit(LIFE_VALUE_DIGIT);

	//--------------------------------------------------------
	//	必要レベルの生成・設定
	//--------------------------------------------------------
	// 必要レベルの生成
	m_pLv = CValueUI::Create
	( // 引数
		VEC3_ZERO,		// 位置
		LV_SPACE,		// 行間
		LV_TITLE_SIZE,	// タイトル大きさ
		LV_VALUE_SIZE,	// 数字大きさ
		pTexture->Regist(mc_apTextureFile[TEXTURE_LV])
	);
	if (UNUSED(m_pLv))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 数字の行間を設定
	m_pLv->GetMultiValue()->SetSpace(LV_VALUE_SPACE);

	// 数字の桁数を設定
	m_pLv->GetMultiValue()->SetDigit(LV_VALUE_DIGIT);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CShop::Uninit(void)
{
	// アニメーション2D情報を破棄
	m_pIcon->Uninit();

	// 数字UI情報を破棄
	m_pLife->Uninit();
	m_pLv->Uninit();

	// 自身のオブジェクトを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CShop::Update(void)
{

}

//============================================================
//	描画処理
//============================================================
void CShop::Draw(void)
{

}

//============================================================
//	描画状況の設定処理
//============================================================
void CShop::SetEnableDraw(const bool bDraw)
{
	// 引数の描画状況を設定
	CObject::SetEnableDraw(bDraw);	// 自身
	m_pIcon->SetEnableDraw(bDraw);	// 購入品アイコン
	m_pLife->SetEnableDraw(bDraw);	// 必要体力
	m_pLv->SetEnableDraw(bDraw);	// 必要レベル
}

//============================================================
//	優先順位の設定処理
//============================================================
void CShop::SetPriority(const int nPriority)
{
	// 引数の優先順位を設定
	CObject::SetPriority(nPriority);	// 自身
	m_pIcon->SetPriority(nPriority);	// 購入品アイコン
	m_pLife->SetPriority(nPriority);	// 必要体力
	m_pLv->SetPriority(nPriority);		// 必要レベル
}

//============================================================
//	生成処理
//============================================================
CShop *CShop::Create
(
	const BUY buy,			// 購入品
	const D3DXVECTOR3& rPos	// 位置
)
{
	// ポインタを宣言
	CShop *pShop = NULL;	// ショップ生成用

	if (UNUSED(pShop))
	{ // 使用されていない場合

		// メモリ確保
		pShop = new CShop;	// ショップ
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pShop))
	{ // 確保に成功している場合

		// ショップの初期化
		if (FAILED(pShop->Init(buy)))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pShop;
			pShop = NULL;

			// 失敗を返す
			return NULL;
		}

		// 位置を設定
		pShop->SetPosition(rPos);

		// 確保したアドレスを返す
		return pShop;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	設定処理
//============================================================
void CShop::Set(const BUY buy)
{
	// 引数の購入品を設定
	m_buy = buy;
}

//============================================================
//	取得処理
//============================================================
CShop::BUY CShop::Get(void)
{
	// 購入品を返す
	return m_buy;
}

//============================================================
//	位置の設定処理
//============================================================
void CShop::SetPosition(const D3DXVECTOR3& rPos)
{
	// 引数の位置を設定
	m_pos = rPos;

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	相対位置の設定処理
//============================================================
void CShop::SetPositionRelative(void)
{
	// アイコンの位置を設定
	m_pIcon->SetPosition(m_pos);

	// 必要体力の表示位置を設定
	m_pLife->SetPosition(m_pos + ADD_POS_LIFE);

	// 必要レベルの表示位置を設定
	m_pLv->SetPosition(m_pos + ADD_POS_LEVEL);
}

//============================================================
//	色の設定処理
//============================================================
void CShop::SetColor(const D3DXCOLOR& rCol)
{
	// 引数の色を代入
	m_col = rCol;

	// アイコンの位置を設定
	m_pIcon->SetColor(rCol);
}

//============================================================
//	位置取得処理
//============================================================
D3DXVECTOR3 CShop::GetPosition(void) const
{
	// 位置を返す
	return m_pos;
}

//============================================================
//	色取得処理
//============================================================
D3DXCOLOR CShop::GetColor(void) const
{
	// 色を返す
	return m_col;
}
