//============================================================
//
//	�e���� [shadow.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "field.h"

//************************************************************
//	�}�N����`
//************************************************************
#define SHADOW_PRIO	(2)		// �e�̗D�揇��

#define MAX_DIS_HEIGHT	(200.0f)	// �e�Ɛe�̏c�̋����̍ő�l
#define MAX_PLUS_SIZE	(120.0f)	// �e�̑傫�����Z�ʂ̍ő�l

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CShadow::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\shadow000.jpg",	// �ʏ�e�N�X�`��
};

//************************************************************
//	�q�N���X [CShadow] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CShadow::CShadow(const D3DXVECTOR3& rSize, const float fMinAlpha, const float fMaxAlpha) : CObject3D(CObject::LABEL_SHADOW, SHADOW_PRIO), m_sizeOrigin(rSize), m_fMinAlpha(fMinAlpha), m_fMaxAlpha(fMaxAlpha)
{
	// �����o�ϐ����N���A
	m_pParentObject = NULL;	// �e�I�u�W�F�N�g
}

//============================================================
//	�f�X�g���N�^
//============================================================
CShadow::~CShadow()
{

}

//============================================================
//	����������
//============================================================
HRESULT CShadow::Init(void)
{
	// �����o�ϐ���������
	m_pParentObject = NULL;	// �e�I�u�W�F�N�g

	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CObject3D::Init()))
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
void CShadow::Uninit(void)
{
	// �I�u�W�F�N�g3D�̏I��
	CObject3D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CShadow::Update(void)
{
	// TODO�F�e�I�u�W�F�N�g�̏I������ǂ����邩

	// �`�����ݒ�
	if (FAILED(SetDrawInfo()))
	{ // �`����̐ݒ�Ɏ��s�����ꍇ

		// �I�u�W�F�N�g�̏I��
		Uninit();

		// �֐��𔲂���
		return;
	}

	// �I�u�W�F�N�g3D�̍X�V
	CObject3D::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CShadow::Draw(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);	// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �I�u�W�F�N�g3D�̕`��
	CObject3D::Draw();

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
CShadow *CShadow::Create
(
	const TEXTURE texture,		// ���
	const D3DXVECTOR3& rSize,	// �傫��
	CObject *pObject,			// �e�I�u�W�F�N�g
	const float fMinAlpha,		// �����x�̍ŏ��l
	const float fMaxAlpha		// �����x�̍ő�l
)
{
	// �ϐ���錾
	int nTextureID;	// �e�N�X�`���C���f�b�N�X

	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^
	CShadow *pShadow = NULL;	// �e�����p

	if (UNUSED(pShadow))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pShadow = new CShadow(rSize, fMinAlpha, fMaxAlpha);	// �e
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pShadow))
	{ // �m�ۂɐ������Ă���ꍇ

		// �e�̏�����
		if (FAILED(pShadow->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return NULL;
		}

		// �e�N�X�`����o�^
		nTextureID = pTexture->Regist(mc_apTextureFile[texture]);

		// �e�N�X�`��������
		pShadow->BindTexture(nTextureID);

		// �e�I�u�W�F�N�g��ݒ�
		pShadow->SetParentObject(pObject);

		// �`�����ݒ�
		if (FAILED(pShadow->SetDrawInfo()))
		{ // �`����̐ݒ�Ɏ��s�����ꍇ

			// �������J��
			delete pShadow;
			pShadow = NULL;

			// ���s��Ԃ�
			assert(false);
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pShadow;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�`����̐ݒ菈��
//============================================================
HRESULT CShadow::SetDrawInfo(void)
{
	// TODO�F�|�C���^�̊m�F�����Ƃ��ꂢ��
	if (CObject::CheckUse(m_pParentObject))
	{ // �e�I�u�W�F�N�g���g�p����Ă����ꍇ

		// �ϐ���錾
		D3DXVECTOR3 posParent = m_pParentObject->GetPosition();	// �e�I�u�W�F�N�g�ʒu
		D3DXVECTOR3 posShadow = VEC3_ZERO;	// �e�ʒu
		D3DXVECTOR3 sizeShadow = VEC3_ZERO;	// �e�傫��
		float fDis = 0.0f;		// �e�Ɛe�̋���
		float fAlpha = 0.0f;	// �e�̓����x

		// �e�̈ʒu�����߂�
		posShadow = posParent;	// �e�I�u�W�F�N�g�̍��W���
		posShadow.y = CManager::GetField()->GetPositionHeight(posParent);	// ������n�ʂɐݒ�

		// �e�Ɛe�̏c���W�̋��������߂�
		fDis = fabsf(posParent.y - posShadow.y);		// �c�̋��������߂�
		useful::LimitNum(fDis, 0.0f, MAX_DIS_HEIGHT);	// �c�̋����𐧌�
		fDis *= 1.0f / MAX_DIS_HEIGHT;					// ������������

		// �e�̑傫�������߂�
		sizeShadow = D3DXVECTOR3(m_sizeOrigin.x + (MAX_PLUS_SIZE * fDis), 0.0f, m_sizeOrigin.z + (MAX_PLUS_SIZE * fDis));

		// ���l�����߂�
		fAlpha = fabsf(fDis - 1.0f);	// ���l��ݒ�
		useful::LimitNum(fAlpha, m_fMinAlpha, m_fMaxAlpha);	// ���l�𐧌�

		// �e�̕`�����ݒ�
		SetPosition(posShadow);	// �ʒu
		SetScaling(sizeShadow);	// �傫��
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha));	// �F

		// ������Ԃ�
		return S_OK;
	}
	else
	{ // �e�I�u�W�F�N�g���g�p����Ă��Ȃ������ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}
}

//============================================================
//	�e�I�u�W�F�N�g�̐ݒ菈��
//============================================================
void CShadow::SetParentObject(CObject *pObject)
{
	// �����̃I�u�W�F�N�g��ݒ�
	m_pParentObject = pObject;
}
