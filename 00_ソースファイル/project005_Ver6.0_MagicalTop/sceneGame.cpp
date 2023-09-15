//============================================================
//
//	ゲーム画面処理 [sceneGame.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "sceneGame.h"
#include "manager.h"
#include "sound.h"
#include "camera.h"

#include "waveManager.h"
#include "timerManager.h"
#include "stage.h"
#include "pause.h"
#include "target.h"
#include "player.h"
#include "score.h"

#include "enemy.h"
#include "magic.h"
#include "sea.h"
#include "wall.h"
#include "scenery.h"
#include "sky.h"
#include "flower.h"
#include "weed.h"

//************************************************************
//	マクロ定義
//************************************************************
#define SCO_POS		(D3DXVECTOR3(824.0f, 42.0f, 0.0f))		// スコア位置
#define SCO_SIZE	(D3DXVECTOR3(60.0f, 80.0f, 0.0f))		// スコア大きさ
#define SCO_SPACE	(D3DXVECTOR3(SCO_SIZE.x, 0.0f, 0.0f))	// スコア空白

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CWaveManager	*CSceneGame::m_pWaveManager  = NULL;	// ウェーブマネージャー
CTimerManager	*CSceneGame::m_pTimerManager = NULL;	// タイマーマネージャー
CPause	*CSceneGame::m_pPause	= NULL;					// ポーズ
CPlayer	*CSceneGame::m_pPlayer	= NULL;					// プレイヤーオブジェクト
CTarget	*CSceneGame::m_pTarget	= NULL;					// ターゲットオブジェクト
CScore	*CSceneGame::m_pScore	= NULL;					// スコアオブジェクト

//************************************************************
//	子クラス [CSceneGame] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSceneGame::CSceneGame(const MODE mode) : CScene(mode)
{

}

//============================================================
//	デストラクタ
//============================================================
CSceneGame::~CSceneGame()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSceneGame::Init(void)
{
	//--------------------------------------------------------
	//	オブジェクト生成
	//--------------------------------------------------------
	// ウェーブマネージャーの生成
	m_pWaveManager = CWaveManager::Create();
	if (UNUSED(m_pWaveManager))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// タイマーマネージャーの生成
	m_pTimerManager = CTimerManager::Create();
	if (UNUSED(m_pTimerManager))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ポーズの生成
	m_pPause = CPause::Create();
	if (UNUSED(m_pPause))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// シーンの初期化
	CScene::Init();

	// 海オブジェクトの生成
	CSea::Create();

	// 壁オブジェクトの生成
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3( 0.0f,    0.0f, -3000.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),   D3DXVECTOR2(6000.0f, 400.0f), XCOL_WHITE, POSGRID2(18, 1));
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3(-3000.0f, 0.0f,  0.0f),    D3DXToRadian(D3DXVECTOR3(0.0f, 90.0f, 0.0f)),  D3DXVECTOR2(6000.0f, 400.0f), XCOL_WHITE, POSGRID2(18, 1));
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3( 0.0f,    0.0f,  3000.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 180.0f, 0.0f)), D3DXVECTOR2(6000.0f, 400.0f), XCOL_WHITE, POSGRID2(18, 1));
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3( 3000.0f, 0.0f,  0.0f),    D3DXToRadian(D3DXVECTOR3(0.0f, 270.0f, 0.0f)), D3DXVECTOR2(6000.0f, 400.0f), XCOL_WHITE, POSGRID2(18, 1));

	// 景色オブジェクトの生成
	CScenery::Create(CScenery::TEXTURE_NORMAL, VEC3_ZERO, VEC3_ZERO,                                    XCOL_WHITE,                        POSGRID2(32, 1), 12000.0f, 1000.0f, D3DCULL_CW, false);
	CScenery::Create(CScenery::TEXTURE_NORMAL, VEC3_ZERO, D3DXToRadian(D3DXVECTOR3(0.0f, 85.0f, 0.0f)), D3DXCOLOR(0.7f, 1.0f, 1.0f, 1.0f), POSGRID2(32, 1), 14000.0f, 1600.0f, D3DCULL_CW, false);
	CScenery::Create(CScenery::TEXTURE_NORMAL, VEC3_ZERO, D3DXToRadian(D3DXVECTOR3(0.0f, 35.0f, 0.0f)), D3DXCOLOR(0.4f, 1.0f, 0.7f, 1.0f), POSGRID2(32, 1), 16000.0f, 2200.0f, D3DCULL_CW, false);

	// 空オブジェクトの生成
	CSky::Create(CSky::TEXTURE_NORMAL, VEC3_ZERO, VEC3_ZERO, XCOL_WHITE, POSGRID2(32, 6), 18000.0f, D3DCULL_CW, false);

	// ターゲットオブジェクトの生成
	m_pTarget = CTarget::Create(CTarget::MODEL_NORMAL, GetStage()->GetStageLimit().center, VEC3_ZERO);
	if (UNUSED(m_pTarget))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// プレイヤーオブジェクトの生成
	m_pPlayer = CPlayer::Create(VEC3_ZERO, VEC3_ZERO);
	if (UNUSED(m_pPlayer))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// スコアオブジェクトの生成
	m_pScore = CScore::Create
	( // 引数
		SCO_POS,	// 位置
		SCO_SIZE,	// 大きさ
		SCO_SPACE	// 空白
	);
	if (UNUSED(m_pScore))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	初期設定
	//--------------------------------------------------------
	// 敵セットアップの読込
	CEnemy::LoadSetup();

	// 魔法セットアップの読込
	CMagic::LoadSetup();

	// マナフラワーセットアップの読込
	CFlower::LoadSetup();

	// 草セットアップの読込
	CWeed::LoadSetup();

	// マナフラワーランダム生成
	CFlower::RandomSpawn(30, CFlower::TYPE_SPRING);	// TODO：定数変更

	// 草ランダム生成
	CWeed::RandomSpawn(200, CWeed::TYPE_SPRING);	// TODO：定数変更

	// カメラを設定
	CManager::GetCamera()->SetState(CCamera::STATE_BARGAINING);	// カメラを寄り引き状態に設定
	CManager::GetCamera()->SetDestBargaining();	// 目標位置を設定

	// タイムを計測開始
	m_pTimerManager->Start();			// 計測を開始
	m_pTimerManager->EnableStop(true);	// 計測を一時停止

	// BGMの再生
	//CManager::GetSound()->Play(CSound::LABEL_BGM_000);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CSceneGame::Uninit(void)
{
	// ウェーブマネージャーの破棄
	if (FAILED(CWaveManager::Release(m_pWaveManager)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// タイマーマネージャーの破棄
	if (FAILED(CTimerManager::Release(m_pTimerManager)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ポーズの破棄
	if (FAILED(CPause::Release(m_pPause)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 終了済みのオブジェクトポインタをNULLにする
	m_pPlayer	= NULL;		// プレイヤーオブジェクト
	m_pTarget	= NULL;		// ターゲットオブジェクト
	m_pScore	= NULL;		// スコアオブジェクト

	// シーンの終了
	CScene::Uninit();

	// 成功を返す
	return S_OK;
}

//============================================================
//	更新処理
//============================================================
void CSceneGame::Update(void)
{
	if (USED(m_pTimerManager))
	{ // 使用中の場合

		// タイマーマネージャーの更新
		m_pTimerManager->Update();
	}
	else { assert(false); }	// 非使用中

	if (USED(m_pPause))
	{ // 使用中の場合

		// ポーズの更新
		m_pPause->Update();
	}
	else { assert(false); }	// 非使用中

	if (!m_pPause->IsPause())
	{ // ポーズ中ではない場合

		if (USED(m_pWaveManager))
		{ // 使用中の場合

			// ウェーブマネージャーの更新
			m_pWaveManager->Update();
		}
		else { assert(false); }	// 非使用中

		// シーンの更新
		CScene::Update();
	}

	// TODO：遷移のタイミングで壊れる問題シーンにも死亡フラグ追加で回避
	//		 シーンマネージャーを用意する
}

//============================================================
//	描画処理
//============================================================
void CSceneGame::Draw(void)
{

}

//============================================================
//	ウェーブマネージャー取得処理
//============================================================
CWaveManager *CSceneGame::GetWaveManager(void)
{
	// ウェーブマネージャーのポインタを返す
	return m_pWaveManager;
}

//============================================================
//	タイマーマネージャー取得処理
//============================================================
CTimerManager *CSceneGame::GetTimerManager(void)
{
	// タイマーマネージャーのポインタを返す
	return m_pTimerManager;
}

//============================================================
//	ポーズ取得処理
//============================================================
CPause *CSceneGame::GetPause(void)
{
	// ポーズのポインタを返す
	return m_pPause;
}

//============================================================
//	プレイヤー取得処理
//============================================================
CPlayer *CSceneGame::GetPlayer(void)
{
	// プレイヤーのポインタを返す
	return m_pPlayer;
}

//============================================================
//	ターゲット取得処理
//============================================================
CTarget *CSceneGame::GetTarget(void)
{
	// ターゲットのポインタを返す
	return m_pTarget;
}

//============================================================
//	スコア取得処理
//============================================================
CScore *CSceneGame::GetScore(void)
{
	// スコアのポインタを返す
	return m_pScore;
}
