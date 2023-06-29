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
#include "object.h"
#include "motion.h"
#include "magic.h"

//************************************************************
//	前方宣言
//************************************************************
class CMultiModel;	// マルチモデルクラス
class CMotion;		// モーションクラス

//************************************************************
//	クラス定義
//************************************************************
// プレイヤークラス
class CPlayer : public CObject
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

	// コンストラクタ
	CPlayer();

	// デストラクタ
	~CPlayer();

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

	// オーバーライド関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	// 静的メンバ関数
	static CPlayer *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot		// 向き
	);

	// メンバ関数
	void SetPosition(const D3DXVECTOR3& rPos);	// 位置設定
	void SetRotation(const D3DXVECTOR3& rRot);	// 向き設定
	D3DXVECTOR3 GetPosition(void) const;		// 位置取得
	D3DXVECTOR3 GetRotation(void) const;		// 向き取得
	float GetRadius(void) const;				// 半径取得

private:
	// メンバ関数
	MOTION Move(MOTION motion);		// 移動
	MOTION Jump(MOTION motion);		// ジャンプ
	MOTION Magic(MOTION motion);	// 魔法
	MOTION Land(MOTION motion);		// 着地
	void Motion(MOTION motion);		// モーション
	void Collision(void);			// 当たり判定
	void Rot(void);					// 向き
	void Camera(void);				// カメラ
	void LoadSetup(void);			// セットアップ

	// 静的メンバ変数
	static const char *mc_apModelFile[];	// モデル定数

	// メンバ変数
	CMultiModel	*m_apMultiModel[MAX_PARTS];	// モデルの情報
	CMotion		*m_pMotion;	// モーションの情報
	D3DXMATRIX	m_mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3	m_pos;		// 現在位置
	D3DXVECTOR3	m_oldPos;	// 過去位置
	D3DXVECTOR3	m_move;		// 移動量
	D3DXVECTOR3	m_rot;		// 現在向き
	D3DXVECTOR3 m_destRot;	// 目標向き
	CMagic::TYPE m_magic;	// 魔法
	int m_nNumModel;		// パーツの総数
	bool m_bJump;			// ジャンプ状況
};

#endif	// _PLAYER_H_
