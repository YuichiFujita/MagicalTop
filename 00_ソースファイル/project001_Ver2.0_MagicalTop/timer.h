//============================================================
//
//	タイマーヘッダー [timer.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TIMER_H_
#define _TIMER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	マクロ定義
//************************************************************
#define MAX_MIN		(2)	// タイマーの分の桁数
#define MAX_SEC		(2)	// タイマーの秒の桁数
#define MAX_MSEC	(3)	// タイマーのミリ秒の桁数

#define MAX_TIMER	(MAX_MIN + MAX_SEC + MAX_MSEC)	// タイマーの桁数 (分・秒・ミリ秒)

//************************************************************
//	前方宣言
//************************************************************
class CValue;	// 数字クラス

//************************************************************
//	クラス定義
//************************************************************
// タイマークラス
class CTimer : public CObject
{
public:
	// コンストラクタ
	CTimer();

	// デストラクタ
	~CTimer();

	// 計測列挙
	typedef enum
	{
		STATE_NONE = 0,	// 処理なし
		STATE_MEASURE,	// 計測中
		STATE_END,		// 計測終了 
		STATE_MAX		// この列挙型の総数
	}STATE;

	// オーバーライド関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	// 静的メンバ関数
	static CTimer *Create(void);	// 生成

	// メンバ関数
	void Start(void);	// 計測開始
	void End(void);		// 計測終了
	void EnableStop(const bool bStop);	// 計測停止設定

#if 0
	void AddMSec(const int nMSec);	// ミリ秒加算
	void AddSec(const int nSec);	// 秒加算
	void AddMin(const int nMin);	// 分加算
#endif

	int GetMSec(void);	// ミリ秒取得
	int GetSec(void);	// 秒取得
	int GetMin(void);	// 分取得

private:
	// メンバ関数
	void SetTexNum(void);	// 数字のテクスチャ座標設定

	// メンバ変数
	CValue *m_apValue[MAX_TIMER];	// 数値の情報
	DWORD m_dwStartTime;	// 開始時間
	DWORD m_dwTime;			// 経過時間
	DWORD m_dwStopTime;		// 停止時間
	STATE m_state;			// 計測状態
	bool  m_bStop;			// 計測停止状況
};

#endif	// _TIMER_H_

// TODO：タイマー加減算できるようにする
