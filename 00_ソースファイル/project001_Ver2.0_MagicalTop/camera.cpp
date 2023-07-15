//============================================================
//
//	カメラ処理 [camera.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"

#include "player.h"
#include "target.h"

//************************************************************
//	マクロ定義
//************************************************************
// カメラ基本情報マクロ
#define VIEW_ANGLE	(D3DXToRadian(45.0f))	// 視野角
#define VIEW_ASPECT	((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// アスペクト比
#define VIEW_NEAR	(10.0f)		// モデルが見えるZ軸の最小値
#define VIEW_FAR	(50000.0f)	// モデルが見えるZ軸の最大値

// カメラ追従マクロ
#define REV_ROT		(0.2f)		// カメラ向きの補正係数
#define POS_R_PLUS	(25.0f)		// プレイヤーの位置と注視点の位置の距離
#define POS_V_Y		(900.0f)	// 追従時の視点の y座標
#define REV_POS_V	(D3DXVECTOR2(0.225f, 0.045f))	// 視点の補正係数
#define REV_POS_R	(D3DXVECTOR2(0.25f, 0.05f))		// 注視点の補正係数

// カメラ寄り引きマクロ
#define REV_CENTER	(1.0f)		// 中間位置の補正係数 (0.0f：ターゲット方向, 1.0f：プレイヤー方向)
#define FIRST_DIS	(-800.0f)	// カメラ距離の最低値
#define POSR_Y		(350.0f)	// カメラ注視点のy座標
#define MIN_POSV_Y	(600.0f)	// カメラ視点位置の最低値
#define REV_PULS_DIS	(0.85f)	// カメラ距離加算量の補正係数
#define REV_PLUS_POSV_Y	(1.0f)	// カメラ視点y位置加算量の補正係数

// カメラ操作マクロ
#define REV_MOVE_MOUSE	(1.6f)		// マウス操作でのカメラの移動の補正係数
#define REV_DIS_MOUSE	(-0.3f)		// マウス操作でのカメラの距離の補正係数
#define REV_ROT_MOUSE	(0.008f)	// マウス操作でのカメラの回転量の補正係数

#define MIN_DIS	(-10000.0f)	// カメラの視点から注視点への距離の最小
#define MAX_DIS	(-1.0f)		// カメラの視点から注視点への距離の最大
#define LIMIT_ROT_HIGH	(D3DX_PI - 0.1f)	// x回転の制限値 (上)
#define LIMIT_ROT_LOW	(0.1f)				// x回転の制限値 (下)

//************************************************************
//	親クラス [CCamera] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CCamera::CCamera()
{
	// メンバ変数をクリア
	memset(&m_camera, 0, sizeof(m_camera));	// カメラの情報
}

//============================================================
//	デストラクタ
//============================================================
CCamera::~CCamera()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CCamera::Init(void)
{
	// カメラ情報を初期化
#if 0
	m_camera.posV		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 現在の視点
	m_camera.posR		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 現在の注視点
	m_camera.destPosV	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目標の視点
	m_camera.destPosR	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目標の注視点
	m_camera.vecU		= D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 上方向ベクトル
	m_camera.rot		= D3DXVECTOR3(1.6f, 0.0f, 0.0f);	// 現在の向き
	m_camera.destRot	= D3DXVECTOR3(1.6f, 0.0f, 0.0f);	// 目標の向き
	m_camera.fDis		= -800.0f;							// 視点と注視点の距離
#else
	m_camera.posV		= D3DXVECTOR3(0.0f, 400.0f, 0.0f);	// 現在の視点
	m_camera.posR		= D3DXVECTOR3(0.0f, 400.0f, 0.0f);	// 現在の注視点
	m_camera.destPosV	= D3DXVECTOR3(0.0f, 400.0f, 0.0f);	// 目標の視点
	m_camera.destPosR	= D3DXVECTOR3(0.0f, 400.0f, 0.0f);	// 目標の注視点
	m_camera.vecU		= D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 上方向ベクトル
	m_camera.rot		= D3DXVECTOR3(1.6f, 0.0f, 0.0f);	// 現在の向き
	m_camera.destRot	= D3DXVECTOR3(1.6f, 0.0f, 0.0f);	// 目標の向き
	m_camera.fDis		= -800.0f;							// 視点と注視点の距離
#endif

	// ビューポート情報を初期化
	m_camera.viewport.X			= 0;				// 左上隅のピクセル座標 (x)
	m_camera.viewport.Y			= 0;				// 左上隅のピクセル座標 (y)
	m_camera.viewport.Width		= SCREEN_WIDTH;		// 描画する画面の横幅
	m_camera.viewport.Height	= SCREEN_HEIGHT;	// 描画する画面の縦幅
	m_camera.viewport.MinZ		= 0.0f;
	m_camera.viewport.MaxZ		= 1.0f;

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CCamera::Uninit(void)
{

}

//============================================================
//	更新処理
//============================================================
void CCamera::Update(void)
{
#if 1
#if 0
	// カメラの更新 (追従)
	Follow();
#else
	// カメラの更新 (寄り引き)
	Bargaining();
#endif
#else
	// カメラの更新 (操作)
	Control();
#endif

	// デバッグ表示
	CManager::GetDebugProc()->Print(" 視点 ：%f %f %f\n", m_camera.posV.x, m_camera.posV.y, m_camera.posV.z);
	CManager::GetDebugProc()->Print("注視点：%f %f %f\n", m_camera.posR.x, m_camera.posR.y, m_camera.posR.z);
	CManager::GetDebugProc()->Print(" 向き ：%f %f %f\n", m_camera.rot.x, m_camera.rot.y, m_camera.rot.z);
	CManager::GetDebugProc()->Print(" 距離 ：%f\n", m_camera.fDis);
}

//============================================================
//	カメラ設定処理
//============================================================
void CCamera::SetCamera(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスのポインタ

	// ビューポートの設定
	pDevice->SetViewport(&m_camera.viewport);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_camera.mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH
	( // 引数
		&m_camera.mtxProjection,	// プロジェクションマトリックス
		VIEW_ANGLE,		// 視野角
		VIEW_ASPECT,	// 画面のアスペクト比
		VIEW_NEAR,		// Z軸の最小値
		VIEW_FAR		// Z軸の最大値
	);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_camera.mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_camera.mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH
	( // 引数
		&m_camera.mtxView,	// ビューマトリックス
		&m_camera.posV,		// 視点
		&m_camera.posR,		// 注視点
		&m_camera.vecU		// 上方向ベクトル
	);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_camera.mtxView);
}

//============================================================
//	カメラの目標位置の設定処理
//============================================================
void CCamera::SetDestCamera(void)
{
#if 0
	// 変数を宣言
	D3DXVECTOR3 pos = CManager::GetPlayer()->GetPosition();		// プレイヤー位置
	D3DXVECTOR3 rot = CManager::GetPlayer()->GetRotation();		// プレイヤー向き

	// 目標の注視点の位置を更新
	m_camera.destPosR.x = m_camera.posR.x = pos.x + sinf(rot.y + D3DX_PI) * POS_R_PLUS;	// プレイヤーの位置より少し前
	m_camera.destPosR.y = m_camera.posR.y = pos.y;										// プレイヤーの位置と同じ
	m_camera.destPosR.z = m_camera.posR.z = pos.z + cosf(rot.y + D3DX_PI) * POS_R_PLUS;	// プレイヤーの位置より少し前

	// 目標の視点の位置を更新
	m_camera.destPosV.x = m_camera.posV.x = m_camera.posR.x + ((m_camera.fDis * sinf(m_camera.rot.x)) * sinf(m_camera.rot.y));	// 目標注視点から距離分離れた位置
	m_camera.destPosV.y = m_camera.posV.y = POS_V_Y;																			// 固定の高さ
	m_camera.destPosV.z = m_camera.posV.z = m_camera.posR.z + ((m_camera.fDis * sinf(m_camera.rot.x)) * cosf(m_camera.rot.y));	// 目標注視点から距離分離れた位置

	// TODO：最初にカメラ動かないように直す
#else
	if (USED(CManager::GetPlayer()) && USED(CManager::GetTarget()))
	{ // プレイヤー・ターゲットが使用されている場合

		// 変数を宣言
		D3DXVECTOR3 posPlayer = CManager::GetPlayer()->GetPosition();	// プレイヤー位置
		D3DXVECTOR3 posTarget = CManager::GetTarget()->GetPosition();	// ターゲット位置
		D3DXVECTOR3 destPosR = VEC3_ZERO;	// カメラの注視点の目標位置
		D3DXVECTOR3 diffPosR = VEC3_ZERO;	// カメラの注視点の位置差分
		D3DXVECTOR3 diffPosV = VEC3_ZERO;	// カメラの視点の位置差分
		float fDisTarget = sqrtf((posPlayer.x - posTarget.x) * (posPlayer.x - posTarget.x) + (posPlayer.z - posTarget.z) * (posPlayer.z - posTarget.z)) * 0.5f;	// ターゲットとプレイヤー間の距離
		float fDiffRot = 0.0f;	// カメラの向き差分

		// 向きを求める
		m_camera.rot.y = m_camera.destRot.y - m_camera.rot.y;
		useful::NormalizeRot(m_camera.rot.y);	// 向き正規化

		// ターゲットとプレイヤーの中間位置を求める
		destPosR = (posPlayer - posTarget) * REV_CENTER;	// 中間位置を設定
		destPosR.y = 0.0f;	// y座標を初期化

		// カメラの向きをターゲットに向かせる
		m_camera.rot.y = atan2f(posTarget.x - posPlayer.x, posTarget.z - posPlayer.z);

		// カメラの距離を設定
		m_camera.fDis = FIRST_DIS - (fDisTarget * REV_PULS_DIS);

		// 目標の注視点の位置を更新
		m_camera.posV.x = destPosR.x;
		m_camera.posV.y = POSR_Y;
		m_camera.posV.z = destPosR.z;

		// 目標の視点の位置を更新
		m_camera.posR.x = m_camera.destPosR.x + ((m_camera.fDis * sinf(m_camera.rot.x)) * sinf(m_camera.rot.y));
		m_camera.posR.y = MIN_POSV_Y + (fDisTarget * REV_PLUS_POSV_Y);
		m_camera.posR.z = m_camera.destPosR.z + ((m_camera.fDis * sinf(m_camera.rot.x)) * cosf(m_camera.rot.y));
	}
#endif
}

//============================================================
//	向きの設定処理
//============================================================
void CCamera::SetRotation(const D3DXVECTOR3& rRot)
{
	// 引数のカメラの向きを設定
	m_camera.rot = rRot;

	// 向きを正規化
	useful::NormalizeRot(m_camera.rot.x);
	useful::NormalizeRot(m_camera.rot.y);
	useful::NormalizeRot(m_camera.rot.z);
}

//============================================================
//	目標向きの設定処理
//============================================================
void CCamera::SetDestRotation(const D3DXVECTOR3& rRot)
{
	// 引数のカメラの目標向きを設定
	m_camera.destRot = rRot;

	// 向きを正規化
	useful::NormalizeRot(m_camera.destRot.x);
	useful::NormalizeRot(m_camera.destRot.y);
	useful::NormalizeRot(m_camera.destRot.z);
}

//============================================================
//	カメラ取得処理
//============================================================
CCamera::Camera CCamera::GetCamera(void)
{
	// カメラの情報を返す
	return m_camera;
}

//============================================================
//	向き取得処理
//============================================================
D3DXVECTOR3 CCamera::GetRotation(void) const
{
	// カメラの向きを返す
	return m_camera.rot;
}

//============================================================
//	目標向き取得処理
//============================================================
D3DXVECTOR3 CCamera::GetDestRotation(void) const
{
	// カメラの目標向きを返す
	return m_camera.destRot;
}

//============================================================
//	生成処理
//============================================================
CCamera *CCamera::Create(void)
{
	// ポインタを宣言
	CCamera *pCamera = NULL;	// カメラ生成用

	if (UNUSED(pCamera))
	{ // 使用されていない場合

		// メモリを確保
		pCamera = new CCamera;	// カメラ
	}
	else { assert(false); return NULL; }	// 使用中

	if (USED(pCamera))
	{ // 確保に成功している場合

		// カメラの初期化
		if (FAILED(pCamera->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pCamera;
			pCamera = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pCamera;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CCamera::Release(CCamera *&prCamera)
{
	if (USED(prCamera))
	{ // 使用中の場合

		// カメラの終了
		prCamera->Uninit();

		// メモリ開放
		delete prCamera;
		prCamera = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	カメラの更新処理 (追従)
//============================================================
void CCamera::Follow(void)
{
	// 変数を宣言
	D3DXVECTOR3 diffRot  = VEC3_ZERO;	// カメラの向き差分
	D3DXVECTOR3 diffPosV = VEC3_ZERO;	// カメラの視点の位置差分
	D3DXVECTOR3 diffPosR = VEC3_ZERO;	// カメラの注視点の位置差分
	D3DXVECTOR3 pos =  CManager::GetPlayer()->GetPosition();	// プレイヤー位置
	D3DXVECTOR3 rot =  CManager::GetPlayer()->GetRotation();	// プレイヤー向き

	// 目標の向きまでの差分を計算
	diffRot = m_camera.destRot - m_camera.rot;
	useful::NormalizeRot(diffRot.y);		// 差分向き正規化

	// 向きを更新
	m_camera.rot.y += diffRot.y * REV_ROT;
	useful::NormalizeRot(m_camera.rot.y);	// 向き正規化

	// 目標の注視点の位置を更新
	m_camera.destPosR.x = pos.x + sinf(rot.y + D3DX_PI) * POS_R_PLUS;	// プレイヤーの位置より少し前
	m_camera.destPosR.y = pos.y;										// プレイヤーの位置と同じ
	m_camera.destPosR.z = pos.z + cosf(rot.y + D3DX_PI) * POS_R_PLUS;	// プレイヤーの位置より少し前

	// 目標の視点の位置を更新
	m_camera.destPosV.x = m_camera.destPosR.x + ((m_camera.fDis * sinf(m_camera.rot.x)) * sinf(m_camera.rot.y));	// 目標注視点から距離分離れた位置
	m_camera.destPosV.y = POS_V_Y;																					// 固定の高さ
	m_camera.destPosV.z = m_camera.destPosR.z + ((m_camera.fDis * sinf(m_camera.rot.x)) * cosf(m_camera.rot.y));	// 目標注視点から距離分離れた位置

	// 目標の位置までの差分を計算
	diffPosV = m_camera.destPosV - m_camera.posV;	// 視点
	diffPosR = m_camera.destPosR - m_camera.posR;	// 注視点

	// 視点の位置を更新
	m_camera.posV.x += diffPosV.x * REV_POS_V.x;
	m_camera.posV.y += diffPosV.y * REV_POS_V.y;
	m_camera.posV.z += diffPosV.z * REV_POS_V.x;

	// 注視点の位置を更新
	m_camera.posR.x += diffPosR.x * REV_POS_R.x;
	m_camera.posR.y += diffPosR.y * REV_POS_R.y;
	m_camera.posR.z += diffPosR.z * REV_POS_R.x;
}

//============================================================
//	カメラの更新処理 (寄り引き)
//============================================================
void CCamera::Bargaining(void)
{
	if (USED(CManager::GetPlayer()) && USED(CManager::GetTarget()))
	{ // プレイヤー・ターゲットが使用されている場合

		// 変数を宣言
		D3DXVECTOR3 posPlayer = CManager::GetPlayer()->GetPosition();	// プレイヤー位置
		D3DXVECTOR3 posTarget = CManager::GetTarget()->GetPosition();	// ターゲット位置
		D3DXVECTOR3 destPosR = VEC3_ZERO;	// カメラの注視点の目標位置
		D3DXVECTOR3 diffPosR = VEC3_ZERO;	// カメラの注視点の位置差分
		D3DXVECTOR3 diffPosV = VEC3_ZERO;	// カメラの視点の位置差分
		float fDisTarget = CManager::GetPlayer()->GetDistanceTarget();	// ターゲットとプレイヤー間の距離
		float fDiffRot = 0.0f;	// カメラの向き差分

		// 差分向きを求める
		fDiffRot = m_camera.destRot.y - m_camera.rot.y;
		useful::NormalizeRot(fDiffRot);			// 差分向き正規化

		// 向きを更新
		m_camera.rot.y += fDiffRot * REV_ROT;
		useful::NormalizeRot(m_camera.rot.y);	// 向き正規化

		// ターゲットとプレイヤーの中間位置を求める
		destPosR = (posPlayer - posTarget) * REV_CENTER;	// 中間位置を設定
		destPosR.y = 0.0f;	// y座標を初期化

		// カメラの向きをターゲットに向かせる
		m_camera.rot.y = atan2f(posTarget.x - posPlayer.x, posTarget.z - posPlayer.z);

		// カメラの距離を設定
		m_camera.fDis = FIRST_DIS - (fDisTarget * REV_PULS_DIS);

		// 目標の注視点の位置を更新
		m_camera.destPosR.x = destPosR.x;
		m_camera.destPosR.y = POSR_Y;
		m_camera.destPosR.z = destPosR.z;

		// 目標の視点の位置を更新
		m_camera.destPosV.x = m_camera.destPosR.x + ((m_camera.fDis * sinf(m_camera.rot.x)) * sinf(m_camera.rot.y));
		m_camera.destPosV.y = MIN_POSV_Y + (fDisTarget * REV_PLUS_POSV_Y);
		m_camera.destPosV.z = m_camera.destPosR.z + ((m_camera.fDis * sinf(m_camera.rot.x)) * cosf(m_camera.rot.y));

		// 目標の位置までの差分を計算
		diffPosV = m_camera.destPosV - m_camera.posV;	// 視点
		diffPosR = m_camera.destPosR - m_camera.posR;	// 注視点

		// 視点の位置を更新
		m_camera.posV.x += diffPosV.x * REV_POS_V.x;
		m_camera.posV.y += diffPosV.y * REV_POS_V.y;
		m_camera.posV.z += diffPosV.z * REV_POS_V.x;

		// 注視点の位置を更新
		m_camera.posR.x += diffPosR.x * REV_POS_R.x;
		m_camera.posR.y += diffPosR.y * REV_POS_R.y;
		m_camera.posR.z += diffPosR.z * REV_POS_R.x;
	}
}

//============================================================
//	カメラの更新処理 (操作)
//============================================================
void CCamera::Control(void)
{
	// 位置の更新
	Move();

	// 距離の更新
	Distance();

	// 向きの更新
	Rotation();
}

//============================================================
//	位置の更新処理 (操作)
//============================================================
void CCamera::Move(void)
{
	// ポインタを宣言
	CInputMouse	*pMouse = CManager::GetMouse();	// マウスの取得

	// 変数を宣言
	D3DXVECTOR3 mouseMove = pMouse->GetMove();	// マウスの移動量

	// マウス操作の更新
	if (pMouse->GetPress(CInputMouse::KEY_LEFT) == true && pMouse->GetPress(CInputMouse::KEY_RIGHT) == true)
	{ // 右クリックと左クリックが押されている場合

		// 視点を移動
		m_camera.posV.x -= sinf(m_camera.rot.y + (D3DX_PI * 0.5f)) * mouseMove.x * REV_MOVE_MOUSE;
		m_camera.posV.z -= cosf(m_camera.rot.y + (D3DX_PI * 0.5f)) * mouseMove.x * REV_MOVE_MOUSE;
		m_camera.posV.x += sinf(m_camera.rot.y) * mouseMove.y * REV_MOVE_MOUSE;
		m_camera.posV.z += cosf(m_camera.rot.y) * mouseMove.y * REV_MOVE_MOUSE;

		// 注視点を移動
		m_camera.posR.x -= sinf(m_camera.rot.y + (D3DX_PI * 0.5f)) * mouseMove.x * REV_MOVE_MOUSE;
		m_camera.posR.z -= cosf(m_camera.rot.y + (D3DX_PI * 0.5f)) * mouseMove.x * REV_MOVE_MOUSE;
		m_camera.posR.x += sinf(m_camera.rot.y) * mouseMove.y * REV_MOVE_MOUSE;
		m_camera.posR.z += cosf(m_camera.rot.y) * mouseMove.y * REV_MOVE_MOUSE;
	}
}

//============================================================
//	距離の更新処理 (操作)
//============================================================
void CCamera::Distance(void)
{
	// ポインタを宣言
	CInputMouse	*pMouse = CManager::GetMouse();	// マウスの取得

	// 変数を宣言
	D3DXVECTOR3 mouseMove = pMouse->GetMove();	// マウスの移動量

	// マウス操作の更新
	if (mouseMove.z != 0.0f)
	{ // マウスホイールが操作された場合

		// 距離を加算
		m_camera.fDis += mouseMove.z * REV_DIS_MOUSE;
	}

	// 距離の補正
	useful::LimitNum(m_camera.fDis, MIN_DIS, MAX_DIS);
}

//============================================================
//	向きの更新処理 (操作)
//============================================================
void CCamera::Rotation(void)
{
	// ポインタを宣言
	CInputMouse	*pMouse = CManager::GetMouse();	// マウスの取得

	// 変数を宣言
	D3DXVECTOR3 mouseMove = pMouse->GetMove();	// マウスの移動量

	// マウス操作の更新
	if (pMouse->GetPress(CInputMouse::KEY_LEFT) == true && pMouse->GetPress(CInputMouse::KEY_RIGHT) == false)
	{ // 左クリックだけが押されている場合

		// カメラの y軸を回転
		m_camera.rot.y += mouseMove.x * REV_ROT_MOUSE;

		// カメラの x軸を回転
		m_camera.rot.x += mouseMove.y * REV_ROT_MOUSE;
	}

	// 向きの補正
	useful::LimitNum(m_camera.rot.x, LIMIT_ROT_LOW, LIMIT_ROT_HIGH);
	useful::NormalizeRot(m_camera.rot.y);

	// 視点の更新
	m_camera.posV.x = m_camera.posR.x + ((m_camera.fDis * sinf(m_camera.rot.x)) * sinf(m_camera.rot.y));
	m_camera.posV.y = m_camera.posR.y + ((m_camera.fDis * cosf(m_camera.rot.x)));
	m_camera.posV.z = m_camera.posR.z + ((m_camera.fDis * sinf(m_camera.rot.x)) * cosf(m_camera.rot.y));

	// マウス操作の更新
	if (pMouse->GetPress(CInputMouse::KEY_RIGHT) == true && pMouse->GetPress(CInputMouse::KEY_LEFT) == false)
	{ // 右クリックだけが押されている場合

		// カメラの y軸を回転
		m_camera.rot.y += mouseMove.x * REV_ROT_MOUSE;

		// カメラの x軸を回転
		m_camera.rot.x += mouseMove.y * REV_ROT_MOUSE;
	}

	// 向きの補正
	useful::LimitNum(m_camera.rot.x, LIMIT_ROT_LOW, LIMIT_ROT_HIGH);
	useful::NormalizeRot(m_camera.rot.y);

	// 注視点の更新
	m_camera.posR.x = m_camera.posV.x + ((-m_camera.fDis * sinf(m_camera.rot.x)) * sinf(m_camera.rot.y));
	m_camera.posR.y = m_camera.posV.y + ((-m_camera.fDis * cosf(m_camera.rot.x)));
	m_camera.posR.z = m_camera.posV.z + ((-m_camera.fDis * sinf(m_camera.rot.x)) * cosf(m_camera.rot.y));
}
