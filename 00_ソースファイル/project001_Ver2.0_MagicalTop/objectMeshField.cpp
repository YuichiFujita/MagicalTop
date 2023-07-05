//============================================================
//
//	オブジェクトメッシュフィールド処理 [objectMeshField.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "objectMeshField.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"

//************************************************************
//	マクロ定義
//************************************************************
#define NUM_VTX_TRIANGLE	(3)	// 三角形ポリゴンの頂点数

//************************************************************
//	子クラス [CObjectMeshField] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObjectMeshField::CObjectMeshField()
{
	// メンバ変数をクリア
	m_pVtxBuff = NULL;		// 頂点バッファ
	m_pIdxBuff = NULL;		// インデックスバッファ
	m_part = GRID2_ZERO;	// 分割数
	m_nNumVtx = 0;			// 必要頂点数
	m_nNumIdx = 0;			// 必要インデックス数
	m_nTextureID = 0;		// テクスチャインデックス
	memset(&m_meshField, 0, sizeof(m_meshField));	// メッシュフィールドの情報
}

//============================================================
//	オーバーロードコンストラクタ
//============================================================
CObjectMeshField::CObjectMeshField(const CObject::LABEL label, const int nPriority) : CObject(label, nPriority)
{
	// メンバ変数をクリア
	m_pVtxBuff = NULL;		// 頂点バッファ
	m_pIdxBuff = NULL;		// インデックスバッファ
	m_part = GRID2_ZERO;	// 分割数
	m_nNumVtx = 0;			// 必要頂点数
	m_nNumIdx = 0;			// 必要インデックス数
	m_nTextureID = 0;		// テクスチャインデックス
	memset(&m_meshField, 0, sizeof(m_meshField));	// メッシュフィールドの情報
}

//============================================================
//	デストラクタ
//============================================================
CObjectMeshField::~CObjectMeshField()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObjectMeshField::Init(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスのポインタ

	// メンバ変数を初期化
	m_pVtxBuff = NULL;		// 頂点バッファ
	m_pIdxBuff = NULL;		// インデックスバッファ
	m_part = GRID2_ZERO;	// 分割数
	m_nNumVtx = 0;			// 必要頂点数
	m_nNumIdx = 0;			// 必要インデックス数
	m_nTextureID = -1;		// テクスチャインデックス

	m_meshField.pos = VEC3_ZERO;	// 位置
	m_meshField.rot = VEC3_ZERO;	// 向き
	m_meshField.size = VEC2_ZERO;	// 大きさ
	m_meshField.col = XCOL_WHITE;	// 色
	m_meshField.cull = D3DCULL_CCW;	// カリング状況
	m_meshField.bLight = true;		// ライティング状況

	// 分割数を設定
	if (FAILED(SetPattern(GRID2_ONE)))
	{ // 分割数の設定に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CObjectMeshField::Uninit(void)
{
	// 頂点バッファの破棄
	if (USED(m_pVtxBuff))
	{ // 頂点バッファが使用中の場合

		// メモリ開放
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// インデックスバッファの破棄
	if (USED(m_pIdxBuff))
	{ // インデックスバッファが使用中の場合

		// メモリ開放
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// オブジェクトメッシュフィールドを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CObjectMeshField::Update(void)
{

}

//============================================================
//	描画処理
//============================================================
void CObjectMeshField::Draw(void)
{
	// 変数を宣言
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスのポインタ
	CTexture *pTexture = CManager::GetTexture();						// テクスチャへのポインタ

	// ポリゴンの裏面のみを表示状態にする
	pDevice->SetRenderState(D3DRS_CULLMODE, m_meshField.cull);

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, m_meshField.bLight);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_meshField.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_meshField.rot.y, m_meshField.rot.x, m_meshField.rot.z);
	D3DXMatrixMultiply(&m_meshField.mtxWorld, &m_meshField.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_meshField.pos.x, m_meshField.pos.y, m_meshField.pos.z);
	D3DXMatrixMultiply(&m_meshField.mtxWorld, &m_meshField.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_meshField.mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(m_nTextureID));

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive
	( // 引数
		D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,
		0,
		m_nNumVtx,		// 使用する頂点数
		0,				// インデックスバッファの開始地点
		m_nNumIdx - 2	// プリミティブ (ポリゴン) 数
	);

	// ポリゴンの表面のみを表示状態にする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
}

//============================================================
//	生成処理
//============================================================
CObjectMeshField *CObjectMeshField::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXVECTOR2& rSize,	// 大きさ
	const D3DXCOLOR& rCol,		// 色
	const POSGRID2& rPart,		// 分割数
	const D3DCULL cull,			// カリング状況
	const bool bLight			// ライティング状況
)
{
	// ポインタを宣言
	CObjectMeshField *pObjectMeshField = NULL;		// オブジェクトメッシュフィールド生成用

	if (UNUSED(pObjectMeshField))
	{ // 使用されていない場合

		// メモリ確保
		pObjectMeshField = new CObjectMeshField;	// オブジェクトメッシュフィールド
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pObjectMeshField))
	{ // 確保に成功している場合

		if (SUCCEEDED(pObjectMeshField->GetResult()))
		{ // オブジェクトの情報設定に成功した場合

			// オブジェクトメッシュフィールドの初期化
			if (FAILED(pObjectMeshField->Init()))
			{ // 初期化に失敗した場合

				// メモリ開放
				delete pObjectMeshField;
				pObjectMeshField = NULL;

				// 失敗を返す
				return NULL;
			}

			// 位置を設定
			pObjectMeshField->SetPosition(rPos);

			// 向きを設定
			pObjectMeshField->SetRotation(rRot);

			// 大きさを設定
			pObjectMeshField->SetScalingBiaxial(rSize);

			// 色を設定
			pObjectMeshField->SetColor(rCol);

			// カリングを設定
			pObjectMeshField->SetCulling(cull);

			// ライティングを設定
			pObjectMeshField->SetLighting(bLight);

			// 分割数を設定
			if (FAILED(pObjectMeshField->SetPattern(rPart)))
			{ // 分割数の設定に失敗した場合

				// メモリ開放
				delete pObjectMeshField;
				pObjectMeshField = NULL;

				// 失敗を返す
				return NULL;
			}

			// 確保したアドレスを返す
			return pObjectMeshField;
		}
		else
		{ // オブジェクトの情報設定に失敗した場合

			// メモリ開放
			delete pObjectMeshField;
			pObjectMeshField = NULL;

			// 失敗を返す
			return NULL;
		}
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	テクスチャ割当処理
//============================================================
void CObjectMeshField::BindTexture(const int nTextureID)
{
	// テクスチャインデックスを代入
	m_nTextureID = nTextureID;
}

//============================================================
//	位置の設定処理
//============================================================
void CObjectMeshField::SetPosition(const D3DXVECTOR3& rPos)
{
	// 引数の位置を設定
	m_meshField.pos = rPos;
}

//============================================================
//	向きの設定処理
//============================================================
void CObjectMeshField::SetRotation(const D3DXVECTOR3& rRot)
{
	// 引数の向きを設定
	m_meshField.rot = rRot;

	// 向きの正規化
	useful::NormalizeRot(m_meshField.rot.z);
}

//============================================================
//	大きさの設定処理
//============================================================
void CObjectMeshField::SetScalingBiaxial(const D3DXVECTOR2& rSize)
{
	// 引数の大きさを設定
	m_meshField.size = rSize;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	色の設定処理
//============================================================
void CObjectMeshField::SetColor(const D3DXCOLOR& rCol)
{
	// 引数の色を設定
	m_meshField.col = rCol;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	カリング設定処理
//============================================================
void CObjectMeshField::SetCulling(const D3DCULL cull)
{
	// 引数のカリング状況を設定
	m_meshField.cull = cull;
}

//============================================================
//	ライティング設定処理
//============================================================
void CObjectMeshField::SetLighting(const bool bLight)
{
	// 引数のライティング状況を設定
	m_meshField.bLight = bLight;
}

//============================================================
//	分割数の設定処理
//============================================================
HRESULT CObjectMeshField::SetPattern(const POSGRID2& rPart)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスのポインタ

	// 引数の分割数を設定
	m_part = rPart;

	// 必要頂点・インデックス数を求める
	m_nNumVtx = (m_part.x + 1) * (m_part.y + 1); // 必要頂点数
	m_nNumIdx = (m_part.x + 1) * (((m_part.y + 1) * 2) - 2) + (m_part.y * 2) - 2; // 必要インデックス数

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

	// インデックスバッファの破棄
	if (USED(m_pIdxBuff))
	{ // インデックスバッファが使用中の場合

		// メモリ開放
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// インデックスバッファの情報を設定
	if (UNUSED(m_pIdxBuff))
	{ // 非使用中の場合

		// インデックスバッファの生成
		if (FAILED(pDevice->CreateIndexBuffer
		( // 引数
			sizeof(WORD) * m_nNumIdx,	// 必要インデックス数
			D3DUSAGE_WRITEONLY,	// 使用方法
			D3DFMT_INDEX16,		// インデックスバッファのフォーマット
			D3DPOOL_MANAGED,	// メモリの指定
			&m_pIdxBuff,		// インデックスバッファへのポインタ
			NULL
		)))
		{ // インデックスバッファの生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}
	else { assert(false); return E_FAIL; }	// 使用中

	// 頂点・インデックス情報の設定
	SetVtx();
	SetIdx();

	// 成功を返す
	return S_OK;
}

//============================================================
//	メッシュの頂点位置の設定処理
//============================================================
void CObjectMeshField::SetMeshVertexPosition(const int nID, const D3DXVECTOR3& rPos)
{
	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	if (USED(m_pVtxBuff))
	{ // 使用中の場合

		if (nID < (m_part.x + 1) * (m_part.y + 1))
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
}

//============================================================
//	位置取得処理
//============================================================
D3DXVECTOR3 CObjectMeshField::GetPosition(void) const
{
	// 位置を返す
	return m_meshField.pos;
}

//============================================================
//	向き取得処理
//============================================================
D3DXVECTOR3 CObjectMeshField::GetRotation(void) const
{
	// 向きを返す
	return m_meshField.rot;
}

//============================================================
//	大きさ取得処理
//============================================================
D3DXVECTOR2 CObjectMeshField::GetScalingBiaxial(void) const
{
	// 大きさを返す
	return m_meshField.size;
}

//============================================================
//	色取得処理
//============================================================
D3DXCOLOR CObjectMeshField::GetColor(void) const
{
	// 色を返す
	return m_meshField.col;
}

//============================================================
//	カリング取得処理
//============================================================
D3DCULL CObjectMeshField::GetCulling(void) const
{
	// カリング状況を返す
	return m_meshField.cull;
}

//============================================================
//	ライティング取得処理
//============================================================
bool CObjectMeshField::GetLighting(void) const
{
	// ライティング状況を返す
	return m_meshField.bLight;
}

//============================================================
//	分割数取得処理
//============================================================
POSGRID2 CObjectMeshField::GetPattern(void) const
{
	// 分割数を返す
	return m_part;
}

//============================================================
//	メッシュの頂点位置の設定処理
//============================================================
D3DXVECTOR3 CObjectMeshField::GetMeshVertexPosition(const int nID)
{
	// 変数を宣言
	D3DXVECTOR3 pos;	// 頂点座標の代入用

	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	if (USED(m_pVtxBuff))
	{ // 使用中の場合

		if (nID < (m_part.x + 1) * (m_part.y + 1))
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
//	メッシュの着地位置の取得処理
//============================================================
float CObjectMeshField::GetPositionHeight(const D3DXVECTOR3&rPos)
{
	// 変数を宣言
	int nNumCul;		// 法線ベクトル用の頂点計算用
	int nNumVtx;		// 法線を求める頂点番号
	D3DXVECTOR3 nor;	// 法線ベクトル

	// 変数配列を宣言
	D3DXVECTOR3 aVtxPos[NUM_VTX_TRIANGLE];		// ポリゴンの頂点座標
	D3DXVECTOR3 aVtxMtxPos[NUM_VTX_TRIANGLE];	// ポリゴンの位置・向き反映を行った頂点座標

	for (int nCntVtx = 0; nCntVtx < 2; nCntVtx++)
	{ // 四角ポリゴンに使用する三角の数分繰り返す

		// 法線ベクトル用の頂点計算用の値を設定
		nNumCul = (nCntVtx == 0) ? 1: -1;

		for (int nCntHeight = 0; nCntHeight < m_part.y; nCntHeight++)
		{ // 縦の分割数分繰り返す

			for (int nCntWidth = 0; nCntWidth < m_part.x; nCntWidth++)
			{ // 横の分割数分繰り返す

				// 法線を求める頂点番号を設定
				nNumVtx = ((nCntWidth + 1) + (nCntHeight * (m_part.x + 1))) + (nCntVtx * m_part.x);

				// ポリゴンの頂点位置を取得
				aVtxPos[0] = GetMeshVertexPosition(nNumVtx);
				aVtxPos[1] = GetMeshVertexPosition(nNumVtx - (1 * nNumCul));
				aVtxPos[2] = GetMeshVertexPosition(nNumVtx + ((m_part.x + 1) * nNumCul));

				for (int nCnt = 0; nCnt < NUM_VTX_TRIANGLE; nCnt++)
				{ // 三角形ポリゴンの頂点数分繰り返す

					// 変数を宣言
					D3DXMATRIX mtxWorld, mtxRot, mtxTrans;	// 計算用マトリックス

					// ワールドマトリックスの初期化
					D3DXMatrixIdentity(&mtxWorld);

					// 頂点位置を反映
					D3DXMatrixTranslation(&mtxTrans, aVtxPos[nCnt].x, aVtxPos[nCnt].y, aVtxPos[nCnt].z);
					D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

					// ポリゴン向きを反映
					D3DXMatrixRotationYawPitchRoll(&mtxRot, m_meshField.rot.y, m_meshField.rot.x, m_meshField.rot.z);
					D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

					// ポリゴン位置を反映
					D3DXMatrixTranslation(&mtxTrans, m_meshField.pos.x, m_meshField.pos.y, m_meshField.pos.z);
					D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

					// 計算したマトリックスから座標を設定
					aVtxMtxPos[nCnt] = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
				}

				if (collision::TrianglePillar(aVtxMtxPos[0], aVtxMtxPos[1], aVtxMtxPos[2], rPos))
				{ // ポリゴンの範囲内にいる場合

					// 法線を求める
					useful::NormalizeNormal(aVtxMtxPos[1], aVtxMtxPos[0], aVtxMtxPos[2], nor);

					if (nor.y != 0.0f)
					{ // 法線が設定されている場合

						// プレイヤーの着地点を返す
						return (((rPos.x - aVtxMtxPos[0].x) * nor.x + (-aVtxMtxPos[0].y) * nor.y + (rPos.z - aVtxMtxPos[0].z) * nor.z) * -1.0f) / nor.y;
					}
				}
			}
		}
	}

	// 着地範囲外の場合現在のy座標を返す
	return rPos.y;
}

//============================================================
//	範囲外の着地処理
//============================================================
bool CObjectMeshField::LandPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove, const float fHeight)
{
	// 変数を宣言
	float fLandHeight = GetPositionHeight(rPos);	// 着地位置

	if (rPos.y - fHeight < fLandHeight)
	{ // 位置が地面より下の場合

		// 位置を補正
		rPos.y = fLandHeight + fHeight;

		// 移動量を初期化
		rMove.y = 0.0f;

		// 着地している状況を返す
		return true;
	}

	// 着地していない状況を返す
	return false;
}

//============================================================
//	頂点情報の設定処理
//============================================================
void CObjectMeshField::SetVtx(void)
{
	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	if (USED(m_pVtxBuff))
	{ // 使用中の場合

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntHeight = 0; nCntHeight < m_part.y + 1; nCntHeight++)
		{ // 縦の分割数 +1回繰り返す

			for (int nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
			{ // 横の分割数 +1回繰り返す

				// 頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3
				( // 引数
					nCntWidth * (m_meshField.size.x / (float)m_part.x) - (m_meshField.size.x * 0.5f),		// x
					0.0f,																					// y
					-(nCntHeight * (m_meshField.size.y / (float)m_part.y) - (m_meshField.size.y * 0.5f))	// z
				);

				// 法線ベクトルの設定
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// 頂点カラーの設定
				pVtx[0].col = m_meshField.col;

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(1.0f * nCntWidth, 1.0f * nCntHeight);

				// 頂点データのポインタを 1つ分進める
				pVtx += 1;
			}
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//============================================================
//	インデックス情報の設定処理
//============================================================
void CObjectMeshField::SetIdx(void)
{
	// ポインタを宣言
	WORD *pIdx;	// インデックス情報へのポインタ

	if (USED(m_pIdxBuff))
	{ // 使用中の場合

		// インデックスバッファをロックし、頂点番号データへのポインタを取得
		m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntHeight = 0, nCntWidth = 0; nCntHeight < m_part.y; nCntHeight++)
		{ // 縦の分割数 +1回繰り返す

			for (nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
			{ // 横の分割数 +1回繰り返す

				pIdx[0] = (m_part.x + 1) * (nCntHeight + 1) + nCntWidth;
				pIdx[1] = (m_part.x + 1) * nCntHeight + nCntWidth;

				// インデックスデータのポインタを 2つ分進める
				pIdx += 2;
			}

			if (nCntHeight != m_part.y - 1)
			{ // 一番手前の分割場所ではない場合

				pIdx[0] = (m_part.x + 1) * nCntHeight + nCntWidth - 1;
				pIdx[1] = (m_part.x + 1) * (nCntHeight + 2);

				// インデックスデータのポインタを 2つ分進める
				pIdx += 2;
			}
		}

		// インデックスバッファをアンロックする
		m_pIdxBuff->Unlock();
	}
}

//============================================================
//	スクロールのテクスチャ座標の設定処理
//============================================================
void CObjectMeshField::SetScrollTex(const float fTexU, const float fTexV)
{
	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	if (USED(m_pVtxBuff))
	{ // 使用中の場合

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntHeight = 0; nCntHeight < m_part.y + 1; nCntHeight++)
		{ // 縦の分割数 +1回繰り返す

			for (int nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
			{ // 横の分割数 +1回繰り返す

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2((float)nCntWidth + fTexU, (float)nCntHeight + fTexV);

				// 頂点データのポインタを 1つ分進める
				pVtx += 1;
			}
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}
