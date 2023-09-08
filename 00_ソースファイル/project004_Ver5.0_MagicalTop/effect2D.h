//============================================================
//
//	�G�t�F�N�g2D�w�b�_�[ [effect2D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EFFECT2D_H_
#define _EFFECT2D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object2D.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�t�F�N�g2D�N���X
class CEffect2D : public CObject2D
{
public:
	// ��ޗ�
	typedef enum
	{
		TYPE_NORMAL = 0,	// �ʏ�e�N�X�`��
		TYPE_MAX			// ���̗񋓌^�̑���
	}TYPE;

	// �R���X�g���N�^
	CEffect2D();
	CEffect2D(const TYPE type, const LABEL label);

	// �f�X�g���N�^
	~CEffect2D();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CEffect2D *Create	// ����
	( // ����
		const TYPE type,					// �e�N�X�`��
		const D3DXVECTOR3& rPos,			// �ʒu
		const D3DXVECTOR3& rMove,			// �ړ���
		const D3DXVECTOR3& rRot,			// ����
		const D3DXCOLOR& rCol,				// �F
		const int nLife,					// ����
		const float fRadius,				// ���a
		const float fSubSize,				// ���a�̌��Z��
		const float fSubAlpha,				// �����x�̌��Z��
		const LABEL label = LABEL_EFFECT2D	// �I�u�W�F�N�g���x��
	);

private:
	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	D3DXVECTOR3	m_move;		// �ړ���
	const TYPE	m_type;		// ��ޒ萔
	int		m_nLife;		// ����
	float	m_fSubSize;		// �傫���̌��Z��
	float	m_fSubAlpha;	// �����x�̌��Z��
};

#endif	// _EFFECT2D_H_
