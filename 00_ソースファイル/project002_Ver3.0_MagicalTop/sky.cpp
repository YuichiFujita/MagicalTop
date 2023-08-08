//============================================================
//
//	�󏈗� [sky.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "sky.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�}�N����`
//************************************************************
#define SKY_ROTMOVE	(0.0005f)	// �����ύX��
#define SKY_PRIO	(1)			// ��̗D�揇��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CSky::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\sky000.png",	// �ʏ�e�N�X�`��
};

//************************************************************
//	�q�N���X [CSky] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSky::CSky() : CObjectMeshDome(CObject::LABEL_SKY, SKY_PRIO)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CSky::~CSky()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSky::Init(void)
{
	// �I�u�W�F�N�g���b�V���h�[���̏�����
	if (FAILED(CObjectMeshDome::Init()))
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
void CSky::Uninit(void)
{
	// �I�u�W�F�N�g���b�V���h�[���̏I��
	CObjectMeshDome::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CSky::Update(void)
{
	// �ϐ���錾
	D3DXVECTOR3 rot = GetRotation();	// ����

	// ������ύX
	rot.y -= SKY_ROTMOVE;

	// ������ݒ�
	SetRotation(rot);

	// �I�u�W�F�N�g���b�V���h�[���̍X�V
	CObjectMeshDome::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CSky::Draw(void)
{
	// �I�u�W�F�N�g���b�V���h�[���̕`��
	CObjectMeshDome::Draw();
}

//============================================================
//	��������
//============================================================
CSky *CSky::Create
(
	const TEXTURE texture,		// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXCOLOR& rCol,		// �F
	const POSGRID2& rPart,		// ������
	const float fRadius,		// ���a
	const D3DCULL cull,			// �J�����O��
	const bool bLight			// ���C�e�B���O��
)
{
	// �ϐ���錾
	int nTextureID;	// �e�N�X�`���C���f�b�N�X

	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^
	CSky *pSky = NULL;	// �󐶐��p

	if (UNUSED(pSky))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pSky = new CSky;	// ��
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pSky))
	{ // �m�ۂɐ������Ă���ꍇ

		// ��̏�����
		if (FAILED(pSky->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pSky;
			pSky = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �e�N�X�`����o�^
		nTextureID = pTexture->Regist(mc_apTextureFile[texture]);

		// �e�N�X�`��������
		pSky->BindTexture(nTextureID);

		// �ʒu��ݒ�
		pSky->SetPosition(rPos);

		// ������ݒ�
		pSky->SetRotation(rRot);

		// �F��ݒ�
		pSky->SetColor(rCol);

		// ���a��ݒ�
		pSky->SetRadius(fRadius);

		// �J�����O��ݒ�
		pSky->SetCulling(cull);

		// ���C�e�B���O��ݒ�
		pSky->SetLighting(bLight);

		// ��������ݒ�
		if (FAILED(pSky->SetPattern(rPart)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// �������J��
			delete pSky;
			pSky = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �e�N�X�`���������̐ݒ�
		pSky->SetTexPattern(POSGRID2(rPart.x, rPart.y - 1));

		// �m�ۂ����A�h���X��Ԃ�
		return pSky;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}
