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

#include "multiModel.h"
#include "motion.h"
#include "magicManager.h"
#include "target.h"
#include "stage.h"
#include "field.h"
#include "particle3D.h"

//************************************************************
//	�}�N����`
//************************************************************
#define PLAYER_SETUP_TXT	"data\\TXT\\player.txt"	// �Z�b�g�A�b�v�e�L�X�g���΃p�X

#define MAX_MOVEX		(5.0f)	// �������s���̑��x�����p
#define PULSROT_MOVEZ	(20)	// �O��ړ����̃v���C���[�����̕ύX��
#define PLUS_MOVEX		(0.5f)	// ���E��]�̈ړ��ʂ̉��Z��
#define PLAY_MOVEZ		(2.0f)	// �O��̈ړ���
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
	"data\\MODEL\\PLAYER\\body.x",		// �̃��f��
	"data\\MODEL\\PLAYER\\head.x",		// �����f��
	"data\\MODEL\\PLAYER\\sleeve_l.x",	// ���r���f��
	"data\\MODEL\\PLAYER\\sleeve_r.x",	// �E�r���f��
	"data\\MODEL\\PLAYER\\wrist_l.x",	// ����񃂃f��
	"data\\MODEL\\PLAYER\\wrist_r.x",	// �E��񃂃f��
	"data\\MODEL\\PLAYER\\hand_l.x",	// ���胂�f��
	"data\\MODEL\\PLAYER\\hand_r.x",	// �E�胂�f��
	"data\\MODEL\\PLAYER\\pants_l.x",	// �������f��
	"data\\MODEL\\PLAYER\\pants_r.x",	// �E�����f��
	"data\\MODEL\\PLAYER\\leg_l.x",		// ���ڃ��f��
	"data\\MODEL\\PLAYER\\leg_r.x",		// �E�ڃ��f��
	"data\\MODEL\\PLAYER\\shoes_l.x",	// ���C���f��
	"data\\MODEL\\PLAYER\\shoes_r.x",	// �E�C���f��
	"data\\MODEL\\PLAYER\\knife.x",		// �i�C�t���f��
};

//************************************************************
//	�q�N���X [CPlayer] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPlayer::CPlayer() : CObject(CObject::LABEL_PLAYER)
{
	// �����o�ϐ����N���A
	memset(&m_apMultiModel[0], 0, sizeof(m_apMultiModel));	// ���f���̏��
	m_pMotion		= NULL;				// ���[�V�����̏��
	m_pMagic		= NULL;				// ���@�}�l�[�W���[�̏��
	m_pos			= VEC3_ZERO;		// ���݈ʒu
	m_oldPos		= VEC3_ZERO;		// �ߋ��ʒu
	m_move			= VEC3_ZERO;		// �ړ���
	m_rot			= VEC3_ZERO;		// ���݌���
	m_destRot		= VEC3_ZERO;		// �ڕW����
	m_rotation		= ROTATION_LEFT;	// ��]����
	m_nNumModel		= 0;				// �p�[�c�̑���
	m_fDisTarget	= 0.0f;				// �^�[�Q�b�g�Ƃ̋���
	m_bJump			= false;			// �W�����v��
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
	m_pos			= VEC3_ZERO;		// ���݈ʒu
	m_oldPos		= VEC3_ZERO;		// �ߋ��ʒu
	m_move			= VEC3_ZERO;		// �ړ���
	m_rot			= VEC3_ZERO;		// ���݌���
	m_destRot		= VEC3_ZERO;		// �ڕW����
	m_rotation		= ROTATION_LEFT;	// ��]����
	m_nNumModel		= 0;				// �p�[�c�̑���
	m_fDisTarget	= 0.0f;				// �^�[�Q�b�g�Ƃ̋���
	m_bJump			= true;				// �W�����v��

	// ���@�}�l�[�W���[�̐���
	m_pMagic = CMagicManager::Create();
	if (UNUSED(m_pMagic))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���[�V�����I�u�W�F�N�g�̐���
	m_pMotion = CMotion::Create();
	if (UNUSED(m_pMotion))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �Z�b�g�A�b�v�̓ǂݍ���
	LoadSetup();

	// ���f�����̐ݒ�
	m_pMotion->SetModel(&m_apMultiModel[0], m_nNumModel);

	// ���[�V�����̐ݒ�
	m_pMotion->Set(MOTION_NEUTRAL);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CPlayer::Uninit(void)
{
	// �}���`���f����j��
	for (int nCntPlayer = 0; nCntPlayer < MAX_PARTS; nCntPlayer++)
	{ // �p�[�c�̍ő吔���J��Ԃ�

		if (USED(m_apMultiModel[nCntPlayer]))
		{ // �p�[�c���g�p���̏ꍇ

			// �j������
			if (FAILED(m_apMultiModel[nCntPlayer]->Release(m_apMultiModel[nCntPlayer])))
			{ // �j���Ɏ��s�����ꍇ

				// ��O����
				assert(false);
			}
		}
	}

	// ���[�V������j��
	if (FAILED(m_pMotion->Release(m_pMotion)))
	{ // �j���Ɏ��s�����ꍇ

		// ��O����
		assert(false);
	}

	// ���@�}�l�[�W���[��j��
	if (FAILED(m_pMagic->Release(m_pMagic)))
	{ // �j���Ɏ��s�����ꍇ

		// ��O����
		assert(false);
	}

	// �v���C���[��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CPlayer::Update(void)
{
	// �ϐ���錾
	MOTION currentMotion = MOTION_NEUTRAL;	// ���݂̃��[�V����
	D3DXVECTOR3 posTarget = CManager::GetTarget()->GetPosition();	// �^�[�Q�b�g�ʒu

	// �ߋ��ʒu���X�V
	m_oldPos = m_pos;

	// �^�[�Q�b�g�Ƃ̋�����ݒ�
	m_fDisTarget = sqrtf((m_pos.x - posTarget.x) * (m_pos.x - posTarget.x) + (m_pos.z - posTarget.z) * (m_pos.z - posTarget.z)) * 0.5f;

	// �ړ�����
	currentMotion = Move(currentMotion);

	// �����X�V
	Rot();

	// �W�����v����
	currentMotion = Jump(currentMotion);

	// �d�͂����Z
	m_move.y -= PLAY_GRAVITY;

	// �ړ��ʂ����Z
	m_pos += m_move;

	// �ړ��ʂ�����
	m_move.x += (0.0f - m_move.x) * PLAY_REV;
	m_move.z += (0.0f - m_move.z) * PLAY_REV;

	// �����蔻��
	CollisionTarget();	// �^�[�Q�b�g
	CollisionEnemy();	// �G

	// �X�e�[�W�͈͊O�̕␳
	CManager::GetStage()->LimitPosition(m_pos, PLAY_RADIUS);

	// ���n����
	if (Land(currentMotion) == MOTION_LANDING)
	{ // ���n���Ă����ꍇ

		if (m_pMotion->GetType() == MOTION_JUMP)
		{ // �Đ������[�V�������W�����v�������ꍇ

			// ���n���[�V�����̐ݒ�
			m_pMotion->Set(MOTION_LANDING);
		}
	}

	// �ˌ�����
	currentMotion = Magic(currentMotion);

	// �J��������
	Camera();

	// ���[�V�����X�V
	Motion(currentMotion);
}

//============================================================
//	�`�揈��
//============================================================
void CPlayer::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntPlayer = 0; nCntPlayer < m_nNumModel; nCntPlayer++)
	{ // �p�[�c�̑������J��Ԃ�

		// �p�[�c�̕`��
		m_apMultiModel[nCntPlayer]->Draw();
	}
}

//============================================================
//	�q�b�g����
//============================================================
void CPlayer::Hit(const int nDmg)
{
#if 0
	// �̗͂���_���[�W�����Z
	m_pLifeGauge->AddLife(-nDmg);

	if (m_pLifeGauge->GetLife() > 0)
	{ // �����Ă���ꍇ

		// �p�[�e�B�N��3D�I�u�W�F�N�g�𐶐�
		CParticle3D::Create(CParticle3D::TYPE_DAMAGE, GetPosition());
	}
	else
#endif

	{ // ����ł���ꍇ

		// �p�[�e�B�N��3D�I�u�W�F�N�g�𐶐�
		CParticle3D::Create(CParticle3D::TYPE_DAMAGE, GetPosition(), D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f));
		CParticle3D::Create(CParticle3D::TYPE_DAMAGE, GetPosition(), D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f));

		// TODO�FPlayer��Uninit�ǂ�����̂���
#if 0
		// �v���C���[�I�u�W�F�N�g�̏I��
		Uninit();
#endif
	}
}

//============================================================
//	��������
//============================================================
CPlayer *CPlayer::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot)
{
	// �|�C���^��錾
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
//	�ʒu�̐ݒ菈��
//============================================================
void CPlayer::SetPosition(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CPlayer::SetRotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_rot = rRot;

	// �����̐��K��
	useful::NormalizeRot(m_rot.x);
	useful::NormalizeRot(m_rot.y);
	useful::NormalizeRot(m_rot.z);
}

//============================================================
//	�}�g���b�N�X�擾����
//============================================================
D3DXMATRIX CPlayer::GetMtxWorld(void) const
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans, mtxWorld;	// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X��Ԃ�
	return mtxWorld;
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CPlayer::GetPosition(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CPlayer::GetRotation(void) const
{
	// ������Ԃ�
	return m_rot;
}

//============================================================
//	�^�[�Q�b�g�Ƃ̋����擾����
//============================================================
float CPlayer::GetDistanceTarget(void) const
{
	// �^�[�Q�b�g�Ƃ̋�����Ԃ�
	return m_fDisTarget;
}

//============================================================
//	���a�擾����
//============================================================
float CPlayer::GetRadius(void) const
{
	// ���a��Ԃ�
	return PLAY_RADIUS;
}

//============================================================
//	�ړ�����
//============================================================
CPlayer::MOTION CPlayer::Move(MOTION motion)
{
	// �ϐ���錾
	D3DXVECTOR3			rot				= CManager::GetCamera()->GetRotation();		// �J�����̌���
	CStage::StageLimit	limit			= CManager::GetStage()->GetStageLimit();	// �X�e�[�W�͈̔�
	float				fAverageLimit	= (fabsf(limit.fLeft) + fabsf(limit.fRight) + fabsf(limit.fNear) + fabsf(limit.fFar)) * 0.25f;	// �͈͂̕���

	MOTION	currentMotion = motion;	// ���݂̃��[�V����
	int		nRotation;				// ��]����

	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetPad();		// �p�b�h

	// �ړ����[�V������ݒ�
	currentMotion = MOTION_MOVE;

	// �ړ��ʂ�ݒ�
	switch (m_rotation)
	{ // ��]�������Ƃ̏���
	case ROTATION_LEFT:

		// �ړ��ʂ��X�V
		m_move.x += sinf(rot.y - (D3DX_PI * 0.5f)) * (m_fDisTarget * (MAX_MOVEX / fAverageLimit));
		m_move.z += cosf(rot.y - (D3DX_PI * 0.5f)) * (m_fDisTarget * (MAX_MOVEX / fAverageLimit));

		// �ڕW�������X�V
		m_destRot.y = D3DXToRadian(90) + rot.y;

		// ����]��ݒ�
		nRotation = 1;

		break;

	case ROTATION_RIGHT:

		// �ړ��ʂ��X�V
		m_move.x -= sinf(rot.y - (D3DX_PI * 0.5f)) * (m_fDisTarget * (MAX_MOVEX / fAverageLimit));
		m_move.z -= cosf(rot.y - (D3DX_PI * 0.5f)) * (m_fDisTarget * (MAX_MOVEX / fAverageLimit));

		// �ڕW�������X�V
		m_destRot.y = D3DXToRadian(270) + rot.y;

		// �E��]��ݒ�
		nRotation = -1;

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �ړ�����
	if (pKeyboard->GetPress(DIK_W))
	{ // ���ړ��̑��삪�s��ꂽ�ꍇ

		// �ړ��ʂ��X�V
		m_move.x += sinf(rot.y) * PLAY_MOVEZ;
		m_move.z += cosf(rot.y) * PLAY_MOVEZ;

		// �ڕW�������X�V
		m_destRot.y = D3DXToRadian(180 - (PULSROT_MOVEZ * nRotation)) + rot.y;
	}
	else if (pKeyboard->GetPress(DIK_S))
	{ // ��O�ړ��̑��삪�s��ꂽ�ꍇ

		// �ړ��ʂ��X�V
		m_move.x -= sinf(rot.y) * PLAY_MOVEZ;
		m_move.z -= cosf(rot.y) * PLAY_MOVEZ;

		// �ڕW�������X�V
		m_destRot.y = D3DXToRadian(0 + (PULSROT_MOVEZ * nRotation)) + rot.y;
	}

	// �ړ��ʑ����E���]����
	else if (pKeyboard->GetPress(DIK_A))
	{ // ���ړ��̑��삪�s��ꂽ�ꍇ

		if (m_rotation == ROTATION_LEFT)
		{ // ��]���������̏ꍇ

			// �ړ��ʂ��X�V
			m_move.x += sinf(rot.y - (D3DX_PI * 0.5f)) * PLUS_MOVEX;
			m_move.z += cosf(rot.y - (D3DX_PI * 0.5f)) * PLUS_MOVEX;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(90) + rot.y;
		}
		else
		{ // ��]�������E�̏ꍇ

			// ��]���������ɂ���
			m_rotation = ROTATION_LEFT;
		}
	}
	else if (pKeyboard->GetPress(DIK_D))
	{ // �E�ړ��̑��삪�s��ꂽ�ꍇ

		if (m_rotation == ROTATION_RIGHT)
		{ // ��]�������E�̏ꍇ

			// �ړ��ʂ��X�V
			m_move.x -= sinf(rot.y - (D3DX_PI * 0.5f)) * PLUS_MOVEX;
			m_move.z -= cosf(rot.y - (D3DX_PI * 0.5f)) * PLUS_MOVEX;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(270) + rot.y;
		}
		else
		{ // ��]���������̏ꍇ

			// ��]�������E�ɂ���
			m_rotation = ROTATION_RIGHT;
		}
	}

	// ���݂̃��[�V������Ԃ�
	return currentMotion;		// TODO�F���͂�ҋ@���[�V��������Ȃ�
}

//============================================================
//	�W�����v����
//============================================================
CPlayer::MOTION CPlayer::Jump(MOTION motion)
{
	// �ϐ���錾
	MOTION currentMotion = motion;	// ���݂̃��[�V����

	// �|�C���^��錾
	CInputKeyboard	*pKeyboard = CManager::GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad = CManager::GetPad();				// �p�b�h

	// �W�����v����
	if (m_bJump == false)
	{ // �W�����v���Ă��Ȃ��ꍇ

		if (pKeyboard->GetTrigger(DIK_SPACE))
		{ // �W�����v�̑��삪�s��ꂽ�ꍇ

			// �W�����v���[�V������ݒ�
			currentMotion = MOTION_JUMP;

			// ��ړ���
			m_move.y += PLAY_JUMP;

			// �W�����v���Ă����Ԃɂ���
			m_bJump = true;
		}
	}

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//============================================================
//	���@����
//============================================================
CPlayer::MOTION CPlayer::Magic(MOTION motion)
{
	// �ϐ���錾
	MOTION currentMotion = motion;	// ���݂̃��[�V����

	// �|�C���^��錾
	CInputKeyboard	*pKeyboard = CManager::GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad = CManager::GetPad();				// �p�b�h

	// ���@�̃��b�N�I��
	m_pMagic->LockOnMagic(m_pos, D3DXVECTOR3(m_rot.x, m_rot.y + D3DX_PI, m_rot.z));

	if (pKeyboard->GetTrigger(DIK_RETURN))
	{ // ���@�̑��삪�s��ꂽ�ꍇ

		// �A�N�V�������[�V������ݒ�
		currentMotion = MOTION_ACTION;

		// ���@�̔���
		m_pMagic->ShotMagic();
	}

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//============================================================
//	���n����
//============================================================
CPlayer::MOTION CPlayer::Land(MOTION motion)
{
	// �ϐ���錾
	MOTION currentMotion = motion;	// ���݂̃��[�V����

	// ���n����
	if (CManager::GetField()->LandPosition(m_pos, m_move)
	||  CManager::GetStage()->LandPosition(m_pos, m_move, 0.0f))
	{ // �v���C���[�����n���Ă����ꍇ

		// ���n���[�V������ݒ�
		currentMotion = MOTION_LANDING;

		// �W�����v���Ă��Ȃ���Ԃɂ���
		m_bJump = false;
	}
	else
	{ // ���n���Ă��Ȃ��ꍇ

		// �W�����v���Ă����Ԃɂ���
		m_bJump = true;
	}

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//============================================================
//	���[�V��������
//============================================================
void CPlayer::Motion(MOTION motion)
{
	// �ϐ���錾
	MOTION animMotion = (MOTION)m_pMotion->GetType();	// ���ݍĐ����̃��[�V����

	if (m_pMotion->IsLoop(animMotion) == true)
	{ // ���[�v���郂�[�V�����������ꍇ

		if (m_pMotion->GetType() != motion)
		{ // ���݂̃��[�V�������Đ����̃��[�V�����ƈ�v���Ȃ��ꍇ

			// ���݂̃��[�V�����̐ݒ�
			m_pMotion->Set(motion);
		}
	}

	// ���[�V�����̍X�V
	m_pMotion->Update();

	// ���[�V�����̑J��
	if (m_pMotion->IsFinish())
	{ // ���[�V�������I�����Ă����ꍇ

		switch (m_pMotion->GetType())
		{ // ���[�V�����̎�ނ��Ƃ̏���
		case MOTION_ACTION:		// �A�N�V�������

			// �ҋ@���[�V�����Ɉڍs
			m_pMotion->Set(MOTION_NEUTRAL);

			// �����𔲂���
			break;

		case MOTION_JUMP:		// �W�����v���

			// ����

			// �����𔲂���
			break;

		case MOTION_LANDING:	// ���n���

			// �ҋ@���[�V�����Ɉڍs
			m_pMotion->Set(MOTION_NEUTRAL);

			// �����𔲂���
			break;
		}
	}
}

//============================================================
//	��������
//============================================================
void CPlayer::Rot(void)
{
	// �ϐ���錾
	float fDiffRot = 0.0f;	// ��������

	// �ڕW�����̐��K��
	useful::NormalizeRot(m_destRot.y);

	// �ڕW�����܂ł̍������v�Z
	fDiffRot = m_destRot.y - m_rot.y;

	// ���������̐��K��
	useful::NormalizeRot(fDiffRot);

	// �����̍X�V
	m_rot.y += fDiffRot * PLAY_REV_ROTA;

	// �����̐��K��
	useful::NormalizeRot(m_rot.y);
}

//============================================================
//	�J��������
//============================================================
void CPlayer::Camera(void)
{
	// �ϐ���錾
	D3DXVECTOR3 rot = CManager::GetCamera()->GetDestRotation();	// �J��������

	// �|�C���^��錾
	CInputKeyboard	*pKeyboard = CManager::GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad = CManager::GetPad();				// �p�b�h

	// �J��������
	if (pKeyboard->GetPress(DIK_J))
	{ // �J��������]�̑��삪�s��ꂽ�ꍇ

		// �J����������ύX
		rot.y -= PLAY_CAM_ROTA;
	}
	if (pKeyboard->GetPress(DIK_L))
	{ // �J�����E��]�̑��삪�s��ꂽ�ꍇ

		// �J����������ύX
		rot.y += PLAY_CAM_ROTA;
	}

	// �J�����̌�����ݒ�
	CManager::GetCamera()->SetDestRotation(rot);
}

//============================================================
//	�^�[�Q�b�g�Ƃ̓����蔻��
//============================================================
void CPlayer::CollisionTarget(void)
{
	// �|�C���^��錾
	CTarget *pTarget = CManager::GetTarget();	// �^�[�Q�b�g���

	if (USED(pTarget))
	{ // �^�[�Q�b�g���g�p����Ă���ꍇ

		// �^�[�Q�b�g�Ƃ̏Փ˔���
		collision::CirclePillar
		( // ����
			m_pos,					// ����ʒu
			pTarget->GetPosition(),	// ����ڕW�ʒu
			PLAY_RADIUS,			// ���蔼�a
			pTarget->GetRadius()	// ����ڕW���a
		);
	}
}

//============================================================
//	�G�Ƃ̓����蔻��
//============================================================
void CPlayer::CollisionEnemy(void)
{
	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
		{ // �I�u�W�F�N�g�̑������J��Ԃ�

			// �|�C���^��錾
			CObject *pObject = CObject::GetObject(nCntPri, nCntObject);	// �I�u�W�F�N�g

			if (UNUSED(pObject)
			||  pObject->GetLabel() != CObject::LABEL_ENEMY)
			{ // �I�u�W�F�N�g����g�p���E���x�����G�ł͂Ȃ��ꍇ

				// ���̌J��Ԃ��Ɉڍs
				continue;
			}

			// �^�[�Q�b�g�Ƃ̏Փ˔���
			collision::CirclePillar
			( // ����
				m_pos,					// ����ʒu
				pObject->GetPosition(),	// ����ڕW�ʒu
				PLAY_RADIUS,			// ���蔼�a
				pObject->GetRadius()	// ����ڕW���a
			);
		}
	}
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
void CPlayer::LoadSetup(void)
{
	// �ϐ���錾
	CMotion::MotionInfo info;		// �|�[�Y�̑���p
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 rot = VEC3_ZERO;	// �����̑���p
	int nID			= 0;	// �C���f�b�N�X�̑���p
	int nParentID	= 0;	// �e�C���f�b�N�X�̑���p
	int nNowPose	= 0;	// ���݂̃|�[�Y�ԍ�
	int nNowKey		= 0;	// ���݂̃L�[�ԍ�
	int nLoop		= 0;	// ���[�v��ON/OFF�̕ϊ��p
	int nEnd		= 0;	// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	int nModelID	= 0;	// ���f���C���f�b�N�X

	// �|�C���^��錾
	CModel *pModel = CManager::GetModel();	// ���f���ւ̃|�C���^

	// �|�[�Y����p�̕ϐ���������
	memset(&info, 0, sizeof(info));

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(PLAYER_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			// �L�����N�^�[�̐ݒ�
			if (strcmp(&aString[0], "CHARACTERSET") == 0)
			{ // �ǂݍ��񂾕����� CHARACTERSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_CHARACTERSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "PARTSSET") == 0)
					{ // �ǂݍ��񂾕����� PARTSSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_PARTSSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "INDEX") == 0)
							{ // �ǂݍ��񂾕����� INDEX �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nID);			// ���f���̃C���f�b�N�X��ǂݍ���
							}
							else if (strcmp(&aString[0], "PARENT") == 0)
							{ // �ǂݍ��񂾕����� PARENT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nParentID);	// ���f���̐e�̃C���f�b�N�X��ǂݍ���
							}
							else if (strcmp(&aString[0], "POS") == 0)
							{ // �ǂݍ��񂾕����� POS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &pos.x);		// X���W��ǂݍ���
								fscanf(pFile, "%f", &pos.y);		// Y���W��ǂݍ���
								fscanf(pFile, "%f", &pos.z);		// Z���W��ǂݍ���
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // �ǂݍ��񂾕����� ROT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &rot.x);		// X������ǂݍ���
								fscanf(pFile, "%f", &rot.y);		// Y������ǂݍ���
								fscanf(pFile, "%f", &rot.z);		// Z������ǂݍ���
							}
						} while (strcmp(&aString[0], "END_PARTSSET") != 0);	// �ǂݍ��񂾕����� END_PARTSSET �ł͂Ȃ��ꍇ���[�v

						// ���f���̐���
						m_apMultiModel[nID] = CMultiModel::Create(pos, rot);

						// ���f����o�^
						nModelID = pModel->Regist(mc_apModelFile[(MODEL)nID]);

						// ���f��������
						m_apMultiModel[nID]->BindModel(pModel->GetModel(nModelID));

						// �e���f���̐ݒ�
						if (nParentID == NONE_IDX)
						{ // �e���Ȃ��ꍇ

							// NULL��ݒ�
							m_apMultiModel[nID]->SetParent(NULL);
						}
						else
						{ // �e������ꍇ

							// �e�̃A�h���X��ݒ�
							m_apMultiModel[nID]->SetParent(m_apMultiModel[nParentID]);
						}

						// �p�[�c�̑��������Z
						m_nNumModel++;
					}
				} while (strcmp(&aString[0], "END_CHARACTERSET") != 0);		// �ǂݍ��񂾕����� END_CHARACTERSET �ł͂Ȃ��ꍇ���[�v
			}

			// ���[�V�����̐ݒ�
			else if (strcmp(&aString[0], "MOTIONSET") == 0)
			{ // �ǂݍ��񂾕����� MOTIONSET �̏ꍇ

				// ���݂̃|�[�Y�ԍ���������
				nNowPose = 0;

				do
				{ // �ǂݍ��񂾕����� END_MOTIONSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "LOOP") == 0)
					{ // �ǂݍ��񂾕����� LOOP �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nLoop);		// ���[�v��ON/OFF��ǂݍ���

						// �ǂݍ��񂾒l��bool�^�ɕϊ�
						info.bLoop = (nLoop == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "NUM_KEY") == 0)
					{ // �ǂݍ��񂾕����� NUM_KEY �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &info.nNumKey);	// �L�[�̑�����ǂݍ���
					}
					else if (strcmp(&aString[0], "KEYSET") == 0)
					{ // �ǂݍ��񂾕����� KEYSET �̏ꍇ

						// ���݂̃L�[�ԍ���������
						nNowKey = 0;

						do
						{ // �ǂݍ��񂾕����� END_KEYSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "FRAME") == 0)
							{ // �ǂݍ��񂾕����� FRAME �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &info.aKeyInfo[nNowPose].nFrame);	// �L�[���؂�ւ��܂ł̃t���[������ǂݍ���
							}
							else if (strcmp(&aString[0], "KEY") == 0)
							{ // �ǂݍ��񂾕����� KEY �̏ꍇ

								do
								{ // �ǂݍ��񂾕����� END_KEY �ł͂Ȃ��ꍇ���[�v

									// �t�@�C�����當�����ǂݍ���
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "POS") == 0)
									{ // �ǂݍ��񂾕����� POS �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);									// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.x);	// X�ʒu��ǂݍ���
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.y);	// Y�ʒu��ǂݍ���
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.z);	// Z�ʒu��ǂݍ���

										// �ǂݍ��񂾈ʒu�Ƀp�[�c�̏����ʒu�����Z
										info.aKeyInfo[nNowPose].aKey[nNowKey].pos += m_apMultiModel[nNowKey]->GetPosition();
									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{ // �ǂݍ��񂾕����� ROT �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);									// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);	// X������ǂݍ���
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);	// Y������ǂݍ���
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);	// Z������ǂݍ���

										// �ǂݍ��񂾌����Ƀp�[�c�̏������������Z
										info.aKeyInfo[nNowPose].aKey[nNowKey].rot += m_apMultiModel[nNowKey]->GetRotation();
									}

								} while (strcmp(&aString[0], "END_KEY") != 0);	// �ǂݍ��񂾕����� END_KEY �ł͂Ȃ��ꍇ���[�v

								// ���݂̃L�[�ԍ������Z
								nNowKey++;
							}
						} while (strcmp(&aString[0], "END_KEYSET") != 0);	// �ǂݍ��񂾕����� END_KEYSET �ł͂Ȃ��ꍇ���[�v

						// ���݂̃|�[�Y�ԍ������Z
						nNowPose++;
					}
				} while (strcmp(&aString[0], "END_MOTIONSET") != 0);	// �ǂݍ��񂾕����� END_MOTIONSET �ł͂Ȃ��ꍇ���[�v

				// ���[�V�������̐ݒ�
				m_pMotion->SetInfo(info);
			}

		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�v���C���[�Z�b�g�A�b�v�t�@�C���̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}
