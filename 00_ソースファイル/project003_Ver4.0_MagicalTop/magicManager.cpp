//============================================================
//
//	魔法マネージャー処理 [magicManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "magicManager.h"
#include "manager.h"
#include "input.h"
#include "camera.h"
#include "sceneGame.h"
#include "object.h"
#include "lockCursor.h"
#include "objectGauge2D.h"
#include "multiModel.h"
#include "player.h"
#include "target.h"
#include "enemy.h"
#include "stage.h"
#include "collision.h"

//************************************************************
//	マクロ定義
//************************************************************
#define MAX_MANA	(100)	// 最大マナ
#define MANA_FRAME	(10)	// マナ変動フレーム

#define GAUGE_POS		(D3DXVECTOR3(260.0f, 540.0f, 0.0f))	// 位置
#define GAUGE_GAUGESIZE	(D3DXVECTOR3(200.0f, 30.0f, 0.0f))	// ゲージ大きさ
#define GAUGE_FRONTCOL	(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f))	// 表ゲージ色
#define GAUGE_BACKCOL	(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	// 裏ゲージ色

#define HEAL_CNT	(120)	// 回復状態に移行するまでのカウンター
#define NORMAL_CNT	(40)	// 通常状態に移行するまでのカウンター

#define HEAL_SAFE_PLUS	(2)		// 回復状態移行カウンターのセーフエリア時の加算量
#define HEALCNT_AREAMUL	(10)	// セーフエリア外での回復カウンター設定用係数
#define SUB_TARGLIFE	(-2)	// セーフエリアでマナ回復時のターゲットへのダメージ量
#define SUB_SHOTROT		(0.3f)	// 発射向きの減算量

//************************************************************
//	親クラス [CMagicManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CMagicManager::CMagicManager()
{
	// メンバ変数をクリア
	m_pMana = NULL;			// マナの情報
	m_state = STATE_NORMAL;	// 状態
	m_nCounterMagic = 0;	// 魔法管理カウンター
	m_nCounterState = 0;	// 状態管理カウンター
	m_nCounterHeal = 0;		// 回復管理カウンター
}

//============================================================
//	デストラクタ
//============================================================
CMagicManager::~CMagicManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CMagicManager::Init(void)
{
	// メンバ変数を初期化
	m_pMana = NULL;			// マナの情報
	m_state = STATE_NORMAL;	// 状態
	m_nCounterMagic = 0;	// 魔法管理カウンター
	m_nCounterState = 0;	// 状態管理カウンター
	m_nCounterHeal = 0;		// 回復管理カウンター

	// マナの生成
	m_pMana = CObjectGauge2D::Create
	( // 引数
		CObject::LABEL_GAUGE,	// オブジェクトラベル
		MAX_MANA,				// 最大マナ
		MANA_FRAME,				// マナ変動フレーム
		GAUGE_POS,				// 位置
		GAUGE_GAUGESIZE,		// ゲージ大きさ
		GAUGE_FRONTCOL,			// 表ゲージ色
		GAUGE_BACKCOL			// 裏ゲージ色
	);
	if (UNUSED(m_pMana))
	{ // 非使用中の場合

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
void CMagicManager::Uninit(void)
{

}

//============================================================
//	更新処理
//============================================================
void CMagicManager::Update(void)
{
	if (CSceneGame::GetPlayer()->GetState() != CPlayer::STATE_NONE
	&&  CSceneGame::GetPlayer()->GetState() != CPlayer::STATE_FADEIN
	&&  CSceneGame::GetPlayer()->GetState() != CPlayer::STATE_FADEOUT)
	{ // プレイヤーが何もしない状態ではない場合

		if (m_nCounterMagic > 0)
		{ // カウンターが 0より大きい場合

			// カウンターを減算
			m_nCounterMagic--;
		}

		switch (m_state)
		{ // 状態ごとの処理
		case STATE_NORMAL:	// 通常状態

			if (m_nCounterState < HEAL_CNT)
			{ // カウンターが一定値より小さい場合

				if (CSceneGame::GetStage()->GetAreaPlayer() == CStage::AREA_SAFE)
				{ // セーフエリアにいる場合

					// カウンターを加算
					m_nCounterState += HEAL_SAFE_PLUS;
				}
				else
				{ // セーフエリアにいない場合

					// カウンターを加算
					m_nCounterState++;
				}
			}
			else
			{ // カウンターが一定値以上の場合

				// カウンターを初期化
				m_nCounterState = 0;

				// 状態を変更
				m_state = STATE_HEAL;	// 回復状態
			}

			break;

		case STATE_ATTACK:	// 攻撃状態

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

			if (CSceneGame::GetStage()->GetAreaPlayer() == CStage::AREA_SAFE)
			{ // セーフエリアにいる場合

				if (m_pMana->GetNum() < MAX_MANA)
				{ // マナが減っている場合

					// マナを回復
					m_pMana->AddNum(1);

					// ターゲットの体力を減算
					CSceneGame::GetTarget()->AddLife(SUB_TARGLIFE);
				}
			}
			else
			{ // セーフエリアにいない場合

				if (m_nCounterHeal < HEALCNT_AREAMUL * CSceneGame::GetStage()->GetAreaPlayer())
				{ // カウンターが一定値より小さい場合

					// カウンターを加算
					m_nCounterHeal++;
				}
				else
				{ // カウンターが一定値以上の場合

					// カウンターを初期化
					m_nCounterHeal = 0;

					// マナを回復
					m_pMana->AddNum(1);
				}
			}

			break;

		default:	// 例外処理
			assert(false);
			break;
		}
	}
}

//============================================================
//	魔法の発射処理
//============================================================
bool CMagicManager::ShotMagic(void)
{
	if (m_pMana->GetNum() > 0 && m_nCounterMagic <= 0)
	{ // マナがある且つ、クールタイムが終了した場合

		// ポインタを宣言
		CPlayer *pPlayer = CSceneGame::GetPlayer();	// プレイヤー

		// 変数を宣言
		CMagic::StatusInfo status = CMagic::GetStatusInfo();			// 魔法ステータス
		D3DXVECTOR3 posPlayer = pPlayer->GetPosition();					// プレイヤー位置
		D3DXVECTOR3 posTarget = CSceneGame::GetTarget()->GetPosition();	// ターゲット位置

		D3DXVECTOR3 magicPos, vecMove;	// 発射位置・移動方向
		D3DXMATRIX  mtxTrans, mtxWorld;	// 計算用マトリックス
		float fRotVec = 0.0f;			// 発射方向

		// 発射向きを設定
		fRotVec = atan2f(posPlayer.x - posTarget.x, posPlayer.z - posTarget.z) - SUB_SHOTROT;

		// 向きを補正
		useful::NormalizeRot(fRotVec);

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxWorld);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, status.shotPos.x, status.shotPos.y, status.shotPos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		// マトリックスを掛け合わせる
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, pPlayer->GetMultiModel(status.nShotParts)->GetMtxWorld());

		// マトリックスから発射位置を求める
		magicPos = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);

		// 向きを正規化
		useful::NormalizeRot(fRotVec);

		// 移動方向を設定
		vecMove = D3DXVECTOR3(sinf(fRotVec), 0.0f, cosf(fRotVec));

		// 魔法オブジェクトの生成
		CMagic::Create
		( // 引数
			magicPos,	// 位置
			VEC3_ZERO,	// 向き
			vecMove		// 移動方向
		);

		// カウンターを設定
		m_nCounterMagic = status.nCoolTime;

		// マナを消費
		m_pMana->AddNum(-1);

		// カウンターを初期化
		m_nCounterState = 0;

		// 状態を設定
		m_state = STATE_ATTACK;	// 攻撃状態

		// 発射した状態を返す
		return true;
	}

	// 発射していない状態を返す
	return false;
}

//============================================================
//	生成処理
//============================================================
CMagicManager *CMagicManager::Create()
{
	// ポインタを宣言
	CMagicManager *pMagicManager = NULL;	// 魔法マネージャー生成用

	if (UNUSED(pMagicManager))
	{ // 使用されていない場合

		// メモリ確保
		pMagicManager = new CMagicManager;	// 魔法マネージャー
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pMagicManager))
	{ // 使用されている場合
		
		// 魔法マネージャーの初期化
		if (FAILED(pMagicManager->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pMagicManager;
			pMagicManager = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pMagicManager;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CMagicManager::Release(CMagicManager *&prMagicManager)
{
	if (USED(prMagicManager))
	{ // 使用中の場合

		// 魔法マネージャーの終了
		prMagicManager->Uninit();

		// メモリ開放
		delete prMagicManager;
		prMagicManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}
