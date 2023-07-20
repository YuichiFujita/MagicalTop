//============================================================
//
//	入力ヘッダー [input.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _INPUT_H_
#define _INPUT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	マクロ定義
//************************************************************
// キーボード
#define MAX_NUMKEY	(256)	// キーの最大数

// パッド用
#define MAX_PLAYER	(4)		// プレイヤーの最大数

//************************************************************
//	クラス定義
//************************************************************
// 入力クラス
class CInput
{
public:
	// コンストラクタ
	CInput();

	// デストラクタ
	virtual ~CInput();

	// 仮想関数
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期化
	virtual void Uninit(void);		// 終了

	// 純粋仮想関数
	virtual void Update(void) = 0;	// 更新

protected:
	// メンバ変数
	static int m_nNumAll;	// 入力デバイスの総数
	static LPDIRECTINPUT8 m_pInput;		// DirectInputオブジェクト
	LPDIRECTINPUTDEVICE8  m_pDevice;	// DirectInputデバイス
};

// キーボードクラス
class CInputKeyboard : public CInput
{
public:
	// コンストラクタ
	CInputKeyboard();

	// デストラクタ
	~CInputKeyboard();

	// オーバーライド関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新

	// 静的メンバ関数
	static CInputKeyboard *Create(HINSTANCE hInstance, HWND hWnd);	// 生成
	static HRESULT Release(CInputKeyboard *&pKeyboard);				// 破棄

	// メンバ関数
	bool GetPress(int nKey);	// プレス取得
	bool GetTrigger(int nKey);	// トリガー取得
	bool GetRelease(int nKey);	// リリース取得

private:
	// メンバ変数
	BYTE m_aKeyStatePress[MAX_NUMKEY];		// プレス情報
	BYTE m_aKeyStateTrigger[MAX_NUMKEY];	// トリガー情報
	BYTE m_aKeyStateRelease[MAX_NUMKEY];	// リリース情報
};

// マウスクラス
class CInputMouse : public CInput
{
public:
	// コンストラクタ
	CInputMouse();

	// デストラクタ
	~CInputMouse();

	// マウスキー列挙
	typedef enum
	{
		KEY_LEFT = 0,	// マウス右クリック
		KEY_RIGHT,		// マウス左クリック
		KEY_WHEEL,		// マウスホイール押し込み
		KEY_MAX			// この列挙型の総数
	}KEY;

	// オーバーライド関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新

	// 静的メンバ関数
	static CInputMouse *Create(HINSTANCE hInstance, HWND hWnd);	// 生成
	static HRESULT Release(CInputMouse *&pMouse);				// 破棄

	// メンバ関数
	bool GetPress(int nMouKey);		// プレス取得
	bool GetTrigger(int nMouKey);	// トリガー取得
	bool GetRelease(int nMouKey);	// リリース取得
	D3DXVECTOR3 GetMove(void);		// 移動量取得

private:
	// メンバ変数
	DIMOUSESTATE m_mouseStatePress;		// マウスのプレス情報
	DIMOUSESTATE m_mouseStateTrigger;	// マウスのトリガー情報
	DIMOUSESTATE m_mouseStateRelease;	// マウスのリリース情報
};

// パッドクラス
class CInputPad
{
public:
	// コンストラクタ
	CInputPad();

	// デストラクタ
	~CInputPad();

	// パッドキー列挙
	typedef enum
	{
		KEY_UP = 0,		// 方向キー 上
		KEY_DOWN,		// 方向キー 下
		KEY_LEFT,		// 方向キー 左
		KEY_RIGHT,		// 方向キー 右
		KEY_START,		// STARTボタン
		KEY_BACK,		// BACKボタン
		KEY_LSTICKPUSH,	// 左スティック押し込み
		KEY_RSTICKPUSH,	// 右スティック押し込み
		KEY_L1,			// LBボタン
		KEY_R1,			// RBボタン
		KEY_L2,			// LTボタン
		KEY_R2,			// RTボタン
		KEY_A,			// Aボタン
		KEY_B,			// Bボタン
		KEY_X,			// Xボタン
		KEY_Y,			// Yボタン
		KEY_LSTICK_X,	// 左スティック 左右
		KEY_LSTICK_Y,	// 左スティック 上下
		KEY_RSTICK_X,	// 右スティック 左右
		KEY_RSTICK_Y,	// 右スティック 上下
		KEY_MAX			// この列挙型の総数
	}KEY;
	
	// 振動種類列挙
	typedef enum
	{
		TYPE_NONE = 0,		// 何もしていない
		TYPE_DAMAGE,		// ダメージ
		TYPE_BIG_DAMAGE,	// 大ダメージ
		TYPE_DEATH,			// 死亡
		TYPE_MAX			// この列挙型の総数
	}TYPE;

	// 振動構造体
	typedef struct
	{
		_XINPUT_VIBRATION vibration;	// バイブ情報
		TYPE type;	// バイブ種類
		int  nTime;	// バイブ時間
	}VIBRATION;

	// 静的メンバ関数
	static CInputPad *Create(void);				// 生成
	static HRESULT Release(CInputPad *&pPad);	// 破棄

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void SetVibration(TYPE type, int nPlayer = 0);	// バイブ設定
	bool GetPress(KEY nJoyKey, int nPlayer = 0);	// プレス取得 (ボタン)
	SHORT GetPressLStickX(int nPlayer = 0);			// プレス取得 (LスティックX)
	SHORT GetPressLStickY(int nPlayer = 0);			// プレス取得 (LスティックY)
	SHORT GetPressRStickX(int nPlayer = 0);			// プレス取得 (RスティックX)
	SHORT GetPressRStickY(int nPlayer = 0);			// プレス取得 (RスティックY)
	bool GetTrigger(KEY nJoyKey, int nPlayer = 0);	// トリガー取得 (ボタン)
	bool GetRelease(KEY nJoyKey, int nPlayer = 0);	// リリース取得 (ボタン)

private:
	// メンバ関数
	void UpdateVibration(VIBRATION *pVibration, int nID);	// バイブレーション更新

	// メンバ変数
	VIBRATION    m_aVibration[MAX_PLAYER];			// パッドのバイブ情報
	XINPUT_STATE m_aJoyKeyStatePress[MAX_PLAYER];	// パッドのプレス情報
	XINPUT_STATE m_aJoyKeyStateTrigger[MAX_PLAYER];	// パッドのトリガー情報
	XINPUT_STATE m_aJoyKeyStateRelease[MAX_PLAYER];	// パッドのリリース情報
};

#endif	// _INPUT_H_
