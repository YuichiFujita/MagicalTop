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

//************************************************************
//	親クラス [CMagicManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CMagicManager::CMagicManager()
{
	// メンバ変数をクリア
	memset(&m_apLockCursor[0], 0, sizeof(m_apLockCursor));	// ロックオン表示情報
	m_pMana = NULL;				// マナの情報
	m_magic = CMagic::TYPE_FLY;	// 魔法
	m_state = STATE_NORMAL;		// 状態
	m_nCounterMagic = 0;		// 魔法管理カウンター
	m_nCounterState = 0;		// 状態管理カウンター
	m_nCounterHeal = 0;			// 回復管理カウンター
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
	memset(&m_apLockCursor[0], 0, sizeof(m_apLockCursor));	// ロックオン表示情報
	m_pMana = NULL;				// マナの情報
	m_magic = CMagic::TYPE_FLY;	// 魔法
	m_state = STATE_NORMAL;		// 状態
	m_nCounterMagic = 0;		// 魔法管理カウンター
	m_nCounterState = 0;		// 状態管理カウンター
	m_nCounterHeal = 0;			// 回復管理カウンター

	for (int nCntLock = 0; nCntLock < MAX_LOCK; nCntLock++)
	{ // ロックオンの最大数分繰り返す

		// ロックオン表示の生成
		m_apLockCursor[nCntLock] = CLockCursor::Create(VEC3_ZERO);
		if (UNUSED(m_apLockCursor[nCntLock]))
		{ // 非使用中の場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

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

//============================================================
//	魔法のロックオン処理
//============================================================
void CMagicManager::LockOnMagic(const D3DXVECTOR3& rPos)
{
	// 変数を宣言
	CMagic::StatusInfo status = CMagic::GetStatusInfo(m_magic);	// 魔法ステータス
	int nCurrentLock = 0;	// 現在のロックオン数

	// 変数配列を宣言
	LockInfo aLockData[MAX_LOCK] = {};	// ロックオン情報

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // 優先順位の総数分繰り返す

		// ポインタを宣言
		CObject *pObjectTop = CObject::GetTop(nCntPri);	// 先頭オブジェクト

		if (USED(pObjectTop))
		{ // 先頭が存在する場合

			// 変数を宣言
			bool  bHit    = false;	// 判定結果
			float fLength = 0.0f;	// 敵との距離

			// ポインタを宣言
			CObject *pObjCheck = pObjectTop;	// オブジェクト確認用

			while (USED(pObjCheck))
			{ // オブジェクトが使用されている場合繰り返す

				// ポインタを宣言
				CObject *pObjectNext = pObjCheck->GetNext();	// 次オブジェクト

				if (pObjCheck->GetLabel() != CObject::LABEL_ENEMY)
				{ // オブジェクトラベルが敵ではない場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				if (pObjCheck->GetState() == CEnemy::STATE_SPAWN
				||  pObjCheck->GetState() == CEnemy::STATE_DEATH)
				{ // 敵の状態がスポーン状態か死亡状態の場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				// 視界内判定
				bHit = collision::Circle2D
				( // 引数
					rPos,						// 判定位置
					pObjCheck->GetPosition(),	// 判定目標位置
					status.fViewRadius,			// 判定半径
					pObjCheck->GetRadius(),		// 判定目標半径
					&fLength					// 判定目標との距離
				);

				if (bHit)
				{ // 視界内の場合

					if (nCurrentLock < status.nLock)
					{ // 現在の魔法の最大ロックオン数より少ない場合

						// 現在のロックオン数を加算
						nCurrentLock++;

						// 情報を設定
						aLockData[nCurrentLock - 1].pObject = pObjCheck;	// ロックオンしたオブジェクト
						aLockData[nCurrentLock - 1].fLength = fLength;		// ロックオンオブジェクトとの距離

						if (nCurrentLock >= status.nLock)
						{ // ロックオンの最大数に到達した場合

							// ロックオンのソート
							SortLockOnMagic(&aLockData[0], nCurrentLock, pObjCheck, fLength);
						}
					}
					else
					{ // ロックオン数が最大の場合

						// ロックオンのソート
						SortLockOnMagic(&aLockData[0], nCurrentLock, pObjCheck, fLength);
					}
				}

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}
		}
	}

	// 前回のロックオンの削除
	for (int nCntLock = 0; nCntLock < MAX_LOCK; nCntLock++)
	{ // ロックオンの最大数分繰り返す

		// 変数を宣言
		bool bSame = false;	// 同一判定

		if (USED(m_apLockCursor[nCntLock]->GetLockObject()))
		{ // オブジェクトをロックオンしていた場合

			for (int nCntCheck = 0; nCntCheck < MAX_LOCK; nCntCheck++)
			{ // ロックオンの最大数分繰り返す

				if (m_apLockCursor[nCntLock]->GetLockObject() == aLockData[nCntCheck].pObject)
				{ // ロック中のオブジェクトが今回のロックオブジェクト同じ場合

					// 同一オブジェクトが存在した状態にする
					bSame = true;

					// 処理を抜ける
					break;
				}
			}
		}

		if (bSame == false)
		{ // 同一オブジェクトが存在しなかった場合

			// 表示しない設定にする
			m_apLockCursor[nCntLock]->SetEnableDraw(false);
		}
	}

	// 今回のロックオンの設定
	for (int nCntLock = 0; nCntLock < nCurrentLock; nCntLock++)
	{ // 今回のロックオン数分繰り返す

		// 表示する設定にする
		m_apLockCursor[nCntLock]->SetEnableDraw(true);

		// ロックするオブジェクトを設定
		m_apLockCursor[nCntLock]->SetLockObject(aLockData[nCntLock].pObject);
	}
}

//============================================================
//	魔法の発射処理
//============================================================
bool CMagicManager::ShotMagic(void)
{
	// 変数を宣言
	D3DXVECTOR3 posPlayer = CSceneGame::GetPlayer()->GetPosition();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = CSceneGame::GetPlayer()->GetRotation();	// プレイヤー向き
	CMagic::StatusInfo status = CMagic::GetStatusInfo(m_magic);		// 魔法ステータス
	int nNumShot = 0;	// 発射数
	bool bLock = false;	// ロックオン状況

	// ポインタを宣言
	CPlayer *pPlayer = CSceneGame::GetPlayer();	// プレイヤーの情報

	if (m_pMana->GetNum() > 0
	&&  m_nCounterMagic <= 0)
	{ // マナがある且つ、クールタイムが終了した場合

		for (int nCntLock = 0; nCntLock < MAX_LOCK; nCntLock++)
		{ // ロックオンの最大数分繰り返す

			if (m_apLockCursor[nCntLock]->IsDraw())
			{ // ロックオンしていた場合

				// 変数を宣言
				D3DXVECTOR3 magicPos, vecMove;	// 発射位置・移動方向
				D3DXMATRIX  mtxTrans, mtxWorld;	// 計算用マトリックス

				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&mtxWorld);

				// 位置を反映
				D3DXMatrixTranslation(&mtxTrans, status.shotPos.x, status.shotPos.y, status.shotPos.z);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

				// マトリックスを掛け合わせる
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &pPlayer->GetMultiModel(status.nShotParts)->GetMtxWorld());

				// マトリックスから発射位置を求める
				magicPos = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);

				// 移動方向を求める
				vecMove = m_apLockCursor[nCntLock]->GetLockObject()->GetPosition() - magicPos;

				// 魔法オブジェクトの生成
				CMagic::Create
				( // 引数
					m_magic,	// 種類
					magicPos,	// 位置
					VEC3_ZERO,	// 向き
					vecMove		// 移動方向
				);

				// カウンターを設定
				m_nCounterMagic = status.nCoolTime;

				// 発射数を加算
				nNumShot++;

				// ロックオンをしている状態にする
				bLock = true;
			}
		}

		if (bLock == false)
		{ // ロックオンされていなかった場合

			// 変数を宣言
			D3DXVECTOR3 magicPos, vecMove;			// 発射位置・移動方向
			D3DXMATRIX  mtxTrans, mtxWorld;			// 計算用マトリックス
			float fRotVec = rotPlayer.y + D3DX_PI;	// 発射方向

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxWorld);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, status.shotPos.x, status.shotPos.y, status.shotPos.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

			// マトリックスを掛け合わせる
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &pPlayer->GetMultiModel(status.nShotParts)->GetMtxWorld());

			// マトリックスから発射位置を求める
			magicPos = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);

			// 向きを正規化
			useful::NormalizeRot(fRotVec);

			// 移動方向を設定
			vecMove = D3DXVECTOR3(sinf(fRotVec), 0.0f, cosf(fRotVec));

			// 魔法オブジェクトの生成
			CMagic::Create
			( // 引数
				m_magic,	// 種類
				magicPos,	// 位置
				VEC3_ZERO,	// 向き
				vecMove		// 移動方向
			);

			// カウンターを設定
			m_nCounterMagic = status.nCoolTime;

			// 発射数を加算
			nNumShot++;
		}

		// マナを消費
		m_pMana->AddNum(-nNumShot);

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
//	魔法の設定処理
//============================================================
void CMagicManager::SetMagic(const CMagic::TYPE type)
{
	// 引数の魔法を設定
	m_magic = type;
}

//============================================================
//	魔法取得処理
//============================================================
CMagic::TYPE CMagicManager::GetMagic(void) const
{
	// 魔法を返す
	return m_magic;
}

//============================================================
//	魔法ロックオン全削除処理
//============================================================
void CMagicManager::DeleteLockOn(void)
{
	// ロックオンの削除
	for (int nCntLock = 0; nCntLock < MAX_LOCK; nCntLock++)
	{ // ロックオンの最大数分繰り返す

		// 表示しない設定にする
		m_apLockCursor[nCntLock]->SetEnableDraw(false);
	}
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

//============================================================
//	ロックオンのソート処理
//============================================================
void CMagicManager::SortLockOnMagic(LockInfo *pLock, const int nNumLock, CObject *pObject, const float fLength)
{
	// 変数を宣言
	LockInfo keepLock;	// ソート用
	int nCurrentMaxID;	// 最大値のインデックス

	// 例外処理
	assert(nNumLock > 0);	// ロックオン数が最小値以下

	if (fLength <= pLock[nNumLock - 1].fLength)
	{ // 最遠のロックオンよりも近いロックオンの場合

		// 情報を設定
		pLock[nNumLock - 1].pObject = pObject;	// ロックオンしたオブジェクト
		pLock[nNumLock - 1].fLength = fLength;	// ロックオンオブジェクトとの距離

		for (int nCntKeep = 0; nCntKeep < (nNumLock - 1); nCntKeep++)
		{ // 入れ替える値の総数 -1回分繰り返す

			// 現在の繰り返し数を代入
			nCurrentMaxID = nCntKeep;

			for (int nCntSort = nCntKeep + 1; nCntSort < nNumLock; nCntSort++)
			{ // 入れ替える値の総数 - ソート回数分繰り返す

				if (pLock[nCurrentMaxID].fLength > pLock[nCntSort].fLength)
				{ // 最大値に設定されている距離より、現在の距離のほうが遠い場合

					// 現在のインデックスを最大値に設定
					nCurrentMaxID = nCntSort;
				}
			}

			if (nCntKeep != nCurrentMaxID)
			{ // 最大値のインデックスに変動があった場合

				// 要素の入れ替え
				keepLock             = pLock[nCntKeep];
				pLock[nCntKeep]      = pLock[nCurrentMaxID];
				pLock[nCurrentMaxID] = keepLock;
			}
		}
	}
}
