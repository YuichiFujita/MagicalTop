//============================================================
//
//	ロックオン表示処理 [lockCursor.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "lockCursor.h"
#include "manager.h"
#include "renderer.h"
#include "objectBillboard.h"
#include "texture.h"

//************************************************************
//	マクロ定義
//************************************************************
#define LOCKCURSOR_PRIO	(4)	// ロックオン表示の優先順位
#define LOCKCURSOR_SIZE	(D3DXVECTOR3(120.0f, 120.0f, 0.0f))	// 大きさ

#define ADD_ROT_UP		(0.05f)		// 向き加算量(上四角)
#define ADD_ROT_DOWN	(0.025f)	// 向き加算量(下四角)

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CLockCursor::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\lockCursor000.png",	// ロックオン表示(魔法陣)
	"data\\TEXTURE\\lockCursor001.png",	// ロックオン表示(上四角)
	"data\\TEXTURE\\lockCursor001.png",	// ロックオン表示(下四角)
};

//************************************************************
//	子クラス [CLockCursor] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CLockCursor::CLockCursor() : CObject(CObject::LABEL_LOCK, LOCKCURSOR_PRIO)
{
	// メンバ変数をクリア
	memset(&m_apBilboard[0], 0, sizeof(m_apBilboard));	// ビルボードの情報
	bDraw = false;	// 描画状況
}

//============================================================
//	デストラクタ
//============================================================
CLockCursor::~CLockCursor()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CLockCursor::Init(void)
{
	// 変数を宣言
	int nTextureID;	// テクスチャインデックス

	// ポインタを宣言
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ

	// メンバ変数を初期化
	memset(&m_apBilboard[0], 0, sizeof(m_apBilboard));	// ビルボードの情報
	bDraw = false;	// 描画状況

	for (int nCntLock = 0; nCntLock < TEXTURE_MAX; nCntLock++)
	{ // テクスチャの最大数分繰り返す

		// ビルボードの生成
		m_apBilboard[nCntLock] = CObjectBillboard::Create
		( // 引数
			VEC3_ZERO,		// 位置
			LOCKCURSOR_SIZE	// 大きさ
		);
		if (UNUSED(m_apBilboard[nCntLock]))
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// テクスチャを登録
		nTextureID = pTexture->Regist(mc_apTextureFile[nCntLock]);

		// テクスチャを割当
		m_apBilboard[nCntLock]->BindTexture(nTextureID);
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CLockCursor::Uninit(void)
{
	// ビルボードを破棄
	for (int nCntLock = 0; nCntLock < TEXTURE_MAX; nCntLock++)
	{ // テクスチャの最大数分繰り返す

		// ビルボードの終了
		m_apBilboard[nCntLock]->Uninit();
	}

	// 自身のオブジェクトを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CLockCursor::Update(void)
{
	// 変数を宣言
	D3DXVECTOR3 rot;	// 向き

	// 変数配列を宣言
	const float fAddRot[] =	// 向き加算量
	{ // 初期値
		ADD_ROT_UP,		// 上四角の向き加算量
		ADD_ROT_DOWN	// 下四角の向き加算量
	};

	for (int nCntLock = 1; nCntLock < TEXTURE_MAX; nCntLock++)
	{ // 回転するビルボード数分繰り返す

		// 向きを取得
		rot = m_apBilboard[nCntLock]->GetRotation();

		// 向きを設定
		m_apBilboard[nCntLock]->SetRotation(D3DXVECTOR3(rot.x, rot.y, rot.z + fAddRot[nCntLock - 1]));
	}

	for (int nCntLock = 0; nCntLock < TEXTURE_MAX; nCntLock++)
	{ // テクスチャの最大数分繰り返す

		// ビルボードの更新
		m_apBilboard[nCntLock]->Update();
	}
}

//============================================================
//	描画処理
//============================================================
void CLockCursor::Draw(void)
{
	if (bDraw)
	{ // 描画する場合

		for (int nCntLock = 0; nCntLock < TEXTURE_MAX; nCntLock++)
		{ // テクスチャの最大数分繰り返す

			// ビルボードの描画
			m_apBilboard[nCntLock]->Draw();
		}
	}
}

//============================================================
//	生成処理
//============================================================
CLockCursor *CLockCursor::Create(void)
{
	// ポインタを宣言
	CLockCursor *pLockCursor = NULL;	// ロックオン表示生成用

	if (UNUSED(pLockCursor))
	{ // 使用されていない場合

		// メモリ確保
		pLockCursor = new CLockCursor;	// ロックオン表示
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pLockCursor))
	{ // 確保に成功している場合

		if (SUCCEEDED(pLockCursor->GetResult()))
		{ // オブジェクトの情報設定に成功した場合

			// ロックオン表示の初期化
			if (FAILED(pLockCursor->Init()))
			{ // 初期化に失敗した場合

				// 失敗を返す
				return NULL;
			}

			// 確保したアドレスを返す
			return pLockCursor;
		}
		else
		{ // オブジェクトの情報設定に失敗した場合

			// メモリ開放
			delete pLockCursor;
			pLockCursor = NULL;

			// 失敗を返す
			return NULL;
		}
	}
	else { assert(false); return NULL; }	// 確保失敗
}
