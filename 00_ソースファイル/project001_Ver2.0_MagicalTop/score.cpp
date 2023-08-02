//============================================================
//
//	スコア処理 [score.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "score.h"
#include "manager.h"
#include "renderer.h"
#include "value.h"

//************************************************************
//	マクロ定義
//************************************************************
#define SCO_POS		(D3DXVECTOR3(824.0f, 42.0f, 0.0f))		// ポリゴン位置
#define SCO_SIZE	(D3DXVECTOR3(60.0f, 80.0f, 0.0f))		// ポリゴン大きさ
#define SCO_SPACE	(D3DXVECTOR3(SCO_SIZE.x, 0.0f, 0.0f))	// ポリゴン間の空白

#define SCO_NUMMIN	(0)			// 最少スコア
#define SCO_NUMMAX	(99999999)	// 最大スコア

//************************************************************
//	子クラス [CScore] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CScore::CScore() : CObject(CObject::LABEL_NUMBER, DEFAULT_PRIO)
{
	// メンバ変数をクリア
	memset(&m_apValue[0], 0, sizeof(m_apValue));	// 数値の情報
	m_nNum = 0;	// スコア
}

//============================================================
//	デストラクタ
//============================================================
CScore::~CScore()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CScore::Init(void)
{
	// メンバ変数を初期化
	memset(&m_apValue[0], 0, sizeof(m_apValue));	// 数値の情報
	m_nNum = 0;	// スコア

	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{ // スコアの桁数分繰り返す

		// 数字の生成
		m_apValue[nCntScore] = CValue::Create(CValue::TEXTURE_NORMAL, SCO_POS + (SCO_SPACE * (float)nCntScore), SCO_SIZE);
		if (UNUSED(m_apValue[nCntScore]))
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CScore::Uninit(void)
{
	// 数字オブジェクトを破棄
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{ // スコアの桁数分繰り返す

		// 数字の終了
		m_apValue[nCntScore]->Uninit();
	}

	// 自身のオブジェクトを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CScore::Update(void)
{
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{ // スコアの桁数分繰り返す

		// 数字の更新
		m_apValue[nCntScore]->Update();
	}
}

//============================================================
//	描画処理
//============================================================
void CScore::Draw(void)
{

}

//============================================================
//	生成処理
//============================================================
CScore *CScore::Create(void)
{
	// ポインタを宣言
	CScore *pScore = NULL;		// スコア生成用

	if (UNUSED(pScore))
	{ // 使用されていない場合

		// メモリ確保
		pScore = new CScore;	// スコア
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pScore))
	{ // 確保に成功している場合

		// スコアの初期化
		if (FAILED(pScore->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pScore;
			pScore = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pScore;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	加算処理
//============================================================
void CScore::Add(const int nNum)
{
	// スコアを引数分加算
	m_nNum += nNum;

	// スコアの補正
	useful::LimitNum(m_nNum, SCO_NUMMIN, SCO_NUMMAX);

	// 数字のテクスチャ座標の設定
	SetTexNum();
}

//============================================================
//	設定処理
//============================================================
void CScore::Set(const int nNum)
{
	// スコアを引数の値に設定
	m_nNum = nNum;

	// スコアの補正
	useful::LimitNum(m_nNum, SCO_NUMMIN, SCO_NUMMAX);

	// 数字のテクスチャ座標の設定
	SetTexNum();
}

//============================================================
//	取得処理
//============================================================
int CScore::Get(void)
{
	// スコアの値を返す
	return m_nNum;
}

//============================================================
//	数字のテクスチャ座標の設定処理
//============================================================
void CScore::SetTexNum(void)
{
	// 変数を宣言
	int aNumDivide[MAX_SCORE];	// 桁数ごとの分解用

	// スコアを桁数ごとに分解
	useful::DivideDigitNum
	( // 引数
		&aNumDivide[0],	// 分解結果の格納配列
		m_nNum,			// 分解する数値
		MAX_SCORE		// 分解する数字の桁数
	);

	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{ // スコアの桁数分繰り返す

		// 数字の設定
		m_apValue[nCntScore]->SetNumber(aNumDivide[nCntScore]);
	}
}
