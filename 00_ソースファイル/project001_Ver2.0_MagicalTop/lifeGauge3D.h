//============================================================
//
//	体力ゲージ3Dヘッダー [lifeGauge3D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _LIFEGAUGE3D_H_
#define _LIFEGAUGE3D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	前方宣言
//************************************************************
class CObjectBillboard;	// オブジェクトビルボードクラス

//************************************************************
//	クラス定義
//************************************************************
// 体力ゲージ3Dクラス
class CLifeGauge3D : public CObject
{
public:
	// テクスチャ列挙
	typedef enum
	{
		TEXTURE_BG = 0,		// テクスチャなし
		TEXTURE_GAUGE,		// テクスチャなし
		TEXTURE_FRAME,		// 体力ゲージフレーム
		TEXTURE_MAX			// この列挙型の総数
	}TEXTURE;

	// 体力変動列挙
	typedef enum
	{
		STATE_NONE = 0,	// 通常状態
		STATE_CHANGE,	// 体力変動状態
		STATE_MAX		// この列挙型の総数
	}STATE;

	// コンストラクタ
	CLifeGauge3D(const int nMax, const int nFrame);

	// デストラクタ
	~CLifeGauge3D();

	// オーバーライド関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	// 静的メンバ関数
	static CLifeGauge3D *Create		// 生成
	( // 引数
		const int nLife,	// 体力
		const int nMax,		// 最大体力
		const int nFrame,	// 体力変動フレーム
		const float fPosUp,	// 体力表示のY位置加算量
		CObject *pObject	// 体力ゲージ表示オブジェクト
	);

	// メンバ関数
	void AddLife(const int nAdd);				// 体力加算
	void SetLifeObject(CObject *pObject);		// 体力表示設定
	void SetLife(const int nLife);				// 体力設定
	void SetPositionUp(const float fUp);		// Y位置加算量設定
	const CObject *GetLifeObject(void) const;	// 体力表示取得
	int GetLife(void) const;					// 体力取得
	float GetPositionUp(void) const;			// Y位置加算量取得

private:
	// メンバ変数
	CObjectBillboard *m_apBilboard[TEXTURE_MAX];	// ビルボードの情報
	CObject *m_pLife;		// 体力ゲージ3D表示オブジェクトの情報
	STATE m_state;			// 状態
	float m_fChange;		// 体力変動量
	float m_fCurrentLife;	// 現在体力
	float m_fPosUp;			// 体力表示のY位置加算量
	int m_nCounterState;	// 状態管理カウンター
	int m_nLife;			// 体力
	const int m_nMaxLife;	// 最大体力定数
	const int m_nFrame;		// 体力変動フレーム定数

	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数
};

#endif	// _LIFEGAUGE3D_H_
