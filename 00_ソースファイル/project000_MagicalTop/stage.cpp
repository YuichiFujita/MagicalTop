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
#include "block.h"

//************************************************************
//	マクロ定義
//************************************************************
#define STAGE_SETUP_TXT	"data\\TXT\\stage.txt"	// セットアップテキスト相対パス

#define INIT_CENTER_POS	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))				// 方眼座標の中心座標の初期値
#define INIT_REVPOS		(D3DXVECTOR3(0.5f, 0.5f, 0.5f))				// 座標変換の補正値の初期値
#define INIT_MAXGRID	(POSGRID3(MAX_GRIDX, MAX_GRIDY, MAX_GRIDZ))	// 方眼座標の設定数の初期値

//************************************************************
//	親クラス [CStage] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CStage::CStage()
{
	// メンバ変数をクリア
	memset(&m_stageLimit, 0, sizeof(m_stageLimit));	// ステージ範囲

	// ステージ方眼座標の初期化
	m_stageGrid.centerPos	= VEC3_ZERO;	// 方眼座標の中心座標
	m_stageGrid.revPos		= VEC3_ZERO;	// 座標変換の補正値
	m_stageGrid.maxGrid		= GRID3_ZERO;	// 方眼座標数

	for (int nCntX = 0; nCntX < MAX_GRIDX; nCntX++)
	{ // 方眼座標Xの最大数分繰り返す

		for (int nCntY = 0; nCntY < MAX_GRIDY; nCntY++)
		{ // 方眼座標Yの最大数分繰り返す

			for (int nCntZ = 0; nCntZ < MAX_GRIDZ; nCntZ++)
			{ // 方眼座標Zの最大数分繰り返す

				// 方眼座標内の情報を初期化
				m_stageGrid.aX[nCntX].aY[nCntY].aZ[nCntZ].pBlock = NULL;	// ブロックのポインタ
			}
		}
	}
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
	// メンバ変数を初期化
	memset(&m_stageLimit, 0, sizeof(m_stageLimit));	// ステージ範囲

	// ステージ方眼座標の初期化
	m_stageGrid.centerPos	= INIT_CENTER_POS;	// 方眼座標の中心座標
	m_stageGrid.revPos		= INIT_REVPOS;		// 座標変換の補正値
	m_stageGrid.maxGrid		= INIT_MAXGRID;		// 方眼座標数

	for (int nCntX = 0; nCntX < MAX_GRIDX; nCntX++)
	{ // 方眼座標Xの最大数分繰り返す

		for (int nCntY = 0; nCntY < MAX_GRIDY; nCntY++)
		{ // 方眼座標Yの最大数分繰り返す

			for (int nCntZ = 0; nCntZ < MAX_GRIDZ; nCntZ++)
			{ // 方眼座標Zの最大数分繰り返す

				// 方眼座標内の情報を初期化
				m_stageGrid.aX[nCntX].aY[nCntY].aZ[nCntZ].pBlock = NULL;	// ブロックのポインタ
			}
		}
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CStage::Uninit(void)
{

}

//============================================================
//	位置の補正処理
//============================================================
void CStage::LimitPosition(D3DXVECTOR3& rPos, const float fRadius)
{
	// 位置を補正
	useful::LimitNum(rPos.x, m_stageLimit.fLeft + fRadius, m_stageLimit.fRight - fRadius);	// 左右の補正
	useful::LimitNum(rPos.z, m_stageLimit.fFar + fRadius, m_stageLimit.fNear - fRadius);	// 前後の補正
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
//	方眼座標の座標変換処理
//============================================================
D3DXVECTOR3 CStage::ChangeGridPosition(const POSGRID3& rGrid)
{
	// 変数を宣言
	D3DXVECTOR3 out;	// 位置計算用
	D3DXVECTOR3 size = CBlock::GetBlockInfo().size;	// キューブの大きさ

	// 例外処理
	assert(size.x > 0 || size.y > 0 || size.z > 0);	// 大きさ未設定

	// 方眼座標から座標を求める
	out.x = m_stageGrid.centerPos.x + (rGrid.x * size.x) - ((m_stageGrid.maxGrid.x - 1) * size.x * m_stageGrid.revPos.x);
	out.y = m_stageGrid.centerPos.y + (rGrid.y * size.y) - ((m_stageGrid.maxGrid.y - 1) * size.y * m_stageGrid.revPos.y);
	out.z = m_stageGrid.centerPos.z + (rGrid.z * size.z) - ((m_stageGrid.maxGrid.z - 1) * size.z * m_stageGrid.revPos.z);

	// 計算した座標を返す
	return out;
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
//	ステージ方眼座標の中心位置の設定処理
//============================================================
void CStage::SetStageGridCenter(const D3DXVECTOR3& rPos)
{
	// 引数の中心位置を設定
	m_stageGrid.centerPos = rPos;
}

//============================================================
//	ステージ方眼座標の位置補正量の設定処理
//============================================================
void CStage::SetStageGridRevision(const D3DXVECTOR3& rRev)
{
	// 引数の補正量を設定
	m_stageGrid.revPos = rRev;
}

//============================================================
//	ステージ方眼座標の最大方眼数の設定処理
//============================================================
void CStage::SetStageGridMax(const POSGRID3& rGrid)
{
	// 引数の最大方眼数を設定
	m_stageGrid.maxGrid = rGrid;
}

//============================================================
//	ステージ方眼座標内の設定処理
//============================================================
void CStage::SetInGrid(const POSGRID3& rGrid, CBlock *pBlock)
{
	if (rGrid.x < MAX_GRIDX
	&&  rGrid.y < MAX_GRIDY
	&&  rGrid.z < MAX_GRIDZ)
	{ // グリッドが範囲内の場合

		if (UNUSED(m_stageGrid.Out(rGrid).pBlock))
		{ // グリッド内にブロックがない場合

			// 引数の方眼座標にオブジェクトの情報を設定
			m_stageGrid.In(rGrid, pBlock);
		}
	}
	else { assert(false); }	// 範囲外
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
//	ステージ方眼座標取得処理
//============================================================
CStage::StageGrid CStage::GetStageGrid(void) const
{
	// ステージ方眼座標を返す
	return m_stageGrid;
}

//============================================================
//	ステージ方眼座標内取得処理
//============================================================
CStage::GridZ CStage::GetInGrid(const POSGRID3& rGrid)
{
	// 引数の方眼座標内の情報を返す
	return m_stageGrid.Out(rGrid);
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
	StageLimit stageLimit;				// ステージ範囲の代入用
	D3DXVECTOR3 centerPos = VEC3_ZERO;	// 方眼座標の中心位置の代入用
	D3DXVECTOR3 revPos = VEC3_ZERO;		// 座標変換の補正値の代入用
	POSGRID3 maxGrid = GRID3_ZERO;		// 方眼座標数の代入用
	POSGRID3 grid = GRID3_ZERO;			// ステージ方眼座標の代入用
	int nType = 0;						// 種類の代入用
	int nEnd = 0;						// テキスト読み込み終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;	// ファイルポインタ
	CBlock *pBlock;	// ブロックオブジェクトポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(STAGE_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			// ステージ範囲の設定
			if (strcmp(&aString[0], "LIMITSET") == 0)
			{ // 読み込んだ文字列が LIMITSET の場合

				do
				{ // 読み込んだ文字列が END_LIMITSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "NEAR") == 0)
					{ // 読み込んだ文字列が NEAR の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &stageLimit.fNear);		// 前位置を読み込む
					}
					else if (strcmp(&aString[0], "FAR") == 0)
					{ // 読み込んだ文字列が FAR の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &stageLimit.fFar);		// 後位置を読み込む
					}
					else if (strcmp(&aString[0], "RIGHT") == 0)
					{ // 読み込んだ文字列が RIGHT の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &stageLimit.fRight);	// 右位置を読み込む
					}
					else if (strcmp(&aString[0], "LEFT") == 0)
					{ // 読み込んだ文字列が LEFT の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &stageLimit.fLeft);		// 左位置を読み込む
					}
					else if (strcmp(&aString[0], "FIELD") == 0)
					{ // 読み込んだ文字列が FIELD の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &stageLimit.fField);	// 地面位置を読み込む
					}
				} while (strcmp(&aString[0], "END_LIMITSET") != 0);	// 読み込んだ文字列が END_LIMITSET ではない場合ループ

				// ステージ範囲の設定
				pStage->SetStageLimit(stageLimit);
			}

			// ブロックの配置
			if (strcmp(&aString[0], "STAGE_BLOCKSET") == 0)
			{ // 読み込んだ文字列が STAGE_BLOCKSET の場合

				do
				{ // 読み込んだ文字列が END_STAGE_BLOCKSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "CENTERPOS") == 0)
					{ // 読み込んだ文字列が CENTERPOS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &centerPos.x);	// 方眼座標の中心位置を読み込む
						fscanf(pFile, "%f", &centerPos.y);	// 方眼座標の中心位置を読み込む
						fscanf(pFile, "%f", &centerPos.z);	// 方眼座標の中心位置を読み込む

						// 方眼座標の中心位置の設定
						pStage->SetStageGridCenter(centerPos);
					}
					else if (strcmp(&aString[0], "REVPOS") == 0)
					{ // 読み込んだ文字列が REVPOS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &revPos.x);		// 座標変換の補正値を読み込む
						fscanf(pFile, "%f", &revPos.y);		// 座標変換の補正値を読み込む
						fscanf(pFile, "%f", &revPos.z);		// 座標変換の補正値を読み込む

						// 座標変換の補正値の設定
						pStage->SetStageGridRevision(revPos);
					}
					else if (strcmp(&aString[0], "MAXGRID") == 0)
					{ // 読み込んだ文字列が MAXGRID の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &maxGrid.x);	// 方眼座標数を読み込む
						fscanf(pFile, "%d", &maxGrid.y);	// 方眼座標数を読み込む
						fscanf(pFile, "%d", &maxGrid.z);	// 方眼座標数を読み込む

						// 方眼座標数の設定
						pStage->SetStageGridMax(maxGrid);
					}
					else if (strcmp(&aString[0], "BLOCKSET") == 0)
					{ // 読み込んだ文字列が BLOCKSET の場合

						do
						{ // 読み込んだ文字列が END_BLOCKSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "TYPE") == 0)
							{ // 読み込んだ文字列が TYPE の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nType);		// 種類を読み込む
							}
							else if (strcmp(&aString[0], "GRID") == 0)
							{ // 読み込んだ文字列が GRID の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &grid.x);		// 方眼座標Xを読み込む
								fscanf(pFile, "%d", &grid.y);		// 方眼座標Yを読み込む
								fscanf(pFile, "%d", &grid.z);		// 方眼座標Zを読み込む
							}
						} while (strcmp(&aString[0], "END_BLOCKSET") != 0);	// 読み込んだ文字列が END_BLOCKSET ではない場合ループ

						// ブロックオブジェクトの生成
						pBlock = CBlock::Create((CBlock::TYPE)nType, pStage->ChangeGridPosition(grid));

						// ステージの方眼座標にブロックを設定
						pStage->SetInGrid(grid, pBlock);
					}
				} while (strcmp(&aString[0], "END_STAGE_BLOCKSET") != 0);	// 読み込んだ文字列が END_STAGE_BLOCKSET ではない場合ループ
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
