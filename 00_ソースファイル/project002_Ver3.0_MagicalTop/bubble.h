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
		MODEL_NORMAL = 0,	// �ʏ�o�u��
		MODEL_MAX			// ���̗񋓌^�̑���
	}MODEL;

	// �R���X�g���N�^
	CBubble(const int nMaxNum, const D3DXVECTOR3& rMaxScale);

	// �f�X�g���N�^
	~CBubble();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CBubble *Create	// ����
	( // ����
		CObject *pObject,	// �e�I�u�W�F�N�g
		const int nMaxNum,	// �ő�l
		const D3DXVECTOR3& rMaxScale,	// �ő�g�嗦
		const D3DXVECTOR3& rPos,		// �ʒu
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXVECTOR3& rScale = VEC3_ONE	// �g�嗦
	);

	// �����o�֐�
	void AddHitNum(const int nAdd);	// �q�b�g�����Z
	void SetHitNum(const int nNum);	// �q�b�g���ݒ�
	int  GetHitNum(void) const;		// �q�b�g���擾

private:
	// �����o�֐�
	void SetParentObject(CObject *pObject);	// �e�I�u�W�F�N�g�ݒ�

	// �ÓI�����o�ϐ�
	static const char *mc_apModelFile[];	// ���f���萔

	// �����o�ϐ�
	CObject *m_pParentObject;		// �e�I�u�W�F�N�g
	const D3DXVECTOR3 m_maxScale;	// �ő�g�嗦�萔
	int m_nHitNum;			// �o�u���q�b�g��
	const int m_nMaxNum;	// �ő�l�萔
};

#endif	// _BUBBLE_H_
