//============================================================
//
//	チュートリアルマネージャー処理 [tutorialManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "tutorialManager.h"
#include "manager.h"
#include "input.h"
#include "camera.h"
#include "texture.h"
#include "object2D.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************


//************************************************************
//	マクロ定義
//************************************************************


//************************************************************
//	親クラス [CTutorialManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTutorialManager::CTutorialManager()
{
	// メンバ変数をクリア
	m_state = STATE_NONE;	// 状態
}

//============================================================
//	デストラクタ
//============================================================
CTutorialManager::~CTutorialManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTutorialManager::Init(void)
{
	// ポインタを宣言
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ

	// メンバ変数を初期化
	m_state = STATE_NONE;	// 状態

#if 0
	//--------------------------------------------------------
	//	選択背景の生成・設定
	//--------------------------------------------------------
	// 選択背景の生成
	m_pSelectBG = CObject2D::Create
	( // 引数
		POS_SELECTBG,	// 位置
		SIZE_SELECTBG,	// 大きさ
		VEC3_ZERO,		// 向き
		COL_SELECTBG	// 色
	);
	if (UNUSED(m_pSelectBG))
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pSelectBG->SetPriority(TUTORIAL_PRIO);

	// 描画をしない設定にする
	m_pSelectBG->SetEnableDraw(false);
#endif

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CTutorialManager::Uninit(void)
{

}

//============================================================
//	更新処理
//============================================================
void CTutorialManager::Update(void)
{
	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:	// 何もしない状態

		// 無し

		break;

	default:	// 例外処理
		assert(false);
		break;
	}
}

//============================================================
//	生成処理
//============================================================
CTutorialManager *CTutorialManager::Create(void)
{
	// ポインタを宣言
	CTutorialManager *pTutorialManager = NULL;	// チュートリアルマネージャー生成用

	if (UNUSED(pTutorialManager))
	{ // 使用されていない場合

		// メモリ確保
		pTutorialManager = new CTutorialManager;	// チュートリアルマネージャー
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pTutorialManager))
	{ // 使用されている場合
		
		// チュートリアルマネージャーの初期化
		if (FAILED(pTutorialManager->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pTutorialManager;
			pTutorialManager = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pTutorialManager;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CTutorialManager::Release(CTutorialManager *&prTutorialManager)
{
	if (USED(prTutorialManager))
	{ // 使用中の場合

		// チュートリアルマネージャーの終了
		prTutorialManager->Uninit();

		// メモリ開放
		delete prTutorialManager;
		prTutorialManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}
