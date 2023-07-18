//============================================================
//
//	影ヘッダー [shadow.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SHADOW_H_
#define _SHADOW_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object3D.h"

//************************************************************
//	クラス定義
//************************************************************
// 影クラス
class CShadow : public CObject3D
{
public:
	// テクスチャ列挙
	typedef enum
	{
		TEXTURE_NORMAL = 0,	// 通常テクスチャ
		TEXTURE_MAX			// この列挙型の総数
	}TEXTURE;

	// コンストラクタ
	CShadow(const D3DXVECTOR3& rSize);

	// デストラクタ
	~CShadow();

	// オーバーライド関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	// 静的メンバ関数
	static CShadow *Create	// 生成
	( // 引数
		const TEXTURE texture,		// 種類
		const D3DXVECTOR3& rSize,	// 大きさ
		CObject *pObject			// 親オブジェクト
	);

	// メンバ関数
	HRESULT SetDrawInfo(void);	// 描画情報設定

private:
	// メンバ関数
	void SetParentObject(CObject *pObject);	// 親オブジェクト設定

	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数

	// メンバ変数
	CObject *m_pParentObject;		// 親オブジェクト
	const D3DXVECTOR3 m_sizeOrigin;	// 大きさ定数
};

#endif	// _SHADOW_H_
