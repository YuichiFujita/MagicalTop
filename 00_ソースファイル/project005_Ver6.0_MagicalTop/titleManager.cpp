//============================================================
//
//	タイトルマネージャー処理 [titleManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "titleManager.h"
#include "manager.h"
#include "input.h"
#include "camera.h"
#include "texture.h"
#include "model.h"
#include "object2D.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CTitleManager::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\title000.png",	// タイトル表示テクスチャ
};
const char *CTitleManager::mc_apLogoTextureFile[] =	// ロゴテクスチャ定数
{
	"data\\TEXTURE\\title000.png",	// MAGICALテクスチャ
	"data\\TEXTURE\\title001.png",	// TOPテクスチャ
};

//************************************************************
//	マクロ定義
//************************************************************
#define TITLE_PRIO	(6)	// タイトルの優先順位

#define POS_LOGO_MAGICAL	(D3DXVECTOR3(440.0f, 160.0f, 0.0f))	// タイトルロゴの位置 (MAGICAL)
#define POS_LOGO_TOP		(D3DXVECTOR3(930.0f, 160.0f, 0.0f))	// タイトルロゴの位置 (TOP)

#define SIZE_TITLE	(D3DXVECTOR3(666.0f, 214.2f, 0.0f))	// タイトルロゴの大きさ
#define INIT_SCALE	(15.0f)	// タイトルロゴの初期拡大率
#define SUB_SCALE	(0.65f)	// タイトルロゴ拡大率の減算量

#define COL_FADE	(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f))	// タイトルフェードの色
#define SUB_ALPHA	(0.008f)	// α値の減算量

//************************************************************
//	親クラス [CTitleManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTitleManager::CTitleManager()
{
	// メンバ変数をクリア
	memset(&m_apLogo[0], 0, sizeof(m_apLogo));	// タイトル表示の情報
	m_pFade = NULL;			// フェードの情報
	m_state = STATE_NONE;	// 状態
	m_nCounterState = 0;	// 状態管理カウンター
	m_fScale = 0.0f;		// タイトル拡大率
}

//============================================================
//	デストラクタ
//============================================================
CTitleManager::~CTitleManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTitleManager::Init(void)
{
	// 変数配列を宣言
	D3DXVECTOR3 aPosLogo[] =	// ロゴの位置
	{ // 初期値
		POS_LOGO_MAGICAL,	// MAGICAL位置
		POS_LOGO_TOP,		// TOP位置
	};

	// ポインタを宣言
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ
	CModel *pModel = CManager::GetModel();			// モデルへのポインタ

	// メンバ変数を初期化
	memset(&m_apLogo[0], 0, sizeof(m_apLogo));	// タイトル表示の情報
	m_pFade = NULL;				// フェードの情報
	m_state = STATE_FADEOUT;	// 状態
	m_nCounterState = 0;		// 状態管理カウンター
	m_fScale = INIT_SCALE;		// タイトル拡大率

	//--------------------------------------------------------
	//	フェードの生成・設定
	//--------------------------------------------------------
	// フェードの生成
	m_pFade = CObject2D::Create
	( // 引数
		SCREEN_CENT,	// 位置
		SCREEN_SIZE,	// 大きさ
		VEC3_ZERO,		// 向き
		COL_FADE		// 色
	);
	if (UNUSED(m_pFade))
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pFade->SetPriority(TITLE_PRIO);

	//--------------------------------------------------------
	//	タイトルロゴの生成・設定
	//--------------------------------------------------------
	for (int nCntTitle = 0; nCntTitle < NUM_LOGO; nCntTitle++)
	{ // タイトルロゴの数分繰り返す

		// タイトルロゴの生成
		m_apLogo[nCntTitle] = CObject2D::Create
		( // 引数
			aPosLogo[nCntTitle],	// 位置
			SIZE_TITLE	// 大きさ
		);
		if (UNUSED(m_apLogo[nCntTitle]))
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// テクスチャを登録・割当
		m_apLogo[nCntTitle]->BindTexture(pTexture->Regist(mc_apLogoTextureFile[nCntTitle]));

		// 優先順位を設定
		m_apLogo[nCntTitle]->SetPriority(TITLE_PRIO);

		// 描画をしない設定にする
		m_apLogo[nCntTitle]->SetEnableDraw(false);
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CTitleManager::Uninit(void)
{
	// フェードの終了
	m_pFade->Uninit();

	for (int nCntTitle = 0; nCntTitle < NUM_LOGO; nCntTitle++)
	{ // タイトルロゴの数分繰り返す

		// タイトルロゴの終了
		m_apLogo[nCntTitle]->Uninit();
	}
}

//============================================================
//	更新処理
//============================================================
void CTitleManager::Update(void)
{
	// 遷移決定の更新
	UpdateStart();

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:		// 何もしない状態

		// 無し

		break;

	case STATE_FADEOUT:		// フェードアウト状態

		// フェードアウトの更新
		UpdateFade();

		break;

	case STATE_MOVE:		// タイトル縮小状態

		// タイトル移動の更新
		UpdateMove();

		break;

	case STATE_WAIT:		// 遷移待機状態

		// 無し

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// フェードの更新
	m_pFade->Update();

	for (int nCntTitle = 0; nCntTitle < NUM_LOGO; nCntTitle++)
	{ // タイトルロゴの数分繰り返す

		// タイトルロゴの更新
		m_apLogo[nCntTitle]->Update();
	}
}

//============================================================
//	生成処理
//============================================================
CTitleManager *CTitleManager::Create(void)
{
	// ポインタを宣言
	CTitleManager *pTitleManager = NULL;	// タイトルマネージャー生成用

	if (UNUSED(pTitleManager))
	{ // 使用されていない場合

		// メモリ確保
		pTitleManager = new CTitleManager;	// タイトルマネージャー
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pTitleManager))
	{ // 使用されている場合
		
		// タイトルマネージャーの初期化
		if (FAILED(pTitleManager->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pTitleManager;
			pTitleManager = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pTitleManager;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CTitleManager::Release(CTitleManager *&prTitleManager)
{
	if (USED(prTitleManager))
	{ // 使用中の場合

		// タイトルマネージャーの終了
		prTitleManager->Uninit();

		// メモリ開放
		delete prTitleManager;
		prTitleManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	フェードアウトの更新処理
//============================================================
void CTitleManager::UpdateFade(void)
{
	// 変数を宣言
	float fAlpha = m_pFade->GetColor().a;	// フェード透明度

	if (fAlpha > 0.0f)
	{ // 透明になっていない場合

		// 透明度を減算
		fAlpha -= SUB_ALPHA;
	}
	else
	{ // 透明になった場合

		// 透明度を補正
		fAlpha = 0.0f;

		for (int nCntTitle = 0; nCntTitle < NUM_LOGO; nCntTitle++)
		{ // タイトルロゴの数分繰り返す

			// 描画をする設定にする
			m_apLogo[nCntTitle]->SetEnableDraw(true);
		}

		// 状態を変更
		m_state = STATE_MOVE;	// タイトル移動状態
	}

	// 透明度を反映
	m_pFade->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, fAlpha));
}

//============================================================
//	タイトル移動の更新処理
//============================================================
void CTitleManager::UpdateMove(void)
{
	if (m_fScale > 1.0f)
	{ // 拡大率が最小値より大きい場合

		// 拡大率を減算
		m_fScale -= SUB_SCALE;

		for (int nCntTitle = 0; nCntTitle < NUM_LOGO; nCntTitle++)
		{ // タイトルロゴの数分繰り返す

			// タイトルロゴの大きさを設定
			m_apLogo[nCntTitle]->SetScaling(SIZE_TITLE * m_fScale);
		}
	}
	else
	{ // 拡大率が最小値以下の場合

		// 拡大率を補正
		m_fScale = 1.0f;

		for (int nCntTitle = 0; nCntTitle < NUM_LOGO; nCntTitle++)
		{ // タイトルロゴの数分繰り返す

			// タイトルロゴの大きさを設定
			m_apLogo[nCntTitle]->SetScaling(SIZE_TITLE);
		}

		// 状態を変更
		m_state = STATE_WAIT;	// 遷移待機状態

		// カメラの更新を再開
		CManager::GetCamera()->SetEnableUpdate(true);
	}
}

//============================================================
//	遷移決定の更新処理
//============================================================
void CTitleManager::UpdateStart(void)
{
	// ポインタを宣言
	CInputKeyboard	*pKeyboard = CManager::GetKeyboard();	// キーボード
	CInputPad		*pPad = CManager::GetPad();	// パッド

	if (pKeyboard->GetTrigger(DIK_RETURN)
	||  pKeyboard->GetTrigger(DIK_SPACE)
	||  pPad->GetTrigger(CInputPad::KEY_A)
	||  pPad->GetTrigger(CInputPad::KEY_B)
	||  pPad->GetTrigger(CInputPad::KEY_X)
	||  pPad->GetTrigger(CInputPad::KEY_Y)
	||  pPad->GetTrigger(CInputPad::KEY_START))
	{
		if (m_state != STATE_WAIT)
		{ // 遷移待機状態ではない場合

			// 演出スキップ
			SkipStaging();
		}
		else
		{ // 遷移待機状態の場合

			// シーンの設定
			CManager::SetScene(CScene::MODE_GAME);	// ゲーム画面
		}
	}
}

//============================================================
//	演出スキップ処理
//============================================================
void CTitleManager::SkipStaging(void)
{
	// フェードを透明にする
	m_pFade->SetColor(XCOL_ABLACK);

	// タイトルロゴを表示状態に設定・大きさを正規化
	for (int nCntTitle = 0; nCntTitle < NUM_LOGO; nCntTitle++)
	{ // タイトルロゴの数分繰り返す

		// タイトルロゴの大きさを設定
		m_apLogo[nCntTitle]->SetScaling(SIZE_TITLE);

		// 描画をする設定にする
		m_apLogo[nCntTitle]->SetEnableDraw(true);
	}

	// カメラの更新を再開
	CManager::GetCamera()->SetEnableUpdate(true);

	// 状態を変更
	m_state = STATE_WAIT;	// 遷移待機状態
}
