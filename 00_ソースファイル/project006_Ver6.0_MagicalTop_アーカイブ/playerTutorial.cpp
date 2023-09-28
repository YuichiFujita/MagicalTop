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
#include "sceneTutorial.h"
#include "tutorialManager.h"
#include "manager.h"
#include "camera.h"
#include "sound.h"
#include "collision.h"
#include "stage.h"
#include "enemy.h"

//************************************************************
//	�q�N���X [CPlayerTutorial] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPlayerTutorial::CPlayerTutorial()
{
	// �����o�ϐ����N���A
	m_nOldNumEnemy = 0;	// �ߋ��̓G�̑���
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
	// �����o�ϐ���������
	m_nOldNumEnemy = CTutorialManager::GetNextLessonCounter(CTutorialManager::LESSON_06);	// �ߋ��̓G�̑���

	// �v���C���[�̏�����
	if (FAILED(CPlayer::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �o���l�̕`����~
	SetEnableDrawExp(false);

	// �_�b�V���̕`����~
	SetEnableDrawDash(false);

	// �}�i�̕`����~
	SetEnableDrawMana(false);

	// �}�i�̉񕜂��ł��Ȃ��悤�ɕύX
	SetEnableHealMana(false);

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
	int nNumEnemy = 0;	// ���݂̓G�̑���
	int nOldMana = 0;	// �񕜑O�̃}�i

	if (CSceneTutorial::GetTutorialManager()->GetLesson() == CTutorialManager::LESSON_06
	&&  CSceneTutorial::GetTutorialManager()->GetState()  == CTutorialManager::STATE_PROGRESSION)
	{ // �`���[�g���A�����i�s��Ԋ��A���b�X��06�F�G�ւ̍U���̏ꍇ

		// ���݂̓G�̑������擾
		nNumEnemy = CEnemy::GetNumAll();

		if (nNumEnemy < m_nOldNumEnemy)
		{ // �G��|���Ă���ꍇ

			for (int nCntPlayer = 0; nCntPlayer < m_nOldNumEnemy - nNumEnemy; nCntPlayer++)
			{ // ���t���[���ɓ|�����G�̐����J��Ԃ�

				// ���b�X���J�E���^�[���Z
				CSceneTutorial::GetTutorialManager()->AddLessonCounter();
			}
		}

		// �ߋ��̓G�̑������X�V
		m_nOldNumEnemy = nNumEnemy;
	}

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

		if (CSceneTutorial::GetTutorialManager()->GetLesson() == CTutorialManager::LESSON_01)
		{ // ���b�X��01�F�z�����܂��̏ꍇ

			// ���b�X���J�E���^�[���Z
			CSceneTutorial::GetTutorialManager()->AddLessonCounter();
		}

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

	// �񕜑O�̃}�i��ݒ�
	nOldMana = GetMana();

	// �v���C���[�̍X�V
	CPlayer::Update();

	if (CSceneTutorial::GetTutorialManager()->GetLesson() == CTutorialManager::LESSON_05)
	{ // ���b�X��05�F�}�i�񕜂̏ꍇ

		if (GetMana() > nOldMana)
		{ // �}�i���񕜂ł��Ă���ꍇ

			// ���b�X���J�E���^�[���Z
			CSceneTutorial::GetTutorialManager()->AddLessonCounter();
		}
	}

	// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V
	UpdateMotion(nCurrentMotion);
}

//============================================================
//	������уq�b�g����
//============================================================
void CPlayerTutorial::HitBlowAway
(
	const D3DXVECTOR3& rPlayerPos,	// �v���C���[�ʒu
	const D3DXVECTOR3& rHitObjPos,	// �I�u�W�F�N�g�ʒu
	const int nDmg					// �_���[�W��
)
{
	// �ϐ���錾
	D3DXVECTOR3 vecAway = VEC3_ZERO;	// ������уx�N�g��

	if (GetState() == STATE_NORMAL)
	{ // �ʏ��Ԃ̏ꍇ

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
void CPlayerTutorial::HitVortex
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

//============================================================
//	�\���̐ݒ菈��
//============================================================
void CPlayerTutorial::SetDisp(const bool bDisp)
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
void CPlayerTutorial::SetEnableDraw(const bool bDraw)
{
	// �ϐ���錾
	bool bMana = (CSceneTutorial::GetTutorialManager()->GetLesson() >= CTutorialManager::LESSON_04) ? bDraw : false;	// �}�i�̕`���
	bool bDash = (CSceneTutorial::GetTutorialManager()->GetLesson() >= CTutorialManager::LESSON_03) ? bDraw : false;	// �_�b�V���̕`���

	// �����̕`��󋵂�ݒ�
	CPlayer::SetEnableDraw(bDraw);	// ���g
	SetEnableDrawLife(bDraw);		// �̗�

	// ���b�X���ɉ������`��󋵂�ݒ�
	SetEnableDrawMana(bMana);	// �}�i
	SetEnableDrawDash(bDash);	// �_�b�V��
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
		if (CSceneTutorial::GetTutorialManager()->GetLesson() > CTutorialManager::LESSON_03)
		{ // ���b�X��03�F���E�������I�����Ă���ꍇ

			// �ϐ���錾
			bool bShot = false;	// ���@�̔��ˏ�

			// ���@���ˑ���
			currentMotion = ControlShotMagic(currentMotion, &bShot);

			if (CSceneTutorial::GetTutorialManager()->GetLesson() == CTutorialManager::LESSON_04)
			{ // ���b�X��04�F�U���̏ꍇ

				if (bShot)
				{ // �U�����삪�s���Ă����ꍇ

					// ���b�X���J�E���^�[���Z
					CSceneTutorial::GetTutorialManager()->AddLessonCounter();
				}
			}
		}

		// �����蔻��
		CollisionTarget(posPlayer);	// �^�[�Q�b�g
		CollisionEnemy(posPlayer);	// �G

		// �o���A�Ƃ̓����蔻��
		if (pStage->CollisionBarrier(posPlayer, fRadiusPlayer))
		{ // �������Ă����ꍇ

			// �Q�������܂�q�b�g
			HitVortex(posPlayer, pStage->GetStageBarrierPosition(), 0);
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
CPlayer::MOTION CPlayerTutorial::UpdateMove(void)
{
	// �ϐ���錾
	MOTION currentMotion = MOTION_MOVE;	// ���݂̃��[�V����
	D3DXVECTOR3 vecTarg = VEC3_ZERO;	// �^�[�Q�b�g�t�����x�N�g��
	D3DXVECTOR3 vecSide = VEC3_ZERO;	// �^�[�Q�b�g�������x�N�g��
	bool bTargMove = false;	// �^�[�Q�b�g�t�����ւ̑����
	bool bSideMove = false;	// �^�[�Q�b�g�������ւ̑����

	// �z�����݂̍X�V
	UpdateAbsorb(vecTarg, vecSide);

	if (CSceneTutorial::GetTutorialManager()->GetLesson() > CTutorialManager::LESSON_01)
	{ // ���b�X��01�F�z�����܂����I�����Ă���ꍇ

		// �^�[�Q�b�g�t�����ւ̉������̑���
		currentMotion = ControlTargAccel(vecTarg, &bTargMove);

		if (CSceneTutorial::GetTutorialManager()->GetLesson() == CTutorialManager::LESSON_02)
		{ // ���b�X��02�F�O������̏ꍇ

			if (bTargMove)
			{ // �^�[�Q�b�g�t�����ւ̑��삪�s���Ă����ꍇ

				// ���b�X���J�E���^�[���Z
				CSceneTutorial::GetTutorialManager()->AddLessonCounter();
			}
		}
	}

	if (CSceneTutorial::GetTutorialManager()->GetLesson() > CTutorialManager::LESSON_02)
	{ // ���b�X��02�F�O��������I�����Ă���ꍇ

		// �^�[�Q�b�g�������ւ̉������̑���
		currentMotion = ControlSideAccel(vecSide, &bSideMove);

		if (CSceneTutorial::GetTutorialManager()->GetLesson() == CTutorialManager::LESSON_03)
		{ // ���b�X��03�F���E�����̏ꍇ

			if (bSideMove)
			{ // �^�[�Q�b�g�������ւ̑��삪�s���Ă����ꍇ

				// ���b�X���J�E���^�[���Z
				CSceneTutorial::GetTutorialManager()->AddLessonCounter();
			}
		}
	}

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}
