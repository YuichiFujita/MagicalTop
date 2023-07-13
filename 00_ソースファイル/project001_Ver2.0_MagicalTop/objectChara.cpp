//============================================================
//
//	�I�u�W�F�N�g�L�����N�^�[���� [objectChara.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectChara.h"
#include "manager.h"
#include "renderer.h"
#include "multiModel.h"
#include "motion.h"

//************************************************************
//	�q�N���X [CObjectChara] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectChara::CObjectChara()
{
	// �����o�ϐ����N���A
	memset(&m_apMultiModel[0], 0, sizeof(m_apMultiModel));	// ���f���̏��
	m_pMotion = NULL;	// ���[�V�����̏��
	m_pos = VEC3_ZERO;	// �ʒu
	m_rot = VEC3_ZERO;	// ����
	m_nNumModel = 0;	// �p�[�c�̑���
}

//============================================================
//	�I�[�o�[���[�h�R���X�g���N�^
//============================================================
CObjectChara::CObjectChara(const CObject::LABEL label, const int nPriority) : CObject(label, nPriority)
{
	// �����o�ϐ����N���A
	memset(&m_apMultiModel[0], 0, sizeof(m_apMultiModel));	// ���f���̏��
	m_pMotion = NULL;	// ���[�V�����̏��
	m_pos = VEC3_ZERO;	// �ʒu
	m_rot = VEC3_ZERO;	// ����
	m_nNumModel = 0;	// �p�[�c�̑���
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObjectChara::~CObjectChara()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectChara::Init(void)
{
	// �����o�ϐ���������
	m_pos = VEC3_ZERO;	// �ʒu
	m_rot = VEC3_ZERO;	// ����
	m_nNumModel = 0;	// �p�[�c�̑���

	// ���[�V�����I�u�W�F�N�g�̐���
	m_pMotion = CMotion::Create();
	if (UNUSED(m_pMotion))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CObjectChara::Uninit(void)
{
	// �}���`���f����j��
	for (int nCntObjectChara = 0; nCntObjectChara < MAX_PARTS; nCntObjectChara++)
	{ // �p�[�c�̍ő吔���J��Ԃ�

		if (USED(m_apMultiModel[nCntObjectChara]))
		{ // �p�[�c���g�p���̏ꍇ

			// �j������
			if (FAILED(m_apMultiModel[nCntObjectChara]->Release(m_apMultiModel[nCntObjectChara])))
			{ // �j���Ɏ��s�����ꍇ

				// ��O����
				assert(false);
			}
		}
	}

	// ���[�V������j��
	if (FAILED(m_pMotion->Release(m_pMotion)))
	{ // �j���Ɏ��s�����ꍇ

		// ��O����
		assert(false);
	}

	// �I�u�W�F�N�g�L�����N�^�[��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObjectChara::Update(void)
{
	// ���[�V�����̍X�V
	m_pMotion->Update();
}

//============================================================
//	�`�揈��
//============================================================
void CObjectChara::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntObjectChara = 0; nCntObjectChara < m_nNumModel; nCntObjectChara++)
	{ // �p�[�c�̑������J��Ԃ�

		// �p�[�c�̕`��
		m_apMultiModel[nCntObjectChara]->Draw();
	}
}

//============================================================
//	�p�[�c���̐ݒ菈��
//============================================================
void CObjectChara::SetPartsInfo
(
	const int nID,				// �p�[�c�C���f�b�N�X
	const int nParentID,		// �e�C���f�b�N�X
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const char *pFileName		// �t�@�C����
)
{
	// �ϐ���錾
	int nModelID = 0;	// ���f���C���f�b�N�X

	// �|�C���^��錾
	CModel *pModel = CManager::GetModel();	// ���f���ւ̃|�C���^

	if (nID < MAX_PARTS && nParentID < MAX_PARTS && USED(pFileName))
	{ // �p�[�c�E�e�C���f�b�N�X���z��͈͓����A�t�@�C���������݂���ꍇ

		// ���f���̐���
		m_apMultiModel[nID] = CMultiModel::Create(rPos, rRot);

		// ���f����o�^
		nModelID = pModel->Regist(pFileName);

		// ���f��������
		m_apMultiModel[nID]->BindModel(pModel->GetModel(nModelID));

		// �e���f���̐ݒ�
		if (nParentID == NONE_IDX)
		{ // �e���Ȃ��ꍇ

			// NULL��ݒ�
			m_apMultiModel[nID]->SetParent(NULL);
		}
		else
		{ // �e������ꍇ

			// �e�̃A�h���X��ݒ�
			m_apMultiModel[nID]->SetParent(m_apMultiModel[nParentID]);
		}

		// �p�[�c�̑��������Z
		m_nNumModel++;
	}
}

//============================================================
//	���f�����̐ݒ菈��
//============================================================
void CObjectChara::SetModelInfo(void)
{
	// ���f�����̐ݒ�
	m_pMotion->SetModel(&m_apMultiModel[0], m_nNumModel);
}

//============================================================
//	���[�V�������̐ݒ菈��
//============================================================
void CObjectChara::SetMotionInfo(CMotion::MotionInfo info)
{
	// ���[�V�������̐ݒ�
	m_pMotion->SetInfo(info);
}

//============================================================
//	���[�V�����̐ݒ菈��
//============================================================
void CObjectChara::SetMotion(const int nType)
{
	// ���[�V�����̐ݒ�
	m_pMotion->Set(nType);
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CObjectChara::SetPosition(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObjectChara::SetRotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_rot = rRot;

	// �����̐��K��
	useful::NormalizeRot(m_rot.x);
	useful::NormalizeRot(m_rot.y);
	useful::NormalizeRot(m_rot.z);
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObjectChara::GetPosition(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CObjectChara::GetRotation(void) const
{
	// ������Ԃ�
	return m_rot;
}
