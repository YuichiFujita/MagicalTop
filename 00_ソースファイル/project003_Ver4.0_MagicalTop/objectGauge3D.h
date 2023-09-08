//============================================================
//
//	オブジェクトゲージ3Dヘッダー [objectGauge3D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OBJECTGAUGE3D_H_
#define _OBJECTGAUGE3D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	クラス定義
//************************************************************
// オブジェクトゲージ3Dクラス
class CObjectGauge3D : public CObject
{
public:
	// 状態列挙
	typedef enum
	{
		STATE_NONE = 0,	// 通常状態
		STATE_CHANGE,	// ゲージ変動状態
		STATE_MAX		// この列挙型の総数
	}STATE;

	// ポリゴン列挙
	typedef enum
	{
		POLYGON_BACK = 0,	// 背景
		POLYGON_FRONT,		// ゲージ
		POLYGON_FRAME,		// 枠
		POLYGON_MAX			// この列挙型の総数
	}POLYGON;

	// フレーム種類列挙
	typedef enum
	{
		TYPE_NONE = 0,	// フレーム無し
		TYPE_PLAYER,	// プレイヤーフレーム
		TYPE_TARGET,	// ターゲットフレーム
		TYPE_MAX		// この列挙型の総数
	}TYPE;

	// コンストラクタ
	CObjectGauge3D();
	CObjectGauge3D(const int nMax, const int nFrame, const CObject::LABEL label, const int nPriority = DEFAULT_PRIO);

	// デストラクタ
	~CObjectGauge3D();

	// オーバーライド関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	// 静的メンバ関数
	static CObjectGauge3D *Create	// 生成
	( // 引数
		const CObject::LABEL label,	// オブジェクトラベル
		CObject *pObject,			// ゲージ表示オブジェクト
		const int nMax,				// 最大表示値
		const int nFrame,			// 表示値変動フレーム
		const float fPosUp,			// 表示Y位置の加算量
		const D3DXVECTOR3& rSizeGauge = VEC3_ONE,	// ゲージ大きさ
		const D3DXCOLOR& rColFront = XCOL_WHITE,	// 表ゲージ色
		const D3DXCOLOR& rColBack = XCOL_BLACK,		// 裏ゲージ色
		const bool bDrawFrame = false,				// 枠描画状況
		const TYPE frameType = TYPE_NONE,			// 枠種類
		const D3DXVECTOR3& rSizeFrame = VEC3_ONE	// 枠大きさ
	);

	// メンバ関数
	void AddNum(const int nAdd);	// ゲージ加算
	void SetNum(const int nNum);	// ゲージ設定
	int GetNum(void) const;			// ゲージ取得
	void BindTexture(const int nPolygonID, const int nTextureID);	// テクスチャ割当
	void SetGaugeObject(CObject *pObject);			// ゲージ表示オブジェクト設定
	void SetPosition(const D3DXVECTOR3& rPos);		// 位置設定
	void SetPositionUp(const float fUp);			// Y位置加算量設定
	void SetScalingGauge(const D3DXVECTOR3& rSize);	// ゲージ大きさ設定
	void SetScalingFrame(const D3DXVECTOR3& rSize);	// 背景大きさ設定
	void SetColorFront(const D3DXCOLOR& rCol);		// 表ゲージ色設定
	void SetColorBack(const D3DXCOLOR& rCol);		// 裏ゲージ色設定
	void SetEnableDrawFrame(const bool bDraw);		// 枠表示状況設定
	const CObject *GetGaugeObject(void) const;		// ゲージ表示オブジェクト取得
	D3DXVECTOR3 GetPosition(void) const;			// 位置取得
	float GetPositionUp(void) const;				// Y位置加算量取得
	D3DXVECTOR3 GetScalingGauge(void) const;		// ゲージ大きさ取得
	D3DXVECTOR3 GetScalingFrame(void) const;		// 背景大きさ取得
	D3DXCOLOR GetColorFront(void) const;			// 表ゲージ色取得
	D3DXCOLOR GetColorBack(void) const;				// 裏ゲージ色取得
	bool GetEnableDrawFrame(void) const;			// 枠表示状況取得

private:
	// メンバ関数
	void SetVtx(void);	// 頂点情報の設定

	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	CObject		*m_pGauge;				// ゲージ3D表示オブジェクトの情報
	D3DXMATRIX	m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3	m_pos;					// 位置
	D3DXVECTOR3	m_sizeGauge;			// ゲージ大きさ
	D3DXVECTOR3	m_sizeFrame;			// 枠大きさ
	D3DXCOLOR	m_colFront;				// 表ゲージ色
	D3DXCOLOR	m_colBack;				// 裏ゲージ色
	STATE m_state;						// 状態
	bool m_bDrawFrame;					// 枠表示状況
	float m_fChange;					// ゲージ変動量
	float m_fCurrentNumGauge;			// 現在表示値
	float m_fAddRight;					// 横幅加算量
	float m_fPosUp;						// 表示Y位置の加算量
	int m_nCounterState;				// 状態管理カウンター
	int m_nNumGauge;					// 表示値
	const int m_nMaxNumGauge;			// 表示値の最大値定数
	const int m_nFrame;					// 表示値の変動フレーム定数
	int m_aTextureID[POLYGON_MAX];		// テクスチャインデックス
};

#endif	// _OBJECTGAUGE3D_H_
