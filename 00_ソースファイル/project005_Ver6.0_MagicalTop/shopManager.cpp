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
#include "player.h"
#include "stage.h"

//************************************************************
//	マクロ定義
//************************************************************
#define SHOP_PRIO	(6)	// ショップ表示の優先順位

#define POS_BG	(D3DXVECTOR3(SCREEN_CENT.x, 330.0f, 0.0f))	// 背景の位置
#define SIZE_BG	(D3DXVECTOR3(1088.0f, 562.0f, 0.0f))		// 背景の大きさ
#define COL_BG	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.55f))		// 背景の色

#define POS_PLAYER_ICON	(D3DXVECTOR3(160.0f, 110.0f, 0.0f))	// プレイヤーアイコンの位置
#define POS_EXP_ICON	(D3DXVECTOR3(785.0f, 110.0f, 0.0f))	// 経験値アイコンの位置
#define SIZE_ICON		(D3DXVECTOR3(100.0f, 100.0f, 0.0f))	// アイコンの大きさ

#define DIGIT_LIFE	(3)	// 体力の桁数
#define POS_LIFE	(D3DXVECTOR3(270.0f, 120.0f, 0.0f))	// 体力のタイトルと数字の位置
#define SPACE_LIFE	(D3DXVECTOR3(100.0f, -2.5f, 0.0f))	// 体力のタイトルと数字の行間

#define DIGIT_LEVEL	(2)	// レベルの桁数
#define POS_LEVEL	(D3DXVECTOR3(920.0f, 120.0f, 0.0f))	// レベルのタイトルと数字の位置
#define SPACE_LEVEL	(D3DXVECTOR3(130.0f, -2.5f, 0.0f))	// レベルのタイトルと数字の行間

#define VALUEUI_SPACE_VALUE	(D3DXVECTOR3(70.0f, 0.0f, 0.0f))	// 数字UIの数字の行間
#define VALUEUI_SIZE_TITLE	(D3DXVECTOR3(252.0f, 80.0f, 0.0f))	// 数字UIのタイトルの大きさ
#define VALUEUI_SIZE_VALUE	(D3DXVECTOR3(80.0f, 80.0f, 0.0f))	// 数字UIの数字の大きさ

#define POS_MAXLIFE				(D3DXVECTOR3(565.0f, 126.0f, 0.0f))	// 最大体力のタイトルと数字の位置
#define SPACE_MAXLIFE			(D3DXVECTOR3(45.0f, 0.0f, 0.0f))	// 最大体力のタイトルと数字の行間
#define MAX_LIFE_SPACE_VALUE	(D3DXVECTOR3(45.0f, 0.0f, 0.0f))	// 最大体力の数字の行間
#define MAX_LIFE_SIZE_TITLE		(D3DXVECTOR3(33.0f, 55.0f, 0.0f))	// 最大体力のタイトルの大きさ
#define MAX_LIFE_SIZE_VALUE		(D3DXVECTOR3(55.0f, 55.0f, 0.0f))	// 最大体力の数字の大きさ

#define POS_SHOP	(D3DXVECTOR3(285.0f, 240.0f, 0.0f))	// ショップの位置
#define SPACE_SHOP	(D3DXVECTOR3(350.0f, 0.0f, 0.0f))	// ショップの空白
#define CHOICE_COL	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	// 選択中カラー
#define DEFAULT_COL	(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f))	// 非選択中カラー

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CShopManager::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\icon000.png",	// プレイヤーアイコンテクスチャ
	"data\\TEXTURE\\icon001.png",	// 経験値アイコンテクスチャ
	"data\\TEXTURE\\shop000.png",	// 体力テクスチャ
	"data\\TEXTURE\\shop001.png",	// 最大体力区切りテクスチャ
	"data\\TEXTURE\\shop002.png",	// レベルテクスチャ
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
	m_pIconPlayer	= NULL;	// プレイヤーアイコン情報
	m_pIconExp		= NULL;	// 経験値アイコン情報
	m_pLife			= NULL;	// プレイヤー体力情報
	m_pMaxLife		= NULL;	// プレイヤー最大体力情報
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
	m_pIconPlayer	= NULL;	// プレイヤーアイコン情報
	m_pIconExp		= NULL;	// 経験値アイコン情報
	m_pLife			= NULL;	// プレイヤー体力情報
	m_pMaxLife		= NULL;	// プレイヤー最大体力情報
	m_pLv			= NULL;	// プレイヤーレベル情報
	m_nSelect		= 0;	// 現在の選択番号
	m_nOldSelect	= 0;	// 過去の選択番号

	//--------------------------------------------------------
	//	背景の生成・設定
	//--------------------------------------------------------
	// 背景情報の生成
	m_pBg = CObject2D::Create
	( // 引数
		POS_BG,		// 位置
		SIZE_BG,	// 大きさ
		VEC3_ZERO,	// 向き
		COL_BG		// 色
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
	//	プレイヤーアイコンの生成・設定
	//--------------------------------------------------------
	// プレイヤーアイコン情報の生成
	m_pIconPlayer = CObject2D::Create
	( // 引数
		POS_PLAYER_ICON,	// 位置
		SIZE_ICON			// 大きさ
	);
	if (UNUSED(m_pIconPlayer))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを割当・設定
	m_pIconPlayer->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_PLAYER]));

	// 優先順位を設定
	m_pIconPlayer->SetPriority(SHOP_PRIO);

	// 描画をしない状態にする
	m_pIconPlayer->SetEnableDraw(false);

	//--------------------------------------------------------
	//	経験値アイコンの生成・設定
	//--------------------------------------------------------
	// 経験値アイコン情報の生成
	m_pIconExp = CObject2D::Create
	( // 引数
		POS_EXP_ICON,	// 位置
		SIZE_ICON		// 大きさ
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
	//	プレイヤー体力の生成・設定
	//--------------------------------------------------------
	// プレイヤーの体力情報の生成
	m_pLife = CValueUI::Create
	( // 引数
		mc_apTextureFile[TEXTURE_LIFE],	// タイトルテクスチャパス
		CValue::TEXTURE_ANGULAR,		// 数字テクスチャ
		DIGIT_LIFE,				// 桁数
		POS_LIFE,				// 位置
		SPACE_LIFE,				// 行間
		VALUEUI_SPACE_VALUE,	// 数字行間
		VALUEUI_SIZE_TITLE,		// タイトル大きさ
		VALUEUI_SIZE_VALUE		// 数字大きさ
	);
	if (UNUSED(m_pLife))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pLife->SetPriority(SHOP_PRIO);

	// 描画をしない状態にする
	m_pLife->SetEnableDraw(false);

	//--------------------------------------------------------
	//	プレイヤー最大体力の生成・設定
	//--------------------------------------------------------
	// プレイヤーの最大体力情報の生成
	m_pMaxLife = CValueUI::Create
	( // 引数
		mc_apTextureFile[TEXTURE_SLASH],	// タイトルテクスチャパス
		CValue::TEXTURE_ANGULAR,			// 数字テクスチャ
		DIGIT_LIFE,				// 桁数
		POS_MAXLIFE,			// 位置
		SPACE_MAXLIFE,			// 行間
		MAX_LIFE_SPACE_VALUE,	// 数字行間
		MAX_LIFE_SIZE_TITLE,	// タイトル大きさ
		MAX_LIFE_SIZE_VALUE		// 数字大きさ
	);
	if (UNUSED(m_pMaxLife))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pMaxLife->SetPriority(SHOP_PRIO);

	// 描画をしない状態にする
	m_pMaxLife->SetEnableDraw(false);

	//--------------------------------------------------------
	//	プレイヤーレベルの生成・設定
	//--------------------------------------------------------
	// プレイヤーのレベル情報の生成
	m_pLv = CValueUI::Create
	( // 引数
		mc_apTextureFile[TEXTURE_LV],	// タイトルテクスチャパス
		CValue::TEXTURE_ANGULAR,		// 数字テクスチャ
		DIGIT_LEVEL,			// 桁数
		POS_LEVEL,				// 位置
		SPACE_LEVEL,			// 行間
		VALUEUI_SPACE_VALUE,	// 数字行間
		VALUEUI_SIZE_TITLE,		// タイトル大きさ
		VALUEUI_SIZE_VALUE		// 数字大きさ
	);
	if (UNUSED(m_pLv))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pLv->SetPriority(SHOP_PRIO);

	// 描画をしない状態にする
	m_pLv->SetEnableDraw(false);

	//--------------------------------------------------------
	//	ショップ情報の生成・設定
	//--------------------------------------------------------
	for (int nCntShop = 0; nCntShop < SELECT_MAX; nCntShop++)
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
	m_pIconPlayer->Uninit();	// プレイヤーアイコン情報
	m_pIconExp->Uninit();		// 経験値アイコン情報

	// 数字UI情報を破棄
	m_pLife->Uninit();		// プレイヤー体力情報
	m_pMaxLife->Uninit();	// プレイヤー最大体力情報
	m_pLv->Uninit();		// プレイヤーレベル情報

	for (int nCntShop = 0; nCntShop < SELECT_MAX; nCntShop++)
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
	m_pLife->GetMultiValue()->SetNum(CScene::GetPlayer()->GetLife());

	// 数値に最大体力を設定
	m_pMaxLife->GetMultiValue()->SetNum(CScene::GetPlayer()->GetMaxLife());

	// 数値にレベルを設定
	m_pLv->GetMultiValue()->SetNum(CScene::GetPlayer()->GetLevel());

	// 購入品選択の更新
	UpdateSelect();

	// オブジェクト2D情報の更新
	m_pBg->Update();			// 背景情報
	m_pIconPlayer->Update();	// プレイヤーアイコン情報
	m_pIconExp->Update();		// 経験値アイコン情報

	// 数字UI情報の更新
	m_pLife->Update();		// プレイヤー体力情報
	m_pMaxLife->Update();	// プレイヤー最大体力情報
	m_pLv->Update();		// プレイヤーレベル情報
}

//============================================================
//	開店状況の設定処理
//============================================================
void CShopManager::SetEnableOpen(const bool bOpen)
{
	// ショップの品ぞろえを全変更
	AllRandomShop();

	// ショップを表示
	SetEnableDraw(bOpen);

	if (bOpen)
	{ // 開店する場合

		// 数値に体力を設定
		m_pLife->GetMultiValue()->SetNum(CScene::GetPlayer()->GetLife());

		// 数値に最大体力を設定
		m_pMaxLife->GetMultiValue()->SetNum(CScene::GetPlayer()->GetMaxLife());

		// 数値にレベルを設定
		m_pLv->GetMultiValue()->SetNum(CScene::GetPlayer()->GetLevel());
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

//============================================================
//	描画状況の設定処理
//============================================================
void CShopManager::SetEnableDraw(const bool bDraw)
{
	// 引数の描画状況を設定
	m_pBg->SetEnableDraw(bDraw);			// 背景情報
	m_pIconPlayer->SetEnableDraw(bDraw);	// プレイヤーアイコン情報
	m_pIconExp->SetEnableDraw(bDraw);		// 経験値アイコン情報
	m_pLife->SetEnableDraw(bDraw);			// プレイヤー体力情報
	m_pMaxLife->SetEnableDraw(bDraw);		// プレイヤー最大体力情報
	m_pLv->SetEnableDraw(bDraw);			// プレイヤーレベル情報

	for (int nCntShop = 0; nCntShop < SELECT_MAX; nCntShop++)
	{ // ショップの品目数分繰り返す

		// 引数の描画状況を設定
		m_apShop[nCntShop]->SetEnableDraw(bDraw);	// ショップ情報
	}
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

	//--------------------------------------------------------
	//	ランダムから省く値を設定・シャッフル数を設定
	//--------------------------------------------------------
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

	//--------------------------------------------------------
	//	シャッフルする値がない場合 (売り切れ)
	//--------------------------------------------------------
	if (nNumShuffle <= 0)
	{ // 購入できるものが存在しない場合

		// 購入品を変更
		m_apShop[nShop]->SetBuy((CShop::BUY)NONE_IDX);

		// 売り切れを返す
		return NONE_IDX;
	}

	//--------------------------------------------------------
	//	シャッフル用配列メモリ確保
	//--------------------------------------------------------
	if (UNUSED(pShuffle))
	{ // シャッフルデータが使われていない場合

		// シャッフルデータ必要数分のメモリ生成
		pShuffle = new int[nNumShuffle];

		// 例外処理
		assert(USED(pShuffle));	// 生成失敗
	}
	else { assert(false); }	// 使用中

	//--------------------------------------------------------
	//	シャッフル用配列に省いた値以外を順に代入
	//--------------------------------------------------------
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

	//--------------------------------------------------------
	//	配列をシャッフルし購入品を設定
	//--------------------------------------------------------
	// 要素をシャッフルする
	useful::Shuffle(pShuffle, nNumShuffle);

	// シャッフルしたデータのどの要素を使用するかを設定
	nHitID = rand() % nNumShuffle;

	// ランダムなインデックスの要素を購入品とする
	nRandomHit = pShuffle[nHitID];

	// 購入品を変更
	m_apShop[nShop]->SetBuy((CShop::BUY)nRandomHit);

	//--------------------------------------------------------
	//	シャッフル用配列メモリ開放
	//--------------------------------------------------------
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
//	ショップ全変更処理
//============================================================
void CShopManager::AllRandomShop(void)
{
	// 変数を宣言
	int nNumOmit = 0;	// シャッフルを省く数

	// 変数配列を宣言
	int aRandomHit[SELECT_MAX] = {};	// ランダムに当たった購入品
	bool aOmitBuy[CShop::BUY_MAX] = {};	// 省くかの情報

	// ポインタを宣言
	int *pShuffle = NULL;	// シャッフルを省くデータの保持用

	//--------------------------------------------------------
	//	省く購入品を設定
	//--------------------------------------------------------

	// TODO：ここに省く購入品を設定する処理

#if 0
	aOmitBuy[0] = true;
	nNumOmit++;
#endif

	//--------------------------------------------------------
	//	ショップ数分変更する
	//--------------------------------------------------------
	for (int nCntShop = 0; nCntShop < SELECT_MAX; nCntShop++)
	{ // ショップの品目数分繰り返す

		// 変数を宣言
		int nOmitID = 0;	// 現在のシャッフルを省く配列の要素

		//----------------------------------------------------
		//	シャッフルから省く値の数分メモリ確保
		//----------------------------------------------------
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

		//----------------------------------------------------
		//	ショップ内容の変更・購入品を出さないように設定
		//----------------------------------------------------
		// ショップの単変更
		aRandomHit[nCntShop] = RandomShop(nCntShop, pShuffle, nNumOmit + nCntShop);
		if (aRandomHit[nCntShop] > NONE_IDX)
		{ // 売り切れではなかった場合

			// 切り替わった購入品を次から省くように設定
			aOmitBuy[aRandomHit[nCntShop]] = true;
		}

		//----------------------------------------------------
		//	シャッフルから省く値のメモリ開放
		//----------------------------------------------------
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
