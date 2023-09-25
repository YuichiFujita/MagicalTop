//============================================================
//
//	�E�F�[�u�}�l�[�W���[���� [waveManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "waveManager.h"
#include "manager.h"
#include "sound.h"
#include "texture.h"
#include "model.h"
#include "modelUI.h"
#include "object2D.h"
#include "value.h"
#include "scene.h"
#include "sceneGame.h"
#include "target.h"
#include "player.h"
#include "camera.h"
#include "field.h"
#include "flower.h"
#include "weed.h"
#include "score.h"
#include "timerManager.h"
#include "warningSpawn.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CWaveManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\wave000.png",	// �E�F�[�u�\���e�N�X�`��
};

const char *CWaveManager::mc_apModelFile[] =	// ���f���萔
{
	"data\\MODEL\\FONT\\season.x",	// SEASON�t�H���g���f��
	"data\\MODEL\\FONT\\end.x",		// END�t�H���g���f��
};

const char *CWaveManager::mc_apSeasonModelFile[] =	// �V�[�Y���\�L���f���萔
{
	"data\\MODEL\\FONT\\spring.x",	// SPRING�t�H���g���f��
	"data\\MODEL\\FONT\\summer.x",	// SUMMER�t�H���g���f��
	"data\\MODEL\\FONT\\autumn.x",	// AUTUMN�t�H���g���f��
	"data\\MODEL\\FONT\\winter.x",	// WINTER�t�H���g���f��
};

CWaveManager::Season CWaveManager::m_aWaveInfo[CWaveManager::SEASON_MAX] = {};	// �E�F�[�u���

//************************************************************
//	�}�N����`
//************************************************************
#define WAVE_SETUP_TXT	"data\\TXT\\wave.txt"	// �Z�b�g�A�b�v�e�L�X�g���΃p�X

#define POS_SEASON_LEFT		(D3DXVECTOR3(-130.0f, 75.0f, -0.0f))	// �V�[�Y�����\���̈ʒu
#define POS_SEASON_RIGHT	(D3DXVECTOR3(130.0f, -75.0f, -0.0f))	// �V�[�Y���E�\���̈ʒu

#define SIZE_SEASON	(D3DXVECTOR3(0.25f, 0.25f, 0.25f))	// �V�[�Y���̊g�嗦
#define SIZE_WAVE	(D3DXVECTOR3(740.0f, 280.0f, 0.0f))	// �E�F�[�u�̑傫��
#define SIZE_NUM	(D3DXVECTOR3(210.0f, 250.0f, 0.0f))	// �����̑傫��

#define WAVE_PRIO	(6)			// �E�F�[�u�̗D�揇��
#define SPACE_EDGE	(155.0f)	// �[���猩���E�F�[�u�\���̂����
#define MUL_SIZEUP		(12.0f)	// �E�F�[�u�E�����̑傫���㏸��Z��
#define MUL_SIZEDOWN	(0.85f)	// �E�F�[�u�E�����̑傫�����~��Z��

#define SEASON_STARTCNT	(90)	// �G�߂̊J�n�����̈ڍs�܂ł̗]�C�J�E���^�[
#define WAVE_STARTCNT	(90)	// �E�F�[�u�J�n�����̈ڍs�܂ł̗]�C�J�E���^�[
#define NEXT_WAITCNT	(30)	// �G�S�Ŏ��̈ڍs�܂ł̗]�C�J�E���^�[
#define SEASON_MOVECNT	(30)	// �V�[�Y���\���̓�����~�܂ł̗]�C�J�E���^�[

#define SEASON_ADD_ROT	(0.2f)		// �V�[�Y���\����X�������Z��
#define SEASON_SUB_ROT	(0.005f)	// �V�[�Y���\����Y�������Z��
#define SEASON_MUL_SIZE	(1.05f)		// �V�[�Y���\���̊g�嗦��Z��

#define PLUS_FLOWER	(10)	// �G�ߐi�s���̃}�i�t�����[�̐�����
#define PLUS_WEED	(10)	// �G�ߐi�s���̑��̐�����
#define WAIT_FRAME	(240)	// ���U���g�J�ڎ��̗]�C�t���[��

//************************************************************
//	�e�N���X [CWaveManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CWaveManager::CWaveManager()
{
	// �����o�ϐ����N���A
	memset(&m_apModelUI[0], 0, sizeof(m_apModelUI));	// �V�[�Y���\���̏��
	m_pObject2D = NULL;			// �E�F�[�u�\���̏��
	m_pValue = NULL;			// �E�F�[�u���\���̏��
	m_state = STATE_NONE;		// ���
	m_nSeason = SEASON_SPRING;	// �G�ߊǗ��J�E���^�[
	m_nWave = 0;				// �E�F�[�u�Ǘ��J�E���^�[
	m_nPoint = 0;				// �o���Ǘ��J�E���^�[
	m_nCounterState = 0;		// ��ԊǗ��J�E���^�[
	m_nCounterFrame = 0;		// �E�F�[�u�]�C�Ǘ��J�E���^�[
}

//============================================================
//	�f�X�g���N�^
//============================================================
CWaveManager::~CWaveManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CWaveManager::Init(void)
{
	// �ϐ��z���錾
	D3DXVECTOR3 aPos[NUM_MODEL_UI] =	// �ʒu�z��
	{ // �����l
		POS_SEASON_LEFT,	// ���ʒu
		POS_SEASON_RIGHT	// �E�ʒu
	};

	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^
	CModel *pModel = CManager::GetModel();			// ���f���ւ̃|�C���^

	// �����o�ϐ���������
	memset(&m_apModelUI[0], 0, sizeof(m_apModelUI));	// �V�[�Y���\���̏��
	m_pObject2D = NULL;					// �E�F�[�u�\���̏��
	m_pValue = NULL;					// �E�F�[�u���\���̏��
	m_state = STATE_SEASON_START_INIT;	// ���
	m_nSeason = SEASON_SPRING;			// �G�ߊǗ��J�E���^�[
	m_nWave = 0;						// �E�F�[�u�Ǘ��J�E���^�[
	m_nPoint = 0;						// �o���Ǘ��J�E���^�[
	m_nCounterState = 0;				// ��ԊǗ��J�E���^�[
	m_nCounterFrame = 0;				// �E�F�[�u�]�C�Ǘ��J�E���^�[

	// �Z�b�g�A�b�v�̓ǂݍ���
	LoadSetup();

	//--------------------------------------------------------
	//	���f��UI�̐����E�ݒ�
	//--------------------------------------------------------
	for (int nCntWave = 0; nCntWave < NUM_MODEL_UI; nCntWave++)
	{ // ���f��UI�̎g�p�����J��Ԃ�

		// �V�[�Y���\���p�̃��f��UI�̐���
		m_apModelUI[nCntWave] = CModelUI::Create
		( // ����
			aPos[nCntWave],	// �ʒu
			VEC3_ZERO,		// ����
			SIZE_SEASON		// �g�嗦
		);
		if (UNUSED(m_apModelUI[nCntWave]))
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// ���f����o�^�E����
		m_apModelUI[nCntWave]->BindModel(pModel->GetModel(pModel->Regist(mc_apModelFile[0])));

		// �D�揇�ʂ�ݒ�
		m_apModelUI[nCntWave]->SetPriority(WAVE_PRIO);

		// �`������Ȃ��ݒ�ɂ���
		m_apModelUI[nCntWave]->SetEnableDraw(false);
	}

	//--------------------------------------------------------
	//	�I�u�W�F�N�g2D�̐����E�ݒ�
	//--------------------------------------------------------
	// �E�F�[�u�\���p�̃I�u�W�F�N�g2D�̐���
	m_pObject2D = CObject2D::Create
	( // ����
		D3DXVECTOR3	// �ʒu
		( // ����
			0.0f + SPACE_EDGE + (SIZE_WAVE.x * 0.5f),	// x
			SCREEN_HEIGHT * 0.5f,						// y
			0.0											// z
		),
		SIZE_WAVE	// �傫��
	);
	if (UNUSED(m_pObject2D))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pObject2D->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_NORMAL]));

	// �D�揇�ʂ�ݒ�
	m_pObject2D->SetPriority(WAVE_PRIO);

	// �`������Ȃ��ݒ�ɂ���
	m_pObject2D->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�����I�u�W�F�N�g�̐����E�ݒ�
	//--------------------------------------------------------
	// �E�F�[�u���\���p�̐����I�u�W�F�N�g�̐���
	m_pValue = CValue::Create
	( // ����
		CValue::TEXTURE_NORMAL,	// �e�N�X�`��
		D3DXVECTOR3				// �ʒu
		( // ����
			(float)SCREEN_WIDTH - SPACE_EDGE - (SIZE_NUM.x * 0.5f),	// x
			SCREEN_HEIGHT * 0.5f,									// y
			0.0f													// z
		),
		SIZE_NUM	// �傫��
	);
	if (UNUSED(m_pValue))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������ݒ�
	m_pValue->SetNumber(0);

	// �D�揇�ʂ�ݒ�
	m_pValue->SetPriority(WAVE_PRIO);

	// �`������Ȃ��ݒ�ɂ���
	m_pValue->SetEnableDraw(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CWaveManager::Uninit(void)
{
	for (int nCntWave = 0; nCntWave < NUM_MODEL_UI; nCntWave++)
	{ // ���f��UI�̎g�p�����J��Ԃ�

		// �V�[�Y���\���p�̃��f��UI��j��
		m_apModelUI[nCntWave]->Uninit();
	}

	// �E�F�[�u�\���p�̃I�u�W�F�N�g2D��j��
	m_pObject2D->Uninit();

	// �E�F�[�u���\���p�̐����I�u�W�F�N�g��j��
	m_pValue->Uninit();

	for (int nCntSeason = 0; nCntSeason < SEASON_MAX; nCntSeason++)
	{ // �G�ߐ����J��Ԃ�

		if (USED(m_aWaveInfo[nCntSeason].pWave))
		{ // �E�F�[�u���g�p����Ă���ꍇ

			for (int nCntWave = 0; nCntWave < m_aWaveInfo[nCntSeason].nNumWave; nCntWave++)
			{ // �E�F�[�u�����J��Ԃ�

				// �o�������������J��
				delete[] m_aWaveInfo[nCntSeason].pWave[nCntWave].pPoint;
				m_aWaveInfo[nCntSeason].pWave[nCntWave].pPoint = NULL;
			}

			// �E�F�[�u�����������J��
			delete[] m_aWaveInfo[nCntSeason].pWave;
			m_aWaveInfo[nCntSeason].pWave = NULL;
		}
	}
}

//============================================================
//	�X�V����
//============================================================
void CWaveManager::Update(void)
{
	// ��Ԃ̍X�V
	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:	// �������Ȃ����

		// ����

		break;

	case STATE_SEASON_START_INIT:	// �G�߂̊J�n���������

		// �G�߂̊J�n������
		InitSeasonStart();

		// �G�߂̊J�n�����ɑ���

	case STATE_SEASON_START:	// �G�߂̊J�n���

		// �G�߂̊J�n
		UpdateSeasonStart();

		break;

	case STATE_WAVE_START_INIT:	// �E�F�[�u�J�n���������

		// �E�F�[�u�J�n������
		InitWaveStart();

		// �E�F�[�u�J�n�����ɑ���

	case STATE_WAVE_START:		// �E�F�[�u�J�n���

		// �E�F�[�u�J�n
		UpdateWaveStart();

		break;

	case STATE_PROGRESSION:		// �E�F�[�u�i�s���

		// �E�F�[�u�i�s
		UpdateProgression();

		break;

	case STATE_SEASON_END_INIT:	// �G�߂̏I�����������

		// �G�߂̏I��������
		InitWaveEnd();

		// �G�߂̏I�������ɑ���

	case STATE_SEASON_END:		// �G�߂̏I�����

		// �G�߂̏I��
		UpdateWaveEnd();

		break;

	case STATE_WAIT:	// ���G�߂̊J�n�ҋ@���

		// levelupManager���Ŏ��̋G�߂Ɉڍs����

		break;

	case STATE_END:		// �I�����

		// ���U���g�J��
		ResultTransition(CGameManager::RESULT_CLEAR);

		// �����𔲂���
		return;

	default:	// ��O����
		assert(false);
		break;
	}

	if (CScene::GetPlayer()->GetState() == CPlayer::STATE_DEATH
	||  CScene::GetTarget()->GetState() == CTarget::STATE_DESTROY)
	{ // �v���C���[�����S��ԁA�܂��̓^�[�Q�b�g���j���Ԃ̏ꍇ

		// ���U���g�J��
		ResultTransition(CGameManager::RESULT_FAILED, WAIT_FRAME);

		// �����𔲂���
		return;
	}

	for (int nCntWave = 0; nCntWave < NUM_MODEL_UI; nCntWave++)
	{ // ���f��UI�̎g�p�����J��Ԃ�

		// �V�[�Y���\���p�̃��f��UI�̍X�V
		m_apModelUI[nCntWave]->Update();
	}

	// �E�F�[�u�\���p�̃I�u�W�F�N�g2D�̍X�V
	m_pObject2D->Update();

	// �E�F�[�u���\���p�̐����I�u�W�F�N�g�̍X�V
	m_pValue->Update();
}

//============================================================
//	�G�߂̈ڍs����
//============================================================
void CWaveManager::NextSeason(void)
{
	if (m_state == STATE_WAIT)
	{ // ���̋G�߂̊J�n�ҋ@��Ԃ̏ꍇ

		// ��Ԃ�ݒ�
		m_state = STATE_SEASON_START_INIT;	// �G�߂̊J�n���

		// �J�����X�V��ON�ɂ���
		CManager::GetCamera()->SetEnableUpdate(true);

		// �J������Ԃ�ݒ�
		CManager::GetCamera()->SetState(CCamera::STATE_UP);	// ��������
	}
	else { assert(false); }	// �g�p�s�^�C�~���O
}

//============================================================
//	��Ԏ擾����
//============================================================
CWaveManager::STATE CWaveManager::GetState(void)
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//============================================================
//	��������
//============================================================
CWaveManager *CWaveManager::Create(void)
{
	// �|�C���^��錾
	CWaveManager *pWaveManager = NULL;	// �E�F�[�u�}�l�[�W���[�����p

	if (UNUSED(pWaveManager))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pWaveManager = new CWaveManager;	// �E�F�[�u�}�l�[�W���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pWaveManager))
	{ // �g�p����Ă���ꍇ
		
		// �E�F�[�u�}�l�[�W���[�̏�����
		if (FAILED(pWaveManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pWaveManager;
			pWaveManager = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pWaveManager;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CWaveManager::Release(CWaveManager *&prWaveManager)
{
	if (USED(prWaveManager))
	{ // �g�p���̏ꍇ

		// �E�F�[�u�}�l�[�W���[�̏I��
		prWaveManager->Uninit();

		// �������J��
		delete prWaveManager;
		prWaveManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	���U���g�J�ڏ���
//============================================================
void CWaveManager::ResultTransition(const CGameManager::RESULT result, const int nWait)
{
	// �|�C���^��錾
	CGameManager *m_pGameManager = CManager::GetGameManager();	// �Q�[���}�l�[�W���[

	// �N���A�󋵂�ݒ�
	m_pGameManager->SetResult(result);

	// �l���X�R�A��ݒ�
	m_pGameManager->SetScore(CSceneGame::GetScore()->Get());

	// �o�ߎ��Ԃ�ݒ�
	m_pGameManager->SetTime(CSceneGame::GetTimerManager()->Get());

	// �^�C���̌v�����I��
	CSceneGame::GetTimerManager()->End();

	// �V�[���̐ݒ�
	CManager::SetScene(CScene::MODE_RESULT, nWait);	// ���U���g���
}

//============================================================
//	�G�߂̐ݒ菈��
//============================================================
void CWaveManager::SetSeason(const SEASON season)
{
	// �n�ʂ�ύX
	CScene::GetField()->SetSeason(season);

	// �Ԃ�ύX
	CFlower::SetSeason(season);

	// ����ύX
	CWeed::SetSeason(season);
}

//============================================================
//	�G�߂̊J�n����������
//============================================================
void CWaveManager::InitSeasonStart(void)
{
	// �|�C���^��錾
	CModel *pModel = CManager::GetModel();	// ���f���ւ̃|�C���^

	for (int nCntWave = 0; nCntWave < NUM_MODEL_UI; nCntWave++)
	{ // ���f��UI�̎g�p�����J��Ԃ�

		// ������������
		m_apModelUI[nCntWave]->SetRotation(VEC3_ZERO);

		// �g�嗦��������
		m_apModelUI[nCntWave]->SetScaling(SIZE_SEASON);

		// �`�������ݒ�ɂ���
		m_apModelUI[nCntWave]->SetEnableDraw(true);
	}

	// ���f����o�^�E����
	m_apModelUI[0]->BindModel(pModel->GetModel(pModel->Regist(mc_apModelFile[MODEL_SEASON])));

	// ���f����o�^�E����
	m_apModelUI[1]->BindModel(pModel->GetModel(pModel->Regist(mc_apSeasonModelFile[m_nSeason])));

	// ��Ԃ�ύX
	m_state = STATE_SEASON_START;	// �G�߂̊J�n���
}

//============================================================
//	�G�߂̊J�n����
//============================================================
void CWaveManager::UpdateSeasonStart(void)
{
	if (m_nCounterState <= SEASON_STARTCNT)
	{ // �J�E���^�[�����l�ȉ��̏ꍇ

		if (m_nCounterState <= SEASON_MOVECNT)
		{ // �J�E���^�[�����l�ȉ��̏ꍇ

			for (int nCntWave = 0; nCntWave < NUM_MODEL_UI; nCntWave++)
			{ // ���f��UI�̎g�p�����J��Ԃ�

				// �ϐ���錾
				D3DXVECTOR3 rotSeason = m_apModelUI[nCntWave]->GetRotation();	// �V�[�Y������
				D3DXVECTOR3 scaleSeason = m_apModelUI[nCntWave]->GetScaling();	// �V�[�Y���g�嗦
				float fMul = (nCntWave == 0) ? 1.0f : -1.0f;					// �������Z�ʂ̌v�Z�p�l

				// ���������Z
				rotSeason.x += SEASON_ADD_ROT * fMul;
				rotSeason.y -= SEASON_SUB_ROT * fMul;

				// �����𐳋K��
				useful::NormalizeRot(rotSeason.x);
				useful::NormalizeRot(rotSeason.y);

				// �g�嗦��傫������
				scaleSeason *= SEASON_MUL_SIZE;

				if (scaleSeason.x > 1.0f)
				{ // �g�嗦�� 1.0f�𒴂����ꍇ

					// �g�嗦��␳
					scaleSeason = VEC3_ONE;
				}

				// �����Ɗg�嗦�̍X�V
				m_apModelUI[nCntWave]->SetRotation(rotSeason);
				m_apModelUI[nCntWave]->SetScaling(scaleSeason);
			}
		}

		// ��ԊǗ��J�E���^�[�����Z
		m_nCounterState++;

		if (m_nCounterState == (int)(SEASON_MOVECNT * 0.5f))
		{ // �J�E���^�[�����l�̏ꍇ

			// �T�E���h�̍Đ�
			CManager::GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// ���艹01
		}
	}
	else
	{ // �J�E���^�[�����l���傫���ꍇ

		for (int nCntWave = 0; nCntWave < NUM_MODEL_UI; nCntWave++)
		{ // ���f��UI�̎g�p�����J��Ԃ�

			// �`������Ȃ��ݒ�ɂ���
			m_apModelUI[nCntWave]->SetEnableDraw(false);
		}

		// ��ԊǗ��J�E���^�[��������
		m_nCounterState = 0;

		// ��Ԃ�ύX
		m_state = STATE_WAVE_START_INIT;	// �E�F�[�u�J�n���������
	}
}

//============================================================
//	�E�F�[�u�J�n����������
//============================================================
void CWaveManager::InitWaveStart(void)
{
	// �ϐ���錾
	D3DXVECTOR3 sizeWave = m_pObject2D->GetScaling();	// �E�F�[�u�����傫��
	D3DXVECTOR3 sizeNum = m_pValue->GetScaling();		// �E�F�[�u�����傫��

	// ������ݒ�
	m_pValue->SetNumber(m_nWave + 1);

	// �I�u�W�F�N�g�̃T�C�Y��傫������
	sizeWave *= MUL_SIZEUP;
	sizeNum *= MUL_SIZEUP;

	// �`�������ݒ�ɂ���
	m_pObject2D->SetEnableDraw(true);
	m_pValue->SetEnableDraw(true);

	// ��Ԃ�ύX
	m_state = STATE_WAVE_START;	// �E�F�[�u�J�n���

	// �傫����ݒ�
	m_pObject2D->SetScaling(sizeWave);
	m_pValue->SetScaling(sizeNum);

	// �T�E���h�̍Đ�
	CManager::GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// ���艹01
}

//============================================================
//	�E�F�[�u�J�n����
//============================================================
void CWaveManager::UpdateWaveStart(void)
{	
	// �ϐ���錾
	D3DXVECTOR3 sizeWave = m_pObject2D->GetScaling();	// �E�F�[�u�����傫��
	D3DXVECTOR3 sizeNum = m_pValue->GetScaling();		// �E�F�[�u�����傫��

	if (m_nCounterState <= WAVE_STARTCNT)
	{ // �J�E���^�[�����l�ȉ��̏ꍇ

		// ��ԊǗ��J�E���^�[�����Z
		m_nCounterState++;

		// �I�u�W�F�N�g�̃T�C�Y������������
		sizeWave *= MUL_SIZEDOWN;
		sizeNum *= MUL_SIZEDOWN;

		// �E�F�[�u�����̃T�C�Y��␳
		if (sizeWave.x < SIZE_WAVE.x
		||  sizeWave.y < SIZE_WAVE.y)
		{ // �傫������������ꍇ

			// �傫����␳
			sizeWave.x = SIZE_WAVE.x;
			sizeWave.y = SIZE_WAVE.y;
		}

		// �E�F�[�u�����̃T�C�Y��␳
		if (sizeNum.x < SIZE_NUM.x
		||  sizeNum.y < SIZE_NUM.y)
		{ // �傫������������ꍇ

			// �傫����␳
			sizeNum.x = SIZE_NUM.x;
			sizeNum.y = SIZE_NUM.y;
		}
	}
	else
	{ // �J�E���^�[�����l���傫���ꍇ

		// ��ԊǗ��J�E���^�[��������
		m_nCounterState = 0;

		// �`������Ȃ��ݒ�ɂ���
		m_pObject2D->SetEnableDraw(false);
		m_pValue->SetEnableDraw(false);

		// ��Ԃ�ύX
		m_state = STATE_PROGRESSION;	// �E�F�[�u�i�s���

		// �v���C���[���ďo��������
		CScene::GetPlayer()->SetRespawn(PLAY_SPAWN_POS);

		if (m_nWave == 0)
		{ // ����E�F�[�u�̏ꍇ

			// �G�߂̐ݒ�
			SetSeason((SEASON)m_nSeason);

			// �}�i�t�����[�����_������
			CFlower::RandomSpawn(PLUS_FLOWER, (CFlower::TYPE)m_nSeason);

			// �������_������
			CWeed::RandomSpawn(PLUS_WEED, (CWeed::TYPE)m_nSeason);

			// �J������Ԃ�ݒ�
			CManager::GetCamera()->SetState(CCamera::STATE_BARGAINING);	// ���������
		}

		// �^�C�����v���ĊJ
		CSceneGame::GetTimerManager()->EnableStop(false);
	}

	// �傫����ݒ�
	m_pObject2D->SetScaling(sizeWave);
	m_pValue->SetScaling(sizeNum);
}

//============================================================
//	�E�F�[�u�i�s����
//============================================================
void CWaveManager::UpdateProgression(void)
{
	if (m_nCounterFrame > 0)
	{ // �J�E���^�[�� 0���傫���ꍇ

		if (CEnemy::GetNumAll() <= 0)
		{ // �G���S�ł��Ă���ꍇ

			// �]�C�Ǘ��J�E���^�[��������
			m_nCounterFrame = 0;
		}
		else
		{ // �G���S�ł��Ă��Ȃ��ꍇ

			// �]�C�Ǘ��J�E���^�[�����Z
			m_nCounterFrame--;
		}
	}
	else if (m_nPoint < m_aWaveInfo[m_nSeason].pWave[m_nWave].nNumPoint)
	{ // �J�E���^�[�� 0�ȉ����A�o�����c���Ă���ꍇ

		for (int nCntType = 0; nCntType < CEnemy::TYPE_MAX; nCntType++)
		{ // �G�̎�ނ̑������J��Ԃ�

			// �G�̎�ނ��ƂɓG�������_���X�|�[��������
			CEnemy::RandomSpawn(m_aWaveInfo[m_nSeason].pWave[m_nWave].pPoint[m_nPoint].aNumSpawn[nCntType], (CEnemy::TYPE)nCntType);
		}

		// �o���x����\��
		CSceneGame::GetWarningSpawn()->SetDisp();

		// �]�C�Ǘ��J�E���^�[��ݒ�
		m_nCounterFrame = m_aWaveInfo[m_nSeason].pWave[m_nWave].pPoint[m_nPoint].nFrame;

		// �o���Ǘ��J�E���^�[�����Z
		m_nPoint++;

		// �T�E���h�̍Đ�
		CManager::GetSound()->Play(CSound::LABEL_SE_WARNING);	// �x����
	}
	else
	{ // �J�E���^�[�� 0�ȉ����A�S�o�����I�������ꍇ

		if (m_nCounterState <= NEXT_WAITCNT)
		{ // �J�E���^�[�����l�ȉ��̏ꍇ

			// ��ԊǗ��J�E���^�[�����Z
			m_nCounterState++;
		}
		else
		{ // �J�E���^�[�����l���傫���ꍇ

			// �G�̑S����
			CEnemy::SetAllVanish();

			// �v���C���[��\�����Ȃ���Ԃɂ���
			CScene::GetPlayer()->SetDisp(false);

			// �^�C�����v����~
			CSceneGame::GetTimerManager()->EnableStop(true);

			// ��ԊǗ��J�E���^�[��������
			m_nCounterState = 0;

			// �E�F�[�u�Ǘ��J�E���^�[�����Z
			m_nWave++;

			if (m_nWave < m_aWaveInfo[m_nSeason].nNumWave)
			{ // �E�F�[�u���܂�����ꍇ

				// �J�E���^�[��������
				m_nPoint = 0;	// �o���Ǘ��J�E���^�[

				// ��Ԃ�ύX
				m_state = STATE_WAVE_START_INIT;	// �E�F�[�u�J�n���������
			}
			else
			{ // �S�E�F�[�u���I�������ꍇ

				// �J�E���^�[��������
				m_nWave = 0;	// �E�F�[�u�Ǘ��J�E���^�[
				m_nPoint = 0;	// �o���Ǘ��J�E���^�[

				// ��Ԃ�ύX
				m_state = STATE_SEASON_END_INIT;	// �G�߂̏I�����������
			}
		}
	}
}

//============================================================
//	�G�߂̏I������������
//============================================================
void CWaveManager::InitWaveEnd(void)
{
	// �|�C���^��錾
	CModel *pModel = CManager::GetModel();	// ���f���ւ̃|�C���^

	for (int nCntWave = 0; nCntWave < NUM_MODEL_UI; nCntWave++)
	{ // ���f��UI�̎g�p�����J��Ԃ�

		// ������������
		m_apModelUI[nCntWave]->SetRotation(VEC3_ZERO);

		// �g�嗦��������
		m_apModelUI[nCntWave]->SetScaling(SIZE_SEASON);

		// �`�������ݒ�ɂ���
		m_apModelUI[nCntWave]->SetEnableDraw(true);

		// ���f����o�^�E����
		m_apModelUI[nCntWave]->BindModel(pModel->GetModel(pModel->Regist(mc_apModelFile[nCntWave])));
	}

	// ��Ԃ�ύX
	m_state = STATE_SEASON_END;	// �G�߂̏I�����
}

//============================================================
//	�G�߂̏I������
//============================================================
void CWaveManager::UpdateWaveEnd(void)
{
	if (m_nCounterState <= SEASON_STARTCNT)
	{ // �J�E���^�[�����l�ȉ��̏ꍇ

		if (m_nCounterState <= SEASON_MOVECNT)
		{ // �J�E���^�[�����l�ȉ��̏ꍇ

			for (int nCntWave = 0; nCntWave < NUM_MODEL_UI; nCntWave++)
			{ // ���f��UI�̎g�p�����J��Ԃ�

				// �ϐ���錾
				D3DXVECTOR3 rotSeason = m_apModelUI[nCntWave]->GetRotation();	// �V�[�Y������
				D3DXVECTOR3 scaleSeason = m_apModelUI[nCntWave]->GetScaling();	// �V�[�Y���g�嗦
				float fMul = (nCntWave == 0) ? 1.0f : -1.0f;					// �������Z�ʂ̌v�Z�p�l

				// ���������Z
				rotSeason.x += SEASON_ADD_ROT * fMul;
				rotSeason.y -= SEASON_SUB_ROT * fMul;

				// �����𐳋K��
				useful::NormalizeRot(rotSeason.x);
				useful::NormalizeRot(rotSeason.y);

				// �g�嗦��傫������
				scaleSeason *= SEASON_MUL_SIZE;

				if (scaleSeason.x > 1.0f)
				{ // �g�嗦�� 1.0f�𒴂����ꍇ

					// �g�嗦��␳
					scaleSeason = VEC3_ONE;
				}

				// �����Ɗg�嗦�̍X�V
				m_apModelUI[nCntWave]->SetRotation(rotSeason);
				m_apModelUI[nCntWave]->SetScaling(scaleSeason);
			}
		}

		// ��ԊǗ��J�E���^�[�����Z
		m_nCounterState++;

		if (m_nCounterState == (int)(SEASON_MOVECNT * 0.5f))
		{ // �J�E���^�[�����l�̏ꍇ

			// �T�E���h�̍Đ�
			CManager::GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// ���艹01
		}
	}
	else
	{ // �J�E���^�[�����l���傫���ꍇ

		for (int nCntWave = 0; nCntWave < NUM_MODEL_UI; nCntWave++)
		{ // ���f��UI�̎g�p�����J��Ԃ�

			// �`������Ȃ��ݒ�ɂ���
			m_apModelUI[nCntWave]->SetEnableDraw(false);
		}

		// ��ԊǗ��J�E���^�[��������
		m_nCounterState = 0;

		// �G�ߊǗ��J�E���^�[�����Z
		m_nSeason++;

		if (m_nSeason < SEASON_MAX)
		{ // �G�߂��܂�����ꍇ

			// ��Ԃ�ύX
			m_state = STATE_WAIT;	// ���G�߂̊J�n�ҋ@���
		}
		else
		{ // �G�߂������Ȃ��ꍇ

			// ��Ԃ�ύX
			m_state = STATE_END;	// �I�����
		}
	}
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
void CWaveManager::LoadSetup(void)
{
	// �ϐ���錾
	int nSeason	= 0;	// �G�߂̐ݒ�p
	int nWave	= 0;	// �E�F�[�u�̐ݒ�p
	int nPoint	= 0;	// �o���̐ݒ�p
	int nType	= 0;	// �o����ނ̑���p
	int nEnd	= 0;	// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	// �ÓI�����o�ϐ��̏����N���A
	memset(&m_aWaveInfo[0], 0, sizeof(m_aWaveInfo));	// �E�F�[�u���

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(WAVE_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			// �E�F�[�u�̐ݒ�
			if (strcmp(&aString[0], "SEASONSET") == 0)
			{ // �ǂݍ��񂾕����� SEASONSET �̏ꍇ

				// �E�F�[�u�ԍ���������
				nWave = 0;

				do
				{ // �ǂݍ��񂾕����� END_SEASONSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "NUM_WAVE") == 0)
					{ // �ǂݍ��񂾕����� NUM_WAVE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &m_aWaveInfo[nSeason].nNumWave);	// �E�F�[�u����ǂݍ���

						if (UNUSED(m_aWaveInfo[nSeason].pWave))
						{ // ��g�p���̏ꍇ

							// �E�F�[�u�����������m��
							m_aWaveInfo[nSeason].pWave = new Wave[m_aWaveInfo[nSeason].nNumWave];

							if (USED(m_aWaveInfo[nSeason].pWave))
							{ // �m�ۂɐ��������ꍇ

								// �������N���A
								memset(m_aWaveInfo[nSeason].pWave, 0, sizeof(Wave) * m_aWaveInfo[nSeason].nNumWave);
							}
							else { assert(false); }	// �m�ێ��s
						}
						else { assert(false); }	// �g�p��
					}
					else if (strcmp(&aString[0], "WAVE") == 0)
					{ // �ǂݍ��񂾕����� WAVE �̏ꍇ

						// �o���ԍ���������
						nPoint = 0;

						do
						{ // �ǂݍ��񂾕����� END_WAVE �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "NUM_POINT") == 0)
							{ // �ǂݍ��񂾕����� NUM_POINT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);									// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aWaveInfo[nSeason].pWave[nWave].nNumPoint);	// �o������ǂݍ���

								if (UNUSED(m_aWaveInfo[nSeason].pWave[nWave].pPoint))
								{ // ��g�p���̏ꍇ

									// �o�������������m��
									m_aWaveInfo[nSeason].pWave[nWave].pPoint = new Point[m_aWaveInfo[nSeason].pWave[nWave].nNumPoint];

									if (USED(m_aWaveInfo[nSeason].pWave[nWave].pPoint))
									{ // �m�ۂɐ��������ꍇ

										// �������N���A
										memset(m_aWaveInfo[nSeason].pWave[nWave].pPoint, 0, sizeof(Point) * m_aWaveInfo[nSeason].pWave[nWave].nNumPoint);
									}
									else { assert(false); }	// �m�ێ��s
								}
								else { assert(false); }	// �g�p��
							}
							else if (strcmp(&aString[0], "POINT") == 0)
							{ // �ǂݍ��񂾕����� POINT �̏ꍇ

								do
								{ // �ǂݍ��񂾕����� END_POINT �ł͂Ȃ��ꍇ���[�v

									// �t�@�C�����當�����ǂݍ���
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "FRAME") == 0)
									{ // �ǂݍ��񂾕����� FRAME �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%d", &m_aWaveInfo[nSeason].pWave[nWave].pPoint[nPoint].nFrame);	// �P�\�t���[����ǂݍ���
									}
									else if (strcmp(&aString[0], "SPAWN") == 0)
									{ // �ǂݍ��񂾕����� SPAWN �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%d", &nType);		// �o����ނ�ǂݍ���
										fscanf(pFile, "%d", &m_aWaveInfo[nSeason].pWave[nWave].pPoint[nPoint].aNumSpawn[nType]);	// �o������ǂݍ���
									}
								} while (strcmp(&aString[0], "END_POINT") != 0);	// �ǂݍ��񂾕����� END_POINT �ł͂Ȃ��ꍇ���[�v

								// ��O����
								assert(nPoint < m_aWaveInfo[nSeason].pWave[nWave].nNumPoint);	// �o���I�[�o�[

								// �o���ԍ������Z
								nPoint++;
							}
						} while (strcmp(&aString[0], "END_WAVE") != 0);	// �ǂݍ��񂾕����� END_WAVE �ł͂Ȃ��ꍇ���[�v

						// ��O����
						assert(nPoint == m_aWaveInfo[nSeason].pWave[nWave].nNumPoint);	// �o�����ݒ�
						assert(nWave < m_aWaveInfo[nSeason].nNumWave);	// �E�F�[�u�I�[�o�[

						// �E�F�[�u�ԍ������Z
						nWave++;
					}
				} while (strcmp(&aString[0], "END_SEASONSET") != 0);	// �ǂݍ��񂾕����� END_SEASONSET �ł͂Ȃ��ꍇ���[�v

				// ��O����
				assert(nWave == m_aWaveInfo[nSeason].nNumWave);	// �E�F�[�u���ݒ�
				assert(nSeason < SEASON_MAX);	// �G�߃I�[�o�[

				// �G�ߔԍ������Z
				nSeason++;
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

		// ��O����
		assert(nSeason == SEASON_MAX);	// �G�ߖ��ݒ�

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�E�F�[�u�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}
