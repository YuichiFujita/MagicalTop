//============================================================
//
//	パーティクル3D処理 [particle3D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "particle3D.h"
#include "effect3D.h"

//************************************************************
//	マクロ定義
//************************************************************
#define DMG_MOVE		(5.0f)		// ダメージの移動量
#define DMG_SPAWN		(35)		// ダメージの生成数
#define DMG_EFF_LIFE	(120)		// ダメージの寿命
#define DMG_SIZE		(80.0f)		// ダメージの大きさ
#define DMG_SUB_SIZE	(2.4f)		// ダメージの半径の減算量
#define DMG_SUB_ALPHA	(0.005f)	// ダメージの透明度の減算量

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const int CParticle3D::mc_aLife[CParticle3D::TYPE_MAX] =	// 寿命定数
{
	0,	// なし
	24,	// ダメージ
};

//************************************************************
//	子クラス [CParticle3D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CParticle3D::CParticle3D() : CObject(CObject::LABEL_PARTICLE3D)
{
	// メンバ変数をクリア
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_col	= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 色
	m_type	= TYPE_NONE;	// 種類
	m_nLife	= 0;			// 寿命
}

//============================================================
//	デストラクタ
//============================================================
CParticle3D::~CParticle3D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CParticle3D::Init(void)
{
	// メンバ変数を初期化
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_col	= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 色
	m_type	= TYPE_NONE;	// 種類
	m_nLife	= 0;			// 寿命

	// 成功を返す
	return S_OK;
}

//============================================================
//	初期化処理
//============================================================
HRESULT CParticle3D::Init(const TYPE type, const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// メンバ変数を初期化
	m_pos	= rPos;				// 位置
	m_col	= rCol;				// 色
	m_type	= type;				// 種類
	m_nLife	= mc_aLife[type];	// 寿命

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CParticle3D::Uninit(void)
{
	// パーティクル3Dオブジェクトを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CParticle3D::Update(void)
{
	if (m_nLife > 0)
	{ // 寿命がある場合

		// 寿命を減算
		m_nLife--;
	}
	else
	{ // 寿命がない場合

		// パーティクル3Dオブジェクトの終了
		Uninit();

		// 関数を抜ける
		return;
	}

	switch (m_type)
	{ // 種類ごとの処理
	case TYPE_DAMAGE:

		// ダメージパーティクル3D
		Damage(m_pos, m_col);

		break;

	default:	// 例外処理
		assert(false);
		break;
	}
}

//============================================================
//	描画処理
//============================================================
void CParticle3D::Draw(void)
{

}

//============================================================
//	ダメージパーティクル3D処理
//============================================================
void CParticle3D::Damage(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// 変数を宣言
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量の代入用
	D3DXVECTOR3 rot  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向きの代入用

	if ((m_nLife + 1) % 12 == 0)
	{ // 寿命が12の倍数の場合

		for (int nCntPart = 0; nCntPart < DMG_SPAWN; nCntPart++)
		{ // 生成されるエフェクト数分繰り返す

			// ベクトルをランダムに設定
			move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
			move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
			move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

			// ベクトルを正規化
			D3DXVec3Normalize(&move, &move);

			// 移動量を設定
			move.x *= DMG_MOVE;
			move.y *= DMG_MOVE;
			move.z *= DMG_MOVE;

			// 向きを設定
			rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// エフェクト3Dオブジェクトの生成
			CEffect3D::Create
			( // 引数
				CEffect3D::TYPE_NORMAL,	// テクスチャ
				rPos,	// 位置
				move,	// 移動量
				rot,	// 向き
				rCol,	// 色
				DMG_EFF_LIFE,		// 寿命
				DMG_SIZE,			// 半径
				DMG_SUB_SIZE,		// 半径の減算量
				DMG_SUB_ALPHA,		// 透明度の減算量
				LABEL_PARTICLE3D	// オブジェクトラベル
			);
		}
	}
}

//============================================================
//	生成処理
//============================================================
CParticle3D *CParticle3D::Create(const TYPE type, const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// ポインタを宣言
	CParticle3D *pParticle3D = NULL;	// パーティクル3D生成用

	if (UNUSED(pParticle3D))
	{ // 使用されていない場合

		// メモリ確保
		pParticle3D = new CParticle3D;	// パーティクル3D
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pParticle3D))
	{ // 確保に成功している場合

		if (SUCCEEDED(pParticle3D->GetResult()))
		{ // オブジェクトの情報設定に成功した場合

			// パーティクル3Dの初期化
			if (FAILED(pParticle3D->Init(type, rPos, rCol)))
			{ // 初期化に失敗した場合

				// メモリ開放
				delete pParticle3D;
				pParticle3D = NULL;

				// 失敗を返す
				return NULL;
			}

			// 確保したアドレスを返す
			return pParticle3D;
		}
		else
		{ // オブジェクトの情報設定に失敗した場合

			// メモリ開放
			delete pParticle3D;
			pParticle3D = NULL;

			// 失敗を返す
			return NULL;
		}
	}
	else { assert(false); return NULL; }	// 確保失敗
}
