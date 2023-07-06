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
//	�e�N���X [CEdit] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEdit::CEdit()
{
	// �����o�ϐ����N���A

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
#define PLAY_RADIUS	(100.0f)	// 
#define VTX_RADIUS	(10.0f)		// 
#define VTX_UP		(5.0f)		// 

	// �|�C���^��錾
	CInputKeyboard *pKey = CManager::GetKeyboard();	// �L�[�{�[�h���
	CPlayer *pPlayer = CManager::GetPlayer();		// �v���C���[���
	CField *pField = CManager::GetField();			// �t�B�[���h���

	if (pKey->GetPress(DIK_RETURN))
	{ // [Enter] �������ꂽ�ꍇ

		// �ϐ���錾
		D3DXVECTOR3 posVtx;	// ���_�ʒu
		D3DXVECTOR3 posPlayer = pPlayer->GetPosition();	// �v���C���[�ʒu
		int nNumVtx = pField->GetNumVertex();			// ���_��

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{ // ���_�����J��Ԃ�

			// ���_�ʒu���擾
			posVtx = pField->GetMeshVertexPosition(nCntVtx);

			if (collision::Circle2D(posPlayer, posVtx, PLAY_RADIUS, VTX_RADIUS))
			{ // �͈͓��̏ꍇ

				// TODO�F���������ɂ���

				// ���_�Ƃ̋��������߂�
				float f = sqrtf(((posPlayer.x - posVtx.x) * (posPlayer.x - posVtx.x)) + ((posPlayer.z - posVtx.z) * (posPlayer.z - posVtx.z)) * 0.5f);
#if 1
				float ff = VTX_UP * -((f / (PLAY_RADIUS + VTX_RADIUS)) - 1.0f);

				// ���_�̍�����ύX
				pField->SetMeshVertexPosition(nCntVtx, D3DXVECTOR3(posVtx.x, posVtx.y + ff, posVtx.z));
#else
				float fff = f * -(((D3DX_PI * 0.5f) / (PLAY_RADIUS + VTX_RADIUS)) - 1.0f);
				useful::NormalizeRot(fff);

				// ���_�̍�����ύX
				pField->SetMeshVertexPosition(nCntVtx, D3DXVECTOR3(posVtx.x, posVtx.y + (-VTX_UP * sinf(D3DXToRadian(fff))), posVtx.z));
#endif
			}
		}

		pField->NormalizeNormal();
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
