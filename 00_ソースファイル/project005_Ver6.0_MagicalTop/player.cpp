//============================================================
//
//	�v���C���[���� [player.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "player.h"
#include "playerTutorial.h"
#include "playerGame.h"
#include "manager.h"
#include "scene.h"
#include "sceneGame.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "collision.h"

#include "multiModel.h"
#include "magicManager.h"
#include "expManager.h"
#include "objectGauge3D.h"
#include "gaugeStar.h"
#include "shadow.h"
#include "objectOrbit.h"
#include "enemy.h"
#include "target.h"
#include "stage.h"
#include "field.h"
#include "particle3D.h"

//************************************************************
//	�}�N����`
//************************************************************
#define PLAYER_SETUP_TXT	"data\\TXT\\player.txt"				// �Z�b�g�A�b�v�e�L�X�g���΃p�X
#define PLAY_SHADOW_SIZE	(D3DXVECTOR3(80.0f, 0.0f, 80.0f))	// �e�̑傫��
#define PLAY_ORBIT_COL		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f))	// �O�Ղ̐F
#define PLAY_ORBIT_PART		(12)	// �O�Ղ̕�����
#define PLAY_ORBIT_PRIO		(4)		// �O�Ղ̗D�揇��

#define RESPAWN_ROT	(D3DXVECTOR3(0.0f, -HALF_PI, 0.0f))	// �ďo�����̌���

#define GAUGE_PLUS_Y	(140.0f)	// �Q�[�WY�ʒu���Z��
#define GAUGE_GAUGESIZE	(D3DXVECTOR3(110.0f, 15.0f, 0.0f))	// �Q�[�W�傫��
#define GAUGE_FRONTCOL	(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f))	// �\�Q�[�W�F
#define GAUGE_BACKCOL	(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	// ���Q�[�W�F

#define STAR_ADDPOS		(D3DXVECTOR3(0.0f, 50.0f, 0.0f))	// �\���ʒu�̉��Z��
#define STAR_RADIUS		(40.0f)	// ���a
#define STAR_HEAL		(4)		// �Q�[�W�񕜗�
#define STAR_WAIT_HEAL	(120)	// �񕜑ҋ@�J�E���g
#define STAR_DIS_CENTER	(65.0f)	// ���S����̋���
#define STAR_FLICKER	(8.0f)	// �h��߂���

#define VORTEX_ADDROT	(0.085f)	// �Q�������܂ꎞ�̃v���C���[��]���x
#define VORTEX_ADDDIS	(2.5f)		// �Q�������܂ꎞ�̃v���C���[�ƉQ�̋��������
#define VORTEX_ADDPOSY	(6.5f)		// �Q�������܂ꎞ�̍������W���Z��
#define VORTEX_HITDMG	(50)		// �Q�������܂ꎞ�̃_���[�W��

#define PLAY_AWAY_REV	(0.08f)		// �󒆂̃v���C���[�ړ��ʂ̌����W��
#define PLAY_LAND_REV	(0.2f)		// �n��̃v���C���[�ړ��ʂ̌����W��
#define PLAY_REV_ROTA	(0.15f)		// �v���C���[�����ύX�̌����W��
#define PLAY_GRAVITY	(1.0f)		// �v���C���[�d��
#define PLAY_RADIUS		(20.0f)		// �v���C���[���a
#define ENE_HIT_DMG		(30)		// �G�q�b�g���̃_���[�W��

#define FADE_LEVEL		(0.01f)		// �t�F�[�h�̃��l�̉�����
#define AWAY_SIDE_MOVE	(25.0f)		// ������ю��̉��ړ���
#define AWAY_UP_MOVE	(18.0f)		// ������ю��̏�ړ���
#define INVULN_CNT		(16)		// ���G��ԂɈڍs����܂ł̃J�E���^�[
#define NORMAL_CNT		(180)		// �ʏ��ԂɈڍs����܂ł̃J�E���^�[

#define CHANGE_ALPHA_CNT	(15)	// �����x�ύX�J�E���g
#define INVULN_MIN_ALPHA	(0.55f)	// �ŏ������x
#define INVULN_MAX_ALPHA	(1.0f)	// �ő哧���x

#define ALWAYS_ADDROT		(0.3f)	// �펞�����Ɍ��������
#define PLUS_INSIDE_MOVE	(3.0f)	// �^�[�Q�b�g����̋����ɉ������O���ւ̈ړ����Z��
#define PLUS_OUTSIDE_MOVE	(1.5f)	// �^�[�Q�b�g����̋����ɉ����������ւ̈ړ����Z��
#define INSIDE_ADDROT		(0.8f)	// �O���ړ����̌����ϓ���
#define OUTSIDE_SUBROT		(0.5f)	// �����ړ����̌����ϓ���

#define MOVE_INSIDE			(0.55f)	// �����ւ̈ړ���
#define MOVE_LEFT			(0.55f)	// �����ւ̈ړ���
#define MOVE_LEFT_DECELE	(0.4f)	// ���ړ��̌�����

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CPlayer::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\orbit000.jpg",	// �O�Ղ̃e�N�X�`��
};

const char *CPlayer::mc_apModelFile[] =	// ���f���萔
{
	"data\\MODEL\\PLAYER\\00_waist.x",	// ��
	"data\\MODEL\\PLAYER\\01_body.x",	// ��
	"data\\MODEL\\PLAYER\\02_cloak.x",	// �}���g
	"data\\MODEL\\PLAYER\\03_head.x",	// ��
	"data\\MODEL\\PLAYER\\04_hat.x",	// �X�q
	"data\\MODEL\\PLAYER\\05_armUL.x",	// ����r
	"data\\MODEL\\PLAYER\\06_armUR.x",	// �E��r
	"data\\MODEL\\PLAYER\\07_armDL.x",	// �����r
	"data\\MODEL\\PLAYER\\08_armDR.x",	// �E���r
	"data\\MODEL\\PLAYER\\09_handL.x",	// ����
	"data\\MODEL\\PLAYER\\10_handR.x",	// �E��
	"data\\MODEL\\PLAYER\\11_legUL.x",	// ��������
	"data\\MODEL\\PLAYER\\12_legUR.x",	// �E������
	"data\\MODEL\\PLAYER\\13_legDL.x",	// ����
	"data\\MODEL\\PLAYER\\14_legDR.x",	// �E��
	"data\\MODEL\\PLAYER\\15_footL.x",	// ����
	"data\\MODEL\\PLAYER\\16_footR.x",	// �E��
	"data\\MODEL\\PLAYER\\17_rod.x",	// ��
};

CPlayer::StatusInfo CPlayer::m_aStatusInfo[LEVEL_MAX] = {};	// �X�e�[�^�X���

//************************************************************
//	�q�N���X [CPlayer] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPlayer::CPlayer() : CObjectChara(CObject::LABEL_PLAYER)
{
	// �����o�ϐ����N���A
	memset(&m_level, 0, sizeof(m_level));	// ���x��
	m_pMagic		= NULL;			// ���@�}�l�[�W���[�̏��
	m_pExp			= NULL;			// �o���l�}�l�[�W���[�̏��
	m_pLife			= NULL;			// �̗͂̏��
	m_pDash			= NULL;			// �_�b�V���̏��
	m_pShadow		= NULL;			// �e�̏��
	m_pOrbit		= NULL;			// �O�Ղ̏��
	m_oldPos		= VEC3_ZERO;	// �ߋ��ʒu
	m_move			= VEC3_ZERO;	// �ړ���
	m_destRot		= VEC3_ZERO;	// �ڕW����
	m_state			= STATE_NONE;	// ���
	m_nCounterState	= 0;			// ��ԊǗ��J�E���^�[
	m_nNumModel		= 0;			// �p�[�c�̑���
	m_fDisTarget	= 0.0f;			// �^�[�Q�b�g�Ƃ̋���
	m_fVortexRot	= 0.0f;			// �Q��������
	m_fVortexDis	= 0.0f;			// �Q���Ƃ̋���
	m_bJump			= false;		// �W�����v��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CPlayer::~CPlayer()
{

}

//============================================================
//	����������
//============================================================
HRESULT CPlayer::Init(void)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// �����o�ϐ���������
	m_pMagic		= NULL;			// ���@�}�l�[�W���[�̏��
	m_pExp			= NULL;			// �o���l�}�l�[�W���[�̏��
	m_pLife			= NULL;			// �̗͂̏��
	m_pDash			= NULL;			// �_�b�V���̏��
	m_pShadow		= NULL;			// �e�̏��
	m_pOrbit		= NULL;			// �O�Ղ̏��
	m_oldPos		= VEC3_ZERO;	// �ߋ��ʒu
	m_move			= VEC3_ZERO;	// �ړ���
	m_destRot		= VEC3_ZERO;	// �ڕW����
	m_state			= STATE_NONE;	// ���
	m_nCounterState	= 0;			// ��ԊǗ��J�E���^�[
	m_nNumModel		= 0;			// �p�[�c�̑���
	m_fDisTarget	= 0.0f;			// �^�[�Q�b�g�Ƃ̋���
	m_fVortexRot	= 0.0f;			// �Q��������
	m_fVortexDis	= 0.0f;			// �Q���Ƃ̋���
	m_bJump			= true;			// �W�����v��

	// ���x������������
	m_level.nLife		= LEVEL_00;	// �̗�
	m_level.nDefense	= LEVEL_00;	// �h���
	m_level.nDash		= LEVEL_00;	// �_�b�V��
	m_level.nDefense	= LEVEL_00;	// �h���
	m_level.nSpeed		= LEVEL_00;	// �f����
	m_level.nMulExp		= LEVEL_00;	// �o���l�{�[�i�X

	// �I�u�W�F�N�g�L�����N�^�[�̏�����
	if (FAILED(CObjectChara::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �Z�b�g�A�b�v�̓ǂݍ���
	LoadSetup();

	// ���f�����̐ݒ�
	SetModelInfo();

	// ���@�}�l�[�W���[�̐���
	m_pMagic = CMagicManager::Create(this, m_aStatusInfo[m_level.nMana].nMana);
	if (UNUSED(m_pMagic))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �o���l�}�l�[�W���[�̐���
	m_pExp = CExpManager::Create();
	if (UNUSED(m_pExp))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �̗͂̐���
	m_pLife = CObjectGauge3D::Create
	( // ����
		CObject::LABEL_GAUGE,				// �I�u�W�F�N�g���x��
		this,								// �Q�[�W�\���I�u�W�F�N�g
		m_aStatusInfo[m_level.nLife].nLife,	// �ő�\���l
		(int)(NORMAL_CNT * 0.25f),			// �\���l�ϓ��t���[��
		GAUGE_PLUS_Y,						// �\��Y�ʒu�̉��Z��
		GAUGE_GAUGESIZE,					// �Q�[�W�傫��
		GAUGE_FRONTCOL,						// �\�Q�[�W�F
		GAUGE_BACKCOL,						// ���Q�[�W�F
		true,								// �g�`���
		CObjectGauge3D::TYPE_PLAYER,		// �g���
		GAUGE_GAUGESIZE						// �g�傫��
	);
	if (UNUSED(m_pLife))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �_�b�V���̐���
	m_pDash = CGaugeStar::Create
	( // ����
		m_aStatusInfo[m_level.nDash].nDash,	// �ő�Q�[�W��
		STAR_RADIUS,						// ���a
		STAR_HEAL,							// �Q�[�W�񕜗�
		STAR_WAIT_HEAL,						// �񕜑ҋ@�J�E���g
		this,								// �e�I�u�W�F�N�g
		STAR_ADDPOS,						// �\���ʒu�̉��Z��
		STAR_DIS_CENTER,					// ���S����̋���
		STAR_FLICKER						// �h��߂���
	);
	if (UNUSED(m_pDash))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�̐���
	m_pShadow = CShadow::Create(CShadow::TEXTURE_NORMAL, PLAY_SHADOW_SIZE, this);
	if (UNUSED(m_pShadow))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �O�Ղ̐���
	m_pOrbit = CObjectOrbit::Create(GetMultiModel(MODEL_ROD)->GetPtrMtxWorld(), PLAY_ORBIT_COL, CObjectOrbit::OFFSET_ROD, PLAY_ORBIT_PART);
	if (UNUSED(m_pOrbit))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����Ǎ��E����
	m_pOrbit->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_ORBIT]));

	// �D�揇�ʂ�ݒ�
	m_pOrbit->SetPriority(PLAY_ORBIT_PRIO);

	// �����x��ݒ�
	SetAlpha(0.0f);

	// �\���̐ݒ�
	SetDisp(false);

	// ���[�V�����̐ݒ�
	SetMotion(MOTION_MOVE);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CPlayer::Uninit(void)
{
	// ���@�}�l�[�W���[��j��
	if (FAILED(m_pMagic->Release(m_pMagic)))
	{ // �j���Ɏ��s�����ꍇ

		// ��O����
		assert(false);
	}

	// �o���l�}�l�[�W���[��j��
	if (FAILED(m_pExp->Release(m_pExp)))
	{ // �j���Ɏ��s�����ꍇ

		// ��O����
		assert(false);
	}

	// �e��j��
	m_pShadow->Uninit();

	// �O�Ղ�j��
	m_pOrbit->Uninit();

	// �I�u�W�F�N�g�L�����N�^�[�̏I��
	CObjectChara::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CPlayer::Update(void)
{
	// ���@�}�l�[�W���[�̍X�V
	m_pMagic->Update();

	// �o���l�}�l�[�W���[�̍X�V
	m_pExp->Update();

	// �e�̍X�V
	m_pShadow->Update();

	// �O�Ղ̍X�V
	m_pOrbit->Update();
}

//============================================================
//	�`�揈��
//============================================================
void CPlayer::Draw(void)
{
	// �I�u�W�F�N�g�L�����N�^�[�̕`��
	CObjectChara::Draw();
}

//============================================================
//	�q�b�g����
//============================================================
void CPlayer::Hit(const int nDmg)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = GetPosition();	// �v���C���[�ʒu
	int nRevDamage = 0;	// �␳��̃_���[�W��

	if (IsDeath() != true)
	{ // ���S�t���O�������Ă��Ȃ��ꍇ

		if (m_state == STATE_NORMAL)
		{ // �ʏ��Ԃ̏ꍇ

			// �_���[�W�ʂ�␳
			nRevDamage = nDmg - (int)(m_aStatusInfo[m_level.nDefense].nDefense * 0.25f);
			useful::LimitNum(nRevDamage, 0, m_aStatusInfo[m_level.nLife].nLife);	// �_���[�W�ʐ���

			// �̗͂���_���[�W�����Z
			m_pLife->AddNum(-nRevDamage);

			if (m_pLife->GetNum() > 0)
			{ // �����Ă���ꍇ

				// �J�E���^�[��������
				m_nCounterState = 0;

				// ��Ԃ�ύX
				m_state = STATE_DAMAGE;	// �_���[�W���
			}
			else
			{ // ����ł���ꍇ

				// �J�E���^�[��������
				m_nCounterState = 0;

				// ��Ԃ�ύX
				m_state = STATE_DEATH;	// ���S���

				// ���[�V�����̐ݒ�
				SetMotion(MOTION_DEATH);	// ���S���[�V����

				// �T�E���h�̍Đ�
				CManager::GetSound()->Play(CSound::LABEL_SE_BREAK);	// �j��
			}

			// �p�[�e�B�N��3D�I�u�W�F�N�g�𐶐�
			CParticle3D::Create(CParticle3D::TYPE_PLAYER_DAMAGE, pos);
		}
	}
}

//============================================================
//	��������
//============================================================
CPlayer *CPlayer::Create
(
	const CScene::MODE mode,	// ���[�h
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot		// ����
)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = rPos;		// ���W�ݒ�p

	// �|�C���^��錾
	CPlayer *pPlayer = NULL;	// �v���C���[�����p

	if (UNUSED(pPlayer))
	{ // �g�p����Ă��Ȃ��ꍇ

		switch (mode)
		{ // ���[�h���Ƃ̏���
		case CScene::MODE_TITLE:

			// ����

			break;

		case CScene::MODE_TUTORIAL:

			// �������m��
			pPlayer = new CPlayerTutorial;	// �`���[�g���A���v���C���[

			break;

		case CScene::MODE_GAME:

			// �������m��
			pPlayer = new CPlayerGame;	// �Q�[���v���C���[

			break;

		case CScene::MODE_RESULT:

			// ����

			break;

		default:	// ��O����
			assert(false);
			break;
		}
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pPlayer))
	{ // �g�p����Ă���ꍇ

		// �v���C���[�̏�����
		if (FAILED(pPlayer->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pPlayer;
			pPlayer = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		CScene::GetStage()->LimitPosition(pos, PLAY_RADIUS);	// �X�e�[�W�͈͊O�␳
		pos.y = CScene::GetField()->GetPositionHeight(pos);		// ������n�ʂɐݒ�
		pPlayer->SetPosition(pos);

		// ������ݒ�
		pPlayer->SetRotation(rRot);
	}

	// �m�ۂ����A�h���X��Ԃ�
	return pPlayer;
}

//============================================================
//	�X�e�[�^�X���擾����
//============================================================
CPlayer::StatusInfo CPlayer::GetStatusInfo(const LEVEL level)
{
	// �����̃��x���̃X�e�[�^�X����Ԃ�
	return m_aStatusInfo[level];
}

//============================================================
//	�X�e�[�^�X���x���̉��Z����
//============================================================
void CPlayer::AddLevelStatus(const LEVELINFO level)
{
	// �ϐ���錾
	int nLife = 0;	// �̗͂̕ۑ��p
	int nMana = 0;	// �}�i�̕ۑ��p

	// �����̃��x�������Z
	switch (level)
	{ // ���x����񂲂Ƃ̏���
	case LEVELINFO_LIFE:	// �̗�

		// �̗͂̃��x�������Z
		m_level.nLife++;

		// ���݂̗̑͂��擾
		nLife = m_pLife->GetNum();

		// �ő�̗͂��X�V
		m_pLife->SetMaxNum(m_aStatusInfo[m_level.nLife].nLife);

		// �̗͂̑��������قǂ܂ł̗̑͂ɉ��Z���Đݒ�
		m_pLife->SetNum(nLife + (m_aStatusInfo[m_level.nLife].nLife - m_aStatusInfo[m_level.nLife - 1].nLife));

		// �̗̓I�[�o�[
		assert(m_level.nLife < LEVEL_MAX);

		break;

	case LEVELINFO_MANA:		// �}�i

		// �}�i�̃��x�������Z
		m_level.nMana++;

		// ���݂̃}�i���擾
		nMana = m_pMagic->GetMana();

		// �ő�}�i���X�V
		m_pMagic->SetMaxMana(m_aStatusInfo[m_level.nMana].nMana);

		// �}�i�̑��������قǂ܂ł̃}�i�ɉ��Z���Đݒ�
		m_pMagic->SetMana(nMana + (m_aStatusInfo[m_level.nMana].nMana - m_aStatusInfo[m_level.nMana - 1].nMana));

		// �}�i�I�[�o�[
		assert(m_level.nMana < LEVEL_MAX);

		break;

	case LEVELINFO_DASH:	// �_�b�V��

		// �_�b�V���̃��x�������Z
		m_level.nDash++;

		// �ő�_�b�V�����X�V
		m_pDash->SetMaxNumGauge(m_aStatusInfo[m_level.nDash].nDash);

		// �_�b�V���I�[�o�[
		assert(m_level.nDash < LEVEL_MAX);

		break;

	case LEVELINFO_DEFENSE:	// �h���

		// �h��͂̃��x�������Z
		m_level.nDefense++;

		// �h��̓I�[�o�[
		assert(m_level.nDefense < LEVEL_MAX);

		break;

	case LEVELINFO_SPEED:	// �f����

		// �f�����̃��x�������Z
		m_level.nSpeed++;

		// �f�����I�[�o�[
		assert(m_level.nSpeed < LEVEL_MAX);

		break;

	case LEVELINFO_EXP_UP:	// �o���l�{�[�i�X

		// �o���l�{�[�i�X�̃��x�������Z
		m_level.nMulExp++;

		// �o���l�{�[�i�X�I�[�o�[
		assert(m_level.nMulExp < LEVEL_MAX);

		break;

	default:	// ��O����
		assert(false);
		break;
	}
}

//============================================================
//	���x���̉��Z����
//============================================================
void CPlayer::AddLevel(const int nAdd)
{
	// ���x�������Z
	m_pExp->AddLevel(nAdd);
}

//============================================================
//	�o���l�̉��Z����
//============================================================
void CPlayer::AddExp(const int nAdd)
{
	// �o���l�����Z
	m_pExp->AddExp(nAdd);
}

//============================================================
//	�̗͂̉��Z����
//============================================================
void CPlayer::AddLife(const int nAdd)
{
	// �̗͂����Z
	m_pLife->AddNum(nAdd);
}

//============================================================
//	�ďo���̐ݒ菈��
//============================================================
void CPlayer::SetRespawn(D3DXVECTOR3& rPos)
{
	// ����������
	SetMotion(CPlayer::MOTION_MOVE);	// ���V���[�V������ݒ�
	m_state = STATE_NORMAL;	// �ʏ��Ԃ�ݒ�
	m_nCounterState = 0;	// �J�E���^�[��������

	// �ʒu��␳�E�ݒ�
	CScene::GetStage()->LimitPosition(rPos, PLAY_RADIUS);	// �X�e�[�W�͈͊O�̕␳
	UpdateLanding(rPos);	// ���n����
	SetPosition(rPos);		// �ʒu��ݒ�

	// ������ݒ�
	SetRotation(RESPAWN_ROT);

	// �^�[�Q�b�g�Ƃ̋�����ݒ�
	UpdateDisTarget();

	// �}�i�Q�[�W��S�񕜂�����
	m_pMagic->SetMana(m_pMagic->GetMaxMana());

	// �_�b�V���Q�[�W��S�񕜂�����
	m_pDash->HealNumGauge();

	// �O�Ղ̏��������s����Ԃɂ���
	m_pOrbit->SetEnableInit(false);

	// �\������ݒ�ɂ���
	SetDisp(true);

	// �}�e���A�����Đݒ�
	ResetMaterial();

	// �����x��s�����ɍĐݒ�
	SetAlpha(1.0f);

	// �J�����X�V��ON�ɂ���
	CManager::GetCamera()->SetEnableUpdate(true);

	// �J�����ڕW�ʒu�ݒ�
	CManager::GetCamera()->SetDestBargaining();
}

//============================================================
//	�}�i�̐ݒ菈��
//============================================================
void CPlayer::SetMana(const int nMana)
{
	// �����̃}�i��ݒ�
	m_pMagic->SetMana(nMana);
}

//============================================================
//	��Ԃ̐ݒ菈��
//============================================================
void CPlayer::SetState(const int nState)
{
	// �����̏�Ԃ�ݒ�
	m_state = (STATE)nState;
}

//============================================================
//	�X�V�󋵂̐ݒ菈��
//============================================================
void CPlayer::SetEnableUpdate(const bool bUpdate)
{
	// �����̍X�V�󋵂�ݒ�
	CObject::SetEnableUpdate(bUpdate);		// ���g
	m_pShadow->SetEnableUpdate(bUpdate);	// �e
	m_pOrbit->SetEnableUpdate(bUpdate);		// �O��
	m_pDash->SetEnableUpdate(bUpdate);		// �_�b�V��
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CPlayer::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	CObject::SetEnableDraw(bDraw);		// ���g
	m_pShadow->SetEnableDraw(bDraw);	// �e
	m_pOrbit->SetEnableDraw(bDraw);		// �O��
}

//============================================================
//	�}�i�̕`��󋵂̐ݒ菈��
//============================================================
void CPlayer::SetEnableDrawMana(const bool bDraw)
{
	// �����̕`��󋵂��}�i�ɐݒ�
	m_pMagic->SetEnableManaDraw(bDraw);
}

//============================================================
//	�o���l�̕`��󋵂̐ݒ菈��
//============================================================
void CPlayer::SetEnableDrawExp(const bool bDraw)
{
	// �����̕`��󋵂��o���l�ɐݒ�
	m_pExp->SetEnableExpDraw(bDraw);
}

//============================================================
//	�̗͂̕`��󋵂̐ݒ菈��
//============================================================
void CPlayer::SetEnableDrawLife(const bool bDraw)
{
	// �����̕`��󋵂�̗͂ɐݒ�
	m_pLife->SetEnableDraw(bDraw);
}

//============================================================
//	�_�b�V���̕`��󋵂̐ݒ菈��
//============================================================
void CPlayer::SetEnableDrawDash(const bool bDraw)
{
	// �����̕`��󋵂��_�b�V���ɐݒ�
	m_pDash->SetEnableDraw(bDraw);
}

//============================================================
//	�}�i�̉񕜏󋵂̐ݒ菈��
//============================================================
void CPlayer::SetEnableHealMana(const bool bHeal)
{
	// �����̉񕜏󋵂��}�i�̉񕜏󋵂ɐݒ�
	m_pMagic->SetEnableHealMana(bHeal);
}

//============================================================
//	�}�g���b�N�X�擾����
//============================================================
D3DXMATRIX CPlayer::GetMtxWorld(void) const
{
	// �ϐ���錾
	D3DXMATRIX  mtxRot, mtxTrans, mtxWorld;	// �v�Z�p�}�g���b�N�X
	D3DXVECTOR3 posPlayer = GetPosition();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetRotation();	// �v���C���[����

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotPlayer.y, rotPlayer.x, rotPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, posPlayer.x, posPlayer.y, posPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X��Ԃ�
	return mtxWorld;
}

//============================================================
//	�^�[�Q�b�g�Ƃ̋����擾����
//============================================================
float CPlayer::GetDistanceTarget(void) const
{
	// �^�[�Q�b�g�Ƃ̋�����Ԃ�
	return m_fDisTarget;
}

//============================================================
//	�X�e�[�^�X���x���擾����
//============================================================
int CPlayer::GetLevelStatus(const LEVELINFO level) const
{
	// �����̃��x����Ԃ�
	switch (level)
	{ // ���x����񂲂Ƃ̏���
	case LEVELINFO_LIFE:	// �̗�

		// �̗͂̃��x����Ԃ�
		return m_level.nLife;

	case LEVELINFO_MANA:	// �}�i

		// �}�i�̃��x����Ԃ�
		return m_level.nMana;

	case LEVELINFO_DASH:	// �_�b�V��

		// �_�b�V���̃��x����Ԃ�
		return m_level.nDash;

	case LEVELINFO_DEFENSE:	// �h���

		// �h��͂̃��x����Ԃ�
		return m_level.nDefense;

	case LEVELINFO_SPEED:	// �f����

		// �f�����̃��x����Ԃ�
		return m_level.nSpeed;

	case LEVELINFO_EXP_UP:	// �o���l�{�[�i�X

		// �o���l�{�[�i�X�̃��x����Ԃ�
		return m_level.nMulExp;

	default:	// ��O����
		assert(false);
		return NONE_IDX;
	}
}

//============================================================
//	���x���擾����
//============================================================
int CPlayer::GetLevel(void) const
{
	// ���x����Ԃ�
	return m_pExp->GetLevel();
}

//============================================================
//	�̗͎擾����
//============================================================
int CPlayer::GetLife(void) const
{
	// �̗͂�Ԃ�
	return m_pLife->GetNum();
}

//============================================================
//	�ő�̗͎擾����
//============================================================
int CPlayer::GetMaxLife(void) const
{
	// �ő�̗͂�Ԃ�
	return m_pLife->GetMaxNum();
}

//============================================================
//	�}�i�擾����
//============================================================
int CPlayer::GetMana(void) const
{
	// �}�i��Ԃ�
	return m_pMagic->GetMana();
}

//============================================================
//	�ő�}�i�擾����
//============================================================
int CPlayer::GetMaxMana(void) const
{
	// �ő�}�i��Ԃ�
	return m_pMagic->GetMaxMana();
}

//============================================================
//	��Ԏ擾����
//============================================================
int CPlayer::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//============================================================
//	���a�擾����
//============================================================
float CPlayer::GetRadius(void) const
{
	// ���a��Ԃ�
	return PLAY_RADIUS;
}

//============================================================
//	������т̐ݒ菈��
//============================================================
void CPlayer::SetBlowAway(const D3DXVECTOR3& rVecAway)
{
	// �ړ��ʂ�ݒ�
	m_move = rVecAway * AWAY_SIDE_MOVE;
	m_move.y = AWAY_UP_MOVE;
}

//============================================================
//	�Q�������܂�̐ݒ菈��
//============================================================
void CPlayer::SetVortex(const D3DXVECTOR3& rPlayerPos, const D3DXVECTOR3& rHitPos)
{
	// �������܂�n�߂̌�����ݒ�
	m_fVortexRot = atan2f(rPlayerPos.x - rHitPos.x, rPlayerPos.z - rHitPos.z);

	// �Q�Ƃ̋�����ݒ�
	m_fVortexDis = sqrtf((rPlayerPos.x - rHitPos.x) * (rPlayerPos.x - rHitPos.x) + (rPlayerPos.z - rHitPos.z) * (rPlayerPos.z - rHitPos.z)) * 0.5f;
}

//============================================================
//	�ߋ��ʒu�̍X�V����
//============================================================
void CPlayer::UpdateOldPosition(void)
{
	// �ߋ��ʒu���X�V
	m_oldPos = GetPosition();
}

//============================================================
//	�^�[�Q�b�g�����̍X�V����
//============================================================
void CPlayer::UpdateDisTarget(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posPlayer = GetPosition();	// �v���C���[�ʒu
	D3DXVECTOR3 posTarget = CScene::GetTarget()->GetPosition();	// �^�[�Q�b�g�ʒu

	// �^�[�Q�b�g�Ƃ̋�����ݒ�
	m_fDisTarget = sqrtf((posPlayer.x - posTarget.x) * (posPlayer.x - posTarget.x)+ (posPlayer.z - posTarget.z) * (posPlayer.z - posTarget.z)) * 0.5f;
}

//============================================================
//	���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V����
//============================================================
void CPlayer::UpdateMotion(int nMotion)
{
	// �ϐ���錾
	int nAnimMotion = GetMotionType();	// ���ݍĐ����̃��[�V����

	if (nMotion != NONE_IDX)
	{ // ���[�V�������ݒ肳��Ă���ꍇ

		if (IsMotionLoop(nAnimMotion))
		{ // ���[�v���郂�[�V�����������ꍇ

			if (nAnimMotion != nMotion)
			{ // ���݂̃��[�V�������Đ����̃��[�V�����ƈ�v���Ȃ��ꍇ

				// ���݂̃��[�V�����̐ݒ�
				SetMotion(nMotion);
			}
		}
	}

	// �I�u�W�F�N�g�L�����N�^�[�̍X�V
	CObjectChara::Update();

	// ���[�V�����̑J��
	if (IsMotionFinish())
	{ // ���[�V�������I�����Ă����ꍇ

		switch (GetMotionType())
		{ // ���[�V�����̎�ނ��Ƃ̏���
		case MOTION_BLOW_AWAY:	// ������я��

			// ����

			// �����𔲂���
			break;
		}
	}
}

//============================================================
//	�z�����݂̍X�V����
//============================================================
void CPlayer::UpdateAbsorb(D3DXVECTOR3& rVecTarg, D3DXVECTOR3& rVecSide)
{
	// �ϐ���錾
	D3DXVECTOR3 posPlayer = GetPosition();	// �v���C���[�ʒu
	D3DXVECTOR3 posTarget = CScene::GetTarget()->GetPosition();	// �^�[�Q�b�g�ʒu
	float fDisTargRate = (1.0f / CScene::GetStage()->GetStageLimit().fRadius) * m_fDisTarget;	// �^�[�Q�b�g�����̊���

	// �^�[�Q�b�g�t�����̃x�N�g�����v�Z
	rVecTarg = posTarget - posPlayer;
	rVecTarg.y = 0.0f;							// �x�N�g���̏c�����𖳎�
	D3DXVec3Normalize(&rVecTarg, &rVecTarg);	// �x�N�g���̐��K��

	// �^�[�Q�b�g�������x�N�g�����v�Z
	rVecSide = D3DXVECTOR3(rVecTarg.z, 0.0f, -rVecTarg.x);

	// �ڕW������ݒ�
	m_destRot.y = atan2f(posTarget.x - posPlayer.x, posTarget.z - posPlayer.z);
	m_destRot.y -= D3DX_PI * 0.5f;	// ������������
	m_destRot.y -= ALWAYS_ADDROT;	// �������ق�̂�������ɂ���

	// �����ւ̈ړ��ʂ�ݒ�
	m_move += rVecTarg * (MOVE_INSIDE + (fDisTargRate * PLUS_INSIDE_MOVE));

	// �����ւ̈ړ��ʂ�ݒ�
	m_move += rVecSide * MOVE_LEFT;
}

//============================================================
//	�^�[�Q�b�g�t�����ւ̉������̑��쏈��
//============================================================
CPlayer::MOTION CPlayer::ControlTargAccel(const D3DXVECTOR3& rVecTarg, bool *pMove)
{
	// �ϐ���錾
	float fDisTargRate = (1.0f / CScene::GetStage()->GetStageLimit().fRadius) * m_fDisTarget;	// �^�[�Q�b�g�����̊���

	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetPad();		// �p�b�h

	if (pKeyboard->GetPress(DIK_W) || pPad->GetPressLStickY() > 0.0f)
	{
		// �O���ւ̈ړ��ʂ�ǉ�
		m_move -= rVecTarg * (m_aStatusInfo[m_level.nSpeed].speed.fOutside + (fDisTargRate * PLUS_OUTSIDE_MOVE));

		// �O������������
		m_destRot.y += INSIDE_ADDROT;

		if (USED(pMove))
		{ // �ړ�����̈������g�p����Ă����ꍇ

			// �ړ����Ă����Ԃɂ���
			*pMove = true;
		}
	}
	else if (pKeyboard->GetPress(DIK_S) || pPad->GetPressLStickY() < 0.0f)
	{
		// �����ւ̈ړ��ʂ�ǉ�
		m_move += rVecTarg * m_aStatusInfo[m_level.nSpeed].speed.fInside;

		// ��������������
		m_destRot.y -= OUTSIDE_SUBROT;

		if (USED(pMove))
		{ // �ړ�����̈������g�p����Ă����ꍇ

			// �ړ����Ă����Ԃɂ���
			*pMove = true;
		}
	}
	else
	{
		if (USED(pMove))
		{ // �ړ�����̈������g�p����Ă����ꍇ

			// �ړ����Ă��Ȃ���Ԃɂ���
			*pMove = false;
		}
	}

	// ���V���[�V������Ԃ�
	return MOTION_MOVE;
}

//============================================================
//	�^�[�Q�b�g�������ւ̉������̑��쏈��
//============================================================
CPlayer::MOTION CPlayer::ControlSideAccel(const D3DXVECTOR3& rVecSide, bool *pMove)
{
	// �ϐ���錾
	MOTION currentMotion = MOTION_MOVE;	// ���݂̃��[�V����

	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetPad();		// �p�b�h

	if (pKeyboard->GetPress(DIK_A) || pPad->GetPress(CInputPad::KEY_L1))
	{
		if (m_pDash->UseGauge())
		{ // �Q�[�W���g�p�ł����ꍇ

			// ���ړ��ʂ�����
			m_move += rVecSide * m_aStatusInfo[m_level.nSpeed].speed.fLeftside;

			// ���݂̃��[�V������ݒ�
			currentMotion = MOTION_ACCELE;	// �������[�V����

			if (USED(pMove))
			{ // �ړ�����̈������g�p����Ă����ꍇ

				// �ړ����Ă����Ԃɂ���
				*pMove = true;
			}
		}
	}
	else if (pKeyboard->GetPress(DIK_D) || pPad->GetPress(CInputPad::KEY_R1))
	{
		// ���ړ��ʂ�����
		m_move -= rVecSide * MOVE_LEFT_DECELE;

		// ���݂̃��[�V������ݒ�
		currentMotion = MOTION_DECELE;	// �������[�V����

		if (USED(pMove))
		{ // �ړ�����̈������g�p����Ă����ꍇ

			// �ړ����Ă����Ԃɂ���
			*pMove = true;
		}
	}
	else
	{
		if (USED(pMove))
		{ // �ړ�����̈������g�p����Ă����ꍇ

			// �ړ����Ă��Ȃ���Ԃɂ���
			*pMove = false;
		}
	}

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//============================================================
//	���@�U���̑��쏈��
//============================================================
CPlayer::MOTION CPlayer::ControlShotMagic(MOTION motion, bool *pShot)
{
	// �ϐ���錾
	MOTION currentMotion = motion;	// ���݂̃��[�V����
	CMagicManager::Shot shot;		// ���@�̔��ˏ�

	// ���@�̔���
	shot = m_pMagic->ShotMagic();	// ���ˏ�����

	if (shot.bControl && m_pMagic->GetMana() > 0)
	{ // ���ˑ�������Ă��銎�A�}�i�c�ʂ�����ꍇ

		// �ˌ��������̒l��0�`2�΂ɂ���
		shot.fRot += D3DX_PI;

		if (shot.fRot >= QRTR_PI && shot.fRot <= D3DX_PI - QRTR_PI)
		{
			// �E�̃A�N�V�������[�V������ݒ�
			currentMotion = MOTION_ACTION_RIGHT;
		}
		else if (shot.fRot >= D3DX_PI - QRTR_PI && shot.fRot <= D3DX_PI + QRTR_PI)
		{
			// ��O�̃A�N�V�������[�V������ݒ�
			currentMotion = MOTION_ACTION_NEAR;
		}
		else if (shot.fRot >= D3DX_PI + QRTR_PI && shot.fRot <= (D3DX_PI * 2) - QRTR_PI)
		{
			// ���̃A�N�V�������[�V������ݒ�
			currentMotion = MOTION_ACTION_LEFT;
		}
		else if (shot.fRot >= (D3DX_PI * 2) - QRTR_PI && shot.fRot <= (D3DX_PI * 2) || shot.fRot >= 0.0f && shot.fRot <= QRTR_PI)
		{
			// ���̃A�N�V�������[�V������ݒ�
			currentMotion = MOTION_ACTION_FAR;
		}
		else
		{
			// ��O����
			assert(false);	// �����̐��K���~�X
		}

		if (USED(pShot))
		{ // �ˌ�����̈������g�p����Ă����ꍇ

			// �ˌ����Ă����Ԃɂ���
			*pShot = true;
		}
	}
	else
	{ // ���˂ł��Ă��Ȃ��ꍇ

		if (USED(pShot))
		{ // �ˌ�����̈������g�p����Ă����ꍇ

			// �ˌ����Ă��Ȃ���Ԃɂ���
			*pShot = false;
		}
	}

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//============================================================
//	���n�󋵂̍X�V����
//============================================================
bool CPlayer::UpdateLanding(D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	bool bLand = false;	// ���n��

	// ���n����
	if (CScene::GetField()->LandPosition(rPos, m_move)
	||  CScene::GetStage()->LandPosition(rPos, m_move, 0.0f))
	{ // �v���C���[�����n���Ă����ꍇ

		// ���n���Ă����Ԃɂ���
		bLand = true;

		// �W�����v���Ă��Ȃ���Ԃɂ���
		m_bJump = false;
	}
	else
	{ // ���n���Ă��Ȃ��ꍇ

		// �W�����v���Ă����Ԃɂ���
		m_bJump = true;
	}

	// ���n�󋵂�Ԃ�
	return bLand;
}

//============================================================
//	�ʒu�̍X�V����
//============================================================
void CPlayer::UpdatePosition(D3DXVECTOR3& rPos)
{
	// �d�͂����Z
	m_move.y -= PLAY_GRAVITY;

	// �ړ��ʂ����Z
	rPos += m_move;

	// �ړ��ʂ�����
	if (m_state == STATE_BLOW_AWAY)
	{ // �󒆂̏ꍇ

		m_move.x += (0.0f - m_move.x) * PLAY_AWAY_REV;
		m_move.z += (0.0f - m_move.z) * PLAY_AWAY_REV;
	}
	else
	{ // �n��̏ꍇ

		m_move.x += (0.0f - m_move.x) * PLAY_LAND_REV;
		m_move.z += (0.0f - m_move.z) * PLAY_LAND_REV;
	}
}

//============================================================
//	�����̍X�V����
//============================================================
void CPlayer::UpdateRotation(D3DXVECTOR3& rRot)
{
	// �ϐ���錾
	float fDiffRot = 0.0f;	// ��������

	// �ڕW�����̐��K��
	useful::NormalizeRot(m_destRot.y);

	// �ڕW�����܂ł̍������v�Z
	fDiffRot = m_destRot.y - rRot.y;

	// ���������̐��K��
	useful::NormalizeRot(fDiffRot);

	// �����̍X�V
	rRot.y += fDiffRot * PLAY_REV_ROTA;

	// �����̐��K��
	useful::NormalizeRot(rRot.y);
}

//============================================================
//	�^�[�Q�b�g�Ƃ̓����蔻��
//============================================================
void CPlayer::CollisionTarget(D3DXVECTOR3& rPos)
{
	// �|�C���^��錾
	CTarget *pTarget = CScene::GetTarget();	// �^�[�Q�b�g���

	if (pTarget->GetState() != CTarget::STATE_DESTROY)
	{ // �^�[�Q�b�g���j�󂳂�Ă��Ȃ��ꍇ

		// �^�[�Q�b�g�Ƃ̏Փ˔���
		collision::CirclePillar
		( // ����
			rPos,					// ����ʒu
			pTarget->GetPosition(),	// ����ڕW�ʒu
			PLAY_RADIUS,			// ���蔼�a
			pTarget->GetRadius()	// ����ڕW���a
		);
	}
}

//============================================================
//	�G�Ƃ̓����蔻��
//============================================================
void CPlayer::CollisionEnemy(D3DXVECTOR3& rPos)
{
	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		// �|�C���^��錾
		CObject *pObjectTop = CObject::GetTop(nCntPri);	// �擪�I�u�W�F�N�g

		if (USED(pObjectTop))
		{ // �擪�����݂���ꍇ

			// �|�C���^��錾
			CObject *pObjCheck = pObjectTop;	// �I�u�W�F�N�g�m�F�p

			while (USED(pObjCheck))
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

				// �|�C���^��錾
				CObject *pObjectNext = pObjCheck->GetNext();	// ���I�u�W�F�N�g

				if (pObjCheck->GetLabel() != CObject::LABEL_ENEMY)
				{ // �I�u�W�F�N�g���x�����G�ł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				if (pObjCheck->GetState() == CEnemy::STATE_SPAWN
				||  pObjCheck->GetState() == CEnemy::STATE_DEATH)
				{ // �G�̏�Ԃ��X�|�[����Ԃ����S��Ԃ̏ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// �G�Ƃ̓����蔻��
				if (collision::CirclePillar
				( // ����
					rPos,						// ����ʒu
					pObjCheck->GetPosition(),	// ����ڕW�ʒu
					PLAY_RADIUS,				// ���蔼�a
					CEnemy::GetStatusInfo(pObjCheck->GetType()).fRadius	// ����ڕW���a
				))
				{ // �������Ă����ꍇ

					// �v���C���[�̃q�b�g����
					HitBlowAway(rPos, pObjCheck->GetPosition(), ENE_HIT_DMG);
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}
}

//============================================================
//	�_���[�W��Ԏ��̍X�V����
//============================================================
CPlayer::MOTION CPlayer::UpdateDamage(void)
{
	// �ϐ���錾
	MOTION currentMotion;	// ���݂̃��[�V����

	// �ʏ��Ԏ��̍X�V
	currentMotion = UpdateNormal();

	if (m_nCounterState < INVULN_CNT)
	{ // �J�E���^�[�����l��菬�����ꍇ

		// �J�E���^�[�����Z
		m_nCounterState++;

		// �}�e���A����ݒ�
		SetMaterial(material::Red());	// ��
	}
	else
	{ // �J�E���^�[�����l�ȏ�̏ꍇ

		// �J�E���^�[��������
		m_nCounterState = 0;

		// �}�e���A���Đݒ�
		ResetMaterial();

		// ��Ԃ�ύX
		m_state = STATE_INVULN;	// ���G���
	}

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//============================================================
//	���G��Ԏ��̍X�V����
//============================================================
CPlayer::MOTION CPlayer::UpdateInvuln(void)
{
	// �ϐ���錾
	MOTION currentMotion;	// ���݂̃��[�V����

	// �ʏ��Ԏ��̍X�V
	currentMotion = UpdateNormal();

	if (m_nCounterState < NORMAL_CNT)
	{ // �J�E���^�[�����l��菬�����ꍇ

		// �J�E���^�[�����Z
		m_nCounterState++;

		if (m_nCounterState % CHANGE_ALPHA_CNT == 0)
		{ // �����x�̕ύX�^�C�~���O�������ꍇ

			if (GetAlpha() > INVULN_MIN_ALPHA)
			{ // �����x���オ���Ă��Ȃ��ꍇ

				// �����x���グ��
				SetAlpha(INVULN_MIN_ALPHA);
			}
			else
			{ // �����x���オ���Ă���ꍇ

				// �����x��������
				SetAlpha(INVULN_MAX_ALPHA);
			}
		}
	}
	else
	{ // �J�E���^�[�����l�ȏ�̏ꍇ

		// �J�E���^�[��������
		m_nCounterState = 0;

		// �����x��s�����ɐݒ�
		SetAlpha(1.0f);

		// ��Ԃ�ύX
		m_state = STATE_NORMAL;	// �ʏ���
	}

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//============================================================
//	������я�Ԏ��̍X�V����
//============================================================
void CPlayer::UpdateBlowAway(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posPlayer = GetPosition();	// �v���C���[�ʒu

	// �ʒu�X�V
	UpdatePosition(posPlayer);

	// �^�[�Q�b�g�Ƃ̓����蔻��
	CollisionTarget(posPlayer);

	// �X�e�[�W�͈͊O�̕␳
	CScene::GetStage()->LimitPosition(posPlayer, PLAY_RADIUS);

	// ���n����
	if (UpdateLanding(posPlayer))
	{ // ���n���Ă����ꍇ

		// ���V���[�V�����Ɉڍs
		SetMotion(MOTION_MOVE);

		// ��Ԃ�ݒ�
		m_state = STATE_DAMAGE;	// �_���[�W���
	}

	// �ʒu�𔽉f
	SetPosition(posPlayer);

	// ������ݒ�
	SetRotation(D3DXVECTOR3(0.0f, atan2f(m_move.x, m_move.z), 0.0f));
}

//============================================================
//	�Q�������܂��Ԏ��̍X�V����
//============================================================
void CPlayer::UpdateVortex(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posPlayer = GetPosition();	// �v���C���[�ʒu

	// ���������Z
	m_fVortexRot -= VORTEX_ADDROT;
	useful::NormalizeRot(m_fVortexRot);

	// ���������Z
	m_fVortexDis += VORTEX_ADDDIS;

	// �ʒu��ݒ�
	posPlayer.x = sinf(m_fVortexRot) * m_fVortexDis;
	posPlayer.y += VORTEX_ADDPOSY;
	posPlayer.z = cosf(m_fVortexRot) * m_fVortexDis;

	// �ʒu�𔽉f
	SetPosition(posPlayer);

	// �t�F�[�h�C����Ԏ��̍X�V
	UpdateFadeIn();

	if (m_state == STATE_NONE)
	{ // �����ɂȂ�؂����ꍇ

		// �v���C���[���ďo��������
		SetRespawn(PLAY_SPAWN_POS);
	}
}

//============================================================
//	�t�F�[�h�A�E�g��Ԏ��̍X�V����
//============================================================
void CPlayer::UpdateFadeOut(void)
{
	// �ϐ���錾
	float fAlpha = GetAlpha();	// �����x

	// �v���C���[���g�̕`����ĊJ
	CObject::SetEnableDraw(true);

	// �����x���グ��
	fAlpha += FADE_LEVEL;

	if (fAlpha >= GetMaxAlpha())
	{ // �����x���オ��؂����ꍇ

		// �����x��␳
		fAlpha = GetMaxAlpha();

		// ��Ԃ�ݒ�
		m_state = STATE_INVULN;	// ���G���
	}

	// �����x��ݒ�
	SetAlpha(fAlpha);
}

//============================================================
//	�t�F�[�h�C����Ԏ��̍X�V����
//============================================================
void CPlayer::UpdateFadeIn(void)
{
	// �ϐ���錾
	float fAlpha = GetAlpha();	// �����x

	// �����x��������
	fAlpha -= FADE_LEVEL;

	if (fAlpha <= 0.0f)
	{ // �����x��������؂����ꍇ

		// �����x��␳
		fAlpha = 0.0f;

		// �v���C���[���g�̕`����~
		CObject::SetEnableDraw(false);

		// ��Ԃ�ݒ�
		m_state = STATE_NONE;	// �������Ȃ����
	}

	// �����x��ݒ�
	SetAlpha(fAlpha);
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
void CPlayer::LoadSetup(void)
{
	// �ϐ���錾
	CMotion::MotionInfo info;		// �|�[�Y�̑���p
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 rot = VEC3_ZERO;	// �����̑���p
	int nLevel		= 0;	// ���x���̑���p
	int nID			= 0;	// �C���f�b�N�X�̑���p
	int nParentID	= 0;	// �e�C���f�b�N�X�̑���p
	int nNowPose	= 0;	// ���݂̃|�[�Y�ԍ�
	int nNowKey		= 0;	// ���݂̃L�[�ԍ�
	int nLoop		= 0;	// ���[�v��ON/OFF�̕ϊ��p
	int nEnd		= 0;	// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	// �|�[�Y����p�̕ϐ���������
	memset(&info, 0, sizeof(info));

	// �ÓI�����o�ϐ��̏����N���A
	memset(&m_aStatusInfo[0], 0, sizeof(m_aStatusInfo));	// �X�e�[�^�X���

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(PLAYER_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			// �X�e�[�^�X�̐ݒ�
			if (strcmp(&aString[0], "STATUSSET") == 0)
			{ // �ǂݍ��񂾕����� STATUSSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_STATUSSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "LEVELSET") == 0)
					{ // �ǂݍ��񂾕����� LEVELSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_LEVELSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "LEVEL") == 0)
							{ // �ǂݍ��񂾕����� LEVEL �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nLevel);		// ���x����ǂݍ���
							}
							else if (strcmp(&aString[0], "LIFE") == 0)
							{ // �ǂݍ��񂾕����� LIFE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aStatusInfo[nLevel].nLife);		// �̗͂�ǂݍ���
							}
							else if (strcmp(&aString[0], "MANA") == 0)
							{ // �ǂݍ��񂾕����� MANA �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aStatusInfo[nLevel].nMana);		// �}�i��ǂݍ���
							}
							else if (strcmp(&aString[0], "DASH") == 0)
							{ // �ǂݍ��񂾕����� DASH �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aStatusInfo[nLevel].nDash);		// �_�b�V����ǂݍ���
							}
							else if (strcmp(&aString[0], "DEFENSE") == 0)
							{ // �ǂݍ��񂾕����� DEFENSE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aStatusInfo[nLevel].nDefense);	// �h��͂�ǂݍ���
							}
							else if (strcmp(&aString[0], "LEFTSIDE") == 0)
							{ // �ǂݍ��񂾕����� LEFTSIDE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);								// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nLevel].speed.fLeftside);	// �����ւ̉����ʂ�ǂݍ���
							}
							else if (strcmp(&aString[0], "INSIDE") == 0)
							{ // �ǂݍ��񂾕����� INSIDE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);								// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nLevel].speed.fInside);		// �����ւ̉����ʂ�ǂݍ���
							}
							else if (strcmp(&aString[0], "OUTSIDE") == 0)
							{ // �ǂݍ��񂾕����� OUTSIDE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);								// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nLevel].speed.fOutside);		// �O���ւ̉����ʂ�ǂݍ���
							}
							else if (strcmp(&aString[0], "EXPBONUS") == 0)
							{ // �ǂݍ��񂾕����� EXPBONUS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nLevel].fMulExp);	// �o���l�{�[�i�X��ǂݍ���
							}
						} while (strcmp(&aString[0], "END_LEVELSET") != 0);	// �ǂݍ��񂾕����� END_LEVELSET �ł͂Ȃ��ꍇ���[�v
					}
				} while (strcmp(&aString[0], "END_STATUSSET") != 0);		// �ǂݍ��񂾕����� END_STATUSSET �ł͂Ȃ��ꍇ���[�v
			}

			// �L�����N�^�[�̐ݒ�
			else if (strcmp(&aString[0], "CHARACTERSET") == 0)
			{ // �ǂݍ��񂾕����� CHARACTERSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_CHARACTERSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "PARTSSET") == 0)
					{ // �ǂݍ��񂾕����� PARTSSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_PARTSSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "INDEX") == 0)
							{ // �ǂݍ��񂾕����� INDEX �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nID);			// ���f���̃C���f�b�N�X��ǂݍ���
							}
							else if (strcmp(&aString[0], "PARENT") == 0)
							{ // �ǂݍ��񂾕����� PARENT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nParentID);	// ���f���̐e�̃C���f�b�N�X��ǂݍ���
							}
							else if (strcmp(&aString[0], "POS") == 0)
							{ // �ǂݍ��񂾕����� POS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &pos.x);		// X���W��ǂݍ���
								fscanf(pFile, "%f", &pos.y);		// Y���W��ǂݍ���
								fscanf(pFile, "%f", &pos.z);		// Z���W��ǂݍ���
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // �ǂݍ��񂾕����� ROT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &rot.x);		// X������ǂݍ���
								fscanf(pFile, "%f", &rot.y);		// Y������ǂݍ���
								fscanf(pFile, "%f", &rot.z);		// Z������ǂݍ���
							}
						} while (strcmp(&aString[0], "END_PARTSSET") != 0);	// �ǂݍ��񂾕����� END_PARTSSET �ł͂Ȃ��ꍇ���[�v

						// �p�[�c���̐ݒ�
						CObjectChara::SetPartsInfo(nID, nParentID, pos, rot, mc_apModelFile[nID]);
					}
				} while (strcmp(&aString[0], "END_CHARACTERSET") != 0);		// �ǂݍ��񂾕����� END_CHARACTERSET �ł͂Ȃ��ꍇ���[�v
			}

			// ���[�V�����̐ݒ�
			else if (strcmp(&aString[0], "MOTIONSET") == 0)
			{ // �ǂݍ��񂾕����� MOTIONSET �̏ꍇ

				// ���݂̃|�[�Y�ԍ���������
				nNowPose = 0;

				do
				{ // �ǂݍ��񂾕����� END_MOTIONSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "LOOP") == 0)
					{ // �ǂݍ��񂾕����� LOOP �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nLoop);		// ���[�v��ON/OFF��ǂݍ���

						// �ǂݍ��񂾒l��bool�^�ɕϊ�
						info.bLoop = (nLoop == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "NUM_KEY") == 0)
					{ // �ǂݍ��񂾕����� NUM_KEY �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &info.nNumKey);	// �L�[�̑�����ǂݍ���
					}
					else if (strcmp(&aString[0], "KEYSET") == 0)
					{ // �ǂݍ��񂾕����� KEYSET �̏ꍇ

						// ���݂̃L�[�ԍ���������
						nNowKey = 0;

						do
						{ // �ǂݍ��񂾕����� END_KEYSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "FRAME") == 0)
							{ // �ǂݍ��񂾕����� FRAME �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &info.aKeyInfo[nNowPose].nFrame);	// �L�[���؂�ւ��܂ł̃t���[������ǂݍ���
							}
							else if (strcmp(&aString[0], "KEY") == 0)
							{ // �ǂݍ��񂾕����� KEY �̏ꍇ

								do
								{ // �ǂݍ��񂾕����� END_KEY �ł͂Ȃ��ꍇ���[�v

									// �t�@�C�����當�����ǂݍ���
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "POS") == 0)
									{ // �ǂݍ��񂾕����� POS �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);									// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.x);	// X�ʒu��ǂݍ���
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.y);	// Y�ʒu��ǂݍ���
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.z);	// Z�ʒu��ǂݍ���

										// �ǂݍ��񂾈ʒu�Ƀp�[�c�̏����ʒu�����Z
										info.aKeyInfo[nNowPose].aKey[nNowKey].pos += GetPartsPosition(nNowKey);
									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{ // �ǂݍ��񂾕����� ROT �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);									// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);	// X������ǂݍ���
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);	// Y������ǂݍ���
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);	// Z������ǂݍ���

										// �ǂݍ��񂾌����Ƀp�[�c�̏������������Z
										info.aKeyInfo[nNowPose].aKey[nNowKey].rot += GetPartsRotation(nNowKey);

										// ���������𐳋K��
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);
									}

								} while (strcmp(&aString[0], "END_KEY") != 0);	// �ǂݍ��񂾕����� END_KEY �ł͂Ȃ��ꍇ���[�v

								// ���݂̃L�[�ԍ������Z
								nNowKey++;
							}
						} while (strcmp(&aString[0], "END_KEYSET") != 0);	// �ǂݍ��񂾕����� END_KEYSET �ł͂Ȃ��ꍇ���[�v

						// ���݂̃|�[�Y�ԍ������Z
						nNowPose++;
					}
				} while (strcmp(&aString[0], "END_MOTIONSET") != 0);	// �ǂݍ��񂾕����� END_MOTIONSET �ł͂Ȃ��ꍇ���[�v

				// ���[�V�������̐ݒ�
				CObjectChara::SetMotionInfo(info);
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�v���C���[�Z�b�g�A�b�v�t�@�C���̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}
