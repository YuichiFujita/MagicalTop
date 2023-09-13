//============================================================
//
//	�G�t�F�N�g3D���� [effect3D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "effect3D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�}�N����`
//************************************************************
#define EFF_PRIO	(4)	// �G�t�F�N�g3D�̗D�揇��
#define PART_PRIO	(4)	// �p�[�e�B�N��3D�̗D�揇��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CEffect3D::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\effect000.jpg",	// �ʏ�e�N�X�`��
};

//************************************************************
//	�q�N���X [CEffect3D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEffect3D::CEffect3D() : CObjectBillboard(CObject::LABEL_EFFECT3D), m_type(TYPE_NORMAL)
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
CEffect3D::CEffect3D(const TYPE type, const LABEL label) : CObjectBillboard(CObject::LABEL_EFFECT3D, (label == LABEL_EFFECT3D) ? EFF_PRIO : PART_PRIO), m_type(type)
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
CEffect3D::~CEffect3D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEffect3D::Init(void)
{
	// �����o�ϐ���������
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
	m_nLife		= 0;	// ����
	m_fSubSize	= 0.0f;	// �傫���̌��Z��
	m_fSubAlpha	= 0.0f;	// �����x�̌��Z��

	// �I�u�W�F�N�g�r���{�[�h�̏�����
	if (FAILED(CObjectBillboard::Init()))
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
void CEffect3D::Uninit(void)
{
	// �I�u�W�F�N�g�r���{�[�h�̏I��
	CObjectBillboard::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEffect3D::Update(void)
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
	CObjectBillboard::SetPosition(pos);

	// ������ݒ�
	CObjectBillboard::SetRotation(rot);

	// �傫����ݒ�
	CObjectBillboard::SetScaling(size);

	// �F��ݒ�
	CObjectBillboard::SetColor(col);

	// �I�u�W�F�N�g�r���{�[�h�̍X�V
	CObjectBillboard::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CEffect3D::Draw(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);	// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �I�u�W�F�N�g�r���{�[�h�̕`��
	CObjectBillboard::Draw();

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//============================================================
//	��������
//============================================================
CEffect3D *CEffect3D::Create
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
	CEffect3D *pEffect3D = NULL;	// �G�t�F�N�g3D�����p

	if (UNUSED(pEffect3D))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pEffect3D = new CEffect3D(type, label);	// �G�t�F�N�g3D
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pEffect3D))
	{ // �m�ۂɐ������Ă���ꍇ

		// �G�t�F�N�g3D�̏�����
		if (FAILED(pEffect3D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pEffect3D;
			pEffect3D = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �e�N�X�`����o�^
		nTextureID = pTexture->Regist(mc_apTextureFile[type]);

		// �e�N�X�`��������
		pEffect3D->BindTexture(nTextureID);

		// �ʒu��ݒ�
		pEffect3D->SetPosition(rPos);

		// ������ݒ�
		pEffect3D->SetRotation(rRot);

		// �傫����ݒ�
		pEffect3D->SetScaling(D3DXVECTOR3(fRadius, fRadius, 0.0f));

		// �F��ݒ�
		pEffect3D->SetColor(rCol);

		// �����̏���ݒ�
		pEffect3D->m_move		= rMove;		// �ړ���
		pEffect3D->m_nLife		= nLife;		// ����
		pEffect3D->m_fSubSize	= fSubSize;		// �傫���̌��Z��
		pEffect3D->m_fSubAlpha	= fSubAlpha;	// �����x�̌��Z��

		// �m�ۂ����A�h���X��Ԃ�
		return pEffect3D;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}
