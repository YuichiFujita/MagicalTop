//============================================================
//
//	エディット処理 [edit.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "edit.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "collision.h"
#include "debugproc.h"
#include "player.h"
#include "field.h"

//************************************************************
//	親クラス [CEdit] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEdit::CEdit()
{
	// メンバ変数をクリア

}

//============================================================
//	デストラクタ
//============================================================
CEdit::~CEdit()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEdit::Init(void)
{
	// メンバ変数を初期化


	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CEdit::Uninit(void)
{

}

//============================================================
//	更新処理
//============================================================
void CEdit::Update(void)
{
#define PLAY_RADIUS	(100.0f)	// 
#define VTX_RADIUS	(10.0f)		// 
#define VTX_UP		(5.0f)		// 

	// ポインタを宣言
	CInputKeyboard *pKey = CManager::GetKeyboard();	// キーボード情報
	CPlayer *pPlayer = CManager::GetPlayer();		// プレイヤー情報
	CField *pField = CManager::GetField();			// フィールド情報

	if (pKey->GetPress(DIK_RETURN))
	{ // [Enter] が押された場合

		// 変数を宣言
		D3DXVECTOR3 posVtx;	// 頂点位置
		D3DXVECTOR3 posPlayer = pPlayer->GetPosition();	// プレイヤー位置
		int nNumVtx = pField->GetNumVertex();			// 頂点数

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{ // 頂点数分繰り返す

			// 頂点位置を取得
			posVtx = pField->GetMeshVertexPosition(nCntVtx);

			if (collision::Circle2D(posPlayer, posVtx, PLAY_RADIUS, VTX_RADIUS))
			{ // 範囲内の場合

				// TODO：いい感じにする

				// 頂点との距離を求める
				float f = sqrtf(((posPlayer.x - posVtx.x) * (posPlayer.x - posVtx.x)) + ((posPlayer.z - posVtx.z) * (posPlayer.z - posVtx.z)) * 0.5f);
#if 1
				float ff = VTX_UP * -((f / (PLAY_RADIUS + VTX_RADIUS)) - 1.0f);

				// 頂点の高さを変更
				pField->SetMeshVertexPosition(nCntVtx, D3DXVECTOR3(posVtx.x, posVtx.y + ff, posVtx.z));
#else
				float fff = f * -(((D3DX_PI * 0.5f) / (PLAY_RADIUS + VTX_RADIUS)) - 1.0f);
				useful::NormalizeRot(fff);

				// 頂点の高さを変更
				pField->SetMeshVertexPosition(nCntVtx, D3DXVECTOR3(posVtx.x, posVtx.y + (-VTX_UP * sinf(D3DXToRadian(fff))), posVtx.z));
#endif
			}
		}

		pField->NormalizeNormal();
	}
}

//============================================================
//	生成処理
//============================================================
CEdit *CEdit::Create(void)
{
	// ポインタを宣言
	CEdit *pEdit = NULL;	// エディット生成用

	if (UNUSED(pEdit))
	{ // 使用されていない場合

		// メモリを確保
		pEdit = new CEdit;	// エディット
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pEdit))
	{ // 確保に成功している場合

		// エディットの初期化
		if (FAILED(pEdit->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pEdit;
			pEdit = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pEdit;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CEdit::Release(CEdit *&prEdit)
{
	if (USED(prEdit))
	{ // 使用中の場合

		// エディットの終了
		prEdit->Uninit();

		// メモリ開放
		delete prEdit;
		prEdit = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}
