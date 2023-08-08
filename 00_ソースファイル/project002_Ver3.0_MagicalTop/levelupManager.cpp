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
#include "shopManager.h"
#include "object2D.h"
#include "stage.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CLevelupManager::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\option000.png",	// 強化テクスチャ
	"data\\TEXTURE\\option001.png",	// 終了テクスチャ
};

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
	m_pOption = NULL;		// 操作情報
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
	// メンバ変数を初期化
	m_pShopManager = NULL;	// ショップマネージャーの情報
	m_pOption = NULL;		// 操作情報
	m_state = STATE_NORMAL;	// 状態

	// ショップマネージャーの生成
	m_pShopManager = CShopManager::Create();
	if (UNUSED(m_pShopManager))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 描画をしない状態にする
	m_pShopManager->SetEnableDraw(false);

	// 操作情報の生成
	m_pOption = CObject2D::Create	// TODO：定数
	( // 引数
		D3DXVECTOR3(1160.5f, 680.0f, 0.0f),
		D3DXVECTOR3(220.0f, 65.0f, 0.0f)
	);
	if (UNUSED(m_pOption))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pOption->SetPriority(6);

	// 描画をしない状態にする
	m_pOption->SetEnableDraw(false);

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
	m_pOption->Uninit();
}

//============================================================
//	更新処理
//============================================================
void CLevelupManager::Update(void)
{
	// ポインタを宣言
	CTexture		*pTexture	= CManager::GetTexture();	// テクスチャ
	CInputKeyboard	*pKeyboard	= CManager::GetKeyboard();	// キーボード
	CInputPad		*pPad		= CManager::GetPad();		// パッド

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NORMAL:		// 通常状態

		if (CSceneGame::GetStage()->GetAreaPlayer() == CStage::AREA_SAFE)
		{ // セーフエリア内の場合

			// 操作情報を表示
			m_pOption->SetEnableDraw(true);

			// 状態を設定
			m_state = STATE_INIT_INSAFE;	// セーフエリア内状態(初期化)
		}

		break;

	case STATE_INIT_INSAFE:	// セーフエリア内状態(初期化)

		// 強化表示のテクスチャを設定
		m_pOption->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_STRENGTHEN]));

		// 状態を設定
		m_state = STATE_INSAFE;	// セーフエリア内状態

		// 処理を抜けずに下に続く

	case STATE_INSAFE:		// セーフエリア内状態

		if (pKeyboard->GetTrigger(DIK_1))
		{
			// ショップを表示
			m_pShopManager->SetEnableDraw(true);

			// 状態を設定
			m_state = STATE_INIT_SELECT;	// 強化選択状態(初期化)
		}

		break;

	case STATE_INIT_SELECT:	// 強化選択状態(初期化)

		// 終了表示のテクスチャを設定
		m_pOption->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_END]));

		// 状態を設定
		m_state = STATE_SELECT;	// 強化選択状態(初期化)

		// 処理を抜けずに下に続く

	case STATE_SELECT:		// 強化選択状態

		if (pKeyboard->GetTrigger(DIK_1))
		{
			// ショップを非表示
			m_pShopManager->SetEnableDraw(false);

			// 状態を設定
			m_state = STATE_INIT_INSAFE;	// セーフエリア内状態(初期化)
		}

		// ショップマネージャーの更新
		m_pShopManager->Update();

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	if (m_state != STATE_NORMAL)
	{ // 状態が通常状態以外の場合

		if (CSceneGame::GetStage()->GetAreaPlayer() != CStage::AREA_SAFE)
		{ // セーフエリア外の場合

			// 描画をしない状態にする
			m_pShopManager->SetEnableDraw(false);
			m_pOption->SetEnableDraw(false);

			// 状態を設定
			m_state = STATE_NORMAL;	// 通常状態
		}
	}

	// 操作情報の更新
	m_pOption->Update();
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
