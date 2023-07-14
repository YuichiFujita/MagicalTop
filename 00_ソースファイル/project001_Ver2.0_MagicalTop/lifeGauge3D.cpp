//============================================================
//
//	体力ゲージ3D処理 [lifeGauge3D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "lifeGauge3D.h"
#include "manager.h"
#include "renderer.h"
#include "objectBillboard.h"
#include "texture.h"

//************************************************************
//	マクロ定義
//************************************************************
#define LIFEGAUGE3D_PRIO	(4)	// 体力ゲージ3Dの優先順位
#define GAUGE_WIDTH	(100.0f)	// 体力ゲージの横幅

#define GAUGE_SIZE	(D3DXVECTOR3(100.0f, 20.0f, 0.0f))	// ゲージサイズ
#define FRAME_SIZE	(D3DXVECTOR3(102.5f, 22.5f, 0.0f))	// ゲージフレームサイズ
#define BG_COL		(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	// ゲージ背景色
#define GAUGE_COL	(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f))	// ゲージ色

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CLifeGauge3D::mc_apTextureFile[] =	// テクスチャ定数
{
	NULL,	// テクスチャなし
	NULL,	// テクスチャなし
	"data\\TEXTURE\\lifeGauge000.png",	// ゲージフレーム
};

//************************************************************
//	子クラス [CLifeGauge3D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CLifeGauge3D::CLifeGauge3D(const int nMax, const int nFrame) : CObject(CObject::LABEL_LOCK, LIFEGAUGE3D_PRIO), m_nMaxLife(nMax), m_nFrame(nFrame)	// TODO：ここロックになってる！！Release見直し！
{
	// メンバ変数をクリア
	memset(&m_apBilboard[0], 0, sizeof(m_apBilboard));	// ビルボードの情報
	m_pLife = NULL;			// 体力ゲージ3D表示オブジェクトの情報
	m_state = STATE_NONE;	// 状態
	m_fChange = 0.0f;		// 体力変動量
	m_fCurrentLife = 0.0f;	// 現在体力
	m_fPosUp = 0.0f;		// 体力表示のY位置加算量
	m_nCounterState = 0;	// 状態管理カウンター
	m_nLife = 0;			// 体力
}

//============================================================
//	デストラクタ
//============================================================
CLifeGauge3D::~CLifeGauge3D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CLifeGauge3D::Init(void)
{
	// 変数を宣言
	int nTextureID;	// テクスチャインデックス

	// 変数配列を宣言
	D3DXVECTOR3 aSize[] =	// 大きさ
	{ // 初期値
		GAUGE_SIZE,	// 体力背景
		GAUGE_SIZE,	// 体力ゲージ
		FRAME_SIZE,	// 体力ゲージフレーム
	};
	D3DXCOLOR aCol[] =		// 色
	{ // 初期値
		BG_COL,		// 体力背景
		GAUGE_COL,	// 体力ゲージ
		XCOL_WHITE,	// 体力ゲージフレーム
	};

	// ポインタを宣言
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ

	// メンバ変数を初期化
	memset(&m_apBilboard[0], 0, sizeof(m_apBilboard));	// ビルボードの情報
	m_pLife = NULL;			// 体力ゲージ3D表示オブジェクトの情報
	m_state = STATE_NONE;	// 状態
	m_fChange = 0.0f;		// 体力変動量
	m_fCurrentLife = 0.0f;	// 現在体力
	m_fPosUp = 0.0f;		// 体力表示のY位置加算量
	m_nCounterState = 0;	// 状態管理カウンター
	m_nLife = 0;			// 体力

	for (int nCntLife = 0; nCntLife < TEXTURE_MAX; nCntLife++)
	{ // テクスチャの最大数分繰り返す

		// ビルボードの生成
		m_apBilboard[nCntLife] = CObjectBillboard::Create
		( // 引数
			VEC3_ZERO,			// 位置
			aSize[nCntLife],	// 大きさ
			VEC3_ZERO,			// 向き
			aCol[nCntLife]		// 色
		);
		if (UNUSED(m_apBilboard[nCntLife]))
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// テクスチャを登録
		nTextureID = pTexture->Regist(mc_apTextureFile[nCntLife]);

		// テクスチャを割当
		m_apBilboard[nCntLife]->BindTexture(nTextureID);
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CLifeGauge3D::Uninit(void)
{
	// ビルボードを破棄
	for (int nCntLife = 0; nCntLife < TEXTURE_MAX; nCntLife++)
	{ // テクスチャの最大数分繰り返す

		// ビルボードの終了
		m_apBilboard[nCntLife]->Uninit();
	}

	// 自身のオブジェクトを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CLifeGauge3D::Update(void)
{
	// 変数を宣言
	D3DXMATRIX mtxLife;	// 体力表示オブジェクトのマトリックス

	// ゲージの設定
	if (m_state == STATE_CHANGE)
	{ // 体力が変動中の場合

		// 変数を宣言
		D3DXVECTOR3 size = m_apBilboard[TEXTURE_GAUGE]->GetScaling();	// 大きさ

		// カウンターを減算
		m_nCounterState--;

		// 現在の体力ゲージから受けたダメージ分を加算
		m_fCurrentLife += m_fChange;

		// 大きさを設定
		useful::LimitNum(m_fCurrentLife, 0.0f, (float)m_nMaxLife);		// 現在体力を補正
		size.x = m_fCurrentLife * (GAUGE_WIDTH / (float)m_nMaxLife);	// ゲージの横幅を設定
		m_apBilboard[TEXTURE_GAUGE]->SetScaling(size);					// 体力を設定

		if (m_nCounterState <= 0)
		{ // カウンターが 0以下になった場合

			// 通常状態にする
			m_state = STATE_NONE;
		}
	}

	// 位置の設定
	for (int nCntLife = 0; nCntLife < TEXTURE_MAX; nCntLife++)
	{ // テクスチャの最大数分繰り返す

		// ビルボードの更新
		m_apBilboard[nCntLife]->Update();

		// TODO：ポインタの確認もっときれいに
		if (CObject::CheckUse(m_pLife))
		{ // 体力表示対象が使用されていた場合

			// ワールドマトリックスを取得
			mtxLife = m_pLife->GetMtxWorld();

			// 位置を設定
			m_apBilboard[nCntLife]->SetPosition(D3DXVECTOR3(mtxLife._41, mtxLife._42 + m_fPosUp, mtxLife._43));
		}
		else
		{ // 体力表示対象が使用されていなかった場合

			// 体力表示しているオブジェクトの情報を初期化
			m_pLife = NULL;

			// オブジェクトの終了
			Uninit();

			// 関数を抜ける
			return;
		}
	}
}

//============================================================
//	描画処理
//============================================================
void CLifeGauge3D::Draw(void)
{
	for (int nCntLife = 0; nCntLife < TEXTURE_MAX; nCntLife++)
	{ // テクスチャの最大数分繰り返す

		// ビルボードの描画
		m_apBilboard[nCntLife]->Draw();
	}
}

//============================================================
//	生成処理
//============================================================
CLifeGauge3D *CLifeGauge3D::Create
(
	const int nLife,	// 体力
	const int nMax,		// 最大体力
	const int nFrame,	// 体力変動フレーム
	const float fPosUp,	// 体力表示のY位置加算量
	CObject *pObject	// 体力ゲージ表示オブジェクト
)
{
	// ポインタを宣言
	CLifeGauge3D *pLifeGauge3D = NULL;	// 体力ゲージ3D生成用

	if (UNUSED(pLifeGauge3D))
	{ // 使用されていない場合

		// メモリ確保
		pLifeGauge3D = new CLifeGauge3D(nMax, nFrame);	// 体力ゲージ3D
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pLifeGauge3D))
	{ // 確保に成功している場合

		if (SUCCEEDED(pLifeGauge3D->GetResult()))
		{ // オブジェクトの情報設定に成功した場合

			// 体力ゲージ3Dの初期化
			if (FAILED(pLifeGauge3D->Init()))
			{ // 初期化に失敗した場合

				// 失敗を返す
				return NULL;
			}

			// 体力の設定
			pLifeGauge3D->SetLife(nLife);

			// 体力表示の設定
			pLifeGauge3D->SetLifeObject(pObject);

			// Y位置加算量の設定
			pLifeGauge3D->SetPositionUp(fPosUp);

			// 確保したアドレスを返す
			return pLifeGauge3D;
		}
		else
		{ // オブジェクトの情報設定に失敗した場合

			// メモリ開放
			delete pLifeGauge3D;
			pLifeGauge3D = NULL;

			// 失敗を返す
			return NULL;
		}
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	体力の加算処理
//============================================================
void CLifeGauge3D::AddLife(const int nAdd)
{
	// 現在の体力を設定
	m_fCurrentLife = (float)m_nLife;

	// 体力変動量を求める
	m_fChange = ((float)nAdd / (float)m_nFrame);

	// 情報を設定
	m_state = STATE_CHANGE;		// 数値変動状態
	m_nCounterState = m_nFrame;	// 状態管理カウンター

	// 体力を引数分加算
	m_nLife += nAdd;

	// 体力の制限
	useful::LimitNum(m_nLife, 0, m_nMaxLife);
}

//============================================================
//	体力表示の設定処理
//============================================================
void CLifeGauge3D::SetLifeObject(CObject *pObject)
{
	// 引数のオブジェクトを設定
	m_pLife = pObject;
}

//============================================================
//	体力の設定処理
//============================================================
void CLifeGauge3D::SetLife(const int nLife)
{
	// 変数を宣言
	D3DXVECTOR3 size = m_apBilboard[TEXTURE_GAUGE]->GetScaling();	// 大きさ

	// 引数の体力を設定
	m_nLife = nLife;

	// 体力の制限
	useful::LimitNum(m_nLife, 0, m_nMaxLife);

	// ゲージを設定
	useful::LimitNum(m_nLife, 0, m_nMaxLife);						// 体力を補正
	size.x = (float)m_nLife * (GAUGE_WIDTH / (float)m_nMaxLife);	// ゲージの横幅を設定
	m_apBilboard[TEXTURE_GAUGE]->SetScaling(size);					// 体力を設定
}

//============================================================
//	Y位置加算量の設定処理
//============================================================
void CLifeGauge3D::SetPositionUp(const float fUp)
{
	// 引数のY位置加算量を設定
	m_fPosUp = fUp;
}

//============================================================
//	体力表示取得処理
//============================================================
const CObject *CLifeGauge3D::GetLifeObject(void) const
{
	// 体力表示中のオブジェクトを返す
	return m_pLife;
}

//============================================================
//	体力取得処理
//============================================================
int CLifeGauge3D::GetLife(void) const
{
	// 体力を返す
	return m_nLife;
}

//============================================================
//	Y位置加算量取得処理
//============================================================
float CLifeGauge3D::GetPositionUp(void) const
{
	// Y位置加算量を返す
	return m_fPosUp;
}
