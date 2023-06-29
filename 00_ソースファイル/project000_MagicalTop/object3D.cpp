//============================================================
//
//	オブジェクト3D処理 [object3D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "object3D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"

//************************************************************
//	マクロ定義
//************************************************************
#define MAX_VERTEX	(4)	// 頂点数

//************************************************************
//	子クラス [CObject3D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObject3D::CObject3D()
{
	// メンバ変数をクリア
	m_pVtxBuff = NULL;	// 頂点バッファへのポインタ
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));		// ワールドマトリックス
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_size	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 大きさ
	m_col	= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 色
	m_nTextureID = 0;	// テクスチャインデックス
}

//============================================================
//	オーバーロードコンストラクタ
//============================================================
CObject3D::CObject3D(const CObject::LABEL label, const int nPriority) : CObject(label, nPriority)
{
	// メンバ変数をクリア
	m_pVtxBuff = NULL;	// 頂点バッファへのポインタ
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));		// ワールドマトリックス
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_size	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 大きさ
	m_col	= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 色
	m_nTextureID = 0;	// テクスチャインデックス
}

//============================================================
//	デストラクタ
//============================================================
CObject3D::~CObject3D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObject3D::Init(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスのポインタ

	// メンバ変数を初期化
	m_pVtxBuff = NULL;	// 頂点バッファへのポインタ
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));		// ワールドマトリックス
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_size	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 大きさ
	m_col	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	m_nTextureID = -1;	// テクスチャインデックス

	if (UNUSED(m_pVtxBuff))
	{ // 非使用中の場合

		// 頂点バッファの生成
		if (FAILED(pDevice->CreateVertexBuffer
		( // 引数
			sizeof(VERTEX_3D) * MAX_VERTEX,	// 必要頂点数
			D3DUSAGE_WRITEONLY,				// 使用方法
			FVF_VERTEX_3D,					// 頂点フォーマット
			D3DPOOL_MANAGED,				// メモリの指定
			&m_pVtxBuff,					// 頂点バッファへのポインタ
			NULL
		)))
		{ // 頂点バッファの生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}
	else { assert(false); return E_FAIL; }	// 使用中

	// 頂点情報の設定
	SetVtx();

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CObject3D::Uninit(void)
{
	// 頂点バッファの破棄
	if (USED(m_pVtxBuff))
	{ // 頂点バッファが使用中の場合

		// メモリ開放
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// オブジェクト3Dを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CObject3D::Update(void)
{

}

//============================================================
//	描画処理
//============================================================
void CObject3D::Draw(void)
{
	// 変数を宣言
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスのポインタ
	CTexture *pTexture = CManager::GetTexture();						// テクスチャへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(m_nTextureID));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//============================================================
//	生成処理
//============================================================
CObject3D *CObject3D::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const D3DXVECTOR3& rRot, const D3DXCOLOR& rCol)
{
	// ポインタを宣言
	CObject3D *pObject3D = NULL;	// オブジェクト3D生成用

	if (UNUSED(pObject3D))
	{ // 使用されていない場合

		// メモリ確保
		pObject3D = new CObject3D;	// オブジェクト3D
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pObject3D))
	{ // 確保に成功している場合

		if (SUCCEEDED(pObject3D->GetResult()))
		{ // オブジェクトの情報設定に成功した場合

			// オブジェクト3Dの初期化
			if (FAILED(pObject3D->Init()))
			{ // 初期化に失敗した場合

				// メモリ開放
				delete pObject3D;
				pObject3D = NULL;

				// 失敗を返す
				return NULL;
			}

			// 位置を設定
			pObject3D->SetPosition(rPos);

			// 向きを設定
			pObject3D->SetRotation(rRot);

			// 大きさを設定
			pObject3D->SetScaling(rSize);

			// 色を設定
			pObject3D->SetColor(rCol);

			// 確保したアドレスを返す
			return pObject3D;
		}
		else
		{ // オブジェクトの情報設定に失敗した場合

			// メモリ開放
			delete pObject3D;
			pObject3D = NULL;

			// 失敗を返す
			return NULL;
		}
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	テクスチャ割当処理
//============================================================
void CObject3D::BindTexture(const int nTextureID)
{
	// テクスチャインデックスを代入
	m_nTextureID = nTextureID;
}

//============================================================
//	位置の設定処理
//============================================================
void CObject3D::SetPosition(const D3DXVECTOR3& rPos)
{
	// 引数の位置を設定
	m_pos = rPos;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	向きの設定処理
//============================================================
void CObject3D::SetRotation(const D3DXVECTOR3& rRot)
{
	// 引数の向きを設定
	m_rot = rRot;

	// 向きの正規化
	useful::NormalizeRot(m_rot.x);
	useful::NormalizeRot(m_rot.y);
	useful::NormalizeRot(m_rot.z);

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	大きさの設定処理
//============================================================
void CObject3D::SetScaling(const D3DXVECTOR3& rSize)
{
	// 引数の大きさを代入
	m_size = rSize;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	色の設定処理
//============================================================
void CObject3D::SetColor(const D3DXCOLOR& rCol)
{
	// 引数の色を代入
	m_col = rCol;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	頂点位置の設定処理
//============================================================
void CObject3D::SetVertexPosition(const int nID, const D3DXVECTOR3& rPos)
{
	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	if (USED(m_pVtxBuff))
	{ // 使用中の場合

		if (nID < MAX_VERTEX)
		{ // インデックスが使用可能な場合

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点データのポインタを引数分進める
			pVtx += nID;

			// 頂点座標の設定
			pVtx[0].pos = rPos;

			// 頂点バッファをアンロックする
			m_pVtxBuff->Unlock();
		}
	}

	// 法線の設定・正規化
	NormalizeNormal();
}

//============================================================
//	位置取得処理
//============================================================
D3DXVECTOR3 CObject3D::GetPosition(void) const
{
	// 位置を返す
	return m_pos;
}

//============================================================
//	向き取得処理
//============================================================
D3DXVECTOR3 CObject3D::GetRotation(void) const
{
	// 向きを返す
	return m_rot;
}

//============================================================
//	大きさ取得処理
//============================================================
D3DXVECTOR3 CObject3D::GetScaling(void) const
{
	// 大きさを返す
	return m_size;
}

//============================================================
//	色取得処理
//============================================================
D3DXCOLOR CObject3D::GetColor(void) const
{
	// 色を返す
	return m_col;
}

//============================================================
//	頂点位置取得処理
//============================================================
D3DXVECTOR3 CObject3D::GetVertexPosition(const int nID)
{
	// 変数を宣言
	D3DXVECTOR3 pos;	// 頂点座標の代入用

	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	if (USED(m_pVtxBuff))
	{ // 使用中の場合

		if (nID < MAX_VERTEX)
		{ // インデックスが使用可能な場合

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点データのポインタを引数分進める
			pVtx += nID;

			// 頂点座標を代入
			pos = pVtx[0].pos;

			// 頂点バッファをアンロックする
			m_pVtxBuff->Unlock();
		}
	}

	// 引数のインデックスの頂点座標を返す
	return pos;
}

//============================================================
//	ポリゴンの着地取得処理
//============================================================
float CObject3D::GetPositionHeight(const D3DXVECTOR3&rPos)
{
	// 変数を宣言
	D3DXVECTOR3 nor;	// 法線ベクトル

	// 変数配列を宣言
	D3DXVECTOR3 aVtxPos[MAX_VERTEX];	// ポリゴンの頂点座標 [※] 0：左上 1：右上 2：左下 3：右下

	for (int nCnt = 0; nCnt < MAX_VERTEX; nCnt++)
	{ // 頂点数分繰り返す

		// 変数を宣言
		D3DXMATRIX mtxWorld, mtxRot, mtxTrans;		// 計算用マトリックス
		D3DXVECTOR3 pos = GetVertexPosition(nCnt);	// 頂点座標
		
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxWorld);

		// 頂点位置を反映
		D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		// ポリゴン向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// ポリゴン位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		// 計算したマトリックスから座標を設定
		aVtxPos[nCnt] = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
	}

	if (collision::LineOuterProduct(aVtxPos[0], aVtxPos[1], rPos) > 0
	&&  collision::LineOuterProduct(aVtxPos[1], aVtxPos[2], rPos) > 0
	&&  collision::LineOuterProduct(aVtxPos[2], aVtxPos[0], rPos) > 0)
	{ // ポリゴンの範囲内にいる場合

		// 法線を求める
		useful::NormalizeNormal(aVtxPos[1], aVtxPos[0], aVtxPos[2], nor);

		if (nor.y != 0.0f)
		{ // 法線が設定されている場合

			// プレイヤーの着地点を返す
			return (((rPos.x - aVtxPos[0].x) * nor.x + (-aVtxPos[0].y) * nor.y + (rPos.z - aVtxPos[0].z) * nor.z) * -1.0f) / nor.y;
		}
	}

	if (collision::LineOuterProduct(aVtxPos[3], aVtxPos[1], rPos) < 0
	&&  collision::LineOuterProduct(aVtxPos[1], aVtxPos[2], rPos) < 0
	&&  collision::LineOuterProduct(aVtxPos[2], aVtxPos[3], rPos) < 0)
	{ // ポリゴンの範囲内にいる場合

		// 法線を求める
		useful::NormalizeNormal(aVtxPos[2], aVtxPos[3], aVtxPos[1], nor);

		if (nor.y != 0.0f)
		{ // 法線が設定されている場合

			// プレイヤーの着地点を返す
			return (((rPos.x - aVtxPos[3].x) * nor.x + (-aVtxPos[3].y) * nor.y + (rPos.z - aVtxPos[3].z) * nor.z) * -1.0f) / nor.y;
		}
	}

	// 着地範囲外の場合現在のy座標を返す
	return rPos.y;
}

//============================================================
//	頂点情報の設定処理
//============================================================
void CObject3D::SetVtx(void)
{
	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_size.x * 0.5f,  m_size.y * 0.5f,  m_size.z * 0.5f);
	pVtx[1].pos = D3DXVECTOR3( m_size.x * 0.5f,  m_size.y * 0.5f,  m_size.z * 0.5f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x * 0.5f, -m_size.y * 0.5f, -m_size.z * 0.5f);
	pVtx[3].pos = D3DXVECTOR3( m_size.x * 0.5f, -m_size.y * 0.5f, -m_size.z * 0.5f);

	// 法線の設定・正規化
	NormalizeNormal();

	// 頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//============================================================
//	法線の正規化処理
//============================================================
void CObject3D::NormalizeNormal(void)
{
	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 法線の正規化
	useful::NormalizeNormal
	( // 引数
		pVtx[1].pos,	// 左位置
		pVtx[0].pos,	// 中心位置
		pVtx[2].pos,	// 右位置
		pVtx[0].nor		// 法線
	);

	// 法線の正規化
	useful::NormalizeNormal
	( // 引数
		pVtx[2].pos,	// 左位置
		pVtx[3].pos,	// 中心位置
		pVtx[1].pos,	// 右位置
		pVtx[3].nor		// 法線
	);

	// 法線ベクトルの設定
	pVtx[1].nor = (pVtx[0].nor + pVtx[3].nor) / 2;
	pVtx[2].nor = (pVtx[0].nor + pVtx[3].nor) / 2;

	// 法線を正規化
	D3DXVec3Normalize(&pVtx[1].nor, &pVtx[1].nor);
	D3DXVec3Normalize(&pVtx[2].nor, &pVtx[2].nor);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
