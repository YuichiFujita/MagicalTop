//============================================================
//
//	ブロック処理 [block.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "block.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	マクロ定義
//************************************************************
#define BLOCK_SETUP_TXT	"data\\TXT\\block.txt"	// セットアップテキスト相対パス
#define BLOCK_PRIO	(1)	// ブロックの優先順位

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CObjectMeshCube::MeshCube CBlock::m_blockInfo = {};			// ブロックステータス情報
CBlock::Type CBlock::m_aTypeInfo[CBlock::TYPE_MAX] = {};	// テクスチャ種類情報

//************************************************************
//	子クラス [CBlock] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CBlock::CBlock() : CObjectMeshCube(CObject::LABEL_BLOCK, BLOCK_PRIO)
{

}

//============================================================
//	デストラクタ
//============================================================
CBlock::~CBlock()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CBlock::Init(void)
{
	// オブジェクトメッシュキューブの初期化
	if (FAILED(CObjectMeshCube::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CBlock::Uninit(void)
{
	// オブジェクトメッシュキューブの終了
	CObjectMeshCube::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CBlock::Update(void)
{
	// オブジェクトメッシュキューブの更新
	CObjectMeshCube::Update();
}

//============================================================
//	描画処理
//============================================================
void CBlock::Draw(void)
{
	// オブジェクトメッシュキューブの描画
	CObjectMeshCube::Draw();
}

//============================================================
//	ブロックのステータス取得処理
//============================================================
CBlock::MeshCube CBlock::GetBlockInfo(void)
{
	// ブロックのステータス情報を返す
	return m_blockInfo;
}

//============================================================
//	生成処理
//============================================================
CBlock *CBlock::Create
(
	const TYPE type,		// 種類
	const D3DXVECTOR3& rPos	// 位置
)
{
	// ポインタを宣言
	CBlock *pBlock = NULL;	// ブロック生成用

	if (UNUSED(pBlock))
	{ // 使用されていない場合

		// メモリ確保
		pBlock = new CBlock;	// ブロック
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pBlock))
	{ // 確保に成功している場合

		if (SUCCEEDED(pBlock->GetResult()))
		{ // オブジェクトの情報設定に成功した場合

			// ブロックの初期化
			if (FAILED(pBlock->Init()))
			{ // 初期化に失敗した場合

				// メモリ開放
				delete pBlock;
				pBlock = NULL;

				// 失敗を返す
				return NULL;
			}

			// テクスチャを割当
			pBlock->BindTexture(m_aTypeInfo[type].texture);

			// 位置を設定
			pBlock->SetPosition(rPos);

			// 向きを設定
			pBlock->SetRotation(m_blockInfo.rot);

			// 大きさを設定
			pBlock->SetScaling(m_blockInfo.size);

			// キューブ色を設定
			pBlock->SetCubeColor(m_blockInfo.aCol[CUBECOL_CUBE]);

			// 縁取り色を設定
			pBlock->SetBorderColor(m_blockInfo.aCol[CUBECOL_BORDER]);

			// 縁取りの状態を設定
			if (FAILED(pBlock->SetBorderState(m_blockInfo.bordState)))
			{ // 分割数の設定に失敗した場合

				// メモリ開放
				delete pBlock;
				pBlock = NULL;

				// 失敗を返す
				return NULL;
			}

			// 縁取りの太さを設定
			pBlock->SetBorderThick(m_blockInfo.fBordThick);

			// テクスチャの状態を設定
			pBlock->SetTextureState(m_aTypeInfo[type].state);

			// テクスチャの分割数Xを設定
			pBlock->SetTexturePatternX(m_blockInfo.aTexPart[CUBEPART_X]);

			// テクスチャの分割数Yを設定
			pBlock->SetTexturePatternY(m_blockInfo.aTexPart[CUBEPART_Y]);

			// テクスチャの分割数Zを設定
			pBlock->SetTexturePatternZ(m_blockInfo.aTexPart[CUBEPART_Z]);

			// カリングを設定
			pBlock->SetCulling(m_blockInfo.cull);

			// ライティングを設定
			pBlock->SetLighting(m_blockInfo.bLight);

			// 確保したアドレスを返す
			return pBlock;
		}
		else
		{ // オブジェクトの情報設定に失敗した場合

			// メモリ開放
			delete pBlock;
			pBlock = NULL;

			// 失敗を返す
			return NULL;
		}
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	セットアップ処理
//============================================================
void CBlock::LoadSetup(void)
{
	// 変数を宣言
	FACETEX texType;			// 種類の読み込み代入用
	int nCurrentType = 0;		// 現在の種類の読み込み数
	int nNumTexture = 0;		// テクスチャの読み込み数の代入用
	int nCurrentTexture = 0;	// 現在のテクスチャ読み込み数
	int nLight = 0;				// ライティングのON/OFFの変換用
	int nEnd = 0;				// テキスト読み込み終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;	// ファイルポインタ
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ
	int *pTextureID = NULL;		// テクスチャインデックスポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(BLOCK_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			// テクスチャの読込
			if (strcmp(&aString[0], "TEXTURELOAD") == 0)
			{ // 読み込んだ文字列が TEXTURELOAD の場合

				do
				{ // 読み込んだ文字列が END_TEXTURELOAD ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "NUM_TEXTURE") == 0)
					{ // 読み込んだ文字列が NUM_TEXTURE の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nNumTexture);	// テクスチャファイル数を読み込む

						if (UNUSED(pTextureID))
						{ // ポインタが使用されていない場合

							// テクスチャファイル名保持数分メモリ確保
							pTextureID = new int[nNumTexture];

							// 例外処理
							assert(USED(pTextureID));	// 非使用中
						}
						else { assert(false); }	// 使用中
					}
					else if (strcmp(&aString[0], "TEXTURE_FILENAME") == 0)
					{ // 読み込んだ文字列が TEXTURE_FILENAME の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%s", &aString[0]);	// テクスチャファイル名を読み込む

						// 例外処理
						assert(nCurrentTexture < nNumTexture);	// テクスチャファイルオーバー

						// テクスチャを登録
						pTextureID[nCurrentTexture] = pTexture->Regist(&aString[0]);

						// テクスチャ読み込み数を加算
						nCurrentTexture++;
					}
				} while (strcmp(&aString[0], "END_TEXTURELOAD") != 0);	// 読み込んだ文字列が END_TEXTURELOAD ではない場合ループ
			}

			// 基本情報の設定
			else if (strcmp(&aString[0], "BASICSET") == 0)
			{ // 読み込んだ文字列が BASICSET の場合

				do
				{ // 読み込んだ文字列が END_BASICSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					// 基本情報の設定
					if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &m_blockInfo.rot.x);	// X向きを読み込む
						fscanf(pFile, "%f", &m_blockInfo.rot.y);	// Y向きを読み込む
						fscanf(pFile, "%f", &m_blockInfo.rot.z);	// Z向きを読み込む
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // 読み込んだ文字列が SIZE の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &m_blockInfo.size.x);	// X大きさを読み込む
						fscanf(pFile, "%f", &m_blockInfo.size.y);	// Y大きさを読み込む
						fscanf(pFile, "%f", &m_blockInfo.size.z);	// Z大きさを読み込む
					}

					// 縁取り情報の設定
					else if (strcmp(&aString[0], "BORDER") == 0)
					{ // 読み込んだ文字列が BORDER の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%d", &m_blockInfo.bordState);	// 縁取りの使用状況を読み込む
					}
					else if (strcmp(&aString[0], "THICK") == 0)
					{ // 読み込んだ文字列が THICK の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f", &m_blockInfo.fBordThick);	// 縁取りの太さを読み込む
					}

					// 表示情報の設定
					else if (strcmp(&aString[0], "CULL") == 0)
					{ // 読み込んだ文字列が CULL の場合

						fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
						fscanf(pFile, "%d", &m_blockInfo.cull);	// カリング状況を読み込む
					}
					else if (strcmp(&aString[0], "LIGHT") == 0)
					{ // 読み込んだ文字列が LIGHT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nLight);		// ライティングのON/OFFを読み込む

						// 読み込んだ値をbool型に変換
						m_blockInfo.bLight = (nLight == 0) ? true : false;
					}

					// 色の設定
					if (strcmp(&aString[0], "COLORSET") == 0)
					{ // 読み込んだ文字列が COLORSET の場合

						do
						{ // 読み込んだ文字列が END_COLORSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "CUBE") == 0)
							{ // 読み込んだ文字列が CUBE の場合

								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%f", &m_blockInfo.aCol[CUBECOL_CUBE].r);	// キューブ色を読み込む
								fscanf(pFile, "%f", &m_blockInfo.aCol[CUBECOL_CUBE].g);	// キューブ色を読み込む
								fscanf(pFile, "%f", &m_blockInfo.aCol[CUBECOL_CUBE].b);	// キューブ色を読み込む
								fscanf(pFile, "%f", &m_blockInfo.aCol[CUBECOL_CUBE].a);	// キューブ透明度を読み込む
							}
							else if (strcmp(&aString[0], "BORDER") == 0)
							{ // 読み込んだ文字列が BORDER の場合

								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%f", &m_blockInfo.aCol[CUBECOL_BORDER].r);	// 縁取り色を読み込む
								fscanf(pFile, "%f", &m_blockInfo.aCol[CUBECOL_BORDER].g);	// 縁取り色を読み込む
								fscanf(pFile, "%f", &m_blockInfo.aCol[CUBECOL_BORDER].b);	// 縁取り色を読み込む
								fscanf(pFile, "%f", &m_blockInfo.aCol[CUBECOL_BORDER].a);	// 縁取り透明度を読み込む
							}
						} while (strcmp(&aString[0], "END_COLORSET") != 0);	// 読み込んだ文字列が END_COLORSET ではない場合ループ
					}

					// テクスチャ分割数の設定
					if (strcmp(&aString[0], "TEXPARTSET") == 0)
					{ // 読み込んだ文字列が TEXPARTSET の場合

						do
						{ // 読み込んだ文字列が END_TEXPARTSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "TEXPART_X") == 0)
							{ // 読み込んだ文字列が TEXPART_X の場合

								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%f", &m_blockInfo.aTexPart[CUBEPART_X].x);	// 左右の横分割数を読み込む
								fscanf(pFile, "%f", &m_blockInfo.aTexPart[CUBEPART_X].y);	// 左右の縦分割数を読み込む
							}
							else if (strcmp(&aString[0], "TEXPART_Y") == 0)
							{ // 読み込んだ文字列が TEXPART_Y の場合

								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%f", &m_blockInfo.aTexPart[CUBEPART_Y].x);	// 上下の横分割数を読み込む
								fscanf(pFile, "%f", &m_blockInfo.aTexPart[CUBEPART_Y].y);	// 上下の縦分割数を読み込む
							}
							else if (strcmp(&aString[0], "TEXPART_Z") == 0)
							{ // 読み込んだ文字列が TEXPART_Z の場合

								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%f", &m_blockInfo.aTexPart[CUBEPART_Z].x);	// 前後の横分割数を読み込む
								fscanf(pFile, "%f", &m_blockInfo.aTexPart[CUBEPART_Z].y);	// 前後の縦分割数を読み込む
							}
						} while (strcmp(&aString[0], "END_TEXPARTSET") != 0);	// 読み込んだ文字列が END_TEXPARTSET ではない場合ループ
					}
				} while (strcmp(&aString[0], "END_BASICSET") != 0);	// 読み込んだ文字列が END_BASICSET ではない場合ループ
			}

			// テクスチャ情報の設定
			else if (strcmp(&aString[0], "TEXTURESET") == 0)
			{ // 読み込んだ文字列が TEXTURESET の場合

				do
				{ // 読み込んだ文字列が END_TEXTURESET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "BLOCKSET") == 0)
					{ // 読み込んだ文字列が BLOCKSET の場合

						do
						{ // 読み込んだ文字列が END_BLOCKSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							// テクスチャの種類の設定
							if (strcmp(&aString[0], "TEXALL") == 0)
							{ // 読み込んだ文字列が TEXALL の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &texType.All);	// 全面のテクスチャを読み込む

								// 例外処理
								assert(nCurrentType < TYPE_MAX);	// テクスチャ種類オーバー

								// テクスチャを登録
								m_aTypeInfo[nCurrentType].texture = FACETEX(pTextureID[texType.All]);

								// テクスチャ状態を設定
								m_aTypeInfo[nCurrentType].state = TEXSTATE_ONE;	// 同一のテクスチャを使用

								// 種類読み込み数を加算
								nCurrentType++;
							}
							else if (strcmp(&aString[0], "TEXSET") == 0)
							{ // 読み込んだ文字列が TEXSET の場合

								do
								{ // 読み込んだ文字列が END_TEXSET ではない場合ループ

									// ファイルから文字列を読み込む
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "TEXLEFT") == 0)
									{ // 読み込んだ文字列が TEXLEFT の場合

										fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
										fscanf(pFile, "%d", &texType.Left);		// 左面のテクスチャを読み込む
									}
									else if (strcmp(&aString[0], "TEXRIGHT") == 0)
									{ // 読み込んだ文字列が TEXRIGHT の場合

										fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
										fscanf(pFile, "%d", &texType.Right);	// 右面のテクスチャを読み込む
									}
									else if (strcmp(&aString[0], "TEXBOTTOM") == 0)
									{ // 読み込んだ文字列が TEXBOTTOM の場合

										fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
										fscanf(pFile, "%d", &texType.Bottom);	// 下面のテクスチャを読み込む
									}
									else if (strcmp(&aString[0], "TEXTOP") == 0)
									{ // 読み込んだ文字列が TEXTOP の場合

										fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
										fscanf(pFile, "%d", &texType.Top);		// 上面のテクスチャを読み込む
									}
									else if (strcmp(&aString[0], "TEXNEAR") == 0)
									{ // 読み込んだ文字列が TEXNEAR の場合

										fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
										fscanf(pFile, "%d", &texType.Near);		// 前面のテクスチャを読み込む
									}
									else if (strcmp(&aString[0], "TEXFAR") == 0)
									{ // 読み込んだ文字列が TEXFAR の場合

										fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
										fscanf(pFile, "%d", &texType.Far);		// 後面のテクスチャを読み込む
									}
								} while (strcmp(&aString[0], "END_TEXSET") != 0);	// 読み込んだ文字列が END_TEXSET ではない場合ループ

								// 例外処理
								assert(nCurrentType < TYPE_MAX);	// テクスチャ種類オーバー

								// テクスチャを登録
								m_aTypeInfo[nCurrentType].texture = FACETEX
								( // 引数
									pTextureID[texType.Left],	// 左
									pTextureID[texType.Right],	// 右
									pTextureID[texType.Bottom],	// 下
									pTextureID[texType.Top],	// 上
									pTextureID[texType.Near],	// 前
									pTextureID[texType.Far]		// 後
								);

								// テクスチャ状態を設定
								m_aTypeInfo[nCurrentType].state = TEXSTATE_SELECT;	// それぞれのテクスチャを使用

								// 種類読み込み数を加算
								nCurrentType++;
							}
						} while (strcmp(&aString[0], "END_BLOCKSET") != 0);	// 読み込んだ文字列が END_BLOCKSET ではない場合ループ
					}
				} while (strcmp(&aString[0], "END_TEXTURESET") != 0);		// 読み込んだ文字列が END_TEXTURESET ではない場合ループ
			}
		} while (nEnd != EOF);	// 読み込んだ文字列が EOF ではない場合ループ
		
		if (USED(pTextureID))
		{ // ポインタが使用中の場合

			// メモリ開放
			delete[] pTextureID;
		}
		else { assert(false); }	// 非使用中

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "ブロックセットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}
