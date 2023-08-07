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
class CObjectTornado;	// オブジェクト竜巻クラス

//************************************************************
//	クラス定義
//************************************************************
// 魔法クラス
class CMagic : public CObject
{
public:
	// 種類列挙
	typedef enum
	{
		TYPE_LV0_NORMAL = 0,	// 初期魔法
		TYPE_LV1_FIRE,			// LV1炎魔法
		TYPE_LV1_WATER,			// LV1水魔法
		TYPE_LV1_WIND,			// LV1風魔法
		TYPE_LV2_FIRE,			// LV2炎魔法
		TYPE_LV2_WATER,			// LV2水魔法
		TYPE_LV2_WIND,			// LV2風魔法
		TYPE_MAX				// この列挙型の総数
	}TYPE;

	// コンストラクタ
	CMagic(const TYPE type);

	// デストラクタ
	~CMagic();

	// ステータス構造体
	typedef struct
	{
		D3DXVECTOR3 shotPos;	// 発射位置
		int		nShotParts;		// 発射パーツ
		int		nLock;			// ロックオン数
		int		nLife;			// 寿命
		int		nDamage;		// 攻撃力
		float	fRadius;		// 半径
		float	fHeight;		// 縦幅
		float	fMove;			// 移動量
		float	fViewRadius;	// 視界範囲
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
		const TYPE type,			// 種類
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const D3DXVECTOR3& rVec		// 移動方向
	);
	static StatusInfo GetStatusInfo(const TYPE type);	// ステータス情報取得

	// メンバ関数
	void SetMove(D3DXVECTOR3 vec, const float fMove);	// 移動量設定
	void SetLife(const int nLife);						// 寿命設定
	void SetPosition(const D3DXVECTOR3& rPos);			// 位置設定
	void SetRotation(const D3DXVECTOR3& rRot);			// 向き設定
	D3DXVECTOR3 GetPosition(void) const;				// 位置取得
	D3DXVECTOR3 GetRotation(void) const;				// 向き取得
	StatusInfo GetStatusInfo(void) const;				// ステータス情報取得

	// 仮想関数
	virtual bool Collision(CObject *pObject);	// 魔法判定

private:
	// メンバ関数
	bool CollisionEnemy(void);	// 敵との当たり判定

	// 静的メンバ変数
	static StatusInfo m_aStatusInfo[TYPE_MAX];	// ステータス情報

	// メンバ変数
	const StatusInfo m_status;	// ステータス定数
	D3DXMATRIX	m_mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3	m_pos;			// 現在位置
	D3DXVECTOR3	m_movePos;		// 位置移動量
	D3DXVECTOR3	m_rot;			// 向き
	D3DXVECTOR3	m_moveRot;		// 向き変更量
	int m_nLife;				// 寿命
};

// 初期魔法クラス
class CNormalMagic : public CMagic
{
public:
	// コンストラクタ
	CNormalMagic(const TYPE type);

	// デストラクタ
	~CNormalMagic();

	// オーバーライド関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
};

// LV2風魔法クラス
class CLv2WindMagic : public CMagic
{
public:
	// コンストラクタ
	CLv2WindMagic(const TYPE type);

	// デストラクタ
	~CLv2WindMagic();

	// オーバーライド関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	bool Collision(CObject *pObject);	// 魔法判定

private:
	// メンバ変数
	CObjectTornado *m_pTornado;	// 竜巻オブジェクト
};

#endif	// _MAGIC_H_
