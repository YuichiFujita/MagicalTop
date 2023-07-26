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
class CObjectGauge2D;	// オブジェクトゲージ2Dクラス
class CShadow;			// 影クラス

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
		MODEL_BODY = 0,	// 体
		MODEL_HEAD,		// 頭
		MODEL_SLEEVE_L,	// 左腕
		MODEL_SLEEVE_R,	// 右腕
		MODEL_WRIST_L,	// 左手首
		MODEL_WRIST_R,	// 右手首
		MODEL_HAND_L,	// 左手
		MODEL_HAND_R,	// 右手
		MODEL_PANTS_L,	// 左足
		MODEL_PANTS_R,	// 右足
		MODEL_LEG_L,	// 左腿
		MODEL_LEG_R,	// 右腿
		MODEL_SHOES_L,	// 左靴
		MODEL_SHOES_R,	// 右靴
		MODEL_KNIFE,	// ナイフ
		MODEL_MAX		// この列挙型の総数
	}MODEL;

	// モーション列挙
	typedef enum
	{
		MOTION_NEUTRAL = 0,	// 待機モーション
		MOTION_MOVE,		// 歩行モーション
		MOTION_ACTION,		// アクションモーション
		MOTION_JUMP,		// ジャンプモーション
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
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot		// 向き
	);

	// メンバ関数
	void SetEnableUpdate(const bool bUpdate);	// 更新状況設定
	void SetEnableDraw(const bool bDraw);		// 描画状況設定
	D3DXMATRIX GetMtxWorld(void) const;			// マトリックス取得
	float GetDistanceTarget(void) const;		// ターゲットとの距離取得
	float GetRadius(void) const;				// 半径取得

private:
	// メンバ関数
	MOTION Move(MOTION motion);						// 移動
	MOTION Jump(MOTION motion);						// ジャンプ
	MOTION Magic(MOTION motion, D3DXVECTOR3& rPos);	// 魔法
	MOTION Land(MOTION motion, D3DXVECTOR3& rPos);	// 着地
	void Motion(MOTION motion);						// モーション
	void Pos(D3DXVECTOR3& rPos);					// 位置
	void Rot(D3DXVECTOR3& rRot);					// 向き
	void CollisionTarget(D3DXVECTOR3& rPos);		// ターゲットとの当たり判定
	void CollisionEnemy(D3DXVECTOR3& rPos);			// 敵との当たり判定
	void LoadSetup(void);							// セットアップ

	// 静的メンバ変数
	static const char *mc_apModelFile[];	// モデル定数

	// メンバ変数
	CMagicManager *m_pMagic;	// 魔法マネージャーの情報
	CObjectGauge2D *m_pLife;	// 体力の情報
	CShadow		*m_pShadow;		// 影の情報
	D3DXVECTOR3	m_oldPos;		// 過去位置
	D3DXVECTOR3	m_move;			// 移動量
	D3DXVECTOR3	m_destRot;		// 目標向き
	ROTATION m_rotation;		// 回転方向
	int   m_nNumModel;			// パーツの総数
	float m_fDisTarget;			// ターゲットとの距離
	bool  m_bJump;				// ジャンプ状況
};

#endif	// _PLAYER_H_
