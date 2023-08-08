//============================================================
//
//	バブル処理 [bubble.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "bubble.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "camera.h"
#include "model.h"

//************************************************************
//	マクロ定義
//************************************************************
#define BUBBLE_PRIO	(4)	// バブル表示の優先順位

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CBubble::mc_apModelFile[] =	// モデル定数
{
	"data\\MODEL\\BUBBLE\\bubble000.x",	// バブルモデル
};

//************************************************************
//	子クラス [CBubble] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CBubble::CBubble(const int nMaxNum, const D3DXVECTOR3& rMaxScale) : CObjectModel(CObject::LABEL_NONE, BUBBLE_PRIO), m_maxScale(rMaxScale), m_nMaxNum(nMaxNum)
{
	// メンバ変数をクリア
	m_pParentObject = NULL;	// 親オブジェクト
	m_nHitNum = 0;			// バブルヒット数
}

//============================================================
//	デストラクタ
//============================================================
CBubble::~CBubble()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CBubble::Init(void)
{
	// メンバ変数を初期化
	m_pParentObject = NULL;	// 親オブジェクト
	m_nHitNum = 0;			// バブルヒット数

	// オブジェクトモデルの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

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
void CBubble::Uninit(void)
{
	// オブジェクトモデルの終了
	CObjectModel::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CBubble::Update(void)
{
	// TODO：ポインタの確認もっときれいに
	if (CObject::CheckUse(m_pParentObject))
	{ // 親オブジェクトが使用されていた場合

		// 変数を宣言
		D3DXVECTOR3 posParent = m_pParentObject->GetPosition();	// 親オブジェクト位置
		D3DXVECTOR3 posBubble = VEC3_ZERO;		// バブル位置
		D3DXVECTOR3 scaleBubble = VEC3_ZERO;	// バブル拡大率

		// バブルの位置を求める
		posBubble = posParent;	// 親オブジェクトの座標代入
		posBubble.y += m_pParentObject->GetHeight() * 0.5f;	// オブジェクトの縦幅の半分を加算

		// バブルの拡大率を求める
		scaleBubble.x = m_nHitNum * (m_maxScale.x / (float)m_nMaxNum);
		scaleBubble.y = m_nHitNum * (m_maxScale.y / (float)m_nMaxNum);
		scaleBubble.z = m_nHitNum * (m_maxScale.z / (float)m_nMaxNum);

		// 位置を設定
		SetPosition(posBubble);

		// 拡大率を設定
		SetScaling(scaleBubble);
	}

	// オブジェクトモデルの更新
	CObjectModel::Update();
}

//============================================================
//	描画処理
//============================================================
void CBubble::Draw(void)
{
	// オブジェクトモデルの描画
	CObjectModel::Draw();
}

//============================================================
//	生成処理
//============================================================
CBubble *CBubble::Create
( // 引数
	CObject *pObject,				// 親オブジェクト
	const int nMaxNum,				// 最大値
	const D3DXVECTOR3& rMaxScale,	// 最大拡大率
	const D3DXVECTOR3& rPos,		// 位置
	const D3DXVECTOR3& rRot,		// 向き
	const D3DXVECTOR3& rScale		// 拡大率
)
{
	// ポインタを宣言
	CModel *pModel = CManager::GetModel();	// モデルへのポインタ
	CBubble *pBubble = NULL;	// バブル生成用

	if (UNUSED(pBubble))
	{ // 使用されていない場合

		// メモリ確保
		pBubble = new CBubble(nMaxNum, rMaxScale);	// バブル
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pBubble))
	{ // 使用されている場合
		
		// バブルの初期化
		if (FAILED(pBubble->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pBubble;
			pBubble = NULL;

			// 失敗を返す
			return NULL;
		}

		// モデルを登録・割当
		pBubble->BindModel(pModel->GetModel(pModel->Regist(mc_apModelFile[MODEL_NORMAL])));

		// 位置を設定
		pBubble->SetPosition(rPos);

		// 向きを設定
		pBubble->SetRotation(rRot);

		// 大きさを設定
		pBubble->SetScaling(rScale);

		// 親オブジェクトを設定
		pBubble->SetParentObject(pObject);

		// 確保したアドレスを返す
		return pBubble;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	ヒット数加算処理
//============================================================
void CBubble::AddHitNum(const int nAdd)
{
	// ヒット数を引数分加算
	m_nHitNum += nAdd;

	// 範囲内制限
	useful::LimitNum(m_nHitNum, 0, m_nMaxNum);
}

//============================================================
//	ヒット数設定処理
//============================================================
void CBubble::SetHitNum(const int nNum)
{
	// 引数の値をヒット数に設定
	m_nHitNum += nNum;

	// 範囲内制限
	useful::LimitNum(m_nHitNum, 0, m_nMaxNum);
}

//============================================================
//	ヒット数取得処理
//============================================================
int CBubble::GetHitNum(void) const
{
	// ヒット数を返す
	return m_nHitNum;
}

//============================================================
//	親オブジェクトの設定処理
//============================================================
void CBubble::SetParentObject(CObject *pObject)
{
	// 引数のオブジェクトを設定
	m_pParentObject = pObject;
}
