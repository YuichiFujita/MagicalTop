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
#include "texture.h"
#include "collision.h"

#include "multiModel.h"
#include "magicManager.h"
#include "expManager.h"
#include "levelupManager.h"
#include "objectGauge3D.h"
#include "gaugeStar.h"
#include "shadow.h"
#include "objectOrbit.h"
#include "enemy.h"
#include "target.h"
#include "stage.h"
#include "field.h"
#include "particle3D.h"

//************************************************************
//	マクロ定義
//************************************************************
#define PLAYER_SETUP_TXT	"data\\TXT\\player.txt"				// セットアップテキスト相対パス
#define PLAY_SHADOW_SIZE	(D3DXVECTOR3(80.0f, 0.0f, 80.0f))	// 影の大きさ
#define PLAY_ORBIT_COL		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f))	// 軌跡の色
#define PLAY_ORBIT_PRIO		(4)		// 軌跡の優先順位

#define GAUGE_PLUS_Y	(140.0f)	// ゲージY位置加算量
#define GAUGE_GAUGESIZE	(D3DXVECTOR3(110.0f, 15.0f, 0.0f))	// ゲージ大きさ
#define GAUGE_FRONTCOL	(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f))	// 表ゲージ色
#define GAUGE_BACKCOL	(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	// 裏ゲージ色

#define STAR_ADDPOS		(D3DXVECTOR3(0.0f, 50.0f, 0.0f))	// 表示位置の加算量
#define STAR_MAX_GAUGE	(60)	// 最大ゲージ量
#define STAR_RADIUS		(40.0f)	// 半径
#define STAR_HEAL		(4)		// ゲージ回復量
#define STAR_WAIT_HEAL	(120)	// 回復待機カウント
#define STAR_DIS_CENTER	(65.0f)	// 中心からの距離
#define STAR_FLICKER	(8.0f)	// 揺らめき量

#define VORTEX_ADDROT	(0.085f)	// 渦巻きこまれ時のプレイヤー回転速度
#define VORTEX_ADDDIS	(2.5f)		// 渦巻きこまれ時のプレイヤーと渦の距離離れ量
#define VORTEX_ADDPOSY	(6.5f)		// 渦巻きこまれ時の高さ座標加算量
#define VORTEX_HITDMG	(50)		// 渦巻きこまれ時のダメージ量

#define MAX_MOVEX		(5.0f)		// 自動歩行時の速度割合用
#define PULSROT_MOVEZ	(20)		// 前後移動時のプレイヤー向きの変更量
#define PLUS_MOVEX		(0.5f)		// 左右回転の移動量の加算量
#define PLAY_MOVEZ		(2.0f)		// 前後の移動量
#define PLAY_AWAY_REV	(0.08f)		// 空中のプレイヤー移動量の減衰係数
#define PLAY_LAND_REV	(0.2f)		// 地上のプレイヤー移動量の減衰係数
#define PLAY_REV_ROTA	(0.15f)		// プレイヤー向き変更の減衰係数
#define PLAY_JUMP		(20.0f)		// プレイヤージャンプ量
#define PLAY_GRAVITY	(1.0f)		// プレイヤー重力
#define PLAY_RADIUS		(20.0f)		// プレイヤー半径
#define PLAY_LIFE		(150)		// プレイヤー体力
#define HURRICANE_DMG	(50)		// バリアのダメージ量
#define ENE_HIT_DMG		(30)		// 敵ヒット時のダメージ量

#define FADE_LEVEL		(0.01f)		// フェードのα値の加減量
#define AWAY_SIDE_MOVE	(25.0f)		// 吹っ飛び時の横移動量
#define AWAY_UP_MOVE	(18.0f)		// 吹っ飛び時の上移動量
#define INVULN_CNT		(16)		// 無敵状態に移行するまでのカウンター
#define NORMAL_CNT		(180)		// 通常状態に移行するまでのカウンター

#define CHANGE_ALPHA_CNT	(15)	// 透明度変更カウント
#define INVULN_MIN_ALPHA	(0.55f)	// 最小透明度
#define INVULN_MAX_ALPHA	(1.0f)	// 最大透明度

#define ALWAYS_ADDROT		(0.3f)	// 常時内側に向かせる量
#define PLUS_INSIDE_MOVE	(3.0f)	// ターゲットからの距離に応じた外側への移動加算量
#define PLUS_OUTSIDE_MOVE	(1.5f)	// ターゲットからの距離に応じた内側への移動加算量
#define INSIDE_ADDROT		(0.8f)	// 外側移動時の向き変動量
#define OUTSIDE_SUBROT		(0.5f)	// 内側移動時の向き変動量

#define MOVE_INSIDE			(0.55f)	// 内側への移動量
#define MOVE_OUTSIDE		(2.55f)	// 外側への移動量
#define MOVE_OUTSIDE_ACCELE	(1.0f)	// 外側移動の加速量
#define MOVE_LEFT			(0.55f)	// 左側への移動量
#define MOVE_LEFT_ACCELE	(1.2f)	// 左移動の加速量
#define MOVE_LEFT_DECELE	(0.4f)	// 左移動の減速量

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CPlayer::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\orbit000.jpg",	// 軌跡のテクスチャ
};

const char *CPlayer::mc_apModelFile[] =	// モデル定数
{
	"data\\MODEL\\PLAYER\\00_waist.x",	// 腰
	"data\\MODEL\\PLAYER\\01_body.x",	// 体
	"data\\MODEL\\PLAYER\\02_cloak.x",	// マント
	"data\\MODEL\\PLAYER\\03_head.x",	// 頭
	"data\\MODEL\\PLAYER\\04_hat.x",	// 帽子
	"data\\MODEL\\PLAYER\\05_armUL.x",	// 左上腕
	"data\\MODEL\\PLAYER\\06_armUR.x",	// 右上腕
	"data\\MODEL\\PLAYER\\07_armDL.x",	// 左下腕
	"data\\MODEL\\PLAYER\\08_armDR.x",	// 右下腕
	"data\\MODEL\\PLAYER\\09_handL.x",	// 左手
	"data\\MODEL\\PLAYER\\10_handR.x",	// 右手
	"data\\MODEL\\PLAYER\\11_legUL.x",	// 左太もも
	"data\\MODEL\\PLAYER\\12_legUR.x",	// 右太もも
	"data\\MODEL\\PLAYER\\13_legDL.x",	// 左脛
	"data\\MODEL\\PLAYER\\14_legDR.x",	// 右脛
	"data\\MODEL\\PLAYER\\15_footL.x",	// 左足
	"data\\MODEL\\PLAYER\\16_footR.x",	// 右足
	"data\\MODEL\\PLAYER\\17_rod.x",	// 杖
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
	m_pExp			= NULL;				// 経験値マネージャーの情報
	m_pLevelup		= NULL;				// 強化マネージャーの情報
	m_pLife			= NULL;				// 体力の情報
	m_pDash			= NULL;				// ダッシュの情報
	m_pShadow		= NULL;				// 影の情報
	m_pOrbit		= NULL;				// 軌跡の情報
	m_oldPos		= VEC3_ZERO;		// 過去位置
	m_move			= VEC3_ZERO;		// 移動量
	m_destRot		= VEC3_ZERO;		// 目標向き
	m_rotation		= ROTATION_LEFT;	// 回転方向
	m_state			= STATE_NONE;		// 状態
	m_nCounterState	= 0;				// 状態管理カウンター
	m_nNumModel		= 0;				// パーツの総数
	m_fDisTarget	= 0.0f;				// ターゲットとの距離
	m_fVortexRot	= 0.0f;				// 渦巻き方向
	m_fVortexDis	= 0.0f;				// 渦巻との距離
	m_bJump			= false;			// ジャンプ状況

	m_fInSideMove		= MOVE_INSIDE;			// 内側移動量
	m_fOutSideMove		= MOVE_OUTSIDE;			// 外側移動量
	m_fAddOutSideMove	= MOVE_OUTSIDE_ACCELE;	// 外側移動の加速量
	m_fSideMove			= MOVE_LEFT;			// 横移動量
	m_fAddMove			= MOVE_LEFT_ACCELE;		// 横移動の加速量
	m_fSubMove			= MOVE_LEFT_DECELE;		// 横移動の減速量
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
	// ポインタを宣言
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ

	// メンバ変数を初期化
	m_pMagic		= NULL;				// 魔法マネージャーの情報
	m_pExp			= NULL;				// 経験値マネージャーの情報
	m_pLevelup		= NULL;				// 強化マネージャーの情報
	m_pLife			= NULL;				// 体力の情報
	m_pDash			= NULL;				// ダッシュの情報
	m_pShadow		= NULL;				// 影の情報
	m_pOrbit		= NULL;				// 軌跡の情報
	m_oldPos		= VEC3_ZERO;		// 過去位置
	m_move			= VEC3_ZERO;		// 移動量
	m_destRot		= VEC3_ZERO;		// 目標向き
	m_rotation		= ROTATION_LEFT;	// 回転方向
	m_state			= STATE_NONE;		// 状態
	m_nCounterState	= 0;				// 状態管理カウンター
	m_nNumModel		= 0;				// パーツの総数
	m_fDisTarget	= 0.0f;				// ターゲットとの距離
	m_fVortexRot	= 0.0f;				// 渦巻き方向
	m_fVortexDis	= 0.0f;				// 渦巻との距離
	m_bJump			= true;				// ジャンプ状況

	m_fInSideMove		= MOVE_INSIDE;			// 内側移動量
	m_fOutSideMove		= MOVE_OUTSIDE;			// 外側移動量
	m_fAddOutSideMove	= MOVE_OUTSIDE_ACCELE;	// 外側移動の加速量
	m_fSideMove			= MOVE_LEFT;			// 横移動量
	m_fAddMove			= MOVE_LEFT_ACCELE;		// 横移動の加速量
	m_fSubMove			= MOVE_LEFT_DECELE;		// 横移動の減速量

	// 魔法マネージャーの生成
	m_pMagic = CMagicManager::Create(this);
	if (UNUSED(m_pMagic))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 経験値マネージャーの生成
	m_pExp = CExpManager::Create();
	if (UNUSED(m_pExp))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 強化マネージャーの生成
	m_pLevelup = CLevelupManager::Create();
	if (UNUSED(m_pLevelup))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 体力の生成
	m_pLife = CObjectGauge3D::Create
	( // 引数
		CObject::LABEL_GAUGE,			// オブジェクトラベル
		this,							// ゲージ表示オブジェクト
		PLAY_LIFE,						// 最大表示値
		(int)(NORMAL_CNT * 0.25f),		// 表示値変動フレーム
		GAUGE_PLUS_Y,					// 表示Y位置の加算量
		GAUGE_GAUGESIZE,				// ゲージ大きさ
		GAUGE_FRONTCOL,					// 表ゲージ色
		GAUGE_BACKCOL,					// 裏ゲージ色
		true,							// 枠描画状況
		CObjectGauge3D::TYPE_PLAYER,	// 枠種類
		GAUGE_GAUGESIZE					// 枠大きさ
	);
	if (UNUSED(m_pLife))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ダッシュの生成
	m_pDash = CGaugeStar::Create
	( // 引数
		STAR_MAX_GAUGE,		// 最大ゲージ量
		STAR_RADIUS,		// 半径
		STAR_HEAL,			// ゲージ回復量
		STAR_WAIT_HEAL,		// 回復待機カウント
		this,				// 親オブジェクト
		STAR_ADDPOS,		// 表示位置の加算量
		STAR_DIS_CENTER,	// 中心からの距離
		STAR_FLICKER		// 揺らめき量
	);
	if (UNUSED(m_pDash))
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

	// 軌跡の生成
	m_pOrbit = CObjectOrbit::Create(GetMultiModel(MODEL_ROD)->GetPtrMtxWorld(), PLAY_ORBIT_COL, CObjectOrbit::OFFSET_ROD);
	if (UNUSED(m_pOrbit))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを読込・割当
	m_pOrbit->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_ORBIT]));

	// 優先順位を設定
	m_pOrbit->SetPriority(PLAY_ORBIT_PRIO);

	// 透明度を設定
	SetAlpha(0.0f);

	// 表示の設定
	SetDisp(false);

	// モーションの設定
	SetMotion(MOTION_MOVE);

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

	// 経験値マネージャーを破棄
	if (FAILED(m_pExp->Release(m_pExp)))
	{ // 破棄に失敗した場合

		// 例外処理
		assert(false);
	}

	// 強化マネージャーを破棄
	if (FAILED(m_pLevelup->Release(m_pLevelup)))
	{ // 破棄に失敗した場合

		// 例外処理
		assert(false);
	}

	// 影を破棄
	m_pShadow->Uninit();

	// 軌跡を破棄
	m_pOrbit->Uninit();

	// オブジェクトキャラクターの終了
	CObjectChara::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CPlayer::Update(void)
{
	// 変数を宣言
	int nCurrentMotion = NONE_IDX;	// 現在のモーション
	D3DXVECTOR3 posPlayer = GetPosition();	// プレイヤー位置
	D3DXVECTOR3 posTarget = CSceneGame::GetTarget()->GetPosition();	// ターゲット位置

	// 過去位置を更新
	m_oldPos = posPlayer;

	// ターゲットとの距離を設定
	m_fDisTarget = sqrtf((posPlayer.x - posTarget.x) * (posPlayer.x - posTarget.x)+ (posPlayer.z - posTarget.z) * (posPlayer.z - posTarget.z)) * 0.5f;

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:		// 何もしない状態

		// 無し

		break;

	case STATE_FADEOUT:		// フェードアウト状態

		// フェードアウト状態時の更新
		UpdateFadeOut();

		break;

	case STATE_NORMAL:		// 通常状態

		// 通常状態時の更新
		nCurrentMotion = UpdateNormal();

		break;

	case STATE_DAMAGE:		// ダメージ状態

		// ダメージ状態時の更新
		nCurrentMotion = UpdateDamage();

		break;

	case STATE_INVULN:		// 無敵状態

		// 無敵状態時の更新
		nCurrentMotion = UpdateInvuln();

		break;

	case STATE_BLOW_AWAY:	// 吹っ飛び状態

		// 吹っ飛び状態時の更新
		UpdateBlowAway();

		break;

	case STATE_VORTEX:		// 渦巻きこまれ状態

		// 渦巻きこまれ状態時の更新
		UpdateVortex();

		break;

	case STATE_FADEIN:		// フェードイン状態

		// フェードイン状態時の更新
		UpdateFadeIn();

		break;

	case STATE_DEATH:		// 死亡状態

		// 無し

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// 魔法マネージャーの更新
	m_pMagic->Update();

	// 経験値マネージャーの更新
	m_pExp->Update();

	// 強化マネージャーの更新
	m_pLevelup->Update();

	// 影の更新
	m_pShadow->Update();

	// 軌跡の更新
	m_pOrbit->Update();

	// モーション・オブジェクトキャラクターの更新
	Motion(nCurrentMotion);
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

		if (m_state == STATE_NORMAL)
		{ // 通常状態の場合

			// 体力からダメージ分減算
			m_pLife->AddNum(-nDmg);

			if (m_pLife->GetNum() > 0)
			{ // 生きている場合

				// パーティクル3Dオブジェクトを生成
				CParticle3D::Create(CParticle3D::TYPE_DAMAGE, pos);

				// カウンターを初期化
				m_nCounterState = 0;

				// 状態を変更
				m_state = STATE_DAMAGE;	// ダメージ状態
			}
			else
			{ // 死んでいる場合

				// パーティクル3Dオブジェクトを生成
				CParticle3D::Create(CParticle3D::TYPE_DAMAGE, pos, D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f));
				CParticle3D::Create(CParticle3D::TYPE_DAMAGE, pos, D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f));

				// 更新と描画を停止
				SetEnableUpdate(false);
				SetEnableDraw(false);

				// カウンターを初期化
				m_nCounterState = 0;

				// 状態を変更
				m_state = STATE_DEATH;	// 死亡状態
			}
		}
	}
}

//============================================================
//	生成処理
//============================================================
CPlayer *CPlayer::Create(D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot)
{
	// 変数を宣言
	D3DXVECTOR3 pos = rPos;		// 座標設定用

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
		CSceneGame::GetStage()->LimitPosition(pos, PLAY_RADIUS);	// ステージ範囲外補正
		pos.y = CSceneGame::GetField()->GetPositionHeight(pos);		// 高さを地面に設定
		pPlayer->SetPosition(pos);

		// 向きを設定
		pPlayer->SetRotation(rRot);

		// 確保したアドレスを返す
		return pPlayer;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	吹っ飛びヒット処理
//============================================================
void CPlayer::HitBlowAway
(
	const D3DXVECTOR3& rPlayerPos,	// プレイヤー位置
	const D3DXVECTOR3& rHitObjPos,	// オブジェクト位置
	const int nDmg					// ダメージ量
)
{
	// 変数を宣言
	D3DXVECTOR3 vecAway = VEC3_ZERO;	// 吹っ飛びベクトル
	STATE oldState = m_state;	// 過去の状態

	// ヒット処理
	Hit(nDmg);

	if (oldState == STATE_NORMAL
	&&  m_state  == STATE_DAMAGE)
	{ // 元の状態が通常状態且つ、現在の状態がダメージ状態の場合

		// 吹っ飛びベクトルを求める
		vecAway = rPlayerPos - rHitObjPos;
		vecAway.y = 0.0f;						// 縦ベクトルを削除
		D3DXVec3Normalize(&vecAway, &vecAway);	// ベクトルを正規化

		// 移動量を設定
		m_move = vecAway * AWAY_SIDE_MOVE;
		m_move.y = AWAY_UP_MOVE;

		// 状態を変更
		m_state = STATE_BLOW_AWAY;	// 吹っ飛び状態

		// 吹っ飛びモーションに移行
		SetMotion(MOTION_BLOW_AWAY);
	}
}

//============================================================
//	渦巻きこまれヒット処理
//============================================================
void CPlayer::HitVortex
(
	D3DXVECTOR3& rPlayerPos,	// プレイヤー位置
	const D3DXVECTOR3& rHitPos	// 当たり判定位置
)
{
	// バリアの衝突判定
	collision::CirclePillar
	( // 引数
		rPlayerPos,											// 判定位置
		CSceneGame::GetStage()->GetStageBarrierPosition(),	// 判定目標位置
		PLAY_RADIUS,										// 判定半径
		CSceneGame::GetStage()->GetStageBarrier().fRadius	// 判定目標半径
	);

	if (m_state == STATE_NORMAL)
	{ // 通常状態の場合

		// ヒット処理
		Hit(HURRICANE_DMG);

		if (m_state != STATE_DEATH)
		{ // 死亡状態の場合

			// 巻き込まれ始めの向きを設定
			m_fVortexRot = atan2f(rPlayerPos.x - rHitPos.x, rPlayerPos.z - rHitPos.z);

			// 渦との距離を設定
			m_fVortexDis = sqrtf((rPlayerPos.x - rHitPos.x) * (rPlayerPos.x - rHitPos.x) + (rPlayerPos.z - rHitPos.z) * (rPlayerPos.z - rHitPos.z)) * 0.5f;

			// カメラ更新をOFFにする
			CManager::GetCamera()->SetEnableUpdate(false);

			// 状態を設定
			m_state = STATE_VORTEX;	// 渦巻きこまれ状態

			// 吹っ飛びモーションに移行
			SetMotion(MOTION_BLOW_AWAY);
		}
	}
}

//============================================================
//	経験値の加算処理
//============================================================
void CPlayer::AddExp(const int nAdd)
{
	// 経験値を加算
	m_pExp->AddExp(nAdd);
}

//============================================================
//	再出現設定の設定処理
//============================================================
void CPlayer::SetRespawn(D3DXVECTOR3& rPos)
{
	// 変数を宣言
	D3DXVECTOR3 posTarget = CSceneGame::GetTarget()->GetPosition();	// ターゲット位置

	// 情報を初期化
	CSceneGame::GetPlayer()->SetMotion(CPlayer::MOTION_MOVE);	// 浮遊モーションを設定
	m_state = STATE_NORMAL;	// 通常状態を設定
	m_nCounterState = 0;	// カウンターを初期化

	// 位置を補正・設定
	CSceneGame::GetStage()->LimitPosition(rPos, PLAY_RADIUS);	// ステージ範囲外の補正
	Land(rPos);				// 着地判定
	SetPosition(rPos);		// 位置を設定

	// ターゲットとの距離を設定
	m_fDisTarget = sqrtf((rPos.x - posTarget.x) * (rPos.x - posTarget.x) + (rPos.z - posTarget.z) * (rPos.z - posTarget.z)) * 0.5f;

	// 軌跡の初期化を行う状態にする
	m_pOrbit->SetEnableInit(false);

	// 表示する設定にする
	SetDisp(true);

	// カメラ更新をONにする
	CManager::GetCamera()->SetEnableUpdate(true);

	// カメラ目標位置設定
	CManager::GetCamera()->SetDestBargaining();
}

//============================================================
//	表示の設定処理
//============================================================
void CPlayer::SetDisp(const bool bDisp)
{
	if (bDisp)
	{ // 表示する状態の場合

		// 状態を設定
		m_state = STATE_FADEOUT;	// フェードアウト状態

		// 描画する設定にする
		SetEnableDraw(true);
	}
	else
	{ // 表示しない状態の場合

		// 状態を設定
		m_state = STATE_FADEIN;		// フェードイン状態

		// 描画しない設定にする
		SetEnableDraw(false);
		CObject::SetEnableDraw(true);	// プレイヤーの描画はONにする
	}
}

//============================================================
//	更新状況の設定処理
//============================================================
void CPlayer::SetEnableUpdate(const bool bUpdate)
{
	// 引数の更新状況を設定
	CObject::SetEnableUpdate(bUpdate);		// 自身
	m_pShadow->SetEnableUpdate(bUpdate);	// 影
	m_pOrbit->SetEnableUpdate(bUpdate);		// 軌跡
	m_pDash->SetEnableUpdate(bUpdate);		// ダッシュ
}

//============================================================
//	描画状況の設定処理
//============================================================
void CPlayer::SetEnableDraw(const bool bDraw)
{
	// 引数の描画状況を設定
	CObject::SetEnableDraw(bDraw);		// 自身
	m_pShadow->SetEnableDraw(bDraw);	// 影
	m_pOrbit->SetEnableDraw(bDraw);		// 軌跡
	m_pDash->SetEnableDraw(bDraw);		// ダッシュ
	m_pLife->SetEnableDraw(bDraw);		// 体力
	m_pMagic->SetEnableManaDraw(bDraw);	// マナ
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
//	レベル取得処理
//============================================================
int CPlayer::GetLevel(void) const
{
	// レベルを返す
	return m_pExp->GetLevel();
}

//============================================================
//	状態取得処理
//============================================================
int CPlayer::GetState(void) const
{
	// 状態を返す
	return m_state;
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
//	通常状態時の更新処理
//============================================================
CPlayer::MOTION CPlayer::UpdateNormal(void)
{
	// 変数を宣言
	MOTION currentMotion;	// 現在のモーション
	D3DXVECTOR3 posPlayer = GetPosition();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = GetRotation();	// プレイヤー向き

	// 移動操作
	currentMotion = Move();

	// 向き更新
	Rot(rotPlayer);

	// 位置更新
	Pos(posPlayer);

	// ステージ範囲外の補正
	CSceneGame::GetStage()->LimitPosition(posPlayer, PLAY_RADIUS);

	// 着地判定
	Land(posPlayer);

	// 魔法発射操作
	currentMotion = Magic(currentMotion);

	// 当たり判定
	CollisionTarget(posPlayer);	// ターゲット
	CollisionEnemy(posPlayer);	// 敵

	// バリアとの当たり判定
	if (CSceneGame::GetStage()->CollisionBarrier(posPlayer, PLAY_RADIUS))
	{ // 当たっていた場合

		// 渦巻きこまれヒット
		HitVortex(posPlayer, CSceneGame::GetStage()->GetStageBarrierPosition());
	}

	// 位置を更新
	SetPosition(posPlayer);

	// 向きを更新
	SetRotation(rotPlayer);

	// 現在のモーションを返す
	return currentMotion;
}

//============================================================
//	ダメージ状態時の更新処理
//============================================================
CPlayer::MOTION CPlayer::UpdateDamage(void)
{
	// 変数を宣言
	MOTION currentMotion;	// 現在のモーション

	// 通常状態時の更新
	currentMotion = UpdateNormal();

	if (m_nCounterState < INVULN_CNT)
	{ // カウンターが一定値より小さい場合

		// カウンターを加算
		m_nCounterState++;

		// マテリアルを設定
		SetMaterial(material::Red());	// 赤
	}
	else
	{ // カウンターが一定値以上の場合

		// カウンターを初期化
		m_nCounterState = 0;

		// マテリアル再設定
		ResetMaterial();

		// 状態を変更
		m_state = STATE_INVULN;	// 無敵状態
	}

	// 現在のモーションを返す
	return currentMotion;
}

//============================================================
//	無敵状態時の更新処理
//============================================================
CPlayer::MOTION CPlayer::UpdateInvuln(void)
{
	// 変数を宣言
	MOTION currentMotion;	// 現在のモーション

	// 通常状態時の更新
	currentMotion = UpdateNormal();

	if (m_nCounterState < NORMAL_CNT)
	{ // カウンターが一定値より小さい場合

		// カウンターを加算
		m_nCounterState++;

		if (m_nCounterState % CHANGE_ALPHA_CNT == 0)
		{ // 透明度の変更タイミングだった場合

			if (GetAlpha() > INVULN_MIN_ALPHA)
			{ // 透明度が上がっていない場合

				// 透明度を上げる
				SetAlpha(INVULN_MIN_ALPHA);
			}
			else
			{ // 透明度が上がっている場合

				// 透明度を下げる
				SetAlpha(INVULN_MAX_ALPHA);
			}
		}
	}
	else
	{ // カウンターが一定値以上の場合

		// カウンターを初期化
		m_nCounterState = 0;

		// 透明度を不透明に設定
		SetAlpha(1.0f);

		// 状態を変更
		m_state = STATE_NORMAL;	// 通常状態
	}

	// 現在のモーションを返す
	return currentMotion;
}

//============================================================
//	吹っ飛び状態時の更新処理
//============================================================
void CPlayer::UpdateBlowAway(void)
{
	// 変数を宣言
	D3DXVECTOR3 posPlayer = GetPosition();	// プレイヤー位置

	// 位置更新
	Pos(posPlayer);

	// ターゲットとの当たり判定
	CollisionTarget(posPlayer);

	// ステージ範囲外の補正
	CSceneGame::GetStage()->LimitPosition(posPlayer, PLAY_RADIUS);

	// 着地判定
	if (Land(posPlayer))
	{ // 着地していた場合

		// 浮遊モーションに移行
		SetMotion(MOTION_MOVE);

		// 状態を設定
		m_state = STATE_DAMAGE;	// ダメージ状態
	}

	// 位置を反映
	SetPosition(posPlayer);

	// 向きを設定
	SetRotation(D3DXVECTOR3(0.0f, atan2f(m_move.x, m_move.z), 0.0f));
}

//============================================================
//	渦巻きこまれ状態時の更新処理
//============================================================
void CPlayer::UpdateVortex(void)
{
	// 変数を宣言
	D3DXVECTOR3 posPlayer = GetPosition();	// プレイヤー位置

	// 向きを減算
	m_fVortexRot -= VORTEX_ADDROT;
	useful::NormalizeRot(m_fVortexRot);

	// 距離を加算
	m_fVortexDis += VORTEX_ADDDIS;

	// 位置を設定
	posPlayer.x = sinf(m_fVortexRot) * m_fVortexDis;
	posPlayer.y += VORTEX_ADDPOSY;
	posPlayer.z = cosf(m_fVortexRot) * m_fVortexDis;

	// 位置を反映
	SetPosition(posPlayer);

	// フェードイン状態時の更新
	UpdateFadeIn();

	if (m_state == STATE_NONE)
	{ // 透明になり切った場合

		// プレイヤーを再出現させる
		SetRespawn(PLAY_SPAWN_POS);
	}
}

//============================================================
//	フェードアウト状態時の更新処理
//============================================================
void CPlayer::UpdateFadeOut(void)
{
	// 変数を宣言
	float fAlpha = GetAlpha();	// 透明度

	// 透明度を上げる
	fAlpha += FADE_LEVEL;

	if (fAlpha >= GetMaxAlpha())
	{ // 透明度が上がり切った場合

		// 透明度を補正
		fAlpha = GetMaxAlpha();

		// 状態を設定
		m_state = STATE_INVULN;	// 無敵状態
	}

	// 透明度を設定
	SetAlpha(fAlpha);
}

//============================================================
//	フェードイン状態時の更新処理
//============================================================
void CPlayer::UpdateFadeIn(void)
{
	// 変数を宣言
	float fAlpha = GetAlpha();	// 透明度

	// 透明度を下げる
	fAlpha -= FADE_LEVEL;

	if (fAlpha <= 0.0f)
	{ // 透明度が下がり切った場合

		// 透明度を補正
		fAlpha = 0.0f;

		// 状態を設定
		m_state = STATE_NONE;	// 何もしない状態
	}

	// 透明度を設定
	SetAlpha(fAlpha);
}

//============================================================
//	移動処理
//============================================================
CPlayer::MOTION CPlayer::Move(void)
{
	// 変数を宣言
	MOTION currentMotion = MOTION_MOVE;	// 現在のモーション
	D3DXVECTOR3 vecTarg, vecSide;		// ターゲット方向ベクトル・横方向ベクトル

	// ポインタを宣言
	CInputKeyboard	*pKeyboard	= CManager::GetKeyboard();	// キーボード
	CInputPad		*pPad		= CManager::GetPad();		// パッド

#if 1

	// 変数を宣言
	D3DXVECTOR3 posPlayer = GetPosition();	// プレイヤー位置
	D3DXVECTOR3 posTarget = CSceneGame::GetTarget()->GetPosition();	// ターゲット位置
	float fDisTargRate = (1.0f / CSceneGame::GetStage()->GetStageLimit().fRadius) * m_fDisTarget;	// ターゲット距離の割合

	// ターゲット方向のベクトルを計算
	vecTarg = posTarget - posPlayer;
	vecTarg.y = 0.0f;						// ベクトルの縦方向を無視
	D3DXVec3Normalize(&vecTarg, &vecTarg);	// ベクトルの正規化

	// 横方向ベクトルを計算
	vecSide = D3DXVECTOR3(vecTarg.z, 0.0f, -vecTarg.x);

	// 目標向きを設定
	m_destRot.y = atan2f(posTarget.x - posPlayer.x, posTarget.z - posPlayer.z);
	m_destRot.y -= D3DX_PI * 0.5f;	// 左を向かせる
	m_destRot.y -= ALWAYS_ADDROT;	// 内側を向かせる

	// 内側への移動量を設定
	m_move += vecTarg * (m_fInSideMove + (fDisTargRate * PLUS_INSIDE_MOVE));

	if (pKeyboard->GetPress(DIK_W) || pPad->GetPressLStickY() > 0.0f)
	{
		// 外側への移動量を追加
		m_move -= vecTarg * (m_fOutSideMove + (fDisTargRate * PLUS_OUTSIDE_MOVE));

		// 外側を向かせる
		m_destRot.y += INSIDE_ADDROT;
	}
	else if (pKeyboard->GetPress(DIK_S) || pPad->GetPressLStickY() < 0.0f)
	{
		// 内側への移動量を追加
		m_move += vecTarg * m_fAddOutSideMove;

		// 内側を向かせる
		m_destRot.y -= OUTSIDE_SUBROT;
	}

	// 左側への移動量を設定
	m_move += vecSide * m_fSideMove;

	if (pKeyboard->GetPress(DIK_A) || pPad->GetPress(CInputPad::KEY_L1))
	{
		if (m_pDash->UseGauge())
		{ // ゲージが使用できた場合

			// 左移動量を加速
			m_move += vecSide * m_fAddMove;

			// 現在のモーションを設定
			currentMotion = MOTION_ACCELE;	// 加速モーション
		}
	}
	else if (pKeyboard->GetPress(DIK_D) || pPad->GetPress(CInputPad::KEY_R1))
	{
		// 左移動量を減速
		m_move -= vecSide * m_fSubMove;

		// 現在のモーションを設定
		currentMotion = MOTION_DECELE;	// 減速モーション
	}

#if 0

	// TODO：速度決める
	if (pKeyboard->GetPress(DIK_R))
	{
		m_fAddOutSideMove += 0.1f;
	}
	else if (pKeyboard->GetPress(DIK_F))
	{
		m_fAddOutSideMove -= 0.1f;
	}
	if (pKeyboard->GetPress(DIK_T))
	{
		m_fSideMove += 0.1f;
	}
	else if (pKeyboard->GetPress(DIK_G))
	{
		m_fSideMove -= 0.1f;
	}
	if (pKeyboard->GetPress(DIK_Y))
	{
		m_fAddMove += 0.1f;
	}
	else if (pKeyboard->GetPress(DIK_H))
	{
		m_fAddMove -= 0.1f;
	}
	if (pKeyboard->GetPress(DIK_U))
	{
		m_fSubMove += 0.1f;
	}
	else if (pKeyboard->GetPress(DIK_J))
	{
		m_fSubMove -= 0.1f;
	}
	if (pKeyboard->GetPress(DIK_I))
	{
		m_fInSideMove += 0.1f;
	}
	else if (pKeyboard->GetPress(DIK_K))
	{
		m_fInSideMove -= 0.1f;
	}
	if (pKeyboard->GetPress(DIK_O))
	{
		m_fOutSideMove += 0.1f;
	}
	else if (pKeyboard->GetPress(DIK_L))
	{
		m_fOutSideMove -= 0.1f;
	}

	useful::LimitNum(m_fAddOutSideMove, 0.1f, 100.0f);
	useful::LimitNum(m_fSideMove, 0.1f, 100.0f);
	useful::LimitNum(m_fAddMove, 0.1f, 100.0f);
	useful::LimitNum(m_fSubMove, 0.1f, 100.0f);
	useful::LimitNum(m_fInSideMove, 0.1f, 100.0f);
	useful::LimitNum(m_fOutSideMove, 0.1f, 100.0f);

	CManager::GetDebugProc()->Print("----------------------------------\n");
	CManager::GetDebugProc()->Print("内側加速量 加減操作：[R/F]\n");
	CManager::GetDebugProc()->Print("横移動量   加減操作：[T/G]\n");
	CManager::GetDebugProc()->Print("加速移動量 加減操作：[Y/H]\n");
	CManager::GetDebugProc()->Print("減速移動量 加減操作：[U/J]\n");
	CManager::GetDebugProc()->Print("内側移動量 加減操作：[I/K]\n");
	CManager::GetDebugProc()->Print("外側移動量 加減操作：[O/L]\n");
	CManager::GetDebugProc()->Print("----------------------------------\n");
	CManager::GetDebugProc()->Print("内側加速量：%f\n", m_fAddOutSideMove);
	CManager::GetDebugProc()->Print("横移動量  ：%f\n", m_fSideMove);
	CManager::GetDebugProc()->Print("加速移動量：%f\n", m_fAddMove);
	CManager::GetDebugProc()->Print("減速移動量：%f\n", m_fSubMove);
	CManager::GetDebugProc()->Print("内側移動量：%f\n", m_fInSideMove);
	CManager::GetDebugProc()->Print("外側移動量：%f\n", m_fOutSideMove);

#endif

#else	// デバッグ用歩行

	// 移動操作
	if (pKeyboard->GetPress(DIK_W))
	{ // 奥移動の操作が行われた場合

		if (pKeyboard->GetPress(DIK_A))
		{ // 左移動の操作も行われた場合 (左奥移動)

			// 移動量を更新
			m_move.x += sinf(rot.y - (D3DX_PI * 0.25f)) * 2.0f;
			m_move.z += cosf(rot.y - (D3DX_PI * 0.25f)) * 2.0f;

			// 目標向きを更新
			m_destRot.y = D3DXToRadian(135) + rot.y;
		}
		else if (pKeyboard->GetPress(DIK_D))
		{ // 右移動の操作も行われた場合 (右奥移動)

			// 移動量を更新
			m_move.x -= sinf(rot.y - (D3DX_PI * 0.75f)) * 2.0f;
			m_move.z -= cosf(rot.y - (D3DX_PI * 0.75f)) * 2.0f;

			// 目標向きを更新
			m_destRot.y = D3DXToRadian(225) + rot.y;
		}
		else
		{ // 奥移動の操作だけが行われた場合 (奥移動)

			// 移動量を更新
			m_move.x += sinf(rot.y) * 2.0f;
			m_move.z += cosf(rot.y) * 2.0f;

			// 目標向きを更新
			m_destRot.y = D3DXToRadian(180) + rot.y;
		}
	}
	else if (pKeyboard->GetPress(DIK_S))
	{ // 手前移動の操作が行われた場合

		if (pKeyboard->GetPress(DIK_A))
		{ // 左移動の操作も行われた場合 (左手前移動)

			// 移動量を更新
			m_move.x += sinf(rot.y - (D3DX_PI * 0.75f)) * 2.0f;
			m_move.z += cosf(rot.y - (D3DX_PI * 0.75f)) * 2.0f;

			// 目標向きを更新
			m_destRot.y = D3DXToRadian(45) + rot.y;
		}
		else if (pKeyboard->GetPress(DIK_D))
		{ // 右移動の操作も行われた場合 (右手前移動)

			// 移動量を更新
			m_move.x -= sinf(rot.y - (D3DX_PI * 0.25f)) * 2.0f;
			m_move.z -= cosf(rot.y - (D3DX_PI * 0.25f)) * 2.0f;

			// 目標向きを更新
			m_destRot.y = D3DXToRadian(315) + rot.y;
		}
		else
		{ // 手前移動の操作だけが行われた場合 (手前移動)

			// 移動量を更新
			m_move.x -= sinf(rot.y) * 2.0f;
			m_move.z -= cosf(rot.y) * 2.0f;

			// 目標向きを更新
			m_destRot.y = D3DXToRadian(0) + rot.y;
		}
	}
	else if (pKeyboard->GetPress(DIK_A))
	{ // 左移動の操作が行われた場合

		// 移動量を更新
		m_move.x += sinf(rot.y - (D3DX_PI * 0.5f)) * 2.0f;
		m_move.z += cosf(rot.y - (D3DX_PI * 0.5f)) * 2.0f;

		// 目標向きを更新
		m_destRot.y = D3DXToRadian(90) + rot.y;
	}
	else if (pKeyboard->GetPress(DIK_D))
	{ // 右移動の操作が行われた場合

		// 移動量を更新
		m_move.x -= sinf(rot.y - (D3DX_PI * 0.5f)) * 2.0f;
		m_move.z -= cosf(rot.y - (D3DX_PI * 0.5f)) * 2.0f;

		// 目標向きを更新
		m_destRot.y = D3DXToRadian(270) + rot.y;
	}

#endif

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
	CMagicManager::Shot shot;		// 魔法の発射状況

	// 魔法の発射
	shot = m_pMagic->ShotMagic();	// 発射情報を代入

	if (shot.bControl && m_pMagic->GetMana() > 0)
	{ // 発射操作をしている且つ、マナ残量がある場合

		// 射撃向きをの値を0～2πにする
		shot.fRot += D3DX_PI;

		if (shot.fRot >= QRTR_PI && shot.fRot <= D3DX_PI - QRTR_PI)
		{
			// 右のアクションモーションを設定
			currentMotion = MOTION_ACTION_RIGHT;
		}
		else if (shot.fRot >= D3DX_PI - QRTR_PI && shot.fRot <= D3DX_PI + QRTR_PI)
		{
			// 手前のアクションモーションを設定
			currentMotion = MOTION_ACTION_NEAR;
		}
		else if (shot.fRot >= D3DX_PI + QRTR_PI && shot.fRot <= (D3DX_PI * 2) - QRTR_PI)
		{
			// 左のアクションモーションを設定
			currentMotion = MOTION_ACTION_LEFT;
		}
		else if (shot.fRot >= (D3DX_PI * 2) - QRTR_PI && shot.fRot <= (D3DX_PI * 2) || shot.fRot >= 0.0f && shot.fRot <= QRTR_PI)
		{
			// 奥のアクションモーションを設定
			currentMotion = MOTION_ACTION_FAR;
		}
		else
		{
			// 例外処理
			assert(false);	// 向きの正規化ミス
		}
	}

	// 現在のモーションを返す
	return currentMotion;
}

//============================================================
//	着地処理
//============================================================
bool CPlayer::Land(D3DXVECTOR3& rPos)
{
	// 変数を宣言
	bool bLand = false;	// 着地状況

	// 着地判定
	if (CSceneGame::GetField()->LandPosition(rPos, m_move)
	||  CSceneGame::GetStage()->LandPosition(rPos, m_move, 0.0f))
	{ // プレイヤーが着地していた場合

		// 着地している状態にする
		bLand = true;

		// ジャンプしていない状態にする
		m_bJump = false;
	}
	else
	{ // 着地していない場合

		// ジャンプしている状態にする
		m_bJump = true;
	}

	// 着地状況を返す
	return bLand;
}

//============================================================
//	モーション処理
//============================================================
void CPlayer::Motion(int nMotion)
{
	// 変数を宣言
	int nAnimMotion = GetMotionType();	// 現在再生中のモーション

	if (nMotion != NONE_IDX)
	{ // モーションが設定されている場合

		if (IsMotionLoop(nAnimMotion))
		{ // ループするモーションだった場合

			if (nAnimMotion != nMotion)
			{ // 現在のモーションが再生中のモーションと一致しない場合

				// 現在のモーションの設定
				SetMotion(nMotion);
			}
		}
	}

	// オブジェクトキャラクターの更新
	CObjectChara::Update();

	// モーションの遷移
	if (IsMotionFinish())
	{ // モーションが終了していた場合

		switch (GetMotionType())
		{ // モーションの種類ごとの処理
		case MOTION_BLOW_AWAY:	// 吹っ飛び状態

			// 無し

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
	if (m_state == STATE_BLOW_AWAY)
	{ // 空中の場合

		m_move.x += (0.0f - m_move.x) * PLAY_AWAY_REV;
		m_move.z += (0.0f - m_move.z) * PLAY_AWAY_REV;
	}
	else
	{ // 地上の場合

		m_move.x += (0.0f - m_move.x) * PLAY_LAND_REV;
		m_move.z += (0.0f - m_move.z) * PLAY_LAND_REV;
	}
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

	if (pTarget->GetState() != CTarget::STATE_DESTROY)
	{ // ターゲットが破壊されていない場合

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

				if (pObjCheck->GetState() == CEnemy::STATE_SPAWN
				||  pObjCheck->GetState() == CEnemy::STATE_DEATH)
				{ // 敵の状態がスポーン状態か死亡状態の場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				// 敵との当たり判定
				if (collision::CirclePillar
				( // 引数
					rPos,						// 判定位置
					pObjCheck->GetPosition(),	// 判定目標位置
					PLAY_RADIUS,				// 判定半径
					CEnemy::GetStatusInfo(pObjCheck->GetType()).fRadius	// 判定目標半径
				))
				{ // 当たっていた場合

					// プレイヤーのヒット処理
					HitBlowAway(rPos, pObjCheck->GetPosition(), ENE_HIT_DMG);
				}

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
