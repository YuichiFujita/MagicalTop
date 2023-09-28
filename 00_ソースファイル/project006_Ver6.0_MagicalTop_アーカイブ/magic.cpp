//============================================================
//
//	���@���� [magic.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "magic.h"
#include "manager.h"
#include "scene.h"
#include "sceneGame.h"
#include "renderer.h"
#include "sound.h"
#include "texture.h"
#include "collision.h"
#include "effect3D.h"
#include "target.h"
#include "enemy.h"
#include "field.h"
#include "stage.h"
#include "bubble.h"
#include "shadow.h"
#include "score.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAGIC_SETUP_TXT	"data\\TXT\\magic.txt"	// �Z�b�g�A�b�v�e�L�X�g���΃p�X

#define SHADOW_SIZE		(D3DXVECTOR3(10.0f, 0.0f, 10.0f))	// �e�̑傫��
#define SHADOW_ALPHA	(0.2f)	// �e�̃��l

#define BUBBLE_POSY_UP	(20.0f)		// �o�u���̏c�ʒu�㏸��
#define BUBBLE_INIT_LEVEL	(10)	// ���x���̏����l

#define MAGIC_PRIO	(3)	// ���@�̗D�揇��

#define MAGIC_NORMAL_DMG	(1)	// �ʏ�V���b�g���̍U����
#define MAGIC_LONG_DMG		(2)	// �����O�V���b�g���̍U����
#define MAGIC_TRICK_DMG		(3)	// �g���b�N�V���b�g���̍U����

#define BONUS_LONG_SCORE	(4000)	// �����O�V���b�g���̃X�R�A���Z��
#define BONUS_TRICK_SCORE	(9000)	// �g���b�N�V���b�g���̃X�R�A���Z��

#define MOVE_NORMAL			(0.05f)	// �ʏ펞�̖��@�̈ړ���
#define MOVE_INHALE_INSIDE	(8.0f)	// �z�����܂ꎞ�̖��@�̓����ւ̈ړ���
#define MOVE_INHALE_LEFT	(14.5f)	// �z�����܂ꎞ�̖��@�̍����ւ̈ړ���
#define MOVE_DELETE			(3.0f)	// �������̖��@�̈ړ���

#define MAGIC_EFF_CNT			(2)		// �G�t�F�N�g�̐����t���[��
#define MAGIC_EFF_MUL_POS		(2.0f)	// �G�t�F�N�g�̐����ʒu�̏�Z��
#define MAGIC_EFF_MUL_MOVE		(1.0f)	// �G�t�F�N�g�̈ړ��ʂ̏�Z��
#define MAGIC_EFF_ALPHA			(0.6f)	// �G�t�F�N�g�̓����x
#define MAGIC_EFF_LIFE			(45)	// �G�t�F�N�g�̎���
#define MAGIC_EFF_MUL_RADIUS	(1.5f)	// �G�t�F�N�g�̔��a�̏�Z��
#define MAGIC_EFF_SUB_RADIUS	(0.2f)	// �G�t�F�N�g�̔��a�̌��Z��
#define MAGIC_EFF_PRIORITY		(3)		// �G�t�F�N�g�̗D�揇��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CMagic::StatusInfo CMagic::m_statusInfo = {};	// �X�e�[�^�X���
CMagic::LevelInfo CMagic::m_level = {};			// ���x�����

//************************************************************
//	�q�N���X [CMagic] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CMagic::CMagic() : CObject(CObject::LABEL_MAGIC)
{
	// �����o�ϐ����N���A
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));	// ���[���h�}�g���b�N�X
	m_pBubble	= NULL;			// �o�u�����
	m_pShadow	= NULL;			// �e���
	m_pos		= VEC3_ZERO;	// ���݈ʒu
	m_movePos	= VEC3_ZERO;	// �ʒu�ړ���
	m_rot		= VEC3_ZERO;	// ����
	m_moveRot	= VEC3_ZERO;	// �����ύX��
	m_state		= STATE_NORMAL;	// ���
	m_nCounterEffect = 0;		// �G�t�F�N�g�Ǘ��J�E���^�[
}

//============================================================
//	�f�X�g���N�^
//============================================================
CMagic::~CMagic()
{

}

//============================================================
//	����������
//============================================================
HRESULT CMagic::Init(void)
{
	// �����o�ϐ���������
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));	// ���[���h�}�g���b�N�X
	m_pBubble	= NULL;			// �o�u�����
	m_pShadow	= NULL;			// �e���
	m_pos		= VEC3_ZERO;	// ���݈ʒu
	m_movePos	= VEC3_ZERO;	// �ʒu�ړ���
	m_rot		= VEC3_ZERO;	// ����
	m_moveRot	= VEC3_ZERO;	// �����ύX��
	m_state		= STATE_NORMAL;	// ���
	m_nCounterEffect = 0;		// �G�t�F�N�g�Ǘ��J�E���^�[

	// �o�u�����̐���
	m_pBubble = CBubble::Create(this, m_statusInfo.nLife + BUBBLE_INIT_LEVEL, VEC3_ALL(m_statusInfo.fBubbleRadius), VEC3_ZERO, 0.0f);
	if (UNUSED(m_pBubble))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pBubble->SetPriority(MAGIC_PRIO);

	// �������x����ݒ�
	m_pBubble->SetLevel(BUBBLE_INIT_LEVEL);

	// �e�̐���
	m_pShadow = CShadow::Create(CShadow::TEXTURE_NORMAL, SHADOW_SIZE, this, SHADOW_ALPHA, SHADOW_ALPHA);
	if (UNUSED(m_pShadow))
	{ // ��g�p���̏ꍇ

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
void CMagic::Uninit(void)
{
	// �o�u���̏I��
	m_pBubble->Uninit();

	// �e�̏I��
	m_pShadow->Uninit();

	// ���@��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CMagic::Update(void)
{
	// �ϐ���錾
	D3DXVECTOR3 vecTarg, vecSide;	// �z�����ݕ����x�N�g��

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NORMAL:	// �ʏ���

		// �ړ��ʂ����Z
		m_pos += m_movePos;

		// �o�u�����x�������Z
		m_pBubble->AddLevel(1);

		if (CScene::GetStage()->CollisionBarrier(m_pos, m_pBubble->GetRadius())
		||  m_pBubble->GetLevel() >= m_statusInfo.nLife)
		{ // �����͈͂ɓ������Ă���܂��͎����������ꍇ

			// ��Ԃ�ݒ�
			m_state = STATE_DELETE;	// �������
		}

		break;

	case STATE_DELETE:	// �������
		
		// �^�[�Q�b�g�����̃x�N�g�����v�Z
		vecTarg = CScene::GetTarget()->GetPosition() - m_pos;
		vecTarg.y = 0.0f;						// �x�N�g���̏c�����𖳎�
		D3DXVec3Normalize(&vecTarg, &vecTarg);	// �x�N�g�����K��

		// �^�[�Q�b�g�������x�N�g�����v�Z
		vecSide = D3DXVECTOR3(vecTarg.z, 0.0f, -vecTarg.x);

		if (CScene::GetStage()->CollisionBarrier(m_pos, m_pBubble->GetRadius()))
		{ // �o���A�̔�����̏ꍇ

			// �ړ��ʂ�ݒ�
			m_movePos = vecTarg * MOVE_DELETE;
			m_movePos += vecSide * MOVE_DELETE;
		}
		else
		{ // �o���A�̔���O�̏ꍇ

			// �ړ��ʂ�ݒ�
			m_movePos = vecTarg * MOVE_INHALE_INSIDE;
			m_movePos += vecSide * MOVE_INHALE_LEFT;
		}

		// �ړ��ʂ����Z
		m_pos += m_movePos;

		// �o�u�����x�������Z
		m_pBubble->AddLevel(-1);

		if (m_pBubble->GetLevel() <= 0)
		{ // ���x����������؂����ꍇ

			// �I�u�W�F�N�g�̏I��
			Uninit();

			// �֐��𔲂���
			return;
		}

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	if (CScene::GetField()->IsPositionRange(m_pos))
	{ // �n�ʂ͈͓̔��̏ꍇ

		// �c���W��n�`�ɓY�킹��
		m_pos.y = CScene::GetField()->GetPositionHeight(m_pos);
		m_pos.y += m_pBubble->GetRadius() + BUBBLE_POSY_UP;
	}

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
			m_pos + (vecRandom * MAGIC_EFF_MUL_POS),		// �ʒu
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

	// �o�u���̍X�V
	m_pBubble->Update();

	// �e�̑傫����ݒ�
	m_pShadow->SetScalingOrigin(SHADOW_SIZE + (D3DXVECTOR3(1.0f, 0.0f, 1.0f) * m_pBubble->GetRadius()));

	// �e�̍X�V
	m_pShadow->Update();

	if (CollisionEnemy())
	{ // �G�ɓ������Ă����ꍇ

		// �I�u�W�F�N�g�̏I��
		Uninit();

		// �T�E���h�̍Đ�
		CManager::GetSound()->Play(CSound::LABEL_SE_STICK);	// �A���e��

		// �֐��𔲂���
		return;
	}
}

//============================================================
//	�`�揈��
//============================================================
void CMagic::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//============================================================
//	��������
//============================================================
CMagic *CMagic::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXVECTOR3& rVec		// �ړ�����
)
{
	// �|�C���^��錾
	CMagic *pMagic = NULL;		// ���@�����p

	if (UNUSED(pMagic))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pMagic = new CMagic;	// ���@
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pMagic))
	{ // �g�p����Ă���ꍇ
		
		// ���@�̏�����
		if (FAILED(pMagic->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pMagic;
			pMagic = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		pMagic->SetPosition(rPos);

		// ������ݒ�
		pMagic->SetRotation(rRot);

		// �ړ��ʂ�ݒ�
		pMagic->SetMove(rVec, pMagic->GetStatusInfo().aLevel[pMagic->m_level.nMove].fMove);

		// �m�ۂ����A�h���X��Ԃ�
		return pMagic;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�X�e�[�^�X���擾����
//============================================================
CMagic::StatusInfo CMagic::GetStatusInfo(void)
{
	// �X�e�[�^�X����Ԃ�
	return m_statusInfo;
}

//============================================================
//	�X�e�[�^�X���x���̉��Z����
//============================================================
void CMagic::AddLevelStatus(const LEVELINFO level)
{
	// �����̃��x�������Z
	switch (level)
	{ // ���x����񂲂Ƃ̏���
	case LEVELINFO_NUM:		// ���ː�

		// ���ː��̃��x�������Z
		m_level.nNumBunnle++;

		// ���ː��I�[�o�[
		assert(m_level.nNumBunnle < LEVEL_MAX);

		break;

	case LEVELINFO_SPEED:	// �e��

		// �e���̃��x�������Z
		m_level.nMove++;

		// �e���I�[�o�[
		assert(m_level.nMove < LEVEL_MAX);

		break;

	case LEVELINFO_RAPID:	// �A�ˑ��x

		// �A�ˑ��x�̃��x�������Z
		m_level.nCoolTime++;

		// �A�ˑ��x�I�[�o�[
		assert(m_level.nCoolTime < LEVEL_MAX);

		break;

	default:	// ��O����
		assert(false);
		break;
	}
}

//============================================================
//	�X�e�[�^�X���x���擾����
//============================================================
int CMagic::GetLevelStatus(const LEVELINFO level)
{
	// �����̃��x����Ԃ�
	switch (level)
	{ // ���x����񂲂Ƃ̏���
	case LEVELINFO_NUM:		// ���ː�

		// ���ː��̃��x����Ԃ�
		return m_level.nNumBunnle;

	case LEVELINFO_SPEED:	// �e��

		// �e���̃��x����Ԃ�
		return m_level.nMove;

	case LEVELINFO_RAPID:	// �A�ˑ��x

		// �A�ˑ��x�̃��x����Ԃ�
		return m_level.nCoolTime;

	default:	// ��O����
		assert(false);
		return NONE_IDX;
	}
}

//============================================================
//	�ړ��ʂ̐ݒ菈��
//============================================================
void CMagic::SetMove(D3DXVECTOR3 vec, const float fMove)
{
	// �x�N�g���̐��K��
	D3DXVec3Normalize(&vec, &vec);

	// �ړ��ʂ�ݒ�
	m_movePos = vec * fMove;
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CMagic::SetPosition(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CMagic::SetRotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_rot = rRot;

	// �����̐��K��
	useful::NormalizeRot(m_rot.x);
	useful::NormalizeRot(m_rot.y);
	useful::NormalizeRot(m_rot.z);
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CMagic::GetPosition(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CMagic::GetRotation(void) const
{
	// ������Ԃ�
	return m_rot;
}

//============================================================
//	�G�Ƃ̓����蔻��
//============================================================
bool CMagic::CollisionEnemy(void)
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
				{ // �G�̏�Ԃ��X�|�[���܂��͎��S��Ԃ̏ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// ���@����
				if (collision::Circle3D
				( // ����
					m_pos,						// ����ʒu
					pObjCheck->GetPosition(),	// ����ڕW�ʒu
					m_pBubble->GetRadius(),		// ���蔼�a
					pObjCheck->GetRadius()		// ����ڕW���a
				))
				{ // ���@�ɓ������Ă����ꍇ

					if (m_state == STATE_NORMAL)
					{ // ��Ԃ��ʏ�̏ꍇ

						if (m_pBubble->GetLevel() / (m_pBubble->GetMaxLevel() / 2) < 1)
						{ // �o�u���T�C�Y�������ȏ㐬�����Ă��Ȃ��ꍇ

							// �G�̃q�b�g����
							pObjCheck->HitKnockBack(MAGIC_NORMAL_DMG, m_movePos);
						}
						else
						{ // �o�u���T�C�Y�������ȏ㐬�����Ă���ꍇ

							if (CManager::GetScene()->GetMode() == CScene::MODE_GAME)
							{ // ���[�h���Q�[���̏ꍇ

								// �X�R�A�����Z
								CSceneGame::GetScore()->Add(BONUS_LONG_SCORE);
							}

							// �G�̃q�b�g����
							pObjCheck->HitKnockBack(MAGIC_LONG_DMG, m_movePos);
						}
					}
					else
					{ // ��Ԃ��ʏ�ł͂Ȃ��ꍇ

						if (CManager::GetScene()->GetMode() == CScene::MODE_GAME)
						{ // ���[�h���Q�[���̏ꍇ

							// �X�R�A�����Z
							CSceneGame::GetScore()->Add(BONUS_TRICK_SCORE);
						}

						// �G�̃q�b�g����
						pObjCheck->HitKnockBack(MAGIC_TRICK_DMG, m_movePos);
					}

					// �������������Ԃ�
					return true;
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}

	// �������Ă��Ȃ������Ԃ�
	return false;
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
void CMagic::LoadSetup(void)
{
	// �ϐ���錾
	int nLevel = 0;	// ���x���̑���p
	int nEnd = 0;	// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	// �ÓI�����o�ϐ��̏����N���A
	memset(&m_statusInfo, 0, sizeof(m_statusInfo));	// �X�e�[�^�X���
	memset(&m_level, 0, sizeof(m_level));			// ���x�����

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(MAGIC_SETUP_TXT, "r");

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

					// ��{���
					if (strcmp(&aString[0], "MAGICSET") == 0)
					{ // �ǂݍ��񂾕����� MAGICSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_MAGICSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "LIFE") == 0)
							{ // �ǂݍ��񂾕����� LIFE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_statusInfo.nLife);	// ������ǂݍ���
							}
							else if (strcmp(&aString[0], "SHOT_PARTS") == 0)
							{ // �ǂݍ��񂾕����� SHOT_PARTS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_statusInfo.nShotParts);	// ���˃p�[�c��ǂݍ���
							}
							else if (strcmp(&aString[0], "SHOT_POS") == 0)
							{ // �ǂݍ��񂾕����� SHOT_POS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_statusInfo.shotPos.x);	// ���ˈʒuX��ǂݍ���
								fscanf(pFile, "%f", &m_statusInfo.shotPos.y);	// ���ˈʒuY��ǂݍ���
								fscanf(pFile, "%f", &m_statusInfo.shotPos.z);	// ���ˈʒuZ��ǂݍ���
							}
							else if (strcmp(&aString[0], "BUBBLE_RADIUS") == 0)
							{ // �ǂݍ��񂾕����� BUBBLE_RADIUS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_statusInfo.fBubbleRadius);	// �o�u�����a��ǂݍ���
							}
							else if (strcmp(&aString[0], "SHOT_ANGLE") == 0)
							{ // �ǂݍ��񂾕����� SHOT_ANGLE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_statusInfo.fShotAngle);		// �ˌ��p�x��ǂݍ���

								// �ǂݍ��񂾊p�x��ϊ�
								m_statusInfo.fShotAngle = D3DXToRadian(m_statusInfo.fShotAngle);
							}
						} while (strcmp(&aString[0], "END_MAGICSET") != 0);	// �ǂݍ��񂾕����� END_MAGICSET �ł͂Ȃ��ꍇ���[�v
					}

					// ���x�����
					else if (strcmp(&aString[0], "LEVELSET") == 0)
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
							else if (strcmp(&aString[0], "NUMBUBBLE") == 0)
							{ // �ǂݍ��񂾕����� NUMBUBBLE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);								// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_statusInfo.aLevel[nLevel].nNumBunnle);	// �o�u������ǂݍ���
							}
							else if (strcmp(&aString[0], "MOVE") == 0)
							{ // �ǂݍ��񂾕����� MOVE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);								// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_statusInfo.aLevel[nLevel].fMove);		// �ړ��ʂ�ǂݍ���
							}
							else if (strcmp(&aString[0], "COOLTIME") == 0)
							{ // �ǂݍ��񂾕����� COOLTIME �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);								// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_statusInfo.aLevel[nLevel].nCoolTime);	// �N�[���^�C����ǂݍ���
							}
						} while (strcmp(&aString[0], "END_LEVELSET") != 0);	// �ǂݍ��񂾕����� END_LEVELSET �ł͂Ȃ��ꍇ���[�v
					}
				} while (strcmp(&aString[0], "END_STATUSSET") != 0);		// �ǂݍ��񂾕����� END_STATUSSET �ł͂Ȃ��ꍇ���[�v
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "���@�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}
