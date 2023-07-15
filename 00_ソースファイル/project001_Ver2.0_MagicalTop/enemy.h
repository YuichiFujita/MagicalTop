//============================================================
//
//	敵ヘッダー [enemy.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENEMY_H_
#define _ENEMY_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "objectChara.h"

//************************************************************
//	前方宣言
//************************************************************
class CLifeGauge3D;	// 体力ゲージ3Dクラス
class CMultiModel;	// マルチモデルクラス

//************************************************************
//	クラス定義
//************************************************************
// 敵クラス
class CEnemy : public CObjectChara
{
public:
	// 種類列挙
	typedef enum
	{
		TYPE_WALK = 0,	// 歩兵
		TYPE_CAR,		// 戦車
		TYPE_FLY,		// ヘリ
		TYPE_MAX		// この列挙型の総数
	}TYPE;

	// コンストラクタ
	CEnemy(const TYPE type);

	// デストラクタ
	~CEnemy();

	// パーツ情報構造体
	typedef struct
	{
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 rot;	// 向き
		int nParentID;		// 親インデックス
	}Parts;

	// パーツ構造体
	typedef struct
	{
		Parts aInfo[MAX_PARTS];	// パーツ情報
		int nNumParts;			// パーツ数
	}PartsInfo;

	// ステータス構造体
	typedef struct
	{
		D3DXVECTOR3 bullPos;		// 弾の発射位置
		int		nLife;				// 体力
		int		nScore;				// スコア加算量
		int		nCounterAttack;		// 攻撃頻度
		int		nBullParts;			// 弾の発射パーツ
		int		nBullLife;			// 弾の寿命
		int		nBullDamage;		// 弾の攻撃力
		float	fBullRadius;		// 弾の半径
		float	fBullMove;			// 弾の移動量
		float	fRadius;			// 半径
		float	fHeight;			// 縦幅
		float	fForwardMove;		// 前進の移動量
		float	fBackwardMove;		// 後退の移動量
		float	fLookRevision;		// プレイヤー方向を向く補正係数
		float	fLifeUp;			// 体力表示のY位置加算量
		float	fFindRadius;		// 検知範囲
		float	fAttackRadius;		// 攻撃範囲
		float	fBackwardRadius;	// 後退範囲
		bool	bBackward;			// 後退の ON/OFF
	}StatusInfo;

	// オーバーライド関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	void Hit(const int nDmg);	// ヒット

	// 静的メンバ関数
	static void LoadSetup(void);	// セットアップ
	static CEnemy *Create	// 生成
	( // 引数
		const TYPE type,			// 種類
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot		// 向き
	);

	// メンバ関数
	void SetMovePosition(const D3DXVECTOR3& rMove);		// 位置移動量設定
	void SetMoveRotation(const D3DXVECTOR3& rMove);		// 向き変更量設定
	D3DXMATRIX GetMtxWorld(void) const;					// マトリックス取得
	D3DXVECTOR3 GetOldPosition(void) const;				// 過去位置取得
	D3DXVECTOR3 GetMovePosition(void) const;			// 位置移動量取得
	D3DXVECTOR3 GetMoveRotation(void) const;			// 向き変更量取得
	float GetRadius(void) const;						// 半径取得
	StatusInfo GetStatusInfo(void) const;				// ステータス情報取得
	PartsInfo GetPartsInfo(void) const;					// パーツ情報取得

	// 純粋仮想関数
	virtual const char* GetModelFileName(const int nModel) const = 0;	// モデルファイル取得

protected:
	// 仮想関数
	virtual void CollisionFind(void);	// 検知範囲の当たり判定

	// メンバ関数
	void Look	// 対象視認
	( // 引数
		const D3DXVECTOR3& rPosLook,	// 視認対象位置
		const D3DXVECTOR3& rPosEnemy,	// 敵位置
		D3DXVECTOR3& rRotEnemy			// 敵向き
	);

	void Attack(const D3DXVECTOR3& rTarget);	// 攻撃
	void CollisionTarget(D3DXVECTOR3& rPos);	// ターゲットとの当たり判定
	void CollisionEnemy(D3DXVECTOR3& rPos);		// 敵との当たり判定

private:
	// 静的メンバ変数
	static StatusInfo m_aStatusInfo[TYPE_MAX];	// ステータス情報
	static PartsInfo m_aPartsInfo[TYPE_MAX];	// パーツ情報

	// メンバ変数
	CLifeGauge3D *m_pLifeGauge;	// 体力の情報
	const StatusInfo m_status;	// ステータス定数
	const PartsInfo m_parts;	// パーツ定数
	D3DXVECTOR3	m_oldPos;		// 過去位置
	D3DXVECTOR3	m_movePos;		// 位置移動量
	D3DXVECTOR3	m_moveRot;		// 向き変更量
	int m_nCounterAtk;			// 攻撃管理カウンター
};

// 戦車敵クラス
class CEnemyCar : public CEnemy
{
public:
	// 種類列挙
	typedef enum
	{
		MODEL_CATERPILLAR = 0,	// キャタピラ
		MODEL_CANNON,			// キャノン
		MODEL_MAX				// この列挙型の総数
	}MODEL;

	// コンストラクタ
	CEnemyCar(const TYPE type);

	// デストラクタ
	~CEnemyCar();

	// オーバーライド関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	const char* GetModelFileName(const int nModel) const;	// モデルファイル取得

private:
	// オーバーライド関数
	void CollisionFind(void);	// 検知範囲の当たり判定

	// メンバ関数
	bool SetRotationCannon		// キャノン向き設定
	( // 引数
		const D3DXVECTOR3& rLookPos,	// 視認対象位置
		const D3DXVECTOR3& rEnemyPos,	// 敵位置
		const D3DXVECTOR3& rEnemyRot	// 敵向き
	);

	// 静的メンバ変数
	static const char *mc_apModelFile[];	// モデル定数
};

#endif	// _ENEMY_H_
