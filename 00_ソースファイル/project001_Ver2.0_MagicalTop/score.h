//============================================================
//
//	スコアヘッダー [score.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SCORE_H_
#define _SCORE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	マクロ定義
//************************************************************
#define SCORE_PRIO	(6)	// スコアの優先順位
#define MAX_SCORE	(8)	// スコアの桁数

//************************************************************
//	前方宣言
//************************************************************
class CValue;	// 数字クラス

//************************************************************
//	クラス定義
//************************************************************
// スコアクラス
class CScore : public CObject
{
public:
	// コンストラクタ
	CScore();

	// デストラクタ
	~CScore();

	// オーバーライド関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	// 静的メンバ関数
	static CScore *Create(void);	// 生成

	// メンバ関数
	void Add(const int nNum);	// 加算
	void Set(const int nNum);	// 設定
	int  Get(void);				// 取得

private:
	// メンバ関数
	void SetTexNum(void);	// 数字のテクスチャ座標設定

	// メンバ変数
	CValue *m_apValue[MAX_SCORE];	// 数値の情報
	int m_nNum;	// スコア
};

#endif	// _SCORE_H_
