//============================================================
//
//	�Q�[����ʏ��� [sceneGame.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "sceneGame.h"
#include "manager.h"
#include "sound.h"
#include "input.h"

#include "waveManager.h"
#include "timerManager.h"
#include "stage.h"
#include "pause.h"
#include "score.h"
#include "warningSpawn.h"

#include "enemy.h"
#include "magic.h"
#include "sea.h"
#include "wall.h"
#include "scenery.h"
#include "sky.h"
#include "flower.h"
#include "weed.h"
#include "player.h"

//************************************************************
//	�}�N����`
//************************************************************
#define TIME_POS		(D3DXVECTOR3(40.0f, 50.0f, 0.0f))			// �^�C�}�[�ʒu
#define TIME_VAL_SIZE	(D3DXVECTOR3(60.0f, 80.0f, 0.0f))			// �^�C�}�[�����傫��
#define TIME_PART_SIZE	(D3DXVECTOR3(35.0f, 80.0f, 0.0f))			// �^�C�}�[��؂�傫��
#define TIME_VAL_SPACE	(D3DXVECTOR3(TIME_VAL_SIZE.x, 0.0f, 0.0f))	// �^�C�}�[������
#define TIME_PART_SPACE	(D3DXVECTOR3(TIME_PART_SIZE.x, 0.0f, 0.0f))	// �^�C�}�[��؂��

#define SCO_POS		(D3DXVECTOR3(825.0f, 50.0f, 0.0f))		// �X�R�A�ʒu
#define SCO_SIZE	(D3DXVECTOR3(60.0f, 80.0f, 0.0f))		// �X�R�A�傫��
#define SCO_SPACE	(D3DXVECTOR3(SCO_SIZE.x, 0.0f, 0.0f))	// �X�R�A��

#define WARNING_SIZE	(D3DXVECTOR3(1000.0f, 500.0f, 0.0f))	// �x���傫��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CWaveManager	*CSceneGame::m_pWaveManager  = NULL;	// �E�F�[�u�}�l�[�W���[
CTimerManager	*CSceneGame::m_pTimerManager = NULL;	// �^�C�}�[�}�l�[�W���[
CWarningSpawn	*CSceneGame::m_pWarningSpawn = NULL;	// �o���x���\���I�u�W�F�N�g
CPause	*CSceneGame::m_pPause	= NULL;					// �|�[�Y
CScore	*CSceneGame::m_pScore	= NULL;					// �X�R�A�I�u�W�F�N�g

bool CSceneGame::m_bControlCamera = false;	// �J�����̑����
bool CSceneGame::m_bDrawUI = true;			// UI�̕`���
bool CSceneGame::m_bDrawPause = true;		// �|�[�Y�̕`���

//************************************************************
//	�q�N���X [CSceneGame] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSceneGame::CSceneGame(const MODE mode) : CScene(mode)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CSceneGame::~CSceneGame()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSceneGame::Init(void)
{
	// �^�C�}�[�}�l�[�W���[�̐���
	m_pTimerManager = CTimerManager::Create
	( // ����
		TIME_POS,		// �ʒu
		TIME_VAL_SIZE,	// �����̑傫��
		TIME_PART_SIZE,	// ��؂�̑傫��
		TIME_VAL_SPACE,	// �����̋�
		TIME_PART_SPACE	// ��؂�̋�
	);
	if (UNUSED(m_pTimerManager))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �X�R�A�I�u�W�F�N�g�̐���
	m_pScore = CScore::Create
	( // ����
		SCO_POS,	// �ʒu
		SCO_SIZE,	// �傫��
		SCO_SPACE	// ��
	);
	if (UNUSED(m_pScore))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �o���x���\���I�u�W�F�N�g�̐���
	m_pWarningSpawn = CWarningSpawn::Create
	( // ����
		SCREEN_CENT,	// �ʒu
		WARNING_SIZE	// �傫��
	);
	if (UNUSED(m_pWarningSpawn))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �V�[���̏�����
	CScene::Init();		// �X�e�[�W�E�n�ʁE�^�[�Q�b�g�E�v���C���[�̐���

	// �E�F�[�u�}�l�[�W���[�̐���
	m_pWaveManager = CWaveManager::Create();
	if (UNUSED(m_pWaveManager))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �|�[�Y�̐���
	m_pPause = CPause::Create();
	if (UNUSED(m_pPause))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

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

	// �^�C�����v���J�n
	m_pTimerManager->Start();			// �v�����J�n
	m_pTimerManager->EnableStop(true);	// �v�����ꎞ��~

	// UI�̕`��󋵂�ݒ�
	SetEnableDrawUI(m_bDrawUI);

	// �|�[�Y�̕`��󋵂�ݒ�
	SetEnableDrawPause(m_bDrawPause);

	// BGM�̍Đ�
	CManager::GetSound()->Play(CSound::LABEL_BGM_GAME);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CSceneGame::Uninit(void)
{
	// �E�F�[�u�}�l�[�W���[�̔j��
	if (FAILED(CWaveManager::Release(m_pWaveManager)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �^�C�}�[�}�l�[�W���[�̔j��
	if (FAILED(CTimerManager::Release(m_pTimerManager)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �|�[�Y�̔j��
	if (FAILED(CPause::Release(m_pPause)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �I���ς݂̃I�u�W�F�N�g�|�C���^��NULL�ɂ���
	m_pScore = NULL;	// �X�R�A�I�u�W�F�N�g

	// �V�[���̏I��
	CScene::Uninit();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�X�V����
//============================================================
void CSceneGame::Update(void)
{
	if (CManager::GetKeyboard()->GetTrigger(DIK_F2))
	{
		SetEnableDrawUI((!m_bDrawUI) ? true : false);
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_F3))
	{
		SetEnableDrawPause((!m_bDrawPause) ? true : false);
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_F4))
	{
		SetEnableControlCamera((!m_bControlCamera) ? true : false);
	}

	if (USED(m_pTimerManager))
	{ // �g�p���̏ꍇ

		// �^�C�}�[�}�l�[�W���[�̍X�V
		m_pTimerManager->Update();
	}
	else { assert(false); }	// ��g�p��

	if (USED(m_pPause))
	{ // �g�p���̏ꍇ

		// �|�[�Y�̍X�V
		m_pPause->Update();
	}
	else { assert(false); }	// ��g�p��

	if (!m_pPause->IsPause())
	{ // �|�[�Y���ł͂Ȃ��ꍇ

		if (USED(m_pWaveManager))
		{ // �g�p���̏ꍇ

			// �E�F�[�u�}�l�[�W���[�̍X�V
			m_pWaveManager->Update();
		}
		else { assert(false); }	// ��g�p��

		// �V�[���̍X�V
		CScene::Update();
	}
	else
	{ // �|�[�Y���̏ꍇ

		if (CManager::GetCamera()->GetState() == CCamera::STATE_CONTROL)
		{ // �J�����������Ԃ̏ꍇ

			// �J�����̍X�V
			CManager::GetCamera()->Update();
		}
	}

	// TODO�F�J�ڂ̃^�C�~���O�ŉ�����V�[���ɂ����S�t���O�ǉ��ŉ��
	//		 �V�[���}�l�[�W���[��p�ӂ���
}

//============================================================
//	�`�揈��
//============================================================
void CSceneGame::Draw(void)
{

}

//============================================================
//	�E�F�[�u�}�l�[�W���[�擾����
//============================================================
CWaveManager *CSceneGame::GetWaveManager(void)
{
	// �E�F�[�u�}�l�[�W���[�̃|�C���^��Ԃ�
	return m_pWaveManager;
}

//============================================================
//	�^�C�}�[�}�l�[�W���[�擾����
//============================================================
CTimerManager *CSceneGame::GetTimerManager(void)
{
	// �^�C�}�[�}�l�[�W���[�̃|�C���^��Ԃ�
	return m_pTimerManager;
}

//============================================================
//	�|�[�Y�擾����
//============================================================
CPause *CSceneGame::GetPause(void)
{
	// �|�[�Y�̃|�C���^��Ԃ�
	return m_pPause;
}

//============================================================
//	�X�R�A�擾����
//============================================================
CScore *CSceneGame::GetScore(void)
{
	// �X�R�A�̃|�C���^��Ԃ�
	return m_pScore;
}

//============================================================
//	�o���x���\���擾����
//============================================================
CWarningSpawn *CSceneGame::GetWarningSpawn(void)
{
	// �o���x���\���̃|�C���^��Ԃ�
	return m_pWarningSpawn;
}

//============================================================
//	�J�����̑���󋵂̐ݒ菈��
//============================================================
void CSceneGame::SetEnableControlCamera(const bool bControl)
{
	// �����̃J��������󋵂�ݒ�
	m_bControlCamera = bControl;

	// �J�����̑���󋵂�ݒ�
	if (bControl)
	{ // ���삷��󋵂̏ꍇ

		// �����ԂɕύX
		CManager::GetCamera()->SetState(CCamera::STATE_CONTROL);
	}
	else
	{ // ���삵�Ȃ��󋵂̏ꍇ

		// ��������ԂɕύX
		CManager::GetCamera()->SetState(CCamera::STATE_BARGAINING);
	}
}

//============================================================
//	UI�̕`��󋵂̐ݒ菈��
//============================================================
void CSceneGame::SetEnableDrawUI(const bool bDraw)
{
	// ������UI�̕`��󋵂�ݒ�
	m_bDrawUI = bDraw;

	// �^�C�}�[�̕`��󋵂�ݒ�
	m_pTimerManager->SetEnableDraw(bDraw);

	// �X�R�A�̕`��󋵂�ݒ�
	m_pScore->SetEnableDraw(bDraw);

	// �v���C���[�̌o���l��ݒ�
	GetPlayer()->SetEnableDrawExp(bDraw);
}

//============================================================
//	�|�[�Y�̕`��󋵂̐ݒ菈��
//============================================================
void CSceneGame::SetEnableDrawPause(const bool bDraw)
{
	// �����̃|�[�Y�̕`��󋵂�ݒ�
	m_bDrawPause = bDraw;

	// �|�[�Y�̕`��󋵂�ݒ�
	m_pPause->SetEnableDraw(m_pPause->IsPause());
}

//============================================================
//	UI�̕`��󋵎擾����
//============================================================
bool CSceneGame::IsDrawUI(void)
{
	// UI�̕`��󋵂�Ԃ�
	return m_bDrawUI;
}

//============================================================
//	�|�[�Y�̕`��󋵎擾����
//============================================================
bool CSceneGame::IsDrawPause(void)
{
	// �|�[�Y�̕`��󋵂�Ԃ�
	return m_bDrawPause;
}
