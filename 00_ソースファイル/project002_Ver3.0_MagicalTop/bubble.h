//============================================================
//
//	バブルヘッダー [bubble.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _BUBBLE_H_
#define _BUBBLE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "objectModel.h"

//************************************************************
//	クラス定義
//************************************************************
// バブルクラス
class CBubble : public CObjectModel
{
public:
	// モデル列挙
	typedef enum
	{
		MODEL_NORMAL = 0,	// 通常バブル
		MODEL_MAX			// この列挙型の総数
	}MODEL;

	// コンストラクタ
	CBubble(const int nMaxNum, const D3DXVECTOR3& rMaxScale);

	// デストラクタ
	~CBubble();

	// オーバーライド関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	// 静的メンバ関数
	static CBubble *Create	// 生成
	( // 引数
		CObject *pObject,	// 親オブジェクト
		const int nMaxNum,	// 最大値
		const D3DXVECTOR3& rMaxScale,	// 最大拡大率
		const D3DXVECTOR3& rPos,		// 位置
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// 向き
		const D3DXVECTOR3& rScale = VEC3_ONE	// 拡大率
	);

	// メンバ関数
	void AddHitNum(const int nAdd);	// ヒット数加算
	void SetHitNum(const int nNum);	// ヒット数設定
	int  GetHitNum(void) const;		// ヒット数取得

private:
	// メンバ関数
	void SetParentObject(CObject *pObject);	// 親オブジェクト設定

	// 静的メンバ変数
	static const char *mc_apModelFile[];	// モデル定数

	// メンバ変数
	CObject *m_pParentObject;		// 親オブジェクト
	const D3DXVECTOR3 m_maxScale;	// 最大拡大率定数
	int m_nHitNum;			// バブルヒット数
	const int m_nMaxNum;	// 最大値定数
};

#endif	// _BUBBLE_H_
