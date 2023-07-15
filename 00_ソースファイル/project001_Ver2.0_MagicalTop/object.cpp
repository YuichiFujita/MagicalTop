//============================================================
//
//	オブジェクト処理 [object.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"
#include "manager.h"
#include "debugproc.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CObject *CObject::m_apTop[MAX_PRIO] = {};	// 先頭のオブジェクトへのポインタ
CObject *CObject::m_apCur[MAX_PRIO] = {};	// 最後尾のオブジェクトへのポインタ
int CObject::m_nNumAll = 0;					// オブジェクトの総数

//************************************************************
//	親クラス [CObject] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObject::CObject()
{
	if (USED(m_apCur[DEFAULT_PRIO]))
	{ // 最後尾が存在する場合

		// 現在の最後尾オブジェクトの次オブジェクトを自身に設定
		m_apCur[DEFAULT_PRIO]->m_pNext = this;

		// 前オブジェクトを設定
		m_pPrev = m_apCur[DEFAULT_PRIO];	// 現在の最後尾オブジェクト

		// 次オブジェクトをクリア
		m_pNext = NULL;

		// 自身の情報アドレスを最後尾に設定
		m_apCur[DEFAULT_PRIO] = this;
	}
	else
	{ // 最後尾が存在しない場合

		// 自身の情報アドレスを先頭に設定
		m_apTop[DEFAULT_PRIO] = this;

		// 自身の情報アドレスを最後尾に設定
		m_apCur[DEFAULT_PRIO] = this;

		// 前オブジェクトのクリア
		m_pPrev = NULL;

		// 次オブジェクトのクリア
		m_pNext = NULL;
	}

	// 自身の情報をクリア
	m_label		= LABEL_NONE;	// オブジェクトラベル
	m_nPriority	= DEFAULT_PRIO;	// 優先順位

	// オブジェクトの総数を加算
	m_nNumAll++;
}

//============================================================
//	オーバーロードコンストラクタ
//============================================================
CObject::CObject(const LABEL label, const int nPriority)
{
	if (USED(m_apCur[nPriority]))
	{ // 最後尾が存在する場合

		// 現在の最後尾オブジェクトの次オブジェクトを自身に設定
		m_apCur[nPriority]->m_pNext = this;

		// 前オブジェクトを設定
		m_pPrev = m_apCur[nPriority];	// 現在の最後尾オブジェクト

		// 次オブジェクトをクリア
		m_pNext = NULL;

		// 自身の情報アドレスを最後尾に設定
		m_apCur[nPriority] = this;
	}
	else
	{ // 最後尾が存在しない場合

		// 自身の情報アドレスを先頭に設定
		m_apTop[nPriority] = this;

		// 自身の情報アドレスを最後尾に設定
		m_apCur[nPriority] = this;

		// 前オブジェクトのクリア
		m_pPrev = NULL;

		// 次オブジェクトのクリア
		m_pNext = NULL;
	}

	// 自身の情報を設定
	m_label		= label;		// オブジェクトラベル
	m_nPriority	= nPriority;	// 優先順位

	// オブジェクトの総数を加算
	m_nNumAll++;
}

//============================================================
//	デストラクタ
//============================================================
CObject::~CObject()
{
	// オブジェクトの総数を減算
	m_nNumAll--;
}

//============================================================
//	ヒット処理
//============================================================
void CObject::Hit(const int nDmg)
{
	// 例外処理
	assert(false);
}

//============================================================
//	位置の設定処理
//============================================================
void CObject::SetPosition(const D3DXVECTOR3& rPos)
{
	// 例外処理
	assert(false);
}

//============================================================
//	向きの設定処理
//============================================================
void CObject::SetRotation(const D3DXVECTOR3& rRot)
{
	// 例外処理
	assert(false);
}

//============================================================
//	マトリックス取得処理
//============================================================
D3DXMATRIX CObject::GetMtxWorld(void) const
{
	// 変数を宣言
	D3DXMATRIX mtxNone;	// 返り値用

	// マトリックスの初期化
	D3DXMatrixIdentity(&mtxNone);

	// 例外処理
	assert(false);

	// 単位マトリックスを返す
	return mtxNone;
}

//============================================================
//	位置取得処理
//============================================================
D3DXVECTOR3 CObject::GetPosition(void) const
{
	// 例外処理
	assert(false);

	// 初期値を返す
	return VEC3_ZERO;
}

//============================================================
//	向き取得処理
//============================================================
D3DXVECTOR3 CObject::GetRotation(void) const
{
	// 例外処理
	assert(false);

	// 初期値を返す
	return VEC3_ZERO;
}

//============================================================
//	大きさ取得処理
//============================================================
D3DXVECTOR3 CObject::GetScaling(void) const
{
	// 例外処理
	assert(false);

	// 初期値を返す
	return VEC3_ZERO;
}

//============================================================
//	半径取得処理
//============================================================
float CObject::GetRadius(void) const
{
	// 例外処理
	assert(false);

	// 初期値を返す
	return 0.0f;
}

//============================================================
//	全破棄処理
//============================================================
void CObject::ReleaseAll(void)
{
	// ポインタを宣言
	CObject *pObject;	// オブジェクト代入用

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // 優先順位の総数分繰り返す

		if (USED(m_apTop[nCntPri]))
		{ // 先頭が存在する場合

			// オブジェクトの先頭を代入
			pObject = m_apTop[nCntPri];

			while (USED(pObject))
			{ // オブジェクトが使用されている場合繰り返す

				// ポインタを宣言
				CObject *pObjectNext = pObject->m_pNext;	// 次のオブジェクトへのポインタ

				// TODO：オブジェクトの他者管理これで本当に大丈夫？
				if (USED(pObjectNext))
				{ // 次のオブジェクトが存在する場合

					while (pObjectNext->m_label == LABEL_NONE)
					{ // 次のオブジェクトのラベルが設定されていない場合繰り返す

						// さらに次のオブジェクトへのポインタを指定
						pObjectNext = pObjectNext->m_pNext;

						if (UNUSED(pObjectNext))
						{ // さらに次のオブジェクトが存在しない場合

							// 処理を抜ける
							break;
						}
					}
				}

				if (pObject->m_label != LABEL_NONE)
				{ // オブジェクトラベルが設定されている場合

					// オブジェクトの終了
					pObject->Uninit();
				}

				// 次のオブジェクトへのポインタを代入
				pObject = pObjectNext;
			}
		}
	}

	// 例外処理
	if (m_nNumAll != 0) { assert(false); }	// 破棄の失敗
}

//============================================================
//	全更新処理
//============================================================
void CObject::UpdateAll(void)
{
	// ポインタを宣言
	CObject *pObject;	// オブジェクト代入用

	// デバッグ表示
	CManager::GetDebugProc()->Print("オブジェクト数：%d\n", m_nNumAll);

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // 優先順位の総数分繰り返す

		if (USED(m_apTop[nCntPri]))
		{ // 先頭が存在する場合

			// オブジェクトの先頭を代入
			pObject = m_apTop[nCntPri];

			while (USED(pObject))
			{ // オブジェクトが使用されている場合繰り返す

				// ポインタを宣言
				CObject *pObjectNext = pObject->m_pNext;	// 次のオブジェクトへのポインタ

				// TODO：オブジェクトの他者管理これで本当に大丈夫？
				if (USED(pObjectNext))
				{ // 次のオブジェクトが存在する場合

					while (pObjectNext->m_label == LABEL_NONE)
					{ // 次のオブジェクトのラベルが設定されていない場合繰り返す

						// さらに次のオブジェクトへのポインタを指定
						pObjectNext = pObjectNext->m_pNext;

						if (UNUSED(pObjectNext))
						{ // さらに次のオブジェクトが存在しない場合

							// 処理を抜ける
							break;
						}
					}
				}

				if (pObject->m_label != LABEL_NONE)
				{ // オブジェクトラベルが設定されている場合

					// オブジェクトの更新
					pObject->Update();
				}

				// 次のオブジェクトへのポインタを代入
				pObject = pObjectNext;
			}
		}
	}
}

//============================================================
//	全描画処理
//============================================================
void CObject::DrawAll(void)
{
	// ポインタを宣言
	CObject *pObject;	// オブジェクト代入用

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // 優先順位の総数分繰り返す

		if (USED(m_apTop[nCntPri]))
		{ // 先頭が存在する場合

			// オブジェクトの先頭を代入
			pObject = m_apTop[nCntPri];

			while (USED(pObject))
			{ // オブジェクトが使用されている場合繰り返す

				// ポインタを宣言
				CObject *pObjectNext = pObject->m_pNext;	// 次のオブジェクトへのポインタ

				// TODO：オブジェクトの他者管理これで本当に大丈夫？
				if (USED(pObjectNext))
				{ // 次のオブジェクトが存在する場合

					while (pObjectNext->m_label == LABEL_NONE)
					{ // 次のオブジェクトのラベルが設定されていない場合繰り返す

						// さらに次のオブジェクトへのポインタを指定
						pObjectNext = pObjectNext->m_pNext;

						if (UNUSED(pObjectNext))
						{ // さらに次のオブジェクトが存在しない場合

							// 処理を抜ける
							break;
						}
					}
				}

				if (pObject->m_label != LABEL_NONE)
				{ // オブジェクトラベルが設定されている場合

					// オブジェクトの描画
					pObject->Draw();
				}

				// 次のオブジェクトへのポインタを代入
				pObject = pObjectNext;
			}
		}
	}
}

//============================================================
//	使用確認処理
//============================================================
bool CObject::CheckUse(const CObject *pObject)
{
	// ポインタを宣言
	CObject *pObjCheck;	// オブジェクト代入用

	if (USED(pObject))
	{ // ポインタが使用中の場合

		for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
		{ // 優先順位の総数分繰り返す

			if (USED(m_apTop[nCntPri]))
			{ // 先頭が存在する場合

				// オブジェクトの先頭を代入
				pObjCheck = m_apTop[nCntPri];

				while (USED(pObjCheck))
				{ // オブジェクトが使用されている場合繰り返す

					// ポインタを宣言
					CObject *pObjectNext = pObjCheck->m_pNext;	// 次のオブジェクトへのポインタ

					if (pObjCheck == pObject)
					{ // 同アドレスの場合

						// 真を返す
						return true;	// TODO：同じアドレスが確保されているだけの可能性がある
					}

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;
				}
			}
		}

		// 偽を返す
		return false;
	}
	else
	{ // ポインタが非使用中の場合

		// 偽を返す
		return false;
	}
}

//============================================================
//	先頭オブジェクト取得処理
//============================================================
CObject *CObject::GetTop(const int nPriority)
{
	// 引数の優先順位の先頭オブジェクトを返す
	return m_apTop[nPriority];
}

//============================================================
//	最後尾オブジェクト取得処理
//============================================================
CObject *CObject::GetCur(const int nPriority)
{
	// 引数の優先順位の最後尾オブジェクトを返す
	return m_apCur[nPriority];
}

//============================================================
//	ラベルの設定処理
//============================================================
void CObject::SetLabel(const LABEL label)
{
	if (label < LABEL_MAX)
	{ // 引数のラベルが使用可能な場合

		// 引数のラベルを設定
		m_label = label;
	}
	else { assert(false); }	// 範囲外
}

//============================================================
//	ラベルの取得処理
//============================================================
CObject::LABEL CObject::GetLabel(void) const
{
	// ラベルを返す
	return m_label;
}

//============================================================
//	優先順位の取得処理
//============================================================
int CObject::GetPriority(void) const
{
	// 優先順位を返す
	return m_nPriority;
}

//============================================================
//	オブジェクト取得処理
//============================================================
CObject *CObject::GetObject(void)
{
	// 自身のポインタを返す
	return this;
}

//============================================================
//	前オブジェクト取得処理
//============================================================
CObject *CObject::GetPrev(void) const
{
	// 前オブジェクトを返す
	return m_pPrev;
}

//============================================================
//	次オブジェクト取得処理
//============================================================
CObject *CObject::GetNext(void) const
{
	// 次オブジェクトを返す
	return m_pNext;
}

//============================================================
//	破棄処理
//============================================================
void CObject::Release(void)
{
	// 前のオブジェクトをつなぎなおす
	if (USED(m_pNext))
	{ // 次のオブジェクトが存在する場合

		// 前のオブジェクトを変更
		m_pNext->m_pPrev = m_pPrev;
	}

	// 次のオブジェクトをつなぎなおす
	if (USED(m_pPrev))
	{ // 前のオブジェクトが存在する場合

		// 次のオブジェクトを変更
		m_pPrev->m_pNext = m_pNext;
	}

	// 先頭オブジェクトの変更
	if (m_apTop[m_nPriority] == this)
	{ // 先頭オブジェクトが破棄するオブジェクトだった場合

		// 次のオブジェクトを先頭に指定
		m_apTop[m_nPriority] = m_pNext;
	}

	// 最後尾オブジェクトの変更
	if (m_apCur[m_nPriority] == this)
	{ // 最後尾オブジェクトが破棄するオブジェクトだった場合

		// 前のオブジェクトを最後尾に指定
		m_apCur[m_nPriority] = m_pPrev;
	}

	if (USED(this))
	{ // 使用されている場合

		// メモリ開放
		delete this;
	}
}
