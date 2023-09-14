//============================================================
//
//	タイトルマネージャーヘッダー [titleManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TITLEMANAGER_H_
#define _TITLEMANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "enemy.h"

//************************************************************
//	マクロ定義
//************************************************************
#define NUM_LOGO	(2)	// ロゴの使用数

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;	// オブジェクト2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// タイトルマネージャークラス
class CTitleManager
{
public:
	// テクスチャ列挙
	typedef enum
	{
		TEXTURE_NORMAL = 0,	// タイトル
		TEXTURE_MAX			// この列挙型の総数
	}TEXTURE;

	// ロゴ列挙
	typedef enum
	{
		LOGO_MAGICAL = 0,	// MAGICAL
		LOGO_TOP,			// TOP
		LOGO_MAX			// この列挙型の総数
	}LOGO;

	// 状態列挙
	typedef enum
	{
		STATE_NONE = 0,		// 何もしない状態
		STATE_FADEOUT,		// フェードアウト状態
		STATE_MOVE,			// タイトル縮小状態
		STATE_WAIT,			// 遷移待機状態
		STATE_MAX			// この列挙型の総数
	}STATE;

	// コンストラクタ
	CTitleManager();

	// デストラクタ
	~CTitleManager();

	// メンバ関数
	HRESULT Init(void);		// 初期化
	void Uninit(void);		// 終了
	void Update(void);		// 更新

	// 静的メンバ関数
	static CTitleManager *Create(void);	// 生成
	static HRESULT Release(CTitleManager *&prTitleManager);	// 破棄

private:
	// メンバ関数
	void UpdateFade(void);	// フェードアウト
	void UpdateMove(void);	// タイトル移動
	void UpdateStart(void);	// 遷移決定
	void SkipStaging(void);	// 演出スキップ

	// 静的メンバ変数
	static const char *mc_apTextureFile[];		// テクスチャ定数
	static const char *mc_apLogoTextureFile[];	// ロゴテクスチャ定数

	// メンバ変数
	CObject2D *m_apLogo[NUM_LOGO];	// タイトル表示の情報
	CObject2D *m_pFade;		// フェードの情報
	STATE m_state;			// 状態
	int m_nCounterState;	// 状態管理カウンター
	float m_fScale;			// タイトル拡大率
};

#endif	// _TITLEMANAGER_H_
