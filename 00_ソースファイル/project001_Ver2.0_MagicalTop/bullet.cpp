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
#include "renderer.h"
#include "texture.h"

#include "score.h"
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
CBullet::CBullet() : CObjectBillboard(CObject::LABEL_BULLET)
{
	// �����o�ϐ����N���A
	m_move  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
	m_type  = TYPE_ENEMY;	// ���
	m_nLife = 0;	// ����
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
	m_move  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
	m_type  = TYPE_ENEMY;	// ���
	m_nLife = 0;	// ����

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

	if (pos.y <= CManager::GetField()->GetPositionHeight(pos))
	{ // �n�ʂɓ������Ă���ꍇ

		// �I�u�W�F�N�g�̏I��
		Uninit();

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
	// �I�u�W�F�N�g�r���{�[�h�̕`��
	CObjectBillboard::Draw();
}

//============================================================
//	�ړ��ʂ̐ݒ菈��
//============================================================
void CBullet::SetMove(D3DXVECTOR3 rot, const float fMove)
{
	// �����̐��K��
	useful::NormalizeRot(rot.y);

	// �ړ��ʂ����߂�
	m_move.x = (fMove * sinf(rot.x)) * sinf(rot.y);
	m_move.y = (fMove * cosf(rot.x));
	m_move.z = (fMove * sinf(rot.x)) * cosf(rot.y);
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
CBullet *CBullet::Create(const TYPE type, const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const D3DXCOLOR& rCol, const D3DXVECTOR3& rRot, const float fMove, const int nLife)
{
	// �ϐ���錾
	int nTextureID;	// �e�N�X�`���C���f�b�N�X

	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^
	CBullet *pBullet = NULL;	// �e�����p

	if (UNUSED(pBullet))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pBullet = new CBullet;	// �e
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pBullet))
	{ // �m�ۂɐ������Ă���ꍇ

		if (SUCCEEDED(pBullet->GetResult()))
		{ // �I�u�W�F�N�g�̏��ݒ�ɐ��������ꍇ

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
			pBullet->SetMove(rRot, fMove);

			// �����̐ݒ�
			pBullet->SetLife(nLife);

			// �m�ۂ����A�h���X��Ԃ�
			return pBullet;
		}
		else
		{ // �I�u�W�F�N�g�̏��ݒ�Ɏ��s�����ꍇ

			// �������J��
			delete pBullet;
			pBullet = NULL;

			// ���s��Ԃ�
			return NULL;
		}
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}
