//============================================================
//
//	ターゲットヘッダー [target.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TARGET_H_
#define _TARGET_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "objectModel.h"
#include "model.h"

//************************************************************
//	前方宣言
//************************************************************
class CObjectMeshCube;	// オブジェクトメッシュキューブクラス

//************************************************************
//	クラス定義
//************************************************************
// ターゲットクラス
class CTarget : public CObjectModel
{
public:
	// 種類列挙
	typedef enum
	{
		MODEL_NORMAL = 0,	// 通常台座
		MODEL_MAX			// この列挙型の総数
	}MODEL;

	// コンストラクタ
	CTarget();

	// デストラクタ
	~CTarget();

	// オーバーライド関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	// 静的メンバ関数
	static CTarget *Create	// 生成
	( // 引数
		const MODEL model,			// モデル
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const D3DXVECTOR3& rScale = VEC3_ONE	// 大きさ
	);

	// メンバ関数
	float GetRadius(void) const;	// 半径取得

private:
	// 静的メンバ変数
	static const char *mc_apModelFile[];	// モデル定数

	// メンバ変数
	CObjectMeshCube *m_pMeshCube;	// メッシュキューブの情報
	float m_fSinRot;	// 浮遊向き
};

#endif	// _TARGET_H_
