//============================================================
//
//	シーンヘッダー [scene.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SCENE_H_
#define _SCENE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	クラス定義
//************************************************************
// シーンクラス
class CScene
{
public:
	// モード列挙
	enum MODE
	{
		MODE_TITLE = 0,	// タイトル画面
		MODE_GAME,		// ゲーム画面
		MODE_RESULT,	// リザルト画面
		MODE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CScene();

	// デストラクタ
	~CScene();

	// 純粋仮想関数
	virtual HRESULT Init(void)		= 0;	// 初期化
	virtual HRESULT Uninit(void)	= 0;	// 終了
	virtual void Draw(void)			= 0;	// 描画

	// 仮想関数
	virtual void Update(void);	// 更新

	// 静的メンバ関数
	static CScene *Create(MODE mode);			// 生成
	static HRESULT Release(CScene *&prScene);	// 破棄

	// メンバ関数
	void SetMode(const MODE mode);	// モード設定
	MODE GetMode(void) const;		// モード取得

private:
	// メンバ変数
	MODE m_mode;	// モード
};

#endif	// _SCENE_H_
