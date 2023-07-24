//============================================================
//
//	シーズンヘッダー [season.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SEASON_H_
#define _SEASON_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "objectModel.h"

//************************************************************
//	クラス定義
//************************************************************
// シーズンクラス
class CSeason : public CObjectModel
{
public:
	// 種類列挙
	typedef enum
	{
		MODEL_NORMAL = 0,	// 通常
		MODEL_MAX			// この列挙型の総数
	}MODEL;

	// コンストラクタ
	CSeason();

	// デストラクタ
	~CSeason();

	// オーバーライド関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	// 静的メンバ関数
	static CSeason *Create(const MODEL model);	// 生成

private:
	// 静的メンバ変数
	static const char *mc_apModelFile[];	// モデル定数
};

#endif	// _SEASON_H_
