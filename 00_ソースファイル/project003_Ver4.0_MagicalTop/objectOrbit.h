//============================================================
//
//	�I�u�W�F�N�g�O�Ճw�b�_�[ [objectOrbit.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECT_ORBIT_H_
#define _OBJECT_ORBIT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_OFFSET	(2)		// �I�t�Z�b�g�̐�
#define MAX_VERTEX	(40)	// �ێ����钸�_�̍ő吔

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g�O�ՃN���X
class CObjectOrbit : public CObject
{
public:
	// �I�t�Z�b�g��
	enum OFFSET
	{
		OFFSET_NORMAL = 0,	// �ʏ�I�t�Z�b�g
		OFFSET_ROD,			// ��I�t�Z�b�g
		OFFSET_MAX,			// ���̗񋓌^�̑���
	};

	// ��ޗ�
	enum TYPE
	{
		TYPE_NORMAL = 0,	// �ʏ�e�N�X�`��
		TYPE_MAX,			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CObjectOrbit();
	CObjectOrbit(const CObject::LABEL label, const int nPriority = DEFAULT_PRIO);

	// �f�X�g���N�^
	~CObjectOrbit();

	// �O�Ս\����
	typedef struct
	{
		D3DXVECTOR3	aOffset[MAX_OFFSET];		// ���[�̃I�t�Z�b�g
		D3DXCOLOR	aCol[MAX_OFFSET];			// ���[�̊�F
		D3DXMATRIX	aMtxWorldPoint[MAX_OFFSET];	// ���[�̃��[���h�}�g���b�N�X
		D3DXVECTOR3	aPosPoint[MAX_VERTEX];		// �v�Z��̊e���_���W
		D3DXCOLOR	aColPoint[MAX_VERTEX];		// �e���_�J���[
		D3DXMATRIX	*pMtxParent;				// �e�̃}�g���b�N�X
		int nTexPart;	// �e�N�X�`��������
		bool bInit;		// ��������
	}Orbit;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CObjectOrbit *Create	// ����
	( // ����
		D3DXMATRIX *pMtxParent,	// �e�}�g���b�N�X
		const D3DXCOLOR& rCol,	// �F
		const OFFSET offset,	// �I�t�Z�b�g
		const TYPE type,		// ���
		const int nTexPart = 1	// �e�N�X�`��������
	);

	// �����o�֐�
	void BindTexture(const int nTextureID);			// �e�N�X�`������
	void SetMatrixParent(D3DXMATRIX *pMtxParent);	// �e�̃}�g���b�N�X�ݒ�
	void SetColor(const D3DXCOLOR& rCol);			// �F�ݒ�
	void SetOffset(const OFFSET offset);			// �I�t�Z�b�g�ݒ�
	void SetTexPart(const int nTexPart);			// �e�N�X�`���������ݒ�
	void SetEnableInit(const bool bInit);			// �������󋵐ݒ�
	HRESULT SetLength(void);						// �����ݒ�

protected:
	// �����o�֐�
	void SetVtx(void);	// ���_���̐ݒ�

private:
	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔
	static const D3DXVECTOR3 mc_aOffset[][MAX_OFFSET];	// �I�t�Z�b�g�̈ʒu������

	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	Orbit m_orbit;		// �O�Ղ̏��
	int m_nNumVtx;		// �K�v���_��
	int m_nTextureID;	// �e�N�X�`���C���f�b�N�X
};

#endif	// _OBJECT_ORBIT_H_
