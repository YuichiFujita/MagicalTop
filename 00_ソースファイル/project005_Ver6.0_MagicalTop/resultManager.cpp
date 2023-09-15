//============================================================
//
//	リザルトマネージャー処理 [resultManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "resultManager.h"
#include "manager.h"
#include "input.h"
#include "camera.h"
#include "texture.h"
#include "model.h"
#include "object2D.h"
#include "score.h"
#include "timerManager.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CResultManager::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\result000.png",	// MISSIONテクスチャ
	"data\\TEXTURE\\result001.png",	// CLEARテクスチャ
	"data\\TEXTURE\\result002.png",	// FAILEDテクスチャ
	"data\\TEXTURE\\result003.png",	// スコア表示テクスチャ
	"data\\TEXTURE\\result004.png",	// タイム表示テクスチャ
};

//************************************************************
//	マクロ定義
//************************************************************
#define RESULT_PRIO	(6)		// リザルトの優先順位
#define WAIT_CNT	(10)	// 状態変更待機フレーム数

#define SIZE_FADE	(SCREEN_SIZE * 0.95f)	// フェードの大きさ
#define ADD_ALPHA	(0.008f)	// α値の加算量
#define SET_ALPHA	(0.6f)		// α値の停止値

#define POS_RESULT_MISSION	(D3DXVECTOR3(360.0f, 170.0f, 0.0f))	// リザルト表示のMISSION位置
#define POS_RESULT_RESULT	(D3DXVECTOR3(920.0f, 170.0f, 0.0f))	// リザルト表示のRESULT位置
#define SIZE_RESULT			(D3DXVECTOR3(632.7f, 203.5f, 0.0f))	// リザルト表示の大きさ
#define SET_RESULT_SCALE	(15.0f)	// リザルト表示の初期拡大率
#define SUB_RESULT_SCALE	(0.65f)	// リザルト表示拡大率の減算量

#define POS_SCORE_LOGO	(D3DXVECTOR3(250.0f, 400.0f, 0.0f))		// スコアロゴ位置
#define SIZE_SCORE_LOGO	(D3DXVECTOR3(487.5f, 154.7f, 0.0f))		// スコアロゴ大きさ
#define POS_SCORE		(D3DXVECTOR3(490.0f, 400.0f, 0.0f))		// スコア位置
#define SIZE_SCORE		(D3DXVECTOR3(94.0f, 112.0f, 0.0f))		// スコア大きさ
#define SPACE_SCORE		(D3DXVECTOR3(SIZE_SCORE.x, 0.0f, 0.0f))	// スコア空白
#define SET_SCORE_SCALE	(8.0f)	// スコア表示の初期拡大率
#define SUB_SCORE_SCALE	(0.95f)	// スコア表示拡大率の減算量

#define POS_TIME_LOGO	(D3DXVECTOR3(250.0f, 560.0f, 0.0f))			// タイムロゴ位置
#define SIZE_TIME_LOGO	(D3DXVECTOR3(487.5f, 154.7f, 0.0f))			// タイムロゴ大きさ
#define POS_TIME		(D3DXVECTOR3(490.0f, 560.0f, 0.0f))			// タイム位置
#define SIZE_TIME_VAL	(D3DXVECTOR3(94.0f, 112.0f, 0.0f))			// タイム数字大きさ
#define SIZE_TIME_PART	(D3DXVECTOR3(48.0f, 112.0f, 0.0f))			// タイム区切り大きさ
#define SPACE_TIME_VAL	(D3DXVECTOR3(SIZE_TIME_VAL.x, 0.0f, 0.0f))	// タイム数字空白
#define SPACE_TIME_PART	(D3DXVECTOR3(SIZE_TIME_PART.x, 0.0f, 0.0f))	// タイム区切り空白
#define SET_TIME_SCALE	(1.0f)	// タイム表示の初期拡大率
#define SUB_TIME_SCALE	(0.95f)	// タイム表示拡大率の減算量

//************************************************************
//	親クラス [CResultManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CResultManager::CResultManager()
{
	// メンバ変数をクリア
	memset(&m_apResult[0], 0, sizeof(m_apResult));	// リザルト表示の情報
	m_pScoreLogo	= NULL;			// スコアロゴの情報
	m_pTimeLogo		= NULL;			// タイムロゴの情報
	m_pFade			= NULL;			// フェードの情報
	m_pScore		= NULL;			// スコアの情報
	m_pTime			= NULL;			// タイムの情報
	m_state			= STATE_NONE;	// 状態
	m_nCounterState	= 0;			// 状態管理カウンター
	m_fScale		= 0.0f;			// ポリゴン拡大率
}

//============================================================
//	デストラクタ
//============================================================
CResultManager::~CResultManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CResultManager::Init(void)
{
	// 変数配列を宣言
	D3DXVECTOR3 aPosResult[] =	// リザルトの位置
	{ // 初期値
		POS_RESULT_MISSION,	// MISSION位置
		POS_RESULT_RESULT,	// RESULT位置
	};

	// ポインタを宣言
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ

	// メンバ変数を初期化
	memset(&m_apResult[0], 0, sizeof(m_apResult));	// リザルト表示の情報
	m_pScoreLogo	= NULL;			// スコアロゴの情報
	m_pTimeLogo		= NULL;			// タイムロゴの情報
	m_pFade			= NULL;			// フェードの情報
	m_pScore		= NULL;			// スコアの情報
	m_pTime			= NULL;			// タイムの情報
	m_state			= STATE_FADEIN;	// 状態
	m_nCounterState	= 0;			// 状態管理カウンター
	m_fScale		= 0.0f;			// ポリゴン拡大率

	//--------------------------------------------------------
	//	フェードの生成・設定
	//--------------------------------------------------------
	// フェードの生成
	m_pFade = CObject2D::Create
	( // 引数
		SCREEN_CENT,	// 位置
		SIZE_FADE,		// 大きさ
		VEC3_ZERO,		// 向き
		XCOL_AWHITE		// 色
	);
	if (UNUSED(m_pFade))
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pFade->SetPriority(RESULT_PRIO);

	//--------------------------------------------------------
	//	リザルト表示の生成・設定
	//--------------------------------------------------------
	for (int nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
	{ // リザルト表示の総数分繰り返す

		// リザルト表示の生成
		m_apResult[nCntResult] = CObject2D::Create
		( // 引数
			aPosResult[nCntResult],			// 位置
			SIZE_RESULT * SET_RESULT_SCALE	// 大きさ
		);
		if (UNUSED(m_apResult[nCntResult]))
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// 優先順位を設定
		m_apResult[nCntResult]->SetPriority(RESULT_PRIO);

		// 描画をしない設定にする
		m_apResult[nCntResult]->SetEnableDraw(false);
	}

	// リザルト表示のテクスチャを設定
	SetTexResult();

	//--------------------------------------------------------
	//	スコアロゴ表示の生成・設定
	//--------------------------------------------------------
	// スコアロゴ表示の生成
	m_pScoreLogo = CObject2D::Create
	( // 引数
		POS_SCORE_LOGO,						// 位置
		SIZE_SCORE_LOGO * SET_SCORE_SCALE	// 大きさ
	);
	if (UNUSED(m_pScoreLogo))
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを登録・割当
	m_pScoreLogo->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_SCORE]));

	// 優先順位を設定
	m_pScoreLogo->SetPriority(RESULT_PRIO);

	// 描画をしない設定にする
	m_pScoreLogo->SetEnableDraw(false);

	//--------------------------------------------------------
	//	スコア表示の生成・設定
	//--------------------------------------------------------
	// スコアオブジェクトの生成
	m_pScore = CScore::Create
	( // 引数
		POS_SCORE,						// 位置
		SIZE_SCORE * SET_SCORE_SCALE,	// 大きさ
		SPACE_SCORE						// 空白
	);
	if (UNUSED(m_pScore))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pScore->SetPriority(RESULT_PRIO);

	// 描画をしない設定にする
	m_pScore->SetEnableDraw(false);

	// スコアを設定
	m_pScore->Set(12345678);	// TODO：獲得スコアの保持

	//--------------------------------------------------------
	//	タイムロゴ表示の生成・設定
	//--------------------------------------------------------
	// タイムロゴ表示の生成
	m_pTimeLogo = CObject2D::Create
	( // 引数
		POS_TIME_LOGO,					// 位置
		SIZE_TIME_LOGO * SET_TIME_SCALE	// 大きさ
	);
	if (UNUSED(m_pTimeLogo))
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを登録・割当
	m_pTimeLogo->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_TIME]));

	// 優先順位を設定
	m_pTimeLogo->SetPriority(RESULT_PRIO);

	// 描画をしない設定にする
	//m_pTimeLogo->SetEnableDraw(false);

	//--------------------------------------------------------
	//	タイム表示の生成・設定
	//--------------------------------------------------------
	// タイマーマネージャーの生成
	m_pTime = CTimerManager::Create
	( // 引数
		POS_TIME,							// 位置
		SIZE_TIME_VAL * SET_TIME_SCALE,		// 数字の大きさ
		SIZE_TIME_PART * SET_TIME_SCALE,	// 区切りの大きさ
		SPACE_TIME_VAL,						// 数字の空白
		SPACE_TIME_PART						// 区切りの空白
	);
	if (UNUSED(m_pTime))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pTime->SetPriority(RESULT_PRIO);

	// 描画をしない設定にする
	//m_pTime->SetEnableDraw(false);

	// タイムを設定
	//m_pTime->(12345678);	// TODO：タイムの保持

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CResultManager::Uninit(void)
{
	for (int nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
	{ // リザルト表示の総数分繰り返す

		// リザルト表示の終了
		m_apResult[nCntResult]->Uninit();
	}

	// スコアロゴ表示の終了
	m_pScoreLogo->Uninit();

	// タイムロゴ表示の終了
	m_pTimeLogo->Uninit();

	// タイムの終了
	m_pTime->Uninit();

	// フェードの終了
	m_pFade->Uninit();

	// 終了済みのオブジェクトポインタをNULLにする
	m_pScore = NULL;	// スコアオブジェクト
}

//============================================================
//	更新処理
//============================================================
void CResultManager::Update(void)
{
	// 遷移決定の更新
	UpdateBack();

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:	// 何もしない状態

		// 無し

		break;

	case STATE_FADEIN:	// フェードイン状態

		// フェードインの更新
		UpdateFade();

		break;

	case STATE_RESULT:	// リザルト表示状態

		// リザルト表示の更新
		UpdateResult();

		break;

	case STATE_SCORE_WAIT:	// スコア表示待機状態

		// 表示待機の更新
		if (UpdateDrawWait())
		{ // 待機完了の場合

			// スコア表示の拡大率を設定
			m_fScale = SET_SCORE_SCALE;

			// スコア表示の描画開始
			m_pScoreLogo->SetEnableDraw(true);
			m_pScore->SetEnableDraw(true);

			// 状態を変更
			m_state = STATE_SCORE;	// スコア表示状態
		}

		break;

	case STATE_SCORE:	// スコア表示状態

		// スコア表示の更新
		UpdateScore();

		break;

	case STATE_WAIT:	// 遷移待機状態

		// 無し

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	for (int nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
	{ // リザルト表示の総数分繰り返す

		// リザルト表示の更新
		m_apResult[nCntResult]->Update();
	}

	// スコアロゴ表示の更新
	m_pScoreLogo->Update();

	// タイムロゴ表示の更新
	m_pTimeLogo->Update();

	// タイムの更新
	m_pTime->Update();

	// フェードの更新
	m_pFade->Update();
}

//============================================================
//	生成処理
//============================================================
CResultManager *CResultManager::Create(void)
{
	// ポインタを宣言
	CResultManager *pResultManager = NULL;	// リザルトマネージャー生成用

	if (UNUSED(pResultManager))
	{ // 使用されていない場合

		// メモリ確保
		pResultManager = new CResultManager;	// リザルトマネージャー
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pResultManager))
	{ // 使用されている場合
		
		// リザルトマネージャーの初期化
		if (FAILED(pResultManager->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pResultManager;
			pResultManager = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pResultManager;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CResultManager::Release(CResultManager *&prResultManager)
{
	if (USED(prResultManager))
	{ // 使用中の場合

		// リザルトマネージャーの終了
		prResultManager->Uninit();

		// メモリ開放
		delete prResultManager;
		prResultManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	フェードインの更新処理
//============================================================
void CResultManager::UpdateFade(void)
{
	// 変数を宣言
	D3DXCOLOR colFade = m_pFade->GetColor();	// フェードの色

	if (colFade.a < SET_ALPHA)
	{ // 透明量が設定値未満の場合

		// 透明度を加算
		colFade.a += ADD_ALPHA;
	}
	else
	{ // 透明量が設定値以上の場合

		// 透明度を補正
		colFade.a = SET_ALPHA;

		for (int nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
		{ // リザルト表示の総数分繰り返す

			// リザルト表示の描画開始
			m_apResult[nCntResult]->SetEnableDraw(true);
		}

		// リザルト表示の拡大率を設定
		m_fScale = SET_RESULT_SCALE;

		// 状態を変更
		m_state = STATE_RESULT;	// リザルト表示状態
	}

	// 透明度を反映
	m_pFade->SetColor(colFade);
}

//============================================================
//	リザルト表示の更新処理
//============================================================
void CResultManager::UpdateResult(void)
{
	if (m_fScale > 1.0f)
	{ // 拡大率が最小値より大きい場合

		// 拡大率を減算
		m_fScale -= SUB_RESULT_SCALE;

		for (int nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
		{ // リザルト表示の総数分繰り返す

			// リザルト表示の大きさを設定
			m_apResult[nCntResult]->SetScaling(SIZE_RESULT * m_fScale);
		}
	}
	else
	{ // 拡大率が最小値以下の場合

		for (int nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
		{ // リザルト表示の総数分繰り返す

			// リザルト表示の大きさを設定
			m_apResult[nCntResult]->SetScaling(SIZE_RESULT);
		}

		// 状態を変更
		m_state = STATE_SCORE_WAIT;	// スコア表示待機状態
	}
}

//============================================================
//	スコア表示の更新処理
//============================================================
void CResultManager::UpdateScore(void)
{
	if (m_fScale > 1.0f)
	{ // 拡大率が最小値より大きい場合

		// 拡大率を減算
		m_fScale -= SUB_SCORE_SCALE;

		// スコア表示の大きさを設定
		m_pScoreLogo->SetScaling(SIZE_SCORE_LOGO * m_fScale);
		m_pScore->SetScaling(SIZE_SCORE * m_fScale);
	}
	else
	{ // 拡大率が最小値以下の場合

		// 拡大率を補正
		m_fScale = 1.0f;

		// スコア表示の大きさを設定
		m_pScoreLogo->SetScaling(SIZE_SCORE_LOGO);
		m_pScore->SetScaling(SIZE_SCORE);

		// 状態を変更
		//m_state = STATE_SCORE;	// スコア表示状態	// TODO
	}
}

//============================================================
//	表示待機処理
//============================================================
bool CResultManager::UpdateDrawWait(void)
{
	if (m_nCounterState < WAIT_CNT)
	{ // カウンターが待機カウントまで達していない場合

		// カウンターを加算
		m_nCounterState++;

		// 待機未完了を返す
		return false;
	}
	else
	{ // カウンターが待機完了した場合

		// カウンターを初期化
		m_nCounterState = 0;

		// 待機完了を返す
		return true;
	}
}

//============================================================
//	遷移決定の更新処理
//============================================================
void CResultManager::UpdateBack(void)
{
	// ポインタを宣言
	CInputKeyboard	*pKeyboard	= CManager::GetKeyboard();	// キーボード
	CInputPad		*pPad		= CManager::GetPad();		// パッド

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
			CManager::SetScene(CScene::MODE_TITLE);	// タイトル画面
		}
	}
}

//============================================================
//	演出スキップ処理
//============================================================
void CResultManager::SkipStaging(void)
{
#if 0

	// リザルトロゴを表示状態に設定・大きさを正規化
	for (int nCntResult = 0; nCntResult < LOGO_MAX; nCntResult++)
	{ // リザルトロゴの総数分繰り返す

		// リザルトロゴの大きさを設定
		m_apLogo[nCntResult]->SetScaling(SIZE_RESULT);

		// 描画をする設定にする
		m_apLogo[nCntResult]->SetEnableDraw(true);
	}

	// 選択表示を描画する設定にする
	for (int nCntResult = 0; nCntResult < SELECT_MAX; nCntResult++)
	{ // 選択項目の総数分繰り返す

		m_apSelect[nCntResult]->SetEnableDraw(true);
	}

	// フェードを透明にする
	m_pFade->SetColor(XCOL_ABLACK);

	// 選択背景を描画する設定にする
	m_pSelectBG->SetEnableDraw(true);

	// カメラの更新を再開
	CManager::GetCamera()->SetEnableUpdate(true);

#endif

	// 状態を変更
	m_state = STATE_WAIT;	// 遷移待機状態
}

//============================================================
//	リザルト表示のテクスチャの設定処理
//============================================================
void CResultManager::SetTexResult(void)
{
	// ポインタを宣言
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ

	// MISSIONテクスチャを登録・割当
	m_apResult[0]->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_MISSION]));

	// RESULTテクスチャを登録・割当
	m_apResult[1]->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_FAILED]));
}
