//============================================================
//
//	�I�u�W�F�N�g�O�Տ��� [objectOrbit.cpp]
//	Author�F���c�E��
//
//============================================================
#if 1
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectOrbit.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_OFFSET	(2)	// �I�t�Z�b�g�̐�

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
	m_nNumAround = 0;	// �Q�̎���
	m_nPattern = 0;		// �Q�̕�����
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
	m_nNumAround = 0;	// �Q�̎���
	m_nPattern = 0;		// �Q�̕�����
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
	m_pVtxBuff = NULL;	// ���_�o�b�t�@
	m_nNumVtx = 0;		// �K�v���_��
	m_nNumAround = 0;	// �Q�̎���
	m_nPattern = 0;		// �Q�̕�����
	m_nTextureID = NONE_IDX;	// �e�N�X�`���C���f�b�N�X

	// �O�Ղ̏���������
	m_orbit.pos     = VEC3_ZERO;		// �ʒu
	m_orbit.rot     = VEC3_ZERO;		// ����
	m_orbit.direRot = VEC3_ZERO;		// ��������
	m_orbit.col     = XCOL_WHITE;		// �F
	m_orbit.pMtxParent  = NULL;		// �e�̃}�g���b�N�X
	m_orbit.fMoveRot    = 0.0f;		// �����̕ύX��
	m_orbit.fThickness  = 0.0f;		// �|���S���̑���
	m_orbit.fOuterPlus  = 0.0f;		// �|���S���O���� y���W���Z��
	m_orbit.fSetWidth   = 0.0f;		// �������̉������
	m_orbit.fSetAlpha   = 0.0f;		// �������̓����x
	m_orbit.fAddWidth   = 0.0f;		// ������̉��Z��
	m_orbit.fAddHeight  = 0.0f;		// �c����̉��Z��
	m_orbit.fSubAlpha   = 0.0f;		// �����x�̌��Z��
	m_orbit.fGrowWidth  = 0.0f;		// ������̐�����
	m_orbit.fGrowHeight = 0.0f;		// �c����̐�����
	m_orbit.fGrowAlpha  = 0.0f;		// �����x�̐�����
	m_orbit.cull   = D3DCULL_NONE;	// �J�����O��
	m_orbit.bLight = false;			// ���C�e�B���O��

	// �Q��ݒ�
	if (FAILED(SetVortex(1, 1)))
	{ // �Q�̐ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

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
	// �O�Ղ̌�����ύX
	m_orbit.rot.y -= m_orbit.fMoveRot;

	// �����𐳋K��
	useful::NormalizeRot(m_orbit.rot.y);

	// �O�Ղ𐬒�������
	m_orbit.fSetWidth  += m_orbit.fGrowWidth * m_nNumVtx;	// ���_����̉�����ʂ����Z
	m_orbit.fAddWidth  += m_orbit.fGrowWidth;	// �O�Ղ̉��̍L��������Z
	m_orbit.fAddHeight += m_orbit.fGrowHeight;	// �O�Ղ̏c�̍L��������Z
	m_orbit.fSetAlpha  -= m_orbit.fGrowAlpha;	// �O�Ղ̓����x�����Z (�����ɂ��Ă���)

	// ���_���̐ݒ�
	SetVtx();

	if (m_orbit.fSetAlpha <= 0.0f)
	{ // �O�Ղ������ɂȂ����ꍇ

		// �O�ՃI�u�W�F�N�g�̏I��
		Uninit();

		// �����𔲂���
		return;
	}
}

//============================================================
//	�`�揈��
//============================================================
void CObjectOrbit::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxOrigin;			// �������̃}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^
	CTexture *pTexture = CManager::GetTexture();						// �e�N�X�`���ւ̃|�C���^

	// �|���S���̗��ʂ݂̂�\����Ԃɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, m_orbit.cull);

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, m_orbit.bLight);

	// ���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// ���e�X�g�̗L�� / �����̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// ���e�X�g�̎Q�ƒl�ݒ�

	//--------------------------------------------------------
	//	�������̃}�g���b�N�X�����߂�
	//--------------------------------------------------------
	// �������̃}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxOrigin);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_orbit.direRot.y, m_orbit.direRot.x, m_orbit.direRot.z);
	D3DXMatrixMultiply(&mtxOrigin, &mtxOrigin, &mtxRot);	// ��������

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_orbit.pos.x, m_orbit.pos.y, m_orbit.pos.z);
	D3DXMatrixMultiply(&mtxOrigin, &mtxOrigin, &mtxTrans);	// �����ʒu

	//--------------------------------------------------------
	//	���[���h�}�g���b�N�X�����߂�
	//--------------------------------------------------------
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_orbit.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_orbit.rot.y, m_orbit.rot.x, m_orbit.rot.z);
	D3DXMatrixMultiply(&m_orbit.mtxWorld, &m_orbit.mtxWorld, &mtxRot);	// ��]��

	//--------------------------------------------------------
	//	�}�g���b�N�X���|�����킹��
	//--------------------------------------------------------
	if (USED(m_orbit.pMtxParent))	// TODO�F�e�}�g���b�N�X��NULL����Ȃ��Ȃ����Ƃ��ɔ��f�ł��Ȃ�
	{ // �e�̃}�g���b�N�X�����݂���ꍇ

		// �e�̃}�g���b�N�X�Ɗ|�����킹��
		D3DXMatrixMultiply(&mtxOrigin, &mtxOrigin, m_orbit.pMtxParent);
	}

	// �������̃}�g���b�N�X�Ɗ|�����킹��
	D3DXMatrixMultiply(&m_orbit.mtxWorld, &m_orbit.mtxWorld, &mtxOrigin);

	//--------------------------------------------------------
	//	�O�Ղ�`��
	//--------------------------------------------------------
	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_orbit.mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetTexture(m_nTextureID));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nNumVtx - 2);

	// �|���S���̕\�ʂ݂̂�\����Ԃɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, true);

	// ���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g�̗L�� / �����̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// ���e�X�g�̎Q�ƒl�ݒ�
}

//============================================================
//	��������
//============================================================
CObjectOrbit *CObjectOrbit::Create
(
	const D3DXVECTOR3& rPos,		// �ʒu
	const D3DXVECTOR3& rDireRot,	// ��������
	const D3DXCOLOR& rCol,			// �F
	D3DXMATRIX *pMtxParent,			// �e�̃}�g���b�N�X
	const int   nNumAround,			// �Q�̎���
	const int   nPattern,			// �Q�̕�����
	const float fMoveRot,			// �����̕ύX��
	const float fThickness,			// �|���S���̑���
	const float fOuterPlus,			// �|���S���O���� y���W���Z��
	const float fSetWidth,			// �������̉������
	const float fSetAlpha,			// �������̓����x
	const float fAddWidth,			// ������̉��Z��
	const float fAddHeight,			// �c����̉��Z��
	const float fSubAlpha,			// �����x�̌��Z��
	const float fGrowWidth,			// ������̐�����
	const float fGrowHeight,		// �c����̐�����
	const float fGrowAlpha,			// �����x�̐�����
	const D3DCULL cull,				// �J�����O��
	const bool bLight				// ���C�e�B���O��
)
{
	// �|�C���^��錾
	CObjectOrbit *pObjectOrbit = NULL;	// �I�u�W�F�N�g�O�Ր����p

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

		// �ʒu��ݒ�
		pObjectOrbit->SetPosition(rPos);

		// ������ݒ�
		pObjectOrbit->SetDirectionRotation(rDireRot);

		// �F��ݒ�
		pObjectOrbit->SetColor(rCol);

		// �e�̃}�g���b�N�X��ݒ�
		pObjectOrbit->SetMatrixParent(pMtxParent);

		// �|���S���̑�����ݒ�
		pObjectOrbit->SetThickness(fThickness);

		// �|���S���O����y���W���Z�ʂ�ݒ�
		pObjectOrbit->SetOuterPlus(fOuterPlus);

		// �������̉�����ʂ�ݒ�
		pObjectOrbit->SetWidth(fSetWidth);

		// �������̓����x��ݒ�
		pObjectOrbit->SetAlpha(fSetAlpha);

		// ������̉��Z�ʂ�ݒ�
		pObjectOrbit->SetAddWidth(fAddWidth);

		// �c����̉��Z�ʂ�ݒ�
		pObjectOrbit->SetAddHeight(fAddHeight);

		// �����x�̌��Z�ʂ�ݒ�
		pObjectOrbit->SetSubAlpha(fSubAlpha);

		// �J�����O��ݒ�
		pObjectOrbit->SetCulling(cull);

		// ���C�e�B���O��ݒ�
		pObjectOrbit->SetLighting(bLight);

		// ��������ݒ�
		pObjectOrbit->SetGrow(fMoveRot, fGrowWidth, fGrowHeight, fGrowAlpha);

		// �Q��ݒ�
		if (FAILED(pObjectOrbit->SetVortex(nNumAround, nPattern)))
		{ // �Q�̐ݒ�Ɏ��s�����ꍇ

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
//	�ʒu�̐ݒ菈��
//============================================================
void CObjectOrbit::SetPosition(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_orbit.pos = rPos;
}

//============================================================
//	���������̐ݒ菈��
//============================================================
void CObjectOrbit::SetDirectionRotation(const D3DXVECTOR3& rRot)
{
	// �����̐���������ݒ�
	m_orbit.direRot = rRot;

	// ���������̐��K��
	useful::NormalizeRot(m_orbit.direRot.x);
	useful::NormalizeRot(m_orbit.direRot.y);
	useful::NormalizeRot(m_orbit.direRot.z);
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CObjectOrbit::SetColor(const D3DXCOLOR& rCol)
{
	// �����̐F��ݒ�
	m_orbit.col = rCol;

	// ���_���̐ݒ�
	SetVtx();
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
//	�|���S���̑����̐ݒ菈��
//============================================================
void CObjectOrbit::SetThickness(const float fThickness)
{
	// �����̃|���S���̑�����ݒ�
	m_orbit.fThickness = fThickness;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�|���S���O����y���W���Z�ʂ̐ݒ菈��
//============================================================
void CObjectOrbit::SetOuterPlus(const float fOuterPlus)
{
	// �����̃|���S���O����y���W���Z�ʂ�ݒ�
	m_orbit.fOuterPlus = fOuterPlus;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�������̉�����ʂ̐ݒ菈��
//============================================================
void CObjectOrbit::SetWidth(const float fSetWidth)
{
	// �����̐������̉�����ʂ�ݒ�
	m_orbit.fSetWidth = fSetWidth;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�������̓����x�̐ݒ菈��
//============================================================
void CObjectOrbit::SetAlpha(const float fSetAlpha)
{
	// �����̐������̓����x��ݒ�
	m_orbit.fSetAlpha = fSetAlpha;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	������̉��Z�ʂ̐ݒ菈��
//============================================================
void CObjectOrbit::SetAddWidth(const float fAddWidth)
{
	// �����̉�����̉��Z�ʂ�ݒ�
	m_orbit.fAddWidth = fAddWidth;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�c����̉��Z�ʂ̐ݒ菈��
//============================================================
void CObjectOrbit::SetAddHeight(const float fAddHeight)
{
	// �����̏c����̉��Z�ʂ�ݒ�
	m_orbit.fAddHeight = fAddHeight;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�����x�̌��Z�ʂ̐ݒ菈��
//============================================================
void CObjectOrbit::SetSubAlpha(const float fSubAlpha)
{
	// �����̓����x�̌��Z�ʂ�ݒ�
	m_orbit.fSubAlpha = fSubAlpha;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�J�����O�ݒ菈��
//============================================================
void CObjectOrbit::SetCulling(const D3DCULL cull)
{
	// �����̃J�����O�󋵂�ݒ�
	m_orbit.cull = cull;
}

//============================================================
//	���C�e�B���O�ݒ菈��
//============================================================
void CObjectOrbit::SetLighting(const bool bLight)
{
	// �����̃��C�e�B���O�󋵂�ݒ�
	m_orbit.bLight = bLight;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObjectOrbit::SetGrow(float fMoveRot, float fGrowWidth, float fGrowHeight, float fGrowAlpha)
{
	// �����̏���ݒ�
	m_orbit.fMoveRot		= fMoveRot;		// �����̕ύX��
	m_orbit.fGrowWidth	= fGrowWidth;	// ������̐�����
	m_orbit.fGrowHeight	= fGrowHeight;	// �c����̐�����
	m_orbit.fGrowAlpha	= fGrowAlpha;	// �����x�̐�����
}

//============================================================
//	�Q�̐ݒ菈��
//============================================================
HRESULT CObjectOrbit::SetVortex(const int nNumAround, const int nPattern)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// �����̉Q��ݒ�
	m_nNumAround = nNumAround;
	m_nPattern = nPattern;

	// �K�v���_�������߂�
	m_nNumVtx = m_nNumAround * m_nPattern * MAX_OFFSET;

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
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObjectOrbit::GetPosition(void) const
{
	// �ʒu��Ԃ�
	return m_orbit.pos;
}

//============================================================
//	���������擾����
//============================================================
D3DXVECTOR3 CObjectOrbit::GetDirectionRotation(void) const
{
	// ����������Ԃ�
	return m_orbit.direRot;
}

//============================================================
//	�F�擾����
//============================================================
D3DXCOLOR CObjectOrbit::GetColor(void) const
{
	// �F��Ԃ�
	return m_orbit.col;
}

//============================================================
//	�|���S���̑����擾����
//============================================================
float CObjectOrbit::GetThickness(void) const
{
	// �|���S���̑�����Ԃ�
	return m_orbit.fThickness;
}

//============================================================
//	�|���S���O����y���W���Z�ʂ̎擾����
//============================================================
float CObjectOrbit::GetOuterPlus(void) const
{
	// �|���S���O����y���W���Z�ʂ�Ԃ�
	return m_orbit.fOuterPlus;
}

//============================================================
//	�������̉�����ʎ擾����
//============================================================
float CObjectOrbit::GetWidth(void) const
{
	// �������̉�����ʂ�Ԃ�
	return m_orbit.fSetWidth;
}

//============================================================
//	�������̓����x�擾����
//============================================================
float CObjectOrbit::GetAlpha(void) const
{
	// �������̓����x��Ԃ�
	return m_orbit.fSetAlpha;
}

//============================================================
//	������̉��Z�ʎ擾����
//============================================================
float CObjectOrbit::GetAddWidth(void) const
{
	// ������̉��Z�ʂ�Ԃ�
	return m_orbit.fAddWidth;
}

//============================================================
//	�c����̉��Z�ʎ擾����
//============================================================
float CObjectOrbit::GetAddHeight(void) const
{
	// �c����̉��Z�ʂ�Ԃ�
	return m_orbit.fAddHeight;
}

//============================================================
//	�����x�̌��Z�ʎ擾����
//============================================================
float CObjectOrbit::GetSubAlpha(void) const
{
	// �����x�̌��Z�ʂ�Ԃ�
	return m_orbit.fSubAlpha;
}

//============================================================
//	�J�����O�擾����
//============================================================
D3DCULL CObjectOrbit::GetCulling(void) const
{
	// �J�����O�󋵂�Ԃ�
	return m_orbit.cull;
}

//============================================================
//	���C�e�B���O�擾����
//============================================================
bool CObjectOrbit::GetLighting(void) const
{
	// ���C�e�B���O�󋵂�Ԃ�
	return m_orbit.bLight;
}

//============================================================
//	���_���̐ݒ菈��
//============================================================
void CObjectOrbit::SetVtx(void)
{
	// �ϐ���錾
	D3DXVECTOR3 vecPos;	// �O�Ղ̒��_�����x�N�g��
	float fWidth  = m_orbit.fSetWidth;	// ���_�����̉������
	float fHeight = 0.0f;					// ���_�����̏c�����
	float fAlpha  = m_orbit.fSetAlpha;	// ���_�J���[�̓����x

	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	if (USED(m_pVtxBuff))
	{ // �O�Ղ̒��_�o�b�t�@���g�p���̏ꍇ

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntWidth = 0; nCntWidth < m_nNumAround; nCntWidth++)
		{ // �Q�̎���̑������J��Ԃ�

			for (int nCntHeight = 0; nCntHeight < m_nPattern; nCntHeight++)
			{ // �Q�̕��������J��Ԃ�

				// ���_�����x�N�g�������߂�
				vecPos.x = sinf(nCntHeight * ((D3DX_PI * 2.0f) / m_nPattern)) * 1.0f;
				vecPos.y = 1.0f;
				vecPos.z = cosf(nCntHeight * ((D3DX_PI * 2.0f) / m_nPattern)) * 1.0f;

				for (int nCntSet = 0; nCntSet < MAX_OFFSET; nCntSet++)
				{ // �I�t�Z�b�g�̑������J��Ԃ�

					// ���_���W�̐ݒ�
					pVtx[0].pos.x = 0.0f + vecPos.x * (fWidth + (nCntSet * m_orbit.fThickness));	// x
					pVtx[0].pos.y = 0.0f + vecPos.y * (fHeight + (nCntSet * m_orbit.fOuterPlus));	// y
					pVtx[0].pos.z = 0.0f + vecPos.z * (fWidth + (nCntSet * m_orbit.fThickness));	// z

					// �@���x�N�g���̐ݒ�
					pVtx[0].nor = VEC3_ZERO;

					if (fAlpha < 0.0f)
					{ // �����x���͈͊O�̏ꍇ

						// �����x��␳
						fAlpha = 0.0f;
					}

					// ���_�J���[�̐ݒ�
					pVtx[0].col = D3DXCOLOR(m_orbit.col.r, m_orbit.col.g, m_orbit.col.b, fAlpha);

					// �e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2
					( // ����
						1.0f,			// u
						1.0f * nCntSet	// v
					);

					// ���_�f�[�^�̃|�C���^��i�߂�
					pVtx += 1;
				}

				// �c�����L����
				fWidth  += m_orbit.fAddWidth;
				fHeight += m_orbit.fAddHeight;

				// ���l�����Z
				fAlpha -= m_orbit.fSubAlpha;
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
	else { assert(false); }	// ��g�p��
}

#endif
