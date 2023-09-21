//============================================================
//
//	強化マネージャー処理 [levelupManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "levelupManager.h"
#include "manager.h"
#include "input.h"
#include "texture.h"
#include "sceneGame.h"
#include "waveManager.h"
#include "shopManager.h"
#include "object2D.h"
#include "stage.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CLevelupManager::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\option000.png",	// 操作テクスチャ
};

//************************************************************
//	マクロ定義
//************************************************************
#define CONTROL_POS		(D3DXVECTOR3(1160.5f, 680.0f, 0.0f))	// 操作表示の位置
#define CONTROL_SIZE	(D3DXVECTOR3(220.0f, 65.0f, 0.0f))		// 操作表示の大きさ
#define CONTROL_PRIO	(6)	// 操作表示の優先順位

//************************************************************
//	親クラス [CLevelupManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CLevelupManager::CLevelupManager()
{
	// メンバ変数をクリア
	m_pShopManager = NULL;	// ショップマネージャーの情報
	m_pControl = NULL;		// 操作情報
	m_state = STATE_NORMAL;	// 状態
}

//============================================================
//	デストラクタ
//============================================================
CLevelupManager::~CLevelupManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CLevelupManager::Init(void)
{
	// ポインタを宣言
	CTexture *pTexture = CManager::GetTexture();	// テクスチャ

	// メンバ変数を初期化
	m_pShopManager = NULL;	// ショップマネージャーの情報
	m_pControl = NULL;		// 操作情報
	m_state = STATE_NORMAL;	// 状態

	// ショップマネージャーの生成
	m_pShopManager = CShopManager::Create();
	if (UNUSED(m_pShopManager))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ショップを閉店する
	m_pShopManager->SetEnableOpen(false);

	// 操作情報の生成
	m_pControl = CObject2D::Create
	( // 引数
		CONTROL_POS,	// 位置
		CONTROL_SIZE	// 大きさ
	);
	if (UNUSED(m_pControl))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを読込・割当
	m_pControl->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_CONTROL]));

	// 優先順位を設定
	m_pControl->SetPriority(CONTROL_PRIO);

	// 描画をしない状態にする
	m_pControl->SetEnableDraw(false);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CLevelupManager::Uninit(void)
{
	// ショップマネージャーを破棄
	if (FAILED(m_pShopManager->Release(m_pShopManager)))
	{ // 破棄に失敗した場合

		// 例外処理
		assert(false);
	}

	// 操作情報を破棄
	m_pControl->Uninit();
}

//============================================================
//	更新処理
//============================================================
void CLevelupManager::Update(void)
{
	// ポインタを宣言
	CInputKeyboard	*pKeyboard	= CManager::GetKeyboard();	// キーボード
	CInputPad		*pPad		= CManager::GetPad();		// パッド

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NORMAL:	// 通常状態

		if (CSceneGame::GetWaveManager()->GetState() == CWaveManager::STATE_WAIT)
		{ // 次の季節へ移行待ちの場合

			// ショップを開店する
			m_pShopManager->SetEnableOpen(true);

			// 操作情報を表示
			m_pControl->SetEnableDraw(true);

			// 状態を設定
			m_state = STATE_SELECT;	// 強化選択状態
		}

		break;

	case STATE_SELECT:	// 強化選択状態

		if (pKeyboard->GetTrigger(DIK_1) || pPad->GetTrigger(CInputPad::KEY_X))
		{ // 強化終了の操作が行われた場合

			// ショップを閉店する
			m_pShopManager->SetEnableOpen(false);

			// 操作情報を非表示
			m_pControl->SetEnableDraw(false);

			// 状態を設定
			m_state = STATE_NORMAL;	// 通常状態

			// 次季節へ移行
			CSceneGame::GetWaveManager()->NextSeason();
		}

		// ショップマネージャーの更新
		m_pShopManager->Update();

		// 操作情報の更新
		m_pControl->Update();

		break;

	default:	// 例外処理
		assert(false);
		break;
	}
}

//============================================================
//	生成処理
//============================================================
CLevelupManager *CLevelupManager::Create(void)
{
	// ポインタを宣言
	CLevelupManager *pLevelupManager = NULL;	// 強化マネージャー生成用

	if (UNUSED(pLevelupManager))
	{ // 使用されていない場合

		// メモリ確保
		pLevelupManager = new CLevelupManager;	// 強化マネージャー
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pLevelupManager))
	{ // 使用されている場合
		
		// 強化マネージャーの初期化
		if (FAILED(pLevelupManager->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pLevelupManager;
			pLevelupManager = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pLevelupManager;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CLevelupManager::Release(CLevelupManager *&prLevelupManager)
{
	if (USED(prLevelupManager))
	{ // 使用中の場合

		// 強化マネージャーの終了
		prLevelupManager->Uninit();

		// メモリ開放
		delete prLevelupManager;
		prLevelupManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}
