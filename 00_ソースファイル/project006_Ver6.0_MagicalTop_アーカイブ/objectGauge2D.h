//============================================================
//
//	�I�u�W�F�N�g�Q�[�W2D�w�b�_�[ [objectGauge2D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECTGAUGE2D_H_
#define _OBJECTGAUGE2D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g�Q�[�W2D�N���X
class CObjectGauge2D : public CObject
{
public:
	// ��ԗ�
	typedef enum
	{
		STATE_NONE = 0,	// �ʏ���
		STATE_CHANGE,	// �Q�[�W�ϓ����
		STATE_MAX		// ���̗񋓌^�̑���
	}STATE;

	// �|���S����
	typedef enum
	{
		POLYGON_BACK = 0,	// �w�i
		POLYGON_FRONT,		// �Q�[�W
		POLYGON_FRAME,		// �g
		POLYGON_MAX			// ���̗񋓌^�̑���
	}POLYGON;

	// �t���[����ޗ�
	typedef enum
	{
		TYPE_NONE = 0,	// �t���[������
		TYPE_EXP,		// �o���l�t���[��
		TYPE_MAX		// ���̗񋓌^�̑���
	}TYPE;

	// �R���X�g���N�^
	CObjectGauge2D();
	CObjectGauge2D(const int nFrame, const CObject::LABEL label, const int nPriority = DEFAULT_PRIO);

	// �f�X�g���N�^
	~CObjectGauge2D();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CObjectGauge2D *Create	// ����
	( // ����
		const CObject::LABEL label,	// �I�u�W�F�N�g���x��
		const int nMax,				// �ő�\���l
		const int nFrame,			// �\���l�ϓ��t���[��
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rSizeGauge = VEC3_ONE,	// �Q�[�W�傫��
		const D3DXCOLOR& rColFront = XCOL_WHITE,	// �\�Q�[�W�F
		const D3DXCOLOR& rColBack = XCOL_BLACK,		// ���Q�[�W�F
		const bool bDrawFrame = false,				// �g�`���
		const TYPE frameType = TYPE_NONE,			// �g���
		const D3DXVECTOR3& rSizeFrame = VEC3_ONE	// �g�傫��
	);

	// �����o�֐�
	void AddNum(const int nAdd);	// �Q�[�W���Z
	void SetNum(const int nNum);	// �Q�[�W�ݒ�
	int  GetNum(void) const;		// �Q�[�W�擾
	void SetMaxNum(const int nMax);	// �Q�[�W�ő�l�ݒ�
	int  GetMaxNum(void) const;		// �Q�[�W�ő�l�擾
	void BindTexture(const int nPolygonID, const int nTextureID);	// �e�N�X�`������
	void SetPosition(const D3DXVECTOR3& rPos);		// �ʒu�ݒ�
	void SetScalingGauge(const D3DXVECTOR3& rSize);	// �Q�[�W�傫���ݒ�
	void SetScalingFrame(const D3DXVECTOR3& rSize);	// �w�i�傫���ݒ�
	void SetColorFront(const D3DXCOLOR& rCol);		// �\�Q�[�W�F�ݒ�
	void SetColorBack(const D3DXCOLOR& rCol);		// ���Q�[�W�F�ݒ�
	void SetEnableDrawFrame(const bool bDraw);		// �g�\���󋵐ݒ�
	D3DXVECTOR3 GetPosition(void) const;			// �ʒu�擾
	D3DXVECTOR3 GetScalingGauge(void) const;		// �Q�[�W�傫���擾
	D3DXVECTOR3 GetScalingFrame(void) const;		// �w�i�傫���擾
	D3DXCOLOR GetColorFront(void) const;			// �\�Q�[�W�F�擾
	D3DXCOLOR GetColorBack(void) const;				// ���Q�[�W�F�擾
	bool GetEnableDrawFrame(void) const;			// �g�\���󋵎擾

private:
	// �����o�֐�
	void SetVtx(void);	// ���_���̐ݒ�

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3	m_pos;					// �ʒu
	D3DXVECTOR3	m_sizeGauge;			// �Q�[�W�傫��
	D3DXVECTOR3	m_sizeFrame;			// �g�傫��
	D3DXCOLOR	m_colFront;				// �\�Q�[�W�F
	D3DXCOLOR	m_colBack;				// ���Q�[�W�F
	STATE m_state;						// ���
	bool m_bDrawFrame;					// �g�\����
	float m_fChange;					// �Q�[�W�ϓ���
	float m_fCurrentNumGauge;			// ���ݕ\���l
	float m_fAddRight;					// �������Z��
	int m_nCounterState;				// ��ԊǗ��J�E���^�[
	int m_nNumGauge;					// �\���l
	int m_nMaxNumGauge;					// �\���l�̍ő�l
	int m_aTextureID[POLYGON_MAX];		// �e�N�X�`���C���f�b�N�X
	const int m_nFrame;					// �\���l�̕ϓ��t���[���萔
};

#endif	// _OBJECTGAUGE2D_H_
