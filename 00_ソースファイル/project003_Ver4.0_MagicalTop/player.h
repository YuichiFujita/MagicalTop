//============================================================
//
//	プレイヤーヘッダー [player.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "objectChara.h"
#include "magic.h"

//************************************************************
//	マクロ定義
//************************************************************
#define PLAY_MAGIC_POS_PLUS_Y	(40.0f)	// プレイヤーの魔法発射時の高さ加算量

//************************************************************
//	前方宣言
//************************************************************
class CMagicManager;	// 魔法マネージャークラス
class CExpManager;		// 経験値マネージャークラス
class CLevelupManager;	// 強化マネージャークラス
class CObjectGauge2D;	// オブジェクトゲージ2Dクラス
class CShadow;			// 影クラス
class CObjectOrbit;		// オブジェクト軌跡クラス

//************************************************************
//	クラス定義
//************************************************************
// プレイヤークラス
class CPlayer : public CObjectChara
{
public:
	// 種類列挙
	typedef enum
	{
		MODEL_WAIST,	// 腰
		MODEL_BODY,		// 体
		MODEL_CLOAK,	// マント
		MODEL_HEAD,		// 頭
		MODEL_HAT,		// 帽子
		MODEL_ARMUL,	// 左上腕
		MODEL_ARMUR,	// 右上腕
		MODEL_ARMDL,	// 左下腕
		MODEL_ARMDR,	// 右下腕
		MODEL_HANDL,	// 左手
		MODEL_HANDR,	// 右手
		MODEL_LEGUL,	// 左太もも
		MODEL_LEGUR,	// 右太もも
		MODEL_LEGDL,	// 左脛
		MODEL_LEGDR,	// 右脛
		MODEL_FOOTL,	// 左足
		MODEL_FOOTR,	// 右足
		MODEL_ROD,		// 杖
		MODEL_MAX		// この列挙型の総数
	}MODEL;

	// モーション列挙
	typedef enum
	{
		MOTION_MOVE = 0,	// 歩行モーション
		MOTION_ACTION,		// アクションモーション
		MOTION_BLOW_AWAY,	// 吹っ飛びモーション
		MOTION_LANDING,		// 着地モーション
		MOTION_MAX			// この列挙型の総数
	}MOTION;

	// 回転列挙
	typedef enum
	{
		ROTATION_LEFT = 0,	// 左回転
		ROTATION_RIGHT,		// 右回転
		ROTATION_MAX		// この列挙型の総数
	}ROTATION;

	// 状態列挙
	typedef enum
	{
		STATE_NONE = 0,		// 何もしない状態
		STATE_FADEOUT,		// フェードアウト状態
		STATE_NORMAL,		// 通常状態
		STATE_DAMAGE,		// ダメージ状態
		STATE_BLOW_AWAY,	// 吹っ飛び状態
		STATE_FADEIN,		// フェードイン状態
		STATE_DEATH,		// 死亡状態
		STATE_MAX			// この列挙型の総数
	}STATE;

	// コンストラクタ
	CPlayer();

	// デストラクタ
	~CPlayer();

	// オーバーライド関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	void Hit(const int nDmg);	// ヒット

	// 静的メンバ関数
	static CPlayer *Create	// 生成
	( // 引数
		D3DXVECTOR3& rPos,		// 位置
		const D3DXVECTOR3& rRot	// 向き
	);

	// メンバ関数
	void AddExp(const int nAdd);				// 経験値加算
	void SetRespawn(D3DXVECTOR3& rPos);			// 再出現設定
	void SetDisp(const bool bDisp);				// 表示設定
	void SetEnableUpdate(const bool bUpdate);	// 更新状況設定
	void SetEnableDraw(const bool bDraw);		// 描画状況設定
	D3DXMATRIX GetMtxWorld(void) const;			// マトリックス取得
	float GetDistanceTarget(void) const;		// ターゲットとの距離取得
	int GetLevel(void) const;					// レベル取得
	int GetState(void) const;					// 状態取得
	float GetRadius(void) const;				// 半径取得

private:
	// メンバ関数
	MOTION UpdateNormal(void);	// 通常状態時の更新
	MOTION UpdateDamage(void);	// ダメージ状態時の更新
	void UpdateBlowAway(void);	// 吹っ飛び状態時の更新
	void UpdateFadeOut(void);	// フェードアウト状態時の更新
	void UpdateFadeIn(void);	// フェードイン状態時の更新

	MOTION Move(void);								// 移動
	MOTION Magic(MOTION motion, D3DXVECTOR3& rPos);	// 魔法
	bool Land(D3DXVECTOR3& rPos);					// 着地
	void Motion(int nMotion);						// モーション
	void Pos(D3DXVECTOR3& rPos);					// 位置
	void Rot(D3DXVECTOR3& rRot);					// 向き
	void CollisionTarget(D3DXVECTOR3& rPos);		// ターゲットとの当たり判定
	void CollisionEnemy(D3DXVECTOR3& rPos);			// 敵との当たり判定
	void LoadSetup(void);							// セットアップ

	// 静的メンバ変数
	static const char *mc_apModelFile[];	// モデル定数

	// メンバ変数
	CMagicManager	*m_pMagic;		// 魔法マネージャーの情報
	CExpManager		*m_pExp;		// 経験値マネージャーの情報
	CLevelupManager	*m_pLevelup;	// 強化マネージャーの情報
	CObjectGauge2D	*m_pLife;		// 体力の情報
	CShadow			*m_pShadow;		// 影の情報
	CObjectOrbit	*m_pOrbit;		// 軌跡の情報

	D3DXVECTOR3	m_oldPos;	// 過去位置
	D3DXVECTOR3	m_move;		// 移動量
	D3DXVECTOR3	m_destRot;	// 目標向き
	ROTATION m_rotation;	// 回転方向
	STATE m_state;			// 状態
	int   m_nCounterState;	// 状態管理カウンター
	int   m_nNumModel;		// パーツの総数
	float m_fDisTarget;		// ターゲットとの距離
	bool  m_bJump;			// ジャンプ状況
};

#endif	// _PLAYER_H_
