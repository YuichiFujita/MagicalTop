//============================================================
//
//	�Q�[���v���C���[���� [playerGame.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "playerGame.h"
#include "manager.h"
#include "camera.h"
#include "sound.h"
#include "collision.h"
#include "stage.h"
#include "levelupManager.h"

//************************************************************
//	�}�N����`
//************************************************************
#define HURRICANE_DMG	(50)	// �n���P�[���̃_���[�W��

//************************************************************
//	�q�N���X [CPlayerGame] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPlayerGame::CPlayerGame()
{
	// �����o�ϐ����N���A
	m_pLevelup = NULL;	// �����}�l�[�W���[�̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CPlayerGame::~CPlayerGame()
{

}

//============================================================
//	����������
//============================================================
HRESULT CPlayerGame::Init(void)
{
	// �����o�ϐ���������
	m_pLevelup = NULL;	// �����}�l�[�W���[�̏��

	// �����}�l�[�W���[�̐���
	m_pLevelup = CLevelupManager::Create();
	if (UNUSED(m_pLevelup))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

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
void CPlayerGame::Uninit(void)
{
	// �����}�l�[�W���[��j��
	if (FAILED(m_pLevelup->Release(m_pLevelup)))
	{ // �j���Ɏ��s�����ꍇ

		// ��O����
		assert(false);
	}

	// �v���C���[�̏I��
	CPlayer::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CPlayerGame::Update(void)
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

		// ���݂̃��[�V�����Ɏ��S���[�V������ݒ�
		nCurrentMotion = CPlayer::MOTION_DEATH;

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �v���C���[�̍X�V
	CPlayer::Update();

	// �����}�l�[�W���[�̍X�V
	m_pLevelup->Update();

	// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V
	UpdateMotion(nCurrentMotion);
}

//============================================================
//	������уq�b�g����
//============================================================
void CPlayerGame::HitBlowAway
(
	const D3DXVECTOR3& rPlayerPos,	// �v���C���[�ʒu
	const D3DXVECTOR3& rHitObjPos,	// �I�u�W�F�N�g�ʒu
	const int nDmg					// �_���[�W��
)
{
	// �ϐ���錾
	D3DXVECTOR3 vecAway = VEC3_ZERO;	// ������уx�N�g��
	STATE oldState = (STATE)GetState();	// �ߋ��̏��

	// �q�b�g����
	Hit(nDmg);

	if (oldState   == STATE_NORMAL
	&&  GetState() == STATE_DAMAGE)
	{ // ���̏�Ԃ��ʏ��Ԋ��A���݂̏�Ԃ��_���[�W��Ԃ̏ꍇ

		// ������уx�N�g�������߂�
		vecAway = rPlayerPos - rHitObjPos;
		vecAway.y = 0.0f;						// �c�x�N�g�����폜
		D3DXVec3Normalize(&vecAway, &vecAway);	// �x�N�g���𐳋K��

		// ������т̐ݒ�
		SetBlowAway(vecAway);

		// ��Ԃ�ύX
		SetState(STATE_BLOW_AWAY);	// ������я��

		// ������у��[�V�����Ɉڍs
		SetMotion(MOTION_BLOW_AWAY);
	}
}

//============================================================
//	�Q�������܂�q�b�g����
//============================================================
void CPlayerGame::HitVortex
(
	D3DXVECTOR3& rPlayerPos,	// �v���C���[�ʒu
	const D3DXVECTOR3& rHitPos,	// �����蔻��ʒu
	const int nDmg				// �_���[�W��
)
{
	// �o���A�̏Փ˔���
	collision::CirclePillar
	( // ����
		rPlayerPos,										// ����ʒu
		CScene::GetStage()->GetStageBarrierPosition(),	// ����ڕW�ʒu
		GetRadius(),									// ���蔼�a
		CScene::GetStage()->GetStageBarrier().fRadius	// ����ڕW���a
	);

	if (GetState() == STATE_NORMAL)
	{ // �ʏ��Ԃ̏ꍇ

		// �q�b�g����
		Hit(nDmg);

		if (GetState() != STATE_DEATH)
		{ // ���S��Ԃ̏ꍇ

			// �Q�������܂�̐ݒ菈��
			SetVortex(rPlayerPos, rHitPos);

			// �J�����X�V��OFF�ɂ���
			CManager::GetCamera()->SetEnableUpdate(false);

			// ��Ԃ�ݒ�
			SetState(STATE_VORTEX);	// �Q�������܂���

			// ������у��[�V�����Ɉڍs
			SetMotion(MOTION_BLOW_AWAY);

			// �T�E���h�̍Đ�
			CManager::GetSound()->Play(CSound::LABEL_SE_HURRICANE);	// �n���P�[���������܂ꉹ
		}
	}
}

//============================================================
//	�\���̐ݒ菈��
//============================================================
void CPlayerGame::SetDisp(const bool bDisp)
{
	if (bDisp)
	{ // �\�������Ԃ̏ꍇ

		// ��Ԃ�ݒ�
		SetState(STATE_FADEOUT);	// �t�F�[�h�A�E�g���

		// �`�悷��ݒ�ɂ���
		SetEnableDraw(true);
	}
	else
	{ // �\�����Ȃ���Ԃ̏ꍇ

		// ��Ԃ�ݒ�
		SetState(STATE_FADEIN);		// �t�F�[�h�C�����

		// �`�悵�Ȃ��ݒ�ɂ���
		SetEnableDraw(false);
		CObject::SetEnableDraw(true);	// �v���C���[�̕`���ON�ɂ���
	}
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CPlayerGame::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	CPlayer::SetEnableDraw(bDraw);	// ���g
	SetEnableDrawMana(bDraw);		// �}�i
	SetEnableDrawLife(bDraw);		// �̗�
	SetEnableDrawDash(bDraw);		// �_�b�V��
}

//============================================================
//	�ʏ��Ԏ��̍X�V����
//============================================================
CPlayer::MOTION CPlayerGame::UpdateNormal(void)
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
			HitVortex(posPlayer, pStage->GetStageBarrierPosition(), HURRICANE_DMG);
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

//============================================================
//	�ړ��ʁE�ڕW�����̍X�V����
//============================================================
CPlayer::MOTION CPlayerGame::UpdateMove(void)
{
	// �ϐ���錾
	MOTION currentMotion = MOTION_MOVE;	// ���݂̃��[�V����
	D3DXVECTOR3 vecTarg = VEC3_ZERO;	// �^�[�Q�b�g�t�����x�N�g��
	D3DXVECTOR3 vecSide = VEC3_ZERO;	// �^�[�Q�b�g�������x�N�g��

	// �z�����݂̍X�V
	UpdateAbsorb(vecTarg, vecSide);

	// �^�[�Q�b�g�t�����ւ̉������̑���
	currentMotion = ControlTargAccel(vecTarg);

	// �^�[�Q�b�g�������ւ̉������̑���
	currentMotion = ControlSideAccel(vecSide);

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}
