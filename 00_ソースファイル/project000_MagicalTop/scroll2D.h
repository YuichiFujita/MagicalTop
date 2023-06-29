//============================================================
//
//	�X�N���[��2D�w�b�_�[ [scroll2D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SCROLL2D_H_
#define _SCROLL2D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object2D.h"

//************************************************************
//	�N���X��`
//************************************************************
// �X�N���[��2D�N���X
class CScroll2D : public CObject2D
{
public:
	// �R���X�g���N�^
	CScroll2D();
	CScroll2D(const CObject::LABEL label, const int nPriority = DEFAULT_PRIO);

	// �f�X�g���N�^
	~CScroll2D();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �I�[�o�[���[�h�����I�[�o�[���C�h�֐�
	HRESULT Init	// ������
	( // ����
		const float fMoveU,	// �����W�̈ړ���
		const float fMoveV	// �c���W�̈ړ���
	);

	// �ÓI�����o�֐�
	static CScroll2D *Create	// ����
	( // ����
		const float fMoveU,			// �����W�̈ړ���
		const float fMoveV,			// �c���W�̈ړ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rSize = VEC3_ONE,	// �傫��
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXCOLOR& rCol = XCOL_WHITE		// �F
	);

	// �����o�֐�
	void SetPosition(const D3DXVECTOR3& rPos);	// �ʒu�ݒ�
	void SetRotation(const D3DXVECTOR3& rRot);	// �����ݒ�
	void SetScaling(const D3DXVECTOR3& rSize);	// �傫���ݒ�
	void SetColor(const D3DXCOLOR& rCol);		// �F�ݒ�
	void SetTexU(const float fTexU);			// �����W�̊J�n�ʒu�ݒ�
	void SetTexV(const float fTexV);			// �c���W�̊J�n�ʒu�ݒ�
	void SetMoveU(const float fMoveU);			// �����W�̈ړ��ʐݒ�
	void SetMoveV(const float fMoveV);			// �c���W�̈ړ��ʐݒ�

private:
	// �����o�ϐ�
	float m_fTexU;	// �e�N�X�`�������W�̊J�n�ʒu
	float m_fTexV;	// �e�N�X�`���c���W�̊J�n�ʒu
	float m_fMoveU;	// �e�N�X�`�������W�̈ړ���
	float m_fMoveV;	// �e�N�X�`���c���W�̈ړ���
};

#endif	// _SCROLL2D_H_
