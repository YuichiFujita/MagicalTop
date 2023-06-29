//============================================================
//
//	オブジェクト3Dヘッダー [object3D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	クラス定義
//************************************************************
// オブジェクト3Dクラス
class CObject3D : public CObject
{
public:
	// コンストラクタ
	CObject3D();
	CObject3D(const CObject::LABEL label, const int nPriority = DEFAULT_PRIO);

	// デストラクタ
	~CObject3D();

	// オーバーライド関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	// 静的メンバ関数
	static CObject3D *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rSize,	// 大きさ
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// 向き
		const D3DXCOLOR& rCol = XCOL_WHITE		// 色
	);

	// メンバ関数
	void BindTexture(const int nTextureID);		// テクスチャ割当
	void SetPosition(const D3DXVECTOR3& rPos);	// 位置設定
	void SetRotation(const D3DXVECTOR3& rRot);	// 向き設定
	void SetScaling(const D3DXVECTOR3& rSize);	// 大きさ設定
	void SetColor(const D3DXCOLOR& rCol);		// 色設定
	void SetVertexPosition(const int nID, const D3DXVECTOR3& rPos);	// 頂点位置設定

	D3DXVECTOR3 GetPosition(void) const;	// 位置取得
	D3DXVECTOR3 GetRotation(void) const;	// 向き取得
	D3DXVECTOR3 GetScaling(void) const;		// 大きさ取得
	D3DXCOLOR GetColor(void) const;			// 色取得
	D3DXVECTOR3 GetVertexPosition(const int nID);		// 頂点位置取得
	float GetPositionHeight(const D3DXVECTOR3&rPos);	// ポリゴンの着地取得

protected:
	// メンバ関数
	void SetVtx(void);	// 頂点情報の設定

private:
	// メンバ関数
	void NormalizeNormal(void);	// 法線の正規化

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	D3DXMATRIX  m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3	m_pos;	// 位置
	D3DXVECTOR3	m_rot;	// 向き
	D3DXVECTOR3	m_size;	// 大きさ
	D3DXCOLOR	m_col;	// 色
	int m_nTextureID;	// テクスチャインデックス
};

#endif	// _OBJECT3D_H_
