//============================================================
//
//	�I�u�W�F�N�g�O�Տ��� [objectOrbit.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectOrbit.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "sceneGame.h"
#include "pause.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CObjectOrbit::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\orbit000.jpg",	// �O�Ղ̃e�N�X�`��
};

const D3DXVECTOR3 CObjectOrbit::mc_aOffset[][MAX_OFFSET]	// �I�t�Z�b�g�̈ʒu������
{
	{ D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.0f, 100.0f, 0.0f) },	// �ʏ�I�t�Z�b�g
	{ D3DXVECTOR3(0.0f, 0.0f, -25.0f),	D3DXVECTOR3(0.0f, 0.0f, 25.0f) },	// ��I�t�Z�b�g
};

//************************************************************
//	�q�N���X [CObjectOrbit] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectOrbit::CObjectOrbit()
{
	// �����o�ϐ����N���A
	m_pVtxBuff = NULL;	// ���_�o�b�t�@
	m_nNumVtx = 0;		// �K�v���_��
	m_nTextureID = 0;	// �e�N�X�`���C���f�b�N�X
	memset(&m_orbit, 0, sizeof(m_orbit));	// �O�Ղ̏��
}

//============================================================
//	�I�[�o�[���[�h�R���X�g���N�^
//============================================================
CObjectOrbit::CObjectOrbit(const CObject::LABEL label, const int nPriority) : CObject(label, nPriority)
{
	// �����o�ϐ����N���A
	m_pVtxBuff = NULL;	// ���_�o�b�t�@
	m_nNumVtx = 0;		// �K�v���_��
	m_nTextureID = 0;	// �e�N�X�`���C���f�b�N�X
	memset(&m_orbit, 0, sizeof(m_orbit));	// �O�Ղ̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObjectOrbit::~CObjectOrbit()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectOrbit::Init(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// �����o�ϐ���������
	m_pVtxBuff = NULL;			// ���_�o�b�t�@
	m_nNumVtx = 0;				// �K�v���_��
	m_nTextureID = NONE_IDX;	// �e�N�X�`���C���f�b�N�X

	// �O�Ղ̏���������
	m_orbit.pMtxParent = NULL;	// �e�̃}�g���b�N�X
	m_orbit.bInit = false;		// ��������
	m_orbit.nTexPart = 1;		// �e�N�X�`��������

	for (int nCntOff = 0; nCntOff < MAX_OFFSET; nCntOff++)
	{ // �I�t�Z�b�g�̐����J��Ԃ�

		m_orbit.aOffset[nCntOff] = VEC3_ZERO;	// ���[�̃I�t�Z�b�g
		m_orbit.aCol[nCntOff] = XCOL_WHITE;		// ���[�̊�F
	}

	for (int nCntVtx = 0; nCntVtx < MAX_VERTEX; nCntVtx++)
	{ // �ێ����钸�_�̍ő吔���J��Ԃ�

		m_orbit.aPosPoint[nCntVtx] = VEC3_ZERO;		// �v�Z��̊e���_���W
		m_orbit.aColPoint[nCntVtx] = XCOL_WHITE;	// �e���_�J���[
	}

	// ������ݒ�
	if (FAILED(SetLength()))
	{ // �����̐ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �����`������Ȃ��ݒ�ɂ���
	SetEnableDraw(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CObjectOrbit::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (USED(m_pVtxBuff))
	{ // ���_�o�b�t�@���g�p���̏ꍇ

		// �������J��
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �I�u�W�F�N�g�O�Ղ�j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObjectOrbit::Update(void)
{

}

//============================================================
//	�`�揈��
//============================================================
void CObjectOrbit::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxIdent;	// �P�ʃ}�g���b�N�X�ݒ�p

	// �P�ʃ}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxIdent);

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^
	CTexture *pTexture = CManager::GetTexture();						// �e�N�X�`���ւ̃|�C���^
	
	//--------------------------------------------------------
	//	�����_�[�X�e�[�g��ύX
	//--------------------------------------------------------
	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �|���S���̗��ʂ�\����Ԃɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//--------------------------------------------------------
	//	�I�t�Z�b�g�̏�����
	//--------------------------------------------------------
	for (int nCntOff = 0; nCntOff < MAX_OFFSET; nCntOff++)
	{ // �I�t�Z�b�g�̐����J��Ԃ�

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_orbit.aMtxWorldPoint[nCntOff]);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&m_orbit.aMtxWorldPoint[nCntOff], m_orbit.aOffset[nCntOff].x, m_orbit.aOffset[nCntOff].y, m_orbit.aOffset[nCntOff].z);

		// �e�̃}�g���b�N�X�Ɗ|�����킹��
		D3DXMatrixMultiply(&m_orbit.aMtxWorldPoint[nCntOff], &m_orbit.aMtxWorldPoint[nCntOff], m_orbit.pMtxParent);
	}

	if (!CSceneGame::GetPause()->IsPause())
	{ // �|�[�Y���ł͂Ȃ��ꍇ

		//----------------------------------------------------
		//	���_���W�ƒ��_�J���[�̏������炷
		//----------------------------------------------------
		for (int nCntVtx = MAX_VERTEX - 1; nCntVtx >= MAX_OFFSET; nCntVtx--)
		{ // �ێ����钸�_�̍ő吔���J��Ԃ� (�I�t�Z�b�g���͊܂܂Ȃ�)

			// ���_�������炷
			m_orbit.aPosPoint[nCntVtx] = m_orbit.aPosPoint[nCntVtx - MAX_OFFSET];
			m_orbit.aColPoint[nCntVtx] = m_orbit.aColPoint[nCntVtx - MAX_OFFSET];
		}

		//----------------------------------------------------
		//	�ŐV�̒��_���W�ƒ��_�J���[�̏���ݒ�
		//----------------------------------------------------
		for (int nCntOff = 0; nCntOff < MAX_OFFSET; nCntOff++)
		{ // �I�t�Z�b�g�̐����J��Ԃ�

			// ���_���W�̐ݒ�
			m_orbit.aPosPoint[nCntOff] = D3DXVECTOR3
			( // ����
				m_orbit.aMtxWorldPoint[nCntOff]._41,	// x
				m_orbit.aMtxWorldPoint[nCntOff]._42,	// y
				m_orbit.aMtxWorldPoint[nCntOff]._43		// z
			);

			// ���_�J���[�̐ݒ�
			m_orbit.aColPoint[nCntOff] = m_orbit.aCol[nCntOff];
		}
	}

	//--------------------------------------------------------
	//	���_���W�ƒ��_�J���[�̏���������
	//--------------------------------------------------------
	if (m_orbit.bInit == false)
	{ // �������ς݂ł͂Ȃ��ꍇ

		for (int nCntVtx = 0; nCntVtx < MAX_VERTEX; nCntVtx++)
		{ // �ێ����钸�_�̍ő吔���J��Ԃ�

			// ���_���W�̐ݒ�
			m_orbit.aPosPoint[nCntVtx] = D3DXVECTOR3
			( // ����
				m_orbit.aMtxWorldPoint[nCntVtx % MAX_OFFSET]._41,	// x
				m_orbit.aMtxWorldPoint[nCntVtx % MAX_OFFSET]._42,	// y
				m_orbit.aMtxWorldPoint[nCntVtx % MAX_OFFSET]._43	// z
			);

			// ���_�J���[�̐ݒ�
			m_orbit.aColPoint[nCntVtx] = m_orbit.aCol[nCntVtx % MAX_OFFSET];
		}

		// �������ς݂ɂ���
		m_orbit.bInit = true;
	}

	//--------------------------------------------------------
	//	�|���S���̕`��
	//--------------------------------------------------------
	// ���_���̐ݒ�
	SetVtx();

	// �P�ʃ}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxIdent);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetTexture(m_nTextureID));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, MAX_VERTEX - 2);

	//--------------------------------------------------------
	//	�����_�[�X�e�[�g�����ɖ߂�
	//--------------------------------------------------------
	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �|���S���̕\�ʂ݂̂�\����Ԃɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//============================================================
//	��������
//============================================================
CObjectOrbit *CObjectOrbit::Create
(
	D3DXMATRIX *pMtxParent,	// �e�}�g���b�N�X
	const D3DXCOLOR& rCol,	// �F
	const OFFSET offset,	// �I�t�Z�b�g
	const TYPE type,		// ���
	const int nTexPart		// �e�N�X�`��������
)
{
	// �ϐ���錾
	int nTextureID;	// �e�N�X�`���C���f�b�N�X

	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^
	CObjectOrbit *pObjectOrbit = NULL;				// �I�u�W�F�N�g�O�Ր����p

	if (UNUSED(pObjectOrbit))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pObjectOrbit = new CObjectOrbit;	// �I�u�W�F�N�g�O��
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pObjectOrbit))
	{ // �m�ۂɐ������Ă���ꍇ

		// �I�u�W�F�N�g�O�Ղ̏�����
		if (FAILED(pObjectOrbit->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pObjectOrbit;
			pObjectOrbit = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �e�N�X�`����o�^
		nTextureID = pTexture->Regist(mc_apTextureFile[type]);

		// �e�N�X�`��������
		pObjectOrbit->BindTexture(nTextureID);

		// �e�̃}�g���b�N�X��ݒ�
		pObjectOrbit->SetMatrixParent(pMtxParent);

		// �F��ݒ�
		pObjectOrbit->SetColor(rCol);

		// �I�t�Z�b�g��ݒ�
		pObjectOrbit->SetOffset(offset);

		// �e�N�X�`����������ݒ�
		pObjectOrbit->SetTexPart(nTexPart);

		// ������ݒ�
		if (FAILED(pObjectOrbit->SetLength()))
		{ // �����̐ݒ�Ɏ��s�����ꍇ

			// �������J��
			delete pObjectOrbit;
			pObjectOrbit = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pObjectOrbit;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�e�N�X�`����������
//============================================================
void CObjectOrbit::BindTexture(const int nTextureID)
{
	// �e�N�X�`���C���f�b�N�X����
	m_nTextureID = nTextureID;
}

//============================================================
//	�e�̃}�g���b�N�X�̐ݒ菈��
//============================================================
void CObjectOrbit::SetMatrixParent(D3DXMATRIX *pMtxParent)
{
	// �����̐e�}�g���b�N�X��ݒ�
	m_orbit.pMtxParent = pMtxParent;
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CObjectOrbit::SetColor(const D3DXCOLOR& rCol)
{
	for (int nCntOff = 0; nCntOff < MAX_OFFSET; nCntOff++)
	{ // �I�t�Z�b�g�̐����J��Ԃ�

		// �����̐F��ݒ�
		m_orbit.aCol[nCntOff] = rCol;
	}
}

//============================================================
//	�I�t�Z�b�g�̐ݒ菈��
//============================================================
void CObjectOrbit::SetOffset(const OFFSET offset)
{
	for (int nCntOff = 0; nCntOff < MAX_OFFSET; nCntOff++)
	{ // �I�t�Z�b�g�̐����J��Ԃ�

		// �����̃I�t�Z�b�g��ݒ�
		m_orbit.aOffset[nCntOff] = mc_aOffset[(int)offset][nCntOff];
	}
}

//============================================================
//	�e�N�X�`���������̐ݒ菈��
//============================================================
void CObjectOrbit::SetTexPart(const int nTexPart)
{
	// ��O����
	assert(nTexPart > 0);	// 0�ȉ��G���[

	// �����̃e�N�X�`����������ݒ�
	m_orbit.nTexPart = nTexPart;
}

//============================================================
//	�������󋵂̐ݒ菈��
//============================================================
void CObjectOrbit::SetEnableInit(const bool bInit)
{
	// �����̏������󋵂�ݒ�
	m_orbit.bInit = bInit;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
HRESULT CObjectOrbit::SetLength(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// �K�v���_�������߂�
	m_nNumVtx = MAX_VERTEX;

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

	// ���_���̐ݒ�
	SetVtx();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	���_���̐ݒ菈��
//============================================================
void CObjectOrbit::SetVtx(void)
{
	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	if (USED(m_pVtxBuff))
	{ // �O�Ղ̒��_�o�b�t�@���g�p���̏ꍇ

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtx = 0; nCntVtx < MAX_VERTEX; nCntVtx++)
		{ // �ێ����钸�_�̍ő吔���J��Ԃ�

			// ���_���W�̐ݒ�
			pVtx[0].pos = m_orbit.aPosPoint[nCntVtx];

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR
			( // ����
				m_orbit.aColPoint[nCntVtx].r,	// r
				m_orbit.aColPoint[nCntVtx].g,	// g
				m_orbit.aColPoint[nCntVtx].b,	// b
				1.0f - (1.0f / (MAX_VERTEX * 0.5f)) * (nCntVtx / 2)	// a
			);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2
			( // ����
				(1.0f / (float)m_orbit.nTexPart) * (nCntVtx / 2),	// u
				1.0f * -((nCntVtx % 2) - 1)							// v
			);

			// ���_�f�[�^�̃|�C���^��i�߂�
			pVtx += 1;
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
	else { assert(false); }	// ��g�p��
}
