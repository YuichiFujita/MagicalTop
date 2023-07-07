//============================================================
//
//	弾処理 [bullet.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

#include "score.h"
#include "field.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CBullet::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\bullet000.png",	// 弾テクスチャ
};

//************************************************************
//	子クラス [CBullet] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CBullet::CBullet() : CObjectBillboard(CObject::LABEL_BULLET)
{
	// メンバ変数をクリア
	m_move  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
	m_type  = TYPE_ENEMY;	// 種類
	m_nLife = 0;	// 寿命
}

//============================================================
//	デストラクタ
//============================================================
CBullet::~CBullet()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CBullet::Init(void)
{
	// メンバ変数を初期化
	m_move  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
	m_type  = TYPE_ENEMY;	// 種類
	m_nLife = 0;	// 寿命

	// オブジェクトビルボードの初期化
	if (FAILED(CObjectBillboard::Init()))
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
void CBullet::Uninit(void)
{
	// オブジェクトビルボードの終了
	CObjectBillboard::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CBullet::Update(void)
{
	// 変数を宣言
	D3DXVECTOR3 pos = CObjectBillboard::GetPosition();	// 位置

	if (m_nLife > 0)
	{ // 寿命が残っている場合

		// カウンターを減算
		m_nLife--;
	}
	else
	{ // 寿命が来た場合

		// オブジェクトの終了
		Uninit();

		// 関数を抜ける
		return;
	}

	// 移動量を加算
	pos += m_move;

	if (pos.y <= CManager::GetField()->GetPositionHeight(pos))
	{ // 地面に当たっている場合

		// オブジェクトの終了
		Uninit();

		// 関数を抜ける
		return;
	}

	// 位置を設定
	CObjectBillboard::SetPosition(pos);

	// オブジェクトビルボードの更新
	CObjectBillboard::Update();
}

//============================================================
//	描画処理
//============================================================
void CBullet::Draw(void)
{
	// オブジェクトビルボードの描画
	CObjectBillboard::Draw();
}

//============================================================
//	移動量の設定処理
//============================================================
void CBullet::SetMove(D3DXVECTOR3 rot, const float fMove)
{
	// 向きの正規化
	useful::NormalizeRot(rot.y);

	// 移動量を求める
	m_move.x = (fMove * sinf(rot.x)) * sinf(rot.y);
	m_move.y = (fMove * cosf(rot.x));
	m_move.z = (fMove * sinf(rot.x)) * cosf(rot.y);
}

//============================================================
//	種類の設定処理
//============================================================
void CBullet::SetType(const TYPE type)
{
	// 引数の種類を代入
	m_type = type;
}

//============================================================
//	寿命の設定処理
//============================================================
void CBullet::SetLife(const int nLife)
{
	// 引数の寿命を代入
	m_nLife = nLife;
}

//============================================================
//	生成処理
//============================================================
CBullet *CBullet::Create(const TYPE type, const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const D3DXCOLOR& rCol, const D3DXVECTOR3& rRot, const float fMove, const int nLife)
{
	// 変数を宣言
	int nTextureID;	// テクスチャインデックス

	// ポインタを宣言
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ
	CBullet *pBullet = NULL;	// 弾生成用

	if (UNUSED(pBullet))
	{ // 使用されていない場合

		// メモリ確保
		pBullet = new CBullet;	// 弾
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pBullet))
	{ // 確保に成功している場合

		if (SUCCEEDED(pBullet->GetResult()))
		{ // オブジェクトの情報設定に成功した場合

			// 弾の初期化
			if (FAILED(pBullet->Init()))
			{ // 初期化に失敗した場合

				// メモリ開放
				delete pBullet;
				pBullet = NULL;

				// 失敗を返す
				return NULL;
			}

			// テクスチャを登録
			nTextureID = pTexture->Regist(mc_apTextureFile[type]);

			// テクスチャを割当
			pBullet->BindTexture(nTextureID);

			// 位置を設定
			pBullet->SetPosition(rPos);

			// 大きさを設定
			pBullet->SetScaling(rSize);

			// 色を設定
			pBullet->SetColor(rCol);

			// 種類を設定
			pBullet->SetType(type);

			// 移動量を設定
			pBullet->SetMove(rRot, fMove);

			// 寿命の設定
			pBullet->SetLife(nLife);

			// 確保したアドレスを返す
			return pBullet;
		}
		else
		{ // オブジェクトの情報設定に失敗した場合

			// メモリ開放
			delete pBullet;
			pBullet = NULL;

			// 失敗を返す
			return NULL;
		}
	}
	else { assert(false); return NULL; }	// 確保失敗
}
