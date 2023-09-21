//============================================================
//
//	�V���b�v�Ȃ���񏈗� [omitShop.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "omitShop.h"
#include "manager.h"

//************************************************************
//	�e�N���X [COmitShop] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
COmitShop::COmitShop()
{
	// �����o�ϐ����N���A
	memset(&m_aOmitShop[0], 0, sizeof(m_aOmitShop));	// �w���i�̏Ȃ���
	m_nNumOmit = 0;	// �Ȃ��w���i��
	m_pOmit = NULL;	// �Ȃ��w���i���
}

//============================================================
//	�f�X�g���N�^
//============================================================
COmitShop::~COmitShop()
{

}

//============================================================
//	����������
//============================================================
void COmitShop::Init(void)
{
	// �����o�ϐ���������
	m_nNumOmit = 0;	// �Ȃ��w���i��
	m_pOmit = NULL;	// �Ȃ��w���i���

	for (int nCntOmit = 0; nCntOmit < CShop::BUY_MAX; nCntOmit++)
	{ // �w���i�̍ő吔���J��Ԃ�

		// �Ȃ��Ȃ���Ԃɐݒ�
		m_aOmitShop[nCntOmit] = false;
	}
}

//============================================================
//	�I������
//============================================================
void COmitShop::Uninit(void)
{
	if (USED(m_pOmit))
	{ // �Ȃ��w���i��񂪎g���Ă���ꍇ

		// �V���b�t�����Ȃ������̃������J��
		delete[] m_pOmit;
		m_pOmit = NULL;
	}
}

//============================================================
//	�Ȃ��w���i��������
//============================================================
int *COmitShop::CreateOmit(void)
{
	// �ϐ���錾
	int nOmitID = 0;	// ���݂̃V���b�t���Ȃ��z��̗v�f�ԍ�

	if (UNUSED(m_pOmit))
	{ // �Ȃ��w���i��񂪎g���Ă��Ȃ��ꍇ

		// �V���b�t�����Ȃ������̃���������
		m_pOmit = new int[m_nNumOmit];

		if (UNUSED(m_pOmit))
		{ // �m�ۂɎ��s�����ꍇ

			// ���s��Ԃ�
			return NULL;
		}
	}
	else { assert(false); return NULL; }	// �g�p��

	for (int nCntOmit = 0; nCntOmit < CShop::BUY_MAX; nCntOmit++)
	{ // �w���i�̍ő吔���J��Ԃ�

		if (m_aOmitShop[nCntOmit])
		{ // �Ȃ��ꍇ

			// ���݂̃C���f�b�N�X���Ȃ��f�[�^�ɑ��
			m_pOmit[nOmitID] = nCntOmit;

			// �Ȃ��f�[�^������C���f�b�N�X�����Z
			nOmitID++;
		}
	}

	// �Ȃ��w���i����Ԃ�
	return m_pOmit;
}

//============================================================
//	�Ȃ��w���i�j������
//============================================================
HRESULT COmitShop::ReleaseOmit(void)
{
	if (USED(m_pOmit))
	{ // �Ȃ��w���i��񂪎g���Ă���ꍇ

		// �V���b�t�����Ȃ������̃������J��
		delete[] m_pOmit;
		m_pOmit = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�Ȃ��w���i���擾����
//============================================================
int COmitShop::GetNumOmit(void) const
{
	// �Ȃ��w���i����Ԃ�
	return m_nNumOmit;
}

//============================================================
//	�Ȃ��󋵎擾����
//============================================================
bool COmitShop::IsOmit(const CShop::BUY buy) const
{
	// �����̍w���i�̏Ȃ��󋵂�Ԃ�
	return m_aOmitShop[buy];
}

//============================================================
//	�Ȃ��󋵂̐ݒ菈��
//============================================================
void COmitShop::SetEnableOmit(const CShop::BUY buy, const bool bOmit)
{
	// �����̍w���i�̏Ȃ��󋵂�ݒ�
	m_aOmitShop[buy] = bOmit;

	// �Ȃ��w���i����������
	m_nNumOmit = 0;

	// �Ȃ��w���i����ݒ�
	for (int nCntOmit = 0; nCntOmit < CShop::BUY_MAX; nCntOmit++)
	{ // �w���i�̍ő吔���J��Ԃ�

		if (m_aOmitShop[nCntOmit])
		{ // �Ȃ���Ԃ������ꍇ

			// �Ȃ��w���i�������Z
			m_nNumOmit++;
		}
	}
}

//============================================================
//	��������
//============================================================
COmitShop *COmitShop::Create(void)
{
	// �|�C���^��錾
	COmitShop *pOmitShop = NULL;	// �V���b�v�Ȃ���񐶐��p

	if (UNUSED(pOmitShop))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pOmitShop = new COmitShop;	// �V���b�v�Ȃ����
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pOmitShop))
	{ // �g�p����Ă���ꍇ
		
		// �V���b�v�Ȃ����̏�����
		pOmitShop->Init();

		// �m�ۂ����A�h���X��Ԃ�
		return pOmitShop;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT COmitShop::Release(COmitShop *&prOmitShop)
{
	if (USED(prOmitShop))
	{ // �g�p���̏ꍇ

		// �V���b�v�Ȃ����̏I��
		prOmitShop->Uninit();

		// �������J��
		delete prOmitShop;
		prOmitShop = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}
