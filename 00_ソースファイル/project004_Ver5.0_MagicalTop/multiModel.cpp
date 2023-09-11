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
	m_pMat	= NULL;			// �}�e���A���ւ̃|�C���^
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
	m_pMat	= NULL;			// �}�e���A���ւ̃|�C���^
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
	// �}�e���A���ւ̃|�C���^��j��
	if (USED(m_pMat))
	{ // �|�C���^���g�p����Ă����ꍇ

		// �������J��
		delete[] m_pMat;
		m_pMat = NULL;
	}
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
		mtxParent = m_pParent->m_mtxWorld;
	}

	// ���[���h�}�g���b�N�X�Ɛe�}�g���b�N�X���|�����킹��
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	for (int nCntMat = 0; nCntMat < (int)m_modelData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&m_pMat[nCntMat].MatD3D);

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

		// �}�e���A���̐ݒ�
		SetMaterial(m_modelData.pBuffMat, (int)m_modelData.dwNumMat);
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
//	�}�e���A���̐ݒ菈��
//============================================================
void CMultiModel::SetMaterial(const D3DXMATERIAL& rMat)
{
	// �����̃}�e���A����S�}�e���A���ɐݒ�
	for (int nCntMat = 0; nCntMat < (int)m_modelData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		m_pMat[nCntMat] = rMat;
	}
}

//============================================================
//	�}�e���A���̍Đݒ菈��
//============================================================
void CMultiModel::ResetMaterial(void)
{
	// �|�C���^��錾
	D3DXMATERIAL *pOriginMat;	// �}�e���A���f�[�^�ւ̃|�C���^

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pOriginMat = (D3DXMATERIAL*)m_modelData.pBuffMat->GetBufferPointer();

	// �S�}�e���A���ɏ����}�e���A�����Đݒ�
	for (int nCntMat = 0; nCntMat < (int)m_modelData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		m_pMat[nCntMat] = pOriginMat[nCntMat];
	}
}

//============================================================
//	�����x�̐ݒ菈��
//============================================================
void CMultiModel::SetAlpha(const float fAlpha)
{
	// �|�C���^��錾
	D3DXMATERIAL *pOriginMat;	// �}�e���A���f�[�^�ւ̃|�C���^

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pOriginMat = (D3DXMATERIAL*)m_modelData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_modelData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		// �ϐ���錾
		float fSetAlpha = fAlpha;	// �ݒ肷�铧���x

		// �����x�̕␳
		useful::LimitNum(fSetAlpha, 0.0f, pOriginMat[nCntMat].MatD3D.Diffuse.a);

		// �����x��ݒ�
		m_pMat[nCntMat].MatD3D.Diffuse.a = fSetAlpha;
	}
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
//	�}�e���A���̐ݒ菈��
//============================================================
HRESULT CMultiModel::SetMaterial(const LPD3DXBUFFER pBuffMat, const int nNumMat)
{
	// �|�C���^��錾
	D3DXMATERIAL *pOriginMat;	// �}�e���A���f�[�^�ւ̃|�C���^

	//--------------------------------------------------------
	//	�������J���E�m��
	//--------------------------------------------------------
	if (USED(m_pMat))
	{ // �|�C���^���g�p����Ă����ꍇ

		// �������J��
		delete[] m_pMat;
		m_pMat = NULL;
	}

	if (UNUSED(m_pMat))
	{ // �|�C���^���g�p����Ă��Ȃ��ꍇ

		// �}�e���A�������̃������m��
		m_pMat = new D3DXMATERIAL[nNumMat];

		if (USED(m_pMat))
		{ // �m�ۂɐ��������ꍇ

			// �������N���A
			memset(m_pMat, 0, sizeof(D3DXMATERIAL) * nNumMat);
		}
		else { assert(false); return E_FAIL; }	// �m�ێ��s
	}
	else { assert(false); return E_FAIL; }	// �g�p��

	//--------------------------------------------------------
	//	�}�e���A������ݒ�
	//--------------------------------------------------------
	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pOriginMat = (D3DXMATERIAL*)m_modelData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < nNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		// �}�e���A���f�[�^���R�s�[
		m_pMat[nCntMat] = pOriginMat[nCntMat];
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�}�g���b�N�X�|�C���^�擾����
//============================================================
D3DXMATRIX *CMultiModel::GetPtrMtxWorld(void)
{
	// ���[���h�}�g���b�N�X�̃|�C���^��Ԃ�
	return &m_mtxWorld;
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
//	�}�e���A���擾����
//============================================================
D3DXMATERIAL CMultiModel::GetMaterial(const int nID) const
{
	// �ϐ���錾
	D3DXMATERIAL mat;	// ��O���̃}�e���A��

	// �}�e���A���̃��������N���A
	ZeroMemory(&mat, sizeof(mat));

	if (nID < (int)m_modelData.dwNumMat)
	{ // �����C���f�b�N�X���}�e���A���̍ő吔�𒴂��Ă��Ȃ��ꍇ

		// �����C���f�b�N�X�̃}�e���A����Ԃ�
		return m_pMat[nID];
	}
	else
	{ // �����C���f�b�N�X���}�e���A���̍ő吔�𒴂��Ă���ꍇ

		// ��O����
		assert(false);

		// ��O���̃}�e���A����Ԃ�
		return mat;
	}
}

//============================================================
//	�����x�擾����
//============================================================
float CMultiModel::GetAlpha(void) const
{
	// �ϐ���錾
	float fAlpha = 0.0f;	// �ł��s�����ȃ}�e���A���̓����x

	// �ł��s�����ȓ����x��T��
	for (int nCntMat = 0; nCntMat < (int)m_modelData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		if (m_pMat[nCntMat].MatD3D.Diffuse.a > fAlpha)
		{ // �}�e���A���̓����x�����s�����������ꍇ

			// ���݂̃}�e���A���̓����x��ۑ�
			fAlpha = m_pMat[nCntMat].MatD3D.Diffuse.a;
		}
	}

	// �S�p�[�c���ōł��s�����������}�e���A���̓����x��Ԃ�
	return fAlpha;
}

//============================================================
//	�ő哧���x�擾����
//============================================================
float CMultiModel::GetMaxAlpha(void) const
{
	// �ϐ���錾
	float fAlpha = 0.0f;		// �ł��s�����ȃ}�e���A���̓����x

	// �|�C���^��錾
	D3DXMATERIAL *pOriginMat;	// �}�e���A���f�[�^�ւ̃|�C���^

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pOriginMat = (D3DXMATERIAL*)m_modelData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_modelData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		if (pOriginMat[nCntMat].MatD3D.Diffuse.a > fAlpha)
		{ // �}�e���A���̓����x�����s�����������ꍇ

			// ���݂̃}�e���A���̓����x��ۑ�
			fAlpha = pOriginMat[nCntMat].MatD3D.Diffuse.a;
		}
	}

	// �S�p�[�c���ōł��s�����������}�e���A���̓����x��Ԃ�
	return fAlpha;
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
