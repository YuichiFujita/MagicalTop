//============================================================
//
//	魔法マネージャーヘッダー [magicManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _MAGICMANAGER_H_
#define _MAGICMANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "magic.h"

//************************************************************
//	前方宣言
//************************************************************
class CObjectGauge2D;	// オブジェクトゲージ2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// 魔法マネージャークラス
class CMagicManager
{
public:
	// 状態列挙
	typedef enum
	{
		STATE_NORMAL = 0,	// 通常状態
		STATE_ATTACK,		// 攻撃状態
		STATE_HEAL,			// 回復状態
		STATE_MAX,			// この列挙型の総数
	}STATE;

	// コンストラクタ
	CMagicManager();

	// デストラクタ
	~CMagicManager();

	// メンバ関数
	HRESULT Init(void);		// 初期化
	void Uninit(void);		// 終了
	void Update(void);		// 更新
	bool ShotMagic(void);	// 魔法発射

	// 静的メンバ関数
	static CMagicManager *Create(void);	// 生成
	static HRESULT Release(CMagicManager *&prMagicManager);	// 破棄

private:
	// メンバ変数
	CObjectGauge2D *m_pMana;	// マナの情報
	STATE m_state;				// 状態
	int m_nCounterMagic;		// 魔法管理カウンター
	int m_nCounterState;		// 状態管理カウンター
	int m_nCounterHeal;			// 回復管理カウンター
};

#endif	// _MAGICMANAGER_H_
