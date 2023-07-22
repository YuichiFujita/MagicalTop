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

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CWaveManager::Season CWaveManager::m_aWaveInfo[CWaveManager::SEASON_MAX] = {};	// �E�F�[�u���

//************************************************************
//	�}�N����`
//************************************************************
#define WAVE_SETUP_TXT	"data\\TXT\\wave.txt"	// �Z�b�g�A�b�v�e�L�X�g���΃p�X

//************************************************************
//	�e�N���X [CWaveManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CWaveManager::CWaveManager()
{
	// �����o�ϐ����N���A
	m_state = STATE_NONE;				// ���
	m_nSeasonCounter = SEASON_SPRING;	// �G�߃J�E���^�[
	m_nWaveCounter = 0;					// �E�F�[�u�J�E���^�[
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
	// �����o�ϐ���������
	m_state = STATE_NONE;				// ���
	m_nSeasonCounter = SEASON_SPRING;	// �G�߃J�E���^�[
	m_nWaveCounter = 0;					// �E�F�[�u�J�E���^�[

	// �Z�b�g�A�b�v�̓ǂݍ���
	LoadSetup();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CWaveManager::Uninit(void)
{
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
	case STATE_NONE:			// �������Ȃ����

		// ����

		break;

	case STATE_SEASON_START:	// �G�߂̊J�n���



		break;

	case STATE_WAVE_START:		// �E�F�[�u�J�n���



		break;

	case STATE_PROGRESSION:		// �E�F�[�u�i�s���



#if 0
		//if ()
		{ // �E�F�[�u���N���A�����ꍇ

			// �E�F�[�u�J�E���^�[�����Z
			m_nWaveCounter++;

			if (m_nWaveCounter < m_aWaveInfo[m_nSeasonCounter].nNumWave)
			{ // �E�F�[�u���܂�����ꍇ

				// ��Ԃ�ύX
				m_state = STATE_WAVE_START;	// �E�F�[�u�J�n���
			}
			else
			{ // �S�E�F�[�u���I�������ꍇ

				// �G�߃J�E���^�[�����Z
				m_nSeasonCounter++;

				// ��Ԃ�ύX
				m_state = STATE_SEASON_END;	// �G�߂̏I�����
			}
		}
#endif

		break;

	case STATE_SEASON_END:		// �G�߂̏I�����

		// �E�F�[�u�J�E���^�[��������
		m_nWaveCounter = 0;

		break;

	case STATE_WAIT:			// ���G�߂̊J�n�ҋ@���



		break;

	default:	// ��O����
		assert(false);
		break;
	}
}

//============================================================
//	�`�揈��
//============================================================
void CWaveManager::Draw(void)
{

}

//============================================================
//	��������
//============================================================
CWaveManager *CWaveManager::Create()
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

								// �o���ԍ������Z
								nPoint++;
							}
						} while (strcmp(&aString[0], "END_WAVE") != 0);	// �ǂݍ��񂾕����� END_WAVE �ł͂Ȃ��ꍇ���[�v

						// �E�F�[�u�ԍ������Z
						nWave++;
					}
				} while (strcmp(&aString[0], "END_SEASONSET") != 0);	// �ǂݍ��񂾕����� END_SEASONSET �ł͂Ȃ��ꍇ���[�v

				// ��O����
				assert(nSeason < SEASON_MAX);	// �G�߃I�[�o�[

				// �G�ߔԍ������Z
				nSeason++;
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�E�F�[�u�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}
