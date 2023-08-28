//============================================================
//
//	タイマーマネージャーヘッダー [timerManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TIMERMANAGER_H_
#define _TIMERMANAGER_H_

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
// タイマーマネージャークラス
class CTimerManager
{
public:
	// コンストラクタ
	CTimerManager();

	// デストラクタ
	~CTimerManager();

	// 計測列挙
	typedef enum
	{
		STATE_NONE = 0,	// 処理なし
		STATE_MEASURE,	// 計測中
		STATE_END,		// 計測終了 
		STATE_MAX		// この列挙型の総数
	}STATE;

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Start(void);	// 計測開始
	void End(void);		// 計測終了

	void EnableStop(const bool bStop);	// 計測停止設定
	void AddMSec(long nMSec);	// ミリ秒加算
	void AddSec(long nSec);		// 秒加算
	void AddMin(long nMin);		// 分加算
	int GetMSec(void);			// ミリ秒取得
	int GetSec(void);			// 秒取得
	int GetMin(void);			// 分取得

	// 静的メンバ関数
	static CTimerManager *Create(void);	// 生成
	static HRESULT Release(CTimerManager *&prTimerManager);	// 破棄

private:
	// メンバ関数
	void SetTexNum(void);	// 数字のテクスチャ座標設定

	// メンバ変数
	CValue *m_apValue[MAX_TIMER];	// 数値の情報
	DWORD m_dwStartTime;			// 開始時間
	DWORD m_dwTime;					// 経過時間
	DWORD m_dwStopStartTime;		// 停止開始時間
	DWORD m_dwStopTime;				// 停止時間
	DWORD m_dwTempTime;				// 経過時間の計算用
	STATE m_state;					// 計測状態
	bool  m_bStop;					// 計測停止状況
};

#endif	// _TIMERMANAGER_H_
