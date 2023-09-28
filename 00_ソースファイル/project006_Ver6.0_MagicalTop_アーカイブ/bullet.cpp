//============================================================
//
//	�e���� [bullet.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "bullet.h"
#include "manager.h"
#include "scene.h"
#include "sceneGame.h"
#include "renderer.h"
#include "sound.h"
#include "texture.h"

#include "collision.h"
#include "target.h"
#include "player.h"
#include "field.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CBullet::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\bullet000.png",	// �e�e�N�X�`��
};

//************************************************************
//	�q�N���X [CBullet] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CBullet::CBullet(const int nDamage) : CObjectBillboard(CObject::LABEL_BULLET), m_nDamage(nDamage)
{
	// �����o�ϐ����N���A
	m_move  = VEC3_ZERO;	// �ړ���
	m_type  = TYPE_ENEMY;	// ���
	m_nLife = 0;			// ����
}

//============================================================
//	�f�X�g���N�^
//============================================================
CBullet::~CBullet()
{

}

//============================================================
//	����������
//============================================================
HRESULT CBullet::Init(void)
{
	// �����o�ϐ���������
	m_move  = VEC3_ZERO;	// �ړ���
	m_type  = TYPE_ENEMY;	// ���
	m_nLife = 0;			// ����

	// �I�u�W�F�N�g�r���{�[�h�̏�����
	if (FAILED(CObjectBillboard::Init()))
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
void CBullet::Uninit(void)
{
	// �I�u�W�F�N�g�r���{�[�h�̏I��
	CObjectBillboard::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CBullet::Update(void)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = CObjectBillboard::GetPosition();	// �ʒu

	if (m_nLife > 0)
	{ // �������c���Ă���ꍇ

		// �J�E���^�[�����Z
		m_nLife--;
	}
	else
	{ // �����������ꍇ

		// �I�u�W�F�N�g�̏I��
		Uninit();

		// �֐��𔲂���
		return;
	}

	// �ړ��ʂ����Z
	pos += m_move;

	// �n�ʂƂ̓����蔻��
	if (pos.y <= CScene::GetField()->GetPositionHeight(pos))
	{ // �n�ʂɓ������Ă���ꍇ

		// �I�u�W�F�N�g�̏I��
		Uninit();

		// �T�E���h�̍Đ�
		CManager::GetSound()->Play(CSound::LABEL_SE_HIT);	// �e���e��

		// �֐��𔲂���
		return;
	}

	// �^�[�Q�b�g�Ƃ̓����蔻��
	if (CollisionTarget())
	{ // �^�[�Q�b�g�ɓ������Ă���ꍇ

		// �I�u�W�F�N�g�̏I��
		Uninit();

		// �T�E���h�̍Đ�
		CManager::GetSound()->Play(CSound::LABEL_SE_HIT);	// �e���e��

		// �֐��𔲂���
		return;
	}

	// �ʒu��ݒ�
	CObjectBillboard::SetPosition(pos);

	// �I�u�W�F�N�g�r���{�[�h�̍X�V
	CObjectBillboard::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CBullet::Draw(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// ���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// ���e�X�g�̗L�� / �����̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 160);				// ���e�X�g�̎Q�ƒl�ݒ�

	// �I�u�W�F�N�g�r���{�[�h�̕`��
	CObjectBillboard::Draw();

	// ���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g�̗L�� / �����̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// ���e�X�g�̎Q�ƒl�ݒ�
}

//============================================================
//	�ړ��ʂ̐ݒ菈��
//============================================================
void CBullet::SetMove(D3DXVECTOR3 vec, const float fMove)
{
	// �x�N�g���̐��K��
	D3DXVec3Normalize(&vec, &vec);

	// �ړ��ʂ�ݒ�
	m_move = vec * fMove;
}

//============================================================
//	��ނ̐ݒ菈��
//============================================================
void CBullet::SetType(const TYPE type)
{
	// �����̎�ނ���
	m_type = type;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CBullet::SetLife(const int nLife)
{
	// �����̎�������
	m_nLife = nLife;
}

//============================================================
//	��������
//============================================================
CBullet *CBullet::Create
(
	const TYPE type,			// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rSize,	// �傫��
	const D3DXCOLOR& rCol,		// �F
	const D3DXVECTOR3& rVec,	// �ړ�����
	const float fMove,			// �ړ����x
	const int nLife,			// ����
	const int nDamage			// �U����
)
{
	// �ϐ���錾
	int nTextureID;	// �e�N�X�`���C���f�b�N�X

	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^
	CBullet *pBullet = NULL;	// �e�����p

	if (UNUSED(pBullet))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pBullet = new CBullet(nDamage);	// �e
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pBullet))
	{ // �m�ۂɐ������Ă���ꍇ

		// �e�̏�����
		if (FAILED(pBullet->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pBullet;
			pBullet = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �e�N�X�`����o�^
		nTextureID = pTexture->Regist(mc_apTextureFile[type]);

		// �e�N�X�`��������
		pBullet->BindTexture(nTextureID);

		// �ʒu��ݒ�
		pBullet->SetPosition(rPos);

		// �傫����ݒ�
		pBullet->SetScaling(rSize);

		// �F��ݒ�
		pBullet->SetColor(rCol);

		// ��ނ�ݒ�
		pBullet->SetType(type);

		// �ړ��ʂ�ݒ�
		pBullet->SetMove(rVec, fMove);

		// �����̐ݒ�
		pBullet->SetLife(nLife);

		// Z�e�X�g��ݒ�
		pBullet->SetFunc(D3DCMP_LESSEQUAL);

		// Z�o�b�t�@�̎g�p�󋵂�ݒ�
		pBullet->SetZEnable(true);

		// �m�ۂ����A�h���X��Ԃ�
		return pBullet;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�^�[�Q�b�g�Ƃ̓����蔻��
//============================================================
bool CBullet::CollisionTarget(void)
{
	// �ϐ���錾
	bool bHit = false;	// �����

	// �|�C���^��錾
	CTarget *pTarget = CScene::GetTarget();	// �^�[�Q�b�g���

	if (USED(pTarget))
	{ // �^�[�Q�b�g���g�p����Ă���ꍇ

		// �ϐ���錾
		D3DXVECTOR3 sizeBullet = GetScaling();

		// �^�[�Q�b�g�Ƃ̓����蔻��
		bHit = collision::Circle3D
		( // ����
			GetPosition(),							// �e�ʒu
			pTarget->GetPosition(),					// �^�[�Q�b�g�ʒu
			(sizeBullet.x + sizeBullet.y) * 0.5f,	// �e���a
			pTarget->GetRadius()					// �^�[�Q�b�g���a
		);

		if (bHit)
		{ // �������Ă����ꍇ

			// �^�[�Q�b�g�̃q�b�g����
			pTarget->Hit(m_nDamage);
		}
	}

	// ����󋵂�Ԃ�
	return bHit;
}

//============================================================
//	�v���C���[�Ƃ̓����蔻��
//============================================================
bool CBullet::CollisionPlayer(void)
{
	// �ϐ���錾
	bool bHit = false;	// �����

	// �|�C���^��錾
	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[���

	if (pPlayer->GetState() != CPlayer::STATE_DEATH)
	{ // �v���C���[���g�p����Ă���ꍇ

		// �ϐ���錾
		D3DXVECTOR3 sizeBullet = GetScaling();

		// �v���C���[�Ƃ̓����蔻��
		bHit = collision::Circle3D
		( // ����
			GetPosition(),							// �e�ʒu
			pPlayer->GetPosition(),					// �v���C���[�ʒu
			(sizeBullet.x + sizeBullet.y) * 0.5f,	// �e���a
			pPlayer->GetRadius()					// �v���C���[���a
		);

		if (bHit)
		{ // �������Ă����ꍇ

			// �v���C���[�̃q�b�g����
			pPlayer->Hit(m_nDamage);
		}
	}

	// ����󋵂�Ԃ�
	return bHit;
}
