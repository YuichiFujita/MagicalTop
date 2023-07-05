//============================================================
//
//	�I�u�W�F�N�g3D���� [object3D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object3D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_VERTEX	(4)	// ���_��

//************************************************************
//	�q�N���X [CObject3D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObject3D::CObject3D()
{
	// �����o�ϐ����N���A
	m_pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));		// ���[���h�}�g���b�N�X
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_size	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �傫��
	m_col	= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �F
	m_nTextureID = 0;	// �e�N�X�`���C���f�b�N�X
}

//============================================================
//	�I�[�o�[���[�h�R���X�g���N�^
//============================================================
CObject3D::CObject3D(const CObject::LABEL label, const int nPriority) : CObject(label, nPriority)
{
	// �����o�ϐ����N���A
	m_pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));		// ���[���h�}�g���b�N�X
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_size	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �傫��
	m_col	= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �F
	m_nTextureID = 0;	// �e�N�X�`���C���f�b�N�X
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObject3D::~CObject3D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObject3D::Init(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// �����o�ϐ���������
	m_pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));		// ���[���h�}�g���b�N�X
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_size	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �傫��
	m_col	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
	m_nTextureID = -1;	// �e�N�X�`���C���f�b�N�X

	if (UNUSED(m_pVtxBuff))
	{ // ��g�p���̏ꍇ

		// ���_�o�b�t�@�̐���
		if (FAILED(pDevice->CreateVertexBuffer
		( // ����
			sizeof(VERTEX_3D) * MAX_VERTEX,	// �K�v���_��
			D3DUSAGE_WRITEONLY,				// �g�p���@
			FVF_VERTEX_3D,					// ���_�t�H�[�}�b�g
			D3DPOOL_MANAGED,				// �������̎w��
			&m_pVtxBuff,					// ���_�o�b�t�@�ւ̃|�C���^
			NULL
		)))
		{ // ���_�o�b�t�@�̐����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}
	else { assert(false); return E_FAIL; }	// �g�p��

	// ���_���̐ݒ�
	SetVtx();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CObject3D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (USED(m_pVtxBuff))
	{ // ���_�o�b�t�@���g�p���̏ꍇ

		// �������J��
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �I�u�W�F�N�g3D��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObject3D::Update(void)
{

}

//============================================================
//	�`�揈��
//============================================================
void CObject3D::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^
	CTexture *pTexture = CManager::GetTexture();						// �e�N�X�`���ւ̃|�C���^

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

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetTexture(m_nTextureID));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//============================================================
//	��������
//============================================================
CObject3D *CObject3D::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const D3DXVECTOR3& rRot, const D3DXCOLOR& rCol)
{
	// �|�C���^��錾
	CObject3D *pObject3D = NULL;	// �I�u�W�F�N�g3D�����p

	if (UNUSED(pObject3D))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pObject3D = new CObject3D;	// �I�u�W�F�N�g3D
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pObject3D))
	{ // �m�ۂɐ������Ă���ꍇ

		if (SUCCEEDED(pObject3D->GetResult()))
		{ // �I�u�W�F�N�g�̏��ݒ�ɐ��������ꍇ

			// �I�u�W�F�N�g3D�̏�����
			if (FAILED(pObject3D->Init()))
			{ // �������Ɏ��s�����ꍇ

				// �������J��
				delete pObject3D;
				pObject3D = NULL;

				// ���s��Ԃ�
				return NULL;
			}

			// �ʒu��ݒ�
			pObject3D->SetPosition(rPos);

			// ������ݒ�
			pObject3D->SetRotation(rRot);

			// �傫����ݒ�
			pObject3D->SetScaling(rSize);

			// �F��ݒ�
			pObject3D->SetColor(rCol);

			// �m�ۂ����A�h���X��Ԃ�
			return pObject3D;
		}
		else
		{ // �I�u�W�F�N�g�̏��ݒ�Ɏ��s�����ꍇ

			// �������J��
			delete pObject3D;
			pObject3D = NULL;

			// ���s��Ԃ�
			return NULL;
		}
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�e�N�X�`����������
//============================================================
void CObject3D::BindTexture(const int nTextureID)
{
	// �e�N�X�`���C���f�b�N�X����
	m_nTextureID = nTextureID;
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CObject3D::SetPosition(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObject3D::SetRotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_rot = rRot;

	// �����̐��K��
	useful::NormalizeRot(m_rot.x);
	useful::NormalizeRot(m_rot.y);
	useful::NormalizeRot(m_rot.z);

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�傫���̐ݒ菈��
//============================================================
void CObject3D::SetScaling(const D3DXVECTOR3& rSize)
{
	// �����̑傫������
	m_size = rSize;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CObject3D::SetColor(const D3DXCOLOR& rCol)
{
	// �����̐F����
	m_col = rCol;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	���_�ʒu�̐ݒ菈��
//============================================================
void CObject3D::SetVertexPosition(const int nID, const D3DXVECTOR3& rPos)
{
	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	if (USED(m_pVtxBuff))
	{ // �g�p���̏ꍇ

		if (nID < MAX_VERTEX)
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

	// �@���̐ݒ�E���K��
	NormalizeNormal();
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObject3D::GetPosition(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CObject3D::GetRotation(void) const
{
	// ������Ԃ�
	return m_rot;
}

//============================================================
//	�傫���擾����
//============================================================
D3DXVECTOR3 CObject3D::GetScaling(void) const
{
	// �傫����Ԃ�
	return m_size;
}

//============================================================
//	�F�擾����
//============================================================
D3DXCOLOR CObject3D::GetColor(void) const
{
	// �F��Ԃ�
	return m_col;
}

//============================================================
//	���_�ʒu�擾����
//============================================================
D3DXVECTOR3 CObject3D::GetVertexPosition(const int nID)
{
	// �ϐ���錾
	D3DXVECTOR3 pos;	// ���_���W�̑���p

	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	if (USED(m_pVtxBuff))
	{ // �g�p���̏ꍇ

		if (nID < MAX_VERTEX)
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
//	�|���S���̒��n�擾����
//============================================================
float CObject3D::GetPositionHeight(const D3DXVECTOR3&rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 nor;	// �@���x�N�g��

	// �ϐ��z���錾
	D3DXVECTOR3 aVtxPos[MAX_VERTEX];	// �|���S���̒��_���W [��] 0�F���� 1�F�E�� 2�F���� 3�F�E��

	for (int nCnt = 0; nCnt < MAX_VERTEX; nCnt++)
	{ // ���_�����J��Ԃ�

		// �ϐ���錾
		D3DXMATRIX mtxWorld, mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X
		D3DXVECTOR3 pos = GetVertexPosition(nCnt);	// ���_���W
		
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxWorld);

		// ���_�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		// �|���S�������𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// �|���S���ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		// �v�Z�����}�g���b�N�X������W��ݒ�
		aVtxPos[nCnt] = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
	}

	if (collision::LineOuterProduct(aVtxPos[0], aVtxPos[1], rPos) > 0
	&&  collision::LineOuterProduct(aVtxPos[1], aVtxPos[2], rPos) > 0
	&&  collision::LineOuterProduct(aVtxPos[2], aVtxPos[0], rPos) > 0)
	{ // �|���S���͈͓̔��ɂ���ꍇ

		// �@�������߂�
		useful::NormalizeNormal(aVtxPos[1], aVtxPos[0], aVtxPos[2], nor);

		if (nor.y != 0.0f)
		{ // �@�����ݒ肳��Ă���ꍇ

			// �v���C���[�̒��n�_��Ԃ�
			return (((rPos.x - aVtxPos[0].x) * nor.x + (-aVtxPos[0].y) * nor.y + (rPos.z - aVtxPos[0].z) * nor.z) * -1.0f) / nor.y;
		}
	}

	if (collision::LineOuterProduct(aVtxPos[3], aVtxPos[1], rPos) < 0
	&&  collision::LineOuterProduct(aVtxPos[1], aVtxPos[2], rPos) < 0
	&&  collision::LineOuterProduct(aVtxPos[2], aVtxPos[3], rPos) < 0)
	{ // �|���S���͈͓̔��ɂ���ꍇ

		// �@�������߂�
		useful::NormalizeNormal(aVtxPos[2], aVtxPos[3], aVtxPos[1], nor);

		if (nor.y != 0.0f)
		{ // �@�����ݒ肳��Ă���ꍇ

			// �v���C���[�̒��n�_��Ԃ�
			return (((rPos.x - aVtxPos[3].x) * nor.x + (-aVtxPos[3].y) * nor.y + (rPos.z - aVtxPos[3].z) * nor.z) * -1.0f) / nor.y;
		}
	}

	// ���n�͈͊O�̏ꍇ���݂�y���W��Ԃ�
	return rPos.y;
}

//============================================================
//	���_���̐ݒ菈��
//============================================================
void CObject3D::SetVtx(void)
{
	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_size.x * 0.5f,  m_size.y * 0.5f,  m_size.z * 0.5f);
	pVtx[1].pos = D3DXVECTOR3( m_size.x * 0.5f,  m_size.y * 0.5f,  m_size.z * 0.5f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x * 0.5f, -m_size.y * 0.5f, -m_size.z * 0.5f);
	pVtx[3].pos = D3DXVECTOR3( m_size.x * 0.5f, -m_size.y * 0.5f, -m_size.z * 0.5f);

	// �@���̐ݒ�E���K��
	NormalizeNormal();

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//============================================================
//	�@���̐��K������
//============================================================
void CObject3D::NormalizeNormal(void)
{
	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �@���̐��K��
	useful::NormalizeNormal
	( // ����
		pVtx[1].pos,	// ���ʒu
		pVtx[0].pos,	// ���S�ʒu
		pVtx[2].pos,	// �E�ʒu
		pVtx[0].nor		// �@��
	);

	// �@���̐��K��
	useful::NormalizeNormal
	( // ����
		pVtx[2].pos,	// ���ʒu
		pVtx[3].pos,	// ���S�ʒu
		pVtx[1].pos,	// �E�ʒu
		pVtx[3].nor		// �@��
	);

	// �@���x�N�g���̐ݒ�
	pVtx[1].nor = (pVtx[0].nor + pVtx[3].nor) / 2;
	pVtx[2].nor = (pVtx[0].nor + pVtx[3].nor) / 2;

	// �@���𐳋K��
	D3DXVec3Normalize(&pVtx[1].nor, &pVtx[1].nor);
	D3DXVec3Normalize(&pVtx[2].nor, &pVtx[2].nor);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}
