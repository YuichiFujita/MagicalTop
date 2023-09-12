//============================================================
//
//	草処理 [weed.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "weed.h"
#include "manager.h"
#include "sceneGame.h"
#include "renderer.h"
#include "texture.h"

#include "collision.h"
#include "player.h"
#include "target.h"
#include "shadow.h"
#include "field.h"
#include "stage.h"

//************************************************************
//	マクロ定義
//************************************************************
#define WEED_PRIO	(2)	// 草の優先順位

#define SHADOW_SIZE	(D3DXVECTOR3(80.0f, 0.0f, 80.0f))	// 影の大きさ
#define SHADOW_ALPHA	(0.2f)	// 影のα値

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CWeed::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\weed000.png",	// 春草テクスチャ
	"data\\TEXTURE\\weed001.png",	// 夏草テクスチャ
	"data\\TEXTURE\\weed002.png",	// 秋草テクスチャ
	"data\\TEXTURE\\weed003.png",	// 冬草テクスチャ
};

//************************************************************
//	子クラス [CWeed] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CWeed::CWeed(void) : CObject3D(CObject::LABEL_WEED, WEED_PRIO)
{
	// メンバ変数をクリア
	m_pShadow = NULL;		// 影の情報
	m_type = TYPE_SPRING;	// 種類
	m_fSinRot = 0.0f;		// なびき向き
}

//============================================================
//	デストラクタ
//============================================================
CWeed::~CWeed()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CWeed::Init(void)
{
	// メンバ変数を初期化
	m_pShadow = NULL;		// 影の情報
	m_type = TYPE_SPRING;	// 種類
	m_fSinRot = (float)(rand() % 629 - 314) * 0.01f;	// なびき向き

	// 影の生成
	m_pShadow = CShadow::Create(CShadow::TEXTURE_NORMAL, SHADOW_SIZE, this, SHADOW_ALPHA, SHADOW_ALPHA);
	if (UNUSED(m_pShadow))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// オブジェクト3Dの初期化
	if (FAILED(CObject3D::Init()))
	{ // 初期化に失敗した場合

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
void CWeed::Uninit(void)
{
	// 影を破棄
	m_pShadow->Uninit();

	// オブジェクト3Dの終了
	CObject3D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CWeed::Update(void)
{
	// 変数を宣言
	D3DXVECTOR3 pos = GetPosition();	// 位置

	// TODO：花とプレイヤーの当たり判定
#if 0
	// プレイヤーとの当たり判定
	if (CollisionPlayer())
	{ // プレイヤーに当たっている場合

		// オブジェクトの終了
		Uninit();

		// 関数を抜ける
		return;
	}
#endif



	// なびき向きを加算
	m_fSinRot += 0.2f;
	useful::NormalizeRot(m_fSinRot);	// 向き正規化

	// 頂点のずれ量を設定
	SetGapPosition(0, D3DXVECTOR3(0.0f, 0.0f, sinf(m_fSinRot) * 15.0f - 25.0f));
	SetGapPosition(1, D3DXVECTOR3(0.0f, 0.0f, sinf(m_fSinRot) * 15.0f - 25.0f));



	// 位置を求める
	pos.y = CSceneGame::GetField()->GetPositionHeight(pos);	// 高さを地面に設定

	// 位置を更新
	SetPosition(pos);

	// 影の更新
	m_pShadow->Update();

	// オブジェクト3Dの更新
	CObject3D::Update();
}

//============================================================
//	描画処理
//============================================================
void CWeed::Draw(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスのポインタ

	// αテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// αテストの有効 / 無効の設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 160);				// αテストの参照値設定

	// オブジェクト3Dの描画
	CObject3D::Draw();

	// αテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// αテストの有効 / 無効の設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// αテストの参照値設定
}

//============================================================
//	生成処理
//============================================================
CWeed *CWeed::Create
(
	const TYPE type,			// 種類
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXVECTOR3& rSize	// 大きさ
)
{
	// 変数を宣言
	D3DXVECTOR3 pos = rPos;	// 座標設定用
	int nTextureID;	// テクスチャインデックス

	// ポインタを宣言
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ
	CWeed *pWeed = NULL;	// 草生成用

	if (UNUSED(pWeed))
	{ // 使用されていない場合

		// メモリ確保
		pWeed = new CWeed;	// 草
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pWeed))
	{ // 確保に成功している場合

		// 草の初期化
		if (FAILED(pWeed->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pWeed;
			pWeed = NULL;

			// 失敗を返す
			return NULL;
		}

		// テクスチャを登録
		nTextureID = pTexture->Regist(mc_apTextureFile[type]);

		// テクスチャを割当
		pWeed->BindTexture(nTextureID);

		// 原点を設定
		pWeed->SetOrigin(ORIGIN_DOWN);

		// 位置を設定
		pos.y = CSceneGame::GetField()->GetPositionHeight(pos);	// 高さを地面に設定
		pWeed->SetPosition(pos);

		// 向きを設定
		pWeed->SetRotation(rRot);

		// 大きさを設定
		pWeed->SetScaling(rSize);

		// カリングを設定
		pWeed->SetCulling(D3DCULL_NONE);

		// ライティングを設定
		pWeed->SetLighting(false);

		// Zテストを設定
		pWeed->SetFunc(D3DCMP_LESSEQUAL);

		// Zバッファの使用状況を設定
		pWeed->SetZEnable(true);

		// 確保したアドレスを返す
		return pWeed;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	ランダム生成処理
//============================================================
void CWeed::RandomSpawn
(
	const int nNum,				// 生成数
	const TYPE type,			// 種類
	const D3DXVECTOR3& rSize	// 大きさ
)
{
	// 変数を宣言
	D3DXVECTOR3 posSet;	// 位置設定用
	D3DXVECTOR3 rotSet;	// 向き設定用

	D3DXVECTOR3 posTarget = CSceneGame::GetTarget()->GetPosition();		// ターゲット位置
	int nLimit = (int)CSceneGame::GetStage()->GetStageLimit().fRadius;	// ステージ範囲

	if (USED(CSceneGame::GetTarget()))
	{ // ターゲットが使用されている場合

		for (int nCntGrow = 0; nCntGrow < nNum; nCntGrow++)
		{ // 生成数分繰り返す

			// 生成位置を設定
			posSet.x = (float)(rand() % (nLimit * 2) - nLimit + 1);
			posSet.y = 0.0f;
			posSet.z = (float)(rand() % (nLimit * 2) - nLimit + 1);

			// 生成位置を補正
			collision::CirclePillar(posSet, posTarget, rSize.x, CSceneGame::GetStage()->GetStageBarrier().fRadius);	// ターゲット内部の生成防止
			CSceneGame::GetStage()->LimitPosition(posSet, rSize.x);	// ステージ範囲外の生成防止

			// 生成向きを設定
			rotSet = D3DXVECTOR3(0.0f, atan2f(posSet.x - posTarget.x, posSet.z - posTarget.z), 0.0f);

			// 草オブジェクトの生成
			CWeed::Create(type, posSet, rotSet, rSize);
		}
	}
}

//============================================================
//	季節の設定処理
//============================================================
void CWeed::SetSeason(const CWaveManager::SEASON season)
{
	// ポインタを宣言
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ

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

				if (pObjCheck->GetLabel() != CObject::LABEL_WEED)
				{ // オブジェクトラベルが草ではない場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				// 引数の季節のテクスチャを登録・割当
				pObjCheck->BindTexture(pTexture->Regist(mc_apTextureFile[season]));

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}
		}
	}
}

//============================================================
//	プレイヤーとの当たり判定
//============================================================
bool CWeed::CollisionPlayer(void)
{
	// 変数を宣言
	bool bHit = false;	// 判定状況

	// ポインタを宣言
	CPlayer *pPlayer = CSceneGame::GetPlayer();	// プレイヤー情報

	if (pPlayer->GetState() != CPlayer::STATE_DEATH)
	{ // プレイヤーが使用されている場合

		// TODO
	}

	// 判定状況を返す
	return bHit;
}
