//============================================================
//
//	�I�u�W�F�N�g���� [object.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"
#include "manager.h"
#include "debugproc.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CObject *CObject::m_apObject[MAX_PRIO][MAX_OBJECT] = {};	// �I�u�W�F�N�g�̏��
int CObject::m_nNumAll = 0;	// �I�u�W�F�N�g�̑���

//************************************************************
//	�e�N���X [CObject] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObject::CObject()
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // �I�u�W�F�N�g�̍ő吔���J��Ԃ�

		if (UNUSED(m_apObject[DEFAULT_PRIO][nCntObject]))
		{ // �g�p����Ă��Ȃ��ꍇ

			// ���g�̏��A�h���X��ݒ�
			m_apObject[DEFAULT_PRIO][nCntObject] = this;

			// ���g�̃C���f�b�N�X��ݒ�
			m_nID = nCntObject;

			// ���g�̏����N���A
			m_label		= LABEL_NONE;	// �I�u�W�F�N�g���x��
			m_nPriority	= DEFAULT_PRIO;	// �D�揇��

			// ���g�̔����ݒ�
			m_hrResult = S_OK;	// �����ɂ���

			// �I�u�W�F�N�g�̑��������Z
			m_nNumAll++;

			// �֐��𔲂���
			return;
		}
	}

	// ���g�̔����ݒ�
	m_hrResult = E_FAIL;	// ���s�ɂ���
}

//============================================================
//	�I�[�o�[���[�h�R���X�g���N�^
//============================================================
CObject::CObject(const LABEL label, const int nPriority)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // �I�u�W�F�N�g�̍ő吔���J��Ԃ�

		if (UNUSED(m_apObject[nPriority][nCntObject]))
		{ // �g�p����Ă��Ȃ��ꍇ

			// ���g�̏��A�h���X��ݒ�
			m_apObject[nPriority][nCntObject] = this;

			// ���g�̏���ݒ�
			m_label		= label;		// �I�u�W�F�N�g���x��
			m_nID		= nCntObject;	// �C���f�b�N�X
			m_nPriority	= nPriority;	// �D�揇��

			// ���g�̔����ݒ�
			m_hrResult = S_OK;	// �����ɂ���

			// �I�u�W�F�N�g�̑��������Z
			m_nNumAll++;

			// �֐��𔲂���
			return;
		}
	}

	// ���g�̔����ݒ�
	m_hrResult = E_FAIL;	// ���s�ɂ���
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObject::~CObject()
{

}

//============================================================
//	�q�b�g����
//============================================================
void CObject::Hit(const int nDmg)
{
	// ��O����
	assert(false);
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CObject::SetPosition(const D3DXVECTOR3& rPos)
{
	// ��O����
	assert(false);
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObject::SetRotation(const D3DXVECTOR3& rRot)
{
	// ��O����
	assert(false);
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObject::GetPosition(void) const
{
	// ��O����
	assert(false);

	// �����l��Ԃ�
	return VEC3_ZERO;
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CObject::GetRotation(void) const
{
	// ��O����
	assert(false);

	// �����l��Ԃ�
	return VEC3_ZERO;
}

//============================================================
//	�傫���擾����
//============================================================
D3DXVECTOR3 CObject::GetScaling(void) const
{
	// ��O����
	assert(false);

	// �����l��Ԃ�
	return VEC3_ZERO;
}

//============================================================
//	���a�擾����
//============================================================
float CObject::GetRadius(void) const
{
	// ��O����
	assert(false);

	// �����l��Ԃ�
	return 0.0f;
}

//============================================================
//	�S�j������
//============================================================
void CObject::ReleaseAll(void)
{
	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
		{ // �I�u�W�F�N�g�̍ő吔���J��Ԃ�

			if (USED(m_apObject[nCntPri][nCntObject]))
			{ // �g�p����Ă���ꍇ

				if (m_apObject[nCntPri][nCntObject]->GetLabel() != LABEL_NONE)
				{ // �I�u�W�F�N�g���x�����ݒ肳��Ă���ꍇ

					// �I�u�W�F�N�g�̏I��
					m_apObject[nCntPri][nCntObject]->Uninit();
				}
			}
		}
	}

	// ��O����
	if (m_nNumAll != 0) { assert(false); }	// �j���̎��s
}

//============================================================
//	�S�X�V����
//============================================================
void CObject::UpdateAll(void)
{
	// �f�o�b�O�\��
	CManager::GetDebugProc()->Print("�I�u�W�F�N�g���F%d\n", m_nNumAll);

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
		{ // �I�u�W�F�N�g�̍ő吔���J��Ԃ�

			if (USED(m_apObject[nCntPri][nCntObject]))
			{ // �g�p����Ă���ꍇ

				if (m_apObject[nCntPri][nCntObject]->GetLabel() != LABEL_NONE)
				{ // �I�u�W�F�N�g���x�����ݒ肳��Ă���ꍇ

					// �I�u�W�F�N�g�̍X�V
					m_apObject[nCntPri][nCntObject]->Update();
				}
			}
		}
	}
}

//============================================================
//	�S�`�揈��
//============================================================
void CObject::DrawAll(void)
{
	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
		{ // �I�u�W�F�N�g�̍ő吔���J��Ԃ�

			if (USED(m_apObject[nCntPri][nCntObject]))
			{ // �g�p����Ă���ꍇ

				if (m_apObject[nCntPri][nCntObject]->GetLabel() != LABEL_NONE)
				{ // �I�u�W�F�N�g���x�����ݒ肳��Ă���ꍇ

					// �I�u�W�F�N�g�̕`��
					m_apObject[nCntPri][nCntObject]->Draw();
				}
			}
		}
	}
}

//============================================================
//	�I�u�W�F�N�g�擾����
//============================================================
CObject *CObject::GetObject(const int nPriority, const int nID)
{
	if (nPriority < MAX_PRIO && nID < MAX_OBJECT)
	{ // �����̗D�揇�ʁE�C���f�b�N�X���g�p�\�ȏꍇ

		// �����̗D�揇�ʁE�C���f�b�N�X�̃I�u�W�F�N�g��Ԃ�
		return m_apObject[nPriority][nID];
	}
	else { assert(false); return NULL; }	// �͈͊O
}

//============================================================
//	���s����̎擾����
//============================================================
HRESULT CObject::GetResult(void) const
{
	// ���s�����Ԃ�
	return m_hrResult;
}

//============================================================
//	�C���f�b�N�X�擾����
//============================================================
int CObject::GetID(void)
{
	// �C���f�b�N�X��Ԃ�
	return m_nID;
}

//============================================================
//	�D�揇�ʎ擾����
//============================================================
int CObject::GetPriority(void)
{
	// �D�揇�ʂ�Ԃ�
	return m_nPriority;
}

//============================================================
//	���x���̐ݒ菈��
//============================================================
void CObject::SetLabel(const LABEL label)
{
	if (label < LABEL_MAX)
	{ // �����̃��x�����g�p�\�ȏꍇ

		// �����̃��x����ݒ�
		m_label = label;
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	���x���̎擾����
//============================================================
CObject::LABEL CObject::GetLabel(void) const
{
	// ���x����Ԃ�
	return m_label;
}

//============================================================
//	�D�揇�ʂ̐ݒ菈��
//============================================================
void CObject::SetPriority(const int nPriority)
{
	if (nPriority < MAX_PRIO)
	{ // �����̗D�揇�ʂ��g�p�\�ȏꍇ

		// �����̗D�揇�ʂ�ݒ�
		m_nPriority = nPriority;
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	�D�揇�ʂ̎擾����
//============================================================
int CObject::GetPriority(void) const
{
	// �D�揇�ʂ�Ԃ�
	return m_nPriority;
}

//============================================================
//	�j������
//============================================================
void CObject::Release(void)
{
	// �ϐ���錾
	int nPriority = m_nPriority;	// ���g�̗D�揇��
	int nID = m_nID;				// ���g�̃C���f�b�N�X

	if (USED(m_apObject[nPriority][nID]))
	{ // �g�p����Ă���ꍇ

		// �������J��
		delete m_apObject[nPriority][nID];
		m_apObject[nPriority][nID] = NULL;

		// �I�u�W�F�N�g�̑��������Z
		m_nNumAll--;
	}
}
