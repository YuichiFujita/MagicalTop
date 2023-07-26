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
#include "sceneGame.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "collision.h"

#include "magicManager.h"
#include "objectGauge2D.h"
#include "shadow.h"
#include "target.h"
#include "stage.h"
#include "field.h"
#include "particle3D.h"

//************************************************************
//	�}�N����`
//************************************************************
#define PLAYER_SETUP_TXT	"data\\TXT\\player.txt"	// �Z�b�g�A�b�v�e�L�X�g���΃p�X

#define PLAY_SHADOW_SIZE	(D3DXVECTOR3(80.0f, 0.0f, 80.0f))	// �e�̑傫��

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
CPlayer::CPlayer() : CObjectChara(CObject::LABEL_PLAYER)
{
	// �����o�ϐ����N���A
	m_pMagic		= NULL;				// ���@�}�l�[�W���[�̏��
	m_pLife			= NULL;				// �̗͂̏��
	m_pShadow		= NULL;				// �e�̏��
	m_oldPos		= VEC3_ZERO;		// �ߋ��ʒu
	m_move			= VEC3_ZERO;		// �ړ���
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
	m_pMagic		= NULL;				// ���@�}�l�[�W���[�̏��
	m_pLife			= NULL;				// �̗͂̏��
	m_pShadow		= NULL;				// �e�̏��
	m_oldPos		= VEC3_ZERO;		// �ߋ��ʒu
	m_move			= VEC3_ZERO;		// �ړ���
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

	// �̗͂̐���
	m_pLife = CObjectGauge2D::Create
	( // ����
		CObject::LABEL_LIFE,
		100,
		60,
		D3DXVECTOR3(260.0f, 640.0f, 0.0f),
		D3DXVECTOR3(200.0f, 30.0f, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)
	);
	if (UNUSED(m_pLife))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�̐���
	m_pShadow = CShadow::Create(CShadow::TEXTURE_NORMAL, PLAY_SHADOW_SIZE, this);
	if (UNUSED(m_pShadow))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �I�u�W�F�N�g�L�����N�^�[�̏�����
	if (FAILED(CObjectChara::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �Z�b�g�A�b�v�̓ǂݍ���
	LoadSetup();

	// ���f�����̐ݒ�
	SetModelInfo();

	// ���[�V�����̐ݒ�
	SetMotion(MOTION_NEUTRAL);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CPlayer::Uninit(void)
{
	// ���@�}�l�[�W���[��j��
	if (FAILED(m_pMagic->Release(m_pMagic)))
	{ // �j���Ɏ��s�����ꍇ

		// ��O����
		assert(false);
	}

	// �e��j��
	m_pShadow->Uninit();

	// �I�u�W�F�N�g�L�����N�^�[�̏I��
	CObjectChara::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CPlayer::Update(void)
{
	// �ϐ���錾
	MOTION currentMotion  = MOTION_NEUTRAL;	// ���݂̃��[�V����
	D3DXVECTOR3 posPlayer = GetPosition();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetRotation();	// �v���C���[����
	D3DXVECTOR3 posTarget = VEC3_ZERO/*CManager::GetTarget()->GetPosition()*/;	// �^�[�Q�b�g�ʒu	// TODO�F�^�[�Q�b�g�ʒu�̎擾

	// �ߋ��ʒu���X�V
	m_oldPos = posPlayer;

	// �^�[�Q�b�g�Ƃ̋�����ݒ�
	m_fDisTarget = sqrtf((posPlayer.x - posTarget.x) * (posPlayer.x - posTarget.x)+ (posPlayer.z - posTarget.z) * (posPlayer.z - posTarget.z)) * 0.5f;

	// �ړ�����
	currentMotion = Move(currentMotion);

	// �����X�V
	Rot(rotPlayer);

	// �W�����v����
	currentMotion = Jump(currentMotion);

	// �ʒu�X�V
	Pos(posPlayer);

	// �����蔻��
	CollisionTarget(posPlayer);	// �^�[�Q�b�g
	CollisionEnemy(posPlayer);	// �G

	// �X�e�[�W�͈͊O�̕␳
	CSceneGame::GetStage()->LimitPosition(posPlayer, PLAY_RADIUS);

	// ���n����
	if (Land(currentMotion, posPlayer) == MOTION_LANDING)
	{ // ���n���Ă����ꍇ

		if (GetMotionType() == MOTION_JUMP)
		{ // �Đ������[�V�������W�����v�������ꍇ

			// ���n���[�V�����̐ݒ�
			SetMotion(MOTION_LANDING);
		}
	}

	// �ˌ�����
	currentMotion = Magic(currentMotion, posPlayer);

	// �ʒu���X�V
	SetPosition(posPlayer);

	// �������X�V
	SetRotation(rotPlayer);

	// �e�̍X�V
	m_pShadow->Update();

	// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V
	Motion(currentMotion);
}

//============================================================
//	�`�揈��
//============================================================
void CPlayer::Draw(void)
{
	// �I�u�W�F�N�g�L�����N�^�[�̕`��
	CObjectChara::Draw();
}

//============================================================
//	�q�b�g����
//============================================================
void CPlayer::Hit(const int nDmg)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = GetPosition();	// �v���C���[�ʒu

	if (IsDeath() != true)
	{ // ���S�t���O�������Ă��Ȃ��ꍇ

		// �̗͂���_���[�W�����Z
		m_pLife->AddNum(-nDmg);

		if (m_pLife->GetNum() > 0)
		{ // �����Ă���ꍇ

			// �p�[�e�B�N��3D�I�u�W�F�N�g�𐶐�
			CParticle3D::Create(CParticle3D::TYPE_DAMAGE, pos);
		}
		else
		{ // ����ł���ꍇ

			// �p�[�e�B�N��3D�I�u�W�F�N�g�𐶐�
			CParticle3D::Create(CParticle3D::TYPE_DAMAGE, pos, D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f));
			CParticle3D::Create(CParticle3D::TYPE_DAMAGE, pos, D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f));

			// �X�V�ƕ`����~
			SetEnableUpdate(false);
			SetEnableDraw(false);

			// ���@���b�N�I���S�폜
			m_pMagic->DeleteLockOn();
		}
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
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�X�V�󋵂̐ݒ菈��
//============================================================
void CPlayer::SetEnableUpdate(const bool bUpdate)
{
	// �����̍X�V�󋵂�ݒ�
	CObject::SetEnableUpdate(bUpdate);		// ���g
	m_pShadow->SetEnableUpdate(bUpdate);	// �e
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CPlayer::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	CObject::SetEnableDraw(bDraw);		// ���g
	m_pShadow->SetEnableDraw(bDraw);	// �e
}

//============================================================
//	�}�g���b�N�X�擾����
//============================================================
D3DXMATRIX CPlayer::GetMtxWorld(void) const
{
	// �ϐ���錾
	D3DXMATRIX  mtxRot, mtxTrans, mtxWorld;	// �v�Z�p�}�g���b�N�X
	D3DXVECTOR3 posPlayer = GetPosition();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetRotation();	// �v���C���[����

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotPlayer.y, rotPlayer.x, rotPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, posPlayer.x, posPlayer.y, posPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X��Ԃ�
	return mtxWorld;
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
	D3DXVECTOR3 rot = CManager::GetCamera()->GetRotation();			// �J�����̌���
	float fLimit = CSceneGame::GetStage()->GetStageLimit().fRadius;	// �X�e�[�W�͈�

	MOTION	currentMotion = motion;	// ���݂̃��[�V����
	int		nRotation;				// ��]����

	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetPad();		// �p�b�h

#if 1	// �ʏ���s
	// �ړ����[�V������ݒ�
	currentMotion = MOTION_MOVE;

	// �ړ��ʂ�ݒ�
	switch (m_rotation)
	{ // ��]�������Ƃ̏���
	case ROTATION_LEFT:

		// �ړ��ʂ��X�V
		m_move.x += sinf(rot.y - (D3DX_PI * 0.5f)) * (m_fDisTarget * (MAX_MOVEX / fLimit));
		m_move.z += cosf(rot.y - (D3DX_PI * 0.5f)) * (m_fDisTarget * (MAX_MOVEX / fLimit));

		// �ڕW�������X�V
		m_destRot.y = D3DXToRadian(90) + rot.y;

		// ����]��ݒ�
		nRotation = 1;

		break;

	case ROTATION_RIGHT:

		// �ړ��ʂ��X�V
		m_move.x -= sinf(rot.y - (D3DX_PI * 0.5f)) * (m_fDisTarget * (MAX_MOVEX / fLimit));
		m_move.z -= cosf(rot.y - (D3DX_PI * 0.5f)) * (m_fDisTarget * (MAX_MOVEX / fLimit));

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
#else	// �f�o�b�O�p���s
	// �ړ�����
	if (pKeyboard->GetPress(DIK_W))
	{ // ���ړ��̑��삪�s��ꂽ�ꍇ

		// �ړ����[�V������ݒ�
		currentMotion = MOTION_MOVE;

		if (pKeyboard->GetPress(DIK_A))
		{ // ���ړ��̑�����s��ꂽ�ꍇ (�����ړ�)

			// �ړ��ʂ��X�V
			m_move.x += sinf(rot.y - (D3DX_PI * 0.25f)) * 5.0f;
			m_move.z += cosf(rot.y - (D3DX_PI * 0.25f)) * 5.0f;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(135) + rot.y;
		}
		else if (pKeyboard->GetPress(DIK_D))
		{ // �E�ړ��̑�����s��ꂽ�ꍇ (�E���ړ�)

			// �ړ��ʂ��X�V
			m_move.x -= sinf(rot.y - (D3DX_PI * 0.75f)) * 5.0f;
			m_move.z -= cosf(rot.y - (D3DX_PI * 0.75f)) * 5.0f;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(225) + rot.y;
		}
		else
		{ // ���ړ��̑��삾�����s��ꂽ�ꍇ (���ړ�)

			// �ړ��ʂ��X�V
			m_move.x += sinf(rot.y) * 5.0f;
			m_move.z += cosf(rot.y) * 5.0f;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(180) + rot.y;
		}
	}
	else if (pKeyboard->GetPress(DIK_S))
	{ // ��O�ړ��̑��삪�s��ꂽ�ꍇ

		// �ړ����[�V������ݒ�
		currentMotion = MOTION_MOVE;

		if (pKeyboard->GetPress(DIK_A))
		{ // ���ړ��̑�����s��ꂽ�ꍇ (����O�ړ�)

			// �ړ��ʂ��X�V
			m_move.x += sinf(rot.y - (D3DX_PI * 0.75f)) * 5.0f;
			m_move.z += cosf(rot.y - (D3DX_PI * 0.75f)) * 5.0f;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(45) + rot.y;
		}
		else if (pKeyboard->GetPress(DIK_D))
		{ // �E�ړ��̑�����s��ꂽ�ꍇ (�E��O�ړ�)

			// �ړ��ʂ��X�V
			m_move.x -= sinf(rot.y - (D3DX_PI * 0.25f)) * 5.0f;
			m_move.z -= cosf(rot.y - (D3DX_PI * 0.25f)) * 5.0f;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(315) + rot.y;
		}
		else
		{ // ��O�ړ��̑��삾�����s��ꂽ�ꍇ (��O�ړ�)

			// �ړ��ʂ��X�V
			m_move.x -= sinf(rot.y) * 5.0f;
			m_move.z -= cosf(rot.y) * 5.0f;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(0) + rot.y;
		}
	}
	else if (pKeyboard->GetPress(DIK_A))
	{ // ���ړ��̑��삪�s��ꂽ�ꍇ

		// �ړ����[�V������ݒ�
		currentMotion = MOTION_MOVE;

		// �ړ��ʂ��X�V
		m_move.x += sinf(rot.y - (D3DX_PI * 0.5f)) * 5.0f;
		m_move.z += cosf(rot.y - (D3DX_PI * 0.5f)) * 5.0f;

		// �ڕW�������X�V
		m_destRot.y = D3DXToRadian(90) + rot.y;
	}
	else if (pKeyboard->GetPress(DIK_D))
	{ // �E�ړ��̑��삪�s��ꂽ�ꍇ

		// �ړ����[�V������ݒ�
		currentMotion = MOTION_MOVE;

		// �ړ��ʂ��X�V
		m_move.x -= sinf(rot.y - (D3DX_PI * 0.5f)) * 5.0f;
		m_move.z -= cosf(rot.y - (D3DX_PI * 0.5f)) * 5.0f;

		// �ڕW�������X�V
		m_destRot.y = D3DXToRadian(270) + rot.y;
	}
#endif

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
CPlayer::MOTION CPlayer::Magic(MOTION motion, D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	MOTION currentMotion = motion;	// ���݂̃��[�V����

	// �|�C���^��錾
	CInputKeyboard	*pKeyboard = CManager::GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad = CManager::GetPad();				// �p�b�h

	// ���@�̃��b�N�I��
	m_pMagic->LockOnMagic(rPos);

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
CPlayer::MOTION CPlayer::Land(MOTION motion, D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	MOTION currentMotion  = motion;			// ���݂̃��[�V����

	// ���n����
	if (CSceneGame::GetField()->LandPosition(rPos, m_move)
	||  CSceneGame::GetStage()->LandPosition(rPos, m_move, 0.0f))
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
	MOTION animMotion = (MOTION)GetMotionType();	// ���ݍĐ����̃��[�V����

	if (IsMotionLoop(animMotion) == true)
	{ // ���[�v���郂�[�V�����������ꍇ

		if (animMotion != motion)
		{ // ���݂̃��[�V�������Đ����̃��[�V�����ƈ�v���Ȃ��ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(motion);
		}
	}

	// �I�u�W�F�N�g�L�����N�^�[�̍X�V
	CObjectChara::Update();

	// ���[�V�����̑J��
	if (IsMotionFinish())
	{ // ���[�V�������I�����Ă����ꍇ

		switch (GetMotionType())
		{ // ���[�V�����̎�ނ��Ƃ̏���
		case MOTION_ACTION:		// �A�N�V�������

			// �ҋ@���[�V�����Ɉڍs
			SetMotion(MOTION_NEUTRAL);

			// �����𔲂���
			break;

		case MOTION_JUMP:		// �W�����v���

			// ����

			// �����𔲂���
			break;

		case MOTION_LANDING:	// ���n���

			// �ҋ@���[�V�����Ɉڍs
			SetMotion(MOTION_NEUTRAL);

			// �����𔲂���
			break;
		}
	}
}

//============================================================
//	�ʒu����
//============================================================
void CPlayer::Pos(D3DXVECTOR3& rPos)
{
	// �d�͂����Z
	m_move.y -= PLAY_GRAVITY;

	// �ړ��ʂ����Z
	rPos += m_move;

	// �ړ��ʂ�����
	m_move.x += (0.0f - m_move.x) * PLAY_REV;
	m_move.z += (0.0f - m_move.z) * PLAY_REV;
}

//============================================================
//	��������
//============================================================
void CPlayer::Rot(D3DXVECTOR3& rRot)
{
	// �ϐ���錾
	float fDiffRot = 0.0f;	// ��������

	// �ڕW�����̐��K��
	useful::NormalizeRot(m_destRot.y);

	// �ڕW�����܂ł̍������v�Z
	fDiffRot = m_destRot.y - rRot.y;

	// ���������̐��K��
	useful::NormalizeRot(fDiffRot);

	// �����̍X�V
	rRot.y += fDiffRot * PLAY_REV_ROTA;

	// �����̐��K��
	useful::NormalizeRot(rRot.y);
}

//============================================================
//	�^�[�Q�b�g�Ƃ̓����蔻��
//============================================================
void CPlayer::CollisionTarget(D3DXVECTOR3& rPos)
{
	// �|�C���^��錾
	CTarget *pTarget = CSceneGame::GetTarget();	// �^�[�Q�b�g���

	if (USED(pTarget))
	{ // �^�[�Q�b�g���g�p����Ă���ꍇ

		// �^�[�Q�b�g�Ƃ̏Փ˔���
		collision::CirclePillar
		( // ����
			rPos,					// ����ʒu
			pTarget->GetPosition(),	// ����ڕW�ʒu
			PLAY_RADIUS,			// ���蔼�a
			pTarget->GetRadius()	// ����ڕW���a
		);
	}
}

//============================================================
//	�G�Ƃ̓����蔻��
//============================================================
void CPlayer::CollisionEnemy(D3DXVECTOR3& rPos)
{
	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		// �|�C���^��錾
		CObject *pObjectTop = CObject::GetTop(nCntPri);	// �擪�I�u�W�F�N�g

		if (USED(pObjectTop))
		{ // �擪�����݂���ꍇ

			// �|�C���^��錾
			CObject *pObjCheck = pObjectTop;	// �I�u�W�F�N�g�m�F�p

			while (USED(pObjCheck))
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

				// �|�C���^��錾
				CObject *pObjectNext = pObjCheck->GetNext();	// ���I�u�W�F�N�g

				if (pObjCheck->GetLabel() != CObject::LABEL_ENEMY)
				{ // �I�u�W�F�N�g���x�����G�ł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// �G�Ƃ̏Փ˔���
				collision::CirclePillar
				( // ����
					rPos,						// ����ʒu
					pObjCheck->GetPosition(),	// ����ڕW�ʒu
					PLAY_RADIUS,				// ���蔼�a
					pObjCheck->GetRadius()		// ����ڕW���a
				);

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
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

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	// �|�[�Y����p�̕ϐ���������
	memset(&info, 0, sizeof(info));

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

						// �p�[�c���̐ݒ�
						CObjectChara::SetPartsInfo(nID, nParentID, pos, rot, mc_apModelFile[nID]);
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
										info.aKeyInfo[nNowPose].aKey[nNowKey].pos += GetPartsPosition(nNowKey);
									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{ // �ǂݍ��񂾕����� ROT �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);									// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);	// X������ǂݍ���
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);	// Y������ǂݍ���
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);	// Z������ǂݍ���

										// �ǂݍ��񂾌����Ƀp�[�c�̏������������Z
										info.aKeyInfo[nNowPose].aKey[nNowKey].rot += GetPartsRotation(nNowKey);

										// ���������𐳋K��
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);
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
				CObjectChara::SetMotionInfo(info);
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
