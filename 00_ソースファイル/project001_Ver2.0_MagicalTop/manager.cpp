//============================================================
//
//	マネージャー処理 [manager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "model.h"
#include "value.h"
#include "debugproc.h"

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

#ifdef _DEBUG	// デバッグ処理

#include "debug.h"

#endif	// _DEBUG

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CRenderer		*CManager::m_pRenderer	= NULL;		// レンダラーオブジェクト
CInputKeyboard	*CManager::m_pKeyboard	= NULL;		// キーボードオブジェクト
CInputMouse		*CManager::m_pMouse		= NULL;		// マウスオブジェクト
CInputPad		*CManager::m_pPad		= NULL;		// パッドオブジェクト
CSound			*CManager::m_pSound		= NULL;		// サウンドオブジェクト
CCamera			*CManager::m_pCamera	= NULL;		// カメラオブジェクト
CLight			*CManager::m_pLight		= NULL;		// ライトオブジェクト
CTexture		*CManager::m_pTexture	= NULL;		// テクスチャオブジェクト
CModel			*CManager::m_pModel		= NULL;		// モデルオブジェクト
CStage			*CManager::m_pStage		= NULL;		// ステージオブジェクト
CPlayer			*CManager::m_pPlayer	= NULL;		// プレイヤーオブジェクト
CField			*CManager::m_pField		= NULL;		// 地面オブジェクト
CTarget			*CManager::m_pTarget	= NULL;		// ターゲットオブジェクト
CScore			*CManager::m_pScore		= NULL;		// スコアオブジェクト
CTimer			*CManager::m_pTimer		= NULL;		// タイマーオブジェクト
CDebugProc		*CManager::m_pDebugProc = NULL;		// デバッグ表示オブジェクト

#ifdef _DEBUG	// デバッグ処理

CDebug *CManager::m_pDebug = NULL;	// デバッグオブジェクト

#endif	// _DEBUG

//************************************************************
//	親クラス [CManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CManager::CManager()
{

}

//============================================================
//	デストラクタ
//============================================================
CManager::~CManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//--------------------------------------------------------
	//	システムの生成
	//--------------------------------------------------------
	// レンダラーの生成
	m_pRenderer = CRenderer::Create(hWnd);
	if (UNUSED(m_pRenderer))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// キーボードの生成
	m_pKeyboard = CInputKeyboard::Create(hInstance, hWnd);
	if (UNUSED(m_pKeyboard))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// マウスの生成
	m_pMouse = CInputMouse::Create(hInstance, hWnd);
	if (UNUSED(m_pMouse))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// パッドの生成
	m_pPad = CInputPad::Create();
	if (UNUSED(m_pPad))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// サウンドの生成
	m_pSound = CSound::Create(hWnd);
	if (UNUSED(m_pSound))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// カメラの生成
	m_pCamera = CCamera::Create();
	if (UNUSED(m_pCamera))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ライトの生成
	m_pLight = CLight::Create();
	if (UNUSED(m_pLight))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	情報の読込
	//--------------------------------------------------------
	// テクスチャの生成・読込
	m_pTexture = CTexture::Create();
	if (UNUSED(m_pTexture))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// モデルの生成・読込
	m_pModel = CModel::Create();
	if (UNUSED(m_pModel))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	ゲームオブジェクトの生成
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
	m_pField = CField::Create(CField::TEXTURE_NORMAL, D3DXVECTOR3(0.0f, 400.0f, 0.0f), VEC3_ZERO, D3DXVECTOR2(4000.0f, 4000.0f), XCOL_WHITE, POSGRID2(12, 12));
	if (UNUSED(m_pField))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

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
	// 敵セットアップの読み込み
	CEnemy::LoadSetup();

	// 魔法セットアップの読み込み
	CMagic::LoadSetup();

#if 0
	CEnemy::Create(CEnemy::TYPE_CAR, D3DXVECTOR3(2000.0f, 400.0f, 0.0f), VEC3_ZERO);
	CEnemy::Create(CEnemy::TYPE_CAR, D3DXVECTOR3(-2000.0f, 400.0f, 0.0f), VEC3_ZERO);
	CEnemy::Create(CEnemy::TYPE_CAR, D3DXVECTOR3(0.0f, 400.0f, -2000.0f), VEC3_ZERO);
	CEnemy::Create(CEnemy::TYPE_CAR, D3DXVECTOR3(0.0f, 400.0f, 2000.0f), VEC3_ZERO);
#endif

	// TODO：初期設定
#if 0
	// カメラの初期位置を設定
	m_pCamera->SetDestCamera();

	// タイムを計測開始
	m_pTimer->Start();

	// BGMの再生
	m_pSound->Play(CSound::LABEL_BGM_000);
#endif

	//--------------------------------------------------------
	//	デバッグ用
	//--------------------------------------------------------
	// デバッグ表示の生成
	m_pDebugProc = CDebugProc::Create(hWnd);
	if (UNUSED(m_pDebugProc))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

#ifdef _DEBUG	// デバッグ処理

	// デバッグの生成
	m_pDebug = CDebug::Create();
	if (UNUSED(m_pDebug))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

#endif	// _DEBUG

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CManager::Uninit(void)
{
	//--------------------------------------------------------
	//	デバッグ用
	//--------------------------------------------------------
	// デバッグ表示の破棄
	if (FAILED(CDebugProc::Release(m_pDebugProc)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

#ifdef _DEBUG	// デバッグ処理

	// デバッグの破棄
	if (FAILED(CDebug::Release(m_pDebug)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

#endif	// _DEBUG

	//--------------------------------------------------------
	//	サウンドの停止
	//--------------------------------------------------------
	// BGMの停止
	m_pSound->Stop();

	//--------------------------------------------------------
	//	ゲームオブジェクトの破棄
	//--------------------------------------------------------
	// オブジェクトの全破棄
	CObject::ReleaseAll();

	// ステージの破棄
	if (FAILED(CStage::Release(m_pStage)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 終了済みのオブジェクトポインタをNULLにする
	m_pPlayer = NULL;		// プレイヤーオブジェクト
	m_pTarget = NULL;		// ターゲットオブジェクト
	m_pField = NULL;		// 地面オブジェクト
	m_pScore = NULL;		// スコアオブジェクト
	m_pTimer = NULL;		// タイマーオブジェクト

	//--------------------------------------------------------
	//	情報の破棄
	//--------------------------------------------------------
	// テクスチャの破棄
	if (FAILED(CTexture::Release(m_pTexture)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// モデルの破棄
	if (FAILED(CModel::Release(m_pModel)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	システムの破棄
	//--------------------------------------------------------
	// ライトの破棄
	if (FAILED(CLight::Release(m_pLight)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// カメラの破棄
	if (FAILED(CCamera::Release(m_pCamera)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// サウンドの破棄
	if (FAILED(CSound::Release(m_pSound)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// パッドの破棄
	if (FAILED(CInputPad::Release(m_pPad)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// マウスの破棄
	if (FAILED(CInputMouse::Release(m_pMouse)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// キーボードの破棄
	if (FAILED(CInputKeyboard::Release(m_pKeyboard)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// レンダラーの破棄
	if (FAILED(CRenderer::Release(m_pRenderer)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	更新処理
//============================================================
void CManager::Update(void)
{
#ifdef _DEBUG	// デバッグ処理

	// デバッグ表示
	m_pDebugProc->Print("FPS：%d\n", m_pDebug->GetFps());

#endif	// _DEBUG

	if (m_pKeyboard->GetTrigger(DIK_1))
	{
		CEnemy::Create(CEnemy::TYPE_CAR, D3DXVECTOR3(2000.0f, 400.0f, 0.0f), VEC3_ZERO);
		CEnemy::Create(CEnemy::TYPE_CAR, D3DXVECTOR3(-2000.0f, 400.0f, 0.0f), VEC3_ZERO);
		CEnemy::Create(CEnemy::TYPE_CAR, D3DXVECTOR3(0.0f, 400.0f, -2000.0f), VEC3_ZERO);
		CEnemy::Create(CEnemy::TYPE_CAR, D3DXVECTOR3(0.0f, 400.0f, 2000.0f), VEC3_ZERO);
	}

	if (USED(m_pPad))
	{ // 使用中の場合

		// パッドの更新
		m_pPad->Update();
	}
	else { assert(false); }	// 非使用中

	if (USED(m_pMouse))
	{ // 使用中の場合

		// マウスの更新
		m_pMouse->Update();
	}
	else { assert(false); }	// 非使用中

	if (USED(m_pKeyboard))
	{ // 使用中の場合

		// キーボードの更新
		m_pKeyboard->Update();
	}
	else { assert(false); }	// 非使用中

	if (USED(m_pRenderer))
	{ // 使用中の場合

		// レンダラーの更新
		m_pRenderer->Update();
	}
	else { assert(false); }	// 非使用中

	if (USED(m_pLight))
	{ // 使用中の場合

		// ライトの更新
		m_pLight->Update();
	}
	else { assert(false); }	// 非使用中

	if (USED(m_pCamera))
	{ // 使用中の場合

		// カメラの更新
		m_pCamera->Update();
	}
	else { assert(false); }	// 非使用中
}

//============================================================
//	描画処理
//============================================================
void CManager::Draw(void)
{
	if (USED(m_pRenderer))
	{ // 使用中の場合

		// レンダラーの描画
		m_pRenderer->Draw();
	}
	else { assert(false); }	// 非使用中
}

//============================================================
//	生成処理
//============================================================
CManager *CManager::Create(HINSTANCE hInstance, HWND hWnd)
{
	// ポインタを宣言
	CManager *pManager = NULL;		// マネージャー生成用

	if (UNUSED(pManager))
	{ // 使用されていない場合

		// メモリを確保
		pManager = new CManager;	// マネージャー
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pManager))
	{ // 確保に成功している場合

		// マネージャーの初期化
		if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))
		{ // 初期化に失敗した場合

			// マネージャーの終了
			if (FAILED(pManager->Uninit()))
			{ // マネージャーの終了に失敗した場合

				// メモリ開放
				delete pManager;
				pManager = NULL;

				// 失敗を返す
				assert(false);
				return NULL;
			}

			// メモリ開放
			delete pManager;
			pManager = NULL;

			// 失敗を返す
			assert(false);
			return NULL;
		}

		// 確保したアドレスを返す
		return pManager;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CManager::Release(CManager *&prManager)
{
	if (USED(prManager))
	{ // 使用中の場合

		// マネージャーの終了
		if (FAILED(prManager->Uninit()))
		{ // マネージャーの終了に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// メモリ開放
		delete prManager;
		prManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	レンダラー取得処理
//============================================================
CRenderer *CManager::GetRenderer(void)
{
	// レンダラーのポインタを返す
	return m_pRenderer;
}

//============================================================
//	キーボード取得処理
//============================================================
CInputKeyboard *CManager::GetKeyboard(void)
{
	// キーボードのポインタを返す
	return m_pKeyboard;
}

//============================================================
//	マウス取得処理
//============================================================
CInputMouse *CManager::GetMouse(void)
{
	// マウスのポインタを返す
	return m_pMouse;
}

//============================================================
//	パッド取得処理
//============================================================
CInputPad *CManager::GetPad(void)
{
	// パッドのポインタを返す
	return m_pPad;
}

//============================================================
//	サウンド取得処理
//============================================================
CSound *CManager::GetSound(void)
{
	// サウンドのポインタを返す
	return m_pSound;
}

//============================================================
//	カメラ取得処理
//============================================================
CCamera *CManager::GetCamera(void)
{
	// カメラのポインタを返す
	return m_pCamera;
}

//============================================================
//	ライト取得処理
//============================================================
CLight *CManager::GetLight(void)
{
	// ライトのポインタを返す
	return m_pLight;
}

//============================================================
//	テクスチャ取得処理
//============================================================
CTexture *CManager::GetTexture(void)
{
	// テクスチャのポインタを返す
	return m_pTexture;
}

//============================================================
//	モデル取得処理
//============================================================
CModel *CManager::GetModel(void)
{
	// モデルのポインタを返す
	return m_pModel;
}

//============================================================
//	ステージ取得処理
//============================================================
CStage *CManager::GetStage(void)
{
	// ステージのポインタを返す
	return m_pStage;
}

//============================================================
//	プレイヤー取得処理
//============================================================
CPlayer *CManager::GetPlayer(void)
{
	// プレイヤーのポインタを返す
	return m_pPlayer;
}

//============================================================
//	ターゲット取得処理
//============================================================
CTarget *CManager::GetTarget(void)
{
	// ターゲットのポインタを返す
	return m_pTarget;
}

//============================================================
//	地面取得処理
//============================================================
CField *CManager::GetField(void)
{
	// 地面のポインタを返す
	return m_pField;
}

//============================================================
//	スコア取得処理
//============================================================
CScore *CManager::GetScore(void)
{
	// スコアのポインタを返す
	return m_pScore;
}

//============================================================
//	タイマー取得処理
//============================================================
CTimer *CManager::GetTimer(void)
{
	// タイマーのポインタを返す
	return m_pTimer;
}

//============================================================
//	デバッグ表示取得処理
//============================================================
CDebugProc *CManager::GetDebugProc(void)
{
	// デバッグ表示のポインタを返す
	return m_pDebugProc;
}

//============================================================
//	デバッグ取得処理
//============================================================
#ifdef _DEBUG	// デバッグ処理

CDebug *CManager::GetDebug(void)
{
	// デバッグのポインタを返す
	return m_pDebug;
}

#endif	// _DEBUG
