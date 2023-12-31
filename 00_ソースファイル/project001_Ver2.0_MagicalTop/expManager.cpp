//============================================================
//
//	経験値マネージャー処理 [expManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "expManager.h"
#include "objectGauge2D.h"
#include "multiValue.h"

//************************************************************
//	マクロ定義
//************************************************************
#define MAX_EXP			(100)	// 最大経験値
#define EXP_FRAME		(10)	// 経験値変動フレーム
#define GAUGE_POS		(D3DXVECTOR3(260.0f, 440.0f, 0.0f))	// 位置
#define GAUGE_GAUGESIZE	(D3DXVECTOR3(200.0f, 30.0f, 0.0f))	// ゲージ大きさ
#define GAUGE_FRONTCOL	(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))	// 表ゲージ色
#define GAUGE_BACKCOL	(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	// 裏ゲージ色

#define FIRST_LV	(0)	// レベル初期値
#define LV_DIGIT	(2)	// レベル桁数
#define LV_POS		(D3DXVECTOR3(90.0f, 440.0f, 0.0f))	// 位置
#define LV_SIZE		(D3DXVECTOR3(50.0f, 60.0f, 0.0f))	// 数値大きさ
#define LV_SPACE	(D3DXVECTOR3(55.0f, 0.0f, 0.0f))	// 行間

//************************************************************
//	親クラス [CExpManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CExpManager::CExpManager()
{
	// メンバ変数をクリア
	m_pExp = NULL;		// 経験値の情報
	m_pLevel = NULL;	// レベルの情報
}

//============================================================
//	デストラクタ
//============================================================
CExpManager::~CExpManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CExpManager::Init(void)
{
	// メンバ変数を初期化
	m_pExp = NULL;		// 経験値の情報
	m_pLevel = NULL;	// レベルの情報

	// 経験値の生成
	m_pExp = CObjectGauge2D::Create
	( // 引数
		CObject::LABEL_GAUGE,	// オブジェクトラベル
		MAX_EXP,				// 最大経験値
		EXP_FRAME,				// 経験値変動フレーム
		GAUGE_POS,				// 位置
		GAUGE_GAUGESIZE,		// ゲージ大きさ
		GAUGE_FRONTCOL,			// 表ゲージ色
		GAUGE_BACKCOL			// 裏ゲージ色
	);
	if (UNUSED(m_pExp))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ゲージの設定
	m_pExp->SetNum(0);

	// レベルの生成
	m_pLevel = CMultiValue::Create
	( // 引数
		FIRST_LV,	// レベル初期値
		LV_DIGIT,	// レベル桁数
		LV_POS,		// 位置
		LV_SIZE,	// 数値大きさ
		LV_SPACE	// 行間
	);
	if (UNUSED(m_pLevel))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CExpManager::Uninit(void)
{

}

//============================================================
//	更新処理
//============================================================
void CExpManager::Update(void)
{

}

//============================================================
//	経験値の加算処理
//============================================================
void CExpManager::AddExp(const int nAdd)
{
	// 変数を宣言
	int nCurrentExp = m_pExp->GetNum();	// 現在の経験値

	if (nCurrentExp + nAdd >= MAX_EXP)
	{ // 経験値がオーバーする場合

		// レベルを加算
		m_pLevel->AddNum(1);

		// 経験値のあまりを設定
		m_pExp->SetNum((nCurrentExp + nAdd) - MAX_EXP);
	}
	else
	{ // 経験値がオーバーしない場合

		// 経験値を設定
		m_pExp->SetNum(nCurrentExp + nAdd);
	}

	// 例外処理
	assert(nAdd <= MAX_EXP + 1);	// 加算量オーバー
}

//============================================================
//	レベル取得処理
//============================================================
int CExpManager::GetLevel(void) const
{
	// レベルを返す
	return m_pLevel->GetNum();
}

//============================================================
//	生成処理
//============================================================
CExpManager *CExpManager::Create()
{
	// ポインタを宣言
	CExpManager *pExpManager = NULL;	// 経験値マネージャー生成用

	if (UNUSED(pExpManager))
	{ // 使用されていない場合

		// メモリ確保
		pExpManager = new CExpManager;	// 経験値マネージャー
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pExpManager))
	{ // 使用されている場合
		
		// 経験値マネージャーの初期化
		if (FAILED(pExpManager->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pExpManager;
			pExpManager = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pExpManager;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CExpManager::Release(CExpManager *&prExpManager)
{
	if (USED(prExpManager))
	{ // 使用中の場合

		// 経験値マネージャーの終了
		prExpManager->Uninit();

		// メモリ開放
		delete prExpManager;
		prExpManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}
