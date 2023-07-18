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
#include "lifeGauge3D.h"
#include "shadow.h"
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

#define ENE_DMG_FRAME	(20)	// 敵のダメージ状態フレーム

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CEnemy::StatusInfo CEnemy::m_aStatusInfo[CEnemy::TYPE_MAX] = {};	// ステータス情報
CEnemy::PartsInfo CEnemy::m_aPartsInfo[CEnemy::TYPE_MAX] = {};		// パーツ情報

const char *CEnemyCar::mc_apModelFile[] =	// 戦車モデル定数
{
	"data\\MODEL\\ENEMY\\CAR\\caterpillar000.x",	// キャタピラ
	"data\\MODEL\\ENEMY\\CAR\\cannon000.x",			// キャノン
};

//************************************************************
//	子クラス [CEnemy] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemy::CEnemy(const TYPE type) : CObjectChara(CObject::LABEL_ENEMY), m_status(m_aStatusInfo[type]), m_parts(m_aPartsInfo[type])
{
	// メンバ変数をクリア
	m_pLifeGauge = NULL;	// 体力の情報
	m_pShadow = NULL;		// 影の情報
	m_oldPos  = VEC3_ZERO;	// 過去位置
	m_movePos = VEC3_ZERO;	// 位置移動量
	m_moveRot = VEC3_ZERO;	// 向き変更量
	m_nCounterAtk = 0;		// 攻撃管理カウンター
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
	// メンバ変数を初期化
	m_pLifeGauge = NULL;	// 体力の情報
	m_pShadow = NULL;		// 影の情報
	m_oldPos  = VEC3_ZERO;	// 過去位置
	m_movePos = VEC3_ZERO;	// 位置移動量
	m_moveRot = VEC3_ZERO;	// 向き変更量
	m_nCounterAtk = 0;		// 攻撃管理カウンター

	// 体力ゲージ3Dの生成
	m_pLifeGauge = CLifeGauge3D::Create(m_status.nLife, m_status.nLife, (int)(ENE_DMG_FRAME * 0.5f), m_status.fLifeUp, this);
	if (UNUSED(m_pLifeGauge))
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 影の生成
	m_pShadow = CShadow::Create(CShadow::TEXTURE_NORMAL, D3DXVECTOR3(300.0f, 0.0f, 300.0f), this);
	if (UNUSED(m_pShadow))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// オブジェクトキャラクターの初期化
	if (FAILED(CObjectChara::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	for (int nCntEnemy = 0; nCntEnemy < m_parts.nNumParts; nCntEnemy++)
	{ // パーツ数分繰り返す

		// パーツ情報の設定
		CObjectChara::SetPartsInfo
		( // 引数
			nCntEnemy,							// パーツインデックス
			m_parts.aInfo[nCntEnemy].nParentID,	// 親インデックス
			m_parts.aInfo[nCntEnemy].pos,		// 位置
			m_parts.aInfo[nCntEnemy].rot,		// 向き
			GetModelFileName(nCntEnemy)			// ファイル名
		);
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CEnemy::Uninit(void)
{
	// オブジェクトキャラクターの終了
	CObjectChara::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEnemy::Update(void)
{
	// 過去位置を更新
	m_oldPos = GetPosition();

	// オブジェクトキャラクターの更新
	CObjectChara::Update();
}

//============================================================
//	描画処理
//============================================================
void CEnemy::Draw(void)
{
	// オブジェクトキャラクターの描画
	CObjectChara::Draw();
}

//============================================================
//	ヒット処理
//============================================================
void CEnemy::Hit(const int nDmg)
{
	// 変数を宣言
	D3DXVECTOR3 posEnemy = GetPosition();	// 敵位置

	// 体力からダメージ分減算
	m_pLifeGauge->AddLife(-nDmg);

	if (m_pLifeGauge->GetLife() > 0)
	{ // 生きている場合

		// パーティクル3Dオブジェクトを生成
		CParticle3D::Create(CParticle3D::TYPE_DAMAGE, posEnemy);
	}
	else
	{ // 死んでいる場合

		// パーティクル3Dオブジェクトを生成
		CParticle3D::Create(CParticle3D::TYPE_DAMAGE, posEnemy, D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f));
		CParticle3D::Create(CParticle3D::TYPE_DAMAGE, posEnemy, D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f));

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
	else { assert(false); return NULL; }	// 確保失敗
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
	D3DXVECTOR3 posEnemy = GetPosition();	// 敵位置
	D3DXVECTOR3 rotEnemy = GetRotation();	// 敵向き

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotEnemy.y, rotEnemy.x, rotEnemy.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, posEnemy.x, posEnemy.y, posEnemy.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ワールドマトリックスを返す
	return mtxWorld;
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
	D3DXVECTOR3 posEnemy = GetPosition();	// 敵位置
	D3DXVECTOR3 rotEnemy = GetRotation();	// 敵向き
	float fPlayerRadius = CManager::GetPlayer()->GetRadius();	// プレイヤー半径

	// TODO：プレイヤー死んだらManagerのPlayerもNULLにする
	if (USED(CManager::GetPlayer()) && USED(CManager::GetTarget()))
	{ // プレイヤー・ターゲットが使用されている場合

		// 視認対象の設定
		if (collision::Circle2D(CManager::GetPlayer()->GetPosition(), posEnemy, fPlayerRadius, m_status.fFindRadius) == false)
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
		Look(posLook, posEnemy, rotEnemy);

		if (collision::Circle2D(posLook, posEnemy, fPlayerRadius, m_status.fAttackRadius) == false)
		{ // 敵の攻撃範囲外の場合

			// 対象の方向に移動 (前進)
			posEnemy.x -= sinf(rotEnemy.y) * m_status.fForwardMove;
			posEnemy.z -= cosf(rotEnemy.y) * m_status.fForwardMove;

			// ステージ範囲外の補正
			CManager::GetStage()->LimitPosition(posEnemy, m_status.fRadius);
		}
		else
		{ // 敵の攻撃範囲内の場合

			if (collision::Circle2D(posLook, posEnemy, fPlayerRadius, m_status.fBackwardRadius) == true && m_status.bBackward == true)
			{ // 敵の後退範囲内且つ、後退がONの場合

				// 対象の逆方向に移動 (後退)
				posEnemy.x += sinf(rotEnemy.y) * m_status.fBackwardMove;
				posEnemy.z += cosf(rotEnemy.y) * m_status.fBackwardMove;

				// ステージ範囲外の補正
				CManager::GetStage()->LimitPosition(posEnemy, m_status.fRadius);
			}

			// 攻撃
			Attack(posLook);
		}
	}

	// 位置を更新
	SetPosition(posEnemy);

	// 向きを更新
	SetRotation(rotEnemy);
}

//============================================================
//	対象視認処理
//============================================================
void CEnemy::Look(const D3DXVECTOR3& rPosLook, const D3DXVECTOR3& rPosEnemy, D3DXVECTOR3& rRotEnemy)
{
	// 変数を宣言
	float fDestRot = 0.0f;	// 目標向き
	float fDiffRot = 0.0f;	// 向き

	// 視認対象の方向を代入
	fDestRot = atan2f(rPosEnemy.x - rPosLook.x, rPosEnemy.z - rPosLook.z);	// 目標向き

	// 差分向きを求める
	fDiffRot = fDestRot - rRotEnemy.y;
	useful::NormalizeRot(fDiffRot);		// 差分向きの正規化

	// 向きの更新
	rRotEnemy.y += fDiffRot * m_status.fLookRevision;
	useful::NormalizeRot(rRotEnemy.y);	// 向きの正規化
}

//============================================================
//	攻撃処理
//============================================================
void CEnemy::Attack(const D3DXVECTOR3& rTarget)
{
	// カウンターを加算
	m_nCounterAtk++;

	if (m_nCounterAtk >= m_status.nCounterAttack)
	{ // カウンターが一定値以上の場合

		// 変数を宣言
		D3DXVECTOR3 posBull;			// 発射位置
		D3DXVECTOR3 vecBull;			// 発射方向
		D3DXMATRIX  mtxTrans, mtxWorld;	// 計算用マトリックス

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxWorld);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_status.bullPos.x, m_status.bullPos.y, m_status.bullPos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		// マトリックスを掛け合わせる
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &GetMultiModel(m_status.nBullParts)->GetMtxWorld());

		// マトリックスから位置を求める
		posBull = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);

		// 弾の移動方向を求める
		vecBull = rTarget - posBull;
		D3DXVec3Normalize(&vecBull, &vecBull);		// 移動方向を正規化
		useful::LimitNum(vecBull.y, -0.1f, 0.1f);	// 移動方向 yを制限	// TODO：定数マクロ化	// TODO：内側はいられたとき売っちゃうの防止

		// 弾オブジェクトの生成
		CBullet::Create
		( // 引数
			CBullet::TYPE_ENEMY,			// 種類
			posBull,						// 位置
			VEC3_ALL(m_status.fBullRadius),	// 大きさ
			XCOL_WHITE,						// 色
			vecBull,						// 移動方向
			m_status.fBullMove,				// 移動速度
			m_status.nBullLife,				// 寿命
			m_status.nBullDamage			// 攻撃力
		);

		// パーティクル3Dオブジェクトの生成
		CParticle3D::Create(CParticle3D::TYPE_DAMAGE, posBull);

		// カウンターを初期化
		m_nCounterAtk = 0;
	}
}

//============================================================
//	ターゲットとの当たり判定
//============================================================
void CEnemy::CollisionTarget(D3DXVECTOR3& rPos)
{
	// ポインタを宣言
	CTarget *pTarget = CManager::GetTarget();	// ターゲット情報

	if (USED(pTarget))
	{ // ターゲットが使用されている場合

		// ターゲットとの衝突判定
		collision::CirclePillar
		( // 引数
			rPos,					// 判定位置
			pTarget->GetPosition(),	// 判定目標位置
			m_status.fRadius,		// 判定半径
			pTarget->GetRadius()	// 判定目標半径
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

		// ポインタを宣言
		CObject *pObjectTop = CObject::GetTop(nCntPri);	// 先頭オブジェクト

		if (USED(pObjectTop))
		{ // 先頭が存在する場合

			// ポインタを宣言
			CObject *pObjCheck = pObjectTop;	// オブジェクト確認用

			while (USED(pObjCheck))
			{ // オブジェクトが使用されている場合繰り返す

				// ポインタを宣言
				CObject *pObjectNext = pObjCheck->GetNext();	// 次オブジェクト

				if (pObjCheck->GetLabel() != CObject::LABEL_ENEMY || pObjCheck == GetObject())
				{ // オブジェクトラベルが敵ではない、またはオブジェクトが自分自身だった場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				// ターゲットとの衝突判定
				collision::CirclePillar
				( // 引数
					rPos,						// 判定位置
					pObjCheck->GetPosition(),	// 判定目標位置
					m_status.fRadius,			// 判定半径
					pObjCheck->GetRadius()		// 判定目標半径
				);

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}
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
	else { assert(false); return NONE_STRING; }	// 範囲外
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
			Look(posLook, posEnemy, rotEnemy);

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
			CollisionTarget(posEnemy);

			// 敵との当たり判定
			CollisionEnemy(posEnemy);

			// 着地判定
			CManager::GetField()->LandPosition(posEnemy, moveEnemy);

			// ステージ範囲外の補正
			CManager::GetStage()->LimitPosition(posEnemy, status.fRadius);
		}
		else
		{ // 敵の攻撃範囲内の場合

			if (collision::Circle2D(posLook, posEnemy, fPlayerRadius, status.fBackwardRadius) == true && status.bBackward == true)
			{ // 敵の後退範囲内且つ、後退がONの場合

				// 対象の方向を向かせる
				Look(posLook, posEnemy, rotEnemy);

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
			CollisionTarget(posEnemy);

			// 敵との当たり判定
			CollisionEnemy(posEnemy);

			// 着地判定
			CManager::GetField()->LandPosition(posEnemy, moveEnemy);

			// ステージ範囲外の補正
			CManager::GetStage()->LimitPosition(posEnemy, status.fRadius);

			// キャノン向きの設定
			if (SetRotationCannon(posLook, posEnemy, rotEnemy))
			{ // 発射可能状態の場合

				// 攻撃
				Attack(posLook);
			}
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
bool CEnemyCar::SetRotationCannon(const D3DXVECTOR3& rLookPos, const D3DXVECTOR3& rEnemyPos, const D3DXVECTOR3& rEnemyRot)
{
	// 変数を宣言
	D3DXVECTOR3 cannonRot;	// 主砲向き
	float fDestRot = 0.0f;	// 目標向き
	float fDiffRot = 0.0f;	// 向き

	// 主砲向きを取得
	cannonRot = GetMultiModel(MODEL_CANNON)->GetRotation() + rEnemyRot;	// 本体の向きを加算
	useful::NormalizeRot(cannonRot.y);	// 主砲向きの正規化

	// 目標向きの計算
	fDestRot = atan2f(rEnemyPos.x - rLookPos.x, rEnemyPos.z - rLookPos.z);
	useful::NormalizeRot(fDestRot);		// 目標向きの正規化

	// 差分向きの計算
	fDiffRot = fDestRot - cannonRot.y;
	useful::NormalizeRot(fDiffRot);		// 差分向きの正規化

	// 向きの更新
	cannonRot.y += fDiffRot * 0.03f;	// TODO：cannonの向き補正値作る
	useful::NormalizeRot(cannonRot.y);	// キャノン向きの正規化

	// 向きを設定
	GetMultiModel(MODEL_CANNON)->SetRotation(cannonRot - rEnemyRot);	// 本体の向きを減算

	if (fDestRot + D3DX_PI <= cannonRot.y + D3DX_PI + 0.25f
	&&  fDestRot + D3DX_PI >= cannonRot.y + D3DX_PI - 0.25f)	// TODO：cannonの誤差ステータス作る
	{ // 主砲向きと目標向きの誤差が少ない場合

		// 発射可能状態を返す
		return true;
	}
	else
	{ // 主砲向きと目標向きの誤差が多い場合

		// 発射不可能状態を返す
		return false;
	}
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
							else if (strcmp(&aString[0], "BULLET_DAMAGE") == 0)
							{ // 読み込んだ文字列が BULLET_DAMAGE の場合

								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nBullDamage);	// 弾の攻撃力を読み込む
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
							else if (strcmp(&aString[0], "LIFE_POSUP") == 0)
							{ // 読み込んだ文字列が LIFE_POSUP の場合

								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fLifeUp);			// 体力表示のY位置加算量を読み込む
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
