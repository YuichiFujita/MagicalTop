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
CObject *CObject::m_apTop[MAX_PRIO] = {};	// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
CObject *CObject::m_apCur[MAX_PRIO] = {};	// �Ō���̃I�u�W�F�N�g�ւ̃|�C���^
int CObject::m_nNumAll = 0;					// �I�u�W�F�N�g�̑���

//************************************************************
//	�e�N���X [CObject] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObject::CObject()
{
#if 0
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
#else

#if 0
	if (USED(m_apCur[DEFAULT_PRIO]))
	{ // �Ō�������݂���ꍇ

		// ���݂̍Ō���̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g��ݒ�
		m_apCur[DEFAULT_PRIO]->m_pNext = this;

		// �O�̃I�u�W�F�N�g��ݒ�
		m_pPrev = m_apCur[DEFAULT_PRIO];

		// ���̃I�u�W�F�N�g���N���A
		m_pNext = NULL;

		// ���g�̏��A�h���X���Ō���ɐݒ�
		m_apCur[DEFAULT_PRIO] = this;

		// ���g�̏����N���A
		m_label		= LABEL_NONE;	// �I�u�W�F�N�g���x��
		m_nPriority	= DEFAULT_PRIO;	// �D�揇��

		// �I�u�W�F�N�g�̑��������Z
		m_nNumAll++;
	}
	else
	{ // �Ō�������݂��Ȃ��ꍇ

		// ���������I�u�W�F�N�g�̃A�h���X��ݒ�
		m_apTop[DEFAULT_PRIO] = this;	// �擪
		m_apCur[DEFAULT_PRIO] = this;	// �Ō��
	}
#else
	if (USED(m_apCur[DEFAULT_PRIO]))
	{ // �Ō�������݂���ꍇ

		// ���݂̍Ō���̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g��ݒ�
		m_apCur[DEFAULT_PRIO]->m_pNext = this;

		// �O�̃I�u�W�F�N�g��ݒ�
		m_pPrev = m_apCur[DEFAULT_PRIO];

		// ���̃I�u�W�F�N�g���N���A
		m_pNext = NULL;

		// ���g�̏��A�h���X���Ō���ɐݒ�
		m_apCur[DEFAULT_PRIO] = this;
	}
	else
	{ // �Ō�������݂��Ȃ��ꍇ

		// ���������I�u�W�F�N�g�̃A�h���X��ݒ�
		m_apTop[DEFAULT_PRIO] = this;	// �擪
		m_apCur[DEFAULT_PRIO] = this;	// �Ō��

		m_apTop[DEFAULT_PRIO]->m_pPrev = NULL;
		m_apTop[DEFAULT_PRIO]->m_pNext = NULL;
		m_apCur[DEFAULT_PRIO]->m_pPrev = NULL;
		m_apCur[DEFAULT_PRIO]->m_pNext = NULL;
	}

	// ���g�̏����N���A
	m_label		= LABEL_NONE;	// �I�u�W�F�N�g���x��
	m_nPriority	= DEFAULT_PRIO;	// �D�揇��

	// �I�u�W�F�N�g�̑��������Z
	m_nNumAll++;
#endif

#endif
}

//============================================================
//	�I�[�o�[���[�h�R���X�g���N�^
//============================================================
CObject::CObject(const LABEL label, const int nPriority)
{
#if 0
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
#else
	if (USED(m_apCur[nPriority]))
	{ // �Ō�������݂���ꍇ

		// ���݂̍Ō���̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g��ݒ�
		m_apCur[nPriority]->m_pNext = this;

		// �O�̃I�u�W�F�N�g��ݒ�
		m_pPrev = m_apCur[nPriority];

		// ���̃I�u�W�F�N�g���N���A
		m_pNext = NULL;

		// ���g�̏��A�h���X���Ō���ɐݒ�
		m_apCur[nPriority] = this;
	}
	else
	{ // �Ō�������݂��Ȃ��ꍇ

		// ���������I�u�W�F�N�g�̃A�h���X��ݒ�
		m_apTop[nPriority] = this;	// �擪
		m_apCur[nPriority] = this;	// �Ō��

		m_apTop[nPriority]->m_pPrev = NULL;
		m_apTop[nPriority]->m_pNext = NULL;
		m_apCur[nPriority]->m_pPrev = NULL;
		m_apCur[nPriority]->m_pNext = NULL;
	}

	// ���g�̏���ݒ�
	m_label		= label;		// �I�u�W�F�N�g���x��
	m_nPriority	= nPriority;	// �D�揇��

	// �I�u�W�F�N�g�̑��������Z
	m_nNumAll++;
#endif
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
//	�}�g���b�N�X�擾����
//============================================================
D3DXMATRIX CObject::GetMtxWorld(void) const
{
	// �ϐ���錾
	D3DXMATRIX mtxNone;	// �Ԃ�l�p

	// �}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxNone);

	// ��O����
	assert(false);

	// �P�ʃ}�g���b�N�X��Ԃ�
	return mtxNone;
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
#if 0
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
#else
	// �|�C���^��錾
	CObject *pObject;	// �I�u�W�F�N�g����p

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		if (USED(m_apTop[nCntPri]))
		{ // �擪�����݂���ꍇ

			// �I�u�W�F�N�g�̐擪����
			pObject = m_apTop[nCntPri];

			while (USED(pObject))
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

				// �|�C���^��錾
				CObject *pObjectNext = pObject->m_pNext;	// ���̃I�u�W�F�N�g�ւ̃|�C���^

				if (pObject->m_label != LABEL_NONE)
				{ // �I�u�W�F�N�g���x�����ݒ肳��Ă���ꍇ

					// �I�u�W�F�N�g�̏I��
					pObject->Uninit();
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObject = pObjectNext;
			}
		}
	}

	// ��O����
	if (m_nNumAll != 0) { assert(false); }	// �j���̎��s
#endif
}

//============================================================
//	�S�X�V����
//============================================================
void CObject::UpdateAll(void)
{
#if 0
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
#else
	// �|�C���^��錾
	CObject *pObject;	// �I�u�W�F�N�g����p

	// �f�o�b�O�\��
	CManager::GetDebugProc()->Print("�I�u�W�F�N�g���F%d\n", m_nNumAll);

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		if (USED(m_apTop[nCntPri]))
		{ // �擪�����݂���ꍇ

			// �I�u�W�F�N�g�̐擪����
			pObject = m_apTop[nCntPri];

			while (USED(pObject))
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

				// �|�C���^��錾
				CObject *pObjectNext = pObject->m_pNext;	// ���̃I�u�W�F�N�g�ւ̃|�C���^

				if (pObject->m_label != LABEL_NONE)
				{ // �I�u�W�F�N�g���x�����ݒ肳��Ă���ꍇ

					// �I�u�W�F�N�g�̍X�V
					pObject->Update();
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObject = pObjectNext;
			}
		}
	}
#endif
}

//============================================================
//	�S�`�揈��
//============================================================
void CObject::DrawAll(void)
{
#if 0
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
#else
	// �|�C���^��錾
	CObject *pObject;	// �I�u�W�F�N�g����p

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		if (USED(m_apTop[nCntPri]))
		{ // �擪�����݂���ꍇ

			// �I�u�W�F�N�g�̐擪����
			pObject = m_apTop[nCntPri];

			while (USED(pObject))
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

				// �|�C���^��錾
				CObject *pObjectNext = pObject->m_pNext;	// ���̃I�u�W�F�N�g�ւ̃|�C���^

				if (pObject->m_label != LABEL_NONE)
				{ // �I�u�W�F�N�g���x�����ݒ肳��Ă���ꍇ

					// �I�u�W�F�N�g�̕`��
					pObject->Draw();
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObject = pObjectNext;
			}
		}
	}
#endif
}

#if 0
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
//	�g�p�m�F����
//============================================================
bool CObject::CheckUse(const CObject *pObject)
{
	if (USED(pObject))
	{ // �|�C���^���g�p���̏ꍇ

		for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
		{ // �D�揇�ʂ̑������J��Ԃ�

			for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
			{ // �I�u�W�F�N�g�̑������J��Ԃ�

				if (CObject::GetObject(nCntPri, nCntObject) == pObject)
				{ // ���A�h���X�̏ꍇ

					// �^��Ԃ�
					return true;	// TODO�F�����A�h���X���m�ۂ���Ă��邾���̉\��������
				}
			}
		}

		// �U��Ԃ�
		return false;
	}
	else
	{ // �|�C���^����g�p���̏ꍇ

		// �U��Ԃ�
		return false;
	}
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
#else
//============================================================
//	�g�p�m�F����
//============================================================
bool CObject::CheckUse(const CObject *pObject)
{
#if 0
	if (USED(pObject))
	{ // �|�C���^���g�p���̏ꍇ

		for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
		{ // �D�揇�ʂ̑������J��Ԃ�

			for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
			{ // �I�u�W�F�N�g�̑������J��Ԃ�

				if (CObject::GetObject(nCntPri, nCntObject) == pObject)
				{ // ���A�h���X�̏ꍇ

					// �^��Ԃ�
					return true;	// TODO�F�����A�h���X���m�ۂ���Ă��邾���̉\��������
				}
			}
		}

		// �U��Ԃ�
		return false;
	}
	else
	{ // �|�C���^����g�p���̏ꍇ

		// �U��Ԃ�
		return false;
	}
#else
	// �|�C���^��錾
	CObject *pObjCheck;	// �I�u�W�F�N�g����p

	if (USED(pObject))
	{ // �|�C���^���g�p���̏ꍇ

		for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
		{ // �D�揇�ʂ̑������J��Ԃ�

			if (USED(m_apTop[nCntPri]))
			{ // �擪�����݂���ꍇ

				// �I�u�W�F�N�g�̐擪����
				pObjCheck = m_apTop[nCntPri];

				while (USED(pObjCheck))
				{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

					// �|�C���^��錾
					CObject *pObjectNext = pObjCheck->m_pNext;	// ���̃I�u�W�F�N�g�ւ̃|�C���^

					if (pObjCheck == pObject)
					{ // ���A�h���X�̏ꍇ

						// �^��Ԃ�
						return true;	// TODO�F�����A�h���X���m�ۂ���Ă��邾���̉\��������
					}

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;
				}
			}
		}

		// �U��Ԃ�
		return false;
	}
	else
	{ // �|�C���^����g�p���̏ꍇ

		// �U��Ԃ�
		return false;
	}
#endif
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
#if 0
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
#else
	// �A�h���X�̓���ւ�
	if (USED(m_pNext))
	{
		m_pNext->m_pPrev = m_pPrev;
	}
	if (USED(m_pPrev))
	{
		m_pPrev->m_pNext = m_pNext;
	}

	// TODO�FTOP�ECUR�̓���ւ�
	if (m_apTop[m_nPriority] == this)
	{
		m_apTop[m_nPriority] = m_pNext;
	}

	if (m_apCur[m_nPriority] == this)
	{
		m_apCur[m_nPriority] = m_pPrev;
	}

	if (USED(this))
	{ // �g�p����Ă���ꍇ

		// �������J��
		delete this;

		// �I�u�W�F�N�g�̑��������Z
		m_nNumAll--;
	}
#endif
}
#endif
