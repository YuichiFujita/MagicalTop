//============================================================
//
//	マナフラワーヘッダー [flower.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _FLOWER_H_
#define _FLOWER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "objectBillboard.h"

//************************************************************
//	前方宣言
//************************************************************
class CShadow;	// 影クラス

//************************************************************
//	クラス定義
//************************************************************
// マナフラワークラス
class CFlower : public CObjectBillboard
{
public:
	// 種類列挙
	typedef enum
	{
		TYPE_NORMAL = 0,	// 通常
		TYPE_MAX			// この列挙型の総数
	}TYPE;

	// コンストラクタ
	CFlower();

	// デストラクタ
	~CFlower();

	// オーバーライド関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	// 静的メンバ関数
	static CFlower *Create	// 生成
	( // 引数
		const TYPE type,			// 種類
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rSize,	// 大きさ
		const int nLife				// 体力
	);

	// メンバ関数
	void SetLife(const int nLife);	// 体力設定

private:
	// メンバ関数
	bool CollisionPlayer(void);	// プレイヤーとの当たり判定

	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数

	// メンバ変数
	CShadow *m_pShadow;	// 影の情報
	TYPE m_type;		// 種類
	int m_nLife;		// 体力
};

#endif	// _FLOWER_H_
