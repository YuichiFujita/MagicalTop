//============================================================
//
//	ゲームプレイヤーヘッダー [playerGame.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _PLAYER_GAME_H_
#define _PLAYER_GAME_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "player.h"

//************************************************************
//	クラス定義
//************************************************************
// ゲームプレイヤークラス
class CPlayerGame : public CPlayer
{
public:
	// コンストラクタ
	CPlayerGame();

	// デストラクタ
	~CPlayerGame();

	// オーバーライド関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新

private:
	// メンバ関数
	MOTION UpdateNormal(void);	// 通常状態時の更新
};

#endif	// _PLAYER_GAME_H_
