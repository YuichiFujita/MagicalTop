//============================================================
//
//	�I�u�W�F�N�g�r���{�[�h���� [objectBillboard.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectBillboard.h"
#include "manager.h"
#include "renderer.h"
#include "object3D.h"
#include "texture.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_VERTEX	(4)	// ���_��

//************************************************************
//	�q�N���X [CObjectBillboard] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectBillboard::CObjectBillboard()
{
	// �����o�ϐ����N���A
	m_pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));		// ���[���h�}�g���b�N�X
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_size	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �傫��
	m_col	= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �F
	m_fAngle  = 0.0f;	// �Ίp���̊p�x
	m_fLength = 0.0f;	// �Ίp���̒���
	m_nTextureID = 0;	// �e�N�X�`���C���f�b�N�X
}

//============================================================
//	�I�[�o�[���[�h�R���X�g���N�^
//============================================================
CObjectBillboard::CObjectBillboard(const CObject::LABEL label, const int nPriority) : CObject(label, nPriority)
{
	// �����o�ϐ����N���A
	m_pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));		// ���[���h�}�g���b�N�X
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_size	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �傫��
	m_col	= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �F
	m_fAngle  = 0.0f;	// �Ίp���̊p�x
	m_fLength = 0.0f;	// �Ίp���̒���
	m_nTextureID = 0;	// �e�N�X�`���C���f�b�N�X
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObjectBillboard::~CObjectBillboard()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectBillboard::Init(void)
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
	m_fAngle  = 0.0f;	// �Ίp���̊p�x
	m_fLength = 0.0f;	// �Ίp���̒���
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
void CObjectBillboard::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (USED(m_pVtxBuff))
	{ // ���_�o�b�t�@���g�p���̏ꍇ

		// �������J��
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �I�u�W�F�N�g�r���{�[�h��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObjectBillboard::Update(void)
{
	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�`�揈��
//============================================================
void CObjectBillboard::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;		// �r���[�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^
	CTexture *pTexture = CManager::GetTexture();						// �e�N�X�`���ւ̃|�C���^

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);	// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);	// �t�s������߂�
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

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

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�
}

//============================================================
//	��������
//============================================================
CObjectBillboard *CObjectBillboard::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const D3DXVECTOR3& rRot, const D3DXCOLOR& rCol)
{
	// �|�C���^��錾
	CObjectBillboard *pObjectBillboard = NULL;	// �I�u�W�F�N�g�r���{�[�h�����p

	if (UNUSED(pObjectBillboard))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pObjectBillboard = new CObjectBillboard;	// �I�u�W�F�N�g�r���{�[�h
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pObjectBillboard))
	{ // �m�ۂɐ������Ă���ꍇ

		if (SUCCEEDED(pObjectBillboard->GetResult()))
		{ // �I�u�W�F�N�g�̏��ݒ�ɐ��������ꍇ

			// �I�u�W�F�N�g�r���{�[�h�̏�����
			if (FAILED(pObjectBillboard->Init()))
			{ // �������Ɏ��s�����ꍇ

				// �������J��
				delete pObjectBillboard;
				pObjectBillboard = NULL;

				// ���s��Ԃ�
				return NULL;
			}

			// �ʒu��ݒ�
			pObjectBillboard->SetPosition(rPos);

			// ������ݒ�
			pObjectBillboard->SetRotation(rRot);

			// �傫����ݒ�
			pObjectBillboard->SetScaling(rSize);

			// �F��ݒ�
			pObjectBillboard->SetColor(rCol);

			// �m�ۂ����A�h���X��Ԃ�
			return pObjectBillboard;
		}
		else
		{ // �I�u�W�F�N�g�̏��ݒ�Ɏ��s�����ꍇ

			// �������J��
			delete pObjectBillboard;
			pObjectBillboard = NULL;

			// ���s��Ԃ�
			return NULL;
		}
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�e�N�X�`����������
//============================================================
void CObjectBillboard::BindTexture(const int nTextureID)
{
	// �e�N�X�`���C���f�b�N�X����
	m_nTextureID = nTextureID;
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CObjectBillboard::SetPosition(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObjectBillboard::SetRotation(const D3DXVECTOR3& rRot)
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
void CObjectBillboard::SetScaling(const D3DXVECTOR3& rSize)
{
	// �����̑傫������
	m_size = rSize;

	// �Ίp���̊p�x�����߂�
	m_fAngle = atan2f(m_size.x, m_size.y);

	// �Ίp���̒��������߂�
	m_fLength = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y) * 0.5f;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CObjectBillboard::SetColor(const D3DXCOLOR& rCol)
{
	// �����̐F����
	m_col = rCol;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObjectBillboard::GetPosition(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CObjectBillboard::GetRotation(void) const
{
	// ������Ԃ�
	return m_rot;
}

//============================================================
//	�傫���擾����
//============================================================
D3DXVECTOR3 CObjectBillboard::GetScaling(void) const
{
	// �傫����Ԃ�
	return m_size;
}

//============================================================
//	�F�擾����
//============================================================
D3DXCOLOR CObjectBillboard::GetColor(void) const
{
	// �F��Ԃ�
	return m_col;
}

//============================================================
//	���_���̐ݒ菈��
//============================================================
void CObjectBillboard::SetVtx(void)
{
	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos.x = 0.0f + sinf(m_rot.z + (D3DX_PI + m_fAngle)) * m_fLength;
	pVtx[0].pos.y = 0.0f - cosf(m_rot.z + (D3DX_PI + m_fAngle)) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = 0.0f + sinf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[1].pos.y = 0.0f - cosf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = 0.0f + sinf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.y = 0.0f - cosf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = 0.0f + sinf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.y = 0.0f - cosf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	// �@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
