//============================================================
//
//	タイマーマネージャー処理 [timerManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "timerManager.h"
#include "manager.h"
#include "renderer.h"
#include "value.h"

#ifdef _DEBUG	// デバッグ処理

#include "input.h"

#endif	// _DEBUG

//************************************************************
//	マクロ定義
//************************************************************
#define TIME_POS	(D3DXVECTOR3(36.0f, 42.0f, 0.0f))		// ポリゴン位置
#define TIME_SIZE	(D3DXVECTOR3(60.0f, 80.0f, 0.0f))		// ポリゴン大きさ
#define TIME_SPACE	(D3DXVECTOR3(TIME_SIZE.x, 0.0f, 0.0f))	// ポリゴン間の空白

#define TIME_NUMMIN	((DWORD)0)			// 最少タイム
#define TIME_NUMMAX	((DWORD)99 * 60000)	// 最大タイム

//************************************************************
//	子クラス [CTimerManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTimerManager::CTimerManager()
{
	// メンバ変数をクリア
	memset(&m_apValue[0], 0, sizeof(m_apValue));	// 数値の情報
	m_dwStartTime	= 0;			// 開始時間
	m_dwTime		= 0;			// 経過時間
	m_dwStopTime	= 0;			// 停止時間
	m_state			= STATE_NONE;	// 計測状態
	m_bStop			= true;			// 計測停止状況
}

//============================================================
//	デストラクタ
//============================================================
CTimerManager::~CTimerManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTimerManager::Init(void)
{
	// メンバ変数を初期化
	memset(&m_apValue[0], 0, sizeof(m_apValue));	// 数値の情報
	m_dwStartTime	= 0;			// 開始時間
	m_dwTime		= 0;			// 経過時間
	m_dwStopTime	= 0;			// 停止時間
	m_state			= STATE_NONE;	// 計測状態
	m_bStop			= true;			// 計測停止状況

	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{ // タイマーの桁数分繰り返す

		// 数字の生成
		m_apValue[nCntTimer] = CValue::Create(CValue::TEXTURE_NORMAL, TIME_POS + (TIME_SPACE * (float)nCntTimer), TIME_SIZE);
		if (UNUSED(m_apValue[nCntTimer]))
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CTimerManager::Uninit(void)
{
	// 数字オブジェクトを破棄
	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{ // タイマーの桁数分繰り返す

		// 数字の終了
		m_apValue[nCntTimer]->Uninit();
	}
}

//============================================================
//	更新処理
//============================================================
void CTimerManager::Update(void)
{
#ifdef _DEBUG	// デバッグ処理

	if (CManager::GetKeyboard()->GetTrigger(DIK_F2))
	{
		// 停止状況を逆転
		EnableStop((m_bStop) ? false : true);
	}

#if 0
	if (CManager::GetKeyboard()->GetTrigger(DIK_F3))
	{
		// タイマーを加算
		AddMSec(1);
	}
	if (CManager::GetKeyboard()->GetTrigger(DIK_F4))
	{
		// タイマーを加算
		AddSec(1);
	}
	if (CManager::GetKeyboard()->GetTrigger(DIK_F5))
	{
		// タイマーを加算
		AddMin(1);
	}
#endif

#endif	// _DEBUG

	switch (m_state)
	{ // 計測状態ごとの処理
	case STATE_NONE:

		break;

	case STATE_MEASURE:

		if (m_bStop == false)
		{ // 計測中の場合

			// 現在の計測ミリ秒を設定
			m_dwTime = timeGetTime() - m_dwStopTime - m_dwStartTime;

			// タイマーの補正
			useful::LimitNum(m_dwTime, TIME_NUMMIN, TIME_NUMMAX);
		}
		else
		{ // 計測停止中の場合

			// 現在の停止ミリ秒を設定
			m_dwStopTime = timeGetTime() - m_dwTime - m_dwStartTime;

			// タイマーの補正
			useful::LimitNum(m_dwTime, TIME_NUMMIN, TIME_NUMMAX);
		}

		break;

	case STATE_END:

		// 無し

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// 数字のテクスチャ座標の設定
	SetTexNum();

	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{ // タイマーの桁数分繰り返す

		// 数字の更新
		m_apValue[nCntTimer]->Update();
	}

	// デバッグ表示
	CManager::GetDebugProc()->Print("タイマー：[%d:%d:%d]\n", m_dwTime / 60000, (m_dwTime / 1000) % 60, m_dwTime % 1000);
	CManager::GetDebugProc()->Print("停止タイマー：[%d:%d:%d]\n", m_dwStopTime / 60000, (m_dwStopTime / 1000) % 60, m_dwStopTime % 1000);
}

//============================================================
//	生成処理
//============================================================
CTimerManager *CTimerManager::Create(void)
{
	// ポインタを宣言
	CTimerManager *pTimerManager = NULL;	// タイマーマネージャー生成用

	if (UNUSED(pTimerManager))
	{ // 使用されていない場合

		// メモリ確保
		pTimerManager = new CTimerManager;	// タイマーマネージャー
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pTimerManager))
	{ // 確保に成功している場合

		// タイマーマネージャーの初期化
		if (FAILED(pTimerManager->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pTimerManager;
			pTimerManager = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pTimerManager;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CTimerManager::Release(CTimerManager *&prTimerManager)
{
	if (USED(prTimerManager))
	{ // 使用中の場合

		// タイマーマネージャーの終了
		prTimerManager->Uninit();

		// メモリ開放
		delete prTimerManager;
		prTimerManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	計測開始処理
//============================================================
void CTimerManager::Start(void)
{
	if (m_state != STATE_MEASURE)
	{ // タイムの計測中ではない場合

		// タイマーを設定
		m_dwStartTime = timeGetTime();	// 現在時刻を取得

		// 非停止状態にする
		EnableStop(false);

		// 計測開始状態にする
		m_state = STATE_MEASURE;
	}
}

//============================================================
//	計測終了処理
//============================================================
void CTimerManager::End(void)
{
	// 計測終了状態にする
	m_state = STATE_END;
}

//============================================================
//	計測停止の有効無効の設定処理
//============================================================
void CTimerManager::EnableStop(const bool bStop)
{
	// 引数の停止状況を代入
	m_bStop = bStop;
}

#if 0
//============================================================
//	ミリ秒の加算処理
//============================================================
void CTimerManager::AddMSec(const int nMSec)
{
	// 引数のミリ秒を加算
	m_dwTime += nMSec;

	// タイマーの補正
	useful::LimitNum(m_dwTime, TIME_NUMMIN, TIME_NUMMAX);

	// 数字のテクスチャ座標の設定
	SetTexNum();
}

//============================================================
//	秒の加算処理
//============================================================
void CTimerManager::AddSec(const int nSec)
{
	// 引数の秒を加算
	m_dwTime += nSec * 1000;	// ミリ秒に変換

	// タイマーの補正
	useful::LimitNum(m_dwTime, TIME_NUMMIN, TIME_NUMMAX);

	// 数字のテクスチャ座標の設定
	SetTexNum();
}

//============================================================
//	分の加算処理
//============================================================
void CTimerManager::AddMin(const int nMin)
{
	// 引数の分を加算
	m_dwTime += nMin * 60000;	// ミリ秒に変換

	// タイマーの補正
	useful::LimitNum(m_dwTime, TIME_NUMMIN, TIME_NUMMAX);

	// 数字のテクスチャ座標の設定
	SetTexNum();
}
#endif

//============================================================
//	ミリ秒の取得処理
//============================================================
int CTimerManager::GetMSec(void)
{
	// ミリ秒を返す
	return m_dwTime % 1000;
}

//============================================================
//	秒の取得処理
//============================================================
int CTimerManager::GetSec(void)
{
	// 秒を返す
	return (m_dwTime / 1000) % 60;
}

//============================================================
//	分の取得処理
//============================================================
int CTimerManager::GetMin(void)
{
	// 分を返す
	return m_dwTime / 60000;
}

//============================================================
//	数字のテクスチャ座標の設定処理
//============================================================
void CTimerManager::SetTexNum(void)
{
	// 変数を宣言
	int aNumDivide[MAX_TIMER];	// 分の桁数ごとの分解用

	// 分を桁数ごとに分解
	useful::DivideDigitNum(&aNumDivide[0], GetMin(), MAX_MIN);

	// 秒を桁数ごとに分解
	useful::DivideDigitNum(&aNumDivide[MAX_MIN], GetSec(), MAX_SEC);

	// ミリ秒を桁数ごとに分解
	useful::DivideDigitNum(&aNumDivide[MAX_MIN + MAX_SEC], GetMSec(), MAX_MSEC);

	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{ // タイマーの桁数分繰り返す

		// 数字の設定
		m_apValue[nCntTimer]->SetNumber(aNumDivide[nCntTimer]);
	}
}
