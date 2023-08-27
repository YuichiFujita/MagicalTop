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
class CWaveManager;	// ウェーブマネージャークラス
class CPause;		// ポーズクラス
class CStage;		// ステージクラス
class CPlayer;		// プレイヤークラス
class CTarget;		// ターゲットクラス
class CField;		// 地面クラス
class CScore;		// スコアクラス
class CTimer;		// タイマークラス

//************************************************************
//	クラス定義
//************************************************************
// ゲーム画面クラス
class CSceneGame : public CScene
{
public:
	// コンストラクタ
	CSceneGame();

	// デストラクタ
	~CSceneGame();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	HRESULT Uninit(void) override;	// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	// 静的メンバ関数
	static CWaveManager	*GetWaveManager(void);	// ウェーブマネージャー取得
	static CPause		*GetPause(void);		// ポーズ取得
	static CStage		*GetStage(void);		// ステージ取得
	static CPlayer		*GetPlayer(void);		// プレイヤー取得
	static CTarget		*GetTarget(void);		// ターゲット取得
	static CField		*GetField(void);		// 地面取得
	static CScore		*GetScore(void);		// スコア取得
	static CTimer		*GetTimer(void);		// タイマー取得

private:
	// 静的メンバ変数
	static CWaveManager	*m_pWaveManager;	// ウェーブマネージャー
	static CPause		*m_pPause;			// ポーズ
	static CStage		*m_pStage;			// ステージ
	static CPlayer		*m_pPlayer;			// プレイヤーオブジェクト
	static CTarget		*m_pTarget;			// ターゲットオブジェクト
	static CField		*m_pField;			// 地面オブジェクト
	static CScore		*m_pScore;			// スコアオブジェクト
	static CTimer		*m_pTimer;			// タイマーオブジェクト
};

#endif	// _SCENE_GAME_H_
