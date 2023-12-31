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
#include "input.h"
#include "texture.h"
#include "object2D.h"

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
	// ポインタを宣言
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ

	// メンバ変数を初期化
	m_pObject2D = NULL;	// タイトル表示用

	// オブジェクト2Dの生成
	m_pObject2D = CObject2D::Create
	( // 引数
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f),	// 位置
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f)					// 大きさ
	);
	if (UNUSED(m_pObject2D))
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを登録・割当
	m_pObject2D->BindTexture(pTexture->Regist("data\\TEXTURE\\title000.png"));

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CSceneTitle::Uninit(void)
{
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
	||  CManager::GetPad()->GetTrigger(CInputPad::KEY_A)
	||  CManager::GetPad()->GetTrigger(CInputPad::KEY_B)
	||  CManager::GetPad()->GetTrigger(CInputPad::KEY_X)
	||  CManager::GetPad()->GetTrigger(CInputPad::KEY_Y))
	{
		// シーンの設定
		CManager::SetScene(MODE_GAME);	// ゲーム画面
	}

	// シーンの更新
	CScene::Update();
}

//============================================================
//	描画処理
//============================================================
void CSceneTitle::Draw(void)
{

}
