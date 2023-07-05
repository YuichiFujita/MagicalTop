//============================================================
//
//	�v���C���[���� [player.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "collision.h"
#include "debugproc.h"
#include "field.h"

//************************************************************
//	�}�N����`
//************************************************************
#define PLAYER_SETUP_TXT	"data\\TXT\\player.txt"	// �Z�b�g�A�b�v�e�L�X�g���΃p�X

#define PLAY_MOVE		(2.0f)	// �v���C���[�ړ���
#define PLAY_REV		(0.2f)	// �v���C���[�ړ��ʂ̌����W��
#define PLAY_REV_ROTA	(0.15f)	// �v���C���[�����ύX�̌����W��
#define PLAY_CAM_ROTA	(0.04f)	// �J������]��
#define PLAY_JUMP		(20.0f)	// �v���C���[�W�����v��
#define PLAY_GRAVITY	(1.0f)	// �v���C���[�d��
#define PLAY_RADIUS		(20.0f)	// �v���C���[���a

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CPlayer::mc_apModelFile[] =	// ���f���萔
{
	"data\\MODEL\\escapekun000.x",		// �G�X�P�[�v�N���f��
};

//************************************************************
//	�q�N���X [CPlayer] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPlayer::CPlayer() : CObjectModel(CObject::LABEL_PLAYER)
{
	// �����o�ϐ����N���A
	m_move		= VEC3_ZERO;	// �ړ���
	m_destRot	= VEC3_ZERO;	// �ڕW����
	m_bJump		= false;		// �W�����v��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CPlayer::~CPlayer()
{

}

//============================================================
//	����������
//============================================================
HRESULT CPlayer::Init(void)
{
	// �����o�ϐ���������
	m_move		= VEC3_ZERO;	// �ړ���
	m_destRot	= VEC3_ZERO;	// �ڕW����
	m_bJump		= false;		// �W�����v��

	// �I�u�W�F�N�g���f���̏�����
	if (FAILED(CObjectModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CPlayer::Uninit(void)
{
	// �I�u�W�F�N�g���f���̏I��
	CObjectModel::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CPlayer::Update(void)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = GetPosition();	// �ʒu

	// �ړ�����
	Move();

	// �����X�V
	Rot();

	// �W�����v����
	Jump();

	// �d�͂����Z
	m_move.y -= PLAY_GRAVITY;

	// �ړ��ʂ����Z
	pos += m_move;

	// �ړ��ʂ�����
	m_move.x += (0.0f - m_move.x) * PLAY_REV;
	m_move.z += (0.0f - m_move.z) * PLAY_REV;

	// ���n����
	Land(pos);

	// �ʒu�𔽉f
	SetPosition(pos);

	// �I�u�W�F�N�g���f���̍X�V
	CObjectModel::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CPlayer::Draw(void)
{
	// �I�u�W�F�N�g���f���̕`��
	CObjectModel::Draw();
}

//============================================================
//	��������
//============================================================
CPlayer *CPlayer::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot)
{
	// �ϐ���錾
	int nModelID;	// ���f���C���f�b�N�X

	// �|�C���^��錾
	CModel *pModel = CManager::GetModel();	// ���f���ւ̃|�C���^
	CPlayer *pPlayer = NULL;	// �v���C���[�����p

	if (UNUSED(pPlayer))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pPlayer = new CPlayer;	// �v���C���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pPlayer))
	{ // �g�p����Ă���ꍇ
		
		if (SUCCEEDED(pPlayer->GetResult()))
		{ // �I�u�W�F�N�g�̏��ݒ�ɐ��������ꍇ

			// �v���C���[�̏�����
			if (FAILED(pPlayer->Init()))
			{ // �������Ɏ��s�����ꍇ

				// �������J��
				delete pPlayer;
				pPlayer = NULL;

				// ���s��Ԃ�
				return NULL;
			}

			// ���f����o�^
			nModelID = pModel->Regist(mc_apModelFile[MODEL_ESCAPEKUN]);

			// ���f��������
			pPlayer->BindModel(pModel->GetModel(nModelID));

			// �ʒu��ݒ�
			pPlayer->SetPosition(rPos);

			// ������ݒ�
			pPlayer->SetRotation(rRot);

			// �m�ۂ����A�h���X��Ԃ�
			return pPlayer;
		}
		else
		{ // �I�u�W�F�N�g�̏��ݒ�Ɏ��s�����ꍇ

			// �������J��
			delete pPlayer;
			pPlayer = NULL;

			// ���s��Ԃ�
			return NULL;
		}
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�ړ�����
//============================================================
void CPlayer::Move(void)
{
	// �ϐ���錾
	D3DXVECTOR3 rot = CManager::GetCamera()->GetRotation();	// �J�����̌���

	// �|�C���^��錾
	CInputKeyboard	*pKeyboard = CManager::GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad = CManager::GetPad();				// �p�b�h

	// �ړ�����
	if (pKeyboard->GetPress(DIK_W))
	{ // ���ړ��̑��삪�s��ꂽ�ꍇ

		if (pKeyboard->GetPress(DIK_A))
		{ // ���ړ��̑�����s��ꂽ�ꍇ (�����ړ�)

			// �ړ��ʂ��X�V
			m_move.x += sinf(rot.y - (D3DX_PI * 0.25f)) * PLAY_MOVE;
			m_move.z += cosf(rot.y - (D3DX_PI * 0.25f)) * PLAY_MOVE;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(135) + rot.y;
		}
		else if (pKeyboard->GetPress(DIK_D))
		{ // �E�ړ��̑�����s��ꂽ�ꍇ (�E���ړ�)

			// �ړ��ʂ��X�V
			m_move.x -= sinf(rot.y - (D3DX_PI * 0.75f)) * PLAY_MOVE;
			m_move.z -= cosf(rot.y - (D3DX_PI * 0.75f)) * PLAY_MOVE;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(225) + rot.y;
		}
		else
		{ // ���ړ��̑��삾�����s��ꂽ�ꍇ (���ړ�)

			// �ړ��ʂ��X�V
			m_move.x += sinf(rot.y) * PLAY_MOVE;
			m_move.z += cosf(rot.y) * PLAY_MOVE;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(180) + rot.y;
		}
	}
	else if (pKeyboard->GetPress(DIK_S))
	{ // ��O�ړ��̑��삪�s��ꂽ�ꍇ

		if (pKeyboard->GetPress(DIK_A))
		{ // ���ړ��̑�����s��ꂽ�ꍇ (����O�ړ�)

			// �ړ��ʂ��X�V
			m_move.x += sinf(rot.y - (D3DX_PI * 0.75f)) * PLAY_MOVE;
			m_move.z += cosf(rot.y - (D3DX_PI * 0.75f)) * PLAY_MOVE;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(45) + rot.y;
		}
		else if (pKeyboard->GetPress(DIK_D))
		{ // �E�ړ��̑�����s��ꂽ�ꍇ (�E��O�ړ�)

			// �ړ��ʂ��X�V
			m_move.x -= sinf(rot.y - (D3DX_PI * 0.25f)) * PLAY_MOVE;
			m_move.z -= cosf(rot.y - (D3DX_PI * 0.25f)) * PLAY_MOVE;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(315) + rot.y;
		}
		else
		{ // ��O�ړ��̑��삾�����s��ꂽ�ꍇ (��O�ړ�)

			// �ړ��ʂ��X�V
			m_move.x -= sinf(rot.y) * PLAY_MOVE;
			m_move.z -= cosf(rot.y) * PLAY_MOVE;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(0) + rot.y;
		}
	}
	else if (pKeyboard->GetPress(DIK_A))
	{ // ���ړ��̑��삪�s��ꂽ�ꍇ

		// �ړ��ʂ��X�V
		m_move.x += sinf(rot.y - (D3DX_PI * 0.5f)) * PLAY_MOVE;
		m_move.z += cosf(rot.y - (D3DX_PI * 0.5f)) * PLAY_MOVE;

		// �ڕW�������X�V
		m_destRot.y = D3DXToRadian(90) + rot.y;
	}
	else if (pKeyboard->GetPress(DIK_D))
	{ // �E�ړ��̑��삪�s��ꂽ�ꍇ

		// �ړ��ʂ��X�V
		m_move.x -= sinf(rot.y - (D3DX_PI * 0.5f)) * PLAY_MOVE;
		m_move.z -= cosf(rot.y - (D3DX_PI * 0.5f)) * PLAY_MOVE;

		// �ڕW�������X�V
		m_destRot.y = D3DXToRadian(270) + rot.y;
	}
}

//============================================================
//	��������
//============================================================
void CPlayer::Rot(void)
{
	// �ϐ���錾
	D3DXVECTOR3 rot = GetRotation();	// ����
	float fDiffRot = 0.0f;	// ��������

	// �ڕW�����̐��K��
	useful::NormalizeRot(m_destRot.y);

	// �ڕW�����܂ł̍������v�Z
	fDiffRot = m_destRot.y - rot.y;

	// ���������̐��K��
	useful::NormalizeRot(fDiffRot);

	// �����̍X�V
	rot.y += fDiffRot * PLAY_REV_ROTA;

	// �����̐��K��
	useful::NormalizeRot(rot.y);

	// �����𔽉f
	SetRotation(rot);
}

//============================================================
//	�W�����v����
//============================================================
void CPlayer::Jump(void)
{
	// �|�C���^��錾
	CInputKeyboard	*pKeyboard = CManager::GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad = CManager::GetPad();				// �p�b�h

	// �W�����v����
	if (m_bJump == false)
	{ // �W�����v���Ă��Ȃ��ꍇ

		if (pKeyboard->GetTrigger(DIK_SPACE))
		{ // �W�����v�̑��삪�s��ꂽ�ꍇ

			// ��ړ���
			m_move.y += PLAY_JUMP;

			// �W�����v���Ă����Ԃɂ���
			m_bJump = true;
		}
	}
}

//============================================================
//	���n����
//============================================================
void CPlayer::Land(D3DXVECTOR3& rPos)
{
	// ���n����
	if (CManager::GetField()->LandPosition(rPos, m_move))
	{ // �v���C���[�����n���Ă����ꍇ

		// �W�����v���Ă��Ȃ���Ԃɂ���
		m_bJump = false;
	}
	else
	{ // ���n���Ă��Ȃ��ꍇ

		// �W�����v���Ă����Ԃɂ���
		m_bJump = true;
	}
}
