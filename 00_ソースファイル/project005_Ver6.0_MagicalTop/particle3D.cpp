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

#define EXP_FIRE_COL		(D3DXCOLOR(1.0f, 0.38f, 0.23f, 1.0f))	// �����̉��̐F
#define EXP_FIRE_POSGAP		(5.0f)	// �����̉��̈ʒu�����
#define EXP_FIRE_MOVE		(3.24f)	// �����̉��̈ړ���
#define EXP_FIRE_SPAWN		(52)	// �����̉��̐�����
#define EXP_FIRE_EFF_LIFE	(26)	// �����̉��̎���
#define EXP_FIRE_RAND_LIFE	(8)		// �����̉��̃����_���������Z�ʂ̍ő�l
#define EXP_FIRE_SIZE		(3.57f)	// �����̉��̑傫��
#define EXP_FIRE_SUB_SIZE	(-8.0f)	// �����̉��̔��a�̌��Z��

#define EXP_SMOKE_COL		(D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.0f))	// �����̉��̐F
#define EXP_SMOKE_POSGAP	(3.0f)	// �����̉��̈ʒu�����
#define EXP_SMOKE_MOVE		(1.2f)	// �����̉��̈ړ���
#define EXP_SMOKE_SPAWN		(66)	// �����̉��̐�����
#define EXP_SMOKE_EFF_LIFE	(56)	// �����̉��̎���
#define EXP_SMOKE_RAND_LIFE	(12)	// �����̉��̃����_���������Z�ʂ̍ő�l
#define EXP_SMOKE_SIZE		(70.0f)	// �����̉��̑傫��
#define EXP_SMOKE_SUB_SIZE	(-1.0f)	// �����̉��̔��a�̌��Z��

#define MUZZLEFLASH_COL			(D3DXCOLOR(1.0f, 0.38f, 0.23f, 1.0f))	// �}�Y���t���b�V���̐F
#define MUZZLEFLASH_MOVE		(0.64f)		// �}�Y���t���b�V���̈ړ���
#define MUZZLEFLASH_SPAWN		(48)		// �}�Y���t���b�V���̐�����
#define MUZZLEFLASH_EFF_LIFE	(8)			// �}�Y���t���b�V���̎���
#define MUZZLEFLASH_SIZE		(0.57f)		// �}�Y���t���b�V���̑傫��
#define MUZZLEFLASH_SUB_SIZE	(-1.42f)	// �}�Y���t���b�V���̔��a�̌��Z��

#define TANKFIRE_COL		(D3DXCOLOR(1.0f, 0.38f, 0.23f, 1.0f))	// �^���N�t�@�C�A�̐F
#define TANKFIRE_MOVE		(2.64f)		// �^���N�t�@�C�A�̈ړ���
#define TANKFIRE_SPAWN		(58)		// �^���N�t�@�C�A�̐�����
#define TANKFIRE_EFF_LIFE	(22)		// �^���N�t�@�C�A�̎���
#define TANKFIRE_SIZE		(2.57f)		// �^���N�t�@�C�A�̑傫��
#define TANKFIRE_SUB_SIZE	(-3.42f)	// �^���N�t�@�C�A�̔��a�̌��Z��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const int CParticle3D::mc_aLife[CParticle3D::TYPE_MAX] =	// �����萔
{
	0,	// �Ȃ�
	24,	// �_���[�W
	1,	// ����
	1,	// �}�Y���t���b�V��
	1,	// �^���N�t�@�C�A
	24,	// �o�u������
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
	m_pos	= VEC3_ZERO;	// �ʒu
	m_col	= XCOL_WHITE;	// �F
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
	m_pos	= VEC3_ZERO;	// �ʒu
	m_col	= XCOL_WHITE;	// �F
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

		// �_���[�W
		Damage(m_pos, m_col);

		break;

	case TYPE_EXPLOSION:

		// ����
		Explosion(m_pos);

		break;

	case TYPE_MUZZLE_FLASH:

		// �}�Y���t���b�V��
		MuzzleFlash(m_pos);

		break;

	case TYPE_TANK_FIRE:

		// �^���N�t�@�C�A
		TankFire(m_pos);

		break;

	case TYPE_BUBBLE_EXPLOSION:

		// �o�u������
		BubbleExplosion(m_pos);

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
//	�_���[�W
//============================================================
void CParticle3D::Damage(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// �ϐ���錾
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p

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
			rot = VEC3_ZERO;

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
				true,				// ���Z������
				LABEL_PARTICLE3D	// �I�u�W�F�N�g���x��
			);
		}
	}
}

//============================================================
//	����
//============================================================
void CParticle3D::Explosion(const D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 vec  = VEC3_ZERO;	// �x�N�g���̐ݒ�p
	D3DXVECTOR3 pos  = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p
	int nLife = 0;	// �����̑���p

	for (int nCntPart = 0; nCntPart < EXP_SMOKE_SPAWN; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		vec.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&vec, &vec);

		// �ʒu��ݒ�
		pos = rPos + vec * EXP_SMOKE_POSGAP;

		// �ړ��ʂ�ݒ�
		move = vec * EXP_SMOKE_MOVE;

		// ������ݒ�
		rot.x = 0.0f;
		rot.y = 0.0f;
		rot.z = (float)(rand() % 629 - 314) / 100.0f;

		// ������ݒ�
		nLife = (rand() % EXP_SMOKE_RAND_LIFE) + EXP_SMOKE_EFF_LIFE;

		// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
		CEffect3D::Create
		( // ����
			CEffect3D::TYPE_SMOKE,	// �e�N�X�`��
			pos,					// �ʒu
			move,					// �ړ���
			rot,					// ����
			EXP_SMOKE_COL,			// �F
			nLife,					// ����
			EXP_SMOKE_SIZE,			// ���a
			EXP_SMOKE_SUB_SIZE,		// ���a�̌��Z��
			false,					// ���Z������
			LABEL_PARTICLE3D		// �I�u�W�F�N�g���x��
		);
	}

	for (int nCntPart = 0; nCntPart < EXP_FIRE_SPAWN; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		vec.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&vec, &vec);

		// �ʒu��ݒ�
		pos = rPos + vec * EXP_FIRE_POSGAP;

		// �ړ��ʂ�ݒ�
		move = vec * EXP_FIRE_MOVE;

		// ������ݒ�
		rot = VEC3_ZERO;

		// ������ݒ�
		nLife = (rand() % EXP_FIRE_RAND_LIFE) + EXP_FIRE_EFF_LIFE;

		// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
		CEffect3D::Create
		( // ����
			CEffect3D::TYPE_NORMAL,	// �e�N�X�`��
			pos,					// �ʒu
			move,					// �ړ���
			rot,					// ����
			EXP_FIRE_COL,			// �F
			nLife,					// ����
			EXP_FIRE_SIZE,			// ���a
			EXP_FIRE_SUB_SIZE,		// ���a�̌��Z��
			true,					// ���Z������
			LABEL_PARTICLE3D		// �I�u�W�F�N�g���x��
		);
	}
}

//============================================================
//	�}�Y���t���b�V��
//============================================================
void CParticle3D::MuzzleFlash(const D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p
	int nLife = 0;	// �����̑���p

	for (int nCntPart = 0; nCntPart < MUZZLEFLASH_SPAWN; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&move, &move);

		// �ړ��ʂ�ݒ�
		move.x *= MUZZLEFLASH_MOVE;
		move.y *= MUZZLEFLASH_MOVE;
		move.z *= MUZZLEFLASH_MOVE;

		// ������ݒ�
		rot = VEC3_ZERO;

		// ������ݒ�
		nLife = (rand() % 6) + MUZZLEFLASH_EFF_LIFE;

		// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
		CEffect3D::Create
		( // ����
			CEffect3D::TYPE_NORMAL,	// �e�N�X�`��
			rPos,					// �ʒu
			move,					// �ړ���
			rot,					// ����
			MUZZLEFLASH_COL,		// �F
			nLife,					// ����
			MUZZLEFLASH_SIZE,		// ���a
			MUZZLEFLASH_SUB_SIZE,	// ���a�̌��Z��
			true,					// ���Z������
			LABEL_PARTICLE3D		// �I�u�W�F�N�g���x��
		);
	}
}

//============================================================
//	�^���N�t�@�C�A
//============================================================
void CParticle3D::TankFire(const D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p
	int nLife = 0;	// �����̑���p

	for (int nCntPart = 0; nCntPart < TANKFIRE_SPAWN; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&move, &move);

		// �ړ��ʂ�ݒ�
		move.x *= TANKFIRE_MOVE;
		move.y *= TANKFIRE_MOVE;
		move.z *= TANKFIRE_MOVE;

		// ������ݒ�
		rot = VEC3_ZERO;

		// ������ݒ�
		nLife = (rand() % 6) + TANKFIRE_EFF_LIFE;

		// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
		CEffect3D::Create
		( // ����
			CEffect3D::TYPE_NORMAL,	// �e�N�X�`��
			rPos,					// �ʒu
			move,					// �ړ���
			rot,					// ����
			TANKFIRE_COL,			// �F
			nLife,					// ����
			TANKFIRE_SIZE,			// ���a
			TANKFIRE_SUB_SIZE,		// ���a�̌��Z��
			true,					// ���Z������
			LABEL_PARTICLE3D		// �I�u�W�F�N�g���x��
		);
	}
}

//============================================================
//	�o�u������
//============================================================
void CParticle3D::BubbleExplosion(const D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p

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
			rot = VEC3_ZERO;

			// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
			CEffect3D::Create
			( // ����
				CEffect3D::TYPE_BUBBLE,	// �e�N�X�`��
				rPos,				// �ʒu
				move,				// �ړ���
				rot,				// ����
				XCOL_WHITE,			// �F
				DMG_EFF_LIFE,		// ����
				DMG_SIZE,			// ���a
				DMG_SUB_SIZE,		// ���a�̌��Z��
				true,				// ���Z������
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
	else { assert(false); return NULL; }	// �m�ێ��s
}
