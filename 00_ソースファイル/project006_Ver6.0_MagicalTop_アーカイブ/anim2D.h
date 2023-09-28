//============================================================
//
//	�A�j���[�V����2D�w�b�_�[ [anim2D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ANIM2D_H_
#define _ANIM2D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object2D.h"

//************************************************************
//	�N���X��`
//************************************************************
// �A�j���[�V����2D�N���X
class CAnim2D : public CObject2D
{
public:
	// �R���X�g���N�^
	CAnim2D();
	CAnim2D(const CObject::LABEL label, const int nPriority = DEFAULT_PRIO);

	// �f�X�g���N�^
	~CAnim2D();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �I�[�o�[���[�h�����I�[�o�[���C�h�֐�
	HRESULT Init	// ������
	( // ����
		const int nWidthPtrn,	// �e�N�X�`���̉��̕�����
		const int nHeightPtrn	// �e�N�X�`���̏c�̕�����
	);

	// �ÓI�����o�֐�
	static CAnim2D *Create	// ����
	( // ����
		const int nWidthPtrn,		// �e�N�X�`���̉��̕�����
		const int nHeightPtrn,		// �e�N�X�`���̏c�̕�����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rSize = VEC3_ONE,	// �傫��
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXCOLOR& rCol = XCOL_WHITE		// �F
	);

	// �����o�֐�
	void SetPosition(const D3DXVECTOR3& rPos);		// �ʒu�ݒ�
	void SetRotation(const D3DXVECTOR3& rRot);		// �����ݒ�
	void SetScaling(const D3DXVECTOR3& rSize);		// �傫���ݒ�
	void SetColor(const D3DXCOLOR& rCol);			// �F�ݒ�
	void SetPattern(const int nPattern);			// �p�^�[���̐ݒ�
	void SetMaxPattern(const int nMaxPtrn);			// �p�^�[���̑����̐ݒ�
	void SetWidthPattern(const int nWidthPtrn);		// �e�N�X�`���̉��������̐ݒ�
	void SetHeightPattern(const int nHeightPtrn);	// �e�N�X�`���̏c�������̐ݒ�
	void SetCounter(const int nCntChange);			// �J�E���^�[�̐ݒ�
	void SetEnableStop(const bool bStop);			// ��~�󋵂̐ݒ�
	void SetEnablePlayBack(const bool bPlayBack);	// �t�Đ��󋵂̐ݒ�
	int  GetLoopAnimation(void) const;				// �p�^�[���J��Ԃ����̎擾

private:
	// �����o�ϐ�
	int m_nCounter;		// �A�j���[�V�����J�E���^�[
	int m_nCntChange;	// �p�^�[���ύX�J�E���g
	int m_nPattern;		// �A�j���[�V�����p�^�[��
	int m_nMaxPtrn;		// �p�^�[���̑���
	int m_nWidthPtrn;	// �e�N�X�`���̉��̕�����
	int m_nHeightPtrn;	// �e�N�X�`���̏c�̕�����
	int m_nNumLoop;		// �p�^�[���J��Ԃ���
	bool m_bStop;		// ��~��
	bool m_bPlayBack;	// �t�Đ���
};

#endif	// _ANIM2D_H_
