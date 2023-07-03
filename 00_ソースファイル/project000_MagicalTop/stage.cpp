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

//************************************************************
//	マクロ定義
//************************************************************
#define STAGE_SETUP_TXT	"data\\TXT\\stage.txt"	// セットアップテキスト相対パス

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
	if (rPos.y < m_stageLimit.fField)
	{ // 位置が地面より下の場合

		// 位置を補正
		rPos.y = m_stageLimit.fField;

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
	StageLimit stageLimit;	// ステージ範囲の代入用
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
