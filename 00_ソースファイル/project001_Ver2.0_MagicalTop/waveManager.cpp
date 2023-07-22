//============================================================
//
//	ウェーブマネージャー処理 [waveManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "waveManager.h"
#include "manager.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CWaveManager::Season CWaveManager::m_aWaveInfo[CWaveManager::SEASON_MAX] = {};	// ウェーブ情報

//************************************************************
//	マクロ定義
//************************************************************
#define WAVE_SETUP_TXT	"data\\TXT\\wave.txt"	// セットアップテキスト相対パス

//************************************************************
//	親クラス [CWaveManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CWaveManager::CWaveManager()
{
	// メンバ変数をクリア
	m_state = STATE_NONE;				// 状態
	m_nSeasonCounter = SEASON_SPRING;	// 季節カウンター
	m_nWaveCounter = 0;					// ウェーブカウンター
}

//============================================================
//	デストラクタ
//============================================================
CWaveManager::~CWaveManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CWaveManager::Init(void)
{
	// メンバ変数を初期化
	m_state = STATE_NONE;				// 状態
	m_nSeasonCounter = SEASON_SPRING;	// 季節カウンター
	m_nWaveCounter = 0;					// ウェーブカウンター

	// セットアップの読み込み
	LoadSetup();

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CWaveManager::Uninit(void)
{
	for (int nCntSeason = 0; nCntSeason < SEASON_MAX; nCntSeason++)
	{ // 季節数分繰り返す

		if (USED(m_aWaveInfo[nCntSeason].pWave))
		{ // ウェーブが使用されている場合

			for (int nCntWave = 0; nCntWave < m_aWaveInfo[nCntSeason].nNumWave; nCntWave++)
			{ // ウェーブ数分繰り返す

				// 出現情報をメモリ開放
				delete[] m_aWaveInfo[nCntSeason].pWave[nCntWave].pPoint;
				m_aWaveInfo[nCntSeason].pWave[nCntWave].pPoint = NULL;
			}

			// ウェーブ情報をメモリ開放
			delete[] m_aWaveInfo[nCntSeason].pWave;
			m_aWaveInfo[nCntSeason].pWave = NULL;
		}
	}
}

//============================================================
//	更新処理
//============================================================
void CWaveManager::Update(void)
{
	// 状態の更新
	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:			// 何もしない状態

		// 無し

		break;

	case STATE_SEASON_START:	// 季節の開始状態



		break;

	case STATE_WAVE_START:		// ウェーブ開始状態



		break;

	case STATE_PROGRESSION:		// ウェーブ進行状態



#if 0
		//if ()
		{ // ウェーブをクリアした場合

			// ウェーブカウンターを加算
			m_nWaveCounter++;

			if (m_nWaveCounter < m_aWaveInfo[m_nSeasonCounter].nNumWave)
			{ // ウェーブがまだある場合

				// 状態を変更
				m_state = STATE_WAVE_START;	// ウェーブ開始状態
			}
			else
			{ // 全ウェーブが終了した場合

				// 季節カウンターを加算
				m_nSeasonCounter++;

				// 状態を変更
				m_state = STATE_SEASON_END;	// 季節の終了状態
			}
		}
#endif

		break;

	case STATE_SEASON_END:		// 季節の終了状態

		// ウェーブカウンターを初期化
		m_nWaveCounter = 0;

		break;

	case STATE_WAIT:			// 次季節の開始待機状態



		break;

	default:	// 例外処理
		assert(false);
		break;
	}
}

//============================================================
//	描画処理
//============================================================
void CWaveManager::Draw(void)
{

}

//============================================================
//	生成処理
//============================================================
CWaveManager *CWaveManager::Create()
{
	// ポインタを宣言
	CWaveManager *pWaveManager = NULL;	// ウェーブマネージャー生成用

	if (UNUSED(pWaveManager))
	{ // 使用されていない場合

		// メモリ確保
		pWaveManager = new CWaveManager;	// ウェーブマネージャー
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pWaveManager))
	{ // 使用されている場合
		
		// ウェーブマネージャーの初期化
		if (FAILED(pWaveManager->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pWaveManager;
			pWaveManager = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pWaveManager;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CWaveManager::Release(CWaveManager *&prWaveManager)
{
	if (USED(prWaveManager))
	{ // 使用中の場合

		// ウェーブマネージャーの終了
		prWaveManager->Uninit();

		// メモリ開放
		delete prWaveManager;
		prWaveManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	セットアップ処理
//============================================================
void CWaveManager::LoadSetup(void)
{
	// 変数を宣言
	int nSeason	= 0;	// 季節の設定用
	int nWave	= 0;	// ウェーブの設定用
	int nPoint	= 0;	// 出現の設定用
	int nType	= 0;	// 出現種類の代入用
	int nEnd	= 0;	// テキスト読み込み終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;	// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(WAVE_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			// ウェーブの設定
			if (strcmp(&aString[0], "SEASONSET") == 0)
			{ // 読み込んだ文字列が SEASONSET の場合

				// ウェーブ番号を初期化
				nWave = 0;

				do
				{ // 読み込んだ文字列が END_SEASONSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "NUM_WAVE") == 0)
					{ // 読み込んだ文字列が NUM_WAVE の場合

						fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
						fscanf(pFile, "%d", &m_aWaveInfo[nSeason].nNumWave);	// ウェーブ数を読み込む

						if (UNUSED(m_aWaveInfo[nSeason].pWave))
						{ // 非使用中の場合

							// ウェーブ数分メモリ確保
							m_aWaveInfo[nSeason].pWave = new Wave[m_aWaveInfo[nSeason].nNumWave];

							if (USED(m_aWaveInfo[nSeason].pWave))
							{ // 確保に成功した場合

								// メモリクリア
								memset(m_aWaveInfo[nSeason].pWave, 0, sizeof(Wave) * m_aWaveInfo[nSeason].nNumWave);
							}
							else { assert(false); }	// 確保失敗
						}
						else { assert(false); }	// 使用中
					}
					else if (strcmp(&aString[0], "WAVE") == 0)
					{ // 読み込んだ文字列が WAVE の場合

						// 出現番号を初期化
						nPoint = 0;

						do
						{ // 読み込んだ文字列が END_WAVE ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "NUM_POINT") == 0)
							{ // 読み込んだ文字列が NUM_POINT の場合

								fscanf(pFile, "%s", &aString[0]);									// = を読み込む (不要)
								fscanf(pFile, "%d", &m_aWaveInfo[nSeason].pWave[nWave].nNumPoint);	// 出現数を読み込む

								if (UNUSED(m_aWaveInfo[nSeason].pWave[nWave].pPoint))
								{ // 非使用中の場合

									// 出現数分メモリ確保
									m_aWaveInfo[nSeason].pWave[nWave].pPoint = new Point[m_aWaveInfo[nSeason].pWave[nWave].nNumPoint];

									if (USED(m_aWaveInfo[nSeason].pWave[nWave].pPoint))
									{ // 確保に成功した場合

										// メモリクリア
										memset(m_aWaveInfo[nSeason].pWave[nWave].pPoint, 0, sizeof(Point) * m_aWaveInfo[nSeason].pWave[nWave].nNumPoint);
									}
									else { assert(false); }	// 確保失敗
								}
								else { assert(false); }	// 使用中
							}
							else if (strcmp(&aString[0], "POINT") == 0)
							{ // 読み込んだ文字列が POINT の場合

								do
								{ // 読み込んだ文字列が END_POINT ではない場合ループ

									// ファイルから文字列を読み込む
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "FRAME") == 0)
									{ // 読み込んだ文字列が FRAME の場合

										fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
										fscanf(pFile, "%d", &m_aWaveInfo[nSeason].pWave[nWave].pPoint[nPoint].nFrame);	// 猶予フレームを読み込む
									}
									else if (strcmp(&aString[0], "SPAWN") == 0)
									{ // 読み込んだ文字列が SPAWN の場合

										fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
										fscanf(pFile, "%d", &nType);		// 出現種類を読み込む
										fscanf(pFile, "%d", &m_aWaveInfo[nSeason].pWave[nWave].pPoint[nPoint].aNumSpawn[nType]);	// 出現数を読み込む
									}
								} while (strcmp(&aString[0], "END_POINT") != 0);	// 読み込んだ文字列が END_POINT ではない場合ループ

								// 出現番号を加算
								nPoint++;
							}
						} while (strcmp(&aString[0], "END_WAVE") != 0);	// 読み込んだ文字列が END_WAVE ではない場合ループ

						// ウェーブ番号を加算
						nWave++;
					}
				} while (strcmp(&aString[0], "END_SEASONSET") != 0);	// 読み込んだ文字列が END_SEASONSET ではない場合ループ

				// 例外処理
				assert(nSeason < SEASON_MAX);	// 季節オーバー

				// 季節番号を加算
				nSeason++;
			}
		} while (nEnd != EOF);	// 読み込んだ文字列が EOF ではない場合ループ
		
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "ウェーブセットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}
