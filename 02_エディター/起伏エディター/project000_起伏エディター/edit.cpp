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
	pDebug->Print("�y�ۑ��z[F1]�@���Ԃɍ����܂���ł������c\n");
	pDebug->Print("�y�Ǎ��z[F2]�@���Ԃɍ����܂���ł������c\n");
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

}

//============================================================
//	�Ǎ�����
//============================================================
void CEdit::Load(void)
{

}
