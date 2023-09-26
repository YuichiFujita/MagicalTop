//============================================================
//
//	�G���� [enemy.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemy.h"
#include "manager.h"
#include "scene.h"
#include "sceneGame.h"
#include "renderer.h"
#include "sound.h"
#include "texture.h"
#include "multiModel.h"
#include "shadow.h"
#include "objectBillboard.h"
#include "enemyWay.h"
#include "bubble.h"
#include "collision.h"
#include "player.h"
#include "target.h"
#include "expOrb.h"
#include "bullet.h"
#include "stage.h"
#include "score.h"
#include "effect3D.h"
#include "particle3D.h"
#include "field.h"

//************************************************************
//	�}�N����`
//************************************************************
// �G�S�̃}�N��
#define ENEMY_SETUP_TXT	"data\\TXT\\enemy.txt"	// �Z�b�g�A�b�v�e�L�X�g���΃p�X

#define SIZE_WARNING	(D3DXVECTOR3(160.0f, 160.0f, 0.0f))	// �x���\���̑傫��
#define WARNING_PRIO	(4)	// �x���\���̗D�揇��

#define ENE_REV				(0.02f)		// �G�̈ړ��ʂ̌����W��
#define ENE_GRAVITY			(1.0f)		// �G�̏d��
#define ENE_DMG_FRAME		(20)		// �G�̃_���[�W��ԃt���[��
#define ENE_WAR_ADD_POSY	(100.0f)	// �x���\���ʒu y�̉��Z��
#define ENE_BULL_LIMIT_Y	(0.1f)		// �e�̈ړ������̐����l
#define ENE_DEATH_ADD_POSY	(6.0f)		// ���S����Y���W�㏸��
#define ENE_DEATH_POSY		(1300.0f)	// ���S����Y���W
#define ENE_SUB_ALPHA		(0.02f)		// �������̃��l���Z��

#define DAMAGE_CNT		(10)		// �_���[�W��Ԉێ��J�E���g
#define BUBBLE_SUB_CNT	(420)		// �o�u�������J�E���g
#define LIMIT_RADIUS	(2400.0f)	// �G�̃X�e�[�W�ړ����a

#define MAGIC_EFF_CNT			(2)		// �G�t�F�N�g�̐����t���[��
#define MAGIC_EFF_MUL_POS		(2.0f)	// �G�t�F�N�g�̐����ʒu�̏�Z��
#define MAGIC_EFF_MUL_MOVE		(1.0f)	// �G�t�F�N�g�̈ړ��ʂ̏�Z��
#define MAGIC_EFF_ALPHA			(0.6f)	// �G�t�F�N�g�̓����x
#define MAGIC_EFF_LIFE			(45)	// �G�t�F�N�g�̎���
#define MAGIC_EFF_MUL_RADIUS	(1.5f)	// �G�t�F�N�g�̔��a�̏�Z��
#define MAGIC_EFF_SUB_RADIUS	(0.2f)	// �G�t�F�N�g�̔��a�̌��Z��
#define MAGIC_EFF_PRIORITY		(3)		// �G�t�F�N�g�̗D�揇��

// �����}�N��
#define HUMAN_FALL	(5.5f)	// �����������̗������x

// ��ԃ}�N��
#define ENECAR_REV_MUL		(0.03f)	// ��C�̌����␳�l
#define ENECAR_SHOTRANGE	(0.25f)	// ��C�ˌ����̋��e���������̌덷��

#define SPAWN_WAIT_CNT	(40)	// ��ԃX�|�[�����ҋ@�J�E���g

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
// �G�S�̃����o�ϐ�
CEnemy::StatusInfo CEnemy::m_aStatusInfo[CEnemy::TYPE_MAX] = {};	// �X�e�[�^�X���
CEnemy::PartsInfo CEnemy::m_aPartsInfo[CEnemy::TYPE_MAX] = {};		// �p�[�c���
CMotion::Info CEnemy::m_aMotionInfo[CEnemy::TYPE_MAX] = {};			// ���[�V�������
int CEnemy::m_nNumAll = 0;											// �G�̑���

const char *CEnemy::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\warning000.png",	// �x���\���e�N�X�`��
};

// ���������o�ϐ�
const char *CEnemyHuman::mc_apModelFile[] =	// �������f���萔
{
	"data\\MODEL\\ENEMY\\HUMAN\\00_waist.x",				// ��
	"data\\MODEL\\ENEMY\\HUMAN\\01_body.x",					// ��
	"data\\MODEL\\ENEMY\\HUMAN\\02_head.x",					// ��
	"data\\MODEL\\ENEMY\\HUMAN\\03_armUL.x",				// ����r
	"data\\MODEL\\ENEMY\\HUMAN\\04_armUR.x",				// �E��r
	"data\\MODEL\\ENEMY\\HUMAN\\05_armDL.x",				// �����r
	"data\\MODEL\\ENEMY\\HUMAN\\06_armDR.x",				// �E���r
	"data\\MODEL\\ENEMY\\HUMAN\\07_handL.x",				// ����
	"data\\MODEL\\ENEMY\\HUMAN\\08_handR.x",				// �E��
	"data\\MODEL\\ENEMY\\HUMAN\\09_legUL.x",				// ��������
	"data\\MODEL\\ENEMY\\HUMAN\\10_legUR.x",				// �E������
	"data\\MODEL\\ENEMY\\HUMAN\\11_legDL.x",				// ����
	"data\\MODEL\\ENEMY\\HUMAN\\12_legDR.x",				// �E��
	"data\\MODEL\\ENEMY\\HUMAN\\13_footL.x",				// ����
	"data\\MODEL\\ENEMY\\HUMAN\\14_footR.x",				// �E��
	"data\\MODEL\\ENEMY\\HUMAN\\15_waistpouchBack.x",		// �E�G�X�g�|�[�`(��)
	"data\\MODEL\\ENEMY\\HUMAN\\16_waistpouchRight.x",		// �E�G�X�g�|�[�`(��)
	"data\\MODEL\\ENEMY\\HUMAN\\17_waistpouchBackright.x",	// �E�G�X�g�|�[�`(����)
	"data\\MODEL\\ENEMY\\HUMAN\\18_legURpouch.x",			// �E�������|�[�`
	"data\\MODEL\\ENEMY\\HUMAN\\19_armor.x",				// �A�[�}�[
	"data\\MODEL\\ENEMY\\HUMAN\\20_knife.x",				// �i�C�t
	"data\\MODEL\\ENEMY\\HUMAN\\21_assault.x",				// �e
};

// ��ԃ����o�ϐ�
const char *CEnemyCar::mc_apModelFile[] =	// ��ԃ��f���萔
{
	"data\\MODEL\\ENEMY\\CAR\\caterpillar000.x",	// �L���^�s��
	"data\\MODEL\\ENEMY\\CAR\\cannon000.x",			// �L���m��
};

//************************************************************
//	�q�N���X [CEnemy] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemy::CEnemy(const TYPE type) : CObjectChara(CObject::LABEL_ENEMY), m_type(type), m_status(m_aStatusInfo[type]), m_parts(m_aPartsInfo[type]), m_motion(m_aMotionInfo[type])
{
	// �����o�ϐ����N���A
	m_pShadow	= NULL;			// �e�̏��
	m_pWarning	= NULL;			// �x���̏��
	m_pWay		= NULL;			// �����̏��
	m_pBubble	= NULL;			// �o�u���̏��
	m_oldPos	= VEC3_ZERO;	// �ߋ��ʒu
	m_movePos	= VEC3_ZERO;	// �ʒu�ړ���
	m_moveKnock	= VEC3_ZERO;	// �m�b�N�o�b�N�ړ���
	m_moveRot	= VEC3_ZERO;	// �����ύX��
	m_deathMoveRot		= VEC3_ZERO;	// ���S���̌����ύX��
	m_state				= STATE_SPAWN;	// ���
	m_nCounterEffect	= 0;			// �G�t�F�N�g�Ǘ��J�E���^�[
	m_nCounterBubble	= 0;			// �o�u���Ǘ��J�E���^�[
	m_nCounterState		= 0;			// ��ԊǗ��J�E���^�[
	m_nCounterAtk		= 0;			// �U���Ǘ��J�E���^�[

	// �G�̑��������Z
	m_nNumAll++;
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemy::~CEnemy()
{
	// �G�̑��������Z
	m_nNumAll--;
}

//============================================================
//	����������
//============================================================
HRESULT CEnemy::Init(void)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// �����o�ϐ���������
	m_pShadow	= NULL;			// �e�̏��
	m_pWarning	= NULL;			// �x���̏��
	m_pWay		= NULL;			// �����̏��
	m_pBubble	= NULL;			// �o�u���̏��
	m_oldPos	= VEC3_ZERO;	// �ߋ��ʒu
	m_movePos	= VEC3_ZERO;	// �ʒu�ړ���
	m_moveKnock	= VEC3_ZERO;	// �m�b�N�o�b�N�ړ���
	m_moveRot	= VEC3_ZERO;	// �����ύX��
	m_state		= STATE_SPAWN;	// ���
	m_nCounterEffect	= 0;	// �G�t�F�N�g�Ǘ��J�E���^�[
	m_nCounterBubble	= 0;	// �o�u���Ǘ��J�E���^�[
	m_nCounterState		= 0;	// ��ԊǗ��J�E���^�[
	m_nCounterAtk		= 0;	// �U���Ǘ��J�E���^�[

	// ���S���̌����ύX��
	m_deathMoveRot.x += (rand() % 31 - 15) * 0.001f;
	m_deathMoveRot.y += (rand() % 31 - 15) * 0.001f;
	m_deathMoveRot.z += (rand() % 31 - 15) * 0.001f;

	//--------------------------------------------------------
	//	�e�̐���
	//--------------------------------------------------------
	// �e�̐���
	m_pShadow = CShadow::Create(CShadow::TEXTURE_NORMAL, D3DXVECTOR3(m_status.fShadowRadius, 0.0f, m_status.fShadowRadius), this);
	if (UNUSED(m_pShadow))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	�x���\���̐����E�ݒ�
	//--------------------------------------------------------
	// �x���\���̐���
	m_pWarning = CObjectBillboard::Create(VEC3_ZERO, SIZE_WARNING);
	if (UNUSED(m_pWarning))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pWarning->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_WARNING]));

	// �D�揇�ʂ�ݒ�
	m_pWarning->SetPriority(WARNING_PRIO);

	//--------------------------------------------------------
	//	�����\���̐���
	//--------------------------------------------------------
	// �����\���̐���
	m_pWay = CEnemyWay::Create(this, LIMIT_RADIUS - m_aStatusInfo[m_type].fRadius);
	if (UNUSED(m_pWay))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	�o�u���̐���
	//--------------------------------------------------------
	// �o�u���𐶐�
	m_pBubble = CBubble::Create(this, m_status.nLife, m_status.bubbleSize, VEC3_ZERO, m_status.fHeight * 0.5f);
	if (UNUSED(m_pBubble))
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

	for (int nCntEnemy = 0; nCntEnemy < m_parts.nNumParts; nCntEnemy++)
	{ // �p�[�c�����J��Ԃ�

		// �p�[�c���̐ݒ�
		CObjectChara::SetPartsInfo
		( // ����
			nCntEnemy,							// �p�[�c�C���f�b�N�X
			m_parts.aInfo[nCntEnemy].nParentID,	// �e�C���f�b�N�X
			m_parts.aInfo[nCntEnemy].pos,		// �ʒu
			m_parts.aInfo[nCntEnemy].rot,		// ����
			GetModelFileName(nCntEnemy)			// �t�@�C����
		);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEnemy::Uninit(void)
{
	// �����\�����폜
	m_pWay->Delete();

	// �e�̏I��
	m_pShadow->Uninit();

	// �x���\���̏I��
	m_pWarning->Uninit();

	// �o�u���̏I��
	m_pBubble->Uninit();

	// �I�u�W�F�N�g�L�����N�^�[�̏I��
	CObjectChara::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemy::Update(void)
{
	// �e�̍X�V
	m_pShadow->Update();

	// �x���\���̍X�V
	m_pWarning->Update();

	// �o�u���̍X�V
	m_pBubble->Update();

	// �I�u�W�F�N�g�L�����N�^�[�̍X�V
	CObjectChara::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CEnemy::Draw(void)
{
	// �I�u�W�F�N�g�L�����N�^�[�̕`��
	CObjectChara::Draw();
}

//============================================================
//	�q�b�g����
//============================================================
void CEnemy::Hit(const int nDmg)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = GetPosition();	// �G�ʒu

	if (IsDeath() != true && m_state == STATE_NORMAL)
	{ // ���S�t���O�������Ă��Ȃ����A�ʏ��Ԃ̏ꍇ

		// ���x�������Z
		m_pBubble->AddLevel(nDmg);

		if (m_pBubble->GetLevel() < m_status.nLife)
		{ // �����Ă���ꍇ

			// �J�E���^�[��������
			m_nCounterState = 0;
			m_nCounterBubble = 0;

			// ��Ԃ�ݒ�
			m_state = STATE_DAMAGE;	// �_���[�W���
		}
		else
		{ // ����ł���ꍇ

			// ���[�V�������X�V���Ȃ���Ԃɂ���
			SetEnableMotionUpdate(false);

			// �����\�����폜
			m_pWay->Delete();

			// ��Ԃ�ݒ�
			m_state = STATE_DEATH;	// ���S���

			// �T�E���h�̍Đ�
			CManager::GetSound()->Play(CSound::LABEL_SE_FLY);	// ���V��
		}

		// �p�[�e�B�N��3D�I�u�W�F�N�g�𐶐�
		CParticle3D::Create(CParticle3D::TYPE_BUBBLE_EXPLOSION, pos);
	}
}

//============================================================
//	�m�b�N�o�b�N�q�b�g����
//============================================================
void CEnemy::HitKnockBack(const int nDmg, const D3DXVECTOR3& vec)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = GetPosition();	// �G�ʒu
	D3DXVECTOR3 vecKnock = vec;			// �m�b�N�o�b�N�x�N�g��

	if (IsDeath() != true && m_state == STATE_NORMAL)
	{ // ���S�t���O�������Ă��Ȃ����A�ʏ��Ԃ̏ꍇ

		// ���x�������Z
		m_pBubble->AddLevel(nDmg);

		if (m_pBubble->GetLevel() < m_status.nLife)
		{ // �����Ă���ꍇ

			// �m�b�N�o�b�N�ړ��ʂ�ݒ�
			vecKnock.y = 0.0f;									// �c�x�N�g����������
			D3DXVec3Normalize(&vecKnock, &vecKnock);			// �x�N�g���𐳋K��
			m_moveKnock = vecKnock * m_status.fKnockBackSide;	// �m�b�N�o�b�N���ړ��ʂ��v�Z
			m_moveKnock.y = m_status.fKnockBackUp;				// �m�b�N�o�b�N�c�ړ��ʂ���

			// �J�E���^�[��������
			m_nCounterState = 0;
			m_nCounterBubble = 0;

			// ��Ԃ�ݒ�
			m_state = STATE_DAMAGE;	// �_���[�W���
		}
		else
		{ // ����ł���ꍇ

			// ���[�V�������X�V���Ȃ���Ԃɂ���
			SetEnableMotionUpdate(false);

			// �����\�����폜
			m_pWay->Delete();

			// ��Ԃ�ݒ�
			m_state = STATE_DEATH;	// ���S���

			// �T�E���h�̍Đ�
			CManager::GetSound()->Play(CSound::LABEL_SE_FLY);	// ���V��
		}

		// �p�[�e�B�N��3D�I�u�W�F�N�g�𐶐�
		CParticle3D::Create(CParticle3D::TYPE_BUBBLE_EXPLOSION, pos);
	}
}

//============================================================
//	��������
//============================================================
CEnemy *CEnemy::Create(const TYPE type, const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot)
{
	// �|�C���^��錾
	CEnemy *pEnemy = NULL;	// �G�����p

	if (UNUSED(pEnemy))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		switch (type)
		{ // ��ނ��Ƃ̏���
		case TYPE_HUMAN:	// ���

			// �����𐶐�
			pEnemy = new CEnemyHuman(type);

			break;

		case TYPE_CAR:	// ���

			// ��Ԃ𐶐�
			pEnemy = new CEnemyCar(type);

			break;

		default:	// ��O����
			assert(false);
			break;
		}
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pEnemy))
	{ // �g�p����Ă���ꍇ
		
		// �G�̏�����
		if (FAILED(pEnemy->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pEnemy;
			pEnemy = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		pEnemy->SetPosition(rPos);

		// ������ݒ�
		pEnemy->SetRotation(rRot);

		// �m�ۂ����A�h���X��Ԃ�
		return pEnemy;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�����_����������
//============================================================
void CEnemy::RandomSpawn
(
	const int nNum,	// ������
	const TYPE type	// ���
)
{
	// �ϐ���錾
	D3DXVECTOR3 pos, rot;	// �ʒu�E�����ݒ�p
	D3DXVECTOR3 posTarget;	// �^�[�Q�b�g�ʒu

	// �|�C���^��錾
	CTarget *pTarget = CScene::GetTarget();	// �^�[�Q�b�g���

	if (USED(CScene::GetTarget()))
	{ // �^�[�Q�b�g���g�p����Ă���ꍇ

		for (int nCntGrow = 0; nCntGrow < nNum; nCntGrow++)
		{ // ���������J��Ԃ�

			// �^�[�Q�b�g�ʒu���擾
			posTarget = pTarget->GetPosition();

			// �����ʒu��ݒ�
			pos.x = (float)(rand() % ((int)LIMIT_RADIUS * 2) - (int)LIMIT_RADIUS + 1);
			pos.y = m_aStatusInfo[type].fSpawnHeight;
			pos.z = (float)(rand() % ((int)LIMIT_RADIUS * 2) - (int)LIMIT_RADIUS + 1);

			// ����������ݒ�
			rot = VEC3_ZERO;	// ������������
			rot.y = atan2f(pos.x - posTarget.x, pos.z - posTarget.z);	// �^�[�Q�b�g�̕�������������

			// �ʒu��␳
			pos.x = sinf(rot.y) * (LIMIT_RADIUS - m_aStatusInfo[type].fRadius);
			pos.z = cosf(rot.y) * (LIMIT_RADIUS - m_aStatusInfo[type].fRadius);

			// �G�I�u�W�F�N�g�̐���
			CEnemy::Create(type, pos, rot);
		}
	}
}

//============================================================
//	�X�e�[�^�X���擾����
//============================================================
CEnemy::StatusInfo CEnemy::GetStatusInfo(const int nType)
{
	// �����̎�ނ̃X�e�[�^�X����Ԃ�
	return m_aStatusInfo[nType];
}

//============================================================
//	�S��������
//============================================================
void CEnemy::SetAllVanish(void)
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

				if (pObjCheck->GetLabel() == CObject::LABEL_ENEMY)
				{ // �I�u�W�F�N�g���x�����G�������ꍇ

					// ��Ԃ�ݒ�
					pObjCheck->SetState(STATE_VANISH);	// �������
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}
}

//============================================================
//	�����擾����
//============================================================
int CEnemy::GetNumAll(void)
{
	// ���݂̓G�̑�����Ԃ�
	return m_nNumAll;
}

//============================================================
//	�ߋ��ʒu�̍X�V����
//============================================================
void CEnemy::UpdateOldPosition(void)
{
	// �ߋ��ʒu���X�V
	m_oldPos = GetPosition();
}

//============================================================
//	�m�b�N�o�b�N�̍X�V����
//============================================================
void CEnemy::UpdateKnockBack(D3DXVECTOR3& rPos)
{
	// �m�b�N�o�b�N���ʒu�ɔ��f
	rPos += m_moveKnock;

	// �m�b�N�o�b�N�ɏd�͂����Z
	m_moveKnock.y -= ENE_GRAVITY;

	// �m�b�N�o�b�N������
	m_moveKnock.x += (0.0f - m_moveKnock.x) * m_status.fKnockBackRevision;
	m_moveKnock.z += (0.0f - m_moveKnock.z) * m_status.fKnockBackRevision;
}

//============================================================
//	�ʒu�ړ��ʂ̐ݒ菈��
//============================================================
void CEnemy::SetMovePosition(const D3DXVECTOR3& rMove)
{
	// �����̈ʒu�ړ��ʂ�ݒ�
	m_movePos = rMove;
}

//============================================================
//	�����ύX�ʂ̐ݒ菈��
//============================================================
void CEnemy::SetMoveRotation(const D3DXVECTOR3& rMove)
{
	// �����̌�����ݒ�
	m_moveRot = rMove;

	// �����̐��K��
	useful::NormalizeRot(m_moveRot.x);
	useful::NormalizeRot(m_moveRot.y);
	useful::NormalizeRot(m_moveRot.z);
}

//============================================================
//	��Ԃ̐ݒ菈��
//============================================================
void CEnemy::SetState(const int nState)
{
	// �����̏�Ԃ�ݒ�
	m_state = (STATE)nState;
}

//============================================================
//	�}�g���b�N�X�擾����
//============================================================
D3DXMATRIX CEnemy::GetMtxWorld(void) const
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans, mtxWorld;	// �v�Z�p�}�g���b�N�X
	D3DXVECTOR3 posEnemy = GetPosition();	// �G�ʒu
	D3DXVECTOR3 rotEnemy = GetRotation();	// �G����

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotEnemy.y, rotEnemy.x, rotEnemy.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, posEnemy.x, posEnemy.y, posEnemy.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X��Ԃ�
	return mtxWorld;
}

//============================================================
//	�ߋ��ʒu�擾����
//============================================================
D3DXVECTOR3 CEnemy::GetOldPosition(void) const
{
	// �ߋ��ʒu��Ԃ�
	return m_oldPos;
}

//============================================================
//	�ʒu�ړ��ʎ擾����
//============================================================
D3DXVECTOR3 CEnemy::GetMovePosition(void) const
{
	// �ʒu�ړ��ʂ�Ԃ�
	return m_movePos;
}

//============================================================
//	�����ύX�ʎ擾����
//============================================================
D3DXVECTOR3 CEnemy::GetMoveRotation(void) const
{
	// �����ύX�ʂ�Ԃ�
	return m_moveRot;
}

//============================================================
//	��Ԏ擾����
//============================================================
int CEnemy::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//============================================================
//	��ގ擾����
//============================================================
int CEnemy::GetType(void) const
{
	// ��ނ�Ԃ�
	return m_type;
}

//============================================================
//	���a�擾����
//============================================================
float CEnemy::GetRadius(void) const
{
	// �ϐ���錾
	float fRadius = 0.0f;	// ���a

	if (m_status.fCollRadius < m_pBubble->GetRadius())
	{ // �����蔻��p���a���o�u���̔��a��菬�����ꍇ

		// �o�u���̔��a��ݒ�
		fRadius = m_pBubble->GetRadius();
	}
	else
	{ // �����蔻��p���a���o�u���̔��a���傫���ꍇ

		// �����蔻��p���a��ݒ�
		fRadius = m_status.fCollRadius;
	}

	// ���a��Ԃ�
	return fRadius;
}

//============================================================
//	�c���擾����
//============================================================
float CEnemy::GetHeight(void) const
{
	// �c����Ԃ�
	return m_status.fHeight;
}

//============================================================
//	�X�e�[�^�X���擾����
//============================================================
CEnemy::StatusInfo CEnemy::GetStatusInfo(void) const
{
	// �X�e�[�^�X����Ԃ�
	return m_status;
}

//============================================================
//	�p�[�c���擾����
//============================================================
CEnemy::PartsInfo CEnemy::GetPartsInfo(void) const
{
	// �p�[�c����Ԃ�
	return m_parts;
}

//============================================================
//	���[�V�������擾����
//============================================================
CMotion::Info CEnemy::GetMotionInfo(void) const
{
	// ���[�V��������Ԃ�
	return m_motion;
}

//============================================================
//	�X�|�[������
//============================================================
void CEnemy::Spawn(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posEnemy = GetPosition();		// �G�ʒu
	D3DXVECTOR3 moveEnemy = GetMovePosition();	// �G�ړ���
	D3DXVECTOR3 posWarning = posEnemy;			// �x���ʒu
	D3DXVECTOR3 posBarrier = CScene::GetStage()->GetStageBarrierPosition();	// �o���A�ʒu

	// �ߋ��ʒu�̍X�V
	UpdateOldPosition();

	// �X�|�[����Ԃ̓G�Ƃ̓����蔻��
	CollisionSpawnEnemy(posEnemy);

	// �X�e�[�W�͈͊O�̕␳
	collision::InCirclePillar(posEnemy, posBarrier, m_status.fRadius, LIMIT_RADIUS);

	// �d�͂����Z
	moveEnemy.y -= ENE_GRAVITY;

	// �ړ��ʂ����Z
	posEnemy += moveEnemy;

	// ���n����
	if (CScene::GetField()->LandPosition(posEnemy, moveEnemy))
	{ // ���n�����ꍇ

		// �`������Ȃ��ݒ�ɂ���
		m_pWarning->SetEnableDraw(false);

		// ��Ԃ�ݒ�
		m_state = STATE_NORMAL;	// �ʏ���
	}

	// �ʒu�𔽉f
	SetPosition(posEnemy);

	// �ʒu�ړ��ʂ𔽉f
	SetMovePosition(moveEnemy);

	// �x���ʒu��n�`�ɓY�킹��
	posWarning.y = CScene::GetField()->GetPositionHeight(posWarning);

	// �x���\���ʒu��ݒ�
	m_pWarning->SetPosition(D3DXVECTOR3(posWarning.x, posWarning.y + ENE_WAR_ADD_POSY, posWarning.z));
}

//============================================================
//	�_���[�W����
//============================================================
void CEnemy::Damage(void)
{
	if (m_nCounterState < DAMAGE_CNT)
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

		// ��Ԃ�ݒ�
		m_state = STATE_NORMAL;	// �ʏ���
	}
}

//============================================================
//	���S����
//============================================================
bool CEnemy::Death(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posEnemy = GetPosition();	// �G�ʒu
	float fBonus = CScene::GetPlayer()->GetStatusInfo((CPlayer::LEVEL)CScene::GetPlayer()->GetLevelStatus(CPlayer::LEVELINFO_EXP_UP)).fMulExp;	// �o���l�{�[�i�X��

	// ��Ɉʒu�𓮂���
	posEnemy.y += ENE_DEATH_ADD_POSY;

	// �ʒu�𔽉f
	SetPosition(posEnemy);

	if (m_nCounterEffect < MAGIC_EFF_CNT)
	{ // �J�E���^�[�����l��菬�����ꍇ

		// �J�E���^�[�����Z
		m_nCounterEffect++;
	}
	else
	{ // �J�E���^�[�����l�ȏ�̏ꍇ

		// �ϐ���錾
		D3DXVECTOR3 vecRandom;	// �����_���x�N�g��

		// �|�C���^��錾
		CEffect3D *pEffect3D;	// �G�t�F�N�g���

		// �����_���x�N�g����ݒ�
		vecRandom.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vecRandom.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vecRandom.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �G�t�F�N�g3D�𐶐�
		pEffect3D = CEffect3D::Create
		( // ����
			CEffect3D::TYPE_BUBBLE,							// �e�N�X�`��
			posEnemy + (vecRandom * MAGIC_EFF_MUL_POS),		// �ʒu
			vecRandom * MAGIC_EFF_MUL_MOVE,					// �ړ���
			VEC3_ZERO,										// ����
			D3DXCOLOR(1.0f, 1.0f, 1.0f, MAGIC_EFF_ALPHA),	// �F
			MAGIC_EFF_LIFE,									// ����
			m_pBubble->GetRadius() * MAGIC_EFF_MUL_RADIUS,	// ���a
			MAGIC_EFF_SUB_RADIUS							// ���a�̌��Z��
		);

		// �D�揇�ʂ�ݒ�
		pEffect3D->SetPriority(MAGIC_EFF_PRIORITY);

		// �J�E���^�[��������
		m_nCounterEffect = 0;
	}

	if (posEnemy.y > ENE_DEATH_POSY)
	{ // ���W����ɍs���؂����ꍇ

		if (CManager::GetScene()->GetMode() == CScene::MODE_GAME)
		{ // ���[�h���Q�[���̏ꍇ

			// �X�R�A�����Z
			CSceneGame::GetScore()->Add(m_status.nScore);
		}

		// �o���l�̃����_������
		CExpOrb::RandomSpawn((int)(m_status.nExp * fBonus), posEnemy);

		// �G�I�u�W�F�N�g�̏I��
		Uninit();

		// �T�E���h�̍Đ�
		CManager::GetSound()->Play(CSound::LABEL_SE_SPAWN);	// ������

		// ���S��Ԃ�
		return true;
	}

	// �����S��Ԃ�
	return false;
}

//============================================================
//	��������
//============================================================
bool CEnemy::Vanish(void)
{
	// �ϐ���錾
	float fAlpha = GetAlpha();	// �����x

	if (fAlpha <= 0.0f)
	{ // �����x��������؂����ꍇ

		// �G�I�u�W�F�N�g�̏I��
		Uninit();

		// ������Ԃ�
		return true;
	}

	// �����x��������
	fAlpha -= ENE_SUB_ALPHA;

	// �����x��ݒ�
	SetAlpha(fAlpha);

	// ��������Ԃ�
	return false;
}

//============================================================
//	�Ώێ��F����
//============================================================
void CEnemy::Look(const D3DXVECTOR3& rPosLook, const D3DXVECTOR3& rPosEnemy, D3DXVECTOR3& rRotEnemy)
{
	// �ϐ���錾
	float fDestRot = 0.0f;	// �ڕW����
	float fDiffRot = 0.0f;	// ����

	// ���F�Ώۂ̕�������
	fDestRot = atan2f(rPosEnemy.x - rPosLook.x, rPosEnemy.z - rPosLook.z);	// �ڕW����

	// �������������߂�
	fDiffRot = fDestRot - rRotEnemy.y;
	useful::NormalizeRot(fDiffRot);		// ���������̐��K��

	// �����̍X�V
	rRotEnemy.y += fDiffRot * m_status.fLookRevision;
	useful::NormalizeRot(rRotEnemy.y);	// �����̐��K��
}

//============================================================
//	�U������
//============================================================
void CEnemy::Attack(const D3DXVECTOR3& rLookPos, const D3DXVECTOR3& rThisPos, const float fLookRadius)
{
	// �J�E���^�[�����Z
	m_nCounterAtk++;

	if (m_nCounterAtk >= m_status.nCounterAttack)
	{ // �J�E���^�[�����l�ȏ�̏ꍇ

		// �ϐ���錾
		D3DXVECTOR3 posBull;			// ���ˈʒu
		D3DXVECTOR3 vecBull;			// ���˕���
		D3DXMATRIX  mtxTrans, mtxWorld;	// �v�Z�p�}�g���b�N�X

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxWorld);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_status.bullPos.x, m_status.bullPos.y, m_status.bullPos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		// �}�g���b�N�X���|�����킹��
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &GetMultiModel(m_status.nBullParts)->GetMtxWorld());

		// �}�g���b�N�X����ʒu�����߂�
		posBull = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);

		// �e�̈ړ����������߂�
		vecBull = rLookPos - posBull;
		D3DXVec3Normalize(&vecBull, &vecBull);								// �ړ������𐳋K��
		useful::LimitNum(vecBull.y, -ENE_BULL_LIMIT_Y, ENE_BULL_LIMIT_Y);	// �ړ�����Y�𐧌�

		if (!collision::Circle2D(rThisPos, rLookPos, m_status.fRadius, fLookRadius))
		{ // ���ˈʒu�������ɕW�I�������Ă��Ȃ��ꍇ

			// �e�I�u�W�F�N�g�̐���
			CBullet::Create
			( // ����
				CBullet::TYPE_ENEMY,			// ���
				posBull,						// �ʒu
				VEC3_ALL(m_status.fBullRadius),	// �傫��
				XCOL_WHITE,						// �F
				vecBull,						// �ړ�����
				m_status.fBullMove,				// �ړ����x
				m_status.nBullLife,				// ����
				m_status.nBullDamage			// �U����
			);

			// �J�E���^�[��������
			m_nCounterAtk = 0;

			switch (m_type)
			{ // ��ނ��Ƃ̏���
			case TYPE_HUMAN:	// ����

				// �p�[�e�B�N��3D�I�u�W�F�N�g�̐���
				CParticle3D::Create(CParticle3D::TYPE_MUZZLE_FLASH, posBull);

				// �T�E���h�̍Đ�
				CManager::GetSound()->Play(CSound::LABEL_SE_ENEMY_SHOT000);	// �G�ˌ��� (�e)

				break;

			case TYPE_CAR:		// ���

				// �p�[�e�B�N��3D�I�u�W�F�N�g�̐���
				CParticle3D::Create(CParticle3D::TYPE_TANK_FIRE, posBull);

				// �T�E���h�̍Đ�
				CManager::GetSound()->Play(CSound::LABEL_SE_ENEMY_SHOT001);	// �G�ˌ��� (��C)

				break;

			default:	// ��O����
				assert(false);
				break;
			}
		}
	}
}

//============================================================
//	�^�[�Q�b�g�Ƃ̓����蔻��
//============================================================
void CEnemy::CollisionTarget(D3DXVECTOR3& rPos)
{
	// �|�C���^��錾
	CTarget *pTarget = CScene::GetTarget();	// �^�[�Q�b�g���

	if (USED(pTarget))
	{ // �^�[�Q�b�g���g�p����Ă���ꍇ

		// �^�[�Q�b�g�Ƃ̏Փ˔���
		collision::CirclePillar
		( // ����
			rPos,	// ����ʒu
			pTarget->GetPosition(),	// ����ڕW�ʒu
			m_status.fRadius,		// ���蔼�a
			pTarget->GetRadius() + m_status.fRadius	// ����ڕW���a
		);
	}
}

//============================================================
//	�X�|�[�����̓G�Ƃ̓����蔻��
//============================================================
void CEnemy::CollisionSpawnEnemy(D3DXVECTOR3& rPos)
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

				if (pObjCheck->GetLabel() != CObject::LABEL_ENEMY || pObjCheck == GetObject())
				{ // �I�u�W�F�N�g���x�����G�ł͂Ȃ��A�܂��̓I�u�W�F�N�g���������g�������ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				if (pObjCheck->GetState() != CEnemy::STATE_SPAWN)
				{ // �G�̏�Ԃ��X�|�[����Ԃł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// �^�[�Q�b�g�Ƃ̏Փ˔���
				collision::CirclePillar
				( // ����
					rPos,						// ����ʒu
					pObjCheck->GetPosition(),	// ����ڕW�ʒu
					m_status.fRadius,			// ���蔼�a
					CEnemy::GetStatusInfo(pObjCheck->GetType()).fRadius	// ����ڕW���a
				);

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}
}

//============================================================
//	�ʏ펞�̓G�Ƃ̓����蔻��
//============================================================
void CEnemy::CollisionNormalEnemy(D3DXVECTOR3& rPos)
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

				if (pObjCheck->GetLabel() != CObject::LABEL_ENEMY || pObjCheck == GetObject())
				{ // �I�u�W�F�N�g���x�����G�ł͂Ȃ��A�܂��̓I�u�W�F�N�g���������g�������ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				if (pObjCheck->GetState() != CEnemy::STATE_NORMAL
				&&  pObjCheck->GetState() != CEnemy::STATE_DAMAGE)
				{ // �G�̏�Ԃ��ʏ�E�_���[�W��Ԃł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// �^�[�Q�b�g�Ƃ̏Փ˔���
				collision::CirclePillar
				( // ����
					rPos,						// ����ʒu
					pObjCheck->GetPosition(),	// ����ڕW�ʒu
					m_status.fRadius,			// ���蔼�a
					CEnemy::GetStatusInfo(pObjCheck->GetType()).fRadius	// ����ڕW���a
				);

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}
}

//============================================================
//	�x���̕`��󋵂̐ݒ菈��
//============================================================
void CEnemy::SetDrawWarning(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	m_pWarning->SetEnableDraw(bDraw);
}

//============================================================
//	�x���̈ʒu�̐ݒ菈��
//============================================================
void CEnemy::SetPositionWarning(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pWarning->SetPosition(rPos);
}

//============================================================
//	�����_����]����
//============================================================
void CEnemy::RandomRotation(void)
{
	// �ϐ���錾
	D3DXVECTOR3 rotEnemy = GetPartsRotation(0);	// �G�̐e�p�[�c����

	// ���������Z
	rotEnemy += m_deathMoveRot;

	// �G�̐e�p�[�c������ݒ�
	SetPartsRotation(0, rotEnemy);
}

//============================================================
//	�o�u���폜����
//============================================================
void CEnemy::SubBubble(void)
{
	if (m_pBubble->GetLevel() > 0)
	{ // �o�u�����x���� 0���傫���ꍇ

		if (m_nCounterBubble < BUBBLE_SUB_CNT)
		{ // �J�E���^�[�����l��菬�����ꍇ

			// �J�E���^�[�����Z
			m_nCounterBubble++;
		}
		else
		{ // �J�E���^�[�����l�ȏ�̏ꍇ

			// �J�E���^�[��������
			m_nCounterBubble = 0;

			// ���x�������Z
			m_pBubble->AddLevel(-1);
		}
	}
}

//************************************************************
//	�q�N���X [CEnemyHuman] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyHuman::CEnemyHuman(const TYPE type) : CEnemy(type)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyHuman::~CEnemyHuman()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyHuman::Init(void)
{
	// �ϐ���錾
	CMotion::Info motion = GetMotionInfo();

	// �G�̏�����
	if (FAILED(CEnemy::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	for (int nCntEnemy = 0; nCntEnemy < motion.nNumMotion; nCntEnemy++)
	{ // �ǂݍ��񂾃��[�V���������J��Ԃ�

		// ���[�V�������̐ݒ�
		CObjectChara::SetMotionInfo(motion.aMotionInfo[nCntEnemy]);
	}

	// ���f�����̐ݒ�
	SetModelInfo();

	// ���[�V�����̐ݒ�
	SetMotion(MOTION_MOVE);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEnemyHuman::Uninit(void)
{
	// �G�̏I��
	CEnemy::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemyHuman::Update(void)
{
	switch (GetState())
	{ // ��Ԃ��Ƃ̏���
	case STATE_SPAWN:

		// �X�|�[������̍X�V
		Spawn();

		break;

	case STATE_NORMAL:

		// �o�u���폜
		SubBubble();

		// �G�̓���̍X�V
		CollisionFind();

		break;

	case STATE_DAMAGE:

		// �_���[�W����̍X�V
		Damage();

		// �G�̓���̍X�V
		CollisionFind();

		break;

	case STATE_DEATH:

		// ���S����̍X�V
		if (Death())
		{ // ���S�����ꍇ

			// �����𔲂���
			return;
		}

		// �����_����]
		RandomRotation();

		break;

	case STATE_VANISH:

		// ��������̍X�V
		if (Vanish())
		{ // ���������ꍇ

			// �����𔲂���
			return;
		}

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �G�̍X�V
	CEnemy::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CEnemyHuman::Draw(void)
{
	// �G�̕`��
	CEnemy::Draw();
}

//============================================================
//	���f���t�@�C���擾����
//============================================================
const char* CEnemyHuman::GetModelFileName(const int nModel) const
{
	if (nModel < MODEL_MAX)
	{ // �g�p�ł���C���f�b�N�X�̏ꍇ

		// �����̃C���f�b�N�X�̃��f����Ԃ�
		return mc_apModelFile[nModel];
	}
	else { assert(false); return NONE_STRING; }	// �͈͊O
}

//============================================================
//	�X�|�[������
//============================================================
void CEnemyHuman::Spawn(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posEnemy = GetPosition();		// �G�ʒu
	D3DXVECTOR3 moveEnemy = GetMovePosition();	// �G�ړ���
	D3DXVECTOR3 posWarning = posEnemy;			// �x���ʒu

	// �ߋ��ʒu�̍X�V
	UpdateOldPosition();

	// �X�|�[����Ԃ̓G�Ƃ̓����蔻��
	CollisionSpawnEnemy(posEnemy);

	// �ʒu�����Z
	posEnemy.y -= HUMAN_FALL;
	
	// ���n����
	if (CScene::GetField()->LandPosition(posEnemy, moveEnemy))
	{ // ���n�����ꍇ

		// �x���̕`��󋵂̐ݒ�
		SetDrawWarning(false);	// �`�悵�Ȃ�
	
		// ��Ԃ̐ݒ�
		SetState(STATE_NORMAL);	// �ʏ���
	}

	// �ʒu�𔽉f
	SetPosition(posEnemy);

	// �ʒu�ړ��ʂ𔽉f
	SetMovePosition(moveEnemy);

	// �x���ʒu��n�`�ɓY�킹��
	posWarning.y = CScene::GetField()->GetPositionHeight(posWarning);

	// �x���̈ʒu�̐ݒ�
	SetPositionWarning(D3DXVECTOR3(posWarning.x, posWarning.y + ENE_WAR_ADD_POSY, posWarning.z));
}

//============================================================
//	���m�͈͂̓����蔻��
//============================================================
void CEnemyHuman::CollisionFind(void)
{
	// �ϐ���錾
	StatusInfo  status		= GetStatusInfo();		// �G�X�e�[�^�X
	D3DXVECTOR3 posEnemy	= GetPosition();		// �G�ʒu
	D3DXVECTOR3 moveEnemy	= GetMovePosition();	// �G�ړ���
	D3DXVECTOR3 rotEnemy	= GetRotation();		// �G����
	D3DXVECTOR3 posLook		= VEC3_ZERO;			// ���F�Ώۈʒu
	float fLookRadius		= 0.0f;					// ���F�Ώ۔��a

	D3DXVECTOR3 posBarrier	= CScene::GetStage()->GetStageBarrierPosition();	// �o���A�ʒu
	float fPlayerRadius		= CScene::GetPlayer()->GetRadius();	// �v���C���[���a

	// �ߋ��ʒu�̍X�V
	UpdateOldPosition();

	if (CScene::GetTarget()->GetState() != CTarget::STATE_DESTROY)
	{ // �^�[�Q�b�g���󂳂�Ă��Ȃ���ꍇ

		// ���F�Ώۈʒu��ݒ�
		posLook = CScene::GetTarget()->GetPosition();	// �^�[�Q�b�g�ʒu

		// ���F�Ώ۔��a��ݒ�
		fLookRadius = CScene::GetTarget()->GetRadius();	// �^�[�Q�b�g���a

		// ���F�Ώۂ̍U������
		if (collision::Circle2D(posLook, posEnemy, fPlayerRadius, status.fAttackRadius) == false)
		{ // �G�̍U���͈͊O�̏ꍇ

			// �Ώۂ̕�������������
			Look(posLook, posEnemy, rotEnemy);

			// �Ώۂ̕����Ɉړ� (�O�i)
			moveEnemy.x -= sinf(rotEnemy.y) * status.fForwardMove;
			moveEnemy.z -= cosf(rotEnemy.y) * status.fForwardMove;

			// �d�͂����Z
			moveEnemy.y -= ENE_GRAVITY;

			// �ړ��ʂ����Z
			posEnemy += moveEnemy;

			// �m�b�N�o�b�N�̍X�V
			UpdateKnockBack(posEnemy);

			// �ړ��ʂ�����
			moveEnemy.x += (0.0f - moveEnemy.x) * ENE_REV;
			moveEnemy.z += (0.0f - moveEnemy.z) * ENE_REV;

			// �^�[�Q�b�g�Ƃ̓����蔻��
			CollisionTarget(posEnemy);

			// �ʏ��Ԃ̓G�Ƃ̓����蔻��
			CollisionNormalEnemy(posEnemy);

			// ���n����
			CScene::GetField()->LandPosition(posEnemy, moveEnemy);

			// �X�e�[�W�͈͊O�̕␳
			collision::InCirclePillar(posEnemy, posBarrier, status.fRadius, LIMIT_RADIUS);

			if (GetMotionType() != MOTION_MOVE)
			{ // ���݂̃��[�V���������s����Ȃ��ꍇ

				// ���[�V������ݒ�
				SetMotion(MOTION_MOVE);	// ���s���[�V����
			}
		}
		else
		{ // �G�̍U���͈͓��̏ꍇ

			// �m�b�N�o�b�N�̍X�V
			UpdateKnockBack(posEnemy);

			// �Ώۂ̕�������������
			Look(posLook, posEnemy, rotEnemy);

			// �^�[�Q�b�g�Ƃ̓����蔻��
			CollisionTarget(posEnemy);

			// �ʏ��Ԃ̓G�Ƃ̓����蔻��
			CollisionNormalEnemy(posEnemy);

			// ���n����
			CScene::GetField()->LandPosition(posEnemy, moveEnemy);

			// �X�e�[�W�͈͊O�̕␳
			collision::InCirclePillar(posEnemy, posBarrier, status.fRadius, LIMIT_RADIUS);

			// �U��
			Attack(posLook, posEnemy, fLookRadius);

			if (GetMotionType() != MOTION_ACTION)
			{ // ���݂̃��[�V�������U������Ȃ��ꍇ

				// ���[�V������ݒ�
				SetMotion(MOTION_ACTION);	// �U�����[�V����
			}
		}
	}

	// �ʒu�𔽉f
	SetPosition(posEnemy);

	// �ʒu�ړ��ʂ𔽉f
	SetMovePosition(moveEnemy);

	// �����𔽉f
	SetRotation(rotEnemy);
}

//************************************************************
//	�q�N���X [CEnemyCar] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyCar::CEnemyCar(const TYPE type) : CEnemy(type)
{
	// �����o�ϐ����N���A
	m_spawn = SPAWN_WAIT;	// �X�|�[�����
	m_nCounterSpawn = 0;	// �X�|�[���Ǘ��J�E���^�[
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyCar::~CEnemyCar()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyCar::Init(void)
{
	// �����o�ϐ���������
	m_spawn = SPAWN_WAIT;	// �X�|�[�����
	m_nCounterSpawn = 0;	// �X�|�[���Ǘ��J�E���^�[

	// �G�̏�����
	if (FAILED(CEnemy::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEnemyCar::Uninit(void)
{
	// �G�̏I��
	CEnemy::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemyCar::Update(void)
{
	switch (GetState())
	{ // ��Ԃ��Ƃ̏���
	case STATE_SPAWN:

		// �X�|�[������̍X�V
		Spawn();

		break;

	case STATE_NORMAL:

		// �o�u���폜
		SubBubble();

		// �G�̓���̍X�V
		CollisionFind();

		break;

	case STATE_DAMAGE:

		// �_���[�W����̍X�V
		Damage();

		// �G�̓���̍X�V
		CollisionFind();

		break;

	case STATE_DEATH:

		// ���S����̍X�V
		if (Death())
		{ // ���S�����ꍇ

			// �����𔲂���
			return;
		}

		// �����_����]
		RandomRotation();

		break;

	case STATE_VANISH:

		// ��������̍X�V
		if (Vanish())
		{ // ���������ꍇ

			// �����𔲂���
			return;
		}

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �G�̍X�V
	CEnemy::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CEnemyCar::Draw(void)
{
	// �G�̕`��
	CEnemy::Draw();
}

//============================================================
//	���f���t�@�C���擾����
//============================================================
const char* CEnemyCar::GetModelFileName(const int nModel) const
{
	if (nModel < MODEL_MAX)
	{ // �g�p�ł���C���f�b�N�X�̏ꍇ

		// �����̃C���f�b�N�X�̃��f����Ԃ�
		return mc_apModelFile[nModel];
	}
	else { assert(false); return NONE_STRING; }	// �͈͊O
}

//============================================================
//	�X�|�[������
//============================================================
void CEnemyCar::Spawn(void)
{
	// �ϐ���錾
	StatusInfo  status = GetStatusInfo();		// �G�X�e�[�^�X
	D3DXVECTOR3 posEnemy = GetPosition();		// �G�ʒu
	D3DXVECTOR3 moveEnemy = GetMovePosition();	// �G�ړ���
	D3DXVECTOR3 posWarning = posEnemy;			// �x���ʒu
	D3DXVECTOR3 posBarrier = CScene::GetStage()->GetStageBarrierPosition();	// �o���A�ʒu

	// �ߋ��ʒu�̍X�V
	UpdateOldPosition();

	// �X�|�[����Ԃ̓G�Ƃ̓����蔻��
	CollisionSpawnEnemy(posEnemy);

	// �X�e�[�W�͈͊O�̕␳
	collision::InCirclePillar(posEnemy, posBarrier, status.fRadius, LIMIT_RADIUS);

	switch (m_spawn)
	{ // �X�|�[����Ԃ��Ƃ̏���
	case STATE_SPAWN:

		if (m_nCounterSpawn < SPAWN_WAIT_CNT)
		{ // �J�E���^�[�����l��菬�����ꍇ

			// �J�E���^�[�����Z
			m_nCounterSpawn++;
		}
		else
		{ // �J�E���^�[�����l�ȏ�̏ꍇ

			// �J�E���^�[��������
			m_nCounterSpawn = 0;

			// ��Ԃ�ύX
			m_spawn = SPAWN_FALL;	// �������
		}

		break;

	case STATE_NORMAL:
	
		// �d�͂����Z
		moveEnemy.y -= ENE_GRAVITY;
	
		// �ړ��ʂ����Z
		posEnemy += moveEnemy;
	
		// ���n����
		if (CScene::GetField()->LandPosition(posEnemy, moveEnemy))
		{ // ���n�����ꍇ

			// �x���̕`��󋵂̐ݒ�
			SetDrawWarning(false);	// �`�悵�Ȃ�
	
			// ��Ԃ̐ݒ�
			SetState(STATE_NORMAL);	// �ʏ���
		}

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �ʒu�𔽉f
	SetPosition(posEnemy);

	// �ʒu�ړ��ʂ𔽉f
	SetMovePosition(moveEnemy);

	// �x���ʒu��n�`�ɓY�킹��
	posWarning.y = CScene::GetField()->GetPositionHeight(posWarning);

	// �x���̈ʒu�̐ݒ�
	SetPositionWarning(D3DXVECTOR3(posWarning.x, posWarning.y + ENE_WAR_ADD_POSY, posWarning.z));
}

//============================================================
//	���m�͈͂̓����蔻��
//============================================================
void CEnemyCar::CollisionFind(void)
{
	// �ϐ���錾
	StatusInfo  status		= GetStatusInfo();		// �G�X�e�[�^�X
	D3DXVECTOR3 posEnemy	= GetPosition();		// �G�ʒu
	D3DXVECTOR3 moveEnemy	= GetMovePosition();	// �G�ړ���
	D3DXVECTOR3 rotEnemy	= GetRotation();		// �G����
	D3DXVECTOR3 posLook		= VEC3_ZERO;			// ���F�Ώۈʒu
	D3DXVECTOR3 rotCannon	= VEC3_ZERO;			// �L���m������
	float fLookRadius		= 0.0f;					// ���F�Ώ۔��a

	D3DXVECTOR3 posBarrier	= CScene::GetStage()->GetStageBarrierPosition();	// �o���A�ʒu
	float fPlayerRadius		= CScene::GetPlayer()->GetRadius();	// �v���C���[���a

	// �ߋ��ʒu�̍X�V
	UpdateOldPosition();

	if (CScene::GetTarget()->GetState() != CTarget::STATE_DESTROY)
	{ // �^�[�Q�b�g���󂳂�Ă��Ȃ���ꍇ

		// ���F�Ώۈʒu��ݒ�
		posLook = CScene::GetTarget()->GetPosition();	// �^�[�Q�b�g�ʒu

		// ���F�Ώ۔��a��ݒ�
		fLookRadius = CScene::GetTarget()->GetRadius();	// �^�[�Q�b�g���a

		// ���F�Ώۂ̍U������
		if (collision::Circle2D(posLook, posEnemy, fPlayerRadius, status.fAttackRadius) == false)
		{ // �G�̍U���͈͊O�̏ꍇ

			// �Ώۂ̕�������������
			Look(posLook, posEnemy, rotEnemy);

			// �Ώۂ̕����Ɉړ� (�O�i)
			moveEnemy.x -= sinf(rotEnemy.y) * status.fForwardMove;
			moveEnemy.z -= cosf(rotEnemy.y) * status.fForwardMove;

			// �d�͂����Z
			moveEnemy.y -= ENE_GRAVITY;

			// �ړ��ʂ����Z
			posEnemy += moveEnemy;

			// �m�b�N�o�b�N�̍X�V
			UpdateKnockBack(posEnemy);

			// �ړ��ʂ�����
			moveEnemy.x += (0.0f - moveEnemy.x) * ENE_REV;
			moveEnemy.z += (0.0f - moveEnemy.z) * ENE_REV;

			// �^�[�Q�b�g�Ƃ̓����蔻��
			CollisionTarget(posEnemy);

			// �ʏ��Ԃ̓G�Ƃ̓����蔻��
			CollisionNormalEnemy(posEnemy);

			// ���n����
			CScene::GetField()->LandPosition(posEnemy, moveEnemy);

			// �X�e�[�W�͈͊O�̕␳
			collision::InCirclePillar(posEnemy, posBarrier, status.fRadius, LIMIT_RADIUS);
		}
		else
		{ // �G�̍U���͈͓��̏ꍇ

			// �m�b�N�o�b�N�̍X�V
			UpdateKnockBack(posEnemy);

			// �^�[�Q�b�g�Ƃ̓����蔻��
			CollisionTarget(posEnemy);

			// �ʏ��Ԃ̓G�Ƃ̓����蔻��
			CollisionNormalEnemy(posEnemy);

			// ���n����
			CScene::GetField()->LandPosition(posEnemy, moveEnemy);

			// �X�e�[�W�͈͊O�̕␳
			collision::InCirclePillar(posEnemy, posBarrier, status.fRadius, LIMIT_RADIUS);

			// �L���m�������̐ݒ�
			if (SetRotationCannon(posLook, posEnemy, rotEnemy))
			{ // ���ˉ\��Ԃ̏ꍇ

				// �U��
				Attack(posLook, posEnemy, fLookRadius);
			}
		}
	}

	// �ʒu�𔽉f
	SetPosition(posEnemy);

	// �ʒu�ړ��ʂ𔽉f
	SetMovePosition(moveEnemy);

	// �����𔽉f
	SetRotation(rotEnemy);
}

//============================================================
//	�L���m�������̐ݒ菈��
//============================================================
bool CEnemyCar::SetRotationCannon(const D3DXVECTOR3& rLookPos, const D3DXVECTOR3& rEnemyPos, const D3DXVECTOR3& rEnemyRot)
{
	// �ϐ���錾
	D3DXVECTOR3 cannonRot;	// ��C����
	float fDestRot = 0.0f;	// �ڕW����
	float fDiffRot = 0.0f;	// ����

	// ��C�������擾
	cannonRot = GetMultiModel(MODEL_CANNON)->GetRotation() + rEnemyRot;	// �{�̂̌��������Z
	useful::NormalizeRot(cannonRot.y);	// ��C�����̐��K��

	// �ڕW�����̌v�Z
	fDestRot = atan2f(rEnemyPos.x - rLookPos.x, rEnemyPos.z - rLookPos.z);
	useful::NormalizeRot(fDestRot);		// �ڕW�����̐��K��

	// ���������̌v�Z
	fDiffRot = fDestRot - cannonRot.y;
	useful::NormalizeRot(fDiffRot);		// ���������̐��K��

	// �����̍X�V
	cannonRot.y += fDiffRot * ENECAR_REV_MUL;
	useful::NormalizeRot(cannonRot.y);	// �L���m�������̐��K��

	// ������ݒ�
	GetMultiModel(MODEL_CANNON)->SetRotation(cannonRot - rEnemyRot);	// �{�̂̌��������Z

	if (fDestRot + D3DX_PI <= cannonRot.y + D3DX_PI + ENECAR_SHOTRANGE
	&&  fDestRot + D3DX_PI >= cannonRot.y + D3DX_PI - ENECAR_SHOTRANGE)
	{ // ��C�����ƖڕW�����̌덷�����Ȃ��ꍇ

		// ���ˉ\��Ԃ�Ԃ�
		return true;
	}
	else
	{ // ��C�����ƖڕW�����̌덷�������ꍇ

		// ���˕s�\��Ԃ�Ԃ�
		return false;
	}
}

//************************************************************
//	�q�N���X [CEnemy] �̃Z�b�g�A�b�v�֐�
//************************************************************
//============================================================
//	�Z�b�g�A�b�v����
//============================================================
void CEnemy::LoadSetup(void)
{
	// �ϐ���錾
	CMotion::MotionInfo info;		// �|�[�Y�̑���p
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 rot = VEC3_ZERO;	// �����̑���p
	int nType		= 0;			// ��ނ̑���p
	int nID			= 0;			// �C���f�b�N�X�̑���p
	int nNowPose	= 0;			// ���݂̃|�[�Y�ԍ�
	int nNowKey		= 0;			// ���݂̃L�[�ԍ�
	int nLoop		= 0;			// ���[�v��ON/OFF�̕ϊ��p
	int nBackward	= 0;			// ��ނ�ON/OFF�̕ϊ��p
	int nEnd		= 0;			// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	// �ÓI�����o�ϐ��̏����N���A
	memset(&m_aStatusInfo[0], 0, sizeof(m_aStatusInfo));	// �X�e�[�^�X���
	memset(&m_aPartsInfo[0], 0, sizeof(m_aPartsInfo));		// �p�[�c���
	memset(&m_aMotionInfo[0], 0, sizeof(m_aMotionInfo));	// ���[�V�������

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(ENEMY_SETUP_TXT, "r");

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

					if (strcmp(&aString[0], "ENEMYSET") == 0)
					{ // �ǂݍ��񂾕����� ENEMYSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_ENEMYSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "TYPE") == 0)
							{ // �ǂݍ��񂾕����� TYPE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nType);		// ��ނ�ǂݍ���
							}
							else if (strcmp(&aString[0], "LIFE") == 0)
							{ // �ǂݍ��񂾕����� LIFE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nLife);	// �̗͂�ǂݍ���
							}
							else if (strcmp(&aString[0], "SCORE") == 0)
							{ // �ǂݍ��񂾕����� SCORE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nScore);	// �X�R�A���Z�ʂ�ǂݍ���
							}
							else if (strcmp(&aString[0], "EXP") == 0)
							{ // �ǂݍ��񂾕����� EXP �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nExp);	// �o���l�����ʂ�ǂݍ���
							}
							else if (strcmp(&aString[0], "COLL_RADIUS") == 0)
							{ // �ǂݍ��񂾕����� COLL_RADIUS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fCollRadius);	// �����蔻��̔��a��ǂݍ���
							}
							else if (strcmp(&aString[0], "RADIUS") == 0)
							{ // �ǂݍ��񂾕����� RADIUS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fRadius);	// ���a��ǂݍ���
							}
							else if (strcmp(&aString[0], "HEIGHT") == 0)
							{ // �ǂݍ��񂾕����� HEIGHT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fHeight);	// �c����ǂݍ���
							}
							else if (strcmp(&aString[0], "KNOCKBACK_UP") == 0)
							{ // �ǂݍ��񂾕����� KNOCKBACK_UP �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fKnockBackUp);	// �m�b�N�o�b�N��ړ��ʂ�ǂݍ���
							}
							else if (strcmp(&aString[0], "KNOCKBACK_SIDE") == 0)
							{ // �ǂݍ��񂾕����� KNOCKBACK_SIDE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fKnockBackSide);	// �m�b�N�o�b�N���ړ��ʂ�ǂݍ���
							}
							else if (strcmp(&aString[0], "KNOCKBACK_REV") == 0)
							{ // �ǂݍ��񂾕����� KNOCKBACK_REV �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);								// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fKnockBackRevision);	// �m�b�N�o�b�N�����W����ǂݍ���
							}
							else if (strcmp(&aString[0], "SPAWN_HEIGHT") == 0)
							{ // �ǂݍ��񂾕����� SPAWN_HEIGHT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fSpawnHeight);	// ��������Y���W��ǂݍ���
							}
							else if (strcmp(&aString[0], "BULLET_LIFE") == 0)
							{ // �ǂݍ��񂾕����� BULLET_LIFE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nBullLife);	// �e�̎�����ǂݍ���
							}
							else if (strcmp(&aString[0], "BULLET_DAMAGE") == 0)
							{ // �ǂݍ��񂾕����� BULLET_DAMAGE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nBullDamage);	// �e�̍U���͂�ǂݍ���
							}
							else if (strcmp(&aString[0], "BULLET_MOVE") == 0)
							{ // �ǂݍ��񂾕����� BULLET_MOVE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fBullMove);	// �e�̈ړ��ʂ�ǂݍ���
							}
							else if (strcmp(&aString[0], "BULLET_RADIUS") == 0)
							{ // �ǂݍ��񂾕����� BULLET_RADIUS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fBullRadius);	// �e�̔��a��ǂݍ���
							}
							else if (strcmp(&aString[0], "BULLET_PARTS") == 0)
							{ // �ǂݍ��񂾕����� BULLET_PARTS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nBullParts);	// �e�̔��˃p�[�c��ǂݍ���
							}
							else if (strcmp(&aString[0], "BULLET_POS") == 0)
							{ // �ǂݍ��񂾕����� BULLET_POS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].bullPos.x);	// �e�̔��ˈʒuX��ǂݍ���
								fscanf(pFile, "%f", &m_aStatusInfo[nType].bullPos.y);	// �e�̔��ˈʒuY��ǂݍ���
								fscanf(pFile, "%f", &m_aStatusInfo[nType].bullPos.z);	// �e�̔��ˈʒuZ��ǂݍ���
							}
							else if (strcmp(&aString[0], "BUBBLE_SIZE") == 0)
							{ // �ǂݍ��񂾕����� BUBBLE_SIZE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].bubbleSize.x);	// �o�u���̑傫��X��ǂݍ���
								fscanf(pFile, "%f", &m_aStatusInfo[nType].bubbleSize.y);	// �o�u���̑傫��Y��ǂݍ���
								fscanf(pFile, "%f", &m_aStatusInfo[nType].bubbleSize.z);	// �o�u���̑傫��Z��ǂݍ���
							}
							else if (strcmp(&aString[0], "SHADOW_RADIUS") == 0)
							{ // �ǂݍ��񂾕����� SHADOW_RADIUS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fShadowRadius);	// �e�̔��a��ǂݍ���
							}
							else if (strcmp(&aString[0], "ATTACK_CNT") == 0)
							{ // �ǂݍ��񂾕����� ATTACK_CNT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nCounterAttack);	// �U���p�x��ǂݍ���
							}
							else if (strcmp(&aString[0], "FORWARD_MOVE") == 0)
							{ // �ǂݍ��񂾕����� FORWARD_MOVE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fForwardMove);	// �O�i�̈ړ��ʂ�ǂݍ���
							}
							else if (strcmp(&aString[0], "LOOK_REV") == 0)
							{ // �ǂݍ��񂾕����� LOOK_REV �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fLookRevision);	// �U������␳�W����ǂݍ���
							}
							else if (strcmp(&aString[0], "ATTACK_RADIUS") == 0)
							{ // �ǂݍ��񂾕����� ATTACK_RADIUS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fAttackRadius);	// �U���͈͂�ǂݍ���
							}
						} while (strcmp(&aString[0], "END_ENEMYSET") != 0);	// �ǂݍ��񂾕����� END_ENEMYSET �ł͂Ȃ��ꍇ���[�v
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

					if (strcmp(&aString[0], "ENEMYSET") == 0)
					{ // �ǂݍ��񂾕����� ENEMYSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_ENEMYSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "TYPE") == 0)
							{ // �ǂݍ��񂾕����� TYPE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nType);		// ��ނ�ǂݍ���
							}
							else if (strcmp(&aString[0], "PARTSSET") == 0)
							{ // �ǂݍ��񂾕����� PARTSSET �̏ꍇ

								do
								{ // �ǂݍ��񂾕����� END_PARTSSET �ł͂Ȃ��ꍇ���[�v

									// �t�@�C�����當�����ǂݍ���
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "INDEX") == 0)
									{ // �ǂݍ��񂾕����� INDEX �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%d", &nID);			// ���f���̃C���f�b�N�X��ǂݍ���

										// �p�[�c�������Z
										m_aPartsInfo[nType].nNumParts++;
									}
									else if (strcmp(&aString[0], "PARENT") == 0)
									{ // �ǂݍ��񂾕����� PARENT �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);								// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%d", &m_aPartsInfo[nType].aInfo[nID].nParentID);	// ���f���̐e�̃C���f�b�N�X��ǂݍ���
									}
									else if (strcmp(&aString[0], "POS") == 0)
									{ // �ǂݍ��񂾕����� POS �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);								// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%f", &m_aPartsInfo[nType].aInfo[nID].pos.x);		// X���W��ǂݍ���
										fscanf(pFile, "%f", &m_aPartsInfo[nType].aInfo[nID].pos.y);		// Y���W��ǂݍ���
										fscanf(pFile, "%f", &m_aPartsInfo[nType].aInfo[nID].pos.z);		// Z���W��ǂݍ���
									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{ // �ǂݍ��񂾕����� ROT �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);								// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%f", &m_aPartsInfo[nType].aInfo[nID].rot.x);		// X������ǂݍ���
										fscanf(pFile, "%f", &m_aPartsInfo[nType].aInfo[nID].rot.y);		// Y������ǂݍ���
										fscanf(pFile, "%f", &m_aPartsInfo[nType].aInfo[nID].rot.z);		// Z������ǂݍ���
									}
								} while (strcmp(&aString[0], "END_PARTSSET") != 0);	// �ǂݍ��񂾕����� END_PARTSSET �ł͂Ȃ��ꍇ���[�v
							}
						} while (strcmp(&aString[0], "END_ENEMYSET") != 0);	// �ǂݍ��񂾕����� END_ENEMYSET �ł͂Ȃ��ꍇ���[�v
					}
				} while (strcmp(&aString[0], "END_CHARACTERSET") != 0);		// �ǂݍ��񂾕����� END_CHARACTERSET �ł͂Ȃ��ꍇ���[�v
			}

			// ���[�V�����̐ݒ�
			else if (strcmp(&aString[0], "MOTIONSET") == 0)
			{ // �ǂݍ��񂾕����� MOTIONSET �̏ꍇ

				// ���[�V��������������
				m_aMotionInfo[nType].nNumMotion = 0;

				do
				{ // �ǂݍ��񂾕����� END_MOTIONSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "TYPE") == 0)
					{ // �ǂݍ��񂾕����� TYPE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nType);		// ��ނ�ǂݍ���
					}
					else if (strcmp(&aString[0], "MOTION") == 0)
					{ // �ǂݍ��񂾕����� MOTION �̏ꍇ

						// ���݂̃|�[�Y�ԍ���������
						nNowPose = 0;

						do
						{ // �ǂݍ��񂾕����� END_MOTION �ł͂Ȃ��ꍇ���[�v

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
												info.aKeyInfo[nNowPose].aKey[nNowKey].pos += m_aPartsInfo[nType].aInfo[nNowKey].pos;
											}
											else if (strcmp(&aString[0], "ROT") == 0)
											{ // �ǂݍ��񂾕����� ROT �̏ꍇ

												fscanf(pFile, "%s", &aString[0]);									// = ��ǂݍ��� (�s�v)
												fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);	// X������ǂݍ���
												fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);	// Y������ǂݍ���
												fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);	// Z������ǂݍ���

												// �ǂݍ��񂾌����Ƀp�[�c�̏������������Z
												info.aKeyInfo[nNowPose].aKey[nNowKey].rot += m_aPartsInfo[nType].aInfo[nNowKey].rot;

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
						} while (strcmp(&aString[0], "END_MOTION") != 0);	// �ǂݍ��񂾕����� END_MOTION �ł͂Ȃ��ꍇ���[�v

						// ���[�V�������̐ݒ�
						m_aMotionInfo[nType].aMotionInfo[m_aMotionInfo[nType].nNumMotion] = info;

						// ���[�V�����������Z
						m_aMotionInfo[nType].nNumMotion++;
					}
				} while (strcmp(&aString[0], "END_MOTIONSET") != 0);	// �ǂݍ��񂾕����� END_MOTIONSET �ł͂Ȃ��ꍇ���[�v
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�G�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}
