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
#include "input.h"
#include "texture.h"
#include "object2D.h"

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
	m_pObject2D->BindTexture(pTexture->Regist("data\\TEXTURE\\result000.png"));

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
