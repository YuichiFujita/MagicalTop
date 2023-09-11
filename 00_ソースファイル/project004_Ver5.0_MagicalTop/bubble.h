//============================================================
//
//	�o�u���w�b�_�[ [bubble.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _BUBBLE_H_
#define _BUBBLE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectModel.h"

//************************************************************
//	�N���X��`
//************************************************************
// �o�u���N���X
class CBubble : public CObjectModel
{
public:
	// ���f����
	typedef enum
	{
		MODEL_BUBBLE = 0,	// �o�u��
		MODEL_BOLD,			// �����
		MODEL_MAX			// ���̗񋓌^�̑���
	}MODEL;

	// ��ԗ�
	typedef enum
	{
		STATE_NORMAL = 0,	// �ʏ���
		STATE_CHANGE,		// �T�C�Y�ϓ����
		STATE_MAX			// ���̗񋓌^�̑���
	}STATE;

	// �R���X�g���N�^
	CBubble(const int nMaxLevel, const D3DXVECTOR3& rMaxScale);

	// �f�X�g���N�^
	~CBubble();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	float GetRadius(void) const;	// ���a�擾

	// �ÓI�����o�֐�
	static CBubble *Create	// ����
	( // ����
		CObject *pObject,		// �e�I�u�W�F�N�g
		const int nMaxLevel,	// �ő僌�x��
		const D3DXVECTOR3& rMaxScale,	// �ő�g�嗦
		const D3DXVECTOR3& rPos,		// �ʒu
		const float fPosUp,				// Y�ʒu���Z��
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXVECTOR3& rScale = VEC3_ONE	// �g�嗦
	);

	// �����o�֐�
	void SetPosition(const D3DXVECTOR3& rPos);	// �ʒu�ݒ�
	void SetRotation(const D3DXVECTOR3& rRot);	// �����ݒ�
	void SetScaling(const D3DXVECTOR3& rScale);	// �傫���ݒ�
	void AddLevel(const int nAdd);				// ���x�����Z
	void SetLevel(const int nNum);				// ���x���ݒ�
	int GetLevel(void) const;					// ���x���擾
	float GetMaxRadius(void) const;				// �ő唼�a�擾
	int GetMaxLevel(void) const;				// �ő僌�x���擾
	void SetPositionUp(const float fUp);		// Y�ʒu���Z�ʐݒ�
	float GetPositionUp(void) const;			// Y�ʒu���Z�ʎ擾

private:
	// �����o�֐�
	void SetParentObject(CObject *pObject);	// �e�I�u�W�F�N�g�ݒ�

	// �ÓI�����o�ϐ�
	static const char *mc_apModelFile[];	// ���f���萔

	// �����o�ϐ�
	CObjectModel *m_pBold;		// ����胂�f��
	D3DXVECTOR3 m_currentScale;	// ���݂̊g�嗦
	D3DXVECTOR3 m_destScale;	// �ڕW�̊g�嗦
	CObject *m_pParentObject;	// �e�I�u�W�F�N�g
	float m_fPosUp;	// �o�u����Y�ʒu���Z��
	int m_nLevel;	// �傫�����x��

	const D3DXVECTOR3 m_maxScale;	// �ő�g�嗦�萔
	const int m_nMaxLevel;			// �ő僌�x���萔
};

#endif	// _BUBBLE_H_
