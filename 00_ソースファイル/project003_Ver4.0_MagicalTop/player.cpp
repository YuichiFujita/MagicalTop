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
#include "manager.h"
#include "sceneGame.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "collision.h"

#include "multiModel.h"
#include "magicManager.h"
#include "expManager.h"
#include "levelupManager.h"
#include "objectGauge2D.h"
#include "objectOrbit.h"
#include "shadow.h"
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

#define MAX_MOVEX		(5.0f)		// �������s���̑��x�����p
#define PULSROT_MOVEZ	(20)		// �O��ړ����̃v���C���[�����̕ύX��
#define PLUS_MOVEX		(0.5f)		// ���E��]�̈ړ��ʂ̉��Z��
#define PLAY_MOVEZ		(2.0f)		// �O��̈ړ���
#define PLAY_REV		(0.2f)		// �v���C���[�ړ��ʂ̌����W��
#define PLAY_REV_ROTA	(0.15f)		// �v���C���[�����ύX�̌����W��
#define PLAY_CAM_ROTA	(0.04f)		// �J������]��
#define PLAY_JUMP		(20.0f)		// �v���C���[�W�����v��
#define PLAY_GRAVITY	(1.0f)		// �v���C���[�d��
#define PLAY_RADIUS		(20.0f)		// �v���C���[���a
#define PLAY_LIFE		(250)		// �v���C���[�̗�
#define NORMAL_CNT		(180)		// �ʏ��ԂɈڍs����܂ł̃J�E���^�[
#define BARRIER_DMG		(50)		// �o���A�̃_���[�W��
#define ENE_HIT_DMG		(20)		// �G�q�b�g���̃_���[�W��
#define AWAY_SIDE_MOVE	(100.0f)	// ������ю��̉��ړ���
#define AWAY_UP_MOVE	(30.0f)		// ������ю��̏�ړ���
#define FADE_LEVEL		(0.01f)		// �t�F�[�h�̃��l�̉�����

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
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

//************************************************************
//	�q�N���X [CPlayer] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPlayer::CPlayer() : CObjectChara(CObject::LABEL_PLAYER)
{
	// �����o�ϐ����N���A
	m_pMagic		= NULL;				// ���@�}�l�[�W���[�̏��
	m_pExp			= NULL;				// �o���l�}�l�[�W���[�̏��
	m_pLevelup		= NULL;				// �����}�l�[�W���[�̏��
	m_pLife			= NULL;				// �̗͂̏��
	m_pShadow		= NULL;				// �e�̏��
	m_pOrbit		= NULL;				// �O�Ղ̏��
	m_oldPos		= VEC3_ZERO;		// �ߋ��ʒu
	m_move			= VEC3_ZERO;		// �ړ���
	m_destRot		= VEC3_ZERO;		// �ڕW����
	m_rotation		= ROTATION_LEFT;	// ��]����
	m_state			= STATE_NONE;		// ���
	m_nCounterState	= 0;				// ��ԊǗ��J�E���^�[
	m_nNumModel		= 0;				// �p�[�c�̑���
	m_fDisTarget	= 0.0f;				// �^�[�Q�b�g�Ƃ̋���
	m_bJump			= false;			// �W�����v��
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
	// �����o�ϐ���������
	m_pMagic		= NULL;				// ���@�}�l�[�W���[�̏��
	m_pExp			= NULL;				// �o���l�}�l�[�W���[�̏��
	m_pLevelup		= NULL;				// �����}�l�[�W���[�̏��
	m_pLife			= NULL;				// �̗͂̏��
	m_pShadow		= NULL;				// �e�̏��
	m_pOrbit		= NULL;				// �O�Ղ̏��
	m_oldPos		= VEC3_ZERO;		// �ߋ��ʒu
	m_move			= VEC3_ZERO;		// �ړ���
	m_destRot		= VEC3_ZERO;		// �ڕW����
	m_rotation		= ROTATION_LEFT;	// ��]����
	m_state			= STATE_NONE;		// ���
	m_nCounterState	= 0;				// ��ԊǗ��J�E���^�[
	m_nNumModel		= 0;				// �p�[�c�̑���
	m_fDisTarget	= 0.0f;				// �^�[�Q�b�g�Ƃ̋���
	m_bJump			= true;				// �W�����v��

	// ���@�}�l�[�W���[�̐���
	m_pMagic = CMagicManager::Create();
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

	// �����}�l�[�W���[�̐���
	m_pLevelup = CLevelupManager::Create();
	if (UNUSED(m_pLevelup))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �̗͂̐���
	m_pLife = CObjectGauge2D::Create	// TODO�F�萔
	( // ����
		CObject::LABEL_GAUGE,				// �I�u�W�F�N�g���x��
		PLAY_LIFE,							// �ő�̗�
		(int)(NORMAL_CNT * 0.25f),			// �̗͕ϓ��t���[��
		D3DXVECTOR3(260.0f, 640.0f, 0.0f),	// �ʒu
		D3DXVECTOR3(200.0f, 30.0f, 0.0f),	// �Q�[�W�傫��
		D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),	// �\�Q�[�W�F
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)	// ���Q�[�W�F
	);
	if (UNUSED(m_pLife))
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

	// �O�Ղ̐���
	m_pOrbit = CObjectOrbit::Create(GetMultiModel(MODEL_ROD)->GetPtrMtxWorld(), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), CObjectOrbit::OFFSET_ROD, CObjectOrbit::TYPE_NORMAL, 100, 50);	// �萔
	if (UNUSED(m_pOrbit))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����x��ݒ�
	SetAlpha(0.0f);

	// �e��`�悵�Ȃ��ݒ�ɂ���
	m_pShadow->SetEnableDraw(false);

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

	// �����}�l�[�W���[��j��
	if (FAILED(m_pLevelup->Release(m_pLevelup)))
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
	// �ϐ���錾
	int nCurrentMotion = NONE_IDX;	// ���݂̃��[�V����
	D3DXVECTOR3 posPlayer = GetPosition();	// �v���C���[�ʒu
	D3DXVECTOR3 posTarget = CSceneGame::GetTarget()->GetPosition();	// �^�[�Q�b�g�ʒu

	// �ߋ��ʒu���X�V
	m_oldPos = posPlayer;

	// �^�[�Q�b�g�Ƃ̋�����ݒ�
	m_fDisTarget = sqrtf((posPlayer.x - posTarget.x) * (posPlayer.x - posTarget.x)+ (posPlayer.z - posTarget.z) * (posPlayer.z - posTarget.z)) * 0.5f;

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:		// �������Ȃ����

		// ����

		break;

	case STATE_FADEOUT:		// �t�F�[�h�A�E�g���

		// �t�F�[�h�A�E�g��Ԏ��̍X�V
		UpdateFadeOut();

		break;

	case STATE_NORMAL:		// �ʏ���

		// �ʏ��Ԏ��̍X�V
		nCurrentMotion = UpdateNormal();

		break;

	case STATE_BLOW_AWAY:	// ������я��

		// ������я�Ԏ��̍X�V
		UpdateBlowAway();

		break;

	case STATE_DAMAGE:		// �_���[�W���

		// �_���[�W��Ԏ��̍X�V
		nCurrentMotion = UpdateDamage();

		break;

	case STATE_FADEIN:		// �t�F�[�h�C�����

		// �t�F�[�h�C����Ԏ��̍X�V
		UpdateFadeIn();

		break;

	case STATE_DEATH:		// ���S���

		// ����

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// ���@�}�l�[�W���[�̍X�V
	m_pMagic->Update();

	// �o���l�}�l�[�W���[�̍X�V
	m_pExp->Update();

	// �����}�l�[�W���[�̍X�V
	m_pLevelup->Update();

	// �e�̍X�V
	m_pShadow->Update();

	// �O�Ղ̍X�V
	m_pOrbit->Update();

	// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V
	Motion(nCurrentMotion);
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
	D3DXVECTOR3 vecAway;				// ������уx�N�g��

	if (IsDeath() != true)
	{ // ���S�t���O�������Ă��Ȃ��ꍇ

		if (m_state == STATE_NORMAL)
		{ // �ʏ��Ԃ̏ꍇ

			// �̗͂���_���[�W�����Z
			m_pLife->AddNum(-nDmg);

			if (m_pLife->GetNum() > 0)
			{ // �����Ă���ꍇ

				// ������уx�N�g�������߂�
				vecAway = pos - CSceneGame::GetTarget()->GetPosition();
				vecAway.y = 0.0f;						// �c�x�N�g�����폜
				D3DXVec3Normalize(&vecAway, &vecAway);	// �x�N�g���𐳋K��

				// �ړ��ʂ�ݒ�
				m_move = vecAway * AWAY_SIDE_MOVE;
				m_move.y = AWAY_UP_MOVE;

				// ������у��[�V�����Ɉڍs
				SetMotion(MOTION_BLOW_AWAY);

				// �p�[�e�B�N��3D�I�u�W�F�N�g�𐶐�
				CParticle3D::Create(CParticle3D::TYPE_DAMAGE, pos);

				// �J�E���^�[��������
				m_nCounterState = 0;

				// ��Ԃ�ύX
				m_state = STATE_BLOW_AWAY;	// ������я��
			}
			else
			{ // ����ł���ꍇ

				// �p�[�e�B�N��3D�I�u�W�F�N�g�𐶐�
				CParticle3D::Create(CParticle3D::TYPE_DAMAGE, pos, D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f));
				CParticle3D::Create(CParticle3D::TYPE_DAMAGE, pos, D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f));

				// �X�V�ƕ`����~
				SetEnableUpdate(false);
				SetEnableDraw(false);

				// �J�E���^�[��������
				m_nCounterState = 0;

				// ��Ԃ�ύX
				m_state = STATE_DEATH;	// �j����
			}
		}
	}
}

//============================================================
//	��������
//============================================================
CPlayer *CPlayer::Create(D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot)
{
	// �|�C���^��錾
	CPlayer *pPlayer = NULL;	// �v���C���[�����p

	if (UNUSED(pPlayer))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pPlayer = new CPlayer;	// �v���C���[
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
		pPlayer->SetPosition(rPos);

		// ������ݒ�
		pPlayer->SetRotation(rRot);

		// �m�ۂ����A�h���X��Ԃ�
		return pPlayer;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
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
//	�ďo���ݒ�̐ݒ菈��
//============================================================
void CPlayer::SetRespawn(D3DXVECTOR3& rPos)
{
	// �X�e�[�W�͈͊O�̕␳
	CSceneGame::GetStage()->LimitPosition(rPos, PLAY_RADIUS);

	// ���n����
	Land(rPos);

	// �����̈ʒu��ݒ�
	SetPosition(rPos);

	// �O�Ղ̏��������s����Ԃɂ���
	m_pOrbit->SetEnableInit(false);

	// �\������ݒ�ɂ���
	SetDisp(true);
}

//============================================================
//	�\���̐ݒ菈��
//============================================================
void CPlayer::SetDisp(const bool bDisp)
{
	if (bDisp)
	{ // �\�������Ԃ̏ꍇ

		// ��Ԃ�ݒ�
		m_state = STATE_FADEOUT;	// �t�F�[�h�A�E�g���

		// �e��`�悷��ݒ�ɂ���
		m_pShadow->SetEnableDraw(true);
	}
	else
	{ // �\�����Ȃ���Ԃ̏ꍇ

		// ��Ԃ�ݒ�
		m_state = STATE_FADEIN;		// �t�F�[�h�C�����

		// �e��`�悵�Ȃ��ݒ�ɂ���
		m_pShadow->SetEnableDraw(false);
	}
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
//	���x���擾����
//============================================================
int CPlayer::GetLevel(void) const
{
	// ���x����Ԃ�
	return m_pExp->GetLevel();
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
//	�ʏ��Ԏ��̍X�V����
//============================================================
CPlayer::MOTION CPlayer::UpdateNormal(void)
{
	// �ϐ���錾
	MOTION currentMotion = MOTION_MOVE;		// ���݂̃��[�V����
	D3DXVECTOR3 posPlayer = GetPosition();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetRotation();	// �v���C���[����

	// �ړ�����
	Move();

	// �����X�V
	Rot(rotPlayer);

	// �ʒu�X�V
	Pos(posPlayer);

	// �X�e�[�W�͈͊O�̕␳
	CSceneGame::GetStage()->LimitPosition(posPlayer, PLAY_RADIUS);

	// ���n����
	Land(posPlayer);

	// ���@���ˑ���
	currentMotion = Magic(currentMotion, posPlayer);

	// �����蔻��
	CollisionTarget(posPlayer);	// �^�[�Q�b�g
	CollisionEnemy(posPlayer);	// �G

	// �o���A�Ƃ̓����蔻��
	if (CSceneGame::GetStage()->CollisionBarrier(posPlayer, PLAY_RADIUS))
	{ // �������Ă����ꍇ

		// �q�b�g����
		Hit(BARRIER_DMG);

		// �o���A�̏Փ˔���
		collision::CirclePillar
		( // ����
			posPlayer,											// ����ʒu
			CSceneGame::GetStage()->GetStageBarrierPosition(),	// ����ڕW�ʒu
			PLAY_RADIUS,										// ���蔼�a
			CSceneGame::GetStage()->GetStageBarrier().fRadius	// ����ڕW���a
		);
	}

	// �ʒu���X�V
	SetPosition(posPlayer);

	// �������X�V
	SetRotation(rotPlayer);

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
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

	if (m_nCounterState < NORMAL_CNT)
	{ // �J�E���^�[�����l��菬�����ꍇ

		// �J�E���^�[�����Z
		m_nCounterState++;
	}
	else
	{ // �J�E���^�[�����l�ȏ�̏ꍇ

		// �J�E���^�[��������
		m_nCounterState = 0;

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
	Pos(posPlayer);

	// �^�[�Q�b�g�Ƃ̓����蔻��
	CollisionTarget(posPlayer);

	// �X�e�[�W�͈͊O�̕␳
	CSceneGame::GetStage()->LimitPosition(posPlayer, PLAY_RADIUS);

	// ���n����
	if (Land(posPlayer))
	{ // ���n���Ă����ꍇ

		if (GetMotionType() != MOTION_LANDING)
		{ // ���[�V���������n�ł͂Ȃ��ꍇ

			// ���n���[�V�����Ɉڍs
			SetMotion(MOTION_LANDING);

			// ��Ԃ�ݒ�
			m_state = STATE_DAMAGE;	// �_���[�W���
		}
	}

	// �ʒu�𔽉f
	SetPosition(posPlayer);
}

//============================================================
//	�t�F�[�h�A�E�g��Ԏ��̍X�V����
//============================================================
void CPlayer::UpdateFadeOut(void)
{
	// �ϐ���錾
	float fAlpha = GetAlpha();	// �����x

	// �����x���グ��
	fAlpha += FADE_LEVEL;

	if (fAlpha >= GetMaxAlpha())
	{ // �����x���オ��؂����ꍇ

		// �����x��␳
		fAlpha = GetMaxAlpha();

		// ��Ԃ�ݒ�
		m_state = STATE_NORMAL;	// �ʏ���
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

		// ��Ԃ�ݒ�
		m_state = STATE_NONE;	// �������Ȃ����
	}

	// �����x��ݒ�
	SetAlpha(fAlpha);
}

//============================================================
//	�ړ�����
//============================================================
CPlayer::MOTION CPlayer::Move(void)
{
	// �ϐ���錾
	D3DXVECTOR3 rot = CManager::GetCamera()->GetRotation();			// �J�����̌���
	CStage::AREA area = CSceneGame::GetStage()->GetAreaPlayer();	// �v���C���[�̌��݃G���A

	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetPad();		// �p�b�h

#if 1

#if 0

	if (pKeyboard->GetPress(DIK_W) || pPad->GetPressLStickY() > 0.0f)
	{
		// �ړ��ʂ��X�V
		m_move.x += sinf(rot.y) * 1.5f;
		m_move.z += cosf(rot.y) * 1.5f;

		// �ړ��ʂ��X�V
		m_move.x += sinf(rot.y - (D3DX_PI * 0.5f)) * (((int)area + 1) * (0.6f / (float)CStage::AREA_MAX));
		m_move.z += cosf(rot.y - (D3DX_PI * 0.5f)) * (((int)area + 1) * (0.6f / (float)CStage::AREA_MAX));

		// �ڕW�������X�V
		m_destRot.y = D3DXToRadian(180) + rot.y;
	}
	else if (pKeyboard->GetPress(DIK_S) || pPad->GetPressLStickY() < 0.0f)
	{
		// �ړ��ʂ��X�V
		m_move.x -= sinf(rot.y) * 1.5f;
		m_move.z -= cosf(rot.y) * 1.5f;

		// �ړ��ʂ��X�V
		m_move.x += sinf(rot.y - (D3DX_PI * 0.5f)) * (((int)area + 1) * (0.6f / (float)CStage::AREA_MAX));
		m_move.z += cosf(rot.y - (D3DX_PI * 0.5f)) * (((int)area + 1) * (0.6f / (float)CStage::AREA_MAX));

		// �ڕW�������X�V
		m_destRot.y = D3DXToRadian(0) + rot.y;
	}
	else
	{
		// �ړ��ʂ��X�V
		m_move.x += sinf(rot.y - (D3DX_PI * 0.5f)) * (((int)area + 1) * (MAX_SPEED_WIDTH / (float)CStage::AREA_MAX));
		m_move.z += cosf(rot.y - (D3DX_PI * 0.5f)) * (((int)area + 1) * (MAX_SPEED_WIDTH / (float)CStage::AREA_MAX));

		// �ڕW�������X�V
		m_destRot.y = D3DXToRadian(90) + rot.y;

		if (pKeyboard->GetPress(DIK_A) || pPad->GetPressLStickX() < 0.0f)
		{
			// �ړ��ʂ��X�V
			m_move.x += sinf(rot.y - (D3DX_PI * 0.5f)) * 1.2f;
			m_move.z += cosf(rot.y - (D3DX_PI * 0.5f)) * 1.2f;
		}
		else if (pKeyboard->GetPress(DIK_D) || pPad->GetPressLStickX() > 0.0f)
		{
			// �ړ��ʂ��X�V
			m_move.x -= sinf(rot.y - (D3DX_PI * 0.5f)) * ((((int)area + 1) * (MAX_SPEED_WIDTH / (float)CStage::AREA_MAX))) * 0.8f;
			m_move.z -= cosf(rot.y - (D3DX_PI * 0.5f)) * ((((int)area + 1) * (MAX_SPEED_WIDTH / (float)CStage::AREA_MAX))) * 0.8f;
		}
	}

#else

	// �ϐ���錾
	D3DXVECTOR3 vecTarg, vecSide;	// �^�[�Q�b�g�����x�N�g���E�������x�N�g��

	// �^�[�Q�b�g�����̃x�N�g�����v�Z
	vecTarg = CSceneGame::GetTarget()->GetPosition() - GetPosition();
	vecTarg.y = 0.0f;						// �x�N�g���̏c�����𖳎�
	D3DXVec3Normalize(&vecTarg, &vecTarg);	// �x�N�g���̐��K��

	// �������x�N�g�����v�Z
	vecSide = D3DXVECTOR3(vecTarg.z, 0.0f, -vecTarg.x);


	
	// �c�ړ��ʂ����Z
	m_move += vecTarg * 2.0f;

	if (pKeyboard->GetPress(DIK_W) || pPad->GetPress(CInputPad::KEY_L1))
	{
		m_move -= vecTarg * 3.0f;
	}

	// ���ړ��ʂ����Z
	m_move += vecSide * 1.5f;

	if (pKeyboard->GetPress(DIK_A) || pPad->GetPressLStickX() < 0.0f)
	{
		// �ړ��ʂ��X�V
		m_move += vecSide * 1.4f;
	}
	else if (pKeyboard->GetPress(DIK_D) || pPad->GetPressLStickX() > 0.0f)
	{
		// �ړ��ʂ��X�V
		m_move -= vecSide * 1.0;
	}

	// �ڕW�������X�V
	m_destRot.y = atan2f(m_move.x, m_move.z);
	m_destRot.y += D3DX_PI;

#endif

#else	// �f�o�b�O�p���s
	// �ړ�����
	if (pKeyboard->GetPress(DIK_W))
	{ // ���ړ��̑��삪�s��ꂽ�ꍇ

		if (pKeyboard->GetPress(DIK_A))
		{ // ���ړ��̑�����s��ꂽ�ꍇ (�����ړ�)

			// �ړ��ʂ��X�V
			m_move.x += sinf(rot.y - (D3DX_PI * 0.25f)) * 2.0f;
			m_move.z += cosf(rot.y - (D3DX_PI * 0.25f)) * 2.0f;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(135) + rot.y;
		}
		else if (pKeyboard->GetPress(DIK_D))
		{ // �E�ړ��̑�����s��ꂽ�ꍇ (�E���ړ�)

			// �ړ��ʂ��X�V
			m_move.x -= sinf(rot.y - (D3DX_PI * 0.75f)) * 2.0f;
			m_move.z -= cosf(rot.y - (D3DX_PI * 0.75f)) * 2.0f;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(225) + rot.y;
		}
		else
		{ // ���ړ��̑��삾�����s��ꂽ�ꍇ (���ړ�)

			// �ړ��ʂ��X�V
			m_move.x += sinf(rot.y) * 2.0f;
			m_move.z += cosf(rot.y) * 2.0f;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(180) + rot.y;
		}
	}
	else if (pKeyboard->GetPress(DIK_S))
	{ // ��O�ړ��̑��삪�s��ꂽ�ꍇ

		if (pKeyboard->GetPress(DIK_A))
		{ // ���ړ��̑�����s��ꂽ�ꍇ (����O�ړ�)

			// �ړ��ʂ��X�V
			m_move.x += sinf(rot.y - (D3DX_PI * 0.75f)) * 2.0f;
			m_move.z += cosf(rot.y - (D3DX_PI * 0.75f)) * 2.0f;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(45) + rot.y;
		}
		else if (pKeyboard->GetPress(DIK_D))
		{ // �E�ړ��̑�����s��ꂽ�ꍇ (�E��O�ړ�)

			// �ړ��ʂ��X�V
			m_move.x -= sinf(rot.y - (D3DX_PI * 0.25f)) * 2.0f;
			m_move.z -= cosf(rot.y - (D3DX_PI * 0.25f)) * 2.0f;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(315) + rot.y;
		}
		else
		{ // ��O�ړ��̑��삾�����s��ꂽ�ꍇ (��O�ړ�)

			// �ړ��ʂ��X�V
			m_move.x -= sinf(rot.y) * 2.0f;
			m_move.z -= cosf(rot.y) * 2.0f;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(0) + rot.y;
		}
	}
	else if (pKeyboard->GetPress(DIK_A))
	{ // ���ړ��̑��삪�s��ꂽ�ꍇ

		// �ړ��ʂ��X�V
		m_move.x += sinf(rot.y - (D3DX_PI * 0.5f)) * 2.0f;
		m_move.z += cosf(rot.y - (D3DX_PI * 0.5f)) * 2.0f;

		// �ڕW�������X�V
		m_destRot.y = D3DXToRadian(90) + rot.y;
	}
	else if (pKeyboard->GetPress(DIK_D))
	{ // �E�ړ��̑��삪�s��ꂽ�ꍇ

		// �ړ��ʂ��X�V
		m_move.x -= sinf(rot.y - (D3DX_PI * 0.5f)) * 2.0f;
		m_move.z -= cosf(rot.y - (D3DX_PI * 0.5f)) * 2.0f;

		// �ڕW�������X�V
		m_destRot.y = D3DXToRadian(270) + rot.y;
	}
#endif

	// ���s���[�V������Ԃ�
	return MOTION_MOVE;
}

//============================================================
//	���@����
//============================================================
CPlayer::MOTION CPlayer::Magic(MOTION motion, D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	MOTION currentMotion = motion;	// ���݂̃��[�V����

	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetPad();		// �p�b�h

	if (pKeyboard->GetPress(DIK_SPACE) || pPad->GetPress(CInputPad::KEY_R1))
	{
		// ���@�̔���
		m_pMagic->ShotMagic();

		// �A�N�V�������[�V������ݒ�
		currentMotion = MOTION_ACTION;
	}

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//============================================================
//	���n����
//============================================================
bool CPlayer::Land(D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	bool bLand = false;	// ���n��

	// ���n����
	if (CSceneGame::GetField()->LandPosition(rPos, m_move)
	||  CSceneGame::GetStage()->LandPosition(rPos, m_move, 0.0f))
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
//	���[�V��������
//============================================================
void CPlayer::Motion(int nMotion)
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
		case MOTION_ACTION:		// �A�N�V�������

			// �ҋ@���[�V�����Ɉڍs
			SetMotion(MOTION_MOVE);

			// �����𔲂���
			break;

		case MOTION_BLOW_AWAY:	// ������я��

			// 

			// �����𔲂���
			break;

		case MOTION_LANDING:	// ���n���

			// �ҋ@���[�V�����Ɉڍs
			SetMotion(MOTION_MOVE);

			// �����𔲂���
			break;
		}
	}
}

//============================================================
//	�ʒu����
//============================================================
void CPlayer::Pos(D3DXVECTOR3& rPos)
{
	// �d�͂����Z
	m_move.y -= PLAY_GRAVITY;

	// �ړ��ʂ����Z
	rPos += m_move;

	// �ړ��ʂ�����
	m_move.x += (0.0f - m_move.x) * PLAY_REV;
	m_move.z += (0.0f - m_move.z) * PLAY_REV;
}

//============================================================
//	��������
//============================================================
void CPlayer::Rot(D3DXVECTOR3& rRot)
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
	CTarget *pTarget = CSceneGame::GetTarget();	// �^�[�Q�b�g���

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
				if (collision::Circle2D
				( // ����
					rPos,						// ����ʒu
					pObjCheck->GetPosition(),	// ����ڕW�ʒu
					PLAY_RADIUS,				// ���蔼�a
					pObjCheck->GetRadius()		// ����ڕW���a
				))
				{ // �������Ă����ꍇ

					// �v���C���[�̃q�b�g����
					Hit(ENE_HIT_DMG);
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}
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

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(PLAYER_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			// �L�����N�^�[�̐ݒ�
			if (strcmp(&aString[0], "CHARACTERSET") == 0)
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
