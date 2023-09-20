//============================================================
//
//	ショップマネージャー処理 [shopManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "shopManager.h"
#include "manager.h"
#include "input.h"
#include "texture.h"
#include "sceneGame.h"
#include "object2D.h"
#include "valueUI.h"
#include "shop.h"
#include "multiValue.h"
#include "target.h"
#include "player.h"
#include "stage.h"

//************************************************************
//	マクロ定義
//************************************************************
#define SHOP_PRIO	(6)	// ショップ表示の優先順位

#define POS_SHOP	(D3DXVECTOR3(285.0f, 270.0f, 0.0f))	// ショップの位置
#define SPACE_SHOP	(D3DXVECTOR3(350.0f, 0.0f, 0.0f))	// ショップの空白

#define CHOICE_COL	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	// 選択中カラー
#define DEFAULT_COL	(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f))	// 非選択中カラー

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CShopManager::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\icon000.png",	// ターゲットアイコンテクスチャ
	"data\\TEXTURE\\icon001.png",	// 経験値アイコンテクスチャ
	"data\\TEXTURE\\shop000.png",	// 体力テクスチャ
	"data\\TEXTURE\\shop001.png",	// レベルテクスチャ
};

//************************************************************
//	親クラス [CShopManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CShopManager::CShopManager()
{
	// メンバ変数をクリア
	memset(&m_apShop[0], 0, sizeof(m_apShop));	// ショップ情報
	m_pBg			= NULL;	// 背景情報
	m_pIconTarget	= NULL;	// ターゲットアイコン情報
	m_pIconExp		= NULL;	// 経験値アイコン情報
	m_pLife			= NULL;	// ターゲット体力情報
	m_pLv			= NULL;	// プレイヤーレベル情報
	m_nSelect		= 0;	// 現在の選択番号
	m_nOldSelect	= 0;	// 過去の選択番号
}

//============================================================
//	デストラクタ
//============================================================
CShopManager::~CShopManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CShopManager::Init(void)
{
	// ポインタを宣言
	CTexture *pTexture = CManager::GetTexture();	// テクスチャ

	// メンバ変数を初期化
	memset(&m_apShop[0], 0, sizeof(m_apShop));	// ショップ情報
	m_pBg			= NULL;	// 背景情報
	m_pIconTarget	= NULL;	// ターゲットアイコン情報
	m_pIconExp		= NULL;	// 経験値アイコン情報
	m_pLife			= NULL;	// ターゲット体力情報
	m_pLv			= NULL;	// プレイヤーレベル情報
	m_nSelect		= 0;	// 現在の選択番号
	m_nOldSelect	= 0;	// 過去の選択番号

	//--------------------------------------------------------
	//	背景の生成・設定
	//--------------------------------------------------------
	// 背景情報の生成
	m_pBg = CObject2D::Create	// TODO：定数
	( // 引数
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.85f, SCREEN_HEIGHT * 0.78f, 0.0f),
		VEC3_ZERO,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.55f)
	);
	if (UNUSED(m_pBg))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pBg->SetPriority(SHOP_PRIO);

	// 描画をしない状態にする
	m_pBg->SetEnableDraw(false);

	//--------------------------------------------------------
	//	ターゲットアイコンの生成・設定
	//--------------------------------------------------------
	// ターゲットアイコン情報の生成
	m_pIconTarget = CObject2D::Create	// TODO：定数
	( // 引数
		D3DXVECTOR3(160.0f, 150.0f, 0.0f),
		D3DXVECTOR3(120.0f, 120.0f, 0.0f)
	);
	if (UNUSED(m_pIconTarget))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを割当・設定
	m_pIconTarget->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_TARGET]));

	// 優先順位を設定
	m_pIconTarget->SetPriority(SHOP_PRIO);

	// 描画をしない状態にする
	m_pIconTarget->SetEnableDraw(false);

	//--------------------------------------------------------
	//	経験値アイコンの生成・設定
	//--------------------------------------------------------
	// 経験値アイコン情報の生成
	m_pIconExp = CObject2D::Create	// TODO：定数
	( // 引数
		D3DXVECTOR3(750.0f, 145.0f, 0.0f),
		D3DXVECTOR3(120.0f, 120.0f, 0.0f)
	);
	if (UNUSED(m_pIconExp))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを割当・設定
	m_pIconExp->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_EXP]));

	// 優先順位を設定
	m_pIconExp->SetPriority(SHOP_PRIO);

	// 描画をしない状態にする
	m_pIconExp->SetEnableDraw(false);

	//--------------------------------------------------------
	//	ターゲット体力の生成・設定
	//--------------------------------------------------------
	// ターゲットの体力情報の生成
	m_pLife = CValueUI::Create
	( // 引数
		D3DXVECTOR3(280.0f, 160.0f, 0.0f),	// 
		D3DXVECTOR3(110.0f, -10.0f, 0.0f),	// 
		D3DXVECTOR3(280.0f, 90.0f, 0.0f),	// 
		D3DXVECTOR3(80.0f, 90.0f, 0.0f),	// 
		pTexture->Regist(mc_apTextureFile[TEXTURE_LIFE])
	);
	if (UNUSED(m_pLife))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 数字の行間を設定
	m_pLife->GetMultiValue()->SetSpace(D3DXVECTOR3(80.0f, 0.0f, 0.0f));

	// 数字の桁数を設定
	m_pLife->GetMultiValue()->SetDigit(4);

	// 優先順位を設定
	m_pLife->SetPriority(SHOP_PRIO);

	// 描画をしない状態にする
	m_pLife->SetEnableDraw(false);

	//--------------------------------------------------------
	//	プレイヤーレベルの生成・設定
	//--------------------------------------------------------
	// プレイヤーのレベル情報の生成
	m_pLv = CValueUI::Create
	( // 引数
		D3DXVECTOR3(900.0f, 160.0f, 0.0f),	// 
		D3DXVECTOR3(140.0f, -8.0f, 0.0f),	// 
		D3DXVECTOR3(280.0f, 90.0f, 0.0f),	// 
		D3DXVECTOR3(80.0f, 90.0f, 0.0f),	// 
		pTexture->Regist(mc_apTextureFile[TEXTURE_LV])
	);
	if (UNUSED(m_pLv))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 数字の行間を設定
	m_pLv->GetMultiValue()->SetSpace(D3DXVECTOR3(80.0f, 0.0f, 0.0f));

	// 数字の桁数を設定
	m_pLv->GetMultiValue()->SetDigit(2);

	// 優先順位を設定
	m_pLv->SetPriority(SHOP_PRIO);

	// 描画をしない状態にする
	m_pLv->SetEnableDraw(false);

	//--------------------------------------------------------
	//	ショップ情報の生成・設定
	//--------------------------------------------------------
	for (int nCntShop = 0; nCntShop < MAX_SHOP; nCntShop++)
	{ // ショップの品目数分繰り返す

		// ショップ情報の生成
		m_apShop[nCntShop] = CShop::Create
		( // 引数
			CShop::BUY_PLAYLIFE,					// 購入品
			POS_SHOP + (float)nCntShop * SPACE_SHOP	// 位置
		);
		if (UNUSED(m_apShop[nCntShop]))
		{ // 非使用中の場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// 優先順位を設定
		m_apShop[nCntShop]->SetPriority(SHOP_PRIO);

		// 描画をしない状態にする
		m_apShop[nCntShop]->SetEnableDraw(false);
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CShopManager::Uninit(void)
{
	// オブジェクト2D情報を破棄
	m_pBg->Uninit();			// 背景情報
	m_pIconTarget->Uninit();	// ターゲットアイコン情報
	m_pIconExp->Uninit();		// 経験値アイコン情報

	// 数字UI情報を破棄
	m_pLife->Uninit();	// ターゲット体力情報
	m_pLv->Uninit();	// プレイヤーレベル情報

	for (int nCntShop = 0; nCntShop < MAX_SHOP; nCntShop++)
	{ // ショップの品目数分繰り返す

		if (USED(m_apShop[nCntShop]))
		{ // ショップが使用されている場合

			// ショップ情報を破棄
			m_apShop[nCntShop]->Uninit();
		}
	}
}

//============================================================
//	更新処理
//============================================================
void CShopManager::Update(void)
{
	// 数値に体力を設定
	m_pLife->GetMultiValue()->SetNum(CScene::GetTarget()->GetLife());

	// 数値にレベルを設定
	m_pLv->GetMultiValue()->SetNum(CScene::GetPlayer()->GetLevel());

	// 購入品選択の更新
	UpdateSelect();

	// オブジェクト2D情報の更新
	m_pBg->Update();			// 背景情報
	m_pIconTarget->Update();	// ターゲットアイコン情報
	m_pIconExp->Update();		// 経験値アイコン情報

	// 数字UI情報の更新
	m_pLife->Update();	// ターゲット体力情報
	m_pLv->Update();	// プレイヤーレベル情報
}

//============================================================
//	ショップ全変更処理
//============================================================
void CShopManager::AllRandomShop(void)
{
	// 変数を宣言
	int nNumOmit = 0;	// シャッフルを省く数

	// 変数配列を宣言
	int aRandomHit[MAX_SHOP] = {};		// ランダムに当たった購入品
	bool aOmitBuy[CShop::BUY_MAX] = {};	// 省くかの情報

	// ポインタを宣言
	int *pShuffle = NULL;	// シャッフルを省くデータの保持用

	// TODO：ここに省く購入品を設定する処理

#if 0
	aOmitBuy[0] = true;
	nNumOmit++;
#endif

	for (int nCntShop = 0; nCntShop < MAX_SHOP; nCntShop++)
	{ // ショップの品目数分繰り返す

		// 変数を宣言
		int nOmitID = 0;	// 現在のシャッフルを省く配列の要素

		if (nNumOmit + nCntShop > 0)
		{ // 確保する場合

			if (UNUSED(pShuffle))
			{ // シャッフルデータが使われていない場合

				// シャッフルを省く数分のメモリ生成
				pShuffle = new int[nNumOmit + nCntShop];

				// 例外処理
				assert(USED(pShuffle));	// 生成失敗
			}
			else { assert(false); }	// 使用中

			for (int nCntShopMenu = 0; nCntShopMenu < CShop::BUY_MAX; nCntShopMenu++)
			{ // 購入品の最大数分繰り返す

				if (aOmitBuy[nCntShopMenu])
				{ // 省く場合

					// 現在のインデックスを省くデータに代入
					pShuffle[nOmitID] = nCntShopMenu;

					// 省くデータを入れるインデックスを加算
					nOmitID++;
				}
			}
		}

		// ショップの単変更
		aRandomHit[nCntShop] = RandomShop(nCntShop, pShuffle, nNumOmit + nCntShop);
		if (aRandomHit[nCntShop] > NONE_IDX)
		{ // 売り切れではなかった場合

			// 切り替わった購入品を次から省くように設定
			aOmitBuy[aRandomHit[nCntShop]] = true;
		}

		if (nNumOmit + nCntShop > 0)
		{ // 確保している場合

			if (USED(pShuffle))
			{ // シャッフルデータが使われている場合

				// シャッフルを省く数分のメモリ開放
				delete[] pShuffle;
				pShuffle = NULL;

				// 例外処理
				assert(UNUSED(pShuffle));	// 開放失敗
			}
			else { assert(false); }	// 非使用中
		}
	}
}

//============================================================
//	描画状況の設定処理
//============================================================
void CShopManager::SetEnableDraw(const bool bDraw)
{
	// 引数の描画状況を設定
	m_pBg->SetEnableDraw(bDraw);			// 背景情報
	m_pIconTarget->SetEnableDraw(bDraw);	// ターゲットアイコン情報
	m_pIconExp->SetEnableDraw(bDraw);		// 経験値アイコン情報
	m_pLife->SetEnableDraw(bDraw);			// ターゲット体力情報
	m_pLv->SetEnableDraw(bDraw);			// プレイヤーレベル情報

	for (int nCntShop = 0; nCntShop < MAX_SHOP; nCntShop++)
	{ // ショップの品目数分繰り返す

		// 引数の描画状況を設定
		m_apShop[nCntShop]->SetEnableDraw(bDraw);	// ショップ情報
	}
}

//============================================================
//	生成処理
//============================================================
CShopManager *CShopManager::Create(void)
{
	// ポインタを宣言
	CShopManager *pShopManager = NULL;	// ショップマネージャー生成用

	if (UNUSED(pShopManager))
	{ // 使用されていない場合

		// メモリ確保
		pShopManager = new CShopManager;	// ショップマネージャー
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pShopManager))
	{ // 使用されている場合
		
		// ショップマネージャーの初期化
		if (FAILED(pShopManager->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pShopManager;
			pShopManager = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pShopManager;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CShopManager::Release(CShopManager *&prShopManager)
{
	if (USED(prShopManager))
	{ // 使用中の場合

		// ショップマネージャーの終了
		prShopManager->Uninit();

		// メモリ開放
		delete prShopManager;
		prShopManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	ショップ単変更処理
//============================================================
int CShopManager::RandomShop
(
	const int nShop,	// 変更するショップインデックス
	int *pOmit,			// ランダムを省く購入品の先頭アドレス
	const int nNumOmit	// ランダムを省く購入品数
)
{
	// 変数を宣言
	int nNumShuffle = 0;		// シャッフルデータの必要数
	int nShuffleID = 0;			// シャッフルデータを入れるインデックス
	int nHitID = 0;				// シャッフルデータの参照インデックス
	int nRandomHit = NONE_IDX;	// 変更後の購入品

	// 変数配列を宣言
	bool aCanBuy[CShop::BUY_MAX] = {};	// 購入できるかの情報

	// ポインタを宣言
	int *pShuffle = NULL;	// データのシャッフル用

	for (int nCntShopMenu = 0; nCntShopMenu < CShop::BUY_MAX; nCntShopMenu++)
	{ // 購入品の最大数分繰り返す

		// 変数を宣言
		bool bOmit = false;	// ランダム省き状況

		if (USED(pOmit))
		{ // 省く購入品がある場合

			for (int nCntBuy = 0; nCntBuy < nNumOmit; nCntBuy++)
			{ // ランダムを省く購入品数分繰り返す

				if (pOmit[nCntBuy] == nCntShopMenu)
				{ // ランダムを省く購入品だった場合

					// ランダムを省く状態にする
					bOmit = true;

					// 処理を抜ける
					break;
				}
			}
		}

		if (bOmit)
		{ // ランダムを省く場合

			// 購入できない状態にする
			aCanBuy[nCntShopMenu] = false;
		}
		else
		{ // ランダムを省かない場合

			// 購入できる状態にする
			aCanBuy[nCntShopMenu] = true;

			// シャッフルデータ必要数を加算
			nNumShuffle++;
		}
	}

	if (nNumShuffle <= 0)
	{ // 購入できるものが存在しない場合

		// 購入品を変更
		m_apShop[nShop]->SetBuy((CShop::BUY)NONE_IDX);

		// 売り切れを返す
		return NONE_IDX;
	}

	if (UNUSED(pShuffle))
	{ // シャッフルデータが使われていない場合

		// シャッフルデータ必要数分のメモリ生成
		pShuffle = new int[nNumShuffle];

		// 例外処理
		assert(USED(pShuffle));	// 生成失敗
	}
	else { assert(false); }	// 使用中

	for (int nCntShopMenu = 0; nCntShopMenu < CShop::BUY_MAX; nCntShopMenu++)
	{ // 購入品の最大数分繰り返す

		if (aCanBuy[nCntShopMenu])
		{ // 購入できる場合

			// 現在のインデックスをシャッフルデータに代入
			pShuffle[nShuffleID] = nCntShopMenu;

			// シャッフルデータを入れるインデックスを加算
			nShuffleID++;
		}
	}

	// 要素をシャッフルする
	useful::Shuffle(pShuffle, nNumShuffle);

	// シャッフルしたデータのどの要素を使用するかを設定
	nHitID = rand() % nNumShuffle;

	// ランダムなインデックスの要素を購入品とする
	nRandomHit = pShuffle[nHitID];

	// 購入品を変更
	m_apShop[nShop]->SetBuy((CShop::BUY)nRandomHit);

	if (USED(pShuffle))
	{ // シャッフルデータが使われている場合

		// シャッフルデータ必要数分のメモリ開放
		delete[] pShuffle;
		pShuffle = NULL;

		// 例外処理
		assert(UNUSED(pShuffle));	// 開放失敗
	}
	else { assert(false); }	// 非使用中

	// 変更後の購入品を返す
	return nRandomHit;
}

//============================================================
//	購入品選択の更新処理
//============================================================
void CShopManager::UpdateSelect(void)
{
	// ポインタを宣言
	CInputKeyboard	*pKeyboard	= CManager::GetKeyboard();	// キーボード
	CInputPad		*pPad		= CManager::GetPad();		// パッド

	// 過去の選択番号を更新
	m_nOldSelect = m_nSelect;

	// 選択操作
	if (pKeyboard->GetTrigger(DIK_LEFT) || pPad->GetTrigger(CInputPad::KEY_LEFT))
	{
		// 選択中番号を減算
		m_nSelect = (m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX;
	}
	else if (pKeyboard->GetTrigger(DIK_RIGHT) || pPad->GetTrigger(CInputPad::KEY_RIGHT))
	{
		// 選択中番号を加算
		m_nSelect = (m_nSelect + 1) % SELECT_MAX;
	}

	// 選択中のカラーを設定
	m_apShop[m_nOldSelect]->SetColor(DEFAULT_COL);	// 過去の選択を暗くする
	m_apShop[m_nSelect]->SetColor(CHOICE_COL);		// 現在の選択を明るくする
}
