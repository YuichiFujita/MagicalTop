//============================================================
//
//	マナフラワー処理 [flower.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "flower.h"
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
#define FLOWER_PRIO	(3)	// マナフラワーの優先順位

#define SHADOW_SIZE	(D3DXVECTOR3(80.0f, 0.0f, 80.0f))	// 影の大きさ
#define SHADOW_ALPHA	(0.2f)	// 影のα値

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CFlower::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\magicFlower000.png",	// マナフラワーテクスチャ
};
int CFlower::m_nNumAll = 0;	// マナフラワーの総数

//************************************************************
//	子クラス [CFlower] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CFlower::CFlower(void) : CObjectBillboard(CObject::LABEL_FLOWER, FLOWER_PRIO)
{
	// メンバ変数をクリア
	m_pShadow = NULL;		// 影の情報
	m_type = TYPE_NORMAL;	// 種類
	m_nLife = 0;			// 寿命

	// マナフラワーの総数を加算
	m_nNumAll++;
}

//============================================================
//	デストラクタ
//============================================================
CFlower::~CFlower()
{
	// マナフラワーの総数を減算
	m_nNumAll--;
}

//============================================================
//	初期化処理
//============================================================
HRESULT CFlower::Init(void)
{
	// メンバ変数を初期化
	m_type = TYPE_NORMAL;	// 種類
	m_nLife = 0;			// 寿命

	// 影の生成
	m_pShadow = CShadow::Create(CShadow::TEXTURE_NORMAL, SHADOW_SIZE, this, SHADOW_ALPHA, SHADOW_ALPHA);
	if (UNUSED(m_pShadow))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// オブジェクトビルボードの初期化
	if (FAILED(CObjectBillboard::Init()))
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
void CFlower::Uninit(void)
{
	// 影を破棄
	m_pShadow->Uninit();

	// オブジェクトビルボードの終了
	CObjectBillboard::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CFlower::Update(void)
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

	// 位置を求める
	pos.y = CSceneGame::GetField()->GetPositionHeight(pos);	// 高さを地面に設定

	// 位置を更新
	SetPosition(pos);

	// 影の更新
	m_pShadow->Update();

	// オブジェクトビルボードの更新
	CObjectBillboard::Update();
}

//============================================================
//	描画処理
//============================================================
void CFlower::Draw(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスのポインタ

	// αテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// αテストの有効 / 無効の設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 160);				// αテストの参照値設定

	// オブジェクトビルボードの描画
	CObjectBillboard::Draw();

	// αテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// αテストの有効 / 無効の設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// αテストの参照値設定
}

//============================================================
//	生成処理
//============================================================
CFlower *CFlower::Create
(
	const TYPE type,			// 種類
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rSize,	// 大きさ
	const int nLife				// 体力
)
{
	// 変数を宣言
	D3DXVECTOR3 pos = rPos;	// 座標設定用
	int nTextureID;	// テクスチャインデックス

	// ポインタを宣言
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ
	CFlower *pFlower = NULL;	// マナフラワー生成用

	if (UNUSED(pFlower))
	{ // 使用されていない場合

		// メモリ確保
		pFlower = new CFlower;	// マナフラワー
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pFlower))
	{ // 確保に成功している場合

		// マナフラワーの初期化
		if (FAILED(pFlower->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pFlower;
			pFlower = NULL;

			// 失敗を返す
			return NULL;
		}

		// テクスチャを登録
		nTextureID = pTexture->Regist(mc_apTextureFile[type]);

		// テクスチャを割当
		pFlower->BindTexture(nTextureID);

		// 原点を設定
		pFlower->SetOrigin(ORIGIN_DOWN);

		// 位置を設定
		pos.y = CSceneGame::GetField()->GetPositionHeight(pos);	// 高さを地面に設定
		pFlower->SetPosition(pos);

		// 大きさを設定
		pFlower->SetScaling(rSize);

		// 体力を設定
		pFlower->SetLife(nLife);

		// 回転を設定
		pFlower->SetRotate(ROTATE_LATERAL);

		// Zテストを設定
		pFlower->SetFunc(D3DCMP_LESSEQUAL);

		// Zバッファの使用状況を設定
		pFlower->SetZEnable(true);

		// 確保したアドレスを返す
		return pFlower;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	ランダム生成処理
//============================================================
void CFlower::RandomSpawn
(
	const int nNum,				// 生成数
	const TYPE type,			// 種類
	const D3DXVECTOR3& rSize,	// 大きさ
	const int nLife				// 体力
)
{
	// 変数を宣言
	D3DXVECTOR3 pos;	// 位置設定用
	int nLimit = (int)CSceneGame::GetStage()->GetStageLimit().fRadius;	// ステージ範囲

	// ポインタを宣言
	CTarget *pTarget = CSceneGame::GetTarget();	// ターゲット情報

	if (USED(CSceneGame::GetTarget()))
	{ // ターゲットが使用されている場合

		for (int nCntGrow = 0; nCntGrow < nNum; nCntGrow++)
		{ // 生成数分繰り返す

			// 生成位置を設定
			pos.x = (float)(rand() % (nLimit * 2) - nLimit + 1);
			pos.y = 0.0f;
			pos.z = (float)(rand() % (nLimit * 2) - nLimit + 1);

			// 生成位置を補正
			collision::CirclePillar(pos, pTarget->GetPosition(), rSize.x, pTarget->GetRadius());	// ターゲット内部の生成防止
			CSceneGame::GetStage()->LimitPosition(pos, rSize.x);									// ステージ範囲外の生成防止

			// マナフラワーオブジェクトの生成
			CFlower::Create(type, pos, rSize, nLife);
		}
	}
}

//============================================================
//	総数取得処理
//============================================================
int CFlower::GetNumAll(void)
{
	// 現在のマナフラワーの総数を返す
	return m_nNumAll;
}

//============================================================
//	体力の設定処理
//============================================================
void CFlower::SetLife(const int nLife)
{
	// 引数の体力を設定
	m_nLife = nLife;
}

//============================================================
//	プレイヤーとの当たり判定
//============================================================
bool CFlower::CollisionPlayer(void)
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
