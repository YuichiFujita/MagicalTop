//============================================================
//
//	景色処理 [scenery.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "scenery.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CScenery::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\scenery000.png",	// 通常テクスチャ
};

//************************************************************
//	子クラス [CScenery] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CScenery::CScenery() : CObjectMeshCylinder(CObject::LABEL_SCENERY)
{

}

//============================================================
//	デストラクタ
//============================================================
CScenery::~CScenery()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CScenery::Init(void)
{
	// オブジェクトメッシュシリンダーの初期化
	if (FAILED(CObjectMeshCylinder::Init()))
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
void CScenery::Uninit(void)
{
	// オブジェクトメッシュシリンダーの終了
	CObjectMeshCylinder::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CScenery::Update(void)
{
	// オブジェクトメッシュシリンダーの更新
	CObjectMeshCylinder::Update();
}

//============================================================
//	描画処理
//============================================================
void CScenery::Draw(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスのポインタ

	// αテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// αテストの有効 / 無効の設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 200);				// αテストの参照値設定

	// オブジェクトメッシュシリンダーの描画
	CObjectMeshCylinder::Draw();

	// αテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// αテストの有効 / 無効の設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// αテストの参照値設定
}

//============================================================
//	生成処理
//============================================================
CScenery *CScenery::Create
(
	const TEXTURE texture,		// 種類
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXCOLOR& rCol,		// 色
	const POSGRID2& rPart,		// 分割数
	const float fRadius,		// 半径
	const float fHeight,		// 縦幅
	const D3DCULL cull,			// カリング状況
	const bool bLight			// ライティング状況
)
{
	// 変数を宣言
	int nTextureID;	// テクスチャインデックス

	// ポインタを宣言
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ
	CScenery *pScenery = NULL;	// 景色生成用

	if (UNUSED(pScenery))
	{ // 使用されていない場合

		// メモリ確保
		pScenery = new CScenery;	// 景色
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pScenery))
	{ // 確保に成功している場合

		if (SUCCEEDED(pScenery->GetResult()))
		{ // オブジェクトの情報設定に成功した場合

			// 景色の初期化
			if (FAILED(pScenery->Init()))
			{ // 初期化に失敗した場合

				// メモリ開放
				delete pScenery;
				pScenery = NULL;

				// 失敗を返す
				return NULL;
			}

			// テクスチャを登録
			nTextureID = pTexture->Regist(mc_apTextureFile[texture]);

			// テクスチャを割当
			pScenery->BindTexture(nTextureID);

			// 位置を設定
			pScenery->SetPosition(rPos);

			// 向きを設定
			pScenery->SetRotation(rRot);

			// 色を設定
			pScenery->SetColor(rCol);

			// 半径を設定
			pScenery->SetRadius(fRadius);

			// 縦幅を設定
			pScenery->SetHeight(fHeight);

			// カリングを設定
			pScenery->SetCulling(cull);

			// ライティングを設定
			pScenery->SetLighting(bLight);

			// 分割数を設定
			if (FAILED(pScenery->SetPattern(rPart)))
			{ // 分割数の設定に失敗した場合

				// メモリ開放
				delete pScenery;
				pScenery = NULL;

				// 失敗を返す
				return NULL;
			}

			// 確保したアドレスを返す
			return pScenery;
		}
		else
		{ // オブジェクトの情報設定に失敗した場合

			// メモリ開放
			delete pScenery;
			pScenery = NULL;

			// 失敗を返す
			return NULL;
		}
	}
	else { assert(false); return NULL; }	// 確保失敗
}
