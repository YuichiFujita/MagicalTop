//============================================================
//
//	チュートリアルプレイヤー処理 [playerTutorial.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "playerTutorial.h"
#include "stage.h"

//************************************************************
//	子クラス [CPlayerTutorial] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CPlayerTutorial::CPlayerTutorial()
{

}

//============================================================
//	デストラクタ
//============================================================
CPlayerTutorial::~CPlayerTutorial()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CPlayerTutorial::Init(void)
{
	// プレイヤーの初期化
	if (FAILED(CPlayer::Init()))
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
void CPlayerTutorial::Uninit(void)
{
	// プレイヤーの終了
	CPlayer::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CPlayerTutorial::Update(void)
{
	// 変数を宣言
	int nCurrentMotion = NONE_IDX;	// 現在のモーション

	// 過去位置の更新
	UpdateOldPosition();

	// ターゲット距離の更新
	UpdateDisTarget();

	switch (GetState())
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

	// プレイヤーの更新
	CPlayer::Update();

	// モーション・オブジェクトキャラクターの更新
	UpdateMotion(nCurrentMotion);
}

//============================================================
//	通常状態時の更新処理
//============================================================
CPlayer::MOTION CPlayerTutorial::UpdateNormal(void)
{
	// 変数を宣言
	MOTION currentMotion = MOTION_MOVE;		// 現在のモーション
	D3DXVECTOR3 posPlayer = GetPosition();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = GetRotation();	// プレイヤー向き
	float fRadiusPlayer = GetRadius();		// プレイヤー半径

	// ポインタを宣言
	CStage *pStage = CScene::GetStage();	// ステージ情報

	if (USED(pStage))
	{ // ステージが使用されている場合

		// 移動操作
		currentMotion = UpdateMove();

		// 向き更新
		UpdateRotation(rotPlayer);

		// 位置更新
		UpdatePosition(posPlayer);

		// ステージ範囲外の補正
		pStage->LimitPosition(posPlayer, fRadiusPlayer);

		// 着地判定
		UpdateLanding(posPlayer);

		// 魔法発射操作
		currentMotion = ControlShotMagic(currentMotion);

		// 当たり判定
		CollisionTarget(posPlayer);	// ターゲット
		CollisionEnemy(posPlayer);	// 敵

		// バリアとの当たり判定
		if (pStage->CollisionBarrier(posPlayer, fRadiusPlayer))
		{ // 当たっていた場合

			// 渦巻きこまれヒット
			HitVortex(posPlayer, pStage->GetStageBarrierPosition());
		}

		// 位置を更新
		SetPosition(posPlayer);

		// 向きを更新
		SetRotation(rotPlayer);
	}
	else { assert(false); }	// 非使用中

	// 現在のモーションを返す
	return currentMotion;
}
