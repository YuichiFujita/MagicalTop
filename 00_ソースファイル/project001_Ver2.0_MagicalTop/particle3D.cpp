//============================================================
//
//	�p�[�e�B�N��3D���� [particle3D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "particle3D.h"
#include "effect3D.h"

//************************************************************
//	�}�N����`
//************************************************************
#define DMG_MOVE		(5.0f)		// �_���[�W�̈ړ���
#define DMG_SPAWN		(35)		// �_���[�W�̐�����
#define DMG_EFF_LIFE	(120)		// �_���[�W�̎���
#define DMG_SIZE		(80.0f)		// �_���[�W�̑傫��
#define DMG_SUB_SIZE	(2.4f)		// �_���[�W�̔��a�̌��Z��
#define DMG_SUB_ALPHA	(0.005f)	// �_���[�W�̓����x�̌��Z��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const int CParticle3D::mc_aLife[CParticle3D::TYPE_MAX] =	// �����萔
{
	0,	// �Ȃ�
	24,	// �_���[�W
};

//************************************************************
//	�q�N���X [CParticle3D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CParticle3D::CParticle3D() : CObject(CObject::LABEL_PARTICLE3D)
{
	// �����o�ϐ����N���A
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_col	= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �F
	m_type	= TYPE_NONE;	// ���
	m_nLife	= 0;			// ����
}

//============================================================
//	�f�X�g���N�^
//============================================================
CParticle3D::~CParticle3D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CParticle3D::Init(void)
{
	// �����o�ϐ���������
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_col	= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �F
	m_type	= TYPE_NONE;	// ���
	m_nLife	= 0;			// ����

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	����������
//============================================================
HRESULT CParticle3D::Init(const TYPE type, const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// �����o�ϐ���������
	m_pos	= rPos;				// �ʒu
	m_col	= rCol;				// �F
	m_type	= type;				// ���
	m_nLife	= mc_aLife[type];	// ����

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CParticle3D::Uninit(void)
{
	// �p�[�e�B�N��3D�I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CParticle3D::Update(void)
{
	if (m_nLife > 0)
	{ // ����������ꍇ

		// ���������Z
		m_nLife--;
	}
	else
	{ // �������Ȃ��ꍇ

		// �p�[�e�B�N��3D�I�u�W�F�N�g�̏I��
		Uninit();

		// �֐��𔲂���
		return;
	}

	switch (m_type)
	{ // ��ނ��Ƃ̏���
	case TYPE_DAMAGE:

		// �_���[�W�p�[�e�B�N��3D
		Damage(m_pos, m_col);

		break;

	default:	// ��O����
		assert(false);
		break;
	}
}

//============================================================
//	�`�揈��
//============================================================
void CParticle3D::Draw(void)
{

}

//============================================================
//	�_���[�W�p�[�e�B�N��3D����
//============================================================
void CParticle3D::Damage(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// �ϐ���錾
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����̑���p

	if ((m_nLife + 1) % 12 == 0)
	{ // ������12�̔{���̏ꍇ

		for (int nCntPart = 0; nCntPart < DMG_SPAWN; nCntPart++)
		{ // ���������G�t�F�N�g�����J��Ԃ�

			// �x�N�g���������_���ɐݒ�
			move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
			move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
			move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

			// �x�N�g���𐳋K��
			D3DXVec3Normalize(&move, &move);

			// �ړ��ʂ�ݒ�
			move.x *= DMG_MOVE;
			move.y *= DMG_MOVE;
			move.z *= DMG_MOVE;

			// ������ݒ�
			rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
			CEffect3D::Create
			( // ����
				CEffect3D::TYPE_NORMAL,	// �e�N�X�`��
				rPos,	// �ʒu
				move,	// �ړ���
				rot,	// ����
				rCol,	// �F
				DMG_EFF_LIFE,		// ����
				DMG_SIZE,			// ���a
				DMG_SUB_SIZE,		// ���a�̌��Z��
				DMG_SUB_ALPHA,		// �����x�̌��Z��
				LABEL_PARTICLE3D	// �I�u�W�F�N�g���x��
			);
		}
	}
}

//============================================================
//	��������
//============================================================
CParticle3D *CParticle3D::Create(const TYPE type, const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// �|�C���^��錾
	CParticle3D *pParticle3D = NULL;	// �p�[�e�B�N��3D�����p

	if (UNUSED(pParticle3D))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pParticle3D = new CParticle3D;	// �p�[�e�B�N��3D
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pParticle3D))
	{ // �m�ۂɐ������Ă���ꍇ

		if (SUCCEEDED(pParticle3D->GetResult()))
		{ // �I�u�W�F�N�g�̏��ݒ�ɐ��������ꍇ

			// �p�[�e�B�N��3D�̏�����
			if (FAILED(pParticle3D->Init(type, rPos, rCol)))
			{ // �������Ɏ��s�����ꍇ

				// �������J��
				delete pParticle3D;
				pParticle3D = NULL;

				// ���s��Ԃ�
				return NULL;
			}

			// �m�ۂ����A�h���X��Ԃ�
			return pParticle3D;
		}
		else
		{ // �I�u�W�F�N�g�̏��ݒ�Ɏ��s�����ꍇ

			// �������J��
			delete pParticle3D;
			pParticle3D = NULL;

			// ���s��Ԃ�
			return NULL;
		}
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}
