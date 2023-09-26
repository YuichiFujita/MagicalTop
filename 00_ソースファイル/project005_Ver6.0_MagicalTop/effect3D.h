//============================================================
//
//	�G�t�F�N�g3D�w�b�_�[ [effect3D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EFFECT3D_H_
#define _EFFECT3D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectBillboard.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�t�F�N�g3D�N���X
class CEffect3D : public CObjectBillboard
{
public:
	// ��ޗ�
	typedef enum
	{
		TYPE_NORMAL = 0,	// �ʏ�e�N�X�`��
		TYPE_BUBBLE,		// �o�u���e�N�X�`��
		TYPE_SMOKE,			// ���e�N�X�`��
		TYPE_HEAL,			// �񕜃e�N�X�`��
		TYPE_LEAF,			// �t���σe�N�X�`��
		TYPE_PIECE_S,		// ������e�N�X�`�� (��)
		TYPE_PIECE_M,		// ������e�N�X�`�� (��)
		TYPE_PIECE_L,		// ������e�N�X�`�� (��)
		TYPE_MAX			// ���̗񋓌^�̑���
	}TYPE;

	// �R���X�g���N�^
	CEffect3D();
	CEffect3D(const TYPE type, const LABEL label);

	// �f�X�g���N�^
	~CEffect3D();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CEffect3D *Create	// ����
	( // ����
		const TYPE type,					// �e�N�X�`��
		const D3DXVECTOR3& rPos,			// �ʒu
		const D3DXVECTOR3& rMove,			// �ړ���
		const D3DXVECTOR3& rRot,			// ����
		const D3DXCOLOR& rCol,				// �F
		const int nLife,					// ����
		const float fRadius,				// ���a
		const float fSubSize,				// ���a�̌��Z��
		const bool bAdd = true,				// ���Z������
		const LABEL label = LABEL_EFFECT3D	// �I�u�W�F�N�g���x��
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
	bool	m_bAdd;			// ���Z������
};

#endif	// _EFFECT3D_H_
