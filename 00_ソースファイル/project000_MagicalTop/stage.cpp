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
#include "block.h"

//************************************************************
//	�}�N����`
//************************************************************
#define STAGE_SETUP_TXT	"data\\TXT\\stage.txt"	// �Z�b�g�A�b�v�e�L�X�g���΃p�X

#define INIT_CENTER_POS	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))				// ������W�̒��S���W�̏����l
#define INIT_REVPOS		(D3DXVECTOR3(0.5f, 0.5f, 0.5f))				// ���W�ϊ��̕␳�l�̏����l
#define INIT_MAXGRID	(POSGRID3(MAX_GRIDX, MAX_GRIDY, MAX_GRIDZ))	// ������W�̐ݒ萔�̏����l

//************************************************************
//	�e�N���X [CStage] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CStage::CStage()
{
	// �����o�ϐ����N���A
	memset(&m_stageLimit, 0, sizeof(m_stageLimit));	// �X�e�[�W�͈�

	// �X�e�[�W������W�̏�����
	m_stageGrid.centerPos	= VEC3_ZERO;	// ������W�̒��S���W
	m_stageGrid.revPos		= VEC3_ZERO;	// ���W�ϊ��̕␳�l
	m_stageGrid.maxGrid		= GRID3_ZERO;	// ������W��

	for (int nCntX = 0; nCntX < MAX_GRIDX; nCntX++)
	{ // ������WX�̍ő吔���J��Ԃ�

		for (int nCntY = 0; nCntY < MAX_GRIDY; nCntY++)
		{ // ������WY�̍ő吔���J��Ԃ�

			for (int nCntZ = 0; nCntZ < MAX_GRIDZ; nCntZ++)
			{ // ������WZ�̍ő吔���J��Ԃ�

				// ������W���̏���������
				m_stageGrid.aX[nCntX].aY[nCntY].aZ[nCntZ].pBlock = NULL;	// �u���b�N�̃|�C���^
			}
		}
	}
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
	// �����o�ϐ���������
	memset(&m_stageLimit, 0, sizeof(m_stageLimit));	// �X�e�[�W�͈�

	// �X�e�[�W������W�̏�����
	m_stageGrid.centerPos	= INIT_CENTER_POS;	// ������W�̒��S���W
	m_stageGrid.revPos		= INIT_REVPOS;		// ���W�ϊ��̕␳�l
	m_stageGrid.maxGrid		= INIT_MAXGRID;		// ������W��

	for (int nCntX = 0; nCntX < MAX_GRIDX; nCntX++)
	{ // ������WX�̍ő吔���J��Ԃ�

		for (int nCntY = 0; nCntY < MAX_GRIDY; nCntY++)
		{ // ������WY�̍ő吔���J��Ԃ�

			for (int nCntZ = 0; nCntZ < MAX_GRIDZ; nCntZ++)
			{ // ������WZ�̍ő吔���J��Ԃ�

				// ������W���̏���������
				m_stageGrid.aX[nCntX].aY[nCntY].aZ[nCntZ].pBlock = NULL;	// �u���b�N�̃|�C���^
			}
		}
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CStage::Uninit(void)
{

}

//============================================================
//	�ʒu�̕␳����
//============================================================
void CStage::LimitPosition(D3DXVECTOR3& rPos, const float fRadius)
{
	// �ʒu��␳
	useful::LimitNum(rPos.x, m_stageLimit.fLeft + fRadius, m_stageLimit.fRight - fRadius);	// ���E�̕␳
	useful::LimitNum(rPos.z, m_stageLimit.fFar + fRadius, m_stageLimit.fNear - fRadius);	// �O��̕␳
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
//	������W�̍��W�ϊ�����
//============================================================
D3DXVECTOR3 CStage::ChangeGridPosition(const POSGRID3& rGrid)
{
	// �ϐ���錾
	D3DXVECTOR3 out;	// �ʒu�v�Z�p
	D3DXVECTOR3 size = CBlock::GetBlockInfo().size;	// �L���[�u�̑傫��

	// ��O����
	assert(size.x > 0 || size.y > 0 || size.z > 0);	// �傫�����ݒ�

	// ������W������W�����߂�
	out.x = m_stageGrid.centerPos.x + (rGrid.x * size.x) - ((m_stageGrid.maxGrid.x - 1) * size.x * m_stageGrid.revPos.x);
	out.y = m_stageGrid.centerPos.y + (rGrid.y * size.y) - ((m_stageGrid.maxGrid.y - 1) * size.y * m_stageGrid.revPos.y);
	out.z = m_stageGrid.centerPos.z + (rGrid.z * size.z) - ((m_stageGrid.maxGrid.z - 1) * size.z * m_stageGrid.revPos.z);

	// �v�Z�������W��Ԃ�
	return out;
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
//	�X�e�[�W������W�̒��S�ʒu�̐ݒ菈��
//============================================================
void CStage::SetStageGridCenter(const D3DXVECTOR3& rPos)
{
	// �����̒��S�ʒu��ݒ�
	m_stageGrid.centerPos = rPos;
}

//============================================================
//	�X�e�[�W������W�̈ʒu�␳�ʂ̐ݒ菈��
//============================================================
void CStage::SetStageGridRevision(const D3DXVECTOR3& rRev)
{
	// �����̕␳�ʂ�ݒ�
	m_stageGrid.revPos = rRev;
}

//============================================================
//	�X�e�[�W������W�̍ő���ᐔ�̐ݒ菈��
//============================================================
void CStage::SetStageGridMax(const POSGRID3& rGrid)
{
	// �����̍ő���ᐔ��ݒ�
	m_stageGrid.maxGrid = rGrid;
}

//============================================================
//	�X�e�[�W������W���̐ݒ菈��
//============================================================
void CStage::SetInGrid(const POSGRID3& rGrid, CBlock *pBlock)
{
	if (rGrid.x < MAX_GRIDX
	&&  rGrid.y < MAX_GRIDY
	&&  rGrid.z < MAX_GRIDZ)
	{ // �O���b�h���͈͓��̏ꍇ

		if (UNUSED(m_stageGrid.Out(rGrid).pBlock))
		{ // �O���b�h���Ƀu���b�N���Ȃ��ꍇ

			// �����̕�����W�ɃI�u�W�F�N�g�̏���ݒ�
			m_stageGrid.In(rGrid, pBlock);
		}
	}
	else { assert(false); }	// �͈͊O
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
//	�X�e�[�W������W�擾����
//============================================================
CStage::StageGrid CStage::GetStageGrid(void) const
{
	// �X�e�[�W������W��Ԃ�
	return m_stageGrid;
}

//============================================================
//	�X�e�[�W������W���擾����
//============================================================
CStage::GridZ CStage::GetInGrid(const POSGRID3& rGrid)
{
	// �����̕�����W���̏���Ԃ�
	return m_stageGrid.Out(rGrid);
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
	StageLimit stageLimit;				// �X�e�[�W�͈͂̑���p
	D3DXVECTOR3 centerPos = VEC3_ZERO;	// ������W�̒��S�ʒu�̑���p
	D3DXVECTOR3 revPos = VEC3_ZERO;		// ���W�ϊ��̕␳�l�̑���p
	POSGRID3 maxGrid = GRID3_ZERO;		// ������W���̑���p
	POSGRID3 grid = GRID3_ZERO;			// �X�e�[�W������W�̑���p
	int nType = 0;						// ��ނ̑���p
	int nEnd = 0;						// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^
	CBlock *pBlock;	// �u���b�N�I�u�W�F�N�g�|�C���^

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

					if (strcmp(&aString[0], "NEAR") == 0)
					{ // �ǂݍ��񂾕����� NEAR �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &stageLimit.fNear);		// �O�ʒu��ǂݍ���
					}
					else if (strcmp(&aString[0], "FAR") == 0)
					{ // �ǂݍ��񂾕����� FAR �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &stageLimit.fFar);		// ��ʒu��ǂݍ���
					}
					else if (strcmp(&aString[0], "RIGHT") == 0)
					{ // �ǂݍ��񂾕����� RIGHT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &stageLimit.fRight);	// �E�ʒu��ǂݍ���
					}
					else if (strcmp(&aString[0], "LEFT") == 0)
					{ // �ǂݍ��񂾕����� LEFT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &stageLimit.fLeft);		// ���ʒu��ǂݍ���
					}
					else if (strcmp(&aString[0], "FIELD") == 0)
					{ // �ǂݍ��񂾕����� FIELD �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &stageLimit.fField);	// �n�ʈʒu��ǂݍ���
					}
				} while (strcmp(&aString[0], "END_LIMITSET") != 0);	// �ǂݍ��񂾕����� END_LIMITSET �ł͂Ȃ��ꍇ���[�v

				// �X�e�[�W�͈͂̐ݒ�
				pStage->SetStageLimit(stageLimit);
			}

			// �u���b�N�̔z�u
			if (strcmp(&aString[0], "STAGE_BLOCKSET") == 0)
			{ // �ǂݍ��񂾕����� STAGE_BLOCKSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_STAGE_BLOCKSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "CENTERPOS") == 0)
					{ // �ǂݍ��񂾕����� CENTERPOS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &centerPos.x);	// ������W�̒��S�ʒu��ǂݍ���
						fscanf(pFile, "%f", &centerPos.y);	// ������W�̒��S�ʒu��ǂݍ���
						fscanf(pFile, "%f", &centerPos.z);	// ������W�̒��S�ʒu��ǂݍ���

						// ������W�̒��S�ʒu�̐ݒ�
						pStage->SetStageGridCenter(centerPos);
					}
					else if (strcmp(&aString[0], "REVPOS") == 0)
					{ // �ǂݍ��񂾕����� REVPOS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &revPos.x);		// ���W�ϊ��̕␳�l��ǂݍ���
						fscanf(pFile, "%f", &revPos.y);		// ���W�ϊ��̕␳�l��ǂݍ���
						fscanf(pFile, "%f", &revPos.z);		// ���W�ϊ��̕␳�l��ǂݍ���

						// ���W�ϊ��̕␳�l�̐ݒ�
						pStage->SetStageGridRevision(revPos);
					}
					else if (strcmp(&aString[0], "MAXGRID") == 0)
					{ // �ǂݍ��񂾕����� MAXGRID �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &maxGrid.x);	// ������W����ǂݍ���
						fscanf(pFile, "%d", &maxGrid.y);	// ������W����ǂݍ���
						fscanf(pFile, "%d", &maxGrid.z);	// ������W����ǂݍ���

						// ������W���̐ݒ�
						pStage->SetStageGridMax(maxGrid);
					}
					else if (strcmp(&aString[0], "BLOCKSET") == 0)
					{ // �ǂݍ��񂾕����� BLOCKSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_BLOCKSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "TYPE") == 0)
							{ // �ǂݍ��񂾕����� TYPE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nType);		// ��ނ�ǂݍ���
							}
							else if (strcmp(&aString[0], "GRID") == 0)
							{ // �ǂݍ��񂾕����� GRID �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &grid.x);		// ������WX��ǂݍ���
								fscanf(pFile, "%d", &grid.y);		// ������WY��ǂݍ���
								fscanf(pFile, "%d", &grid.z);		// ������WZ��ǂݍ���
							}
						} while (strcmp(&aString[0], "END_BLOCKSET") != 0);	// �ǂݍ��񂾕����� END_BLOCKSET �ł͂Ȃ��ꍇ���[�v

						// �u���b�N�I�u�W�F�N�g�̐���
						pBlock = CBlock::Create((CBlock::TYPE)nType, pStage->ChangeGridPosition(grid));

						// �X�e�[�W�̕�����W�Ƀu���b�N��ݒ�
						pStage->SetInGrid(grid, pBlock);
					}
				} while (strcmp(&aString[0], "END_STAGE_BLOCKSET") != 0);	// �ǂݍ��񂾕����� END_STAGE_BLOCKSET �ł͂Ȃ��ꍇ���[�v
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
