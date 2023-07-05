//============================================================
//
//	�I�u�W�F�N�g���b�V���t�B�[���h���� [objectMeshField.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectMeshField.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"

//************************************************************
//	�}�N����`
//************************************************************
#define NUM_VTX_TRIANGLE	(3)	// �O�p�`�|���S���̒��_��

//************************************************************
//	�q�N���X [CObjectMeshField] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectMeshField::CObjectMeshField()
{
	// �����o�ϐ����N���A
	m_pVtxBuff = NULL;		// ���_�o�b�t�@
	m_pIdxBuff = NULL;		// �C���f�b�N�X�o�b�t�@
	m_part = GRID2_ZERO;	// ������
	m_nNumVtx = 0;			// �K�v���_��
	m_nNumIdx = 0;			// �K�v�C���f�b�N�X��
	m_nTextureID = 0;		// �e�N�X�`���C���f�b�N�X
	memset(&m_meshField, 0, sizeof(m_meshField));	// ���b�V���t�B�[���h�̏��
}

//============================================================
//	�I�[�o�[���[�h�R���X�g���N�^
//============================================================
CObjectMeshField::CObjectMeshField(const CObject::LABEL label, const int nPriority) : CObject(label, nPriority)
{
	// �����o�ϐ����N���A
	m_pVtxBuff = NULL;		// ���_�o�b�t�@
	m_pIdxBuff = NULL;		// �C���f�b�N�X�o�b�t�@
	m_part = GRID2_ZERO;	// ������
	m_nNumVtx = 0;			// �K�v���_��
	m_nNumIdx = 0;			// �K�v�C���f�b�N�X��
	m_nTextureID = 0;		// �e�N�X�`���C���f�b�N�X
	memset(&m_meshField, 0, sizeof(m_meshField));	// ���b�V���t�B�[���h�̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObjectMeshField::~CObjectMeshField()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectMeshField::Init(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// �����o�ϐ���������
	m_pVtxBuff = NULL;		// ���_�o�b�t�@
	m_pIdxBuff = NULL;		// �C���f�b�N�X�o�b�t�@
	m_part = GRID2_ZERO;	// ������
	m_nNumVtx = 0;			// �K�v���_��
	m_nNumIdx = 0;			// �K�v�C���f�b�N�X��
	m_nTextureID = -1;		// �e�N�X�`���C���f�b�N�X

	m_meshField.pos = VEC3_ZERO;	// �ʒu
	m_meshField.rot = VEC3_ZERO;	// ����
	m_meshField.size = VEC2_ZERO;	// �傫��
	m_meshField.col = XCOL_WHITE;	// �F
	m_meshField.cull = D3DCULL_CCW;	// �J�����O��
	m_meshField.bLight = true;		// ���C�e�B���O��

	// ��������ݒ�
	if (FAILED(SetPattern(GRID2_ONE)))
	{ // �������̐ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CObjectMeshField::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (USED(m_pVtxBuff))
	{ // ���_�o�b�t�@���g�p���̏ꍇ

		// �������J��
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̔j��
	if (USED(m_pIdxBuff))
	{ // �C���f�b�N�X�o�b�t�@���g�p���̏ꍇ

		// �������J��
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// �I�u�W�F�N�g���b�V���t�B�[���h��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObjectMeshField::Update(void)
{

}

//============================================================
//	�`�揈��
//============================================================
void CObjectMeshField::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^
	CTexture *pTexture = CManager::GetTexture();						// �e�N�X�`���ւ̃|�C���^

	// �|���S���̗��ʂ݂̂�\����Ԃɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, m_meshField.cull);

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, m_meshField.bLight);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_meshField.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_meshField.rot.y, m_meshField.rot.x, m_meshField.rot.z);
	D3DXMatrixMultiply(&m_meshField.mtxWorld, &m_meshField.mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_meshField.pos.x, m_meshField.pos.y, m_meshField.pos.z);
	D3DXMatrixMultiply(&m_meshField.mtxWorld, &m_meshField.mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_meshField.mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetTexture(m_nTextureID));

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive
	( // ����
		D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
		0,
		0,
		m_nNumVtx,		// �g�p���钸�_��
		0,				// �C���f�b�N�X�o�b�t�@�̊J�n�n�_
		m_nNumIdx - 2	// �v���~�e�B�u (�|���S��) ��
	);

	// �|���S���̕\�ʂ݂̂�\����Ԃɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
}

//============================================================
//	��������
//============================================================
CObjectMeshField *CObjectMeshField::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXVECTOR2& rSize,	// �傫��
	const D3DXCOLOR& rCol,		// �F
	const POSGRID2& rPart,		// ������
	const D3DCULL cull,			// �J�����O��
	const bool bLight			// ���C�e�B���O��
)
{
	// �|�C���^��錾
	CObjectMeshField *pObjectMeshField = NULL;		// �I�u�W�F�N�g���b�V���t�B�[���h�����p

	if (UNUSED(pObjectMeshField))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pObjectMeshField = new CObjectMeshField;	// �I�u�W�F�N�g���b�V���t�B�[���h
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pObjectMeshField))
	{ // �m�ۂɐ������Ă���ꍇ

		if (SUCCEEDED(pObjectMeshField->GetResult()))
		{ // �I�u�W�F�N�g�̏��ݒ�ɐ��������ꍇ

			// �I�u�W�F�N�g���b�V���t�B�[���h�̏�����
			if (FAILED(pObjectMeshField->Init()))
			{ // �������Ɏ��s�����ꍇ

				// �������J��
				delete pObjectMeshField;
				pObjectMeshField = NULL;

				// ���s��Ԃ�
				return NULL;
			}

			// �ʒu��ݒ�
			pObjectMeshField->SetPosition(rPos);

			// ������ݒ�
			pObjectMeshField->SetRotation(rRot);

			// �傫����ݒ�
			pObjectMeshField->SetScalingBiaxial(rSize);

			// �F��ݒ�
			pObjectMeshField->SetColor(rCol);

			// �J�����O��ݒ�
			pObjectMeshField->SetCulling(cull);

			// ���C�e�B���O��ݒ�
			pObjectMeshField->SetLighting(bLight);

			// ��������ݒ�
			if (FAILED(pObjectMeshField->SetPattern(rPart)))
			{ // �������̐ݒ�Ɏ��s�����ꍇ

				// �������J��
				delete pObjectMeshField;
				pObjectMeshField = NULL;

				// ���s��Ԃ�
				return NULL;
			}

			// �m�ۂ����A�h���X��Ԃ�
			return pObjectMeshField;
		}
		else
		{ // �I�u�W�F�N�g�̏��ݒ�Ɏ��s�����ꍇ

			// �������J��
			delete pObjectMeshField;
			pObjectMeshField = NULL;

			// ���s��Ԃ�
			return NULL;
		}
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�e�N�X�`����������
//============================================================
void CObjectMeshField::BindTexture(const int nTextureID)
{
	// �e�N�X�`���C���f�b�N�X����
	m_nTextureID = nTextureID;
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CObjectMeshField::SetPosition(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_meshField.pos = rPos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObjectMeshField::SetRotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_meshField.rot = rRot;

	// �����̐��K��
	useful::NormalizeRot(m_meshField.rot.z);
}

//============================================================
//	�傫���̐ݒ菈��
//============================================================
void CObjectMeshField::SetScalingBiaxial(const D3DXVECTOR2& rSize)
{
	// �����̑傫����ݒ�
	m_meshField.size = rSize;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CObjectMeshField::SetColor(const D3DXCOLOR& rCol)
{
	// �����̐F��ݒ�
	m_meshField.col = rCol;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�J�����O�ݒ菈��
//============================================================
void CObjectMeshField::SetCulling(const D3DCULL cull)
{
	// �����̃J�����O�󋵂�ݒ�
	m_meshField.cull = cull;
}

//============================================================
//	���C�e�B���O�ݒ菈��
//============================================================
void CObjectMeshField::SetLighting(const bool bLight)
{
	// �����̃��C�e�B���O�󋵂�ݒ�
	m_meshField.bLight = bLight;
}

//============================================================
//	�������̐ݒ菈��
//============================================================
HRESULT CObjectMeshField::SetPattern(const POSGRID2& rPart)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// �����̕�������ݒ�
	m_part = rPart;

	// �K�v���_�E�C���f�b�N�X�������߂�
	m_nNumVtx = (m_part.x + 1) * (m_part.y + 1); // �K�v���_��
	m_nNumIdx = (m_part.x + 1) * (((m_part.y + 1) * 2) - 2) + (m_part.y * 2) - 2; // �K�v�C���f�b�N�X��

	// ���_�o�b�t�@�̔j��
	if (USED(m_pVtxBuff))
	{ // ���_�o�b�t�@���g�p���̏ꍇ

		// �������J��
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// ���_�o�b�t�@�̏���ݒ�
	if (UNUSED(m_pVtxBuff))
	{ // ��g�p���̏ꍇ

		// ���_�o�b�t�@�̐���
		if (FAILED(pDevice->CreateVertexBuffer
		( // ����
			sizeof(VERTEX_3D) * m_nNumVtx,	// �K�v���_��
			D3DUSAGE_WRITEONLY,	// �g�p���@
			FVF_VERTEX_3D,		// ���_�t�H�[�}�b�g
			D3DPOOL_MANAGED,	// �������̎w��
			&m_pVtxBuff,		// ���_�o�b�t�@�ւ̃|�C���^
			NULL
		)))
		{ // ���_�o�b�t�@�̐����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}
	else { assert(false); return E_FAIL; }	// �g�p��

	// �C���f�b�N�X�o�b�t�@�̔j��
	if (USED(m_pIdxBuff))
	{ // �C���f�b�N�X�o�b�t�@���g�p���̏ꍇ

		// �������J��
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̏���ݒ�
	if (UNUSED(m_pIdxBuff))
	{ // ��g�p���̏ꍇ

		// �C���f�b�N�X�o�b�t�@�̐���
		if (FAILED(pDevice->CreateIndexBuffer
		( // ����
			sizeof(WORD) * m_nNumIdx,	// �K�v�C���f�b�N�X��
			D3DUSAGE_WRITEONLY,	// �g�p���@
			D3DFMT_INDEX16,		// �C���f�b�N�X�o�b�t�@�̃t�H�[�}�b�g
			D3DPOOL_MANAGED,	// �������̎w��
			&m_pIdxBuff,		// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
			NULL
		)))
		{ // �C���f�b�N�X�o�b�t�@�̐����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}
	else { assert(false); return E_FAIL; }	// �g�p��

	// ���_�E�C���f�b�N�X���̐ݒ�
	SetVtx();
	SetIdx();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	���b�V���̒��_�ʒu�̐ݒ菈��
//============================================================
void CObjectMeshField::SetMeshVertexPosition(const int nID, const D3DXVECTOR3& rPos)
{
	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	if (USED(m_pVtxBuff))
	{ // �g�p���̏ꍇ

		if (nID < (m_part.x + 1) * (m_part.y + 1))
		{ // �C���f�b�N�X���g�p�\�ȏꍇ

			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// ���_�f�[�^�̃|�C���^���������i�߂�
			pVtx += nID;

			// ���_���W�̐ݒ�
			pVtx[0].pos = rPos;

			// ���_�o�b�t�@���A�����b�N����
			m_pVtxBuff->Unlock();
		}
	}
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObjectMeshField::GetPosition(void) const
{
	// �ʒu��Ԃ�
	return m_meshField.pos;
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CObjectMeshField::GetRotation(void) const
{
	// ������Ԃ�
	return m_meshField.rot;
}

//============================================================
//	�傫���擾����
//============================================================
D3DXVECTOR2 CObjectMeshField::GetScalingBiaxial(void) const
{
	// �傫����Ԃ�
	return m_meshField.size;
}

//============================================================
//	�F�擾����
//============================================================
D3DXCOLOR CObjectMeshField::GetColor(void) const
{
	// �F��Ԃ�
	return m_meshField.col;
}

//============================================================
//	�J�����O�擾����
//============================================================
D3DCULL CObjectMeshField::GetCulling(void) const
{
	// �J�����O�󋵂�Ԃ�
	return m_meshField.cull;
}

//============================================================
//	���C�e�B���O�擾����
//============================================================
bool CObjectMeshField::GetLighting(void) const
{
	// ���C�e�B���O�󋵂�Ԃ�
	return m_meshField.bLight;
}

//============================================================
//	�������擾����
//============================================================
POSGRID2 CObjectMeshField::GetPattern(void) const
{
	// ��������Ԃ�
	return m_part;
}

//============================================================
//	���b�V���̒��_�ʒu�̐ݒ菈��
//============================================================
D3DXVECTOR3 CObjectMeshField::GetMeshVertexPosition(const int nID)
{
	// �ϐ���錾
	D3DXVECTOR3 pos;	// ���_���W�̑���p

	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	if (USED(m_pVtxBuff))
	{ // �g�p���̏ꍇ

		if (nID < (m_part.x + 1) * (m_part.y + 1))
		{ // �C���f�b�N�X���g�p�\�ȏꍇ

			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// ���_�f�[�^�̃|�C���^���������i�߂�
			pVtx += nID;

			// ���_���W����
			pos = pVtx[0].pos;

			// ���_�o�b�t�@���A�����b�N����
			m_pVtxBuff->Unlock();
		}
	}

	// �����̃C���f�b�N�X�̒��_���W��Ԃ�
	return pos;
}

//============================================================
//	���b�V���̒��n�ʒu�̎擾����
//============================================================
float CObjectMeshField::GetPositionHeight(const D3DXVECTOR3&rPos)
{
	// �ϐ���錾
	int nNumCul;		// �@���x�N�g���p�̒��_�v�Z�p
	int nNumVtx;		// �@�������߂钸�_�ԍ�
	D3DXVECTOR3 nor;	// �@���x�N�g��

	// �ϐ��z���錾
	D3DXVECTOR3 aVtxPos[NUM_VTX_TRIANGLE];		// �|���S���̒��_���W
	D3DXVECTOR3 aVtxMtxPos[NUM_VTX_TRIANGLE];	// �|���S���̈ʒu�E�������f���s�������_���W

	for (int nCntVtx = 0; nCntVtx < 2; nCntVtx++)
	{ // �l�p�|���S���Ɏg�p����O�p�̐����J��Ԃ�

		// �@���x�N�g���p�̒��_�v�Z�p�̒l��ݒ�
		nNumCul = (nCntVtx == 0) ? 1: -1;

		for (int nCntHeight = 0; nCntHeight < m_part.y; nCntHeight++)
		{ // �c�̕��������J��Ԃ�

			for (int nCntWidth = 0; nCntWidth < m_part.x; nCntWidth++)
			{ // ���̕��������J��Ԃ�

				// �@�������߂钸�_�ԍ���ݒ�
				nNumVtx = ((nCntWidth + 1) + (nCntHeight * (m_part.x + 1))) + (nCntVtx * m_part.x);

				// �|���S���̒��_�ʒu���擾
				aVtxPos[0] = GetMeshVertexPosition(nNumVtx);
				aVtxPos[1] = GetMeshVertexPosition(nNumVtx - (1 * nNumCul));
				aVtxPos[2] = GetMeshVertexPosition(nNumVtx + ((m_part.x + 1) * nNumCul));

				for (int nCnt = 0; nCnt < NUM_VTX_TRIANGLE; nCnt++)
				{ // �O�p�`�|���S���̒��_�����J��Ԃ�

					// �ϐ���錾
					D3DXMATRIX mtxWorld, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

					// ���[���h�}�g���b�N�X�̏�����
					D3DXMatrixIdentity(&mtxWorld);

					// ���_�ʒu�𔽉f
					D3DXMatrixTranslation(&mtxTrans, aVtxPos[nCnt].x, aVtxPos[nCnt].y, aVtxPos[nCnt].z);
					D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

					// �|���S�������𔽉f
					D3DXMatrixRotationYawPitchRoll(&mtxRot, m_meshField.rot.y, m_meshField.rot.x, m_meshField.rot.z);
					D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

					// �|���S���ʒu�𔽉f
					D3DXMatrixTranslation(&mtxTrans, m_meshField.pos.x, m_meshField.pos.y, m_meshField.pos.z);
					D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

					// �v�Z�����}�g���b�N�X������W��ݒ�
					aVtxMtxPos[nCnt] = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
				}

				if (collision::TrianglePillar(aVtxMtxPos[0], aVtxMtxPos[1], aVtxMtxPos[2], rPos))
				{ // �|���S���͈͓̔��ɂ���ꍇ

					// �@�������߂�
					useful::NormalizeNormal(aVtxMtxPos[1], aVtxMtxPos[0], aVtxMtxPos[2], nor);

					if (nor.y != 0.0f)
					{ // �@�����ݒ肳��Ă���ꍇ

						// �v���C���[�̒��n�_��Ԃ�
						return (((rPos.x - aVtxMtxPos[0].x) * nor.x + (-aVtxMtxPos[0].y) * nor.y + (rPos.z - aVtxMtxPos[0].z) * nor.z) * -1.0f) / nor.y;
					}
				}
			}
		}
	}

	// ���n�͈͊O�̏ꍇ���݂�y���W��Ԃ�
	return rPos.y;
}

//============================================================
//	�͈͊O�̒��n����
//============================================================
bool CObjectMeshField::LandPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove, const float fHeight)
{
	// �ϐ���錾
	float fLandHeight = GetPositionHeight(rPos);	// ���n�ʒu

	if (rPos.y - fHeight < fLandHeight)
	{ // �ʒu���n�ʂ�艺�̏ꍇ

		// �ʒu��␳
		rPos.y = fLandHeight + fHeight;

		// �ړ��ʂ�������
		rMove.y = 0.0f;

		// ���n���Ă���󋵂�Ԃ�
		return true;
	}

	// ���n���Ă��Ȃ��󋵂�Ԃ�
	return false;
}

//============================================================
//	���_���̐ݒ菈��
//============================================================
void CObjectMeshField::SetVtx(void)
{
	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	if (USED(m_pVtxBuff))
	{ // �g�p���̏ꍇ

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntHeight = 0; nCntHeight < m_part.y + 1; nCntHeight++)
		{ // �c�̕����� +1��J��Ԃ�

			for (int nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
			{ // ���̕����� +1��J��Ԃ�

				// ���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3
				( // ����
					nCntWidth * (m_meshField.size.x / (float)m_part.x) - (m_meshField.size.x * 0.5f),		// x
					0.0f,																					// y
					-(nCntHeight * (m_meshField.size.y / (float)m_part.y) - (m_meshField.size.y * 0.5f))	// z
				);

				// �@���x�N�g���̐ݒ�
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// ���_�J���[�̐ݒ�
				pVtx[0].col = m_meshField.col;

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(1.0f * nCntWidth, 1.0f * nCntHeight);

				// ���_�f�[�^�̃|�C���^�� 1���i�߂�
				pVtx += 1;
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//============================================================
//	�C���f�b�N�X���̐ݒ菈��
//============================================================
void CObjectMeshField::SetIdx(void)
{
	// �|�C���^��錾
	WORD *pIdx;	// �C���f�b�N�X���ւ̃|�C���^

	if (USED(m_pIdxBuff))
	{ // �g�p���̏ꍇ

		// �C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
		m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntHeight = 0, nCntWidth = 0; nCntHeight < m_part.y; nCntHeight++)
		{ // �c�̕����� +1��J��Ԃ�

			for (nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
			{ // ���̕����� +1��J��Ԃ�

				pIdx[0] = (m_part.x + 1) * (nCntHeight + 1) + nCntWidth;
				pIdx[1] = (m_part.x + 1) * nCntHeight + nCntWidth;

				// �C���f�b�N�X�f�[�^�̃|�C���^�� 2���i�߂�
				pIdx += 2;
			}

			if (nCntHeight != m_part.y - 1)
			{ // ��Ԏ�O�̕����ꏊ�ł͂Ȃ��ꍇ

				pIdx[0] = (m_part.x + 1) * nCntHeight + nCntWidth - 1;
				pIdx[1] = (m_part.x + 1) * (nCntHeight + 2);

				// �C���f�b�N�X�f�[�^�̃|�C���^�� 2���i�߂�
				pIdx += 2;
			}
		}

		// �C���f�b�N�X�o�b�t�@���A�����b�N����
		m_pIdxBuff->Unlock();
	}
}

//============================================================
//	�X�N���[���̃e�N�X�`�����W�̐ݒ菈��
//============================================================
void CObjectMeshField::SetScrollTex(const float fTexU, const float fTexV)
{
	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	if (USED(m_pVtxBuff))
	{ // �g�p���̏ꍇ

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntHeight = 0; nCntHeight < m_part.y + 1; nCntHeight++)
		{ // �c�̕����� +1��J��Ԃ�

			for (int nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
			{ // ���̕����� +1��J��Ԃ�

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2((float)nCntWidth + fTexU, (float)nCntHeight + fTexV);

				// ���_�f�[�^�̃|�C���^�� 1���i�߂�
				pVtx += 1;
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}
