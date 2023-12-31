//============================================================
//
//	レンダラーヘッダー [renderer.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _RENDERER_H_
#define _RENDERER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	クラス定義
//************************************************************
// レンダラークラス
class CRenderer
{
public:
	// コンストラクタ
	CRenderer();

	// デストラクタ
	~CRenderer();

	// メンバ関数
	HRESULT Init(HWND hWnd, BOOL bWindow);		// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	LPDIRECT3DDEVICE9 GetDevice(void) const;	// デバイス取得

	// 静的メンバ関数
	static CRenderer *Create(HWND hWnd, BOOL bWindow);	// 生成
	static HRESULT Release(CRenderer *&pRenderer);		// 破棄

private:
	// メンバ関数
	HRESULT CRenderer::CreateDevice(HWND hWnd, D3DPRESENT_PARAMETERS d3dpp);	// デバイス生成

	// メンバ変数
	LPDIRECT3D9			m_pD3D;			// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9	m_pD3DDevice;	// Direct3Dデバイス
};

#endif	// _RENDERER_H_
