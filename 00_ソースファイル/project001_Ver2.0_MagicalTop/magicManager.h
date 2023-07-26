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
//	マクロ定義
//************************************************************
#define MAX_LOCK	(6)	// ロックオンの最大数

//************************************************************
//	前方宣言
//************************************************************
class CLockCursor;	// ロックオン表示

//************************************************************
//	クラス定義
//************************************************************
// 魔法マネージャークラス
class CMagicManager
{
public:
	// コンストラクタ
	CMagicManager();

	// デストラクタ
	~CMagicManager();

	// ロックオン情報構造体
	typedef struct
	{
		CObject *pObject;	// ロックオンしたオブジェクト
		float fLength;		// ロックオンオブジェクトとの距離
	}LockInfo;

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void LockOnMagic(const D3DXVECTOR3& rPos);	// 魔法ロックオン
	void ShotMagic(void);						// 魔法発射
	void SetMagic(const CMagic::TYPE type);		// 魔法設定
	CMagic::TYPE GetMagic(void) const;			// 魔法取得
	void DeleteLockOn(void);					// 魔法ロックオン全削除

	// 静的メンバ関数
	static CMagicManager *Create(void);	// 生成
	static HRESULT Release(CMagicManager *&prMagicManager);	// 破棄

private:
	// メンバ関数
	void SortLockOnMagic(LockInfo *pLock, const int nNumLock, CObject *pObject, const float fLength);	// ロックオンソート

	// メンバ変数
	CLockCursor *m_apLockCursor[MAX_LOCK];	// ロックオン表示情報
	CMagic::TYPE m_magic;	// 魔法
};

#endif	// _MAGICMANAGER_H_
