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
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "collision.h"
#include "debugproc.h"

#include "multiModel.h"
#include "motion.h"
#include "magicManager.h"
#include "target.h"
#include "stage.h"
#include "field.h"
#include "particle3D.h"

//************************************************************
//	マクロ定義
//************************************************************
#define PLAYER_SETUP_TXT	"data\\TXT\\player.txt"	// セットアップテキスト相対パス

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
CPlayer::CPlayer() : CObject(CObject::LABEL_PLAYER)
{
	// メンバ変数をクリア
	memset(&m_apMultiModel[0], 0, sizeof(m_apMultiModel));	// モデルの情報
	m_pMotion		= NULL;				// モーションの情報
	m_pMagic		= NULL;				// 魔法マネージャーの情報
	m_pos			= VEC3_ZERO;		// 現在位置
	m_oldPos		= VEC3_ZERO;		// 過去位置
	m_move			= VEC3_ZERO;		// 移動量
	m_rot			= VEC3_ZERO;		// 現在向き
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
	m_pos			= VEC3_ZERO;		// 現在位置
	m_oldPos		= VEC3_ZERO;		// 過去位置
	m_move			= VEC3_ZERO;		// 移動量
	m_rot			= VEC3_ZERO;		// 現在向き
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

	// モーションオブジェクトの生成
	m_pMotion = CMotion::Create();
	if (UNUSED(m_pMotion))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// セットアップの読み込み
	LoadSetup();

	// モデル情報の設定
	m_pMotion->SetModel(&m_apMultiModel[0], m_nNumModel);

	// モーションの設定
	m_pMotion->Set(MOTION_NEUTRAL);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CPlayer::Uninit(void)
{
	// マルチモデルを破棄
	for (int nCntPlayer = 0; nCntPlayer < MAX_PARTS; nCntPlayer++)
	{ // パーツの最大数分繰り返す

		if (USED(m_apMultiModel[nCntPlayer]))
		{ // パーツが使用中の場合

			// 破棄処理
			if (FAILED(m_apMultiModel[nCntPlayer]->Release(m_apMultiModel[nCntPlayer])))
			{ // 破棄に失敗した場合

				// 例外処理
				assert(false);
			}
		}
	}

	// モーションを破棄
	if (FAILED(m_pMotion->Release(m_pMotion)))
	{ // 破棄に失敗した場合

		// 例外処理
		assert(false);
	}

	// 魔法マネージャーを破棄
	if (FAILED(m_pMagic->Release(m_pMagic)))
	{ // 破棄に失敗した場合

		// 例外処理
		assert(false);
	}

	// プレイヤーを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CPlayer::Update(void)
{
	// 変数を宣言
	MOTION currentMotion = MOTION_NEUTRAL;	// 現在のモーション
	D3DXVECTOR3 posTarget = CManager::GetTarget()->GetPosition();	// ターゲット位置

	// 過去位置を更新
	m_oldPos = m_pos;

	// ターゲットとの距離を設定
	m_fDisTarget = sqrtf((m_pos.x - posTarget.x) * (m_pos.x - posTarget.x) + (m_pos.z - posTarget.z) * (m_pos.z - posTarget.z)) * 0.5f;

	// 移動操作
	currentMotion = Move(currentMotion);

	// 向き更新
	Rot();

	// ジャンプ操作
	currentMotion = Jump(currentMotion);

	// 重力を加算
	m_move.y -= PLAY_GRAVITY;

	// 移動量を加算
	m_pos += m_move;

	// 移動量を減衰
	m_move.x += (0.0f - m_move.x) * PLAY_REV;
	m_move.z += (0.0f - m_move.z) * PLAY_REV;

	// 当たり判定
	CollisionTarget();	// ターゲット
	CollisionEnemy();	// 敵

	// ステージ範囲外の補正
	CManager::GetStage()->LimitPosition(m_pos, PLAY_RADIUS);

	// 着地判定
	if (Land(currentMotion) == MOTION_LANDING)
	{ // 着地していた場合

		if (m_pMotion->GetType() == MOTION_JUMP)
		{ // 再生中モーションがジャンプだった場合

			// 着地モーションの設定
			m_pMotion->Set(MOTION_LANDING);
		}
	}

	// 射撃操作
	currentMotion = Magic(currentMotion);

	// カメラ操作
	Camera();

	// モーション更新
	Motion(currentMotion);
}

//============================================================
//	描画処理
//============================================================
void CPlayer::Draw(void)
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

	for (int nCntPlayer = 0; nCntPlayer < m_nNumModel; nCntPlayer++)
	{ // パーツの総数分繰り返す

		// パーツの描画
		m_apMultiModel[nCntPlayer]->Draw();
	}
}

//============================================================
//	ヒット処理
//============================================================
void CPlayer::Hit(const int nDmg)
{
#if 0
	// 体力からダメージ分減算
	m_pLifeGauge->AddLife(-nDmg);

	if (m_pLifeGauge->GetLife() > 0)
	{ // 生きている場合

		// パーティクル3Dオブジェクトを生成
		CParticle3D::Create(CParticle3D::TYPE_DAMAGE, GetPosition());
	}
	else
#endif

	{ // 死んでいる場合

		// パーティクル3Dオブジェクトを生成
		CParticle3D::Create(CParticle3D::TYPE_DAMAGE, GetPosition(), D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f));
		CParticle3D::Create(CParticle3D::TYPE_DAMAGE, GetPosition(), D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f));

		// TODO：PlayerのUninitどうするのこれ
#if 0
		// プレイヤーオブジェクトの終了
		Uninit();
#endif
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
		
		if (SUCCEEDED(pPlayer->GetResult()))
		{ // オブジェクトの情報設定に成功した場合

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
		else
		{ // オブジェクトの情報設定に失敗した場合

			// メモリ開放
			delete pPlayer;
			pPlayer = NULL;

			// 失敗を返す
			return NULL;
		}
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	位置の設定処理
//============================================================
void CPlayer::SetPosition(const D3DXVECTOR3& rPos)
{
	// 引数の位置を設定
	m_pos = rPos;
}

//============================================================
//	向きの設定処理
//============================================================
void CPlayer::SetRotation(const D3DXVECTOR3& rRot)
{
	// 引数の向きを設定
	m_rot = rRot;

	// 向きの正規化
	useful::NormalizeRot(m_rot.x);
	useful::NormalizeRot(m_rot.y);
	useful::NormalizeRot(m_rot.z);
}

//============================================================
//	マトリックス取得処理
//============================================================
D3DXMATRIX CPlayer::GetMtxWorld(void) const
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
D3DXVECTOR3 CPlayer::GetPosition(void) const
{
	// 位置を返す
	return m_pos;
}

//============================================================
//	向き取得処理
//============================================================
D3DXVECTOR3 CPlayer::GetRotation(void) const
{
	// 向きを返す
	return m_rot;
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
	D3DXVECTOR3			rot				= CManager::GetCamera()->GetRotation();		// カメラの向き
	CStage::StageLimit	limit			= CManager::GetStage()->GetStageLimit();	// ステージの範囲
	float				fAverageLimit	= (fabsf(limit.fLeft) + fabsf(limit.fRight) + fabsf(limit.fNear) + fabsf(limit.fFar)) * 0.25f;	// 範囲の平均

	MOTION	currentMotion = motion;	// 現在のモーション
	int		nRotation;				// 回転方向

	// ポインタを宣言
	CInputKeyboard	*pKeyboard	= CManager::GetKeyboard();	// キーボード
	CInputPad		*pPad		= CManager::GetPad();		// パッド

	// 移動モーションを設定
	currentMotion = MOTION_MOVE;

	// 移動量を設定
	switch (m_rotation)
	{ // 回転方向ごとの処理
	case ROTATION_LEFT:

		// 移動量を更新
		m_move.x += sinf(rot.y - (D3DX_PI * 0.5f)) * (m_fDisTarget * (MAX_MOVEX / fAverageLimit));
		m_move.z += cosf(rot.y - (D3DX_PI * 0.5f)) * (m_fDisTarget * (MAX_MOVEX / fAverageLimit));

		// 目標向きを更新
		m_destRot.y = D3DXToRadian(90) + rot.y;

		// 左回転を設定
		nRotation = 1;

		break;

	case ROTATION_RIGHT:

		// 移動量を更新
		m_move.x -= sinf(rot.y - (D3DX_PI * 0.5f)) * (m_fDisTarget * (MAX_MOVEX / fAverageLimit));
		m_move.z -= cosf(rot.y - (D3DX_PI * 0.5f)) * (m_fDisTarget * (MAX_MOVEX / fAverageLimit));

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
CPlayer::MOTION CPlayer::Magic(MOTION motion)
{
	// 変数を宣言
	MOTION currentMotion = motion;	// 現在のモーション

	// ポインタを宣言
	CInputKeyboard	*pKeyboard = CManager::GetKeyboard();	// キーボード
	CInputPad		*pPad = CManager::GetPad();				// パッド

	// 魔法のロックオン
	m_pMagic->LockOnMagic(m_pos, D3DXVECTOR3(m_rot.x, m_rot.y + D3DX_PI, m_rot.z));

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
CPlayer::MOTION CPlayer::Land(MOTION motion)
{
	// 変数を宣言
	MOTION currentMotion = motion;	// 現在のモーション

	// 着地判定
	if (CManager::GetField()->LandPosition(m_pos, m_move)
	||  CManager::GetStage()->LandPosition(m_pos, m_move, 0.0f))
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
	MOTION animMotion = (MOTION)m_pMotion->GetType();	// 現在再生中のモーション

	if (m_pMotion->IsLoop(animMotion) == true)
	{ // ループするモーションだった場合

		if (m_pMotion->GetType() != motion)
		{ // 現在のモーションが再生中のモーションと一致しない場合

			// 現在のモーションの設定
			m_pMotion->Set(motion);
		}
	}

	// モーションの更新
	m_pMotion->Update();

	// モーションの遷移
	if (m_pMotion->IsFinish())
	{ // モーションが終了していた場合

		switch (m_pMotion->GetType())
		{ // モーションの種類ごとの処理
		case MOTION_ACTION:		// アクション状態

			// 待機モーションに移行
			m_pMotion->Set(MOTION_NEUTRAL);

			// 処理を抜ける
			break;

		case MOTION_JUMP:		// ジャンプ状態

			// 無し

			// 処理を抜ける
			break;

		case MOTION_LANDING:	// 着地状態

			// 待機モーションに移行
			m_pMotion->Set(MOTION_NEUTRAL);

			// 処理を抜ける
			break;
		}
	}
}

//============================================================
//	向き処理
//============================================================
void CPlayer::Rot(void)
{
	// 変数を宣言
	float fDiffRot = 0.0f;	// 差分向き

	// 目標向きの正規化
	useful::NormalizeRot(m_destRot.y);

	// 目標向きまでの差分を計算
	fDiffRot = m_destRot.y - m_rot.y;

	// 差分向きの正規化
	useful::NormalizeRot(fDiffRot);

	// 向きの更新
	m_rot.y += fDiffRot * PLAY_REV_ROTA;

	// 向きの正規化
	useful::NormalizeRot(m_rot.y);
}

//============================================================
//	カメラ処理
//============================================================
void CPlayer::Camera(void)
{
	// 変数を宣言
	D3DXVECTOR3 rot = CManager::GetCamera()->GetDestRotation();	// カメラ向き

	// ポインタを宣言
	CInputKeyboard	*pKeyboard = CManager::GetKeyboard();	// キーボード
	CInputPad		*pPad = CManager::GetPad();				// パッド

	// カメラ操作
	if (pKeyboard->GetPress(DIK_J))
	{ // カメラ左回転の操作が行われた場合

		// カメラ向きを変更
		rot.y -= PLAY_CAM_ROTA;
	}
	if (pKeyboard->GetPress(DIK_L))
	{ // カメラ右回転の操作が行われた場合

		// カメラ向きを変更
		rot.y += PLAY_CAM_ROTA;
	}

	// カメラの向きを設定
	CManager::GetCamera()->SetDestRotation(rot);
}

//============================================================
//	ターゲットとの当たり判定
//============================================================
void CPlayer::CollisionTarget(void)
{
	// ポインタを宣言
	CTarget *pTarget = CManager::GetTarget();	// ターゲット情報

	if (USED(pTarget))
	{ // ターゲットが使用されている場合

		// ターゲットとの衝突判定
		collision::CirclePillar
		( // 引数
			m_pos,					// 判定位置
			pTarget->GetPosition(),	// 判定目標位置
			PLAY_RADIUS,			// 判定半径
			pTarget->GetRadius()	// 判定目標半径
		);
	}
}

//============================================================
//	敵との当たり判定
//============================================================
void CPlayer::CollisionEnemy(void)
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

			// ターゲットとの衝突判定
			collision::CirclePillar
			( // 引数
				m_pos,					// 判定位置
				pObject->GetPosition(),	// 判定目標位置
				PLAY_RADIUS,			// 判定半径
				pObject->GetRadius()	// 判定目標半径
			);
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
	int nModelID	= 0;	// モデルインデックス

	// ポインタを宣言
	CModel *pModel = CManager::GetModel();	// モデルへのポインタ

	// ポーズ代入用の変数を初期化
	memset(&info, 0, sizeof(info));

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;	// ファイルポインタ

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

						// モデルの生成
						m_apMultiModel[nID] = CMultiModel::Create(pos, rot);

						// モデルを登録
						nModelID = pModel->Regist(mc_apModelFile[(MODEL)nID]);

						// モデルを割当
						m_apMultiModel[nID]->BindModel(pModel->GetModel(nModelID));

						// 親モデルの設定
						if (nParentID == NONE_IDX)
						{ // 親がない場合

							// NULLを設定
							m_apMultiModel[nID]->SetParent(NULL);
						}
						else
						{ // 親がいる場合

							// 親のアドレスを設定
							m_apMultiModel[nID]->SetParent(m_apMultiModel[nParentID]);
						}

						// パーツの総数を加算
						m_nNumModel++;
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
										info.aKeyInfo[nNowPose].aKey[nNowKey].pos += m_apMultiModel[nNowKey]->GetPosition();
									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{ // 読み込んだ文字列が ROT の場合

										fscanf(pFile, "%s", &aString[0]);									// = を読み込む (不要)
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);	// X向きを読み込む
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);	// Y向きを読み込む
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);	// Z向きを読み込む

										// 読み込んだ向きにパーツの初期向きを加算
										info.aKeyInfo[nNowPose].aKey[nNowKey].rot += m_apMultiModel[nNowKey]->GetRotation();
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
				m_pMotion->SetInfo(info);
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
