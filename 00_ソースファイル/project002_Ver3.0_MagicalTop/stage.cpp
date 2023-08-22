//============================================================
//
//	ステージ処理 [stage.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "stage.h"
#include "manager.h"
#include "texture.h"
#include "sceneGame.h"
#include "player.h"
#include "target.h"
#include "object3D.h"
#include "collision.h"

//************************************************************
//	マクロ定義
//************************************************************
#define STAGE_SETUP_TXT	"data\\TXT\\stage.txt"	// セットアップテキスト相対パス
#define AREA_PRIO	(2)			// エリア表示の優先順位
#define AREA_ROT	(0.025f)	// エリアの回転量

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CStage::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\area001.png",	// エリア表示テクスチャ
};

//************************************************************
//	親クラス [CStage] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CStage::CStage()
{
	// メンバ変数をクリア
	m_pStageArea = NULL;	// ステージエリア表示の情報
	memset(&m_stageWind, 0, sizeof(m_stageWind));		// ステージ風
	memset(&m_aStageArea[0], 0, sizeof(m_aStageArea));	// ステージエリア
	memset(&m_stageLimit, 0, sizeof(m_stageLimit));		// ステージ範囲
	m_area = AREA_NONE;		// プレイヤーの現在エリア
}

//============================================================
//	デストラクタ
//============================================================
CStage::~CStage()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CStage::Init(void)
{
	// ポインタを宣言
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ

	// メンバ変数を初期化
	m_pStageArea = NULL;	// ステージエリア表示の情報
	memset(&m_stageWind, 0, sizeof(m_stageWind));		// ステージ風
	memset(&m_aStageArea[0], 0, sizeof(m_aStageArea));	// ステージエリア
	memset(&m_stageLimit, 0, sizeof(m_stageLimit));		// ステージ範囲
	m_area = AREA_NONE;		// プレイヤーの現在エリア

	// ステージエリア表示の生成
	m_pStageArea = CObject3D::Create
	( // 引数
		VEC3_ZERO,		// 位置
		VEC3_ZERO,		// 大きさ
		VEC3_ZERO,		// 向き
		XCOL_WHITE,		// 色
		false,			// ライティング状況
		D3DCMP_ALWAYS,	// Zテスト設定
		false			// Zバッファの使用状況
	);
	if (UNUSED(m_pStageArea))
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを登録・割当
	m_pStageArea->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_NORMAL]));

	// 優先順位を設定
	m_pStageArea->SetPriority(AREA_PRIO);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CStage::Uninit(void)
{
	// ステージエリア表示を破棄
	m_pStageArea->Uninit();
}

//============================================================
//	更新処理
//============================================================
void CStage::Update(void)
{
	// 変数を宣言
	D3DXVECTOR3 posPlayer = CSceneGame::GetPlayer()->GetPosition();	// プレイヤー位置
	D3DXVECTOR3 posTarget = CSceneGame::GetTarget()->GetPosition();	// ターゲット位置
	D3DXVECTOR3 rotArea = m_pStageArea->GetRotation();				// エリア表示向き
	float fRadiusPlayer = CSceneGame::GetPlayer()->GetRadius();		// プレイヤー半径

	// 現在のエリアを初期化
	m_area = AREA_NONE;

	// エリア表示の位置を設定
	m_pStageArea->SetPosition(posTarget);

	// エリア表示の向きを設定
	rotArea.y -= AREA_ROT;
	m_pStageArea->SetRotation(rotArea);

	for (int nCntArea = 0; nCntArea < AREA_MAX; nCntArea++)
	{ // エリアの最大数分繰り返す

		if (collision::Circle2D(posPlayer, posTarget, fRadiusPlayer, m_aStageArea[nCntArea].fRadius))
		{ // エリア内の場合

			// エリア表示の大きさを設定
			m_pStageArea->SetScaling(D3DXVECTOR3(m_aStageArea[nCntArea].fRadius * 2.0f, 0.0f, m_aStageArea[nCntArea].fRadius * 2.0f));

			// エリア表示の色を設定
			m_pStageArea->SetColor(m_aStageArea[nCntArea].col);

			// 現在のエリアを設定
			m_area = (AREA)nCntArea;

			// 処理を抜ける
			break;
		}
	}

	// 例外処理
	assert(m_area != AREA_NONE);	// エリア外

	// TODO：風速
#if 1
	if (m_stageWind.nCounter < 60)
	{
		// カウンターを加算
		m_stageWind.nCounter++;
	}
	else
	{

		// 変数を宣言
		float fRot = (float)(rand() % 629 - 314) * 0.01f;	// 向き
		float fScale = (float)(rand() % 8 + 1);				// 大きさ

#if 0
		int nRRRRot = rand() % 4;
		float fRot = 0.0f;

		for (int nCnt = 0; nCnt <= nRRRRot; nCnt++)
		{
			fRot += D3DX_PI * 0.5f;
		}
#endif

		// カウンターを初期化
		m_stageWind.nCounter = 0;

		// 風のベクトルを設定
		m_stageWind.vecWind.x = sinf(fRot) * fScale;
		m_stageWind.vecWind.y = 0.0f;
		m_stageWind.vecWind.z = cosf(fRot) * fScale;
	}

	CManager::GetDebugProc()->Print("%f %f %f\n", m_stageWind.vecWind.x, m_stageWind.vecWind.y, m_stageWind.vecWind.z);
#endif
}

//============================================================
//	位置の補正処理
//============================================================
void CStage::LimitPosition(D3DXVECTOR3& rPos, const float fRadius)
{
	switch (m_stageLimit.mode)
	{ // 制限モードごとの処理
	case LIMIT_BOX:		// 矩形範囲

		// 角柱の内側制限
		collision::InBoxPillar
		( // 引数
			rPos,				// 判定位置
			VEC3_ZERO,			// 判定原点位置
			VEC3_ALL(fRadius),	// 判定サイズ(右・上・後)
			VEC3_ALL(fRadius),	// 判定サイズ(左・下・前)
			D3DXVECTOR3(fabsf(m_stageLimit.fRight), 0.0f, fabsf(m_stageLimit.fFar)),	// 判定原点サイズ(右・上・後)
			D3DXVECTOR3(fabsf(m_stageLimit.fLeft),  0.0f, fabsf(m_stageLimit.fNear))	// 判定原点サイズ(左・下・前)
		);

		break;

	case LIMIT_CIRCLE:	// 円範囲

		// 円柱の内側制限
		collision::InCirclePillar
		( // 引数
			rPos,					// 判定位置
			VEC3_ZERO,				// 判定原点位置
			fRadius,				// 判定半径
			m_stageLimit.fRadius	// 判定原点半径
		);

		break;

	default:	// 例外処理
		assert(false);
		break;
	}
}

//============================================================
//	範囲外の着地処理
//============================================================
bool CStage::LandPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove, const float fHeight)
{
	if (rPos.y - fHeight < m_stageLimit.fField)
	{ // 位置が地面より下の場合

		// 位置を補正
		rPos.y = m_stageLimit.fField + fHeight;

		// 移動量を初期化
		rMove.y = 0.0f;

		// 着地している状況を返す
		return true;
	}

	// 着地していない状況を返す
	return false;
}

//============================================================
//	ステージ範囲の設定処理
//============================================================
void CStage::SetStageLimit(const StageLimit& rLimit)
{
	// 引数のステージ範囲を設定
	m_stageLimit = rLimit;
}

//============================================================
//	ステージ範囲取得処理
//============================================================
CStage::StageLimit CStage::GetStageLimit(void) const
{
	// ステージ範囲を返す
	return m_stageLimit;
}

//============================================================
//	ステージエリアの設定処理
//============================================================
void CStage::SetStageArea(const int nID, const StageArea& rArea)
{
	// 引数インデックスのステージエリアを設定
	m_aStageArea[nID] = rArea;
}

//============================================================
//	ステージエリア取得処理
//============================================================
CStage::StageArea CStage::GetStageArea(const int nID) const
{
	// 引数インデックスのステージエリアを返す
	return m_aStageArea[nID];
}

//============================================================
//	プレイヤーの現在エリア取得処理
//============================================================
CStage::AREA CStage::GetAreaPlayer(void) const
{
	// プレイヤーの現在エリアを返す
	return m_area;
}

//============================================================
//	風の方向取得処理
//============================================================
D3DXVECTOR3 CStage::GetVecWind(void) const
{
	// 風の方向を返す
	return m_stageWind.vecWind;
}

//============================================================
//	生成処理
//============================================================
CStage *CStage::Create(void)
{
	// ポインタを宣言
	CStage *pStage = NULL;		// ステージ生成用

	if (UNUSED(pStage))
	{ // 使用されていない場合

		// メモリ確保
		pStage = new CStage;	// ステージ
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pStage))
	{ // 確保に成功している場合

		// ステージの初期化
		if (FAILED(pStage->Init()))
		{ // 初期化に失敗した場合

			// 失敗を返す
			return NULL;
		}

		// セットアップの読み込み
		LoadSetup(pStage);

		// 確保したアドレスを返す
		return pStage;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CStage::Release(CStage *&prStage)
{
	if (USED(prStage))
	{ // 使用中の場合

		// ステージの終了
		prStage->Uninit();

		// メモリ開放
		delete prStage;
		prStage = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	セットアップ処理
//============================================================
void CStage::LoadSetup(CStage *pStage)
{
	// 変数を宣言
	StageArea stageArea;	// ステージエリアの代入用
	StageLimit stageLimit;	// ステージ範囲の代入用
	int nArea = 0;			// エリアの読み込み数
	int nEnd = 0;			// テキスト読み込み終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;	// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(STAGE_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			// ステージエリアの設定
			if (strcmp(&aString[0], "AREASET") == 0)
			{ // 読み込んだ文字列が AREASET の場合

				do
				{ // 読み込んだ文字列が END_AREASET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "AREA") == 0)
					{ // 読み込んだ文字列が AREA の場合

						do
						{ // 読み込んだ文字列が END_AREA ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "COL") == 0)
							{ // 読み込んだ文字列が COL の場合

								fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
								fscanf(pFile, "%f", &stageArea.col.r);		// 赤色を読み込む
								fscanf(pFile, "%f", &stageArea.col.g);		// 緑色を読み込む
								fscanf(pFile, "%f", &stageArea.col.b);		// 青色を読み込む
								fscanf(pFile, "%f", &stageArea.col.a);		// α値を読み込む
							}
							else if (strcmp(&aString[0], "RADIUS") == 0)
							{ // 読み込んだ文字列が RADIUS の場合

								fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
								fscanf(pFile, "%f", &stageArea.fRadius);	// 半径を読み込む
							}
						} while (strcmp(&aString[0], "END_AREA") != 0);	// 読み込んだ文字列が END_AREA ではない場合ループ

						// 例外処理
						assert(nArea < AREA_MAX);	// エリア数オーバー

						// ステージエリアの設定
						pStage->SetStageArea(nArea, stageArea);

						// エリア数を加算
						nArea++;
					}
				} while (strcmp(&aString[0], "END_AREASET") != 0);	// 読み込んだ文字列が END_AREASET ではない場合ループ

				// 例外処理
				assert(nArea == AREA_MAX);	// エリア未設定
			}

			// ステージ範囲の設定
			else if (strcmp(&aString[0], "LIMITSET") == 0)
			{ // 読み込んだ文字列が LIMITSET の場合

				do
				{ // 読み込んだ文字列が END_LIMITSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "NEAR") == 0)
					{ // 読み込んだ文字列が NEAR の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &stageLimit.fNear);		// 前位置を読み込む

						// 制限モードを矩形範囲に設定
						stageLimit.mode = LIMIT_BOX;
					}
					else if (strcmp(&aString[0], "FAR") == 0)
					{ // 読み込んだ文字列が FAR の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &stageLimit.fFar);		// 後位置を読み込む

						// 制限モードを矩形範囲に設定
						stageLimit.mode = LIMIT_BOX;
					}
					else if (strcmp(&aString[0], "RIGHT") == 0)
					{ // 読み込んだ文字列が RIGHT の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &stageLimit.fRight);	// 右位置を読み込む

						// 制限モードを矩形範囲に設定
						stageLimit.mode = LIMIT_BOX;
					}
					else if (strcmp(&aString[0], "LEFT") == 0)
					{ // 読み込んだ文字列が LEFT の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &stageLimit.fLeft);		// 左位置を読み込む

						// 制限モードを矩形範囲に設定
						stageLimit.mode = LIMIT_BOX;
					}
					else if (strcmp(&aString[0], "RADIUS") == 0)
					{ // 読み込んだ文字列が RADIUS の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &stageLimit.fRadius);	// 半径を読み込む

						// 制限モードを円範囲に設定
						stageLimit.mode = LIMIT_CIRCLE;
					}
					else if (strcmp(&aString[0], "FIELD") == 0)
					{ // 読み込んだ文字列が FIELD の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &stageLimit.fField);	// 地面位置を読み込む
					}
				} while (strcmp(&aString[0], "END_LIMITSET") != 0);	// 読み込んだ文字列が END_LIMITSET ではない場合ループ

				// ステージ範囲の設定
				pStage->SetStageLimit(stageLimit);

				// 例外処理
				assert(stageLimit.mode == CStage::LIMIT_CIRCLE);	// 範囲制限エラー
			}
		} while (nEnd != EOF);	// 読み込んだ文字列が EOF ではない場合ループ
		
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "ステージセットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}
