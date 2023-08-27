//============================================================
//
//	魔法処理 [magic.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "magic.h"
#include "manager.h"
#include "sceneGame.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"
#include "effect3D.h"
#include "target.h"
#include "enemy.h"
#include "field.h"
#include "stage.h"
#include "bubble.h"

//************************************************************
//	マクロ定義
//************************************************************
#define MAGIC_SETUP_TXT	"data\\TXT\\magic.txt"	// セットアップテキスト相対パス

#define PLUS_HIT_RADIUS	(10.0f)	// 当たり判定の拡張値
#define MOVE_INHALE		(10.0f)	// 吸い込まれ時の魔法の移動量
#define MOVE_DELETE		(3.0f)	// 消失時の魔法の移動量

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CMagic::StatusInfo CMagic::m_statusInfo = {};	// ステータス情報

//************************************************************
//	子クラス [CMagic] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CMagic::CMagic() : CObject(CObject::LABEL_MAGIC)
{
	// メンバ変数をクリア
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));	// ワールドマトリックス
	m_pBubble	= NULL;			// バブル情報
	m_pos		= VEC3_ZERO;	// 現在位置
	m_movePos	= VEC3_ZERO;	// 位置移動量
	m_rot		= VEC3_ZERO;	// 向き
	m_moveRot	= VEC3_ZERO;	// 向き変更量
	m_state		= STATE_NORMAL;	// 状態
}

//============================================================
//	デストラクタ
//============================================================
CMagic::~CMagic()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CMagic::Init(void)
{
	// メンバ変数を初期化
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));	// ワールドマトリックス
	m_pBubble	= NULL;			// バブル情報
	m_pos		= VEC3_ZERO;	// 現在位置
	m_movePos	= VEC3_ZERO;	// 位置移動量
	m_rot		= VEC3_ZERO;	// 向き
	m_moveRot	= VEC3_ZERO;	// 向き変更量
	m_state		= STATE_NORMAL;	// 状態

	// バブル情報の生成
	m_pBubble = CBubble::Create(this, m_statusInfo.nLife, VEC3_ALL(m_statusInfo.bubbleRadius), VEC3_ZERO, 0.0f);
	if (UNUSED(m_pBubble))
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
void CMagic::Uninit(void)
{
	// バブルの終了
	m_pBubble->Uninit();

	// 魔法を破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CMagic::Update(void)
{
	// 変数を宣言
	D3DXVECTOR3 vecTarg;

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NORMAL:	// 通常状態

		// 移動量を加算
		m_pos += m_movePos;

		// 位置に風速を加算
		m_pos += CSceneGame::GetStage()->GetVecWind();

		// バブルレベルを加算
		m_pBubble->AddLevel(1);

		if (m_pBubble->GetLevel() >= m_statusInfo.nLife)
		{ // 寿命が来た場合

			// 状態を設定
			m_state = STATE_INHALE;	// 吸い込まれ状態
		}

		break;

	case STATE_INHALE:	// 吸い込まれ状態

		// ターゲット方向のベクトルを計算
		vecTarg = CSceneGame::GetTarget()->GetPosition() - m_pos;
		vecTarg.y = 0.0f;						// ベクトルの縦方向を無視
		D3DXVec3Normalize(&vecTarg, &vecTarg);	// ベクトル正規化
		
		// 移動量を設定
		m_movePos = vecTarg * MOVE_INHALE;

		// ベクトルを90度回転
		vecTarg = D3DXVECTOR3(-vecTarg.z, 0.0f, vecTarg.x);

		// 移動量を加算
		m_movePos += vecTarg * MOVE_INHALE;

		// 移動量を加算
		m_pos += m_movePos;

		break;

	case STATE_DELETE:	// 消失状態

		// ターゲット方向のベクトルを計算
		vecTarg = CSceneGame::GetTarget()->GetPosition() - m_pos;
		vecTarg.y = 0.0f;						// ベクトルの縦方向を無視
		D3DXVec3Normalize(&vecTarg, &vecTarg);	// ベクトル正規化
		
		// 移動量を設定
		m_movePos = vecTarg * MOVE_DELETE;

		// ベクトルを90度回転
		vecTarg = D3DXVECTOR3(-vecTarg.z, 0.0f, vecTarg.x);

		// 移動量を加算
		m_movePos += vecTarg * MOVE_DELETE;

		// 移動量を加算
		m_pos += m_movePos;

		// バブルレベルを減算
		m_pBubble->AddLevel(-1);

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	if (m_state != STATE_DELETE)
	{ // 消失状態ではない場合

		// バリアとの当たり判定
		if (CSceneGame::GetStage()->CollisionBarrier(m_pos, m_pBubble->GetRadius()))
		{ // 当たっていた場合

			// 状態を設定
			m_state = STATE_DELETE;	// 消失状態
		}
	}

	// 縦位置を補正
	CSceneGame::GetField()->LandPosition(m_pos, VEC3_ZERO);

	// バブルの更新
	m_pBubble->Update();

	if (CollisionEnemy())
	{ // 敵に当たっていた場合

		// オブジェクトの終了
		Uninit();

		// 関数を抜ける
		return;
	}
}

//============================================================
//	描画処理
//============================================================
void CMagic::Draw(void)
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
}

//============================================================
//	生成処理
//============================================================
CMagic *CMagic::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXVECTOR3& rVec		// 移動方向
)
{
	// ポインタを宣言
	CMagic *pMagic = NULL;		// 魔法生成用

	if (UNUSED(pMagic))
	{ // 使用されていない場合

		// メモリ確保
		pMagic = new CMagic;	// 魔法
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pMagic))
	{ // 使用されている場合
		
		// 魔法の初期化
		if (FAILED(pMagic->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pMagic;
			pMagic = NULL;

			// 失敗を返す
			return NULL;
		}

		// 位置を設定
		pMagic->SetPosition(rPos);

		// 向きを設定
		pMagic->SetRotation(rRot);

		// 移動量を設定
		pMagic->SetMove(rVec, pMagic->GetStatusInfo().fMove);

		// 確保したアドレスを返す
		return pMagic;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	ステータス情報取得処理
//============================================================
CMagic::StatusInfo CMagic::GetStatusInfo(void)
{
	// ステータス情報を返す
	return m_statusInfo;
}

//============================================================
//	移動量の設定処理
//============================================================
void CMagic::SetMove(D3DXVECTOR3 vec, const float fMove)
{
	// ベクトルの正規化
	D3DXVec3Normalize(&vec, &vec);

	// 移動量を設定
	m_movePos = vec * fMove;
}

//============================================================
//	位置の設定処理
//============================================================
void CMagic::SetPosition(const D3DXVECTOR3& rPos)
{
	// 引数の位置を設定
	m_pos = rPos;
}

//============================================================
//	向きの設定処理
//============================================================
void CMagic::SetRotation(const D3DXVECTOR3& rRot)
{
	// 引数の向きを設定
	m_rot = rRot;

	// 向きの正規化
	useful::NormalizeRot(m_rot.x);
	useful::NormalizeRot(m_rot.y);
	useful::NormalizeRot(m_rot.z);
}

//============================================================
//	位置取得処理
//============================================================
D3DXVECTOR3 CMagic::GetPosition(void) const
{
	// 位置を返す
	return m_pos;
}

//============================================================
//	向き取得処理
//============================================================
D3DXVECTOR3 CMagic::GetRotation(void) const
{
	// 向きを返す
	return m_rot;
}

//============================================================
//	ターゲットとの当たり判定
//============================================================
bool CMagic::CollisionTarget(void)
{
	// ポインタを宣言
	CTarget *pTarget = CSceneGame::GetTarget();	// ターゲット情報

	if (pTarget->GetState() != CTarget::STATE_DESTROY)
	{ // ターゲットが破壊されていない場合

		// ターゲットとの当たり判定
		if (collision::Circle2D
		( // 引数
			m_pos,										// 判定位置
			pTarget->GetPosition(),						// 判定目標位置
			m_pBubble->GetRadius() + PLUS_HIT_RADIUS,	// 判定半径
			pTarget->GetRadius()						// 判定目標半径
		))
		{ // 魔法に当たっていた場合

			// 当たった判定を返す
			return true;
		}
	}

	// 当たっていない判定を返す
	return false;
}

//============================================================
//	敵との当たり判定
//============================================================
bool CMagic::CollisionEnemy(void)
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

				if (pObjCheck->GetLabel() != CObject::LABEL_ENEMY)
				{ // オブジェクトラベルが敵ではない場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				if (pObjCheck->GetState() == CEnemy::STATE_SPAWN
				||  pObjCheck->GetState() == CEnemy::STATE_DEATH)
				{ // 敵の状態がスポーンまたは死亡状態の場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				// 魔法判定
				if (collision::Circle3D
				( // 引数
					m_pos,										// 判定位置
					pObjCheck->GetPosition(),					// 判定目標位置
					m_pBubble->GetRadius() + PLUS_HIT_RADIUS,	// 判定半径
					pObjCheck->GetRadius()						// 判定目標半径
				))
				{ // 魔法に当たっていた場合

					// 敵のヒット処理
					pObjCheck->Hit((m_pBubble->GetLevel() / (m_pBubble->GetMaxLevel() / 2)) + 1);

					// 当たった判定を返す
					return true;
				}

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}
		}
	}

	// 当たっていない判定を返す
	return false;
}

//============================================================
//	セットアップ処理
//============================================================
void CMagic::LoadSetup(void)
{
	// 変数を宣言
	int nType	= 0;	// 種類の代入用
	int nHoming	= 0;	// ホーミングのON/OFFの変換用
	int nEnd	= 0;	// テキスト読み込み終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;	// ファイルポインタ

	// 静的メンバ変数の情報をクリア
	memset(&m_statusInfo, 0, sizeof(m_statusInfo));	// ステータス情報

	// ファイルを読み込み形式で開く
	pFile = fopen(MAGIC_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			// ステータスの設定
			if (strcmp(&aString[0], "STATUSSET") == 0)
			{ // 読み込んだ文字列が STATUSSET の場合

				do
				{ // 読み込んだ文字列が END_STATUSSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "MAGICSET") == 0)
					{ // 読み込んだ文字列が MAGICSET の場合

						do
						{ // 読み込んだ文字列が END_MAGICSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "TYPE") == 0)
							{ // 読み込んだ文字列が TYPE の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nType);		// 種類を読み込む
							}
							else if (strcmp(&aString[0], "LIFE") == 0)
							{ // 読み込んだ文字列が LIFE の場合

								fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
								fscanf(pFile, "%d", &m_statusInfo.nLife);	// 寿命を読み込む
							}
							else if (strcmp(&aString[0], "COOLTIME") == 0)
							{ // 読み込んだ文字列が COOLTIME の場合

								fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
								fscanf(pFile, "%d", &m_statusInfo.nCoolTime);	// クールタイムを読み込む
							}
							else if (strcmp(&aString[0], "MOVE") == 0)
							{ // 読み込んだ文字列が MOVE の場合

								fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
								fscanf(pFile, "%f", &m_statusInfo.fMove);		// 移動量を読み込む
							}
							else if (strcmp(&aString[0], "BUBBLE_RADIUS") == 0)
							{ // 読み込んだ文字列が BUBBLE_RADIUS の場合

								fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
								fscanf(pFile, "%f", &m_statusInfo.bubbleRadius);	// バブル半径を読み込む
							}
							else if (strcmp(&aString[0], "SHOT_PARTS") == 0)
							{ // 読み込んだ文字列が SHOT_PARTS の場合

								fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
								fscanf(pFile, "%d", &m_statusInfo.nShotParts);	// 発射パーツを読み込む
							}
							else if (strcmp(&aString[0], "SHOT_POS") == 0)
							{ // 読み込んだ文字列が SHOT_POS の場合

								fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
								fscanf(pFile, "%f", &m_statusInfo.shotPos.x);	// 発射位置Xを読み込む
								fscanf(pFile, "%f", &m_statusInfo.shotPos.y);	// 発射位置Yを読み込む
								fscanf(pFile, "%f", &m_statusInfo.shotPos.z);	// 発射位置Zを読み込む
							}
						} while (strcmp(&aString[0], "END_MAGICSET") != 0);	// 読み込んだ文字列が END_MAGICSET ではない場合ループ
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
		MessageBox(NULL, "魔法セットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}
