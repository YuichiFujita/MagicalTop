//============================================================
//
//	ブロックヘッダー [block.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _BLOCK_H_
#define _BLOCK_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "objectMeshCube.h"

//************************************************************
//	クラス定義
//************************************************************
// ブロッククラス
class CBlock : public CObjectMeshCube
{
public:
	// テクスチャ列挙
	typedef enum
	{
		TYPE_SOIL = 0,	// 土テクスチャ
		TYPE_GRASS,		// 草テクスチャ
		TYPE_STONE,		// 石テクスチャ
		TYPE_MAX		// この列挙型の総数
	}TYPE;

	// コンストラクタ
	CBlock();

	// デストラクタ
	~CBlock();

	// 種類構造体
	typedef struct
	{
		FACETEX  texture;	// テクスチャ
		TEXSTATE state;		// テクスチャ使用状況
	}Type;

	// オーバーライド関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	// 静的メンバ関数
	static void LoadSetup(void);	// セットアップ
	static CBlock *Create	// 生成
	( // 引数
		const TYPE type,		// 種類
		const D3DXVECTOR3& rPos	// 位置
	);

private:
	// 静的メンバ変数
	static MeshCube m_blockInfo;		// ブロックステータス情報
	static Type m_aTypeInfo[TYPE_MAX];	// テクスチャ種類情報
};

#endif	// _BLOCK_H_
