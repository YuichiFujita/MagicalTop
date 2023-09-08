//============================================================
//
//	�I�u�W�F�N�g���f������ [objectModel.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectModel.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�q�N���X [CObjectModel] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectModel::CObjectModel()
{
	// �����o�ϐ����N���A
	memset(&m_modelData, 0, sizeof(m_modelData));	// ���f�����
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));		// ���[���h�}�g���b�N�X
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_scale	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �傫��
}

//============================================================
//	�I�[�o�[���[�h�R���X�g���N�^
//============================================================
CObjectModel::CObjectModel(const CObject::LABEL label, const int nPriority) : CObject(label, nPriority)
{
	// �����o�ϐ����N���A
	memset(&m_modelData, 0, sizeof(m_modelData));	// ���f�����
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));		// ���[���h�}�g���b�N�X
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �傫��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObjectModel::~CObjectModel()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectModel::Init(void)
{
	// �����o�ϐ���������
	memset(&m_modelData, 0, sizeof(m_modelData));	// ���f�����
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));		// ���[���h�}�g���b�N�X
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);		// �傫��

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CObjectModel::Uninit(void)
{
	// �I�u�W�F�N�g���f����j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObjectModel::Update(void)
{

}

//============================================================
//	�`�揈��
//============================================================
void CObjectModel::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;	// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^
	D3DXMATERIAL *pMat;		// �}�e���A���f�[�^�ւ̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �g�嗦�𔽉f
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_modelData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_modelData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, pTexture->GetTexture(m_modelData.pTextureID[nCntMat]));

		if (m_scale != VEC3_ONE)
		{ // �g�嗦���ύX����Ă���ꍇ

			// ���_�@���̎������K����L���ɂ���
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// ���f���̕`��
		m_modelData.pMesh->DrawSubset(nCntMat);

		// ���_�@���̎������K���𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//============================================================
//	��������
//============================================================
CObjectModel *CObjectModel::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot, const D3DXVECTOR3& rScale)
{
	// �|�C���^��錾
	CObjectModel *pObjectModel = NULL;		// �I�u�W�F�N�g���f�������p

	if (UNUSED(pObjectModel))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pObjectModel = new CObjectModel;	// �I�u�W�F�N�g���f��
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pObjectModel))
	{ // �m�ۂɐ������Ă���ꍇ

		// �I�u�W�F�N�g���f���̏�����
		if (FAILED(pObjectModel->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pObjectModel;
			pObjectModel = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		pObjectModel->SetPosition(rPos);

		// ������ݒ�
		pObjectModel->SetRotation(rRot);

		// �傫����ݒ�
		pObjectModel->SetScaling(rScale);

		// �m�ۂ����A�h���X��Ԃ�
		return pObjectModel;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	���f����������
//============================================================
void CObjectModel::BindModel(CModel::Model *pModel)
{
	if (USED(pModel))
	{ // ���蓖�Ă郂�f�����g�p���̏ꍇ

		// ���f�������蓖��
		m_modelData = *pModel;
	}
	else { assert(false); }	// ��g�p��
}

//============================================================
//	�}�g���b�N�X�̐ݒ菈��
//============================================================
void CObjectModel::SetMtxWorld(const D3DXMATRIX& rMtxWorld)
{
	// �����̃}�g���b�N�X��ݒ�
	m_mtxWorld = rMtxWorld;
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CObjectModel::SetPosition(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObjectModel::SetRotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_rot = rRot;
}

//============================================================
//	�傫���̐ݒ菈��
//============================================================
void CObjectModel::SetScaling(const D3DXVECTOR3& rScale)
{
	// �����̑傫������
	m_scale = rScale;
}

//============================================================
//	���f�����̐ݒ菈��
//============================================================
void CObjectModel::SetModelData(const CModel::Model& rModel)
{
	// �����̃��f��������
	m_modelData = rModel;
}

//============================================================
//	�}�g���b�N�X�|�C���^�擾����
//============================================================
D3DXMATRIX *CObjectModel::GetPtrMtxWorld(void)
{
	// ���[���h�}�g���b�N�X�̃|�C���^��Ԃ�
	return &m_mtxWorld;
}

//============================================================
//	�}�g���b�N�X�擾����
//============================================================
D3DXMATRIX CObjectModel::GetMtxWorld(void) const
{
	// ���[���h�}�g���b�N�X��Ԃ�
	return m_mtxWorld;
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObjectModel::GetPosition(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CObjectModel::GetRotation(void) const
{
	// ������Ԃ�
	return m_rot;
}

//============================================================
//	�傫���擾����
//============================================================
D3DXVECTOR3 CObjectModel::GetScaling(void) const
{
	// �傫����Ԃ�
	return m_scale;
}

//============================================================
//	���f�����擾����
//============================================================
CModel::Model CObjectModel::GetModelData(void) const
{
	// ���f������Ԃ�
	return m_modelData;
}
