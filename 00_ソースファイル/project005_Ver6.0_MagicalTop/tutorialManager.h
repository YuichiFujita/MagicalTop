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
class CObject2D;	// オブジェクト2Dクラス

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
		STATE_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CTutorialManager();

	// デストラクタ
	~CTutorialManager();

	// メンバ関数
	HRESULT Init(void);		// 初期化
	void Uninit(void);		// 終了
	void Update(void);		// 更新

	// 静的メンバ関数
	static CTutorialManager *Create(void);	// 生成
	static HRESULT Release(CTutorialManager *&prTutorialManager);	// 破棄

private:
	// 静的メンバ変数
	static const char *mc_apLogoTextureFile[];		// ロゴテクスチャ定数
	static const char *mc_apSelectTextureFile[];	// 選択テクスチャ定数

	// メンバ変数
	STATE m_state;	// 状態
};

#endif	// _TUTORIAL_MANAGER_H_
