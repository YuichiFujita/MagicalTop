//============================================================
//
//	リザルト画面処理 [sceneResult.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "sceneResult.h"
#include "manager.h"
#include "texture.h"
#include "input.h"
#include "camera.h"
#include "object2D.h"

#include "stage.h"
#include "target.h"
#include "field.h"
#include "sea.h"
#include "wall.h"
#include "scenery.h"
#include "sky.h"
#include "flower.h"
#include "weed.h"

//************************************************************
//	マクロ定義
//************************************************************
#define CREATE_FLOWER	(30)	// マナフラワーの生成数
#define CREATE_WEED		(200)	// 草の生成数

//************************************************************
//	子クラス [CSceneResult] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSceneResult::CSceneResult(const MODE mode) : CScene(mode)
{
	// メンバ変数をクリア
	m_pObject2D = NULL;	// タイトル表示用
}

//============================================================
//	デストラクタ
//============================================================
CSceneResult::~CSceneResult()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSceneResult::Init(void)
{
	// 変数を宣言
	CWaveManager::SEASON season = (CWaveManager::SEASON)(rand() % CWaveManager::SEASON_MAX);	// 季節

	// ポインタを宣言
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ
	CTarget *pTarget = NULL;	// ターゲット設定用

	// メンバ変数を初期化
	m_pObject2D = NULL;	// タイトル表示用

	// オブジェクト2Dの生成
	m_pObject2D = CObject2D::Create
	( // 引数
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f),	// 位置
		//D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f)					// 大きさ
		D3DXVECTOR3(100.0f, 100.0f, 0.0f)
	);
	if (UNUSED(m_pObject2D))
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを登録・割当
	m_pObject2D->BindTexture(pTexture->Regist("data\\TEXTURE\\result000.png"));

	//--------------------------------------------------------
	//	オブジェクト生成・初期化
	//--------------------------------------------------------
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
	pTarget = CTarget::Create(CTarget::MODEL_NORMAL, D3DXVECTOR3(0.0f, 400.0f, 0.0f), VEC3_ZERO);
	if (UNUSED(pTarget))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	初期設定
	//--------------------------------------------------------
	// ステージエリア・バリアの描画を停止
	GetStage()->SetEnableDrawArea(false);
	GetStage()->SetEnableDrawBarrier(false);

	// ターゲット体力の描画を停止
	pTarget->SetEnableDrawLife(false);

	// マナフラワーセットアップの読込
	CFlower::LoadSetup();

	// 草セットアップの読込
	CWeed::LoadSetup();

	// マナフラワーランダム生成
	CFlower::RandomSpawn(CREATE_FLOWER, CFlower::TYPE_SPRING);

	// 草ランダム生成
	CWeed::RandomSpawn(CREATE_WEED, CWeed::TYPE_SPRING);

	// 季節を設定
	GetField()->SetSeason(season);	// 地面を変更
	CFlower::SetSeason(season);		// 花を変更
	CWeed::SetSeason(season);		// 草を変更

	// カメラを設定
	CManager::GetCamera()->SetState(CCamera::STATE_ROTATE);	// カメラを回転状態に設定
	CManager::GetCamera()->SetDestRotate();	// 目標位置を設定

	// BGMの再生
	//CManager::GetSound()->Play(CSound::LABEL_BGM_000);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CSceneResult::Uninit(void)
{
	// オブジェクト2Dの終了
	m_pObject2D->Uninit();

	// シーンの終了
	CScene::Uninit();

	// 成功を返す
	return S_OK;
}

//============================================================
//	更新処理
//============================================================
void CSceneResult::Update(void)
{
	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN)
	||  CManager::GetKeyboard()->GetTrigger(DIK_SPACE)
	||  CManager::GetPad()->GetTrigger(CInputPad::KEY_A)
	||  CManager::GetPad()->GetTrigger(CInputPad::KEY_B)
	||  CManager::GetPad()->GetTrigger(CInputPad::KEY_X)
	||  CManager::GetPad()->GetTrigger(CInputPad::KEY_Y)
	||  CManager::GetPad()->GetTrigger(CInputPad::KEY_START))
	{
		// シーンの設定
		CManager::SetScene(MODE_TITLE);	// タイトル画面
	}

	// シーンの更新
	CScene::Update();
}

//============================================================
//	描画処理
//============================================================
void CSceneResult::Draw(void)
{

}
