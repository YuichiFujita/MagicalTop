//============================================================
//
//	シーズン処理 [season.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "season.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "model.h"

//************************************************************
//	マクロ定義
//************************************************************
#define SEASON_PRIO	(5)	// シーズン表示の優先順位

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CSeason::mc_apModelFile[] =	// モデル定数
{
	"data\\MODEL\\FONT\\shot000.x",	// モデル
};

//************************************************************
//	子クラス [CSeason] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSeason::CSeason() : CObjectModel(CObject::LABEL_NONE, SEASON_PRIO)
{

}

//============================================================
//	デストラクタ
//============================================================
CSeason::~CSeason()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSeason::Init(void)
{
	// オブジェクトモデルの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CSeason::Uninit(void)
{
	// オブジェクトモデルの終了
	CObjectModel::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CSeason::Update(void)
{
	// オブジェクトモデルの更新
	CObjectModel::Update();
}

//============================================================
//	描画処理
//============================================================
void CSeason::Draw(void)
{
	// 変数を宣言
	D3DVIEWPORT9 viewportDef;	// カメラのビューポート保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスのポインタ

	// 現在のビューポートを取得
	pDevice->GetViewport(&viewportDef);

	// カメラの設定
	CManager::GetCamera()->SetCamera(CCamera::TYPE_FONT);

	// オブジェクトモデルの描画
	CObjectModel::Draw();

	// カメラの設定を元に戻す
	CManager::GetCamera()->SetCamera(CCamera::TYPE_MAIN);

	// ビューポートを元に戻す
	pDevice->SetViewport(&viewportDef);
}

//============================================================
//	生成処理
//============================================================
CSeason *CSeason::Create(const MODEL model)
{
	// 変数を宣言
	int nModelID;	// モデルインデックス

	// ポインタを宣言
	CModel *pModel = CManager::GetModel();	// モデルへのポインタ
	CSeason *pSeason = NULL;	// シーズン生成用

	if (UNUSED(pSeason))
	{ // 使用されていない場合

		// メモリ確保
		pSeason = new CSeason;	// シーズン
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pSeason))
	{ // 使用されている場合
		
		// シーズンの初期化
		if (FAILED(pSeason->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pSeason;
			pSeason = NULL;

			// 失敗を返す
			return NULL;
		}

		// モデルを登録
		nModelID = pModel->Regist(mc_apModelFile[model]);

		// モデルを割当
		pSeason->BindModel(pModel->GetModel(nModelID));

		// 位置を設定
		pSeason->SetPosition(VEC3_ZERO);

		// 向きを設定
		pSeason->SetRotation(VEC3_ZERO);

		// 大きさを設定
		pSeason->SetScaling(VEC3_ONE);

		// 確保したアドレスを返す
		return pSeason;
	}
	else { assert(false); return NULL; }	// 確保失敗
}
