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
#include "sceneGame.h"
#include "player.h"
#include "target.h"
#include "object3D.h"
#include "collision.h"

//************************************************************
//	�}�N����`
//************************************************************
#define STAGE_SETUP_TXT	"data\\TXT\\stage.txt"	// �Z�b�g�A�b�v�e�L�X�g���΃p�X
#define AREA_PRIO	(2)	// �G���A�\���̗D�揇��

//************************************************************
//	�e�N���X [CStage] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CStage::CStage()
{
	// �����o�ϐ����N���A
	m_pStageArea = NULL;	// �X�e�[�W�G���A�\���̏��
	memset(&m_stageArea, 0, sizeof(m_stageArea));	// �X�e�[�W�G���A
	memset(&m_stageLimit, 0, sizeof(m_stageLimit));	// �X�e�[�W�͈�
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
	m_pStageArea = NULL;	// �X�e�[�W�G���A�\���̏��
	memset(&m_stageArea, 0, sizeof(m_stageArea));	// �X�e�[�W�G���A
	memset(&m_stageLimit, 0, sizeof(m_stageLimit));	// �X�e�[�W�͈�

	// �X�e�[�W�G���A�\���̐���
	m_pStageArea = CObject3D::Create
	( // ����
		VEC3_ZERO,		// �ʒu
		VEC3_ZERO,		// �傫��
		VEC3_ZERO,		// ����
		XCOL_WHITE,		// �F
		false,			// ���C�e�B���O��
		D3DCMP_ALWAYS,	// Z�e�X�g�ݒ�
		false			// Z�o�b�t�@�̎g�p��
	);
	if (UNUSED(m_pStageArea))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pStageArea->BindTexture(pTexture->Regist("data\\TEXTURE\\area000.png"));	// TODO

	// �D�揇�ʂ�ݒ�
	m_pStageArea->SetPriority(AREA_PRIO);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CStage::Uninit(void)
{
	// �X�e�[�W�G���A�\����j��
	m_pStageArea->Uninit();

	if (USED(m_stageArea.pInfo))
	{ // �X�e�[�W�G���A���g�p����Ă���ꍇ

		// �X�e�[�W�G���A�̃������J��
		delete[] m_stageArea.pInfo;
		m_stageArea.pInfo = NULL;
	}
}

//============================================================
//	�X�V����
//============================================================
void CStage::Update(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posPlayer = CSceneGame::GetPlayer()->GetPosition();	// �v���C���[�ʒu
	D3DXVECTOR3 posTarget = CSceneGame::GetTarget()->GetPosition();	// �^�[�Q�b�g�ʒu
	float fRadiusPlayer = CSceneGame::GetPlayer()->GetRadius();		// �v���C���[���a

	// �G���A�\���̈ʒu��ݒ�
	m_pStageArea->SetPosition(posTarget);

	for (int nCntArea = 0; nCntArea < m_stageArea.nNumArea; nCntArea++)
	{ // �G���A�̑������J��Ԃ�

		if (collision::Circle2D(posPlayer, posTarget, fRadiusPlayer, m_stageArea.pInfo[nCntArea].fRadius))
		{ // �G���A���̏ꍇ

			// �G���A�\���̑傫����ݒ�
			m_pStageArea->SetScaling(D3DXVECTOR3(m_stageArea.pInfo[nCntArea].fRadius * 2.0f, 0.0f, m_stageArea.pInfo[nCntArea].fRadius * 2.0f));

			// �G���A�\���̐F��ݒ�
			m_pStageArea->SetColor(m_stageArea.pInfo[nCntArea].col);

			// �����𔲂���
			break;
		}
	}
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
void CStage::SetStageArea(const StageArea& rArea)
{
	if (UNUSED(m_stageArea.pInfo))
	{ // ��g�p���̏ꍇ

		// �����̃G���A�̑������������m��
		m_stageArea.pInfo = new AreaInfo[rArea.nNumArea];

		if (USED(m_stageArea.pInfo))
		{ // �m�ۂɐ��������ꍇ

			// �������N���A
			memset(m_stageArea.pInfo, 0, sizeof(AreaInfo) * rArea.nNumArea);
		}
		else { assert(false); }	// �m�ێ��s
	}
	else { assert(false); }	// �g�p��

	// �����̃X�e�[�W�G���A�̑�����ݒ�
	m_stageArea.nNumArea = rArea.nNumArea;

	// �����̃X�e�[�W�G���A�̏���ݒ�
	for (int nCntArea = 0; nCntArea < rArea.nNumArea; nCntArea++)
	{ // �G���A�̑������J��Ԃ�

		m_stageArea.pInfo[nCntArea] = rArea.pInfo[nCntArea];
	}
}

//============================================================
//	�X�e�[�W�G���A�擾����
//============================================================
CStage::StageArea CStage::GetStageArea(void) const
{
	// �X�e�[�W�G���A��Ԃ�
	return m_stageArea;
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

			// �X�e�[�W�G���A�̐ݒ�
			if (strcmp(&aString[0], "AREASET") == 0)
			{ // �ǂݍ��񂾕����� AREASET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_AREASET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "NUM_AREA") == 0)
					{ // �ǂݍ��񂾕����� NUM_AREA �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &stageArea.nNumArea);	// �G���A�̑�����ǂݍ���

						// ��O����
						assert(stageArea.nNumArea > 0);	// �G���A���Œ���ȉ�

						// �G���A�̑������������m��
						stageArea.pInfo = new AreaInfo[stageArea.nNumArea];

						if (USED(stageArea.pInfo))
						{ // �m�ۂɐ��������ꍇ

							// �������N���A
							memset(stageArea.pInfo, 0, sizeof(AreaInfo) * stageArea.nNumArea);
						}
						else { assert(false); }	// �m�ێ��s
					}
					else if (strcmp(&aString[0], "AREA") == 0)
					{ // �ǂݍ��񂾕����� AREA �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_AREA �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "COL") == 0)
							{ // �ǂݍ��񂾕����� COL �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &stageArea.pInfo[nArea].col.r);		// �ԐF��ǂݍ���
								fscanf(pFile, "%f", &stageArea.pInfo[nArea].col.g);		// �ΐF��ǂݍ���
								fscanf(pFile, "%f", &stageArea.pInfo[nArea].col.b);		// �F��ǂݍ���
								fscanf(pFile, "%f", &stageArea.pInfo[nArea].col.a);		// ���l��ǂݍ���
							}
							else if (strcmp(&aString[0], "RADIUS") == 0)
							{ // �ǂݍ��񂾕����� RADIUS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &stageArea.pInfo[nArea].fRadius);	// ���a��ǂݍ���
							}
						} while (strcmp(&aString[0], "END_AREA") != 0);	// �ǂݍ��񂾕����� END_AREA �ł͂Ȃ��ꍇ���[�v

						// ��O����
						assert(nArea < stageArea.nNumArea);	// �G���A���I�[�o�[

						// �G���A�������Z
						nArea++;
					}
				} while (strcmp(&aString[0], "END_AREASET") != 0);	// �ǂݍ��񂾕����� END_AREASET �ł͂Ȃ��ꍇ���[�v

				// �X�e�[�W�G���A�̐ݒ�
				pStage->SetStageArea(stageArea);

				if (USED(stageArea.pInfo))
				{ // �X�e�[�W�G���A���g�p����Ă���ꍇ

					// �X�e�[�W�G���A�̃������J��
					delete[] stageArea.pInfo;
				}
				else { assert(false); }	// ��g�p��
			}

			// �X�e�[�W�͈͂̐ݒ�
			else if (strcmp(&aString[0], "LIMITSET") == 0)
			{ // �ǂݍ��񂾕����� LIMITSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_LIMITSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "NEAR") == 0)
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
