//============================================================
//
//	�}���`���f������ [multiModel.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "multiModel.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�e�N���X [CMultiModel] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CMultiModel::CMultiModel()
{
	// �����o�ϐ����N���A
	memset(&m_modelData, 0, sizeof(m_modelData));	// ���f�����
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));		// ���[���h�}�g���b�N�X
	m_pParent = NULL;		// �e���f���ւ̃|�C���^
	m_pos	= VEC3_ZERO;	// �ʒu
	m_rot	= VEC3_ZERO;	// ����
	m_scale	= VEC3_ZERO;	// �傫��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CMultiModel::~CMultiModel()
{

}

//============================================================
//	����������
//============================================================
HRESULT CMultiModel::Init(void)
{
	// �����o�ϐ���������
	memset(&m_modelData, 0, sizeof(m_modelData));	// ���f�����
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));		// ���[���h�}�g���b�N�X
	m_pParent = NULL;		// �e���f���ւ̃|�C���^
	m_pos	= VEC3_ZERO;	// �ʒu
	m_rot	= VEC3_ZERO;	// ����
	m_scale	= VEC3_ONE;		// �傫��

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CMultiModel::Uninit(void)
{

}

//============================================================
//	�X�V����
//============================================================
void CMultiModel::Update(void)
{

}

//============================================================
//	�`�揈��
//============================================================
void CMultiModel::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX   mtxParent;	// �e�̃}�g���b�N�X
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

	// �e�}�g���b�N�X��ݒ�
	if (UNUSED(m_pParent))
	{ // �e�����݂��Ȃ��ꍇ

		// ���݂̃}�g���b�N�X���擾
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);	// �ݒ肳�ꂽ�ŐV�̃}�g���b�N�X (���̂̃}�g���b�N�X)
	}
	else
	{ // �e�����݂���ꍇ

		// �e�̃}�g���b�N�X��ݒ�
		mtxParent = m_pParent->GetMtxWorld();
	}

	// ���[���h�}�g���b�N�X�Ɛe�}�g���b�N�X���|�����킹��
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

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

		// ���f���̕`��
		m_modelData.pMesh->DrawSubset(nCntMat);

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
CMultiModel *CMultiModel::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot, const D3DXVECTOR3& rScale)
{
	// �|�C���^��錾
	CMultiModel *pMultiModel = NULL;	// �}���`���f�������p

	if (UNUSED(pMultiModel))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pMultiModel = new CMultiModel;	// �}���`���f��
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pMultiModel))
	{ // �m�ۂɐ������Ă���ꍇ

		// �}���`���f���̏�����
		if (FAILED(pMultiModel->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pMultiModel;
			pMultiModel = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		pMultiModel->SetPosition(rPos);

		// ������ݒ�
		pMultiModel->SetRotation(rRot);

		// �傫����ݒ�
		pMultiModel->SetScaling(rScale);

		// �m�ۂ����A�h���X��Ԃ�
		return pMultiModel;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CMultiModel::Release(CMultiModel *&prMultiModel)
{
	if (USED(prMultiModel))
	{ // �g�p���̏ꍇ

		// �}���`���f���̏I��
		prMultiModel->Uninit();

		// �������J��
		delete prMultiModel;
		prMultiModel = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	���f����������
//============================================================
void CMultiModel::BindModel(CModel::Model *pModel)
{
	if (USED(pModel))
	{ // ���蓖�Ă郂�f�����g�p���̏ꍇ

		// ���f�������蓖��
		m_modelData = *pModel;
	}
	else { assert(false); }	// ��g�p��
}

//============================================================
//	�e���f���̐ݒ菈��
//============================================================
void CMultiModel::SetParent(CMultiModel *pModel)
{
	// �����̃��f���|�C���^��e�ɐݒ�
	m_pParent = pModel;
}

//============================================================
//	�}�g���b�N�X�̐ݒ菈��
//============================================================
void CMultiModel::SetMtxWorld(const D3DXMATRIX& rMtxWorld)
{
	// �����̃}�g���b�N�X��ݒ�
	m_mtxWorld = rMtxWorld;
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CMultiModel::SetPosition(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CMultiModel::SetRotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_rot = rRot;

	// �����̐��K��
	useful::NormalizeRot(m_rot.x);
	useful::NormalizeRot(m_rot.y);
	useful::NormalizeRot(m_rot.z);
}

//============================================================
//	�傫���̐ݒ菈��
//============================================================
void CMultiModel::SetScaling(const D3DXVECTOR3& rScale)
{
	// �����̑傫������
	m_scale = rScale;
}

//============================================================
//	���f�����̐ݒ菈��
//============================================================
void CMultiModel::SetModelData(const CModel::Model& rModel)
{
	// �����̃��f��������
	m_modelData = rModel;
}

//============================================================
//	�}�g���b�N�X�擾����
//============================================================
D3DXMATRIX CMultiModel::GetMtxWorld(void) const
{
	// ���[���h�}�g���b�N�X��Ԃ�
	return m_mtxWorld;
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CMultiModel::GetPosition(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CMultiModel::GetRotation(void) const
{
	// ������Ԃ�
	return m_rot;
}

//============================================================
//	�傫���擾����
//============================================================
D3DXVECTOR3 CMultiModel::GetScaling(void) const
{
	// �傫����Ԃ�
	return m_scale;
}

//============================================================
//	�e�擾����
//============================================================
CMultiModel CMultiModel::GetParent(void) const
{
	// �e��Ԃ�
	return *m_pParent;
}

//============================================================
//	���f�����擾����
//============================================================
CModel::Model CMultiModel::GetModelData(void) const
{
	// ���f������Ԃ�
	return m_modelData;
}
