//============================================================
//
//	�G�f�B�b�g���� [edit.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "edit.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "collision.h"
#include "debugproc.h"
#include "player.h"
#include "field.h"

//************************************************************
//	�}�N����`
//************************************************************
#define SAVE_TXT	"data\\TXT\\save.txt"	// �Z�[�u�e�L�X�g���΃p�X

#define INIT_BRUSH_RADIUS	(100.0f)	// �M�̑����̏����l
#define INIT_VTX_UP			(2.0f)		// ���_�㏸�ʂ̏����l

#define CHANGE_RAD	(1.0f)		// �M�̑����ύX��
#define MIN_RAD		(10.0f)		// �M�̑����̍ŏ��l
#define MAX_RAD		(1000.0f)	// �M�̑����̍ő�l
#define CHANGE_UP	(0.05f)		// �㏸�ʕύX��
#define MIN_UP		(0.5f)		// �㏸�ʂ̍ŏ��l
#define MAX_UP		(50.0f)		// �㏸�ʂ̍ő�l

//************************************************************
//	�e�N���X [CEdit] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEdit::CEdit()
{
	// �����o�ϐ����N���A
	m_fBrushRadius = 0.0f;	// �M�̑���
	m_fVtxUp = 0.0f;		// ���_�̏㏸��
	m_bAutoNor = false;		// �@���̎����ݒ��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEdit::~CEdit()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEdit::Init(void)
{
	// �����o�ϐ���������
	m_fBrushRadius = INIT_BRUSH_RADIUS;	// �M�̑���
	m_fVtxUp = INIT_VTX_UP;				// ���_�̏㏸��
	m_bAutoNor = false;					// �@���̎����ݒ��

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEdit::Uninit(void)
{

}

//============================================================
//	�X�V����
//============================================================
void CEdit::Update(void)
{
	// �|�C���^��錾
	CInputKeyboard *pKey = CManager::GetKeyboard();	// �L�[�{�[�h���

	// ��������\��
	DrawManual();

	// �u���V�ݒ�
	SetBursh();

	// �㏸�ʐݒ�
	SetVtxUp();

	// ���_�ݒ�
	SetVtx();

	// �@���ݒ�
	SetNor();

	if (pKey->GetPress(DIK_F2))
	{ // [F2] �������ꂽ�ꍇ

		// �N���̕ۑ�
		Save();
	}
	if (pKey->GetPress(DIK_F3))
	{ // [F3] �������ꂽ�ꍇ

		// �N���̓Ǎ�
		Load();
	}
}

//============================================================
//	��������
//============================================================
CEdit *CEdit::Create(void)
{
	// �|�C���^��錾
	CEdit *pEdit = NULL;	// �G�f�B�b�g�����p

	if (UNUSED(pEdit))
	{ // �g�p����Ă��Ȃ��ꍇ

		// ���������m��
		pEdit = new CEdit;	// �G�f�B�b�g
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pEdit))
	{ // �m�ۂɐ������Ă���ꍇ

		// �G�f�B�b�g�̏�����
		if (FAILED(pEdit->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pEdit;
			pEdit = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pEdit;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CEdit::Release(CEdit *&prEdit)
{
	if (USED(prEdit))
	{ // �g�p���̏ꍇ

		// �G�f�B�b�g�̏I��
		prEdit->Uninit();

		// �������J��
		delete prEdit;
		prEdit = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�u���V�ݒ菈��
//============================================================
void CEdit::SetBursh(void)
{
	// �|�C���^��錾
	CInputKeyboard *pKey = CManager::GetKeyboard();	// �L�[�{�[�h���

	if (pKey->GetPress(DIK_3))
	{ // [3] �������ꂽ�ꍇ

		// �M�̑��������Z
		m_fBrushRadius += CHANGE_RAD;
	}
	if (pKey->GetPress(DIK_4))
	{ // [4] �������ꂽ�ꍇ

		// �M�̑��������Z
		m_fBrushRadius -= CHANGE_RAD;
	}

	// �M�̑����̕␳
	useful::LimitNum(m_fBrushRadius, MIN_RAD, MAX_RAD);
}

//============================================================
//	�㏸�ʐݒ菈��
//============================================================
void CEdit::SetVtxUp(void)
{
	// �|�C���^��錾
	CInputKeyboard *pKey = CManager::GetKeyboard();	// �L�[�{�[�h���

	if (pKey->GetPress(DIK_5))
	{ // [5] �������ꂽ�ꍇ

		// �㏸�ʂ����Z
		m_fVtxUp += CHANGE_UP;
	}
	if (pKey->GetPress(DIK_6))
	{ // [6] �������ꂽ�ꍇ

		// �㏸�ʂ����Z
		m_fVtxUp -= CHANGE_UP;
	}

	// �㏸�ʂ̕␳
	useful::LimitNum(m_fVtxUp, MIN_UP, MAX_UP);
}

//============================================================
//	���_�ݒ菈��
//============================================================
void CEdit::SetVtx(void)
{
	// �|�C���^��錾
	CInputKeyboard *pKey = CManager::GetKeyboard();	// �L�[�{�[�h���
	CPlayer *pPlayer = CManager::GetPlayer();		// �v���C���[���
	CField *pField = CManager::GetField();			// �t�B�[���h���

	if (pKey->GetPress(DIK_0) || pKey->GetPress(DIK_9))
	{ // [0 or 9] �������ꂽ�ꍇ

		// �ϐ���錾
		D3DXVECTOR3 posVtx;	// ���_�ʒu
		D3DXVECTOR3 posPlayer = pPlayer->GetPosition();	// �v���C���[�ʒu
		int nNumVtx = pField->GetNumVertex();			// ���_��
		int nUp = (pKey->GetPress(DIK_0)) ? 1 : -1;		// �㏸�ʂ̌v�Z�p

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{ // ���_�����J��Ԃ�

			// ���_�ʒu���擾
			posVtx = pField->GetMeshVertexPosition(nCntVtx);

			if (collision::Circle2D(posPlayer, posVtx, m_fBrushRadius, 0.0f))
			{ // �͈͓��̏ꍇ

				// �ϐ���錾
				float fLength = sqrtf(((posPlayer.x - posVtx.x) * (posPlayer.x - posVtx.x)) + ((posPlayer.z - posVtx.z) * (posPlayer.z - posVtx.z)) * 0.5f);	// ���_�Ƃ̋���
				float fVtxUp = (m_fVtxUp * -((fLength / m_fBrushRadius) - 1.0f)) * nUp;	// ���_�㏸��

				// ���_�̍�����ύX
				pField->SetMeshVertexPosition(nCntVtx, D3DXVECTOR3(posVtx.x, posVtx.y + fVtxUp, posVtx.z));
			}
		}
	}
}

//============================================================
//	�@���ݒ菈��
//============================================================
void CEdit::SetNor(void)
{
	// �|�C���^��錾
	CInputKeyboard *pKey = CManager::GetKeyboard();	// �L�[�{�[�h���
	CField *pField = CManager::GetField();			// �t�B�[���h���

	if (pKey->GetTrigger(DIK_1))
	{ // [1] �������ꂽ�ꍇ

		// �������K���󋵂�؂�ւ���
		m_bAutoNor = (m_bAutoNor) ? false : true;
	}

	if (m_bAutoNor)
	{ // �������K����ON�̏ꍇ

		// �@���̐ݒ�E���K��
		pField->NormalizeNormal();
	}
	else
	{ // �������K����OFF�̏ꍇ

		if (pKey->GetTrigger(DIK_2))
		{ // [2] �������ꂽ�ꍇ

			// �@���̐ݒ�E���K��
			pField->NormalizeNormal();	
		}
	}
}

//============================================================
//	��������\������
//============================================================
void CEdit::DrawManual(void)
{
	// �|�C���^��錾
	CDebugProc *pDebug = CManager::GetDebugProc();	// �f�o�b�O�\�����

	// ���������\��
	pDebug->Print("========================================\n");
	pDebug->Print("�y�ۑ��z[F2]\n");
	pDebug->Print("�y�Ǎ��z[F3]\n");
	pDebug->Print("========================================\n");
	pDebug->Print("�y���_�㏸�z[0]\n");
	pDebug->Print("�y���_���~�z[9]\n");
	pDebug->Print("�y�����@�� ON/OFF�z[1]\n");
	pDebug->Print("�y�@���̐��K���z   [2]\n");
	pDebug->Print("�y�M���������z[3]\n");
	pDebug->Print("�y�M���������z[4]\n");
	pDebug->Print("�y�㏸�ʑ����z[5]\n");
	pDebug->Print("�y�㏸�ʌ����z[6]\n");
	pDebug->Print("========================================\n");

	// �@���̎������K���󋵂�\��
	if (m_bAutoNor)
	{ // �������K����ON�̏ꍇ
		pDebug->Print("�y�����@���zON\n");
	}
	else
	{ // �������K����OFF�̏ꍇ
		pDebug->Print("�y�����@���zOFF\n");
	}

	// �M�̑����\��
	pDebug->Print("�y�M�����z%f\n", m_fBrushRadius);

	// �㏸�ʕ\��
	pDebug->Print("�y�㏸�ʁz%f\n", m_fVtxUp);
}

//============================================================
//	�ۑ�����
//============================================================
void CEdit::Save(void)
{
	// �ϐ���錾
	POSGRID2 part = GRID2_ONE;	// ������

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^
	CField *pField = CManager::GetField();	// �t�B�[���h���

	// �t�@�C�����������݌`���ŊJ��
	pFile = fopen(SAVE_TXT, "w");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		// ���o��
		fprintf(pFile, "#==============================================================================\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#	�n�`�ۑ��e�L�X�g [save.txt]\n");
		fprintf(pFile, "#	Author�Fyou\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#==============================================================================\n");
		fprintf(pFile, "<>*************<> �������牺���R�s�[�� [field.txt] �ɒ���t�� <>*************<>\n");
		fprintf(pFile, "\n");

		// �N�����̊J�n�n�_���e�L�X�g�ɏ����o��
		fprintf(pFile, "TERRAINSET\n\n");
		fprintf(pFile, "	FIELDSET\n");

		// �������̏����o��
		part = pField->GetPattern();
		fprintf(pFile, "		# ������\n");
		fprintf(pFile, "		PART = %d %d\n\n", part.x, part.y);

		// ���_���W�̂���ʂ̏����o��
		fprintf(pFile, "		# ���_���W�̂����\n");
		fprintf(pFile, "		GAPSET\n");

		for (int nCntHeight = 0; nCntHeight < part.y + 1; nCntHeight++)
		{ // �c�̕����� +1��J��Ԃ�

			// ��
			fprintf(pFile, "		");

			for (int nCntWidth = 0; nCntWidth < part.x + 1; nCntWidth++)
			{ // ���̕����� +1��J��Ԃ�

				// �ϐ���錾
				int nNumVtx = nCntWidth + (nCntHeight * (part.x + 1));	// ���݂̒��_�ԍ�
				D3DXVECTOR3 posGap = D3DXVECTOR3						// ���_���W�̂����
				( // ����
					0.0f,
					-(pField->GetGapPosition(nNumVtx).y - pField->GetMeshVertexPosition(nNumVtx).y),
					0.0f
				);

				// ���_���W�̂���ʂ������o��
				fprintf(pFile, "%.3f %.3f %.3f , ", posGap.x, posGap.y, posGap.z);
			}

			// ���s
			fprintf(pFile, "\n");
		}

		// �N�����̏I���n�_���e�L�X�g�ɏ����o��
		fprintf(pFile, "	END_FIELDSET\n\n");
		fprintf(pFile, "END_TERRAINSET\n");

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�N���̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}

//============================================================
//	�Ǎ�����
//============================================================
void CEdit::Load(void)
{
	// �ϐ���錾
	POSGRID2 part = GRID2_ZERO;		// �������̑���p
	D3DXVECTOR3 pos = VEC3_ZERO;	// ���_���W�̂���ʂ̑���p
	int nEnd = 0;					// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^
	CField *pField = CManager::GetField();	// �t�B�[���h���

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(SAVE_TXT, "r");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			// �n�`�̐ݒ�
			if (strcmp(&aString[0], "TERRAINSET") == 0)
			{ // �ǂݍ��񂾕����� TERRAINSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_TERRAINSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "FIELDSET") == 0)
					{ // �ǂݍ��񂾕����� FIELDSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_FIELDSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "PART") == 0)
							{ // �ǂݍ��񂾕����� PART �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &part.x);		// ������X��ǂݍ���
								fscanf(pFile, "%d", &part.y);		// ������Y��ǂݍ���

								// �������̐ݒ�
								pField->SetPattern(part);
							}
							else if (strcmp(&aString[0], "GAPSET") == 0)
							{ // �ǂݍ��񂾕����� GAPSET �̏ꍇ

								for (int nCntVtx = 0; nCntVtx < pField->GetNumVertex(); nCntVtx++)
								{ // ���_�����J��Ԃ�

									fscanf(pFile, "%f", &pos.x);		// ���_���W�̂����X��ǂݍ���
									fscanf(pFile, "%f", &pos.y);		// ���_���W�̂����Y��ǂݍ���
									fscanf(pFile, "%f", &pos.z);		// ���_���W�̂����Z��ǂݍ���
									fscanf(pFile, "%s", &aString[0]);	// , ��ǂݍ��� (�s�v)

									// ���_���W�̂���ʂ̐ݒ�
									pField->SetMeshVertexPosition(nCntVtx, pField->GetMeshVertexPosition(nCntVtx) + pos);
								}
							}
						} while (strcmp(&aString[0], "END_FIELDSET") != 0);	// �ǂݍ��񂾕����� END_FIELDSET �ł͂Ȃ��ꍇ���[�v

						// �@���̐ݒ�E���K��
						pField->NormalizeNormal();
					}
				} while (strcmp(&aString[0], "END_TERRAINSET") != 0);	// �ǂݍ��񂾕����� END_TERRAINSET �ł͂Ȃ��ꍇ���[�v
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�N���̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}
