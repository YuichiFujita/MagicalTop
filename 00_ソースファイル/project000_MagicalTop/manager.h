//============================================================
//
//	マネージャーヘッダー [manager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	前方宣言
//************************************************************
class CRenderer;		// レンダラークラス
class CInputKeyboard;	// キーボードクラス
class CInputMouse;		// マウスクラス
class CInputPad;		// パッドクラス
class CSound;			// サウンドクラス
class CCamera;			// カメラクラス
class CLight;			// ライトクラス
class CTexture;			// テクスチャクラス
class CModel;			// モデルクラス
class CStage;			// ステージクラス
class CPlayer;			// プレイヤークラス
class CTarget;			// ターゲットクラス
class CScore;			// スコアクラス
class CTimer;			// タイマークラス
class CDebugProc;		// デバッグ表示クラス

#ifdef _DEBUG	// デバッグ処理

class CDebug;		// デバッグクラス

#endif	// _DEBUG

//************************************************************
//	クラス定義
//************************************************************
// マネージャークラス
class CManager
{
public:
	// コンストラクタ
	CManager();

	// デストラクタ
	~CManager();

	// メンバ関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);	// 初期化
	HRESULT Uninit(void);	// 終了
	void Update(void);		// 更新
	void Draw(void);		// 描画

	// 静的メンバ関数
	static CManager *Create(HINSTANCE hInstance, HWND hWnd);	// 生成
	static HRESULT Release(CManager *&prManager);	// 破棄

	static CRenderer		*GetRenderer(void);		// レンダラー取得
	static CInputKeyboard	*GetKeyboard(void);		// キーボード取得
	static CInputMouse		*GetMouse(void);		// マウス取得
	static CInputPad		*GetPad(void);			// パッド取得
	static CSound			*GetSound(void);		// サウンド取得
	static CCamera			*GetCamera(void);		// カメラ取得
	static CLight			*GetLight(void);		// ライト取得
	static CTexture			*GetTexture(void);		// テクスチャ取得
	static CModel			*GetModel(void);		// モデル取得
	static CStage			*GetStage(void);		// ステージ取得
	static CPlayer			*GetPlayer(void);		// プレイヤー取得
	static CTarget			*GetTarget(void);		// ターゲット取得
	static CScore			*GetScore(void);		// スコア取得
	static CTimer			*GetTimer(void);		// タイマー取得
	static CDebugProc		*GetDebugProc(void);	// デバッグ表示取得

#ifdef _DEBUG	// デバッグ処理

	// 静的メンバ関数
	static CDebug *GetDebug(void);	// デバッグ取得

#endif	// _DEBUG

private:
	// 静的メンバ変数
	static CRenderer		*m_pRenderer;	// レンダラーオブジェクト
	static CInputKeyboard	*m_pKeyboard;	// キーボードオブジェクト
	static CInputMouse		*m_pMouse;		// マウスオブジェクト
	static CInputPad		*m_pPad;		// パッドオブジェクト
	static CSound			*m_pSound;		// サウンドオブジェクト
	static CCamera			*m_pCamera;		// カメラオブジェクト
	static CLight			*m_pLight;		// ライトオブジェクト
	static CTexture			*m_pTexture;	// テクスチャオブジェクト
	static CModel			*m_pModel;		// モデルオブジェクト
	static CStage			*m_pStage;		// ステージオブジェクト
	static CPlayer			*m_pPlayer;		// プレイヤーオブジェクト
	static CTarget			*m_pTarget;		// ターゲットオブジェクト
	static CScore			*m_pScore;		// スコアオブジェクト
	static CTimer			*m_pTimer;		// タイマーオブジェクト
	static CDebugProc		*m_pDebugProc;	// デバッグ表示オブジェクト

#ifdef _DEBUG	// デバッグ処理

	// 静的メンバ変数
	static CDebug *m_pDebug;	// デバッグオブジェクト

#endif	// _DEBUG

};

#endif	// _MANAGER_H_
