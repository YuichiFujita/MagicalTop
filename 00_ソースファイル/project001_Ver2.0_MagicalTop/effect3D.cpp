//============================================================
//
//	エフェクト3D処理 [effect3D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "effect3D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	マクロ定義
//************************************************************
#define EFF_PRIO	(4)	// エフェクト3Dの優先順位
#define PART_PRIO	(4)	// パーティクル3Dの優先順位

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CEffect3D::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\effect000.jpg",	// 通常テクスチャ
};

//************************************************************
//	子クラス [CEffect3D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEffect3D::CEffect3D() : CObjectBillboard(CObject::LABEL_EFFECT3D), m_type(TYPE_NORMAL)
{
	// メンバ変数をクリア
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
	m_nLife		= 0;	// 寿命
	m_fSubSize	= 0.0f;	// 大きさの減算量
	m_fSubAlpha	= 0.0f;	// 透明度の減算量
}

//============================================================
//	オーバーロードコンストラクタ
//============================================================
CEffect3D::CEffect3D(const TYPE type, const LABEL label) : CObjectBillboard(CObject::LABEL_EFFECT3D, (label == LABEL_EFFECT3D) ? EFF_PRIO : PART_PRIO), m_type(type)
{
	// メンバ変数をクリア
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
	m_nLife		= 0;	// 寿命
	m_fSubSize	= 0.0f;	// 大きさの減算量
	m_fSubAlpha	= 0.0f;	// 透明度の減算量
}

//============================================================
//	デストラクタ
//============================================================
CEffect3D::~CEffect3D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEffect3D::Init(void)
{
	// メンバ変数を初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
	m_nLife		= 0;	// 寿命
	m_fSubSize	= 0.0f;	// 大きさの減算量
	m_fSubAlpha	= 0.0f;	// 透明度の減算量

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
void CEffect3D::Uninit(void)
{
	// オブジェクトビルボードの終了
	CObjectBillboard::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEffect3D::Update(void)
{
	// 変数を宣言
	D3DXVECTOR3 pos  = GetPosition();	// 位置
	D3DXVECTOR3 rot  = GetRotation();	// 向き
	D3DXVECTOR3 size = GetScaling();	// 大きさ
	D3DXCOLOR   col  = GetColor();		// 色

	if (m_nLife <= 0		// 寿命を迎えた
	||  size.x  <= 0.0f		// サイズが0.0f以下
	||  size.y  <= 0.0f		// サイズが0.0f以下
	||  col.a   <= 0.0f)	// 完全に透明
	{ // 上記のどれかになった場合

		// オブジェクトを破棄
		Uninit();

		// 関数を抜ける
		return;
	}

	// 移動量を加算
	pos += m_move;

	// 寿命を減算
	m_nLife--;

	// 半径を減算
	size.x -= m_fSubSize;
	size.y -= m_fSubSize;

	// α値を減算
	col.a -= m_fSubAlpha;

	// 位置を設定
	CObjectBillboard::SetPosition(pos);

	// 向きを設定
	CObjectBillboard::SetRotation(rot);

	// 大きさを設定
	CObjectBillboard::SetScaling(size);

	// 色を設定
	CObjectBillboard::SetColor(col);

	// オブジェクトビルボードの更新
	CObjectBillboard::Update();
}

//============================================================
//	描画処理
//============================================================
void CEffect3D::Draw(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスのポインタ

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);	// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Zバッファ更新の有効 / 無効の設定

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// オブジェクトビルボードの描画
	CObjectBillboard::Draw();

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		// Zバッファ更新の有効 / 無効の設定

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//============================================================
//	生成処理
//============================================================
CEffect3D *CEffect3D::Create
(
	const TYPE type,			// テクスチャ
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rMove,	// 移動量
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXCOLOR& rCol,		// 色
	const int nLife,			// 寿命
	const float fRadius,		// 半径
	const float fSubSize,		// 半径の減算量
	const float fSubAlpha,		// 透明度の減算量
	const LABEL label			// オブジェクトラベル
)
{
	// 変数を宣言
	int nTextureID;	// テクスチャインデックス

	// ポインタを宣言
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ
	CEffect3D *pEffect3D = NULL;	// エフェクト3D生成用

	if (UNUSED(pEffect3D))
	{ // 使用されていない場合

		// メモリ確保
		pEffect3D = new CEffect3D(type, label);	// エフェクト3D
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pEffect3D))
	{ // 確保に成功している場合

		// エフェクト3Dの初期化
		if (FAILED(pEffect3D->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pEffect3D;
			pEffect3D = NULL;

			// 失敗を返す
			return NULL;
		}

		// テクスチャを登録
		nTextureID = pTexture->Regist(mc_apTextureFile[type]);

		// テクスチャを割当
		pEffect3D->BindTexture(nTextureID);

		// 位置を設定
		pEffect3D->SetPosition(rPos);

		// 向きを設定
		pEffect3D->SetRotation(rRot);

		// 大きさを設定
		pEffect3D->SetScaling(D3DXVECTOR3(fRadius, fRadius, 0.0f));

		// 色を設定
		pEffect3D->SetColor(rCol);

		// 引数の情報を設定
		pEffect3D->m_move		= rMove;		// 移動量
		pEffect3D->m_nLife		= nLife;		// 寿命
		pEffect3D->m_fSubSize	= fSubSize;		// 大きさの減算量
		pEffect3D->m_fSubAlpha	= fSubAlpha;	// 透明度の減算量

		// 確保したアドレスを返す
		return pEffect3D;
	}
	else { assert(false); return NULL; }	// 確保失敗
}
