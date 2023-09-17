//============================================================
//
//	チュートリアル画面処理 [sceneTutorial.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "sceneTutorial.h"
#include "manager.h"
#include "sound.h"
#include "camera.h"

#include "stage.h"
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
#define SCO_POS		(D3DXVECTOR3(825.0f, 50.0f, 0.0f))		// スコア位置
#define SCO_SIZE	(D3DXVECTOR3(60.0f, 80.0f, 0.0f))		// スコア大きさ
#define SCO_SPACE	(D3DXVECTOR3(SCO_SIZE.x, 0.0f, 0.0f))	// スコア空白

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CScore *CSceneTutorial::m_pScore = NULL;	// スコアオブジェクト

//************************************************************
//	子クラス [CSceneTutorial] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSceneTutorial::CSceneTutorial(const MODE mode) : CScene(mode)
{

}

//============================================================
//	デストラクタ
//============================================================
CSceneTutorial::~CSceneTutorial()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSceneTutorial::Init(void)
{
	//--------------------------------------------------------
	//	チュートリアルの初期化
	//--------------------------------------------------------
	//// リザルトマネージャーの生成
	//m_pResultManager = CResultManager::Create();
	//if (UNUSED(m_pResultManager))
	//{ // 非使用中の場合

	//	// 失敗を返す
	//	assert(false);
	//	return E_FAIL;
	//}

	// シーンの初期化
	CScene::Init();

	//--------------------------------------------------------
	//	オブジェクト生成
	//--------------------------------------------------------
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

	// BGMの再生
	//CManager::GetSound()->Play(CSound::LABEL_BGM_000);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CSceneTutorial::Uninit(void)
{
	// 終了済みのオブジェクトポインタをNULLにする
	m_pScore = NULL;	// スコアオブジェクト

	// シーンの終了
	CScene::Uninit();

	// 成功を返す
	return S_OK;
}

//============================================================
//	更新処理
//============================================================
void CSceneTutorial::Update(void)
{
	// シーンの更新
	CScene::Update();
}

//============================================================
//	描画処理
//============================================================
void CSceneTutorial::Draw(void)
{

}

//============================================================
//	スコア取得処理
//============================================================
CScore *CSceneTutorial::GetScore(void)
{
	// スコアのポインタを返す
	return m_pScore;
}
