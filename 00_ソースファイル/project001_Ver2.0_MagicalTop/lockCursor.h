//============================================================
//
//	���b�N�I���\���w�b�_�[ [lockCursor.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _LOCKCURSOR_H_
#define _LOCKCURSOR_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�O���錾
//************************************************************
class CObjectBillboard;	// �I�u�W�F�N�g�r���{�[�h�N���X

//************************************************************
//	�N���X��`
//************************************************************
// ���b�N�I���\���N���X
class CLockCursor : public CObject
{
public:
	// �e�N�X�`����
	typedef enum
	{
		TEXTURE_LOCK000 = 0,	// ���b�N�I���\��(���@�w)�̃e�N�X�`��
		TEXTURE_LOCK001,		// ���b�N�I���\��(��l�p)�̃e�N�X�`��
		TEXTURE_LOCK002,		// ���b�N�I���\��(���l�p)�̃e�N�X�`��
		TEXTURE_MAX				// ���̗񋓌^�̑���
	}TEXTURE;

	// �R���X�g���N�^
	CLockCursor();

	// �f�X�g���N�^
	~CLockCursor();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CLockCursor *Create		// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const bool bDraw = true		// �`���
	);

	// �����o�֐�
	void SetPosition(const D3DXVECTOR3& rPos);	// �ʒu�ݒ�
	void SetLockObject(CObject *pObject);		// ���b�N�I���ݒ�
	void SetEnableDisp(const bool bDisp);		// ���b�N�I���\���ݒ�
	void SetEnableUpdate(const bool bUpdate);	// �X�V�󋵐ݒ�
	void SetEnableDraw(const bool bDraw);		// �`��󋵐ݒ�
	D3DXVECTOR3 GetPosition(void) const;		// �ʒu�擾
	const CObject *GetLockObject(void) const;	// ���b�N�I���擾

private:
	// �����o�ϐ�
	CObjectBillboard *m_apBilboard[TEXTURE_MAX];	// �r���{�[�h�̏��
	CObject *m_pLock;	// ���b�N�I���I�u�W�F�N�g�̏��

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔
};

#endif	// _LOCKCURSOR_H_
