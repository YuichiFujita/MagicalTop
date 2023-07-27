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
#include "camera.h"

#include "waveManager.h"
#include "stage.h"
#include "target.h"
#include "player.h"
#include "score.h"
#include "timer.h"

#include "enemy.h"
#include "magic.h"
#include "sea.h"
#include "field.h"
#include "wall.h"
#include "scenery.h"
#include "sky.h"
#include "flower.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CWaveManager *CSceneGame::m_pWaveManager = NULL;	// �E�F�[�u�}�l�[�W���[
CStage	*CSceneGame::m_pStage	= NULL;				// �X�e�[�W
CPlayer	*CSceneGame::m_pPlayer	= NULL;				// �v���C���[�I�u�W�F�N�g
CField	*CSceneGame::m_pField	= NULL;				// �n�ʃI�u�W�F�N�g
CTarget	*CSceneGame::m_pTarget	= NULL;				// �^�[�Q�b�g�I�u�W�F�N�g
CScore	*CSceneGame::m_pScore	= NULL;				// �X�R�A�I�u�W�F�N�g
CTimer	*CSceneGame::m_pTimer	= NULL;				// �^�C�}�[�I�u�W�F�N�g

//************************************************************
//	�q�N���X [CSceneGame] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSceneGame::CSceneGame()
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
	// �E�F�[�u�}�l�[�W���[�̐���
	m_pWaveManager = CWaveManager::Create();
	if (UNUSED(m_pWaveManager))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �X�e�[�W�̐���
	m_pStage = CStage::Create();
	if (UNUSED(m_pStage))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �C�I�u�W�F�N�g�̐���
	CSea::Create();

	// �n�ʃI�u�W�F�N�g�̐���
	m_pField = CField::Create(CField::TEXTURE_NORMAL, D3DXVECTOR3(0.0f, 400.0f, 0.0f), VEC3_ZERO, D3DXVECTOR2(4000.0f, 4000.0f), XCOL_WHITE, POSGRID2(80, 80));
	if (UNUSED(m_pField))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �n�`��ݒ�
	m_pField->SetTerrain(CField::TERRAIN_80x80);

	// �ǃI�u�W�F�N�g�̐���
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3( 0.0f,    0.0f, -2000.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),   D3DXVECTOR2(4000.0f, 400.0f), XCOL_WHITE, POSGRID2(12, 1));
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3(-2000.0f, 0.0f,  0.0f),    D3DXToRadian(D3DXVECTOR3(0.0f, 90.0f, 0.0f)),  D3DXVECTOR2(4000.0f, 400.0f), XCOL_WHITE, POSGRID2(12, 1));
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3( 0.0f,    0.0f,  2000.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 180.0f, 0.0f)), D3DXVECTOR2(4000.0f, 400.0f), XCOL_WHITE, POSGRID2(12, 1));
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3( 2000.0f, 0.0f,  0.0f),    D3DXToRadian(D3DXVECTOR3(0.0f, 270.0f, 0.0f)), D3DXVECTOR2(4000.0f, 400.0f), XCOL_WHITE, POSGRID2(12, 1));

	// �i�F�I�u�W�F�N�g�̐���
	CScenery::Create(CScenery::TEXTURE_NORMAL, VEC3_ZERO, VEC3_ZERO,                                    XCOL_WHITE,                        POSGRID2(32, 1), 12000.0f, 1000.0f, D3DCULL_CW, false);
	CScenery::Create(CScenery::TEXTURE_NORMAL, VEC3_ZERO, D3DXToRadian(D3DXVECTOR3(0.0f, 85.0f, 0.0f)), D3DXCOLOR(0.7f, 1.0f, 1.0f, 1.0f), POSGRID2(32, 1), 14000.0f, 1600.0f, D3DCULL_CW, false);
	CScenery::Create(CScenery::TEXTURE_NORMAL, VEC3_ZERO, D3DXToRadian(D3DXVECTOR3(0.0f, 35.0f, 0.0f)), D3DXCOLOR(0.4f, 1.0f, 0.7f, 1.0f), POSGRID2(32, 1), 16000.0f, 2200.0f, D3DCULL_CW, false);

	// ��I�u�W�F�N�g�̐���
	CSky::Create(CSky::TEXTURE_NORMAL, VEC3_ZERO, VEC3_ZERO, XCOL_WHITE, POSGRID2(32, 6), 18000.0f, D3DCULL_CW, false);

	// �v���C���[�I�u�W�F�N�g�̐���
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, -100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	if (UNUSED(m_pPlayer))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �^�[�Q�b�g�I�u�W�F�N�g�̐���
	m_pTarget = CTarget::Create(CTarget::MODEL_NORMAL, D3DXVECTOR3(0.0f, 400.0f, 0.0f), VEC3_ZERO);
	if (UNUSED(m_pTarget))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �X�R�A�I�u�W�F�N�g�̐���
	m_pScore = CScore::Create();
	if (UNUSED(m_pScore))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �^�C�}�[�I�u�W�F�N�g�̐���
	m_pTimer = CTimer::Create();
	if (UNUSED(m_pTimer))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	�����ݒ�
	//--------------------------------------------------------
	// �G�Z�b�g�A�b�v�̓Ǎ�
	CEnemy::LoadSetup();

	// ���@�Z�b�g�A�b�v�̓Ǎ�
	CMagic::LoadSetup();

	// �}�i�t�����[�����_������
	CFlower::RandomSpawn(30, CFlower::TYPE_NORMAL, D3DXVECTOR3(25.0f, 50.0f, 0.0f), 10);	// TODO�F�萔�ύX

	// TODO�F�����ݒ�
#if 0
	// �J�����̏����ʒu��ݒ�
	CManager::GetCamera()->SetDestCamera();

	// �^�C�����v���J�n
	m_pTimer->Start();

	// BGM�̍Đ�
	CManager::GetSound()->Play(CSound::LABEL_BGM_000);
#endif

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

	// �X�e�[�W�̔j��
	if (FAILED(CStage::Release(m_pStage)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �I���ς݂̃I�u�W�F�N�g�|�C���^��NULL�ɂ���
	m_pPlayer	= NULL;		// �v���C���[�I�u�W�F�N�g
	m_pTarget	= NULL;		// �^�[�Q�b�g�I�u�W�F�N�g
	m_pField	= NULL;		// �n�ʃI�u�W�F�N�g
	m_pScore	= NULL;		// �X�R�A�I�u�W�F�N�g
	m_pTimer	= NULL;		// �^�C�}�[�I�u�W�F�N�g

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�X�V����
//============================================================
void CSceneGame::Update(void)
{
	if (USED(m_pStage))
	{ // �g�p���̏ꍇ

		// �X�e�[�W�̍X�V
		m_pStage->Update();
	}
	else { assert(false); }	// ��g�p��

	if (USED(m_pWaveManager))
	{ // �g�p���̏ꍇ

		// �E�F�[�u�}�l�[�W���[�̍X�V
		m_pWaveManager->Update();
	}
	else { assert(false); }	// ��g�p��

	// TODO�F�J�ڂ̃^�C�~���O�ŉ����莀�S�t���O�ŉ��
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
//	�X�e�[�W�擾����
//============================================================
CStage *CSceneGame::GetStage(void)
{
	// �X�e�[�W�̃|�C���^��Ԃ�
	return m_pStage;
}

//============================================================
//	�v���C���[�擾����
//============================================================
CPlayer *CSceneGame::GetPlayer(void)
{
	// �v���C���[�̃|�C���^��Ԃ�
	return m_pPlayer;
}

//============================================================
//	�^�[�Q�b�g�擾����
//============================================================
CTarget *CSceneGame::GetTarget(void)
{
	// �^�[�Q�b�g�̃|�C���^��Ԃ�
	return m_pTarget;
}

//============================================================
//	�n�ʎ擾����
//============================================================
CField *CSceneGame::GetField(void)
{
	// �n�ʂ̃|�C���^��Ԃ�
	return m_pField;
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
//	�^�C�}�[�擾����
//============================================================
CTimer *CSceneGame::GetTimer(void)
{
	// �^�C�}�[�̃|�C���^��Ԃ�
	return m_pTimer;
}
