//============================================================
//
//	�J�������� [camera.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "player.h"
#include "debugproc.h"

//************************************************************
//	�}�N����`
//************************************************************
#define VIEW_ANGLE	(D3DXToRadian(45.0f))	// ����p
#define VIEW_ASPECT	((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �A�X�y�N�g��

#define VIEW_NEAR	(10.0f)		// ���f����������Z���̍ŏ��l
#define VIEW_FAR	(50000.0f)	// ���f����������Z���̍ő�l

#define REV_ROT		(0.2f)		// �J���������̕␳�W��
#define POS_R_PLUS	(25.0f)		// �v���C���[�̈ʒu�ƒ����_�̈ʒu�̋���
#define POS_V_Y		(900.0f)	// �Ǐ]���̎��_�� y���W
#define REV_POS_V	(D3DXVECTOR2(0.225f, 0.045f))	// ���_�̕␳�W��
#define REV_POS_R	(D3DXVECTOR2(0.25f, 0.05f))		// �����_�̕␳�W��

#define REV_MOVE_MOUSE	(1.6f)		// �}�E�X����ł̃J�����̈ړ��̕␳�W��
#define REV_DIS_MOUSE	(-0.3f)		// �}�E�X����ł̃J�����̋����̕␳�W��
#define REV_ROT_MOUSE	(0.008f)	// �}�E�X����ł̃J�����̉�]�ʂ̕␳�W��

#define MIN_DIS	(-10000.0f)	// �J�����̎��_���璍���_�ւ̋����̍ŏ�
#define MAX_DIS	(-1.0f)		// �J�����̎��_���璍���_�ւ̋����̍ő�
#define LIMIT_ROT_HIGH	(D3DX_PI - 0.1f)	// x��]�̐����l (��)
#define LIMIT_ROT_LOW	(0.1f)				// x��]�̐����l (��)

//************************************************************
//	�e�N���X [CCamera] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CCamera::CCamera()
{
	// �����o�ϐ����N���A
	memset(&m_camera, 0, sizeof(m_camera));	// �J�����̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CCamera::~CCamera()
{

}

//============================================================
//	����������
//============================================================
HRESULT CCamera::Init(void)
{
	// �J��������������
	m_camera.posV		= D3DXVECTOR3(0.0f, 400.0f, 0.0f);	// ���݂̎��_
	m_camera.posR		= D3DXVECTOR3(0.0f, 400.0f, 0.0f);	// ���݂̒����_
	m_camera.destPosV	= D3DXVECTOR3(0.0f, 400.0f, 0.0f);	// �ڕW�̎��_
	m_camera.destPosR	= D3DXVECTOR3(0.0f, 400.0f, 0.0f);	// �ڕW�̒����_
	m_camera.vecU		= D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// ������x�N�g��
	m_camera.rot		= D3DXVECTOR3(1.6f, 0.0f, 0.0f);	// ���݂̌���
	m_camera.destRot	= D3DXVECTOR3(1.6f, 0.0f, 0.0f);	// �ڕW�̌���
	m_camera.fDis		= -800.0f;							// ���_�ƒ����_�̋���

	// �r���[�|�[�g����������
	m_camera.viewport.X			= 0;				// ������̃s�N�Z�����W (x)
	m_camera.viewport.Y			= 0;				// ������̃s�N�Z�����W (y)
	m_camera.viewport.Width		= SCREEN_WIDTH;		// �`�悷���ʂ̉���
	m_camera.viewport.Height	= SCREEN_HEIGHT;	// �`�悷���ʂ̏c��
	m_camera.viewport.MinZ		= 0.0f;
	m_camera.viewport.MaxZ		= 1.0f;

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CCamera::Uninit(void)
{

}

//============================================================
//	�X�V����
//============================================================
void CCamera::Update(void)
{
#if 0
	// �J�����̈ʒu�̍X�V (�Ǐ])
	MoveFollowCamera();
#else
	// �J�����̈ʒu�̍X�V (����)
	MoveCamera();

	// �J�����̋����̍X�V (����)
	DisCamera();

	// �J�����̌����̍X�V (����)
	RotCamera();
#endif

	// �f�o�b�O�\��
	CManager::GetDebugProc()->Print(" ���_ �F%f %f %f\n", m_camera.posV.x, m_camera.posV.y, m_camera.posV.z);
	CManager::GetDebugProc()->Print("�����_�F%f %f %f\n", m_camera.posR.x, m_camera.posR.y, m_camera.posR.z);
	CManager::GetDebugProc()->Print(" ���� �F%f %f %f\n", m_camera.rot.x, m_camera.rot.y, m_camera.rot.z);
	CManager::GetDebugProc()->Print(" ���� �F%f\n", m_camera.fDis);
}

//============================================================
//	�J�����ݒ菈��
//============================================================
void CCamera::SetCamera(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// �r���[�|�[�g�̐ݒ�
	pDevice->SetViewport(&m_camera.viewport);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_camera.mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH
	( // ����
		&m_camera.mtxProjection,	// �v���W�F�N�V�����}�g���b�N�X
		VIEW_ANGLE,		// ����p
		VIEW_ASPECT,	// ��ʂ̃A�X�y�N�g��
		VIEW_NEAR,		// Z���̍ŏ��l
		VIEW_FAR		// Z���̍ő�l
	);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_camera.mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_camera.mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH
	( // ����
		&m_camera.mtxView,	// �r���[�}�g���b�N�X
		&m_camera.posV,		// ���_
		&m_camera.posR,		// �����_
		&m_camera.vecU		// ������x�N�g��
	);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_camera.mtxView);
}

//============================================================
//	�J�����̖ڕW�ʒu�̐ݒ菈��
//============================================================
void CCamera::SetDestCamera(void)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = CManager::GetPlayer()->GetPosition();		// �v���C���[�ʒu
	D3DXVECTOR3 rot = CManager::GetPlayer()->GetRotation();		// �v���C���[����

	// �ڕW�̒����_�̈ʒu���X�V
	m_camera.destPosR.x = m_camera.posR.x = pos.x + sinf(rot.y + D3DX_PI) * POS_R_PLUS;	// �v���C���[�̈ʒu��菭���O
	m_camera.destPosR.y = m_camera.posR.y = pos.y;										// �v���C���[�̈ʒu�Ɠ���
	m_camera.destPosR.z = m_camera.posR.z = pos.z + cosf(rot.y + D3DX_PI) * POS_R_PLUS;	// �v���C���[�̈ʒu��菭���O

	// �ڕW�̎��_�̈ʒu���X�V
	m_camera.destPosV.x = m_camera.posV.x = m_camera.posR.x + ((m_camera.fDis * sinf(m_camera.rot.x)) * sinf(m_camera.rot.y));	// �ڕW�����_���狗�������ꂽ�ʒu
	m_camera.destPosV.y = m_camera.posV.y = POS_V_Y;																			// �Œ�̍���
	m_camera.destPosV.z = m_camera.posV.z = m_camera.posR.z + ((m_camera.fDis * sinf(m_camera.rot.x)) * cosf(m_camera.rot.y));	// �ڕW�����_���狗�������ꂽ�ʒu
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CCamera::SetRotation(const D3DXVECTOR3& rRot)
{
	// �����̃J�����̌�����ݒ�
	m_camera.rot = rRot;

	// �����𐳋K��
	useful::NormalizeRot(m_camera.rot.x);
	useful::NormalizeRot(m_camera.rot.y);
	useful::NormalizeRot(m_camera.rot.z);
}

//============================================================
//	�ڕW�����̐ݒ菈��
//============================================================
void CCamera::SetDestRotation(const D3DXVECTOR3& rRot)
{
	// �����̃J�����̖ڕW������ݒ�
	m_camera.destRot = rRot;

	// �����𐳋K��
	useful::NormalizeRot(m_camera.destRot.x);
	useful::NormalizeRot(m_camera.destRot.y);
	useful::NormalizeRot(m_camera.destRot.z);
}

//============================================================
//	�J�����擾����
//============================================================
CCamera::Camera CCamera::GetCamera(void)
{
	// �J�����̏���Ԃ�
	return m_camera;
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CCamera::GetRotation(void) const
{
	// �J�����̌�����Ԃ�
	return m_camera.rot;
}

//============================================================
//	�ڕW�����擾����
//============================================================
D3DXVECTOR3 CCamera::GetDestRotation(void) const
{
	// �J�����̖ڕW������Ԃ�
	return m_camera.destRot;
}

//============================================================
//	��������
//============================================================
CCamera *CCamera::Create(void)
{
	// �|�C���^��錾
	CCamera *pCamera = NULL;	// �J���������p

	if (UNUSED(pCamera))
	{ // �g�p����Ă��Ȃ��ꍇ

		// ���������m��
		pCamera = new CCamera;	// �J����
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pCamera))
	{ // �m�ۂɐ������Ă���ꍇ

		// �J�����̏�����
		if (FAILED(pCamera->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pCamera;
			pCamera = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pCamera;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CCamera::Release(CCamera *&prCamera)
{
	if (USED(prCamera))
	{ // �g�p���̏ꍇ

		// �J�����̏I��
		prCamera->Uninit();

		// �������J��
		delete prCamera;
		prCamera = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�ʒu�̍X�V���� (�Ǐ])
//============================================================
void CCamera::MoveFollowCamera(void)
{
	// �ϐ���錾
	D3DXVECTOR3 diffRot  = VEC3_ZERO;	// �J�����̌�������
	D3DXVECTOR3 diffPosV = VEC3_ZERO;	// �J�����̎��_�̈ʒu����
	D3DXVECTOR3 diffPosR = VEC3_ZERO;	// �J�����̒����_�̈ʒu����
	D3DXVECTOR3 pos =  CManager::GetPlayer()->GetPosition();	// �v���C���[�ʒu
	D3DXVECTOR3 rot =  CManager::GetPlayer()->GetRotation();	// �v���C���[����

	// �ڕW�̌����܂ł̍������v�Z
	diffRot = m_camera.destRot - m_camera.rot;
	useful::NormalizeRot(diffRot.y);		// �����������K��

	// �������X�V
	m_camera.rot.y += diffRot.y * REV_ROT;
	useful::NormalizeRot(m_camera.rot.y);	// �������K��

	// �ڕW�̒����_�̈ʒu���X�V
	m_camera.destPosR.x = pos.x + sinf(rot.y + D3DX_PI) * POS_R_PLUS;	// �v���C���[�̈ʒu��菭���O
	m_camera.destPosR.y = pos.y;										// �v���C���[�̈ʒu�Ɠ���
	m_camera.destPosR.z = pos.z + cosf(rot.y + D3DX_PI) * POS_R_PLUS;	// �v���C���[�̈ʒu��菭���O

	// �ڕW�̎��_�̈ʒu���X�V
	m_camera.destPosV.x = m_camera.destPosR.x + ((m_camera.fDis * sinf(m_camera.rot.x)) * sinf(m_camera.rot.y));	// �ڕW�����_���狗�������ꂽ�ʒu
	m_camera.destPosV.y = POS_V_Y;																					// �Œ�̍���
	m_camera.destPosV.z = m_camera.destPosR.z + ((m_camera.fDis * sinf(m_camera.rot.x)) * cosf(m_camera.rot.y));	// �ڕW�����_���狗�������ꂽ�ʒu

	// �ڕW�̈ʒu�܂ł̍������v�Z
	diffPosV = m_camera.destPosV - m_camera.posV;	// ���_
	diffPosR = m_camera.destPosR - m_camera.posR;	// �����_

	// ���_�̈ʒu���X�V
	m_camera.posV.x += diffPosV.x * REV_POS_V.x;
	m_camera.posV.y += diffPosV.y * REV_POS_V.y;
	m_camera.posV.z += diffPosV.z * REV_POS_V.x;

	// �����_�̈ʒu���X�V
	m_camera.posR.x += diffPosR.x * REV_POS_R.x;
	m_camera.posR.y += diffPosR.y * REV_POS_R.y;
	m_camera.posR.z += diffPosR.z * REV_POS_R.x;
}

//============================================================
//	�ʒu�̍X�V���� (����)
//============================================================
void CCamera::MoveCamera(void)
{
	// �|�C���^��錾
	CInputMouse	*pMouse = CManager::GetMouse();	// �}�E�X�̎擾

	// �ϐ���錾
	D3DXVECTOR3 mouseMove = pMouse->GetMove();	// �}�E�X�̈ړ���

	// �}�E�X����̍X�V
	if (pMouse->GetPress(CInputMouse::KEY_LEFT) == true && pMouse->GetPress(CInputMouse::KEY_RIGHT) == true)
	{ // �E�N���b�N�ƍ��N���b�N��������Ă���ꍇ

		// ���_���ړ�
		m_camera.posV.x -= sinf(m_camera.rot.y + (D3DX_PI * 0.5f)) * mouseMove.x * REV_MOVE_MOUSE;
		m_camera.posV.z -= cosf(m_camera.rot.y + (D3DX_PI * 0.5f)) * mouseMove.x * REV_MOVE_MOUSE;
		m_camera.posV.x += sinf(m_camera.rot.y) * mouseMove.y * REV_MOVE_MOUSE;
		m_camera.posV.z += cosf(m_camera.rot.y) * mouseMove.y * REV_MOVE_MOUSE;

		// �����_���ړ�
		m_camera.posR.x -= sinf(m_camera.rot.y + (D3DX_PI * 0.5f)) * mouseMove.x * REV_MOVE_MOUSE;
		m_camera.posR.z -= cosf(m_camera.rot.y + (D3DX_PI * 0.5f)) * mouseMove.x * REV_MOVE_MOUSE;
		m_camera.posR.x += sinf(m_camera.rot.y) * mouseMove.y * REV_MOVE_MOUSE;
		m_camera.posR.z += cosf(m_camera.rot.y) * mouseMove.y * REV_MOVE_MOUSE;
	}
}

//============================================================
//	�����̍X�V���� (����)
//============================================================
void CCamera::DisCamera(void)
{
	// �|�C���^��錾
	CInputMouse	*pMouse = CManager::GetMouse();	// �}�E�X�̎擾

	// �ϐ���錾
	D3DXVECTOR3 mouseMove = pMouse->GetMove();	// �}�E�X�̈ړ���

	// �}�E�X����̍X�V
	if (mouseMove.z != 0.0f)
	{ // �}�E�X�z�C�[�������삳�ꂽ�ꍇ

		// ���������Z
		m_camera.fDis += mouseMove.z * REV_DIS_MOUSE;
	}

	// �����̕␳
	useful::LimitNum(m_camera.fDis, MIN_DIS, MAX_DIS);
}

//============================================================
//	�����̍X�V���� (����)
//============================================================
void CCamera::RotCamera(void)
{
	// �|�C���^��錾
	CInputMouse	*pMouse = CManager::GetMouse();	// �}�E�X�̎擾

	// �ϐ���錾
	D3DXVECTOR3 mouseMove = pMouse->GetMove();	// �}�E�X�̈ړ���

	// �}�E�X����̍X�V
	if (pMouse->GetPress(CInputMouse::KEY_LEFT) == true && pMouse->GetPress(CInputMouse::KEY_RIGHT) == false)
	{ // ���N���b�N������������Ă���ꍇ

		// �J������ y������]
		m_camera.rot.y += mouseMove.x * REV_ROT_MOUSE;

		// �J������ x������]
		m_camera.rot.x += mouseMove.y * REV_ROT_MOUSE;
	}

	// �����̕␳
	useful::LimitNum(m_camera.rot.x, LIMIT_ROT_LOW, LIMIT_ROT_HIGH);
	useful::NormalizeRot(m_camera.rot.y);

	// ���_�̍X�V
	m_camera.posV.x = m_camera.posR.x + ((m_camera.fDis * sinf(m_camera.rot.x)) * sinf(m_camera.rot.y));
	m_camera.posV.y = m_camera.posR.y + ((m_camera.fDis * cosf(m_camera.rot.x)));
	m_camera.posV.z = m_camera.posR.z + ((m_camera.fDis * sinf(m_camera.rot.x)) * cosf(m_camera.rot.y));

	// �}�E�X����̍X�V
	if (pMouse->GetPress(CInputMouse::KEY_RIGHT) == true && pMouse->GetPress(CInputMouse::KEY_LEFT) == false)
	{ // �E�N���b�N������������Ă���ꍇ

		// �J������ y������]
		m_camera.rot.y += mouseMove.x * REV_ROT_MOUSE;

		// �J������ x������]
		m_camera.rot.x += mouseMove.y * REV_ROT_MOUSE;
	}

	// �����̕␳
	useful::LimitNum(m_camera.rot.x, LIMIT_ROT_LOW, LIMIT_ROT_HIGH);
	useful::NormalizeRot(m_camera.rot.y);

	// �����_�̍X�V
	m_camera.posR.x = m_camera.posV.x + ((-m_camera.fDis * sinf(m_camera.rot.x)) * sinf(m_camera.rot.y));
	m_camera.posR.y = m_camera.posV.y + ((-m_camera.fDis * cosf(m_camera.rot.x)));
	m_camera.posR.z = m_camera.posV.z + ((-m_camera.fDis * sinf(m_camera.rot.x)) * cosf(m_camera.rot.y));
}
