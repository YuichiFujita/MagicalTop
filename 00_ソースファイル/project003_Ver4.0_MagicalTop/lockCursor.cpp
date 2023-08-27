//============================================================
//
//	���b�N�I���\������ [lockCursor.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "lockCursor.h"
#include "manager.h"
#include "renderer.h"
#include "objectBillboard.h"
#include "texture.h"

//************************************************************
//	�}�N����`
//************************************************************
#define LOCKCURSOR_PRIO	(4)	// ���b�N�I���\���̗D�揇��
#define LOCKCURSOR_SIZE	(D3DXVECTOR3(120.0f, 120.0f, 0.0f))	// �傫��
#define ADD_ROT_UP		(0.05f)		// �������Z��(��l�p)
#define ADD_ROT_DOWN	(0.025f)	// �������Z��(���l�p)

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CLockCursor::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\lockCursor000.png",	// ���b�N�I���\��(���@�w)
	"data\\TEXTURE\\lockCursor001.png",	// ���b�N�I���\��(��l�p)
	"data\\TEXTURE\\lockCursor001.png",	// ���b�N�I���\��(���l�p)
};

//************************************************************
//	�q�N���X [CLockCursor] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CLockCursor::CLockCursor() : CObject(CObject::LABEL_LOCK, DEFAULT_PRIO)
{
	// �����o�ϐ����N���A
	memset(&m_apBilboard[0], 0, sizeof(m_apBilboard));	// �r���{�[�h�̏��
	m_pLock = NULL;		// ���b�N�I���I�u�W�F�N�g�̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CLockCursor::~CLockCursor()
{

}

//============================================================
//	����������
//============================================================
HRESULT CLockCursor::Init(void)
{
	// �ϐ���錾
	int nTextureID;	// �e�N�X�`���C���f�b�N�X

	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// �����o�ϐ���������
	memset(&m_apBilboard[0], 0, sizeof(m_apBilboard));	// �r���{�[�h�̏��
	m_pLock = NULL;		// ���b�N�I���I�u�W�F�N�g�̏��

	for (int nCntLock = 0; nCntLock < TEXTURE_MAX; nCntLock++)
	{ // �e�N�X�`���̍ő吔���J��Ԃ�

		// �r���{�[�h�̐���
		m_apBilboard[nCntLock] = CObjectBillboard::Create
		( // ����
			VEC3_ZERO,		// �ʒu
			LOCKCURSOR_SIZE	// �傫��
		);
		if (UNUSED(m_apBilboard[nCntLock]))
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����o�^
		nTextureID = pTexture->Regist(mc_apTextureFile[nCntLock]);

		// �e�N�X�`��������
		m_apBilboard[nCntLock]->BindTexture(nTextureID);

		// �D�揇�ʂ�ݒ�
		m_apBilboard[nCntLock]->SetPriority(LOCKCURSOR_PRIO);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CLockCursor::Uninit(void)
{
	// �r���{�[�h��j��
	for (int nCntLock = 0; nCntLock < TEXTURE_MAX; nCntLock++)
	{ // �e�N�X�`���̍ő吔���J��Ԃ�

		// �r���{�[�h�̏I��
		m_apBilboard[nCntLock]->Uninit();
	}

	// ���g�̃I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CLockCursor::Update(void)
{
	// �ϐ���錾
	D3DXVECTOR3 rot;		// ����
	D3DXMATRIX  mtxLock;	// ���b�N�I���I�u�W�F�N�g�̃}�g���b�N�X

	// �ϐ��z���錾
	const float fAddRot[] =	// �������Z��
	{ // �����l
		ADD_ROT_UP,		// ��l�p�̌������Z��
		ADD_ROT_DOWN	// ���l�p�̌������Z��
	};

	for (int nCntLock = 1; nCntLock < TEXTURE_MAX; nCntLock++)
	{ // ��]����r���{�[�h�����J��Ԃ�

		// �������擾
		rot = m_apBilboard[nCntLock]->GetRotation();

		// ������ݒ�
		m_apBilboard[nCntLock]->SetRotation(D3DXVECTOR3(rot.x, rot.y, rot.z + fAddRot[nCntLock - 1]));
	}

	for (int nCntLock = 0; nCntLock < TEXTURE_MAX; nCntLock++)
	{ // �e�N�X�`���̍ő吔���J��Ԃ�

		// �r���{�[�h�̍X�V
		m_apBilboard[nCntLock]->Update();

		// TODO�F�|�C���^�̊m�F�����Ƃ��ꂢ��
		if (CObject::CheckUse(m_pLock))
		{ // ���b�N�I���Ώۂ��g�p����Ă����ꍇ

			// ���[���h�}�g���b�N�X���擾
			mtxLock = m_pLock->GetMtxWorld();

			// �ʒu��ݒ�
			m_apBilboard[nCntLock]->SetPosition(D3DXVECTOR3(mtxLock._41, mtxLock._42, mtxLock._43));
		}
		else
		{ // ���b�N�I���Ώۂ��g�p����Ă��Ȃ������ꍇ

			// ���b�N�I�����Ă���I�u�W�F�N�g�̏���������
			m_pLock = NULL;
		}
	}
}

//============================================================
//	�`�揈��
//============================================================
void CLockCursor::Draw(void)
{

}

//============================================================
//	��������
//============================================================
CLockCursor *CLockCursor::Create(const D3DXVECTOR3& rPos, const bool bDraw)
{
	// �|�C���^��錾
	CLockCursor *pLockCursor = NULL;	// ���b�N�I���\�������p

	if (UNUSED(pLockCursor))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pLockCursor = new CLockCursor;	// ���b�N�I���\��
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pLockCursor))
	{ // �m�ۂɐ������Ă���ꍇ

		// ���b�N�I���\���̏�����
		if (FAILED(pLockCursor->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		pLockCursor->SetPosition(rPos);

		// �`��󋵂�ݒ�
		pLockCursor->SetEnableDraw(bDraw);

		// �m�ۂ����A�h���X��Ԃ�
		return pLockCursor;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CLockCursor::SetPosition(const D3DXVECTOR3& rPos)
{
	for (int nCntLock = 0; nCntLock < TEXTURE_MAX; nCntLock++)
	{ // �e�N�X�`���̍ő吔���J��Ԃ�

		// �����̈ʒu��ݒ�
		m_apBilboard[nCntLock]->SetPosition(rPos);
	}
}

//============================================================
//	���b�N�I���̐ݒ菈��
//============================================================
void CLockCursor::SetLockObject(CObject *pObject)
{
	// �����̃I�u�W�F�N�g��ݒ�
	m_pLock = pObject;
}

//============================================================
//	���b�N�I���\���̐ݒ菈��
//============================================================
void CLockCursor::SetEnableDisp(const bool bDisp)
{
	// �����̕`��󋵂̐ݒ�
	SetEnableDraw(bDisp);

	if (bDisp == false)
	{ // �`�悵�Ȃ��ݒ肾�����ꍇ

		// ���b�N�I�����Ă���I�u�W�F�N�g�̏���������
		m_pLock = NULL;

		for (int nCntLock = 0; nCntLock < TEXTURE_MAX; nCntLock++)
		{ // �e�N�X�`���̍ő吔���J��Ԃ�

			// ������������
			m_apBilboard[nCntLock]->SetRotation(VEC3_ZERO);
		}
	}
}

//============================================================
//	�X�V�󋵂̐ݒ菈��
//============================================================
void CLockCursor::SetEnableUpdate(const bool bUpdate)
{
	// �����̍X�V�󋵂�ݒ�
	CObject::SetEnableUpdate(bUpdate);	// ���g

	for (int nCntLock = 0; nCntLock < TEXTURE_MAX; nCntLock++)
	{ // �e�N�X�`���̍ő吔���J��Ԃ�

		// �����̍X�V�󋵂�ݒ�
		m_apBilboard[nCntLock]->SetEnableUpdate(bUpdate);	// ���b�N�I���\��
	}
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CLockCursor::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	CObject::SetEnableDraw(bDraw);		// ���g

	for (int nCntLock = 0; nCntLock < TEXTURE_MAX; nCntLock++)
	{ // �e�N�X�`���̍ő吔���J��Ԃ�

		// �����̕`��󋵂�ݒ�
		m_apBilboard[nCntLock]->SetEnableDraw(bDraw);		// ���b�N�I���\��
	}
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CLockCursor::GetPosition(void) const
{
	// �擪�A�h���X�̈ʒu��Ԃ�
	return m_apBilboard[0]->GetPosition();
}

//============================================================
//	���b�N�I���擾����
//============================================================
const CObject *CLockCursor::GetLockObject(void) const
{
	// ���b�N�I�����̃I�u�W�F�N�g��Ԃ�
	return m_pLock;
}
