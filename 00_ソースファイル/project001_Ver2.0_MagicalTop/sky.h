//============================================================
//
//	空ヘッダー [sky.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SKY_H_
#define _SKY_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "objectMeshDome.h"

//************************************************************
//	クラス定義
//************************************************************
// 空クラス
class CSky : public CObjectMeshDome
{
public:
	// テクスチャ列挙
	typedef enum
	{
		TEXTURE_NORMAL = 0,	// 通常テクスチャ
		TEXTURE_MAX			// この列挙型の総数
	}TEXTURE;

	// コンストラクタ
	CSky();

	// デストラクタ
	~CSky();

	// オーバーライド関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	// 静的メンバ関数
	static CSky *Create	// 生成
	( // 引数
		const TEXTURE texture,		// 種類
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const D3DXCOLOR& rCol,		// 色
		const POSGRID2& rPart,		// 分割数
		const float fRadius,		// 半径
		const D3DCULL cull = D3DCULL_CCW,	// カリング状況
		const bool bLight = true			// ライティング状況
	);

private:
	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数
};

#endif	// _SKY_H_
