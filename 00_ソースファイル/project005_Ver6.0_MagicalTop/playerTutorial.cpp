//============================================================
//
//	�`���[�g���A���v���C���[���� [playerTutorial.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "playerTutorial.h"
#include "stage.h"

//************************************************************
//	�q�N���X [CPlayerTutorial] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPlayerTutorial::CPlayerTutorial()
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CPlayerTutorial::~CPlayerTutorial()
{

}

//============================================================
//	����������
//============================================================
HRESULT CPlayerTutorial::Init(void)
{
	// �v���C���[�̏�����
	if (FAILED(CPlayer::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CPlayerTutorial::Uninit(void)
{
	// �v���C���[�̏I��
	CPlayer::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CPlayerTutorial::Update(void)
{
	// �ϐ���錾
	int nCurrentMotion = NONE_IDX;	// ���݂̃��[�V����

	// �ߋ��ʒu�̍X�V
	UpdateOldPosition();

	// �^�[�Q�b�g�����̍X�V
	UpdateDisTarget();

	switch (GetState())
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:		// �������Ȃ����

		// ����

		break;

	case STATE_FADEOUT:		// �t�F�[�h�A�E�g���

		// �t�F�[�h�A�E�g��Ԏ��̍X�V
		UpdateFadeOut();

		break;

	case STATE_NORMAL:		// �ʏ���

		// �ʏ��Ԏ��̍X�V
		nCurrentMotion = UpdateNormal();

		break;

	case STATE_DAMAGE:		// �_���[�W���

		// �_���[�W��Ԏ��̍X�V
		nCurrentMotion = UpdateDamage();

		break;

	case STATE_INVULN:		// ���G���

		// ���G��Ԏ��̍X�V
		nCurrentMotion = UpdateInvuln();

		break;

	case STATE_BLOW_AWAY:	// ������я��

		// ������я�Ԏ��̍X�V
		UpdateBlowAway();

		break;

	case STATE_VORTEX:		// �Q�������܂���

		// �Q�������܂��Ԏ��̍X�V
		UpdateVortex();

		break;

	case STATE_FADEIN:		// �t�F�[�h�C�����

		// �t�F�[�h�C����Ԏ��̍X�V
		UpdateFadeIn();

		break;

	case STATE_DEATH:		// ���S���

		// ����

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �v���C���[�̍X�V
	CPlayer::Update();

	// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V
	UpdateMotion(nCurrentMotion);
}

//============================================================
//	�ʏ��Ԏ��̍X�V����
//============================================================
CPlayer::MOTION CPlayerTutorial::UpdateNormal(void)
{
	// �ϐ���錾
	MOTION currentMotion = MOTION_MOVE;		// ���݂̃��[�V����
	D3DXVECTOR3 posPlayer = GetPosition();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetRotation();	// �v���C���[����
	float fRadiusPlayer = GetRadius();		// �v���C���[���a

	// �|�C���^��錾
	CStage *pStage = CScene::GetStage();	// �X�e�[�W���

	if (USED(pStage))
	{ // �X�e�[�W���g�p����Ă���ꍇ

		// �ړ�����
		currentMotion = UpdateMove();

		// �����X�V
		UpdateRotation(rotPlayer);

		// �ʒu�X�V
		UpdatePosition(posPlayer);

		// �X�e�[�W�͈͊O�̕␳
		pStage->LimitPosition(posPlayer, fRadiusPlayer);

		// ���n����
		UpdateLanding(posPlayer);

		// ���@���ˑ���
		currentMotion = ControlShotMagic(currentMotion);

		// �����蔻��
		CollisionTarget(posPlayer);	// �^�[�Q�b�g
		CollisionEnemy(posPlayer);	// �G

		// �o���A�Ƃ̓����蔻��
		if (pStage->CollisionBarrier(posPlayer, fRadiusPlayer))
		{ // �������Ă����ꍇ

			// �Q�������܂�q�b�g
			HitVortex(posPlayer, pStage->GetStageBarrierPosition());
		}

		// �ʒu���X�V
		SetPosition(posPlayer);

		// �������X�V
		SetRotation(rotPlayer);
	}
	else { assert(false); }	// ��g�p��

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}
