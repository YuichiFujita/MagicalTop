//============================================================
//
//	チュートリアルマネージャーヘッダー [tutorialManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TUTORIAL_MANAGER_H_
#define _TUTORIAL_MANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;		// オブジェクト2Dクラス
class CObjectGauge2D;	// オブジェクトゲージ2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// チュートリアルマネージャークラス
class CTutorialManager
{
public:
	// 状態列挙
	enum STATE
	{
		STATE_NONE = 0,		// 何もしない状態
		STATE_WAIT,			// 待機状態
		STATE_LETTER,		// 手紙表示状態
		STATE_FADEIN,		// フェードイン状態
		STATE_EXPLAIN,		// 説明表示状態
		STATE_FADEOUT,		// フェードアウト状態
		STATE_PROGRESSION,	// 進行状態
		STATE_NEXTWAIT,		// 次レッスン待機状態
		STATE_END,			// 終了状態
		STATE_MAX			// この列挙型の総数
	};

	// レッスン列挙
	enum LESSON
	{
		LESSON_NONE = 0,	// レッスン無し
		LESSON_01,			// レッスン01：吸い込まれる
		LESSON_02,			// レッスン02：前後加速
		LESSON_03,			// レッスン03：左右加速
		LESSON_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CTutorialManager();

	// デストラクタ
	~CTutorialManager();

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新

	void AddLessonCounter(void);	// レッスンカウンター加算
	int GetLesson(void) const;		// レッスン取得

	// 静的メンバ関数
	static CTutorialManager *Create(void);	// 生成
	static HRESULT Release(CTutorialManager *&prTutorialManager);	// 破棄

private:
	// メンバ関数
	void UpdateWait(void);		// 待機
	void UpdateLetter(void);	// 手紙表示
	void UpdateFadeIn(void);	// フェードイン
	void UpdateExplain(void);	// 説明表示
	void UpdateFadeOut(void);	// フェードアウト
	void UpdateNextWait(void);	// 次レッスン待機
	void NextLesson(void);		// 次レッスンへの移行

	// 静的メンバ変数
	static const char *mc_apTextureFile[];			// テクスチャ定数
	static const char *mc_apLessonTextureFile[];	// レッスンテクスチャ定数
	static const int mc_aNextLesson[];				// レッスン移行カウント
	static const int mc_aNextLessonWait[];			// 次レッスン余韻カウント

	// メンバ変数
	CObjectGauge2D *m_pConterLesson;	// レッスン管理カウンターの情報
	CObject2D *m_pFade;		// フェードの情報
	CObject2D *m_pExplain;	// 説明表示の情報
	STATE m_state;			// 状態
	int m_nCounterState;	// 状態管理カウンター
	int m_nLesson;			// レッスン
	float m_fScale;			// ポリゴン拡大率
};

#endif	// _TUTORIAL_MANAGER_H_
