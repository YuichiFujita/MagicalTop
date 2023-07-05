//============================================================
//
//	地面処理 [field.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "field.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CField::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\field000.jpg",	// 通常テクスチャ
};

//************************************************************
//	子クラス [CField] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CField::CField() : CObjectMeshField(CObject::LABEL_FIELD)
{

}

//============================================================
//	デストラクタ
//============================================================
CField::~CField()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CField::Init(void)
{
	// オブジェクトメッシュフィールドの初期化
	if (FAILED(CObjectMeshField::Init()))
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
void CField::Uninit(void)
{
	// オブジェクトメッシュフィールドの終了
	CObjectMeshField::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CField::Update(void)
{
	// オブジェクトメッシュフィールドの更新
	CObjectMeshField::Update();
}

//============================================================
//	描画処理
//============================================================
void CField::Draw(void)
{
	// オブジェクトメッシュフィールドの描画
	CObjectMeshField::Draw();
}

//============================================================
//	生成処理
//============================================================
CField *CField::Create
(
	const TEXTURE texture,		// 種類
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXVECTOR2& rSize,	// 大きさ
	const D3DXCOLOR& rCol,		// 色
	const POSGRID2& rPart,		// 分割数
	const D3DCULL cull,			// カリング状況
	const bool bLight			// ライティング状況
)
{
	// 変数を宣言
	int nTextureID;	// テクスチャインデックス

	// ポインタを宣言
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ
	CField *pField = NULL;	// 地面生成用

	if (UNUSED(pField))
	{ // 使用されていない場合

		// メモリ確保
		pField = new CField;	// 地面
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pField))
	{ // 確保に成功している場合

		if (SUCCEEDED(pField->GetResult()))
		{ // オブジェクトの情報設定に成功した場合

			// 地面の初期化
			if (FAILED(pField->Init()))
			{ // 初期化に失敗した場合

				// メモリ開放
				delete pField;
				pField = NULL;

				// 失敗を返す
				return NULL;
			}

			// テクスチャを登録
			nTextureID = pTexture->Regist(mc_apTextureFile[texture]);

			// テクスチャを割当
			pField->BindTexture(nTextureID);

			// 位置を設定
			pField->SetPosition(rPos);

			// 向きを設定
			pField->SetRotation(rRot);

			// 大きさを設定
			pField->SetScalingBiaxial(rSize);

			// 色を設定
			pField->SetColor(rCol);

			// カリングを設定
			pField->SetCulling(cull);

			// ライティングを設定
			pField->SetLighting(bLight);

			// 分割数を設定
			if (FAILED(pField->SetPattern(rPart)))
			{ // 分割数の設定に失敗した場合

				// メモリ開放
				delete pField;
				pField = NULL;

				// 失敗を返す
				return NULL;
			}

			// 確保したアドレスを返す
			return pField;
		}
		else
		{ // オブジェクトの情報設定に失敗した場合

			// メモリ開放
			delete pField;
			pField = NULL;

			// 失敗を返す
			return NULL;
		}
	}
	else { assert(false); return NULL; }	// 確保失敗
}
