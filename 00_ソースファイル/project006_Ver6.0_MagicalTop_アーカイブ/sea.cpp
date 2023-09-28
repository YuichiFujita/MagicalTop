//============================================================
//
//	�C���� [sea.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "sea.h"
#include "manager.h"
#include "renderer.h"
#include "scrollMeshField.h"
#include "texture.h"

//************************************************************
//	�}�N����`
//************************************************************
#define SEA_PRIO	(0)		// �C�̗D�揇��
#define ADD_ROT		(50)	// ���_���Ƃ̃J�[�u���Z��
#define ADD_UP		(40.0f)	// �g�̍ō��_

#define ADD_SINROT	(D3DXToRadian(0.5f))	// ���t���[���̔g�ł��������Z��

#define SEA_DOWN	(D3DXVECTOR2(-0.0f, 0.002f))	// �C(��)�̈ړ���
#define SEA_UP		(D3DXVECTOR2(-0.0f, 0.0001f))	// �C(��)�̈ړ���

#define SEA_ROT		(D3DXVECTOR3(0.0f, D3DXToRadian(45), 0.0f))	// ����
#define SEA_SIZE	(D3DXVECTOR2(24000.0f, 24000.0f))			// �傫��

#define SEA_COL		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	// �F
#define SEA_PART	(POSGRID2(128, 128))				// ������

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CSea::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\sea000.jpg",	// �C(��)
	"data\\TEXTURE\\sea000.png",	// �C(��)
};

//************************************************************
//	�q�N���X [CSea] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSea::CSea() : CObject(CObject::LABEL_SEA, SEA_PRIO)
{
	// �����o�ϐ����N���A
	memset(&m_apScrollMeshField[0], 0, sizeof(m_apScrollMeshField));	// �X�N���[�����b�V���t�B�[���h�̏��
	m_fSinRot = 0.0f;	// �g�ł�����
}

//============================================================
//	�f�X�g���N�^
//============================================================
CSea::~CSea()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSea::Init(void)
{
	// �ϐ���錾
	int nTextureID;	// �e�N�X�`���C���f�b�N�X

	// �ϐ��z���錾
	D3DXVECTOR2 aMoveTexU[TEXTURE_MAX] =	// �e�N�X�`���̉��ړ��ʂ̐ݒ�p
	{ // �����l
		SEA_UP,		// �C(��)�̈ړ���
		SEA_DOWN,	// �C(��)�̈ړ���
	};

	D3DXCOLOR aCol[TEXTURE_MAX] =	// �F�̐ݒ�p
	{ // �����l
		XCOL_WHITE,	// �C(��)�̐F
		SEA_COL,	// �C(��)�̐F
	};

	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// �����o�ϐ���������
	memset(&m_apScrollMeshField[0], 0, sizeof(m_apScrollMeshField));	// �X�N���[�����b�V���t�B�[���h�̏��
	m_fSinRot = 0.0f;	// �g�ł�����

	for (int nCntSea = 0; nCntSea < TEXTURE_MAX; nCntSea++)
	{ // �e�N�X�`���̍ő吔���J��Ԃ�

		// �X�N���[�����b�V���t�B�[���h�̐���
		m_apScrollMeshField[nCntSea] = CScrollMeshField::Create
		( // ����
			aMoveTexU[nCntSea].x,	// �����W�̈ړ���
			aMoveTexU[nCntSea].y,	// �c���W�̈ړ���
			VEC3_ZERO,		// �ʒu
			SEA_ROT,		// ����
			SEA_SIZE,		// �傫��
			aCol[nCntSea],	// �F
			SEA_PART,		// ������
			D3DCULL_CCW,	// �J�����O��
			false			// ���C�e�B���O��
		);
		if (UNUSED(m_apScrollMeshField[nCntSea]))
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����o�^
		nTextureID = pTexture->Regist(mc_apTextureFile[nCntSea]);

		// �e�N�X�`��������
		m_apScrollMeshField[nCntSea]->BindTexture(nTextureID);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CSea::Uninit(void)
{
	// �X�N���[�����b�V���t�B�[���h��j��
	for (int nCntSea = 0; nCntSea < TEXTURE_MAX; nCntSea++)
	{ // �e�N�X�`���̍ő吔���J��Ԃ�

		// �X�N���[�����b�V���t�B�[���h�̏I��
		m_apScrollMeshField[nCntSea]->Uninit();
	}

	// ���g�̃I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CSea::Update(void)
{
	// �ϐ���錾
	POSGRID2 part = m_apScrollMeshField[0]->GetPattern();	// ������
	D3DXVECTOR3 pos;	// ���_���W�擾�p
	int nLine;			// ���_�̍s
	float fSinRot;		// �T�C���J�[�u�Ɏg�p����p�x

	// ���b�V���̒��_���W�̍X�V
	for (int nCntVtx = 0; nCntVtx < (part.x + 1) * (part.y + 1); nCntVtx++)
	{ // �g�p���_�����J��Ԃ�

		for (int nCntSea = 0; nCntSea < TEXTURE_MAX; nCntSea++)
		{ // �e�N�X�`���̍ő吔���J��Ԃ�

			// ���_���W���擾
			pos = m_apScrollMeshField[nCntSea]->GetMeshVertexPosition(nCntVtx);

			// �g�ł��p�̊p�x�����߂�
			nLine = nCntVtx / (part.x + 1);							// ���_�̍s
			fSinRot = m_fSinRot + D3DXToRadian(nLine * ADD_ROT);	// �T�C���J�[�u�Ɏg�p����p�x

			// �T�C���J�[�u�p�̌�����␳
			useful::NormalizeRot(fSinRot);

			// ���_���W��ݒ�
			m_apScrollMeshField[nCntSea]->SetMeshVertexPosition(nCntVtx, D3DXVECTOR3(pos.x, (sinf(fSinRot) * ADD_UP) + ADD_UP, pos.z));
		}
	}

	// �g�ł����������Z
	m_fSinRot += ADD_SINROT;

	// ������␳
	useful::NormalizeRot(m_fSinRot);

	// �X�N���[���̍X�V
	for (int nCntSea = 0; nCntSea < TEXTURE_MAX; nCntSea++)
	{ // �e�N�X�`���̍ő吔���J��Ԃ�

		// �X�N���[�����b�V���t�B�[���h�̍X�V
		m_apScrollMeshField[nCntSea]->Update();
	}
}

//============================================================
//	�`�揈��
//============================================================
void CSea::Draw(void)
{

}

//============================================================
//	��������
//============================================================
CSea *CSea::Create(void)
{
	// �|�C���^��錾
	CSea *pSea = NULL;	// �C�����p

	if (UNUSED(pSea))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pSea = new CSea;	// �C
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pSea))
	{ // �m�ۂɐ������Ă���ꍇ

		// �C�̏�����
		if (FAILED(pSea->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pSea;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}
