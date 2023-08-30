//============================================================
//
//	オブジェクト軌跡処理 [objectOrbit.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "objectOrbit.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "sceneGame.h"
#include "pause.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CObjectOrbit::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\orbit000.jpg",	// 軌跡のテクスチャ
};

const D3DXVECTOR3 CObjectOrbit::mc_aOffset[][MAX_OFFSET]	// オフセットの位置加減量
{
	{ D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.0f, 100.0f, 0.0f) },	// 通常オフセット
	{ D3DXVECTOR3(0.0f, 0.0f, -25.0f),	D3DXVECTOR3(0.0f, 0.0f, 25.0f) },	// 杖オフセット
};

//************************************************************
//	子クラス [CObjectOrbit] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObjectOrbit::CObjectOrbit()
{
	// メンバ変数をクリア
	m_pVtxBuff = NULL;	// 頂点バッファ
	m_nNumVtx = 0;		// 必要頂点数
	m_nTextureID = 0;	// テクスチャインデックス
	memset(&m_orbit, 0, sizeof(m_orbit));	// 軌跡の情報
}

//============================================================
//	オーバーロードコンストラクタ
//============================================================
CObjectOrbit::CObjectOrbit(const CObject::LABEL label, const int nPriority) : CObject(label, nPriority)
{
	// メンバ変数をクリア
	m_pVtxBuff = NULL;	// 頂点バッファ
	m_nNumVtx = 0;		// 必要頂点数
	m_nTextureID = 0;	// テクスチャインデックス
	memset(&m_orbit, 0, sizeof(m_orbit));	// 軌跡の情報
}

//============================================================
//	デストラクタ
//============================================================
CObjectOrbit::~CObjectOrbit()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObjectOrbit::Init(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスのポインタ

	// メンバ変数を初期化
	m_pVtxBuff = NULL;			// 頂点バッファ
	m_nNumVtx = 0;				// 必要頂点数
	m_nTextureID = NONE_IDX;	// テクスチャインデックス

	// 軌跡の情報を初期化
	m_orbit.pMtxParent = NULL;	// 親のマトリックス
	m_orbit.bInit = false;		// 初期化状況
	m_orbit.nTexPart = 1;		// テクスチャ分割数

	for (int nCntOff = 0; nCntOff < MAX_OFFSET; nCntOff++)
	{ // オフセットの数分繰り返す

		m_orbit.aOffset[nCntOff] = VEC3_ZERO;	// 両端のオフセット
		m_orbit.aCol[nCntOff] = XCOL_WHITE;		// 両端の基準色
	}

	for (int nCntVtx = 0; nCntVtx < MAX_VERTEX; nCntVtx++)
	{ // 維持する頂点の最大数分繰り返す

		m_orbit.aPosPoint[nCntVtx] = VEC3_ZERO;		// 計算後の各頂点座標
		m_orbit.aColPoint[nCntVtx] = XCOL_WHITE;	// 各頂点カラー
	}

	// 長さを設定
	if (FAILED(SetLength()))
	{ // 長さの設定に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 自動描画をしない設定にする
	SetEnableDraw(false);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CObjectOrbit::Uninit(void)
{
	// 頂点バッファの破棄
	if (USED(m_pVtxBuff))
	{ // 頂点バッファが使用中の場合

		// メモリ開放
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// オブジェクト軌跡を破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CObjectOrbit::Update(void)
{

}

//============================================================
//	描画処理
//============================================================
void CObjectOrbit::Draw(void)
{
	// 変数を宣言
	D3DXMATRIX mtxIdent;	// 単位マトリックス設定用

	// 単位マトリックスの初期化
	D3DXMatrixIdentity(&mtxIdent);

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスのポインタ
	CTexture *pTexture = CManager::GetTexture();						// テクスチャへのポインタ
	
	//--------------------------------------------------------
	//	レンダーステートを変更
	//--------------------------------------------------------
	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ポリゴンの両面を表示状態にする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//--------------------------------------------------------
	//	オフセットの初期化
	//--------------------------------------------------------
	for (int nCntOff = 0; nCntOff < MAX_OFFSET; nCntOff++)
	{ // オフセットの数分繰り返す

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_orbit.aMtxWorldPoint[nCntOff]);

		// 位置を反映
		D3DXMatrixTranslation(&m_orbit.aMtxWorldPoint[nCntOff], m_orbit.aOffset[nCntOff].x, m_orbit.aOffset[nCntOff].y, m_orbit.aOffset[nCntOff].z);

		// 親のマトリックスと掛け合わせる
		D3DXMatrixMultiply(&m_orbit.aMtxWorldPoint[nCntOff], &m_orbit.aMtxWorldPoint[nCntOff], m_orbit.pMtxParent);
	}

	if (!CSceneGame::GetPause()->IsPause())
	{ // ポーズ中ではない場合

		//----------------------------------------------------
		//	頂点座標と頂点カラーの情報をずらす
		//----------------------------------------------------
		for (int nCntVtx = MAX_VERTEX - 1; nCntVtx >= MAX_OFFSET; nCntVtx--)
		{ // 維持する頂点の最大数分繰り返す (オフセット分は含まない)

			// 頂点情報をずらす
			m_orbit.aPosPoint[nCntVtx] = m_orbit.aPosPoint[nCntVtx - MAX_OFFSET];
			m_orbit.aColPoint[nCntVtx] = m_orbit.aColPoint[nCntVtx - MAX_OFFSET];
		}

		//----------------------------------------------------
		//	最新の頂点座標と頂点カラーの情報を設定
		//----------------------------------------------------
		for (int nCntOff = 0; nCntOff < MAX_OFFSET; nCntOff++)
		{ // オフセットの数分繰り返す

			// 頂点座標の設定
			m_orbit.aPosPoint[nCntOff] = D3DXVECTOR3
			( // 引数
				m_orbit.aMtxWorldPoint[nCntOff]._41,	// x
				m_orbit.aMtxWorldPoint[nCntOff]._42,	// y
				m_orbit.aMtxWorldPoint[nCntOff]._43		// z
			);

			// 頂点カラーの設定
			m_orbit.aColPoint[nCntOff] = m_orbit.aCol[nCntOff];
		}
	}

	//--------------------------------------------------------
	//	頂点座標と頂点カラーの情報を初期化
	//--------------------------------------------------------
	if (m_orbit.bInit == false)
	{ // 初期化済みではない場合

		for (int nCntVtx = 0; nCntVtx < MAX_VERTEX; nCntVtx++)
		{ // 維持する頂点の最大数分繰り返す

			// 頂点座標の設定
			m_orbit.aPosPoint[nCntVtx] = D3DXVECTOR3
			( // 引数
				m_orbit.aMtxWorldPoint[nCntVtx % MAX_OFFSET]._41,	// x
				m_orbit.aMtxWorldPoint[nCntVtx % MAX_OFFSET]._42,	// y
				m_orbit.aMtxWorldPoint[nCntVtx % MAX_OFFSET]._43	// z
			);

			// 頂点カラーの設定
			m_orbit.aColPoint[nCntVtx] = m_orbit.aCol[nCntVtx % MAX_OFFSET];
		}

		// 初期化済みにする
		m_orbit.bInit = true;
	}

	//--------------------------------------------------------
	//	ポリゴンの描画
	//--------------------------------------------------------
	// 頂点情報の設定
	SetVtx();

	// 単位マトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxIdent);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(m_nTextureID));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, MAX_VERTEX - 2);

	//--------------------------------------------------------
	//	レンダーステートを元に戻す
	//--------------------------------------------------------
	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ポリゴンの表面のみを表示状態にする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//============================================================
//	生成処理
//============================================================
CObjectOrbit *CObjectOrbit::Create
(
	D3DXMATRIX *pMtxParent,	// 親マトリックス
	const D3DXCOLOR& rCol,	// 色
	const OFFSET offset,	// オフセット
	const TYPE type,		// 種類
	const int nTexPart		// テクスチャ分割数
)
{
	// 変数を宣言
	int nTextureID;	// テクスチャインデックス

	// ポインタを宣言
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ
	CObjectOrbit *pObjectOrbit = NULL;				// オブジェクト軌跡生成用

	if (UNUSED(pObjectOrbit))
	{ // 使用されていない場合

		// メモリ確保
		pObjectOrbit = new CObjectOrbit;	// オブジェクト軌跡
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pObjectOrbit))
	{ // 確保に成功している場合

		// オブジェクト軌跡の初期化
		if (FAILED(pObjectOrbit->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pObjectOrbit;
			pObjectOrbit = NULL;

			// 失敗を返す
			return NULL;
		}

		// テクスチャを登録
		nTextureID = pTexture->Regist(mc_apTextureFile[type]);

		// テクスチャを割当
		pObjectOrbit->BindTexture(nTextureID);

		// 親のマトリックスを設定
		pObjectOrbit->SetMatrixParent(pMtxParent);

		// 色を設定
		pObjectOrbit->SetColor(rCol);

		// オフセットを設定
		pObjectOrbit->SetOffset(offset);

		// テクスチャ分割数を設定
		pObjectOrbit->SetTexPart(nTexPart);

		// 長さを設定
		if (FAILED(pObjectOrbit->SetLength()))
		{ // 長さの設定に失敗した場合

			// メモリ開放
			delete pObjectOrbit;
			pObjectOrbit = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pObjectOrbit;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	テクスチャ割当処理
//============================================================
void CObjectOrbit::BindTexture(const int nTextureID)
{
	// テクスチャインデックスを代入
	m_nTextureID = nTextureID;
}

//============================================================
//	親のマトリックスの設定処理
//============================================================
void CObjectOrbit::SetMatrixParent(D3DXMATRIX *pMtxParent)
{
	// 引数の親マトリックスを設定
	m_orbit.pMtxParent = pMtxParent;
}

//============================================================
//	色の設定処理
//============================================================
void CObjectOrbit::SetColor(const D3DXCOLOR& rCol)
{
	for (int nCntOff = 0; nCntOff < MAX_OFFSET; nCntOff++)
	{ // オフセットの数分繰り返す

		// 引数の色を設定
		m_orbit.aCol[nCntOff] = rCol;
	}
}

//============================================================
//	オフセットの設定処理
//============================================================
void CObjectOrbit::SetOffset(const OFFSET offset)
{
	for (int nCntOff = 0; nCntOff < MAX_OFFSET; nCntOff++)
	{ // オフセットの数分繰り返す

		// 引数のオフセットを設定
		m_orbit.aOffset[nCntOff] = mc_aOffset[(int)offset][nCntOff];
	}
}

//============================================================
//	テクスチャ分割数の設定処理
//============================================================
void CObjectOrbit::SetTexPart(const int nTexPart)
{
	// 例外処理
	assert(nTexPart > 0);	// 0以下エラー

	// 引数のテクスチャ分割数を設定
	m_orbit.nTexPart = nTexPart;
}

//============================================================
//	初期化状況の設定処理
//============================================================
void CObjectOrbit::SetEnableInit(const bool bInit)
{
	// 引数の初期化状況を設定
	m_orbit.bInit = bInit;
}

//============================================================
//	長さの設定処理
//============================================================
HRESULT CObjectOrbit::SetLength(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスのポインタ

	// 必要頂点数を求める
	m_nNumVtx = MAX_VERTEX;

	// 頂点バッファの破棄
	if (USED(m_pVtxBuff))
	{ // 頂点バッファが使用中の場合

		// メモリ開放
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// 頂点バッファの情報を設定
	if (UNUSED(m_pVtxBuff))
	{ // 非使用中の場合

		// 頂点バッファの生成
		if (FAILED(pDevice->CreateVertexBuffer
		( // 引数
			sizeof(VERTEX_3D) * m_nNumVtx,	// 必要頂点数
			D3DUSAGE_WRITEONLY,	// 使用方法
			FVF_VERTEX_3D,		// 頂点フォーマット
			D3DPOOL_MANAGED,	// メモリの指定
			&m_pVtxBuff,		// 頂点バッファへのポインタ
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
//	頂点情報の設定処理
//============================================================
void CObjectOrbit::SetVtx(void)
{
	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	if (USED(m_pVtxBuff))
	{ // 軌跡の頂点バッファが使用中の場合

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtx = 0; nCntVtx < MAX_VERTEX; nCntVtx++)
		{ // 維持する頂点の最大数分繰り返す

			// 頂点座標の設定
			pVtx[0].pos = m_orbit.aPosPoint[nCntVtx];

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR
			( // 引数
				m_orbit.aColPoint[nCntVtx].r,	// r
				m_orbit.aColPoint[nCntVtx].g,	// g
				m_orbit.aColPoint[nCntVtx].b,	// b
				1.0f - (1.0f / (MAX_VERTEX * 0.5f)) * (nCntVtx / 2)	// a
			);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2
			( // 引数
				(1.0f / (float)m_orbit.nTexPart) * (nCntVtx / 2),	// u
				1.0f * -((nCntVtx % 2) - 1)							// v
			);

			// 頂点データのポインタを進める
			pVtx += 1;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
	else { assert(false); }	// 非使用中
}
