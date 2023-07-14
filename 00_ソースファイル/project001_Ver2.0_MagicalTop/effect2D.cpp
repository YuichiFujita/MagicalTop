//============================================================
//
//	�G�t�F�N�g2D���� [effect2D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "effect2D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�}�N����`
//************************************************************
#define EFF_PRIO	(2)	// �G�t�F�N�g2D�̗D�揇��
#define PART_PRIO	(4)	// �p�[�e�B�N��2D�̗D�揇��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CEffect2D::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\effect000.jpg",	// �ʏ�e�N�X�`��
};

//************************************************************
//	�q�N���X [CEffect2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEffect2D::CEffect2D() : CObject2D(CObject::LABEL_EFFECT2D), m_type(TYPE_NORMAL)
{
	// �����o�ϐ����N���A
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
	m_nLife		= 0;	// ����
	m_fSubSize	= 0.0f;	// �傫���̌��Z��
	m_fSubAlpha	= 0.0f;	// �����x�̌��Z��
}

//============================================================
//	�I�[�o�[���[�h�R���X�g���N�^
//============================================================
CEffect2D::CEffect2D(const TYPE type, const LABEL label) : CObject2D(CObject::LABEL_EFFECT2D, (label == LABEL_EFFECT2D) ? EFF_PRIO : PART_PRIO), m_type(type)
{
	// �����o�ϐ����N���A
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
	m_nLife		= 0;	// ����
	m_fSubSize	= 0.0f;	// �傫���̌��Z��
	m_fSubAlpha	= 0.0f;	// �����x�̌��Z��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEffect2D::~CEffect2D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEffect2D::Init(void)
{
	// �����o�ϐ���������
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
	m_nLife		= 0;	// ����
	m_fSubSize	= 0.0f;	// �傫���̌��Z��
	m_fSubAlpha	= 0.0f;	// �����x�̌��Z��

	// �I�u�W�F�N�g2D�̏�����
	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

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
void CEffect2D::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I��
	CObject2D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEffect2D::Update(void)
{
	// �ϐ���錾
	D3DXVECTOR3 pos  = GetPosition();	// �ʒu
	D3DXVECTOR3 rot  = GetRotation();	// ����
	D3DXVECTOR3 size = GetScaling();	// �傫��
	D3DXCOLOR   col  = GetColor();		// �F

	if (m_nLife <= 0		// �������}����
	||  size.x  <= 0.0f		// �T�C�Y��0.0f�ȉ�
	||  size.y  <= 0.0f		// �T�C�Y��0.0f�ȉ�
	||  col.a   <= 0.0f)	// ���S�ɓ���
	{ // ��L�̂ǂꂩ�ɂȂ����ꍇ

		// �I�u�W�F�N�g��j��
		Uninit();

		// �֐��𔲂���
		return;
	}

	// �ړ��ʂ����Z
	pos += m_move;

	// ���������Z
	m_nLife--;

	// ���a�����Z
	size.x -= m_fSubSize;
	size.y -= m_fSubSize;

	// ���l�����Z
	col.a -= m_fSubAlpha;

	// �ʒu��ݒ�
	CObject2D::SetPosition(pos);

	// ������ݒ�
	CObject2D::SetRotation(rot);

	// �傫����ݒ�
	CObject2D::SetScaling(size);

	// �F��ݒ�
	CObject2D::SetColor(col);

	// �I�u�W�F�N�g2D�̍X�V
	CObject2D::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CEffect2D::Draw(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �I�u�W�F�N�g2D�̕`��
	CObject2D::Draw();

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//============================================================
//	��������
//============================================================
CEffect2D *CEffect2D::Create
(
	const TYPE type,			// �e�N�X�`��
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rMove,	// �ړ���
	const D3DXVECTOR3& rRot,	// ����
	const D3DXCOLOR& rCol,		// �F
	const int nLife,			// ����
	const float fRadius,		// ���a
	const float fSubSize,		// ���a�̌��Z��
	const float fSubAlpha,		// �����x�̌��Z��
	const LABEL label			// �I�u�W�F�N�g���x��
)
{
	// �ϐ���錾
	int nTextureID;	// �e�N�X�`���C���f�b�N�X

	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^
	CEffect2D *pEffect2D = NULL;	// �G�t�F�N�g2D�����p

	if (UNUSED(pEffect2D))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pEffect2D = new CEffect2D(type, label);	// �G�t�F�N�g2D
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pEffect2D))
	{ // �m�ۂɐ������Ă���ꍇ

		// �G�t�F�N�g2D�̏�����
		if (FAILED(pEffect2D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pEffect2D;
			pEffect2D = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �e�N�X�`����o�^
		nTextureID = pTexture->Regist(mc_apTextureFile[type]);

		// �e�N�X�`��������
		pEffect2D->BindTexture(nTextureID);

		// �ʒu��ݒ�
		pEffect2D->SetPosition(rPos);

		// ������ݒ�
		pEffect2D->SetRotation(rRot);

		// �傫����ݒ�
		pEffect2D->SetScaling(D3DXVECTOR3(fRadius, fRadius, 0.0f));

		// �F��ݒ�
		pEffect2D->SetColor(rCol);

		// �����̏���ݒ�
		pEffect2D->m_move		= rMove;		// �ړ���
		pEffect2D->m_nLife		= nLife;		// ����
		pEffect2D->m_fSubSize	= fSubSize;		// �傫���̌��Z��
		pEffect2D->m_fSubAlpha	= fSubAlpha;	// �����x�̌��Z��

		// �m�ۂ����A�h���X��Ԃ�
		return pEffect2D;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}
