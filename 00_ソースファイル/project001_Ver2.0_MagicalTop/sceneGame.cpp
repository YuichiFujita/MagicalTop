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
#include "stage.h"
#include "target.h"
#include "player.h"
#include "score.h"
#include "timer.h"

#include "enemy.h"
#include "magic.h"
#include "sea.h"
#include "field.h"
#include "wall.h"
#include "scenery.h"
#include "sky.h"
#include "flower.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CWaveManager *CSceneGame::m_pWaveManager = NULL;	// ウェーブマネージャー
CStage	*CSceneGame::m_pStage	= NULL;				// ステージ
CPlayer	*CSceneGame::m_pPlayer	= NULL;				// プレイヤーオブジェクト
CField	*CSceneGame::m_pField	= NULL;				// 地面オブジェクト
CTarget	*CSceneGame::m_pTarget	= NULL;				// ターゲットオブジェクト
CScore	*CSceneGame::m_pScore	= NULL;				// スコアオブジェクト
CTimer	*CSceneGame::m_pTimer	= NULL;				// タイマーオブジェクト

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
	m_pField = CField::Create(CField::TEXTURE_NORMAL, D3DXVECTOR3(0.0f, 400.0f, 0.0f), VEC3_ZERO, D3DXVECTOR2(4000.0f, 4000.0f), XCOL_WHITE, POSGRID2(80, 80));
	if (UNUSED(m_pField))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 地形を設定
	m_pField->SetTerrain(CField::TERRAIN_80x80);

	// 壁オブジェクトの生成
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3( 0.0f,    0.0f, -2000.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),   D3DXVECTOR2(4000.0f, 400.0f), XCOL_WHITE, POSGRID2(12, 1));
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3(-2000.0f, 0.0f,  0.0f),    D3DXToRadian(D3DXVECTOR3(0.0f, 90.0f, 0.0f)),  D3DXVECTOR2(4000.0f, 400.0f), XCOL_WHITE, POSGRID2(12, 1));
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3( 0.0f,    0.0f,  2000.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 180.0f, 0.0f)), D3DXVECTOR2(4000.0f, 400.0f), XCOL_WHITE, POSGRID2(12, 1));
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3( 2000.0f, 0.0f,  0.0f),    D3DXToRadian(D3DXVECTOR3(0.0f, 270.0f, 0.0f)), D3DXVECTOR2(4000.0f, 400.0f), XCOL_WHITE, POSGRID2(12, 1));

	// 景色オブジェクトの生成
	CScenery::Create(CScenery::TEXTURE_NORMAL, VEC3_ZERO, VEC3_ZERO,                                    XCOL_WHITE,                        POSGRID2(32, 1), 12000.0f, 1000.0f, D3DCULL_CW, false);
	CScenery::Create(CScenery::TEXTURE_NORMAL, VEC3_ZERO, D3DXToRadian(D3DXVECTOR3(0.0f, 85.0f, 0.0f)), D3DXCOLOR(0.7f, 1.0f, 1.0f, 1.0f), POSGRID2(32, 1), 14000.0f, 1600.0f, D3DCULL_CW, false);
	CScenery::Create(CScenery::TEXTURE_NORMAL, VEC3_ZERO, D3DXToRadian(D3DXVECTOR3(0.0f, 35.0f, 0.0f)), D3DXCOLOR(0.4f, 1.0f, 0.7f, 1.0f), POSGRID2(32, 1), 16000.0f, 2200.0f, D3DCULL_CW, false);

	// 空オブジェクトの生成
	CSky::Create(CSky::TEXTURE_NORMAL, VEC3_ZERO, VEC3_ZERO, XCOL_WHITE, POSGRID2(32, 6), 18000.0f, D3DCULL_CW, false);

	// プレイヤーオブジェクトの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, -100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	if (UNUSED(m_pPlayer))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ターゲットオブジェクトの生成
	m_pTarget = CTarget::Create(CTarget::MODEL_NORMAL, D3DXVECTOR3(0.0f, 400.0f, 0.0f), VEC3_ZERO);
	if (UNUSED(m_pTarget))
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

	// タイマーオブジェクトの生成
	m_pTimer = CTimer::Create();
	if (UNUSED(m_pTimer))
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
	CFlower::RandomSpawn(30, CFlower::TYPE_NORMAL, D3DXVECTOR3(25.0f, 50.0f, 0.0f), 10);	// TODO：定数変更

	// TODO：初期設定
#if 0
	// カメラの初期位置を設定
	CManager::GetCamera()->SetDestCamera();

	// タイムを計測開始
	m_pTimer->Start();

	// BGMの再生
	CManager::GetSound()->Play(CSound::LABEL_BGM_000);
#endif

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
	m_pTimer	= NULL;		// タイマーオブジェクト

	// 成功を返す
	return S_OK;
}

//============================================================
//	更新処理
//============================================================
void CSceneGame::Update(void)
{
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

	// TODO：遷移のタイミングで壊れる問題死亡フラグで回避
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

//============================================================
//	タイマー取得処理
//============================================================
CTimer *CSceneGame::GetTimer(void)
{
	// タイマーのポインタを返す
	return m_pTimer;
}
