//============================================================
//
//	�n�ʏ��� [field.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "field.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CField::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\field000.jpg",	// �ʏ�e�N�X�`��
};

//************************************************************
//	�q�N���X [CField] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CField::CField() : CObjectMeshField(CObject::LABEL_FIELD)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CField::~CField()
{

}

//============================================================
//	����������
//============================================================
HRESULT CField::Init(void)
{
	// �I�u�W�F�N�g���b�V���t�B�[���h�̏�����
	if (FAILED(CObjectMeshField::Init()))
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
void CField::Uninit(void)
{
	// �I�u�W�F�N�g���b�V���t�B�[���h�̏I��
	CObjectMeshField::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CField::Update(void)
{
	// �I�u�W�F�N�g���b�V���t�B�[���h�̍X�V
	CObjectMeshField::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CField::Draw(void)
{
	// �I�u�W�F�N�g���b�V���t�B�[���h�̕`��
	CObjectMeshField::Draw();
}

//============================================================
//	��������
//============================================================
CField *CField::Create
(
	const TEXTURE texture,		// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXVECTOR2& rSize,	// �傫��
	const D3DXCOLOR& rCol,		// �F
	const POSGRID2& rPart,		// ������
	const D3DCULL cull,			// �J�����O��
	const bool bLight			// ���C�e�B���O��
)
{
	// �ϐ���錾
	int nTextureID;	// �e�N�X�`���C���f�b�N�X

	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^
	CField *pField = NULL;	// �n�ʐ����p

	if (UNUSED(pField))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pField = new CField;	// �n��
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pField))
	{ // �m�ۂɐ������Ă���ꍇ

		if (SUCCEEDED(pField->GetResult()))
		{ // �I�u�W�F�N�g�̏��ݒ�ɐ��������ꍇ

			// �n�ʂ̏�����
			if (FAILED(pField->Init()))
			{ // �������Ɏ��s�����ꍇ

				// �������J��
				delete pField;
				pField = NULL;

				// ���s��Ԃ�
				return NULL;
			}

			// �e�N�X�`����o�^
			nTextureID = pTexture->Regist(mc_apTextureFile[texture]);

			// �e�N�X�`��������
			pField->BindTexture(nTextureID);

			// �ʒu��ݒ�
			pField->SetPosition(rPos);

			// ������ݒ�
			pField->SetRotation(rRot);

			// �傫����ݒ�
			pField->SetScalingBiaxial(rSize);

			// �F��ݒ�
			pField->SetColor(rCol);

			// �J�����O��ݒ�
			pField->SetCulling(cull);

			// ���C�e�B���O��ݒ�
			pField->SetLighting(bLight);

			// ��������ݒ�
			if (FAILED(pField->SetPattern(rPart)))
			{ // �������̐ݒ�Ɏ��s�����ꍇ

				// �������J��
				delete pField;
				pField = NULL;

				// ���s��Ԃ�
				return NULL;
			}

			// �m�ۂ����A�h���X��Ԃ�
			return pField;
		}
		else
		{ // �I�u�W�F�N�g�̏��ݒ�Ɏ��s�����ꍇ

			// �������J��
			delete pField;
			pField = NULL;

			// ���s��Ԃ�
			return NULL;
		}
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}
