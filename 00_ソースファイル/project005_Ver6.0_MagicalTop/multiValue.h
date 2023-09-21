//============================================================
//
//	�}���`�����w�b�_�[ [multiValue.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _MULTIVALUE_H_
#define _MULTIVALUE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"
#include "value.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_DIGIT	(8)	// �����̍ő吔

//************************************************************
//	�O���錾
//************************************************************
class CValue;	// �����N���X

//************************************************************
//	�N���X��`
//************************************************************
// �}���`�����N���X
class CMultiValue : public CObject
{
public:
	// �R���X�g���N�^
	CMultiValue();

	// �f�X�g���N�^
	~CMultiValue();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	void SetEnableDraw(const bool bDraw);	// �`��󋵐ݒ�
	void SetPriority(const int nPriority);	// �D�揇�ʐݒ�

	// �ÓI�����o�֐�
	static CMultiValue *Create	// ����
	( // ����
		const CValue::TEXTURE texture,	// �e�N�X�`��
		const int nNum,					// ����
		const int nDigit,				// ����
		const D3DXVECTOR3& rPos,		// �ʒu
		const D3DXVECTOR3& rSize,		// �傫��
		const D3DXVECTOR3& rSpace,		// �s��
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXCOLOR& rCol = XCOL_WHITE		// �F
	);

	// �����o�֐�
	void AddNum(const int nNum);		// ���l���Z
	void SetNum(const int nNum);		// ���l�ݒ�
	HRESULT SetDigit(const int nDigit);	// �����ݒ�
	void SetMin(const int nMin);		// �ŏ��l�ݒ�
	void SetMax(const int nMax);		// �ő�l�ݒ�
	int GetNum(void) const;				// ���l�擾
	int GetDigit(void) const;			// �����擾
	int GetMin(void) const;				// �ŏ��l�擾
	int GetMax(void) const;				// �ő�l�擾

	void SetPosition(const D3DXVECTOR3& rPos);		// �ʒu�ݒ�
	void SetRotation(const D3DXVECTOR3& rRot);		// �����ݒ�
	void SetScaling(const D3DXVECTOR3& rSize);		// �傫���ݒ�
	void SetColor(const D3DXCOLOR& rCol);			// �F�ݒ�
	void SetSpace(const D3DXVECTOR3& rSpace);		// �s�Ԑݒ�
	void SetTexture(const CValue::TEXTURE texture);	// �e�N�X�`���ݒ�

	D3DXVECTOR3 GetPosition(void) const;	// �ʒu�擾
	D3DXVECTOR3 GetRotation(void) const;	// �����擾
	D3DXVECTOR3 GetScaling(void) const;		// �傫���擾
	D3DXCOLOR GetColor(void) const;			// �F�擾
	D3DXVECTOR3 GetSpace(void) const;		// �s�Ԏ擾

private:
	// �����o�֐�
	void SetNumPosition(void);	// �����̈ʒu�ݒ�
	void SetTexNum(void);		// �����̃e�N�X�`�����W�ݒ�

	// �����o�ϐ�
	CValue *m_apValue[MAX_DIGIT];	// ���l�̏��
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_space;	// ���l�̍s��
	int m_nNum;		// ����
	int m_nDigit;	// ����
	int m_nMin;		// �ŏ��l
	int m_nMax;		// �ő�l
};

#endif	// _MULTIVALUE_H_
