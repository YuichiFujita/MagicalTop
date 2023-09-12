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
#include "field.h"
#include "wall.h"
#include "scenery.h"
#include "sky.h"
#include "flower.h"
#include "weed.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CWaveManager	*CSceneGame::m_pWaveManager  = NULL;	// ウェーブマネージャー
CTimerManager	*CSceneGame::m_pTimerManager = NULL;	// タイマーマネージャー
CPause	*CSceneGame::m_pPause	= NULL;					// ポーズ
CStage	*CSceneGame::m_pStage	= NULL;					// ステージ
CPlayer	*CSceneGame::m_pPlayer	= NULL;					// プレイヤーオブジェクト
CField	*CSceneGame::m_pField	= NULL;					// 地面オブジェクト
CTarget	*CSceneGame::m_pTarget	= NULL;					// ターゲットオブジェクト
CScore	*CSceneGame::m_pScore	= NULL;					// スコアオブジェクト

//************************************************************
//	子クラス [CSceneGame] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSceneGame::CSceneGame()
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

	// ステージの生成
	m_pStage = CStage::Create();
	if (UNUSED(m_pStage))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 海オブジェクトの生成
	CSea::Create();

	// 地面オブジェクトの生成
	m_pField = CField::Create(CField::TEXTURE_SPRING, D3DXVECTOR3(0.0f, 400.0f, 0.0f), VEC3_ZERO, D3DXVECTOR2(6000.0f, 6000.0f), XCOL_WHITE, POSGRID2(120, 120));
	if (UNUSED(m_pField))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 地形を設定
	m_pField->SetTerrain(CField::TERRAIN_120x120);

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
	m_pTarget = CTarget::Create(CTarget::MODEL_NORMAL, D3DXVECTOR3(0.0f, 400.0f, 0.0f), VEC3_ZERO);
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
	m_pScore = CScore::Create();
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

	// マナフラワーランダム生成
	CFlower::RandomSpawn(30, CFlower::TYPE_SPRING, D3DXVECTOR3(50.0f, 50.0f, 0.0f), 10);	// TODO：定数変更

	// 草ランダム生成
	CWeed::RandomSpawn(100, CWeed::TYPE_SPRING, D3DXVECTOR3(60.0f, 90.0f, 0.0f));	// TODO：定数変更

	// カメラ目標位置設定
	CManager::GetCamera()->SetDestBargainingCamera();

	// タイムを計測開始
	m_pTimerManager->Start();
	m_pTimerManager->EnableStop(true);

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

	// ステージの破棄
	if (FAILED(CStage::Release(m_pStage)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 終了済みのオブジェクトポインタをNULLにする
	m_pPlayer	= NULL;		// プレイヤーオブジェクト
	m_pTarget	= NULL;		// ターゲットオブジェクト
	m_pField	= NULL;		// 地面オブジェクト
	m_pScore	= NULL;		// スコアオブジェクト

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

		if (USED(m_pStage))
		{ // 使用中の場合

			// ステージの更新
			m_pStage->Update();
		}
		else { assert(false); }	// 非使用中

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
//	ステージ取得処理
//============================================================
CStage *CSceneGame::GetStage(void)
{
	// ステージのポインタを返す
	return m_pStage;
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
//	地面取得処理
//============================================================
CField *CSceneGame::GetField(void)
{
	// 地面のポインタを返す
	return m_pField;
}

//============================================================
//	スコア取得処理
//============================================================
CScore *CSceneGame::GetScore(void)
{
	// スコアのポインタを返す
	return m_pScore;
}
