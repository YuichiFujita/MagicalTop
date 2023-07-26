//============================================================
//
//	プレイヤー処理 [player.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "player.h"
#include "manager.h"
#include "sceneGame.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "collision.h"

#include "magicManager.h"
#include "objectGauge2D.h"
#include "shadow.h"
#include "target.h"
#include "stage.h"
#include "field.h"
#include "particle3D.h"

//************************************************************
//	マクロ定義
//************************************************************
#define PLAYER_SETUP_TXT	"data\\TXT\\player.txt"	// セットアップテキスト相対パス

#define PLAY_SHADOW_SIZE	(D3DXVECTOR3(80.0f, 0.0f, 80.0f))	// 影の大きさ

#define MAX_MOVEX		(5.0f)	// 自動歩行時の速度割合用
#define PULSROT_MOVEZ	(20)	// 前後移動時のプレイヤー向きの変更量
#define PLUS_MOVEX		(0.5f)	// 左右回転の移動量の加算量
#define PLAY_MOVEZ		(2.0f)	// 前後の移動量
#define PLAY_REV		(0.2f)	// プレイヤー移動量の減衰係数
#define PLAY_REV_ROTA	(0.15f)	// プレイヤー向き変更の減衰係数
#define PLAY_CAM_ROTA	(0.04f)	// カメラ回転量
#define PLAY_JUMP		(20.0f)	// プレイヤージャンプ量
#define PLAY_GRAVITY	(1.0f)	// プレイヤー重力
#define PLAY_RADIUS		(20.0f)	// プレイヤー半径

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CPlayer::mc_apModelFile[] =	// モデル定数
{
	"data\\MODEL\\PLAYER\\body.x",		// 体モデル
	"data\\MODEL\\PLAYER\\head.x",		// 頭モデル
	"data\\MODEL\\PLAYER\\sleeve_l.x",	// 左腕モデル
	"data\\MODEL\\PLAYER\\sleeve_r.x",	// 右腕モデル
	"data\\MODEL\\PLAYER\\wrist_l.x",	// 左手首モデル
	"data\\MODEL\\PLAYER\\wrist_r.x",	// 右手首モデル
	"data\\MODEL\\PLAYER\\hand_l.x",	// 左手モデル
	"data\\MODEL\\PLAYER\\hand_r.x",	// 右手モデル
	"data\\MODEL\\PLAYER\\pants_l.x",	// 左足モデル
	"data\\MODEL\\PLAYER\\pants_r.x",	// 右足モデル
	"data\\MODEL\\PLAYER\\leg_l.x",		// 左腿モデル
	"data\\MODEL\\PLAYER\\leg_r.x",		// 右腿モデル
	"data\\MODEL\\PLAYER\\shoes_l.x",	// 左靴モデル
	"data\\MODEL\\PLAYER\\shoes_r.x",	// 右靴モデル
	"data\\MODEL\\PLAYER\\knife.x",		// ナイフモデル
};

//************************************************************
//	子クラス [CPlayer] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CPlayer::CPlayer() : CObjectChara(CObject::LABEL_PLAYER)
{
	// メンバ変数をクリア
	m_pMagic		= NULL;				// 魔法マネージャーの情報
	m_pLife			= NULL;				// 体力の情報
	m_pShadow		= NULL;				// 影の情報
	m_oldPos		= VEC3_ZERO;		// 過去位置
	m_move			= VEC3_ZERO;		// 移動量
	m_destRot		= VEC3_ZERO;		// 目標向き
	m_rotation		= ROTATION_LEFT;	// 回転方向
	m_nNumModel		= 0;				// パーツの総数
	m_fDisTarget	= 0.0f;				// ターゲットとの距離
	m_bJump			= false;			// ジャンプ状況
}

//============================================================
//	デストラクタ
//============================================================
CPlayer::~CPlayer()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CPlayer::Init(void)
{
	// メンバ変数を初期化
	m_pMagic		= NULL;				// 魔法マネージャーの情報
	m_pLife			= NULL;				// 体力の情報
	m_pShadow		= NULL;				// 影の情報
	m_oldPos		= VEC3_ZERO;		// 過去位置
	m_move			= VEC3_ZERO;		// 移動量
	m_destRot		= VEC3_ZERO;		// 目標向き
	m_rotation		= ROTATION_LEFT;	// 回転方向
	m_nNumModel		= 0;				// パーツの総数
	m_fDisTarget	= 0.0f;				// ターゲットとの距離
	m_bJump			= true;				// ジャンプ状況

	// 魔法マネージャーの生成
	m_pMagic = CMagicManager::Create();
	if (UNUSED(m_pMagic))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 体力の生成
	m_pLife = CObjectGauge2D::Create
	( // 引数
		CObject::LABEL_LIFE,
		100,
		60,
		D3DXVECTOR3(260.0f, 640.0f, 0.0f),
		D3DXVECTOR3(200.0f, 30.0f, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)
	);
	if (UNUSED(m_pLife))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 影の生成
	m_pShadow = CShadow::Create(CShadow::TEXTURE_NORMAL, PLAY_SHADOW_SIZE, this);
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

	// セットアップの読み込み
	LoadSetup();

	// モデル情報の設定
	SetModelInfo();

	// モーションの設定
	SetMotion(MOTION_NEUTRAL);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CPlayer::Uninit(void)
{
	// 魔法マネージャーを破棄
	if (FAILED(m_pMagic->Release(m_pMagic)))
	{ // 破棄に失敗した場合

		// 例外処理
		assert(false);
	}

	// 影を破棄
	m_pShadow->Uninit();

	// オブジェクトキャラクターの終了
	CObjectChara::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CPlayer::Update(void)
{
	// 変数を宣言
	MOTION currentMotion  = MOTION_NEUTRAL;	// 現在のモーション
	D3DXVECTOR3 posPlayer = GetPosition();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = GetRotation();	// プレイヤー向き
	D3DXVECTOR3 posTarget = VEC3_ZERO/*CManager::GetTarget()->GetPosition()*/;	// ターゲット位置	// TODO：ターゲット位置の取得

	// 過去位置を更新
	m_oldPos = posPlayer;

	// ターゲットとの距離を設定
	m_fDisTarget = sqrtf((posPlayer.x - posTarget.x) * (posPlayer.x - posTarget.x)+ (posPlayer.z - posTarget.z) * (posPlayer.z - posTarget.z)) * 0.5f;

	// 移動操作
	currentMotion = Move(currentMotion);

	// 向き更新
	Rot(rotPlayer);

	// ジャンプ操作
	currentMotion = Jump(currentMotion);

	// 位置更新
	Pos(posPlayer);

	// 当たり判定
	CollisionTarget(posPlayer);	// ターゲット
	CollisionEnemy(posPlayer);	// 敵

	// ステージ範囲外の補正
	CSceneGame::GetStage()->LimitPosition(posPlayer, PLAY_RADIUS);

	// 着地判定
	if (Land(currentMotion, posPlayer) == MOTION_LANDING)
	{ // 着地していた場合

		if (GetMotionType() == MOTION_JUMP)
		{ // 再生中モーションがジャンプだった場合

			// 着地モーションの設定
			SetMotion(MOTION_LANDING);
		}
	}

	// 射撃操作
	currentMotion = Magic(currentMotion, posPlayer);

	// 位置を更新
	SetPosition(posPlayer);

	// 向きを更新
	SetRotation(rotPlayer);

	// 影の更新
	m_pShadow->Update();

	// モーション・オブジェクトキャラクターの更新
	Motion(currentMotion);
}

//============================================================
//	描画処理
//============================================================
void CPlayer::Draw(void)
{
	// オブジェクトキャラクターの描画
	CObjectChara::Draw();
}

//============================================================
//	ヒット処理
//============================================================
void CPlayer::Hit(const int nDmg)
{
	// 変数を宣言
	D3DXVECTOR3 pos = GetPosition();	// プレイヤー位置

	if (IsDeath() != true)
	{ // 死亡フラグが立っていない場合

		// 体力からダメージ分減算
		m_pLife->AddNum(-nDmg);

		if (m_pLife->GetNum() > 0)
		{ // 生きている場合

			// パーティクル3Dオブジェクトを生成
			CParticle3D::Create(CParticle3D::TYPE_DAMAGE, pos);
		}
		else
		{ // 死んでいる場合

			// パーティクル3Dオブジェクトを生成
			CParticle3D::Create(CParticle3D::TYPE_DAMAGE, pos, D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f));
			CParticle3D::Create(CParticle3D::TYPE_DAMAGE, pos, D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f));

			// 更新と描画を停止
			SetEnableUpdate(false);
			SetEnableDraw(false);

			// 魔法ロックオン全削除
			m_pMagic->DeleteLockOn();
		}
	}
}

//============================================================
//	生成処理
//============================================================
CPlayer *CPlayer::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot)
{
	// ポインタを宣言
	CPlayer *pPlayer = NULL;	// プレイヤー生成用

	if (UNUSED(pPlayer))
	{ // 使用されていない場合

		// メモリ確保
		pPlayer = new CPlayer;	// プレイヤー
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pPlayer))
	{ // 使用されている場合

		// プレイヤーの初期化
		if (FAILED(pPlayer->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pPlayer;
			pPlayer = NULL;

			// 失敗を返す
			return NULL;
		}

		// 位置を設定
		pPlayer->SetPosition(rPos);

		// 向きを設定
		pPlayer->SetRotation(rRot);

		// 確保したアドレスを返す
		return pPlayer;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	更新状況の設定処理
//============================================================
void CPlayer::SetEnableUpdate(const bool bUpdate)
{
	// 引数の更新状況を設定
	CObject::SetEnableUpdate(bUpdate);		// 自身
	m_pShadow->SetEnableUpdate(bUpdate);	// 影
}

//============================================================
//	描画状況の設定処理
//============================================================
void CPlayer::SetEnableDraw(const bool bDraw)
{
	// 引数の描画状況を設定
	CObject::SetEnableDraw(bDraw);		// 自身
	m_pShadow->SetEnableDraw(bDraw);	// 影
}

//============================================================
//	マトリックス取得処理
//============================================================
D3DXMATRIX CPlayer::GetMtxWorld(void) const
{
	// 変数を宣言
	D3DXMATRIX  mtxRot, mtxTrans, mtxWorld;	// 計算用マトリックス
	D3DXVECTOR3 posPlayer = GetPosition();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = GetRotation();	// プレイヤー向き

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotPlayer.y, rotPlayer.x, rotPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, posPlayer.x, posPlayer.y, posPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ワールドマトリックスを返す
	return mtxWorld;
}

//============================================================
//	ターゲットとの距離取得処理
//============================================================
float CPlayer::GetDistanceTarget(void) const
{
	// ターゲットとの距離を返す
	return m_fDisTarget;
}

//============================================================
//	半径取得処理
//============================================================
float CPlayer::GetRadius(void) const
{
	// 半径を返す
	return PLAY_RADIUS;
}

//============================================================
//	移動処理
//============================================================
CPlayer::MOTION CPlayer::Move(MOTION motion)
{
	// 変数を宣言
	D3DXVECTOR3 rot = CManager::GetCamera()->GetRotation();			// カメラの向き
	float fLimit = CSceneGame::GetStage()->GetStageLimit().fRadius;	// ステージ範囲

	MOTION	currentMotion = motion;	// 現在のモーション
	int		nRotation;				// 回転方向

	// ポインタを宣言
	CInputKeyboard	*pKeyboard	= CManager::GetKeyboard();	// キーボード
	CInputPad		*pPad		= CManager::GetPad();		// パッド

#if 1	// 通常歩行
	// 移動モーションを設定
	currentMotion = MOTION_MOVE;

	// 移動量を設定
	switch (m_rotation)
	{ // 回転方向ごとの処理
	case ROTATION_LEFT:

		// 移動量を更新
		m_move.x += sinf(rot.y - (D3DX_PI * 0.5f)) * (m_fDisTarget * (MAX_MOVEX / fLimit));
		m_move.z += cosf(rot.y - (D3DX_PI * 0.5f)) * (m_fDisTarget * (MAX_MOVEX / fLimit));

		// 目標向きを更新
		m_destRot.y = D3DXToRadian(90) + rot.y;

		// 左回転を設定
		nRotation = 1;

		break;

	case ROTATION_RIGHT:

		// 移動量を更新
		m_move.x -= sinf(rot.y - (D3DX_PI * 0.5f)) * (m_fDisTarget * (MAX_MOVEX / fLimit));
		m_move.z -= cosf(rot.y - (D3DX_PI * 0.5f)) * (m_fDisTarget * (MAX_MOVEX / fLimit));

		// 目標向きを更新
		m_destRot.y = D3DXToRadian(270) + rot.y;

		// 右回転を設定
		nRotation = -1;

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// 移動操作
	if (pKeyboard->GetPress(DIK_W))
	{ // 奥移動の操作が行われた場合

		// 移動量を更新
		m_move.x += sinf(rot.y) * PLAY_MOVEZ;
		m_move.z += cosf(rot.y) * PLAY_MOVEZ;

		// 目標向きを更新
		m_destRot.y = D3DXToRadian(180 - (PULSROT_MOVEZ * nRotation)) + rot.y;
	}
	else if (pKeyboard->GetPress(DIK_S))
	{ // 手前移動の操作が行われた場合

		// 移動量を更新
		m_move.x -= sinf(rot.y) * PLAY_MOVEZ;
		m_move.z -= cosf(rot.y) * PLAY_MOVEZ;

		// 目標向きを更新
		m_destRot.y = D3DXToRadian(0 + (PULSROT_MOVEZ * nRotation)) + rot.y;
	}

	// 移動量増加・反転操作
	else if (pKeyboard->GetPress(DIK_A))
	{ // 左移動の操作が行われた場合

		if (m_rotation == ROTATION_LEFT)
		{ // 回転方向が左の場合

			// 移動量を更新
			m_move.x += sinf(rot.y - (D3DX_PI * 0.5f)) * PLUS_MOVEX;
			m_move.z += cosf(rot.y - (D3DX_PI * 0.5f)) * PLUS_MOVEX;

			// 目標向きを更新
			m_destRot.y = D3DXToRadian(90) + rot.y;
		}
		else
		{ // 回転方向が右の場合

			// 回転方向を左にする
			m_rotation = ROTATION_LEFT;
		}
	}
	else if (pKeyboard->GetPress(DIK_D))
	{ // 右移動の操作が行われた場合

		if (m_rotation == ROTATION_RIGHT)
		{ // 回転方向が右の場合

			// 移動量を更新
			m_move.x -= sinf(rot.y - (D3DX_PI * 0.5f)) * PLUS_MOVEX;
			m_move.z -= cosf(rot.y - (D3DX_PI * 0.5f)) * PLUS_MOVEX;

			// 目標向きを更新
			m_destRot.y = D3DXToRadian(270) + rot.y;
		}
		else
		{ // 回転方向が左の場合

			// 回転方向を右にする
			m_rotation = ROTATION_RIGHT;
		}
	}
#else	// デバッグ用歩行
	// 移動操作
	if (pKeyboard->GetPress(DIK_W))
	{ // 奥移動の操作が行われた場合

		// 移動モーションを設定
		currentMotion = MOTION_MOVE;

		if (pKeyboard->GetPress(DIK_A))
		{ // 左移動の操作も行われた場合 (左奥移動)

			// 移動量を更新
			m_move.x += sinf(rot.y - (D3DX_PI * 0.25f)) * 5.0f;
			m_move.z += cosf(rot.y - (D3DX_PI * 0.25f)) * 5.0f;

			// 目標向きを更新
			m_destRot.y = D3DXToRadian(135) + rot.y;
		}
		else if (pKeyboard->GetPress(DIK_D))
		{ // 右移動の操作も行われた場合 (右奥移動)

			// 移動量を更新
			m_move.x -= sinf(rot.y - (D3DX_PI * 0.75f)) * 5.0f;
			m_move.z -= cosf(rot.y - (D3DX_PI * 0.75f)) * 5.0f;

			// 目標向きを更新
			m_destRot.y = D3DXToRadian(225) + rot.y;
		}
		else
		{ // 奥移動の操作だけが行われた場合 (奥移動)

			// 移動量を更新
			m_move.x += sinf(rot.y) * 5.0f;
			m_move.z += cosf(rot.y) * 5.0f;

			// 目標向きを更新
			m_destRot.y = D3DXToRadian(180) + rot.y;
		}
	}
	else if (pKeyboard->GetPress(DIK_S))
	{ // 手前移動の操作が行われた場合

		// 移動モーションを設定
		currentMotion = MOTION_MOVE;

		if (pKeyboard->GetPress(DIK_A))
		{ // 左移動の操作も行われた場合 (左手前移動)

			// 移動量を更新
			m_move.x += sinf(rot.y - (D3DX_PI * 0.75f)) * 5.0f;
			m_move.z += cosf(rot.y - (D3DX_PI * 0.75f)) * 5.0f;

			// 目標向きを更新
			m_destRot.y = D3DXToRadian(45) + rot.y;
		}
		else if (pKeyboard->GetPress(DIK_D))
		{ // 右移動の操作も行われた場合 (右手前移動)

			// 移動量を更新
			m_move.x -= sinf(rot.y - (D3DX_PI * 0.25f)) * 5.0f;
			m_move.z -= cosf(rot.y - (D3DX_PI * 0.25f)) * 5.0f;

			// 目標向きを更新
			m_destRot.y = D3DXToRadian(315) + rot.y;
		}
		else
		{ // 手前移動の操作だけが行われた場合 (手前移動)

			// 移動量を更新
			m_move.x -= sinf(rot.y) * 5.0f;
			m_move.z -= cosf(rot.y) * 5.0f;

			// 目標向きを更新
			m_destRot.y = D3DXToRadian(0) + rot.y;
		}
	}
	else if (pKeyboard->GetPress(DIK_A))
	{ // 左移動の操作が行われた場合

		// 移動モーションを設定
		currentMotion = MOTION_MOVE;

		// 移動量を更新
		m_move.x += sinf(rot.y - (D3DX_PI * 0.5f)) * 5.0f;
		m_move.z += cosf(rot.y - (D3DX_PI * 0.5f)) * 5.0f;

		// 目標向きを更新
		m_destRot.y = D3DXToRadian(90) + rot.y;
	}
	else if (pKeyboard->GetPress(DIK_D))
	{ // 右移動の操作が行われた場合

		// 移動モーションを設定
		currentMotion = MOTION_MOVE;

		// 移動量を更新
		m_move.x -= sinf(rot.y - (D3DX_PI * 0.5f)) * 5.0f;
		m_move.z -= cosf(rot.y - (D3DX_PI * 0.5f)) * 5.0f;

		// 目標向きを更新
		m_destRot.y = D3DXToRadian(270) + rot.y;
	}
#endif

	// 現在のモーションを返す
	return currentMotion;		// TODO：もはや待機モーションいらない
}

//============================================================
//	ジャンプ処理
//============================================================
CPlayer::MOTION CPlayer::Jump(MOTION motion)
{
	// 変数を宣言
	MOTION currentMotion = motion;	// 現在のモーション

	// ポインタを宣言
	CInputKeyboard	*pKeyboard = CManager::GetKeyboard();	// キーボード
	CInputPad		*pPad = CManager::GetPad();				// パッド

	// ジャンプ操作
	if (m_bJump == false)
	{ // ジャンプしていない場合

		if (pKeyboard->GetTrigger(DIK_SPACE))
		{ // ジャンプの操作が行われた場合

			// ジャンプモーションを設定
			currentMotion = MOTION_JUMP;

			// 上移動量
			m_move.y += PLAY_JUMP;

			// ジャンプしている状態にする
			m_bJump = true;
		}
	}

	// 現在のモーションを返す
	return currentMotion;
}

//============================================================
//	魔法処理
//============================================================
CPlayer::MOTION CPlayer::Magic(MOTION motion, D3DXVECTOR3& rPos)
{
	// 変数を宣言
	MOTION currentMotion = motion;	// 現在のモーション

	// ポインタを宣言
	CInputKeyboard	*pKeyboard = CManager::GetKeyboard();	// キーボード
	CInputPad		*pPad = CManager::GetPad();				// パッド

	// 魔法のロックオン
	m_pMagic->LockOnMagic(rPos);

	if (pKeyboard->GetTrigger(DIK_RETURN))
	{ // 魔法の操作が行われた場合

		// アクションモーションを設定
		currentMotion = MOTION_ACTION;

		// 魔法の発射
		m_pMagic->ShotMagic();
	}

	// 現在のモーションを返す
	return currentMotion;
}

//============================================================
//	着地処理
//============================================================
CPlayer::MOTION CPlayer::Land(MOTION motion, D3DXVECTOR3& rPos)
{
	// 変数を宣言
	MOTION currentMotion  = motion;			// 現在のモーション

	// 着地判定
	if (CSceneGame::GetField()->LandPosition(rPos, m_move)
	||  CSceneGame::GetStage()->LandPosition(rPos, m_move, 0.0f))
	{ // プレイヤーが着地していた場合

		// 着地モーションを設定
		currentMotion = MOTION_LANDING;

		// ジャンプしていない状態にする
		m_bJump = false;
	}
	else
	{ // 着地していない場合

		// ジャンプしている状態にする
		m_bJump = true;
	}

	// 現在のモーションを返す
	return currentMotion;
}

//============================================================
//	モーション処理
//============================================================
void CPlayer::Motion(MOTION motion)
{
	// 変数を宣言
	MOTION animMotion = (MOTION)GetMotionType();	// 現在再生中のモーション

	if (IsMotionLoop(animMotion) == true)
	{ // ループするモーションだった場合

		if (animMotion != motion)
		{ // 現在のモーションが再生中のモーションと一致しない場合

			// 現在のモーションの設定
			SetMotion(motion);
		}
	}

	// オブジェクトキャラクターの更新
	CObjectChara::Update();

	// モーションの遷移
	if (IsMotionFinish())
	{ // モーションが終了していた場合

		switch (GetMotionType())
		{ // モーションの種類ごとの処理
		case MOTION_ACTION:		// アクション状態

			// 待機モーションに移行
			SetMotion(MOTION_NEUTRAL);

			// 処理を抜ける
			break;

		case MOTION_JUMP:		// ジャンプ状態

			// 無し

			// 処理を抜ける
			break;

		case MOTION_LANDING:	// 着地状態

			// 待機モーションに移行
			SetMotion(MOTION_NEUTRAL);

			// 処理を抜ける
			break;
		}
	}
}

//============================================================
//	位置処理
//============================================================
void CPlayer::Pos(D3DXVECTOR3& rPos)
{
	// 重力を加算
	m_move.y -= PLAY_GRAVITY;

	// 移動量を加算
	rPos += m_move;

	// 移動量を減衰
	m_move.x += (0.0f - m_move.x) * PLAY_REV;
	m_move.z += (0.0f - m_move.z) * PLAY_REV;
}

//============================================================
//	向き処理
//============================================================
void CPlayer::Rot(D3DXVECTOR3& rRot)
{
	// 変数を宣言
	float fDiffRot = 0.0f;	// 差分向き

	// 目標向きの正規化
	useful::NormalizeRot(m_destRot.y);

	// 目標向きまでの差分を計算
	fDiffRot = m_destRot.y - rRot.y;

	// 差分向きの正規化
	useful::NormalizeRot(fDiffRot);

	// 向きの更新
	rRot.y += fDiffRot * PLAY_REV_ROTA;

	// 向きの正規化
	useful::NormalizeRot(rRot.y);
}

//============================================================
//	ターゲットとの当たり判定
//============================================================
void CPlayer::CollisionTarget(D3DXVECTOR3& rPos)
{
	// ポインタを宣言
	CTarget *pTarget = CSceneGame::GetTarget();	// ターゲット情報

	if (USED(pTarget))
	{ // ターゲットが使用されている場合

		// ターゲットとの衝突判定
		collision::CirclePillar
		( // 引数
			rPos,					// 判定位置
			pTarget->GetPosition(),	// 判定目標位置
			PLAY_RADIUS,			// 判定半径
			pTarget->GetRadius()	// 判定目標半径
		);
	}
}

//============================================================
//	敵との当たり判定
//============================================================
void CPlayer::CollisionEnemy(D3DXVECTOR3& rPos)
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

				// 敵との衝突判定
				collision::CirclePillar
				( // 引数
					rPos,						// 判定位置
					pObjCheck->GetPosition(),	// 判定目標位置
					PLAY_RADIUS,				// 判定半径
					pObjCheck->GetRadius()		// 判定目標半径
				);

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}
		}
	}
}

//============================================================
//	セットアップ処理
//============================================================
void CPlayer::LoadSetup(void)
{
	// 変数を宣言
	CMotion::MotionInfo info;		// ポーズの代入用
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	int nID			= 0;	// インデックスの代入用
	int nParentID	= 0;	// 親インデックスの代入用
	int nNowPose	= 0;	// 現在のポーズ番号
	int nNowKey		= 0;	// 現在のキー番号
	int nLoop		= 0;	// ループのON/OFFの変換用
	int nEnd		= 0;	// テキスト読み込み終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;	// ファイルポインタ

	// ポーズ代入用の変数を初期化
	memset(&info, 0, sizeof(info));

	// ファイルを読み込み形式で開く
	pFile = fopen(PLAYER_SETUP_TXT, "r");

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

					if (strcmp(&aString[0], "PARTSSET") == 0)
					{ // 読み込んだ文字列が PARTSSET の場合

						do
						{ // 読み込んだ文字列が END_PARTSSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "INDEX") == 0)
							{ // 読み込んだ文字列が INDEX の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nID);			// モデルのインデックスを読み込む
							}
							else if (strcmp(&aString[0], "PARENT") == 0)
							{ // 読み込んだ文字列が PARENT の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nParentID);	// モデルの親のインデックスを読み込む
							}
							else if (strcmp(&aString[0], "POS") == 0)
							{ // 読み込んだ文字列が POS の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &pos.x);		// X座標を読み込む
								fscanf(pFile, "%f", &pos.y);		// Y座標を読み込む
								fscanf(pFile, "%f", &pos.z);		// Z座標を読み込む
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // 読み込んだ文字列が ROT の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &rot.x);		// X向きを読み込む
								fscanf(pFile, "%f", &rot.y);		// Y向きを読み込む
								fscanf(pFile, "%f", &rot.z);		// Z向きを読み込む
							}
						} while (strcmp(&aString[0], "END_PARTSSET") != 0);	// 読み込んだ文字列が END_PARTSSET ではない場合ループ

						// パーツ情報の設定
						CObjectChara::SetPartsInfo(nID, nParentID, pos, rot, mc_apModelFile[nID]);
					}
				} while (strcmp(&aString[0], "END_CHARACTERSET") != 0);		// 読み込んだ文字列が END_CHARACTERSET ではない場合ループ
			}

			// モーションの設定
			else if (strcmp(&aString[0], "MOTIONSET") == 0)
			{ // 読み込んだ文字列が MOTIONSET の場合

				// 現在のポーズ番号を初期化
				nNowPose = 0;

				do
				{ // 読み込んだ文字列が END_MOTIONSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "LOOP") == 0)
					{ // 読み込んだ文字列が LOOP の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nLoop);		// ループのON/OFFを読み込む

						// 読み込んだ値をbool型に変換
						info.bLoop = (nLoop == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "NUM_KEY") == 0)
					{ // 読み込んだ文字列が NUM_KEY の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &info.nNumKey);	// キーの総数を読み込む
					}
					else if (strcmp(&aString[0], "KEYSET") == 0)
					{ // 読み込んだ文字列が KEYSET の場合

						// 現在のキー番号を初期化
						nNowKey = 0;

						do
						{ // 読み込んだ文字列が END_KEYSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "FRAME") == 0)
							{ // 読み込んだ文字列が FRAME の場合

								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%d", &info.aKeyInfo[nNowPose].nFrame);	// キーが切り替わるまでのフレーム数を読み込む
							}
							else if (strcmp(&aString[0], "KEY") == 0)
							{ // 読み込んだ文字列が KEY の場合

								do
								{ // 読み込んだ文字列が END_KEY ではない場合ループ

									// ファイルから文字列を読み込む
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "POS") == 0)
									{ // 読み込んだ文字列が POS の場合

										fscanf(pFile, "%s", &aString[0]);									// = を読み込む (不要)
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.x);	// X位置を読み込む
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.y);	// Y位置を読み込む
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.z);	// Z位置を読み込む

										// 読み込んだ位置にパーツの初期位置を加算
										info.aKeyInfo[nNowPose].aKey[nNowKey].pos += GetPartsPosition(nNowKey);
									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{ // 読み込んだ文字列が ROT の場合

										fscanf(pFile, "%s", &aString[0]);									// = を読み込む (不要)
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);	// X向きを読み込む
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);	// Y向きを読み込む
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);	// Z向きを読み込む

										// 読み込んだ向きにパーツの初期向きを加算
										info.aKeyInfo[nNowPose].aKey[nNowKey].rot += GetPartsRotation(nNowKey);

										// 初期向きを正規化
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);
									}

								} while (strcmp(&aString[0], "END_KEY") != 0);	// 読み込んだ文字列が END_KEY ではない場合ループ

								// 現在のキー番号を加算
								nNowKey++;
							}
						} while (strcmp(&aString[0], "END_KEYSET") != 0);	// 読み込んだ文字列が END_KEYSET ではない場合ループ

						// 現在のポーズ番号を加算
						nNowPose++;
					}
				} while (strcmp(&aString[0], "END_MOTIONSET") != 0);	// 読み込んだ文字列が END_MOTIONSET ではない場合ループ

				// モーション情報の設定
				CObjectChara::SetMotionInfo(info);
			}

		} while (nEnd != EOF);	// 読み込んだ文字列が EOF ではない場合ループ
		
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "プレイヤーセットアップファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}
