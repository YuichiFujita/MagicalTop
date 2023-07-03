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
#include "object.h"
#include "lockCursor.h"
#include "player.h"
#include "collision.h"

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
	m_magic = CMagic::TYPE_LV0_NORMAL;	// 魔法
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
	m_magic = CMagic::TYPE_LV0_NORMAL;	// 魔法

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
//	魔法のロックオン処理
//============================================================
void CMagicManager::LockOnMagic(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot)
{
	// TODO：見直す

	// 変数を宣言
	CMagic::StatusInfo status = CMagic::GetStatusInfo(m_magic);	// 魔法ステータス
	int nCurrentLock = 0;	// 現在のロックオン数
	float fLength = 0.0f;	// 敵との距離

	// 変数配列を宣言
	float aLockLength[MAX_LOCK] = {};	// ロックオンした敵との距離

	// ポインタを宣言
	CObject *apObject[MAX_LOCK] = {};	// ロックオンしたオブジェクト

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // 優先順位の総数分繰り返す

		for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
		{ // オブジェクトの総数分繰り返す

			// 変数を宣言
			bool bHit = false;	// 判定結果

			// ポインタを宣言
			CObject *pObject = CObject::GetObject(nCntPri, nCntObject);	// オブジェクト

			if (UNUSED(pObject)
			||  pObject->GetLabel() != CObject::LABEL_ENEMY)
			{ // オブジェクトが非使用中・ラベルが敵ではない場合

				// 次の繰り返しに移行
				continue;
			}

			// 視界内判定
			bHit = collision::Sector
			( // 引数
				rPos,					// 判定位置
				pObject->GetPosition(),	// 判定目標位置
				rRot.y,					// 判定向き
				status.fViewRadius,		// 視界範囲
				status.fViewAngle,		// 視野角
				&fLength				// 判定目標との距離
			);

			if (bHit)
			{ // 視界内の場合

				if (nCurrentLock < status.nLock)
				{ // ロックオン数が最大ロックオン数より小さい場合

					// 現在のロックオン数を加算
					nCurrentLock++;
				}

				if (aLockLength[nCurrentLock - 1] < fLength)
				{ // 最遠のロックオンよりも距離が近い場合

					// 変数を宣言
					CObject *pKeepObject;	// 位置ソート用
					float fKeepLength;		// 距離ソート用
					int   nCurrentMaxID;	// 最大値のインデックス

					// 情報を設定
					aLockLength[nCurrentLock - 1]	= fLength;
					apObject[nCurrentLock - 1]		= pObject;

					// TODO：ソート見直し
#if 0
					for (int nCntKeep = 0; nCntKeep < (nCurrentLock - 1); nCntKeep++)
					{ // 入れ替える値の総数 -1回分繰り返す

						// 現在の繰り返し数を代入 (要素1とする)
						nCurrentMaxID = nCntKeep;

						for (int nCntSort = nCntKeep + 1; nCntSort < nCurrentLock; nCntSort++)
						{ // 入れ替える値の総数 -nCntKeep分繰り返す

							if (aLockLength[nCurrentMaxID] < aLockLength[nCntSort])
							{ // 最大値に設定されている値より、現在の値のほうが大きい場合

								// 現在の要素番号を最大値に設定
								nCurrentMaxID = nCntSort;
							}
						}

						if (nCntKeep != nCurrentMaxID)
						{ // 最大値の要素番号に変動があった場合

							// 要素(値)の入れ替え
							fKeepLength					= aLockLength[nCntKeep];
							aLockLength[nCntKeep]		= aLockLength[nCurrentMaxID];
							aLockLength[nCurrentMaxID]	= fKeepLength;

							// 要素(位置)の入れ替え
							pKeepObject				= apObject[nCntKeep];
							apObject[nCntKeep]		= apObject[nCurrentMaxID];
							apObject[nCurrentMaxID]	= pKeepObject;
						}
					}
#else
					// ロックオンのソート
					SortLockOnMagic();
#endif
				}
			}
		}
	}

	// ロックオンの設定
	if (nCurrentLock < status.nLock)
	{ // ロックオン数が最大数じゃない場合

		for (int nCntLock = 0; nCntLock < MAX_LOCK; nCntLock++)
		{ // ロックオンの最大数分繰り返す

			// 変数を宣言
			bool bSame = false;	// 同一判定

			if (USED(m_apLockCursor[nCntLock]->GetLockObject()))
			{ // オブジェクトをロックオンしていた場合

				for (int nCnt = 0; nCnt < MAX_LOCK; nCnt++)
				{ // ロックオンの最大数分繰り返す

					if (m_apLockCursor[nCntLock]->GetLockObject() == apObject[nCnt])
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

				// 描画しない設定にする
				m_apLockCursor[nCntLock]->SetEnableDraw(false);
			}
		}
	}

	for (int nCntLock = 0; nCntLock < nCurrentLock; nCntLock++)
	{ // 今回のロックオン数分繰り返す

		// 描画する設定にする
		m_apLockCursor[nCntLock]->SetEnableDraw(true);

		// ロックオンを設定
		m_apLockCursor[nCntLock]->SetLockObject(apObject[nCntLock]);
	}
}

//============================================================
//	魔法の発射処理
//============================================================
void CMagicManager::ShotMagic(void)
{
	// 変数を宣言
	D3DXVECTOR3 posPlayer = CManager::GetPlayer()->GetPosition();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = CManager::GetPlayer()->GetRotation();	// プレイヤー向き
	bool bLock = false;	// ロックオン状況

	for (int nCntLock = 0; nCntLock < MAX_LOCK; nCntLock++)
	{ // ロックオンの最大数分繰り返す

		if (m_apLockCursor[nCntLock]->IsDraw())
		{ // ロックオンしていた場合

			// 変数を宣言
			float fMoveRot = atan2f	// 敵方向
			( // 引数
				posPlayer.x - m_apLockCursor[nCntLock]->GetLockObject()->GetPosition().x,
				posPlayer.z - m_apLockCursor[nCntLock]->GetLockObject()->GetPosition().z
			);
			D3DXVECTOR3 magicPos = D3DXVECTOR3(posPlayer.x, posPlayer.y + PLAY_MAGIC_POS_PLUS_Y, posPlayer.z);	// 発射位置
			D3DXVECTOR3 magicRot = D3DXVECTOR3(rotPlayer.x + (-D3DX_PI * 0.5f), fMoveRot, 0.0f);	// 発射向き

			// 魔法オブジェクトの生成
			CMagic::Create
			( // 引数
				m_magic,	// 種類
				magicPos,	// 発射位置
				magicRot	// 発射向き
			);

			// ロックオンをしている状態にする
			bLock = true;
		}
	}

	if (bLock == false)
	{ // ロックオンされていなかった場合

		// 変数を宣言
		D3DXVECTOR3 magicPos = D3DXVECTOR3(posPlayer.x, posPlayer.y + PLAY_MAGIC_POS_PLUS_Y, posPlayer.z);	// 発射位置
		D3DXVECTOR3 magicRot = D3DXVECTOR3(rotPlayer.x + (-D3DX_PI * 0.5f), rotPlayer.y, 0.0f);	// 発射向き

		// 魔法オブジェクトの生成
		CMagic::Create
		( // 引数
			m_magic,	// 種類
			magicPos,	// 発射位置
			magicRot	// 発射向き
		);
	}
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
//	生成処理
//============================================================
CMagicManager *CMagicManager::Create()
{
	// ポインタを宣言
	CMagicManager *pMagicManager = NULL;	// 魔法マネージャー生成用

	if (UNUSED(pMagicManager))
	{ // 使用されていない場合

		// メモリ確保
		pMagicManager = new CMagicManager();	// 魔法マネージャー
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
void CMagicManager::SortLockOnMagic(void)
{
	// TODO：作る
}
