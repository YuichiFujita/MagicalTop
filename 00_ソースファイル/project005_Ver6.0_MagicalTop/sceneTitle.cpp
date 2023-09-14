//============================================================
//
//	タイトル画面処理 [sceneTitle.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "sceneTitle.h"
#include "manager.h"
#include "texture.h"
#include "input.h"
#include "camera.h"
#include "object2D.h"

#include "stage.h"
#include "target.h"
#include "player.h"

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
CStage	*CSceneTitle::m_pStage = NULL;	// ステージ
CField	*CSceneTitle::m_pField = NULL;	// 地面オブジェクト

//************************************************************
//	子クラス [CSceneTitle] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSceneTitle::CSceneTitle()
{
	// メンバ変数をクリア
	m_pObject2D = NULL;	// タイトル表示用
}

//============================================================
//	デストラクタ
//============================================================
CSceneTitle::~CSceneTitle()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSceneTitle::Init(void)
{
	// 変数を宣言
	CWaveManager::SEASON season = (CWaveManager::SEASON)(rand() % CWaveManager::SEASON_MAX + 1);	// 季節

	// ポインタを宣言
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ

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
	m_pObject2D->BindTexture(pTexture->Regist("data\\TEXTURE\\title000.png"));

	//--------------------------------------------------------
	//	オブジェクト生成
	//--------------------------------------------------------
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

#if 0
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
#endif

	//--------------------------------------------------------
	//	初期設定
	//--------------------------------------------------------
#if 0
	// マナフラワーセットアップの読込
	CFlower::LoadSetup();

	// 草セットアップの読込
	CWeed::LoadSetup();

	// マナフラワーランダム生成
	CFlower::RandomSpawn(30, CFlower::TYPE_SPRING);	// TODO：定数変更

	// 草ランダム生成
	CWeed::RandomSpawn(200, CWeed::TYPE_SPRING);	// TODO：定数変更
#endif

	// 季節を設定
	m_pField->SetSeason(season);	// 地面を変更
	CFlower::SetSeason(season);		// 花を変更
	CWeed::SetSeason(season);		// 草を変更

	// カメラを設定
	CManager::GetCamera()->SetState(CCamera::STATE_ROTATE);	// カメラを回転状態に設定
	CManager::GetCamera()->SetDestRotate();	// 目標位置を設定

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CSceneTitle::Uninit(void)
{
	// ステージの破棄
	if (FAILED(CStage::Release(m_pStage)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// オブジェクト2Dの終了
	m_pObject2D->Uninit();

	// 成功を返す
	return S_OK;
}

//============================================================
//	更新処理
//============================================================
void CSceneTitle::Update(void)
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
		CManager::SetScene(MODE_GAME);	// ゲーム画面
	}

#if 0
	if (USED(m_pStage))
	{ // 使用中の場合

		// ステージの更新
		m_pStage->Update();
	}
	else { assert(false); }	// 非使用中
#endif

	// シーンの更新
	CScene::Update();
}

//============================================================
//	描画処理
//============================================================
void CSceneTitle::Draw(void)
{

}

//============================================================
//	ステージ取得処理
//============================================================
CStage *CSceneTitle::GetStage(void)
{
	// ステージのポインタを返す
	return m_pStage;
}

//============================================================
//	地面取得処理
//============================================================
CField *CSceneTitle::GetField(void)
{
	// 地面のポインタを返す
	return m_pField;
}
