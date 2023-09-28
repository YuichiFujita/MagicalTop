//============================================================
//
//	�`���[�g���A����ʏ��� [sceneTutorial.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "sceneTutorial.h"
#include "manager.h"
#include "sound.h"
#include "camera.h"
#include "tutorialManager.h"

#include "player.h"
#include "target.h"
#include "stage.h"
#include "enemy.h"
#include "magic.h"
#include "sea.h"
#include "wall.h"
#include "scenery.h"
#include "sky.h"
#include "flower.h"
#include "weed.h"

//************************************************************
//	�}�N����`
//************************************************************
#define SCO_POS		(D3DXVECTOR3(825.0f, 50.0f, 0.0f))		// �X�R�A�ʒu
#define SCO_SIZE	(D3DXVECTOR3(60.0f, 80.0f, 0.0f))		// �X�R�A�傫��
#define SCO_SPACE	(D3DXVECTOR3(SCO_SIZE.x, 0.0f, 0.0f))	// �X�R�A��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CTutorialManager *CSceneTutorial::m_pTutorialManager = NULL;	// �`���[�g���A���}�l�[�W���[

//************************************************************
//	�q�N���X [CSceneTutorial] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSceneTutorial::CSceneTutorial(const MODE mode) : CScene(mode)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CSceneTutorial::~CSceneTutorial()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSceneTutorial::Init(void)
{
	//--------------------------------------------------------
	//	�`���[�g���A���̏�����
	//--------------------------------------------------------
	// �`���[�g���A���}�l�[�W���[�̐���
	m_pTutorialManager = CTutorialManager::Create();
	if (UNUSED(m_pTutorialManager))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �V�[���̏�����
	CScene::Init();

	//--------------------------------------------------------
	//	�I�u�W�F�N�g����
	//--------------------------------------------------------
	// �C�I�u�W�F�N�g�̐���
	CSea::Create();

	// �ǃI�u�W�F�N�g�̐���
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3( 0.0f,    0.0f, -3000.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),   D3DXVECTOR2(6000.0f, 400.0f), XCOL_WHITE, POSGRID2(18, 1));
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3(-3000.0f, 0.0f,  0.0f),    D3DXToRadian(D3DXVECTOR3(0.0f, 90.0f, 0.0f)),  D3DXVECTOR2(6000.0f, 400.0f), XCOL_WHITE, POSGRID2(18, 1));
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3( 0.0f,    0.0f,  3000.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 180.0f, 0.0f)), D3DXVECTOR2(6000.0f, 400.0f), XCOL_WHITE, POSGRID2(18, 1));
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3( 3000.0f, 0.0f,  0.0f),    D3DXToRadian(D3DXVECTOR3(0.0f, 270.0f, 0.0f)), D3DXVECTOR2(6000.0f, 400.0f), XCOL_WHITE, POSGRID2(18, 1));

	// �i�F�I�u�W�F�N�g�̐���
	CScenery::Create(CScenery::TEXTURE_NORMAL, VEC3_ZERO, VEC3_ZERO,                                    XCOL_WHITE,                        POSGRID2(32, 1), 12000.0f, 1000.0f, D3DCULL_CW, false);
	CScenery::Create(CScenery::TEXTURE_NORMAL, VEC3_ZERO, D3DXToRadian(D3DXVECTOR3(0.0f, 85.0f, 0.0f)), D3DXCOLOR(0.7f, 1.0f, 1.0f, 1.0f), POSGRID2(32, 1), 14000.0f, 1600.0f, D3DCULL_CW, false);
	CScenery::Create(CScenery::TEXTURE_NORMAL, VEC3_ZERO, D3DXToRadian(D3DXVECTOR3(0.0f, 35.0f, 0.0f)), D3DXCOLOR(0.4f, 1.0f, 0.7f, 1.0f), POSGRID2(32, 1), 16000.0f, 2200.0f, D3DCULL_CW, false);

	// ��I�u�W�F�N�g�̐���
	CSky::Create(CSky::TEXTURE_NORMAL, VEC3_ZERO, VEC3_ZERO, XCOL_WHITE, POSGRID2(32, 6), 18000.0f, D3DCULL_CW, false);

	//--------------------------------------------------------
	//	�����ݒ�
	//--------------------------------------------------------
	// �G�Z�b�g�A�b�v�̓Ǎ�
	CEnemy::LoadSetup();

	// ���@�Z�b�g�A�b�v�̓Ǎ�
	CMagic::LoadSetup();

	// �}�i�t�����[�Z�b�g�A�b�v�̓Ǎ�
	CFlower::LoadSetup();

	// ���Z�b�g�A�b�v�̓Ǎ�
	CWeed::LoadSetup();

	// �}�i�t�����[�����_������
	CFlower::RandomSpawn(30, CFlower::TYPE_SPRING);	// TODO�F�萔�ύX

	// �������_������
	CWeed::RandomSpawn(200, CWeed::TYPE_SPRING);	// TODO�F�萔�ύX

	// �J������ݒ�
	CManager::GetCamera()->SetState(CCamera::STATE_BARGAINING);	// �J��������������Ԃɐݒ�
	CManager::GetCamera()->SetDestBargaining();	// �ڕW�ʒu��ݒ�

	// BGM�̍Đ�
	CManager::GetSound()->Play(CSound::LABEL_BGM_TUTORIAL);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CSceneTutorial::Uninit(void)
{
	// �`���[�g���A���}�l�[�W���[�̔j��
	if (FAILED(CTutorialManager::Release(m_pTutorialManager)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �V�[���̏I��
	CScene::Uninit();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�X�V����
//============================================================
void CSceneTutorial::Update(void)
{
	if (USED(m_pTutorialManager))
	{ // �g�p���̏ꍇ

		// �`���[�g���A���}�l�[�W���[�̍X�V
		m_pTutorialManager->Update();
	}
	else { assert(false); }	// ��g�p��

	// �v���C���[�̃}�i�����
	UpdateLimitPlayerMana();

	// �^�[�Q�b�g�̗̑͏����
	UpdateLimitTargetLife();

	// �V�[���̍X�V
	CScene::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CSceneTutorial::Draw(void)
{

}

//============================================================
//	�`���[�g���A���}�l�[�W���[�擾����
//============================================================
CTutorialManager *CSceneTutorial::GetTutorialManager(void)
{
	// �`���[�g���A���}�l�[�W���[�̃|�C���^��Ԃ�
	return m_pTutorialManager;
}

//============================================================
//	�v���C���[�̃}�i���������
//============================================================
void CSceneTutorial::UpdateLimitPlayerMana(void)
{
	// �|�C���^��錾
	CPlayer *pPlayer = GetPlayer();	// �v���C���[���

	if (CSceneTutorial::GetTutorialManager()->GetLesson() < CTutorialManager::LESSON_05)
	{ // ���b�X��05�F�}�i�񕜂܂œ��B���Ă��Ȃ��ꍇ

		if (pPlayer->GetMana() < pPlayer->GetMaxMana() / 2)
		{ // �v���C���[�̃}�i��������؂����ꍇ

			// �v���C���[�̃}�i�𔼕��ɐݒ�
			pPlayer->SetMana(pPlayer->GetMaxMana() / 2);
		}
	}
}

//============================================================
//	�^�[�Q�b�g�̗̑͏��������
//============================================================
void CSceneTutorial::UpdateLimitTargetLife(void)
{
	// �|�C���^��錾
	CTarget *pTarget = GetTarget();	// �^�[�Q�b�g���

	if (pTarget->GetLife() < pTarget->GetMaxLife() / 2)
	{ // �^�[�Q�b�g�̗̑͂�������؂����ꍇ

		// �^�[�Q�b�g�̗̑͂𔼕��ɐݒ�
		pTarget->SetLife(pTarget->GetMaxLife() / 2);
	}
}
