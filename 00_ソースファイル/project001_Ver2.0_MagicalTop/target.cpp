//============================================================
//
//	ターゲット処理 [target.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "target.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "objectMeshCube.h"
#include "lifeGauge3D.h"
#include "shadow.h"
#include "flower.h"
#include "particle3D.h"
#include "debugproc.h"

//************************************************************
//	マクロ定義
//************************************************************
#define TARG_SHADOW_SIZE	(D3DXVECTOR3(200.0f, 0.0f, 200.0f))	// 影の大きさ

#define CUBE_SIZE	(D3DXVECTOR3(25.0f, 25.0f, 25.0f))	// キューブの大きさ
#define CUBE_BORD	(2.5f)		// キューブの縁取りの太さ

#define TARG_RADIUS	(100.0f)	// ターゲット半径
#define ADD_POS_Y	(92.0f)		// 台座位置からのキューブ相対位置の加算量
#define MUL_SIN_POS	(10.0f)		// サインカーブの補正係数

#define ADD_SIN_ROT	(D3DXToRadian(1))	// 浮遊向きの加算量
#define ADD_CUBEROT	(D3DXVECTOR3(0.01f, 0.001f, 0.01f))	// キューブの回転量

#define TARG_LIFE	(1000)		// ターゲットの体力
#define TARG_POSUP	(160.0f)	// ターゲットのY位置の加算量
#define TARG_DMG_FRAME	(20)	// ターゲットのダメージ状態フレーム

#define STATE_HEAL_CNT	(240)	// 回復状態に移行するまでのカウンター
#define NORMAL_CNT		(60)	// 通常状態に移行するまでのカウンター
#define WAIT_HEAL_CNT	(60)	// 回復までのカウンター

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CTarget::mc_apModelFile[] =	// モデル定数
{
	"data\\MODEL\\TARGET\\pedestal000.x",	// 台座モデル
};

//************************************************************
//	子クラス [CTarget] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTarget::CTarget() : CObjectModel(CObject::LABEL_TARGET)
{
	// メンバ変数をクリア
	m_pMeshCube		= NULL;			// メッシュキューブの情報
	m_pLifeGauge	= NULL;			// 体力の情報
	m_pShadow		= NULL;			// 影の情報
	m_state			= STATE_NORMAL;	// 状態
	m_fSinRot		= 0.0f;			// 浮遊向き
	m_nCounterState	= 0;			// 状態管理カウンター
	m_nCounterHeal	= 0;			// 回復管理カウンター
}

//============================================================
//	デストラクタ
//============================================================
CTarget::~CTarget()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTarget::Init(void)
{
	// メンバ変数を初期化
	m_pMeshCube		= NULL;			// メッシュキューブの情報
	m_pLifeGauge	= NULL;			// 体力の情報
	m_pShadow		= NULL;			// 影の情報
	m_state			= STATE_NORMAL;	// 状態
	m_fSinRot		= 0.0f;			// 浮遊向き
	m_nCounterState	= 0;			// 状態管理カウンター
	m_nCounterHeal	= 0;			// 回復管理カウンター

	// オブジェクトモデルの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// メッシュキューブの生成
	m_pMeshCube = CObjectMeshCube::Create
	( // 引数
		VEC3_ZERO,	// 位置
		VEC3_ZERO,	// 向き
		CUBE_SIZE,	// 大きさ
		XCOL_WHITE,	// キューブ色
		XCOL_BLACK,	// 縁取り色
		CObjectMeshCube::BORDERSTATE_ON,	// 縁取り状態
		CUBE_BORD,							// 縁取り太さ
		CObjectMeshCube::TEXSTATE_ONE,		// テクスチャ状態
		CObjectMeshCube::FACETEX(-1),		// テクスチャ種類
		VEC2_ONE,		// テクスチャ分割数 x
		VEC2_ONE,		// テクスチャ分割数 y
		VEC2_ONE,		// テクスチャ分割数 z
		D3DCULL_CCW,	// カリング状況
		false			// ライティング状況
	);
	if (UNUSED(m_pMeshCube))
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 体力ゲージ3Dの生成
	m_pLifeGauge = CLifeGauge3D::Create(TARG_LIFE, TARG_LIFE, (int)(TARG_DMG_FRAME * 0.5f), TARG_POSUP, this);
	if (UNUSED(m_pLifeGauge))
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 影の生成
	m_pShadow = CShadow::Create(CShadow::TEXTURE_NORMAL, TARG_SHADOW_SIZE, this);
	if (UNUSED(m_pShadow))
	{ // 生成に失敗した場合

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
void CTarget::Uninit(void)
{
	// オブジェクトメッシュキューブを破棄
	m_pMeshCube->Uninit();

	// 体力ゲージ3Dを破棄
	m_pLifeGauge->Uninit();

	// 影を破棄
	m_pShadow->Uninit();

	// オブジェクトモデルの終了
	CObjectModel::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CTarget::Update(void)
{
	// 変数を宣言
	D3DXVECTOR3 pos = GetPosition();				// 台座位置
	D3DXVECTOR3 rot = m_pMeshCube->GetRotation();	// キューブ向き
	int nNumFlower = CFlower::GetNumAll();			// マナフラワーの総数

	if (m_state == STATE_DESTROY)
	{ // 破壊されていない場合

		// 処理を抜ける
		return;
	}

	// 状態管理
	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NORMAL:	// 通常状態

		if (m_pLifeGauge->GetLife() < TARG_LIFE && nNumFlower > 0)
		{ // 体力が減少している且つ、マナフラワーが一本でも生えている場合

			if (m_nCounterState < STATE_HEAL_CNT)
			{ // カウンターが一定値より小さい場合

				// カウンターを加算
				m_nCounterState++;
			}
			else
			{ // カウンターが一定値以上の場合

				// カウンターを初期化
				m_nCounterState = 0;

				// 状態を変更
				m_state = STATE_HEAL;	// 回復状態
			}
		}

		break;

	case STATE_DAMAGE:	// ダメージ状態

		if (m_nCounterState < NORMAL_CNT)
		{ // カウンターが一定値より小さい場合

			// カウンターを加算
			m_nCounterState++;
		}
		else
		{ // カウンターが一定値以上の場合

			// カウンターを初期化
			m_nCounterState = 0;

			// 状態を変更
			m_state = STATE_NORMAL;	// 通常状態
		}

		break;

	case STATE_HEAL:	// 回復状態

		if (m_pLifeGauge->GetLife() < TARG_LIFE)
		{ // 体力が最大値より少ない場合

			if (m_nCounterHeal < WAIT_HEAL_CNT)
			{ // カウンターが一定値より小さい場合

				// カウンターを加算
				m_nCounterHeal++;
			}
			else
			{ // カウンターが一定値以上の場合

				// カウンターを初期化
				m_nCounterHeal = 0;

				// 体力を回復
				m_pLifeGauge->AddLife(nNumFlower);	// マナフラワー量に応じて回復量増加
			}
		}
		else
		{ // 体力が全回復した場合

			// 状態を変更
			m_state = STATE_NORMAL;	// 通常状態
		}

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// キューブの縦位置を変更
	pos.y += ADD_POS_Y + sinf(m_fSinRot) * MUL_SIN_POS;

	// キューブの向きを変更
	rot += ADD_CUBEROT;

	// キューブの位置・向きを反映
	m_pMeshCube->SetPosition(pos);
	m_pMeshCube->SetRotation(rot);

	// 浮遊向きを加算
	m_fSinRot += ADD_SIN_ROT;
	useful::NormalizeRot(m_fSinRot);	// 向き補正

	// メッシュキューブの更新
	m_pMeshCube->Update();

	// 体力ゲージ3Dの更新
	m_pLifeGauge->Update();

	// 影の更新
	m_pShadow->Update();

	// オブジェクトモデルの更新
	CObjectModel::Update();
}

//============================================================
//	描画処理
//============================================================
void CTarget::Draw(void)
{
	if (m_state != STATE_DESTROY)
	{ // 破壊されていない場合

		// オブジェクトモデルの描画
		CObjectModel::Draw();
	}
}

//============================================================
//	ヒット処理
//============================================================
void CTarget::Hit(const int nDmg)
{
	// 体力からダメージ分減算
	m_pLifeGauge->AddLife(-nDmg);

	if (m_pLifeGauge->GetLife() > 0)
	{ // 生きている場合

		// パーティクル3Dオブジェクトを生成
		CParticle3D::Create(CParticle3D::TYPE_DAMAGE, GetPosition());

		// カウンターを初期化
		m_nCounterState = 0;

		// 状態を変更
		m_state = STATE_DAMAGE;		// ダメージ状態
	}
	else
	{ // 死んでいる場合

		// パーティクル3Dオブジェクトを生成
		CParticle3D::Create(CParticle3D::TYPE_DAMAGE, GetPosition(), D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f));
		CParticle3D::Create(CParticle3D::TYPE_DAMAGE, GetPosition(), D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f));

		// カウンターを初期化
		m_nCounterState = 0;

		// 状態を変更
		m_state = STATE_DESTROY;	// 破壊状態
	}
}

//============================================================
//	生成処理
//============================================================
CTarget *CTarget::Create(const MODEL model, const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot, const D3DXVECTOR3& rScale)
{
	// 変数を宣言
	int nModelID;	// モデルインデックス

	// ポインタを宣言
	CModel *pModel = CManager::GetModel();	// モデルへのポインタ
	CTarget *pTarget = NULL;	// ターゲット生成用

	if (UNUSED(pTarget))
	{ // 使用されていない場合

		// メモリ確保
		pTarget = new CTarget;	// ターゲット
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pTarget))
	{ // 使用されている場合
		
		// ターゲットの初期化
		if (FAILED(pTarget->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pTarget;
			pTarget = NULL;

			// 失敗を返す
			return NULL;
		}

		// モデルを登録
		nModelID = pModel->Regist(mc_apModelFile[model]);

		// モデルを割当
		pTarget->BindModel(pModel->GetModel(nModelID));

		// 位置を設定
		pTarget->SetPosition(rPos);

		// 向きを設定
		pTarget->SetRotation(rRot);

		// 大きさを設定
		pTarget->SetScaling(rScale);

		// メッシュキューブの位置を設定
		pTarget->m_pMeshCube->SetPosition(D3DXVECTOR3(rPos.x, rPos.y + ADD_POS_Y, rPos.z));

		// 確保したアドレスを返す
		return pTarget;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	状態取得処理
//============================================================
int CTarget::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//============================================================
//	半径取得処理
//============================================================
float CTarget::GetRadius(void) const
{
	// 半径を返す
	return TARG_RADIUS;
}
