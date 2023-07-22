//============================================================
//
//	ウェーブマネージャーヘッダー [waveManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _WAVEMANAGER_H_
#define _WAVEMANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	クラス定義
//************************************************************
// ウェーブマネージャークラス
class CWaveManager
{
public:
	// 状態列挙
	typedef enum
	{
		STATE_NONE = 0,		// 何もしない状態
		STATE_SEASON_START,	// 季節の開始状態
		STATE_WAVE_START,	// ウェーブ開始状態
		STATE_PROGRESSION,	// 季節の進行状態
		STATE_SEASON_END,	// 季節の終了状態
		STATE_WAIT,			// 次季節の開始待機状態
		STATE_MAX			// この列挙型の総数
	}STATE;

	// 季節列挙
	typedef enum
	{
		SEASON_SPRING = 0,	// 春
		SEASON_SUMMER,		// 夏
		SEASON_AUTUMN,		// 秋
		SEASON_WINTER,		// 冬
		SEASON_MAX			// この列挙型の総数
	}SEASON;

	// コンストラクタ
	CWaveManager();

	// デストラクタ
	~CWaveManager();

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	// 静的メンバ関数
	static CWaveManager *Create(void);	// 生成
	static HRESULT Release(CWaveManager *&prWaveManager);	// 破棄

private:
	// メンバ変数
	STATE m_state;				// 状態
	SEASON m_nSeasonCounter;	// 季節カウンター
	int m_nWaveCounter;			// ウェーブカウンター
};

#endif	// _WAVEMANAGER_H_
