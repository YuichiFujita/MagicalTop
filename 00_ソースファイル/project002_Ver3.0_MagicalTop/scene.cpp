//============================================================
//
//	シーン処理 [scene.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "scene.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"
#include "camera.h"
#include "sceneTitle.h"
#include "sceneGame.h"
#include "sceneResult.h"

//************************************************************
//	親クラス [CScene] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CScene::CScene()
{

}

//============================================================
//	デストラクタ
//============================================================
CScene::~CScene()
{

}

//============================================================
//	更新処理
//============================================================
void CScene::Update(void)
{
	if (USED(CManager::GetLight()))
	{ // 使用中の場合

		// ライトの更新
		CManager::GetLight()->Update();
	}
	else { assert(false); }	// 非使用中

	if (USED(CManager::GetCamera()))
	{ // 使用中の場合

		// カメラの更新
		CManager::GetCamera()->Update();
	}
	else { assert(false); }	// 非使用中

	if (USED(CManager::GetRenderer()))
	{ // 使用中の場合

		// レンダラーの更新
		CManager::GetRenderer()->Update();
	}
	else { assert(false); }	// 非使用中
}

//============================================================
//	生成処理
//============================================================
CScene *CScene::Create(MODE mode)
{
	// ポインタを宣言
	CScene *pScene = NULL;	// シーン生成用

	if (UNUSED(pScene))
	{ // 使用されていない場合

		// メモリ確保
		switch (mode)
		{ // モードごとの処理
		case MODE_TITLE:

			// タイトル画面を生成
			pScene = new CSceneTitle;

			break;

		case MODE_GAME:

			// ゲーム画面を生成
			pScene = new CSceneGame;

			break;

		case MODE_RESULT:

			// リザルト画面を生成
			pScene = new CSceneResult;

			break;

		default:	// 例外処理
			assert(false);
			break;
		}
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pScene))
	{ // 確保に成功している場合

		// シーンの初期化
		if (FAILED(pScene->Init()))
		{ // 初期化に失敗した場合

			// シーンの終了
			if (FAILED(pScene->Uninit()))
			{ // シーンの終了に失敗した場合

				// メモリ開放
				delete pScene;
				pScene = NULL;

				// 失敗を返す
				assert(false);
				return NULL;
			}

			// メモリ開放
			delete pScene;
			pScene = NULL;

			// 失敗を返す
			assert(false);
			return NULL;
		}

		// モードの設定
		pScene->SetMode(mode);

		// 確保したアドレスを返す
		return pScene;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CScene::Release(CScene *&prScene)
{
	if (USED(prScene))
	{ // 使用中の場合

		// シーンの終了
		if (FAILED(prScene->Uninit()))
		{ // シーンの終了に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// メモリ開放
		delete prScene;
		prScene = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	モードの設定処理
//============================================================
void CScene::SetMode(const MODE mode)
{
	// 引数のモードを設定
	m_mode = mode;
}

//============================================================
//	モード取得処理
//============================================================
CScene::MODE CScene::GetMode(void) const
{
	// 現在のモードを返す
	return m_mode;
}
