//============================================================
//
//	ゲーム画面ヘッダー [sceneGame.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SCENE_GAME_H_
#define _SCENE_GAME_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "scene.h"

//************************************************************
//	前方宣言
//************************************************************
class CWaveManager;		// ウェーブマネージャークラス
class CTimerManager;	// タイマーマネージャークラス
class CPause;			// ポーズクラス
class CScore;			// スコアクラス
class CWarningSpawn;	// 出現警告表示クラス

//************************************************************
//	クラス定義
//************************************************************
// ゲーム画面クラス
class CSceneGame : public CScene
{
public:
	// コンストラクタ
	CSceneGame(const MODE mode);

	// デストラクタ
	~CSceneGame();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	HRESULT Uninit(void) override;	// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	// 静的メンバ関数
	static CWaveManager		*GetWaveManager(void);	// ウェーブマネージャー取得
	static CTimerManager	*GetTimerManager(void);	// タイマーマネージャー取得
	static CPause			*GetPause(void);		// ポーズ取得
	static CScore			*GetScore(void);		// スコア取得
	static CWarningSpawn	*GetWarningSpawn(void);	// 出現警告表示取得

private:
	// 静的メンバ変数
	static CWaveManager		*m_pWaveManager;	// ウェーブマネージャー
	static CTimerManager	*m_pTimerManager;	// タイマーマネージャー
	static CPause			*m_pPause;			// ポーズ
	static CScore			*m_pScore;			// スコアオブジェクト
	static CWarningSpawn	*m_pWarningSpawn;	// 出現警告表示オブジェクト
};

#endif	// _SCENE_GAME_H_
