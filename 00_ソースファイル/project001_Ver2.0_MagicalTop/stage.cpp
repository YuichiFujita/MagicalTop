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
#define AREA_PRIO	(2)	// エリア表示の優先順位

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
	memset(&m_stageArea, 0, sizeof(m_stageArea));	// ステージエリア
	memset(&m_stageLimit, 0, sizeof(m_stageLimit));	// ステージ範囲
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
	memset(&m_stageArea, 0, sizeof(m_stageArea));	// ステージエリア
	memset(&m_stageLimit, 0, sizeof(m_stageLimit));	// ステージ範囲

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
	m_pStageArea->BindTexture(pTexture->Regist("data\\TEXTURE\\area000.png"));	// TODO

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

	if (USED(m_stageArea.pInfo))
	{ // ステージエリアが使用されている場合

		// ステージエリアのメモリ開放
		delete[] m_stageArea.pInfo;
		m_stageArea.pInfo = NULL;
	}
}

//============================================================
//	更新処理
//============================================================
void CStage::Update(void)
{
	// 変数を宣言
	D3DXVECTOR3 posPlayer = CSceneGame::GetPlayer()->GetPosition();	// プレイヤー位置
	D3DXVECTOR3 posTarget = CSceneGame::GetTarget()->GetPosition();	// ターゲット位置
	float fRadiusPlayer = CSceneGame::GetPlayer()->GetRadius();		// プレイヤー半径

	// エリア表示の位置を設定
	m_pStageArea->SetPosition(posTarget);

	for (int nCntArea = 0; nCntArea < m_stageArea.nNumArea; nCntArea++)
	{ // エリアの総数分繰り返す

		if (collision::Circle2D(posPlayer, posTarget, fRadiusPlayer, m_stageArea.pInfo[nCntArea].fRadius))
		{ // エリア内の場合

			// エリア表示の大きさを設定
			m_pStageArea->SetScaling(D3DXVECTOR3(m_stageArea.pInfo[nCntArea].fRadius * 2.0f, 0.0f, m_stageArea.pInfo[nCntArea].fRadius * 2.0f));

			// エリア表示の色を設定
			m_pStageArea->SetColor(m_stageArea.pInfo[nCntArea].col);

			// 処理を抜ける
			break;
		}
	}
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
void CStage::SetStageArea(const StageArea& rArea)
{
	if (UNUSED(m_stageArea.pInfo))
	{ // 非使用中の場合

		// 引数のエリアの総数分メモリ確保
		m_stageArea.pInfo = new AreaInfo[rArea.nNumArea];

		if (USED(m_stageArea.pInfo))
		{ // 確保に成功した場合

			// メモリクリア
			memset(m_stageArea.pInfo, 0, sizeof(AreaInfo) * rArea.nNumArea);
		}
		else { assert(false); }	// 確保失敗
	}
	else { assert(false); }	// 使用中

	// 引数のステージエリアの総数を設定
	m_stageArea.nNumArea = rArea.nNumArea;

	// 引数のステージエリアの情報を設定
	for (int nCntArea = 0; nCntArea < rArea.nNumArea; nCntArea++)
	{ // エリアの総数分繰り返す

		m_stageArea.pInfo[nCntArea] = rArea.pInfo[nCntArea];
	}
}

//============================================================
//	ステージエリア取得処理
//============================================================
CStage::StageArea CStage::GetStageArea(void) const
{
	// ステージエリアを返す
	return m_stageArea;
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

					if (strcmp(&aString[0], "NUM_AREA") == 0)
					{ // 読み込んだ文字列が NUM_AREA の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%d", &stageArea.nNumArea);	// エリアの総数を読み込む

						// 例外処理
						assert(stageArea.nNumArea > 0);	// エリア数最低限以下

						// エリアの総数分メモリ確保
						stageArea.pInfo = new AreaInfo[stageArea.nNumArea];

						if (USED(stageArea.pInfo))
						{ // 確保に成功した場合

							// メモリクリア
							memset(stageArea.pInfo, 0, sizeof(AreaInfo) * stageArea.nNumArea);
						}
						else { assert(false); }	// 確保失敗
					}
					else if (strcmp(&aString[0], "AREA") == 0)
					{ // 読み込んだ文字列が AREA の場合

						do
						{ // 読み込んだ文字列が END_AREA ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "COL") == 0)
							{ // 読み込んだ文字列が COL の場合

								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%f", &stageArea.pInfo[nArea].col.r);		// 赤色を読み込む
								fscanf(pFile, "%f", &stageArea.pInfo[nArea].col.g);		// 緑色を読み込む
								fscanf(pFile, "%f", &stageArea.pInfo[nArea].col.b);		// 青色を読み込む
								fscanf(pFile, "%f", &stageArea.pInfo[nArea].col.a);		// α値を読み込む
							}
							else if (strcmp(&aString[0], "RADIUS") == 0)
							{ // 読み込んだ文字列が RADIUS の場合

								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%f", &stageArea.pInfo[nArea].fRadius);	// 半径を読み込む
							}
						} while (strcmp(&aString[0], "END_AREA") != 0);	// 読み込んだ文字列が END_AREA ではない場合ループ

						// 例外処理
						assert(nArea < stageArea.nNumArea);	// エリア数オーバー

						// エリア数を加算
						nArea++;
					}
				} while (strcmp(&aString[0], "END_AREASET") != 0);	// 読み込んだ文字列が END_AREASET ではない場合ループ

				// ステージエリアの設定
				pStage->SetStageArea(stageArea);

				if (USED(stageArea.pInfo))
				{ // ステージエリアが使用されている場合

					// ステージエリアのメモリ開放
					delete[] stageArea.pInfo;
				}
				else { assert(false); }	// 非使用中
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
