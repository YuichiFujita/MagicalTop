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
#include "letterManager.h"
#include "object2D.h"
#include "objectGauge2D.h"
#include "player.h"

//************************************************************
//	マクロ定義
//************************************************************
#define TUTORIAL_PRIO	(6)	// チュートリアルの優先順位

#define LETTER_WAIT_CNT	(90)	// 待機状態の待機フレーム
#define TITLE_WAIT_CNT	(90)	// タイトル遷移の余韻フレーム

#define COL_FADE_MIN	(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f))	// フェードのα値の最小
#define COL_FADE_MAX	(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f))	// フェードのα値の最大
#define FADE_LEVEL		(0.025f)	// フェードのα値の加減量

#define SIZE_EXPLAIN	(D3DXVECTOR3(1000.0f, 580.0f, 0.0f))	// 説明表示の大きさ
#define SET_SCALE		(0.05f)		// 拡大率の初期値
#define ADD_SCALE		(0.1f)		// 拡大率の加算量

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CTutorialManager::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\tutorial000.png",	// テクスチャ
};

const char *CTutorialManager::mc_apLessonTextureFile[] =	// レッスンテクスチャ定数
{
	NULL,	// レッスン00：テクスチャなし
	"data\\TEXTURE\\lesson000.png",	// レッスン01：吸い込まれる説明のテクスチャ
	"data\\TEXTURE\\lesson001.png",	// レッスン02：前後加速の説明のテクスチャ
	"data\\TEXTURE\\lesson002.png",	// レッスン03：左右加速の説明のテクスチャ
};

const int CTutorialManager::mc_aNextLesson[] =	// レッスン移行カウント
{
	0,		// レッスンなし
	30,		// レッスン01：吸い込まれる終了カウント
	360,	// レッスン02：前後加速の終了カウント
	360,	// レッスン03：左右加速の終了カウント
};

const int CTutorialManager::mc_aNextLessonWait[] =	// 次レッスン余韻カウント
{
	0,		// レッスンなし
	60,		// レッスン01：吸い込まれる終了時の余韻カウント
	90,		// レッスン02：前後加速の終了時の余韻カウント
	90,		// レッスン03：左右加速の終了時の余韻カウント
};

//************************************************************
//	親クラス [CTutorialManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTutorialManager::CTutorialManager()
{
	// メンバ変数をクリア
	m_pLetterManager	= NULL;	// 手紙マネージャーの情報
	m_pConterLesson		= NULL;	// レッスン管理カウンターの情報
	m_pFade		= NULL;			// フェードの情報
	m_pExplain	= NULL;			// 説明表示の情報
	m_state		= STATE_NONE;	// 状態
	m_nLesson	= 0;			// レッスン
	m_fScale	= 0.0f;			// ポリゴン拡大率
	m_nCounterState	= 0;		// 状態管理カウンター
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
	// メンバ変数を初期化
	m_pLetterManager	= NULL;	// 手紙マネージャーの情報
	m_pConterLesson		= NULL;	// レッスン管理カウンターの情報
	m_pFade		= NULL;			// フェードの情報
	m_pExplain	= NULL;			// 説明表示の情報
	m_state		= STATE_WAIT;	// 状態
	m_nLesson	= LESSON_NONE;	// レッスン
	m_fScale	= 0.0f;			// ポリゴン拡大率
	m_nCounterState	= 0;		// 状態管理カウンター

	//--------------------------------------------------------
	//	レッスン管理カウンターの生成・設定
	//--------------------------------------------------------
	// レッスン管理カウンターの生成
	m_pConterLesson = CObjectGauge2D::Create	// TODO:定数
	( // 引数
		CObject::LABEL_GAUGE,
		mc_aNextLesson[0],
		1,
		D3DXVECTOR3(200.0f, 200.0f, 0.0f),
		D3DXVECTOR3(100.0f, 20.0f, 0.0f)
	);
	if (UNUSED(m_pConterLesson))
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	フェードの生成・設定
	//--------------------------------------------------------
	// フェードの生成
	m_pFade = CObject2D::Create
	( // 引数
		SCREEN_CENT,	// 位置
		SCREEN_SIZE,	// 大きさ
		VEC3_ZERO,		// 向き
		COL_FADE_MIN	// 色
	);
	if (UNUSED(m_pFade))
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pFade->SetPriority(TUTORIAL_PRIO);

	//--------------------------------------------------------
	//	選択背景の生成・設定
	//--------------------------------------------------------
	// 選択背景の生成
	m_pExplain = CObject2D::Create
	( // 引数
		SCREEN_CENT,	// 位置
		SIZE_EXPLAIN * SET_SCALE	// 大きさ
	);
	if (UNUSED(m_pExplain))
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pExplain->SetPriority(TUTORIAL_PRIO);

	// 描画を停止
	m_pExplain->SetEnableDraw(false);

	//--------------------------------------------------------
	//	手紙マネージャーの生成・設定
	//--------------------------------------------------------
	// 手紙マネージャーの生成
	m_pLetterManager = CLetterManager::Create();
	if (UNUSED(m_pLetterManager))
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CTutorialManager::Uninit(void)
{
	// 手紙マネージャーの破棄
	if (FAILED(CLetterManager::Release(m_pLetterManager)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// フェードの終了
	m_pFade->Uninit();

	// 選択背景の終了
	m_pExplain->Uninit();

	// 成功を返す
	return S_OK;
}

//============================================================
//	更新処理
//============================================================
void CTutorialManager::Update(void)
{
	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:		// 何もしない状態

		// 無し

		break;

	case STATE_WAIT:		// 待機状態

		// 待機の更新
		UpdateWait();

		break;

	case STATE_LETTER:		// 手紙表示状態

		// 手紙表示の更新
		UpdateLetter();

		break;

	case STATE_FADEIN:		// フェードイン状態

		// フェードインの更新
		UpdateFadeIn();

		break;

	case STATE_EXPLAIN:		// 説明表示状態

		// 説明表示の更新
		UpdateExplain();

		break;

	case STATE_FADEOUT:		// フェードアウト状態

		// フェードアウトの更新
		UpdateFadeOut();

		break;

	case STATE_PROGRESSION:	// 進行状態

		// 無し

		break;

	case STATE_NEXTWAIT:	// 次レッスン待機状態

		// 次レッスン待機の更新
		UpdateNextWait();

		break;

	case STATE_END:			// 終了状態

		// シーンの設定
		CManager::SetScene(CScene::MODE_TITLE, TITLE_WAIT_CNT);	// タイトル画面

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	if (USED(m_pLetterManager))
	{ // 使用中の場合

		// 手紙マネージャーの更新
		m_pLetterManager->Update();
	}
	else { assert(false); }	// 非使用中

	// フェードの更新
	m_pFade->Update();

	// 選択背景の更新
	m_pExplain->Update();
}

//============================================================
//	レッスンカウンターの加算処理
//============================================================
void CTutorialManager::AddLessonCounter(void)
{
	if (m_nLesson < LESSON_MAX)
	{ // レッスンがまだある場合

		// レッスンカウンターを加算
		m_pConterLesson->AddNum(1);

		if (m_pConterLesson->GetNum() >= mc_aNextLesson[m_nLesson])
		{ // レッスンを次に進めるカウントまで到達した場合

			// 状態を設定
			m_state = STATE_NEXTWAIT;	// 次レッスン待機状態
		}
	}
}

//============================================================
//	レッスン取得処理
//============================================================
int CTutorialManager::GetLesson(void) const
{
	// 現在のレッスンを返す
	return m_nLesson;
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
		if (FAILED(prTutorialManager->Uninit()))
		{ // 破棄に失敗した場合

			// メモリ開放
			delete prTutorialManager;
			prTutorialManager = NULL;

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// メモリ開放
		delete prTutorialManager;
		prTutorialManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	待機の更新処理
//============================================================
void CTutorialManager::UpdateWait(void)
{
	if (m_nCounterState < LETTER_WAIT_CNT)
	{ // カウンターが待機時間未満の場合

		// カウンターを加算
		m_nCounterState++;
	}
	else
	{ // 待機時間が終了した場合

		// カウンターを初期化
		m_nCounterState = 0;

		// 手紙マネージャーの状態を設定
		m_pLetterManager->SetState(CLetterManager::STATE_LETTER);	// 手紙の表示状態

		// 状態を設定
		m_state = STATE_LETTER;	// 手紙表示状態
	}
}

//============================================================
//	手紙表示の更新処理
//============================================================
void CTutorialManager::UpdateLetter(void)
{
	if (m_pLetterManager->GetState() == CLetterManager::STATE_END)
	{ // 手紙を読み終えた場合

		// 次レッスンへの移行
		NextLesson();
	}
}

//============================================================
//	フェードインの更新処理
//============================================================
void CTutorialManager::UpdateFadeIn(void)
{
	// 変数を宣言
	D3DXCOLOR colFade = m_pFade->GetColor();	// フェードの色

	if (colFade.a < COL_FADE_MAX.a)
	{ // 透明量が設定値未満の場合

		// 透明度を加算
		colFade.a += FADE_LEVEL;
	}
	else
	{ // 透明量が設定値以上の場合

		// 透明度を補正
		colFade.a = COL_FADE_MAX.a;

		// 説明表示の描画を再開
		m_pExplain->SetEnableDraw(true);

		// 説明表示の初期ポリゴン拡大率を設定
		m_fScale = SET_SCALE;

		// 状態を設定
		m_state = STATE_EXPLAIN;	// 説明表示状態
	}

	// 透明度を反映
	m_pFade->SetColor(colFade);
}

//============================================================
//	説明表示の更新処理
//============================================================
void CTutorialManager::UpdateExplain(void)
{
	if (m_fScale < 1.0f)
	{ // 拡大率が最小値未満の場合

		// 拡大率を加算
		m_fScale += ADD_SCALE;

		// 説明表示の大きさを設定
		m_pExplain->SetScaling(SIZE_EXPLAIN * m_fScale);
	}
	else
	{ // 拡大率が最小値以下の場合

		// 説明表示の大きさを設定
		m_pExplain->SetScaling(SIZE_EXPLAIN);

		if (CManager::GetKeyboard()->GetTrigger(DIK_0))
		{ // TODO：操作

			// 説明表示の初期ポリゴン拡大率を設定
			m_fScale = SET_SCALE;

			// 説明表示の大きさを設定
			m_pExplain->SetScaling(SIZE_EXPLAIN * m_fScale);

			// 説明表示の描画を停止
			m_pExplain->SetEnableDraw(false);

			// 状態を設定
			m_state = STATE_FADEOUT;	// フェードアウト状態
		}
	}
}

//============================================================
//	フェードアウトの更新処理
//============================================================
void CTutorialManager::UpdateFadeOut(void)
{
	// 変数を宣言
	D3DXCOLOR colFade = m_pFade->GetColor();	// フェードの色

	if (colFade.a > COL_FADE_MIN.a)
	{ // 透明量が設定値より大きい場合

		// 透明度を減算
		colFade.a -= FADE_LEVEL;
	}
	else
	{ // 透明量が設定値以下の場合

		// 透明度を補正
		colFade.a = COL_FADE_MIN.a;

		// プレイヤーを再出現させる
		CScene::GetPlayer()->SetRespawn(PLAY_SPAWN_POS);

		// 状態を設定
		m_state = STATE_PROGRESSION;	// 進行状態
	}

	// 透明度を反映
	m_pFade->SetColor(colFade);
}

//============================================================
//	次レッスン待機の更新処理
//============================================================
void CTutorialManager::UpdateNextWait(void)
{
	if (m_nCounterState < mc_aNextLessonWait[m_nLesson])
	{ // カウンターが余韻時間未満の場合

		// カウンターを加算
		m_nCounterState++;
	}
	else
	{ // 余韻時間が終了した場合

		// カウンターを初期化
		m_nCounterState = 0;

		// 次レッスンへの移行
		NextLesson();
	}
}

//============================================================
//	次レッスンへの移行処理
//============================================================
void CTutorialManager::NextLesson(void)
{
	// ポインタを宣言
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ

	//--------------------------------------------------------
	//	レッスン・カウンターを更新
	//--------------------------------------------------------
	// レッスンを次に進める
	m_nLesson++;

	// レッスンカウンターの最大値を設定
	m_pConterLesson->SetMaxNum(mc_aNextLesson[m_nLesson]);

	// レッスンカウンターを初期化
	m_pConterLesson->SetNum(0);

	//--------------------------------------------------------
	//	レッスン表示を更新
	//--------------------------------------------------------
	// レッスン説明テクスチャを登録・割当
	m_pExplain->BindTexture(pTexture->Regist(mc_apLessonTextureFile[m_nLesson]));

	//--------------------------------------------------------
	//	状態を更新
	//--------------------------------------------------------
	// プレイヤーを見えなくする
	CScene::GetPlayer()->SetDisp(false);

	if (m_nLesson < LESSON_MAX)
	{ // レッスンがまだある場合

		// 状態を設定
		m_state = STATE_FADEIN;	// フェードイン状態
	}
	else
	{ // レッスンがもうない場合

		// 状態を設定
		m_state = STATE_END;	// 終了状態
	}
}
