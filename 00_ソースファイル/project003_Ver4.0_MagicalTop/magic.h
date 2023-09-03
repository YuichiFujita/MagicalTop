//============================================================
//
//	魔法ヘッダー [magic.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _MAGIC_H_
#define _MAGIC_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object.h"
#include "motion.h"

//************************************************************
//	前方宣言
//************************************************************
class CBubble;	// バブルクラス
class CShadow;	// 影クラス

//************************************************************
//	クラス定義
//************************************************************
// 魔法クラス
class CMagic : public CObject
{
public:
	// 状態列挙
	typedef enum
	{
		STATE_NORMAL = 0,	// 通常状態
		STATE_INHALE,		// 吸い込まれ状態
		STATE_DELETE,		// 消失状態
		STATE_MAX			// この列挙型の総数
	}STATE;

	// コンストラクタ
	CMagic();

	// デストラクタ
	~CMagic();

	// ステータス構造体
	typedef struct
	{
		D3DXVECTOR3 shotPos;	// 発射位置
		int		nShotParts;		// 発射パーツ
		int		nLife;			// 寿命
		int		nCoolTime;		// クールタイム
		float	fMove;			// 移動量
		float	bubbleRadius;	// バブル半径
	}StatusInfo;

	// オーバーライド関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	// 静的メンバ関数
	static void LoadSetup(void);	// セットアップ
	static CMagic *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const D3DXVECTOR3& rVec		// 移動方向
	);
	static StatusInfo GetStatusInfo(void);	// ステータス情報取得

	// メンバ関数
	void SetMove(D3DXVECTOR3 vec, const float fMove);	// 移動量設定
	void SetPosition(const D3DXVECTOR3& rPos);	// 位置設定
	void SetRotation(const D3DXVECTOR3& rRot);	// 向き設定
	D3DXVECTOR3 GetPosition(void) const;	// 位置取得
	D3DXVECTOR3 GetRotation(void) const;	// 向き取得

private:
	// メンバ関数
	bool CollisionEnemy(void);	// 敵との当たり判定

	// 静的メンバ変数
	static StatusInfo m_statusInfo;	// ステータス情報

	// メンバ変数
	CBubble *m_pBubble;		// バブル情報
	CShadow *m_pShadow;		// 影の情報
	D3DXMATRIX	m_mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3	m_pos;		// 現在位置
	D3DXVECTOR3	m_movePos;	// 位置移動量
	D3DXVECTOR3	m_rot;		// 向き
	D3DXVECTOR3	m_moveRot;	// 向き変更量
	STATE m_state;			// 状態
};

#endif	// _MAGIC_H_
