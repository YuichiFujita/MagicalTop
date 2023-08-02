//============================================================
//
//	ショップマネージャー処理 [shopManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "shopManager.h"
#include "manager.h"
#include "input.h"
#include "texture.h"
#include "sceneGame.h"
#include "object2D.h"
#include "stage.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CShopManager::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\icon000.png",	// ターゲットアイコンテクスチャ
	"data\\TEXTURE\\icon001.png",	// 経験値アイコンテクスチャ
};

//************************************************************
//	親クラス [CShopManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CShopManager::CShopManager()
{
	// メンバ変数をクリア
	m_pBg = NULL;	// 背景情報
}

//============================================================
//	デストラクタ
//============================================================
CShopManager::~CShopManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CShopManager::Init(void)
{
	// ポインタを宣言
	CTexture *pTexture = CManager::GetTexture();	// テクスチャ

	// メンバ変数を初期化
	m_pBg = NULL;	// 背景情報

	// 背景情報の生成
	m_pBg = CObject2D::Create	// TODO：定数
	( // 引数
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.85f, SCREEN_HEIGHT * 0.78f, 0.0f),
		VEC3_ZERO,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.55f)
	);
	if (UNUSED(m_pBg))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pBg->SetPriority(6);

	// 描画をしない状態にする
	m_pBg->SetEnableDraw(false);

	// ターゲットアイコン情報の生成
	m_pIconTarget = CObject2D::Create	// TODO：定数
	( // 引数
		D3DXVECTOR3(180.0f, 160.0f, 0.0f),
		D3DXVECTOR3(120.0f, 120.0f, 0.0f)
	);
	if (UNUSED(m_pIconTarget))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを設定
	m_pIconTarget->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_TARGET]));

	// 優先順位を設定
	m_pIconTarget->SetPriority(6);

	// 描画をしない状態にする
	m_pIconTarget->SetEnableDraw(false);

	// 経験値アイコン情報の生成
	m_pIconExp = CObject2D::Create	// TODO：定数
	( // 引数
		D3DXVECTOR3(700.0f, 160.0f, 0.0f),
		D3DXVECTOR3(120.0f, 120.0f, 0.0f)
	);
	if (UNUSED(m_pIconExp))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを設定
	m_pIconExp->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_EXP]));

	// 優先順位を設定
	m_pIconExp->SetPriority(6);

	// 描画をしない状態にする
	m_pIconExp->SetEnableDraw(false);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CShopManager::Uninit(void)
{
	// オブジェクト2D情報を破棄
	m_pBg->Uninit();
	m_pIconTarget->Uninit();
	m_pIconExp->Uninit();
}

//============================================================
//	更新処理
//============================================================
void CShopManager::Update(void)
{
	// ポインタを宣言
	CInputKeyboard	*pKeyboard	= CManager::GetKeyboard();	// キーボード
	CInputPad		*pPad		= CManager::GetPad();		// パッド

	// オブジェクト2D情報の更新
	m_pBg->Update();
	m_pIconTarget->Update();
	m_pIconExp->Update();
}

//============================================================
//	描画状況の設定処理
//============================================================
void CShopManager::SetEnableDraw(const bool bDraw)
{
	// 引数の描画状況を設定
	m_pBg->SetEnableDraw(bDraw);			// 背景情報
	m_pIconTarget->SetEnableDraw(bDraw);	// ターゲットアイコン情報
	m_pIconExp->SetEnableDraw(bDraw);		// 経験値アイコン情報
}

//============================================================
//	生成処理
//============================================================
CShopManager *CShopManager::Create(void)
{
	// ポインタを宣言
	CShopManager *pShopManager = NULL;	// ショップマネージャー生成用

	if (UNUSED(pShopManager))
	{ // 使用されていない場合

		// メモリ確保
		pShopManager = new CShopManager;	// ショップマネージャー
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pShopManager))
	{ // 使用されている場合
		
		// ショップマネージャーの初期化
		if (FAILED(pShopManager->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pShopManager;
			pShopManager = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pShopManager;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CShopManager::Release(CShopManager *&prShopManager)
{
	if (USED(prShopManager))
	{ // 使用中の場合

		// ショップマネージャーの終了
		prShopManager->Uninit();

		// メモリ開放
		delete prShopManager;
		prShopManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}
