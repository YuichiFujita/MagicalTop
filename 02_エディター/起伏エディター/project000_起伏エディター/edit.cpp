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
//	マクロ定義
//************************************************************
#define INIT_BRUSH_RADIUS	(100.0f)	// 筆の太さの初期値
#define INIT_VTX_UP			(2.0f)		// 頂点上昇量の初期値

#define CHANGE_RAD	(1.0f)		// 筆の太さ変更量
#define MIN_RAD		(10.0f)		// 筆の太さの最小値
#define MAX_RAD		(1000.0f)	// 筆の太さの最大値
#define CHANGE_UP	(0.05f)		// 上昇量変更量
#define MIN_UP		(0.5f)		// 上昇量の最小値
#define MAX_UP		(50.0f)		// 上昇量の最大値

//************************************************************
//	親クラス [CEdit] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEdit::CEdit()
{
	// メンバ変数をクリア
	m_fBrushRadius = 0.0f;	// 筆の太さ
	m_fVtxUp = 0.0f;		// 頂点の上昇量
	m_bAutoNor = false;		// 法線の自動設定状況
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
	m_fBrushRadius = INIT_BRUSH_RADIUS;	// 筆の太さ
	m_fVtxUp = INIT_VTX_UP;				// 頂点の上昇量
	m_bAutoNor = false;					// 法線の自動設定状況

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
	// 操作説明表示
	DrawManual();

	// ブラシ設定
	SetBursh();

	// 上昇量設定
	SetVtxUp();

	// 頂点設定
	SetVtx();

	// 法線設定
	SetNor();
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

//============================================================
//	ブラシ設定処理
//============================================================
void CEdit::SetBursh(void)
{
	// ポインタを宣言
	CInputKeyboard *pKey = CManager::GetKeyboard();	// キーボード情報

	if (pKey->GetPress(DIK_3))
	{ // [3] が押された場合

		// 筆の太さを加算
		m_fBrushRadius += CHANGE_RAD;
	}
	if (pKey->GetPress(DIK_4))
	{ // [4] が押された場合

		// 筆の太さを減算
		m_fBrushRadius -= CHANGE_RAD;
	}

	// 筆の太さの補正
	useful::LimitNum(m_fBrushRadius, MIN_RAD, MAX_RAD);
}

//============================================================
//	上昇量設定処理
//============================================================
void CEdit::SetVtxUp(void)
{
	// ポインタを宣言
	CInputKeyboard *pKey = CManager::GetKeyboard();	// キーボード情報

	if (pKey->GetPress(DIK_5))
	{ // [5] が押された場合

		// 上昇量を加算
		m_fVtxUp += CHANGE_UP;
	}
	if (pKey->GetPress(DIK_6))
	{ // [6] が押された場合

		// 上昇量を減算
		m_fVtxUp -= CHANGE_UP;
	}

	// 上昇量の補正
	useful::LimitNum(m_fVtxUp, MIN_UP, MAX_UP);
}

//============================================================
//	頂点設定処理
//============================================================
void CEdit::SetVtx(void)
{
	// ポインタを宣言
	CInputKeyboard *pKey = CManager::GetKeyboard();	// キーボード情報
	CPlayer *pPlayer = CManager::GetPlayer();		// プレイヤー情報
	CField *pField = CManager::GetField();			// フィールド情報

	if (pKey->GetPress(DIK_0) || pKey->GetPress(DIK_9))
	{ // [0 or 9] が押された場合

		// 変数を宣言
		D3DXVECTOR3 posVtx;	// 頂点位置
		D3DXVECTOR3 posPlayer = pPlayer->GetPosition();	// プレイヤー位置
		int nNumVtx = pField->GetNumVertex();			// 頂点数
		int nUp = (pKey->GetPress(DIK_0)) ? 1 : -1;		// 上昇量の計算用

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{ // 頂点数分繰り返す

			// 頂点位置を取得
			posVtx = pField->GetMeshVertexPosition(nCntVtx);

			if (collision::Circle2D(posPlayer, posVtx, m_fBrushRadius, 0.0f))
			{ // 範囲内の場合

				// 変数を宣言
				float fLength = sqrtf(((posPlayer.x - posVtx.x) * (posPlayer.x - posVtx.x)) + ((posPlayer.z - posVtx.z) * (posPlayer.z - posVtx.z)) * 0.5f);	// 頂点との距離
				float fVtxUp = (m_fVtxUp * -((fLength / m_fBrushRadius) - 1.0f)) * nUp;	// 頂点上昇量

				// 頂点の高さを変更
				pField->SetMeshVertexPosition(nCntVtx, D3DXVECTOR3(posVtx.x, posVtx.y + fVtxUp, posVtx.z));
			}
		}
	}
}

//============================================================
//	法線設定処理
//============================================================
void CEdit::SetNor(void)
{
	// ポインタを宣言
	CInputKeyboard *pKey = CManager::GetKeyboard();	// キーボード情報
	CField *pField = CManager::GetField();			// フィールド情報

	if (pKey->GetTrigger(DIK_1))
	{ // [1] が押された場合

		// 自動正規化状況を切り替える
		m_bAutoNor = (m_bAutoNor) ? false : true;
	}

	if (m_bAutoNor)
	{ // 自動正規化がONの場合

		// 法線の設定・正規化
		pField->NormalizeNormal();
	}
	else
	{ // 自動正規化がOFFの場合

		if (pKey->GetTrigger(DIK_2))
		{ // [2] が押された場合

			// 法線の設定・正規化
			pField->NormalizeNormal();	
		}
	}
}

//============================================================
//	操作説明表示処理
//============================================================
void CEdit::DrawManual(void)
{
	// ポインタを宣言
	CDebugProc *pDebug = CManager::GetDebugProc();	// デバッグ表示情報

	// 操作説明を表示
	pDebug->Print("========================================\n");
	pDebug->Print("【保存】[F1]　←間に合いませんでした☆彡\n");
	pDebug->Print("【読込】[F2]　←間に合いませんでした☆彡\n");
	pDebug->Print("========================================\n");
	pDebug->Print("【頂点上昇】[0]\n");
	pDebug->Print("【頂点下降】[9]\n");
	pDebug->Print("【自動法線 ON/OFF】[1]\n");
	pDebug->Print("【法線の正規化】   [2]\n");
	pDebug->Print("【筆太さ増加】[3]\n");
	pDebug->Print("【筆太さ減少】[4]\n");
	pDebug->Print("【上昇量増加】[5]\n");
	pDebug->Print("【上昇量減少】[6]\n");
	pDebug->Print("========================================\n");

	// 法線の自動正規化状況を表示
	if (m_bAutoNor)
	{ // 自動正規化がONの場合
		pDebug->Print("【自動法線】ON\n");
	}
	else
	{ // 自動正規化がOFFの場合
		pDebug->Print("【自動法線】OFF\n");
	}

	// 筆の太さ表示
	pDebug->Print("【筆太さ】%f\n", m_fBrushRadius);

	// 上昇量表示
	pDebug->Print("【上昇量】%f\n", m_fVtxUp);
}

//============================================================
//	保存処理
//============================================================
void CEdit::Save(void)
{

}

//============================================================
//	読込処理
//============================================================
void CEdit::Load(void)
{

}
