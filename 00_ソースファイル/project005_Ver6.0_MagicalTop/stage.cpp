//============================================================
//
//	�X�e�[�W���� [stage.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "stage.h"
#include "manager.h"
#include "texture.h"
#include "scene.h"
#include "sceneGame.h"
#include "player.h"
#include "target.h"
#include "field.h"
#include "hurricane.h"
#include "object3D.h"
#include "collision.h"
#include "wind.h"

//************************************************************
//	�}�N����`
//************************************************************
#define STAGE_SETUP_TXT	"data\\TXT\\stage.txt"	// �Z�b�g�A�b�v�e�L�X�g���΃p�X

#define AREA_PRIO	(2)			// �G���A�\���̗D�揇��
#define AREA_ROT	(0.025f)	// �G���A�̉�]��
#define AREA_SUBROT	(0.003f)	// �G���A�̉�]���Z��

#define WIND_POS	(3000.0f)	// �������ʒu
#define WIND_CNT	(60)		// �������J�E���g
#define WIND_SPAWN	(4)			// ��������

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CStage::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\area002.png",	// �o���A�\���e�N�X�`��
	"data\\TEXTURE\\area000.png",	// �G���A�\���e�N�X�`��
};

//************************************************************
//	�e�N���X [CStage] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CStage::CStage()
{
	// �����o�ϐ����N���A
	m_pHurricane = NULL;	// �n���P�[���̏��
	m_pStageBarrier = NULL;	// �o���A�\���̏��
	m_pStageArea = NULL;	// �X�e�[�W�G���A�\���̏��
	m_area = AREA_NONE;		// �v���C���[�̌��݃G���A
	m_bWind = false;		// ��������
	memset(&m_stageWind, 0, sizeof(m_stageWind));		// ����
	memset(&m_stageBarrier, 0, sizeof(m_stageBarrier));	// �o���A
	memset(&m_aStageArea[0], 0, sizeof(m_aStageArea));	// �G���A
	memset(&m_stageLimit, 0, sizeof(m_stageLimit));		// �͈�
}

//============================================================
//	�f�X�g���N�^
//============================================================
CStage::~CStage()
{

}

//============================================================
//	����������
//============================================================
HRESULT CStage::Init(void)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// �����o�ϐ���������
	m_pHurricane = NULL;	// �n���P�[���̏��
	m_pStageBarrier = NULL;	// �o���A�\���̏��
	m_pStageArea = NULL;	// �X�e�[�W�G���A�\���̏��
	m_area = AREA_NONE;		// �v���C���[�̌��݃G���A
	m_bWind = true;			// ��������
	memset(&m_stageWind, 0, sizeof(m_stageWind));		// ����
	memset(&m_stageBarrier, 0, sizeof(m_stageBarrier));	// �o���A
	memset(&m_aStageArea[0], 0, sizeof(m_aStageArea));	// �G���A
	memset(&m_stageLimit, 0, sizeof(m_stageLimit));		// �͈�

	// �n���P�[���̐���
	m_pHurricane = CHurricane::Create(VEC3_ZERO);
	if (UNUSED(m_pHurricane))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �X�e�[�W�G���A�\���̐���
	m_pStageArea = CObject3D::Create
	( // ����
		VEC3_ZERO,					// �ʒu
		VEC3_ZERO,					// �傫��
		VEC3_ZERO,					// ����
		XCOL_WHITE,					// �F
		CObject3D::ORIGIN_CENTER,	// ���_
		D3DCULL_CCW,				// �J�����O��
		false,						// ���C�e�B���O��
		D3DCMP_ALWAYS,				// Z�e�X�g�ݒ�
		false						// Z�o�b�t�@�̎g�p��
	);
	if (UNUSED(m_pStageArea))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pStageArea->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_AREA]));

	// �D�揇�ʂ�ݒ�
	m_pStageArea->SetPriority(AREA_PRIO);

	// �X�e�[�W�o���A�\���̐���
	m_pStageBarrier = CObject3D::Create
	( // ����
		VEC3_ZERO,					// �ʒu
		VEC3_ZERO,					// �傫��
		VEC3_ZERO,					// ����
		XCOL_WHITE,					// �F
		CObject3D::ORIGIN_CENTER,	// ���_
		D3DCULL_CCW,				// �J�����O��
		false,						// ���C�e�B���O��
		D3DCMP_ALWAYS,				// Z�e�X�g�ݒ�
		false						// Z�o�b�t�@�̎g�p��
	);
	if (UNUSED(m_pStageBarrier))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pStageBarrier->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_BARRIER]));

	// �D�揇�ʂ�ݒ�
	m_pStageBarrier->SetPriority(AREA_PRIO);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CStage::Uninit(void)
{
	// �n���P�[���̏I��
	m_pHurricane->Uninit();

	// �X�e�[�W�o���A�\���̏I��
	m_pStageBarrier->Uninit();

	// �X�e�[�W�G���A�\���̏I��
	m_pStageArea->Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CStage::Update(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posCenter;	// ���E�̒��S�ʒu
	D3DXVECTOR3 rotBarrier = m_pStageBarrier->GetRotation();	// �o���A�\������

	// ���E�̒��S�ʒu��ݒ�
	posCenter = m_stageLimit.center;	// ���S���W��ݒ�
	posCenter.y = CScene::GetField()->GetPosition().y;	// �n�ʂ̏c�ʒu��ݒ�

	//--------------------------------------------------------
	//	�n���P�[���̍X�V
	//--------------------------------------------------------
	// �n���P�[���̈ʒu��ݒ�
	m_pHurricane->SetPosition(posCenter);

	// �n���P�[���̍X�V
	m_pHurricane->Update();

	//--------------------------------------------------------
	//	�o���A�\���̍X�V
	//--------------------------------------------------------
	// �o���A�\���̈ʒu��ݒ�
	m_pStageBarrier->SetPosition(posCenter);

	// �o���A�\���̌�����ݒ�
	rotBarrier.y -= AREA_ROT;
	m_pStageBarrier->SetRotation(rotBarrier);

	//--------------------------------------------------------
	//	���̐���
	//--------------------------------------------------------
	if (m_bWind)
	{ // ���𐶐�����ꍇ

		if (m_stageWind.nCounter < WIND_CNT)
		{ // �J�E���^�[�����l��菬�����ꍇ

			// �J�E���^�[�����Z
			m_stageWind.nCounter++;
		}
		else
		{ // �J�E���^�[�����l�ȏ�̏ꍇ

			// �ϐ���錾
			D3DXVECTOR3 spawnPos;	// �����ʒu
			float fRot = (float)(rand() % 629 - 314) * 0.01f;	// ����

			// �J�E���^�[��������
			m_stageWind.nCounter = 0;

			for (int nCntWind = 0; nCntWind < WIND_SPAWN; nCntWind++)
			{ // ���̐��������J��Ԃ�

				// �����ʒu��ݒ�
				spawnPos.x = sinf(fRot + ((D3DX_PI * 0.5f) * nCntWind)) * WIND_POS;
				spawnPos.y = 0.0f;
				spawnPos.z = cosf(fRot + ((D3DX_PI * 0.5f) * nCntWind)) * WIND_POS;

				// ���̐���
				CWind::Create(spawnPos);
			}
		}
	}

	//--------------------------------------------------------
	//	�G���A�\���̍X�V
	//--------------------------------------------------------
	if (USED(CScene::GetPlayer()) && USED(CScene::GetTarget()))
	{ // �v���C���[�E�^�[�Q�b�g���g�p����Ă���ꍇ

		// �ϐ���錾
		D3DXVECTOR3 posPlayer = CScene::GetPlayer()->GetPosition();	// �v���C���[�ʒu
		D3DXVECTOR3 posTarget = CScene::GetTarget()->GetPosition();	// �^�[�Q�b�g�ʒu
		float fRadiusPlayer = CScene::GetPlayer()->GetRadius();		// �v���C���[���a

		// ���݂̃G���A��������
		m_area = AREA_NONE;

		for (int nCntArea = 0; nCntArea < AREA_MAX; nCntArea++)
		{ // �G���A�̍ő吔���J��Ԃ�

			if (collision::Circle2D(posPlayer, posTarget, fRadiusPlayer, m_aStageArea[nCntArea].fRadius))
			{ // �G���A���̏ꍇ

				// �G���A�\���̑傫����ݒ�
				m_pStageArea->SetScaling(D3DXVECTOR3(m_aStageArea[nCntArea].fRadius * 2.0f, 0.0f, m_aStageArea[nCntArea].fRadius * 2.0f));

				// �G���A�\���̐F��ݒ�
				m_pStageArea->SetColor(m_aStageArea[nCntArea].col);

				// ���݂̃G���A��ݒ�
				m_area = (AREA)nCntArea;

				// �����𔲂���
				break;
			}
		}

		// �G���A�\���̈ʒu��ݒ�
		m_pStageArea->SetPosition(posCenter);

		// �G���A�\���̌�����ݒ�
		m_pStageArea->SetRotation(D3DXVECTOR3(0.0f, atan2f(posPlayer.x - posTarget.x, posPlayer.z - posTarget.z), 0.0f));

		// ��O����
		assert(m_area != AREA_NONE);	// �G���A�O
	}
}

//============================================================
//	�o���A�Ƃ̓����蔻��
//============================================================
bool CStage::CollisionBarrier(D3DXVECTOR3& rPos, float fRadius)
{
	// �ϐ���錾
	bool bHit = false;	// ����m�F�p

	// �~�̓����蔻��
	bHit = collision::Circle2D
	( // ����
		rPos,							// ����ʒu
		m_pStageBarrier->GetPosition(),	// ����ڕW�ʒu
		fRadius,						// ���蔼�a
		m_stageBarrier.fRadius			// ����ڕW���a
	);

	// ����󋵂�Ԃ�
	return bHit;
}

//============================================================
//	�ʒu�̕␳����
//============================================================
void CStage::LimitPosition(D3DXVECTOR3& rPos, const float fRadius)
{
	switch (m_stageLimit.mode)
	{ // �������[�h���Ƃ̏���
	case LIMIT_BOX:		// ��`�͈�

		// �p���̓�������
		collision::InBoxPillar
		( // ����
			rPos,				// ����ʒu
			VEC3_ZERO,			// ���茴�_�ʒu
			VEC3_ALL(fRadius),	// ����T�C�Y(�E�E��E��)
			VEC3_ALL(fRadius),	// ����T�C�Y(���E���E�O)
			D3DXVECTOR3(fabsf(m_stageLimit.fRight), 0.0f, fabsf(m_stageLimit.fFar)),	// ���茴�_�T�C�Y(�E�E��E��)
			D3DXVECTOR3(fabsf(m_stageLimit.fLeft),  0.0f, fabsf(m_stageLimit.fNear))	// ���茴�_�T�C�Y(���E���E�O)
		);

		break;

	case LIMIT_CIRCLE:	// �~�͈�

		// �~���̓�������
		collision::InCirclePillar
		( // ����
			rPos,					// ����ʒu
			VEC3_ZERO,				// ���茴�_�ʒu
			fRadius,				// ���蔼�a
			m_stageLimit.fRadius	// ���茴�_���a
		);

		break;

	default:	// ��O����
		assert(false);
		break;
	}
}

//============================================================
//	�͈͊O�̒��n����
//============================================================
bool CStage::LandPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove, const float fHeight)
{
	if (rPos.y - fHeight < m_stageLimit.fField)
	{ // �ʒu���n�ʂ�艺�̏ꍇ

		// �ʒu��␳
		rPos.y = m_stageLimit.fField + fHeight;

		// �ړ��ʂ�������
		rMove.y = 0.0f;

		// ���n���Ă���󋵂�Ԃ�
		return true;
	}

	// ���n���Ă��Ȃ��󋵂�Ԃ�
	return false;
}

//============================================================
//	�X�e�[�W�͈͂̐ݒ菈��
//============================================================
void CStage::SetStageLimit(const StageLimit& rLimit)
{
	// �����̃X�e�[�W�͈͂�ݒ�
	m_stageLimit = rLimit;
}

//============================================================
//	�X�e�[�W�͈͎擾����
//============================================================
CStage::StageLimit CStage::GetStageLimit(void) const
{
	// �X�e�[�W�͈͂�Ԃ�
	return m_stageLimit;
}

//============================================================
//	�X�e�[�W�G���A�̐ݒ菈��
//============================================================
void CStage::SetStageArea(const int nID, const StageArea& rArea)
{
	// �����C���f�b�N�X�̃X�e�[�W�G���A��ݒ�
	m_aStageArea[nID] = rArea;
}

//============================================================
//	�X�e�[�W�G���A�擾����
//============================================================
CStage::StageArea CStage::GetStageArea(const int nID) const
{
	// �����C���f�b�N�X�̃X�e�[�W�G���A��Ԃ�
	return m_aStageArea[nID];
}

//============================================================
//	�X�e�[�W�G���A�̕`��ݒ菈��
//============================================================
void CStage::SetEnableDrawArea(const bool bDraw)
{
	// �����̕`��󋵂��X�e�[�W�G���A�ɐݒ�
	m_pStageArea->SetEnableDraw(bDraw);
}

//============================================================
//	�v���C���[�̌��݃G���A�擾����
//============================================================
CStage::AREA CStage::GetAreaPlayer(void) const
{
	// �v���C���[�̌��݃G���A��Ԃ�
	return m_area;
}

//============================================================
//	�X�e�[�W�o���A�̐ݒ菈��
//============================================================
void CStage::SetStageBarrier(const StageArea& rBarrier)
{
	// �����̃X�e�[�W�o���A��ݒ�
	m_stageBarrier = rBarrier;

	// �G���A�\���̑傫����ݒ�
	m_pStageBarrier->SetScaling(D3DXVECTOR3(m_stageBarrier.fRadius * 2.0f, 0.0f, m_stageBarrier.fRadius * 2.0f));

	// �G���A�\���̐F��ݒ�
	m_pStageBarrier->SetColor(m_stageBarrier.col);
}

//============================================================
//	�X�e�[�W�o���A�擾����
//============================================================
CStage::StageArea CStage::GetStageBarrier(void) const
{
	// �X�e�[�W�o���A��Ԃ�
	return m_stageBarrier;
}

//============================================================
//	�X�e�[�W�o���A�̕`��ݒ菈��
//============================================================
void CStage::SetEnableDrawBarrier(const bool bDraw)
{
	// �����̕`��󋵂��X�e�[�W�o���A�ɐݒ�
	m_pStageBarrier->SetEnableDraw(bDraw);
}

//============================================================
//	�X�e�[�W�o���A�̈ʒu�擾����
//============================================================
D3DXVECTOR3 CStage::GetStageBarrierPosition(void) const
{
	// �o���A�̈ʒu��Ԃ�
	return m_pStageBarrier->GetPosition();
}

//============================================================
//	�n���P�[���̕`��ݒ菈��
//============================================================
void CStage::SetEnableDrawHurricane(const bool bDraw)
{
	// �����̕`��󋵂��n���P�[���ɐݒ�
	m_pHurricane->SetEnableDraw(bDraw);
}

//============================================================
//	���̐����ݒ菈��
//============================================================
void CStage::SetEnebleCreateWind(const bool bCreate)
{
	// �����̐����󋵂�ݒ�
	m_bWind = bCreate;
}

//============================================================
//	��������
//============================================================
CStage *CStage::Create(void)
{
	// �|�C���^��錾
	CStage *pStage = NULL;		// �X�e�[�W�����p

	if (UNUSED(pStage))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pStage = new CStage;	// �X�e�[�W
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pStage))
	{ // �m�ۂɐ������Ă���ꍇ

		// �X�e�[�W�̏�����
		if (FAILED(pStage->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���s��Ԃ�
			return NULL;
		}

		// �Z�b�g�A�b�v�̓ǂݍ���
		LoadSetup(pStage);

		// �m�ۂ����A�h���X��Ԃ�
		return pStage;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CStage::Release(CStage *&prStage)
{
	if (USED(prStage))
	{ // �g�p���̏ꍇ

		// �X�e�[�W�̏I��
		prStage->Uninit();

		// �������J��
		delete prStage;
		prStage = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
void CStage::LoadSetup(CStage *pStage)
{
	// �ϐ���錾
	StageArea stageBarrier;	// �X�e�[�W�o���A�̑���p
	StageArea stageArea;	// �X�e�[�W�G���A�̑���p
	StageLimit stageLimit;	// �X�e�[�W�͈͂̑���p
	int nArea = 0;			// �G���A�̓ǂݍ��ݐ�
	int nEnd = 0;			// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(STAGE_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			// �X�e�[�W�͈͂̐ݒ�
			if (strcmp(&aString[0], "LIMITSET") == 0)
			{ // �ǂݍ��񂾕����� LIMITSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_LIMITSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "CENTER") == 0)
					{ // �ǂݍ��񂾕����� CENTER �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &stageLimit.center.x);	// ���S���WX��ǂݍ���
						fscanf(pFile, "%f", &stageLimit.center.y);	// ���S���WY��ǂݍ���
						fscanf(pFile, "%f", &stageLimit.center.z);	// ���S���WZ��ǂݍ���
					}
					else if (strcmp(&aString[0], "NEAR") == 0)
					{ // �ǂݍ��񂾕����� NEAR �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &stageLimit.fNear);		// �O�ʒu��ǂݍ���

						// �������[�h����`�͈͂ɐݒ�
						stageLimit.mode = LIMIT_BOX;
					}
					else if (strcmp(&aString[0], "FAR") == 0)
					{ // �ǂݍ��񂾕����� FAR �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &stageLimit.fFar);		// ��ʒu��ǂݍ���

						// �������[�h����`�͈͂ɐݒ�
						stageLimit.mode = LIMIT_BOX;
					}
					else if (strcmp(&aString[0], "RIGHT") == 0)
					{ // �ǂݍ��񂾕����� RIGHT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &stageLimit.fRight);	// �E�ʒu��ǂݍ���

						// �������[�h����`�͈͂ɐݒ�
						stageLimit.mode = LIMIT_BOX;
					}
					else if (strcmp(&aString[0], "LEFT") == 0)
					{ // �ǂݍ��񂾕����� LEFT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &stageLimit.fLeft);		// ���ʒu��ǂݍ���

						// �������[�h����`�͈͂ɐݒ�
						stageLimit.mode = LIMIT_BOX;
					}
					else if (strcmp(&aString[0], "RADIUS") == 0)
					{ // �ǂݍ��񂾕����� RADIUS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &stageLimit.fRadius);	// ���a��ǂݍ���

						// �������[�h���~�͈͂ɐݒ�
						stageLimit.mode = LIMIT_CIRCLE;
					}
					else if (strcmp(&aString[0], "FIELD") == 0)
					{ // �ǂݍ��񂾕����� FIELD �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &stageLimit.fField);	// �n�ʈʒu��ǂݍ���
					}
				} while (strcmp(&aString[0], "END_LIMITSET") != 0);	// �ǂݍ��񂾕����� END_LIMITSET �ł͂Ȃ��ꍇ���[�v

				// �X�e�[�W�͈͂̐ݒ�
				pStage->SetStageLimit(stageLimit);

				// ��O����
				assert(stageLimit.mode == CStage::LIMIT_CIRCLE);	// �͈͐����G���[
			}

			// �X�e�[�W�o���A�̐ݒ�
			else if (strcmp(&aString[0], "BARRIERSET") == 0)
			{ // �ǂݍ��񂾕����� BARRIERSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_BARRIERSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "COL") == 0)
					{ // �ǂݍ��񂾕����� COL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &stageBarrier.col.r);	// �ԐF��ǂݍ���
						fscanf(pFile, "%f", &stageBarrier.col.g);	// �ΐF��ǂݍ���
						fscanf(pFile, "%f", &stageBarrier.col.b);	// �F��ǂݍ���
						fscanf(pFile, "%f", &stageBarrier.col.a);	// ���l��ǂݍ���
					}
					else if (strcmp(&aString[0], "RADIUS") == 0)
					{ // �ǂݍ��񂾕����� RADIUS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &stageBarrier.fRadius);	// ���a��ǂݍ���
					}
				} while (strcmp(&aString[0], "END_BARRIERSET") != 0);	// �ǂݍ��񂾕����� END_BARRIERSET �ł͂Ȃ��ꍇ���[�v

				// �X�e�[�W�o���A�̐ݒ�
				pStage->SetStageBarrier(stageBarrier);
			}

			// �X�e�[�W�G���A�̐ݒ�
			else if (strcmp(&aString[0], "AREASET") == 0)
			{ // �ǂݍ��񂾕����� AREASET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_AREASET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "AREA") == 0)
					{ // �ǂݍ��񂾕����� AREA �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_AREA �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "COL") == 0)
							{ // �ǂݍ��񂾕����� COL �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &stageArea.col.r);		// �ԐF��ǂݍ���
								fscanf(pFile, "%f", &stageArea.col.g);		// �ΐF��ǂݍ���
								fscanf(pFile, "%f", &stageArea.col.b);		// �F��ǂݍ���
								fscanf(pFile, "%f", &stageArea.col.a);		// ���l��ǂݍ���
							}
							else if (strcmp(&aString[0], "RADIUS") == 0)
							{ // �ǂݍ��񂾕����� RADIUS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &stageArea.fRadius);	// ���a��ǂݍ���
							}
						} while (strcmp(&aString[0], "END_AREA") != 0);	// �ǂݍ��񂾕����� END_AREA �ł͂Ȃ��ꍇ���[�v

						// ��O����
						assert(nArea < AREA_MAX);	// �G���A���I�[�o�[

						// �X�e�[�W�G���A�̐ݒ�
						pStage->SetStageArea(nArea, stageArea);

						// �G���A�������Z
						nArea++;
					}
				} while (strcmp(&aString[0], "END_AREASET") != 0);	// �ǂݍ��񂾕����� END_AREASET �ł͂Ȃ��ꍇ���[�v

				// ��O����
				assert(nArea == AREA_MAX);	// �G���A���ݒ�
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�X�e�[�W�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}
