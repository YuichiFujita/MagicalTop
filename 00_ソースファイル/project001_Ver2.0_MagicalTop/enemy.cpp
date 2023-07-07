//============================================================
//
//	敵処理 [enemy.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "multiModel.h"
#include "debugproc.h"
#include "collision.h"
#include "player.h"
#include "target.h"
#include "bullet.h"
#include "stage.h"
#include "score.h"
#include "particle3D.h"
#include "field.h"

//************************************************************
//	マクロ定義
//************************************************************
#define ENEMY_SETUP_TXT	"data\\TXT\\enemy.txt"	// セットアップテキスト相対パス

#define ENE_REV		(0.02f)	// プレイヤー移動量の減衰係数
#define ENE_GRAVITY	(1.0f)	// プレイヤー重力

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CEnemy::StatusInfo CEnemy::m_aStatusInfo[CEnemy::TYPE_MAX] = {};	// ステータス情報
CEnemy::PartsInfo CEnemy::m_aPartsInfo[CEnemy::TYPE_MAX] = {};		// パーツ情報

const char *CEnemyCar::mc_apModelFile[] =	// 戦車モデル定数
{
	"data\\MODEL\\caterpillar000.x",	// キャタピラ
	"data\\MODEL\\cannon000.x",			// キャノン
};

//************************************************************
//	子クラス [CEnemy] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemy::CEnemy(const TYPE type) : CObject(CObject::LABEL_ENEMY), m_status(m_aStatusInfo[type]), m_parts(m_aPartsInfo[type])
{
	// メンバ変数をクリア
	memset(&m_apMultiModel[0], 0, sizeof(m_apMultiModel));	// モデルの情報
	m_nNumModel = 0;	// パーツの総数
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));	// ワールドマトリックス
	m_pos		= VEC3_ZERO;	// 現在位置
	m_oldPos	= VEC3_ZERO;	// 過去位置
	m_movePos	= VEC3_ZERO;	// 位置移動量
	m_rot		= VEC3_ZERO;	// 向き
	m_moveRot	= VEC3_ZERO;	// 向き変更量
	m_nLife		= 0;			// 体力
	m_nCounterAtk = 0;			// 攻撃管理カウンター
}

//============================================================
//	デストラクタ
//============================================================
CEnemy::~CEnemy()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemy::Init(void)
{
	// 変数を宣言
	int nModelID;	// モデルインデックス

	// ポインタを宣言
	CModel *pModel = CManager::GetModel();	// モデルへのポインタ

	// メンバ変数を初期化
	memset(&m_apMultiModel[0], 0, sizeof(m_apMultiModel));	// モデルの情報
	m_nNumModel = 0;	// パーツの総数
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));	// ワールドマトリックス
	m_pos		= VEC3_ZERO;		// 現在位置
	m_oldPos	= VEC3_ZERO;		// 過去位置
	m_movePos	= VEC3_ZERO;		// 位置移動量
	m_rot		= VEC3_ZERO;		// 向き
	m_moveRot	= VEC3_ZERO;		// 向き変更量
	m_nLife		= m_status.nLife;	// 体力
	m_nCounterAtk = 0;				// 攻撃管理カウンター

	// パーツ数を代入
	m_nNumModel = m_parts.nNumParts;

	for (int nCntEnemy = 0; nCntEnemy < m_nNumModel; nCntEnemy++)
	{ // パーツ数分繰り返す

		// モデルの生成
		m_apMultiModel[nCntEnemy] = CMultiModel::Create(m_parts.aInfo[nCntEnemy].pos, m_parts.aInfo[nCntEnemy].rot);

		// モデルの登録・割当
		nModelID = pModel->Regist(GetModelFileName(nCntEnemy));
		m_apMultiModel[nCntEnemy]->BindModel(pModel->GetModel(nModelID));

		// 親モデルの設定
		if (m_parts.aInfo[nCntEnemy].nParentID == NONE_IDX)
		{ // 親がない場合

			// NULLを設定
			m_apMultiModel[nCntEnemy]->SetParent(NULL);
		}
		else
		{ // 親がいる場合

			// 親のアドレスを設定
			m_apMultiModel[nCntEnemy]->SetParent(m_apMultiModel[m_parts.aInfo[nCntEnemy].nParentID]);
		}
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CEnemy::Uninit(void)
{
	// マルチモデルを破棄
	for (int nCntEnemy = 0; nCntEnemy < MAX_PARTS; nCntEnemy++)
	{ // パーツの最大数分繰り返す

		if (USED(m_apMultiModel[nCntEnemy]))
		{ // パーツが使用中の場合

			// 破棄処理
			if (FAILED(m_apMultiModel[nCntEnemy]->Release(m_apMultiModel[nCntEnemy])))
			{ // 破棄に失敗した場合

				// 例外処理
				assert(false);
			}
		}
	}

	// 敵を破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CEnemy::Update(void)
{
	// 過去位置を更新
	m_oldPos = m_pos;
}

//============================================================
//	描画処理
//============================================================
void CEnemy::Draw(void)
{
	// 変数を宣言
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスのポインタ

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

	for (int nCntEnemy = 0; nCntEnemy < m_nNumModel; nCntEnemy++)
	{ // パーツの総数分繰り返す

		// パーツの描画
		m_apMultiModel[nCntEnemy]->Draw();
	}
}

//============================================================
//	ヒット処理
//============================================================
void CEnemy::Hit(const int nDmg)
{
	// 体力からダメージ分減算
	m_nLife -= nDmg;

	if (m_nLife > 0)
	{ // 生きている場合

		// パーティクル3Dオブジェクトを生成
		CParticle3D::Create(CParticle3D::TYPE_DAMAGE, m_pos);
	}
	else
	{ // 死んでいる場合

		// パーティクル3Dオブジェクトを生成
		CParticle3D::Create(CParticle3D::TYPE_DAMAGE, m_pos, D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f));
		CParticle3D::Create(CParticle3D::TYPE_DAMAGE, m_pos, D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f));

		// スコアを加算
		CManager::GetScore()->Add(m_status.nScore);

		// 敵オブジェクトの終了
		Uninit();
	}
}

//============================================================
//	生成処理
//============================================================
CEnemy *CEnemy::Create(const TYPE type, const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot)
{
	// ポインタを宣言
	CEnemy *pEnemy = NULL;	// 敵生成用

	if (UNUSED(pEnemy))
	{ // 使用されていない場合

		// メモリ確保
		switch (type)
		{ // 種類ごとの処理
		case TYPE_CAR:	// 戦車

			// 戦車を生成
			pEnemy = new CEnemyCar(type);

			break;

		default:	// 例外処理
			assert(false);
			break;
		}
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pEnemy))
	{ // 使用されている場合
		
		if (SUCCEEDED(pEnemy->GetResult()))
		{ // オブジェクトの情報設定に成功した場合

			// 敵の初期化
			if (FAILED(pEnemy->Init()))
			{ // 初期化に失敗した場合

				// メモリ開放
				delete pEnemy;
				pEnemy = NULL;

				// 失敗を返す
				return NULL;
			}

			// 位置を設定
			pEnemy->SetPosition(rPos);

			// 向きを設定
			pEnemy->SetRotation(rRot);

			// 確保したアドレスを返す
			return pEnemy;
		}
		else
		{ // オブジェクトの情報設定に失敗した場合

			// メモリ開放
			delete pEnemy;
			pEnemy = NULL;

			// 失敗を返す
			return NULL;
		}
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	位置の設定処理
//============================================================
void CEnemy::SetPosition(const D3DXVECTOR3& rPos)
{
	// 引数の位置を設定
	m_pos = rPos;
}

//============================================================
//	位置移動量の設定処理
//============================================================
void CEnemy::SetMovePosition(const D3DXVECTOR3& rMove)
{
	// 引数の位置移動量を設定
	m_movePos = rMove;
}

//============================================================
//	向きの設定処理
//============================================================
void CEnemy::SetRotation(const D3DXVECTOR3& rRot)
{
	// 引数の向きを設定
	m_rot = rRot;

	// 向きの正規化
	useful::NormalizeRot(m_rot.x);
	useful::NormalizeRot(m_rot.y);
	useful::NormalizeRot(m_rot.z);
}

//============================================================
//	向き変更量の設定処理
//============================================================
void CEnemy::SetMoveRotation(const D3DXVECTOR3& rMove)
{
	// 引数の向きを設定
	m_moveRot = rMove;

	// 向きの正規化
	useful::NormalizeRot(m_moveRot.x);
	useful::NormalizeRot(m_moveRot.y);
	useful::NormalizeRot(m_moveRot.z);
}

//============================================================
//	マトリックス取得処理
//============================================================
D3DXMATRIX CEnemy::GetMtxWorld(void) const
{
	// 変数を宣言
	D3DXMATRIX mtxRot, mtxTrans, mtxWorld;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ワールドマトリックスを返す
	return mtxWorld;
}

//============================================================
//	位置取得処理
//============================================================
D3DXVECTOR3 CEnemy::GetPosition(void) const
{
	// 位置を返す
	return m_pos;
}

//============================================================
//	過去位置取得処理
//============================================================
D3DXVECTOR3 CEnemy::GetOldPosition(void) const
{
	// 過去位置を返す
	return m_oldPos;
}

//============================================================
//	位置移動量取得処理
//============================================================
D3DXVECTOR3 CEnemy::GetMovePosition(void) const
{
	// 位置移動量を返す
	return m_movePos;
}

//============================================================
//	向き取得処理
//============================================================
D3DXVECTOR3 CEnemy::GetRotation(void) const
{
	// 向きを返す
	return m_rot;
}

//============================================================
//	向き変更量取得処理
//============================================================
D3DXVECTOR3 CEnemy::GetMoveRotation(void) const
{
	// 向き変更量を返す
	return m_moveRot;
}

//============================================================
//	半径取得処理
//============================================================
float CEnemy::GetRadius(void) const
{
	// 半径を返す
	return m_status.fRadius;
}

//============================================================
//	マルチモデル取得処理
//============================================================
CMultiModel *CEnemy::GetMultiModel(const int nID) const
{
	if (nID < m_nNumModel)
	{ // 使用可能なインデックスの場合

		// マルチモデルの情報を返す
		return m_apMultiModel[nID];
	}
	else { assert(false); return m_apMultiModel[0]; }
}

//============================================================
//	ステータス情報取得処理
//============================================================
CEnemy::StatusInfo CEnemy::GetStatusInfo(void) const
{
	// ステータス情報を返す
	return m_status;
}

//============================================================
//	パーツ情報取得処理
//============================================================
CEnemy::PartsInfo CEnemy::GetPartsInfo(void) const
{
	// パーツ情報を返す
	return m_parts;
}

//============================================================
//	検知範囲の当たり判定
//============================================================
void CEnemy::CollisionFind(void)
{
	// 変数を宣言
	D3DXVECTOR3 posLook;	// 視認対象位置
	float fPlayerRadius = CManager::GetPlayer()->GetRadius();	// プレイヤー半径

	// TODO：プレイヤー死んだらManagerのPlayerもNULLにする
	if (USED(CManager::GetPlayer()) && USED(CManager::GetTarget()))
	{ // プレイヤー・ターゲットが使用されている場合

		// 視認対象の設定
		if (collision::Circle2D(CManager::GetPlayer()->GetPosition(), m_pos, fPlayerRadius, m_status.fFindRadius) == false)
		{ // 敵の検知範囲外の場合

			// 視認対象位置を設定
			posLook = CManager::GetTarget()->GetPosition();	// ターゲット位置
		}
		else
		{ // 敵の検知範囲内の場合

			// 視認対象位置を設定
			posLook = CManager::GetPlayer()->GetPosition();	// プレイヤー位置
		}

		// 対象の方向を向かせる
		Look(posLook);

		if (collision::Circle2D(posLook, m_pos, fPlayerRadius, m_status.fAttackRadius) == false)
		{ // 敵の攻撃範囲外の場合

			// 対象の方向に移動 (前進)
			m_pos.x -= sinf(m_rot.y) * m_status.fForwardMove;
			m_pos.z -= cosf(m_rot.y) * m_status.fForwardMove;

			// ステージ範囲外の補正
			CManager::GetStage()->LimitPosition(m_pos, m_status.fRadius);
		}
		else
		{ // 敵の攻撃範囲内の場合

			if (collision::Circle2D(posLook, m_pos, fPlayerRadius, m_status.fBackwardRadius) == true && m_status.bBackward == true)
			{ // 敵の後退範囲内且つ、後退がONの場合

				// 対象の逆方向に移動 (後退)
				m_pos.x += sinf(m_rot.y) * m_status.fBackwardMove;
				m_pos.z += cosf(m_rot.y) * m_status.fBackwardMove;

				// ステージ範囲外の補正
				CManager::GetStage()->LimitPosition(m_pos, m_status.fRadius);
			}

			// 攻撃
			Attack(D3DXVECTOR3(m_rot.x + (-D3DX_PI * 0.5f), m_rot.y, 0.0f));
		}
	}
}

//============================================================
//	対象視認処理
//============================================================
void CEnemy::Look(const D3DXVECTOR3& rPos)
{
	// 変数を宣言
	float fDestRot = 0.0f;	// 目標向き
	float fDiffRot = 0.0f;	// 向き

	// プレイヤーの向きを代入
	fDestRot = atan2f(m_pos.x - rPos.x, m_pos.z - rPos.z);	// 目標向き

	// 目標向きまでの差分を計算
	fDiffRot = fDestRot - m_rot.y;

	// 差分向きの正規化
	useful::NormalizeRot(fDiffRot);

	// 向きの更新
	m_rot.y += fDiffRot * m_status.fLookRevision;

	// 向きの正規化
	useful::NormalizeRot(m_rot.y);
}

//============================================================
//	攻撃処理
//============================================================
void CEnemy::Attack(const D3DXVECTOR3& rRot)
{
	// カウンターを加算
	m_nCounterAtk++;

	if (m_nCounterAtk >= m_status.nCounterAttack)
	{ // カウンターが一定値以上の場合

		// 変数を宣言
		D3DXMATRIX mtx = m_apMultiModel[m_status.nBullParts]->GetMtxWorld();			// 発射パーツのマトリックス
		D3DXVECTOR3 pos = D3DXVECTOR3(mtx._41, mtx._42, mtx._43) + m_status.bullPos;	// 発射位置

		// カウンターを初期化
		m_nCounterAtk = 0;

		// 弾オブジェクトの生成
		CBullet::Create
		( // 引数
			CBullet::TYPE_ENEMY,			// 種類
			pos,							// 位置
			VEC3_ALL(m_status.fBullRadius),	// 大きさ
			XCOL_WHITE,						// 色
			rRot,							// 射撃向き
			m_status.fBullMove,				// 移動速度
			m_status.nBullLife				// 寿命
		);
	}
}

//============================================================
//	ターゲットとの当たり判定
//============================================================
void CEnemy::CollisionTarget(D3DXVECTOR3& rPos, D3DXVECTOR3& rPosOld)
{
	// ポインタを宣言
	CTarget *pTarget = CManager::GetTarget();	// ターゲット情報

	if (USED(pTarget))
	{ // ターゲットが使用されている場合

		// 変数を宣言
		D3DXVECTOR3 posTarget = pTarget->GetPosition();
		D3DXVECTOR3 sizeTarget = VEC3_ALL(pTarget->GetRadius());
		D3DXVECTOR3 sizeEnemy = VEC3_ALL(m_status.fRadius);

		// ターゲットとの衝突判定
		collision::BoxPillar
		( // 引数
			rPos,		// 判定位置
			rPosOld,	// 判定過去位置
			posTarget,	// 判定目標位置
			sizeEnemy,	// 判定サイズ(右・上・後)
			sizeEnemy,	// 判定サイズ(左・下・前)
			sizeTarget,	// 判定目標サイズ(右・上・後)
			sizeTarget	// 判定目標サイズ(左・下・前)
		);
	}
}

//============================================================
//	敵との当たり判定
//============================================================
void CEnemy::CollisionEnemy(D3DXVECTOR3& rPos)
{
	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // 優先順位の総数分繰り返す

		for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
		{ // オブジェクトの総数分繰り返す

			// ポインタを宣言
			CObject *pObject = CObject::GetObject(nCntPri, nCntObject);	// オブジェクト

			if (UNUSED(pObject)
			||  pObject->GetLabel() != CObject::LABEL_ENEMY)
			{ // オブジェクトが非使用中・ラベルが敵ではない場合

				// 次の繰り返しに移行
				continue;
			}

			if (pObject->GetID() == GetID()
			&&  pObject->GetPriority() == GetPriority())
			{ // オブジェクトが自分自身だった場合

				// 次の繰り返しに移行
				continue;
			}

			// ターゲットとの衝突判定
			collision::CirclePillar
			( // 引数
				rPos,					// 判定位置
				pObject->GetPosition(),	// 判定目標位置
				m_status.fRadius,		// 判定半径
				pObject->GetRadius()	// 判定目標半径
			);
		}
	}
}

//************************************************************
//	子クラス [CEnemyCar] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyCar::CEnemyCar(const TYPE type) : CEnemy(type)
{

}

//============================================================
//	デストラクタ
//============================================================
CEnemyCar::~CEnemyCar()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemyCar::Init(void)
{
	// 敵の初期化
	if (FAILED(CEnemy::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CEnemyCar::Uninit(void)
{
	// 敵の終了
	CEnemy::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEnemyCar::Update(void)
{
	// 敵の更新
	CEnemy::Update();

	// 敵の動作の更新
	CollisionFind();
}

//============================================================
//	描画処理
//============================================================
void CEnemyCar::Draw(void)
{
	// 敵の描画
	CEnemy::Draw();
}

//============================================================
//	モデルファイル取得処理
//============================================================
const char* CEnemyCar::GetModelFileName(const int nModel) const
{
	if (nModel < MODEL_MAX)
	{ // 使用できるインデックスの場合

		// 引数のインデックスのモデルを返す
		return mc_apModelFile[nModel];
	}
	else { assert(false); return "\0"; }	// 範囲外
}

//============================================================
//	検知範囲の当たり判定
//============================================================
void CEnemyCar::CollisionFind(void)
{
	// 変数を宣言
	StatusInfo  status		= GetStatusInfo();		// 敵ステータス
	D3DXVECTOR3 posEnemy	= GetPosition();		// 敵位置
	D3DXVECTOR3 moveEnemy	= GetMovePosition();	// 敵移動量
	D3DXVECTOR3 rotEnemy	= GetRotation();		// 敵向き
	D3DXVECTOR3 posLook		= VEC3_ZERO;			// 視認対象位置
	D3DXVECTOR3 rotCannon	= VEC3_ZERO;			// キャノン向き
	float fPlayerRadius = CManager::GetPlayer()->GetRadius();	// プレイヤー半径

	// TODO：移動の確認・移動量、向き変更量を使う
	if (USED(CManager::GetPlayer()) && USED(CManager::GetTarget()))
	{ // プレイヤー・ターゲットが使用されている場合

		// 視認対象の検知判定
		if (collision::Circle2D(CManager::GetPlayer()->GetPosition(), posEnemy, fPlayerRadius, status.fFindRadius) == false)
		{ // 敵の検知範囲外の場合

			// 視認対象位置を設定
			posLook = CManager::GetTarget()->GetPosition();	// ターゲット位置
		}
		else
		{ // 敵の検知範囲内の場合

			// 視認対象位置を設定
			posLook = CManager::GetPlayer()->GetPosition();	// プレイヤー位置
		}

		// 視認対象の攻撃判定
		if (collision::Circle2D(posLook, posEnemy, fPlayerRadius, status.fAttackRadius) == false)
		{ // 敵の攻撃範囲外の場合

			// 対象の方向を向かせる
			Look(posLook);

			// 向きを取得
			rotEnemy = GetRotation();

			// 対象の方向に移動 (前進)
			moveEnemy.x -= sinf(rotEnemy.y) * status.fForwardMove;
			moveEnemy.z -= cosf(rotEnemy.y) * status.fForwardMove;

			// 重力を加算
			moveEnemy.y -= ENE_GRAVITY;

			// 移動量を加算
			posEnemy += moveEnemy;

			// 移動量を減衰
			moveEnemy.x += (0.0f - moveEnemy.x) * ENE_REV;
			moveEnemy.z += (0.0f - moveEnemy.z) * ENE_REV;

			// ターゲットとの当たり判定
			CollisionTarget(posEnemy, GetOldPosition());

			// 敵との当たり判定
			CollisionEnemy(posEnemy);

			// 着地判定
			CManager::GetField()->LandPosition(posEnemy, moveEnemy);

			// ステージ範囲外の補正
			CManager::GetStage()->LimitPosition(posEnemy, status.fRadius);
		}
		else
		{ // 敵の攻撃範囲内の場合

			// キャノン向きの設定
			SetRotationCannon(posLook, rotCannon);

			if (collision::Circle2D(posLook, posEnemy, fPlayerRadius, status.fBackwardRadius) == true && status.bBackward == true)
			{ // 敵の後退範囲内且つ、後退がONの場合

				// 対象の方向を向かせる
				Look(posLook);

				// 向きを取得
				rotEnemy = GetRotation();

				// 対象の逆方向に移動 (後退)
				moveEnemy.x += sinf(rotEnemy.y) * status.fBackwardMove;
				moveEnemy.z += cosf(rotEnemy.y) * status.fBackwardMove;

				// 重力を加算
				moveEnemy.y -= ENE_GRAVITY;

				// 移動量を加算
				posEnemy += moveEnemy;

				// 移動量を減衰
				moveEnemy.x += (0.0f - moveEnemy.x) * ENE_REV;
				moveEnemy.z += (0.0f - moveEnemy.z) * ENE_REV;
			}

			// ターゲットとの当たり判定
			CollisionTarget(posEnemy, GetOldPosition());

			// 敵との当たり判定
			CollisionEnemy(posEnemy);

			// 着地判定
			CManager::GetField()->LandPosition(posEnemy, moveEnemy);

			// ステージ範囲外の補正
			CManager::GetStage()->LimitPosition(posEnemy, status.fRadius);

			// 攻撃
			Attack
			( // 引数
				D3DXVECTOR3	// 弾の発射向き
				( // 引数
					rotCannon.x + (-D3DX_PI * 0.5f),	// x
					rotCannon.y,						// y
					0.0f								// z
				)
			);
		}
	}

	// 位置を反映
	SetPosition(posEnemy);

	// 位置移動量を反映
	SetMovePosition(moveEnemy);

	// 向きを反映
	SetRotation(rotEnemy);
}

//============================================================
//	キャノン向きの設定処理
//============================================================
void CEnemyCar::SetRotationCannon(const D3DXVECTOR3& rLookPos, D3DXVECTOR3& rRotCannon)
{
	// 変数を宣言
	D3DXVECTOR3 posEnemy = GetPosition();	// 敵位置
	float fDestRot = 0.0f;	// 目標向き
	float fDiffRot = 0.0f;	// 向き

	// キャノンの向きを取得
	rRotCannon = GetMultiModel(MODEL_CANNON)->GetRotation() + GetRotation();	// 本体の向きを加算
	useful::NormalizeRot(rRotCannon.y);	// キャノン向きの正規化

	// 目標向きの計算
	fDestRot = atan2f(posEnemy.x - rLookPos.x, posEnemy.z - rLookPos.z);
	useful::NormalizeRot(fDestRot);		// 目標向きの正規化

	// 差分向きの計算
	fDiffRot = fDestRot - rRotCannon.y;
	useful::NormalizeRot(fDiffRot);		// 差分向きの正規化

	// 向きの更新
	rRotCannon.y += fDiffRot * 0.03f;	// TODO：cannonの向き補正値作る
	useful::NormalizeRot(rRotCannon.y);	// キャノン向きの正規化

	// 向きを設定
	GetMultiModel(MODEL_CANNON)->SetRotation(rRotCannon - GetRotation());		// 本体の向きを減算
}

//************************************************************
//	子クラス [CEnemy] のセットアップ関数
//************************************************************
//============================================================
//	セットアップ処理
//============================================================
void CEnemy::LoadSetup(void)
{
	// 変数を宣言
	int nType		= 0;	// 種類の代入用
	int nID			= 0;	// インデックスの代入用
	int nBackward	= 0;	// 後退のON/OFFの変換用
	int nEnd		= 0;	// テキスト読み込み終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;	// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(ENEMY_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			// キャラクターの設定
			if (strcmp(&aString[0], "CHARACTERSET") == 0)
			{ // 読み込んだ文字列が CHARACTERSET の場合

				do
				{ // 読み込んだ文字列が END_CHARACTERSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "ENEMYSET") == 0)
					{ // 読み込んだ文字列が ENEMYSET の場合

						do
						{ // 読み込んだ文字列が END_ENEMYSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "TYPE") == 0)
							{ // 読み込んだ文字列が TYPE の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nType);		// 種類を読み込む
							}
							else if (strcmp(&aString[0], "PARTSSET") == 0)
							{ // 読み込んだ文字列が PARTSSET の場合

								do
								{ // 読み込んだ文字列が END_PARTSSET ではない場合ループ

									// ファイルから文字列を読み込む
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "INDEX") == 0)
									{ // 読み込んだ文字列が INDEX の場合

										fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
										fscanf(pFile, "%d", &nID);			// モデルのインデックスを読み込む

										// パーツ数を加算
										m_aPartsInfo[nType].nNumParts++;
									}
									else if (strcmp(&aString[0], "PARENT") == 0)
									{ // 読み込んだ文字列が PARENT の場合

										fscanf(pFile, "%s", &aString[0]);								// = を読み込む (不要)
										fscanf(pFile, "%d", &m_aPartsInfo[nType].aInfo[nID].nParentID);	// モデルの親のインデックスを読み込む
									}
									else if (strcmp(&aString[0], "POS") == 0)
									{ // 読み込んだ文字列が POS の場合

										fscanf(pFile, "%s", &aString[0]);								// = を読み込む (不要)
										fscanf(pFile, "%f", &m_aPartsInfo[nType].aInfo[nID].pos.x);		// X座標を読み込む
										fscanf(pFile, "%f", &m_aPartsInfo[nType].aInfo[nID].pos.y);		// Y座標を読み込む
										fscanf(pFile, "%f", &m_aPartsInfo[nType].aInfo[nID].pos.z);		// Z座標を読み込む
									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{ // 読み込んだ文字列が ROT の場合

										fscanf(pFile, "%s", &aString[0]);								// = を読み込む (不要)
										fscanf(pFile, "%f", &m_aPartsInfo[nType].aInfo[nID].rot.x);		// X向きを読み込む
										fscanf(pFile, "%f", &m_aPartsInfo[nType].aInfo[nID].rot.y);		// Y向きを読み込む
										fscanf(pFile, "%f", &m_aPartsInfo[nType].aInfo[nID].rot.z);		// Z向きを読み込む
									}
								} while (strcmp(&aString[0], "END_PARTSSET") != 0);	// 読み込んだ文字列が END_PARTSSET ではない場合ループ
							}
						} while (strcmp(&aString[0], "END_ENEMYSET") != 0);	// 読み込んだ文字列が END_ENEMYSET ではない場合ループ
					}
				} while (strcmp(&aString[0], "END_CHARACTERSET") != 0);		// 読み込んだ文字列が END_CHARACTERSET ではない場合ループ
			}

			// ステータスの設定
			else if (strcmp(&aString[0], "STATUSSET") == 0)
			{ // 読み込んだ文字列が STATUSSET の場合

				do
				{ // 読み込んだ文字列が END_STATUSSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "ENEMYSET") == 0)
					{ // 読み込んだ文字列が ENEMYSET の場合

						do
						{ // 読み込んだ文字列が END_ENEMYSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "TYPE") == 0)
							{ // 読み込んだ文字列が TYPE の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nType);		// 種類を読み込む
							}
							else if (strcmp(&aString[0], "LIFE") == 0)
							{ // 読み込んだ文字列が LIFE の場合

								fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nLife);	// 体力を読み込む
							}
							else if (strcmp(&aString[0], "SCORE") == 0)
							{ // 読み込んだ文字列が SCORE の場合

								fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nScore);	// スコア加算量を読み込む
							}
							else if (strcmp(&aString[0], "RADIUS") == 0)
							{ // 読み込んだ文字列が RADIUS の場合

								fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fRadius);	// 半径を読み込む
							}
							else if (strcmp(&aString[0], "HEIGHT") == 0)
							{ // 読み込んだ文字列が HEIGHT の場合

								fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fHeight);	// 縦幅を読み込む
							}
							else if (strcmp(&aString[0], "BULLET_LIFE") == 0)
							{ // 読み込んだ文字列が BULLET_LIFE の場合

								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nBullLife);	// 弾の寿命を読み込む
							}
							else if (strcmp(&aString[0], "BULLET_MOVE") == 0)
							{ // 読み込んだ文字列が BULLET_MOVE の場合

								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fBullMove);	// 弾の移動量を読み込む
							}
							else if (strcmp(&aString[0], "BULLET_RADIUS") == 0)
							{ // 読み込んだ文字列が BULLET_RADIUS の場合

								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fBullRadius);	// 弾の半径を読み込む
							}
							else if (strcmp(&aString[0], "BULLET_PARTS") == 0)
							{ // 読み込んだ文字列が BULLET_PARTS の場合

								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nBullParts);	// 弾の発射パーツを読み込む
							}
							else if (strcmp(&aString[0], "BULLET_POS") == 0)
							{ // 読み込んだ文字列が BULLET_POS の場合

								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].bullPos.x);	// 弾の発射位置Xを読み込む
								fscanf(pFile, "%f", &m_aStatusInfo[nType].bullPos.y);	// 弾の発射位置Yを読み込む
								fscanf(pFile, "%f", &m_aStatusInfo[nType].bullPos.z);	// 弾の発射位置Zを読み込む
							}
							else if (strcmp(&aString[0], "ATTACK_CNT") == 0)
							{ // 読み込んだ文字列が ATTACK_CNT の場合

								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nCounterAttack);	// 攻撃頻度を読み込む
							}
							else if (strcmp(&aString[0], "FORWARD_MOVE") == 0)
							{ // 読み込んだ文字列が FORWARD_MOVE の場合

								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fForwardMove);	// 前進の移動量を読み込む
							}
							else if (strcmp(&aString[0], "BACKWARD_MOVE") == 0)
							{ // 読み込んだ文字列が BACKWARD_MOVE の場合

								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fBackwardMove);	// 後退の移動量を読み込む
							}
							else if (strcmp(&aString[0], "LOOK_REV") == 0)
							{ // 読み込んだ文字列が LOOK_REV の場合

								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fLookRevision);	// 振り向き補正係数を読み込む
							}
							else if (strcmp(&aString[0], "FIND_RADIUS") == 0)
							{ // 読み込んだ文字列が FIND_RADIUS の場合

								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fFindRadius);		// 検知範囲を読み込む
							}
							else if (strcmp(&aString[0], "ATTACK_RADIUS") == 0)
							{ // 読み込んだ文字列が ATTACK_RADIUS の場合

								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fAttackRadius);	// 攻撃範囲を読み込む
							}
							else if (strcmp(&aString[0], "BACKWARD_RADIUS") == 0)
							{ // 読み込んだ文字列が BACKWARD_RADIUS の場合

								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fBackwardRadius);	// 後退範囲を読み込む
							}
							else if (strcmp(&aString[0], "BACWARD_ENABLE") == 0)
							{ // 読み込んだ文字列が BACWARD_ENABLE の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nBackward);	// 後退のON/OFFを読み込む

								// 読み込んだ値をbool型に変換
								m_aStatusInfo[nType].bBackward = (nBackward == 0) ? true : false;
							}
						} while (strcmp(&aString[0], "END_ENEMYSET") != 0);	// 読み込んだ文字列が END_ENEMYSET ではない場合ループ
					}
				} while (strcmp(&aString[0], "END_STATUSSET") != 0);		// 読み込んだ文字列が END_STATUSSET ではない場合ループ
			}
		} while (nEnd != EOF);	// 読み込んだ文字列が EOF ではない場合ループ
		
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "敵セットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}
