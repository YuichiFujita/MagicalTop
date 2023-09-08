//============================================================
//
//	���w�b�_�[ [wind.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _WIND_H_
#define _WIND_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectOrbit.h"

//************************************************************
//	�N���X��`
//************************************************************
// ���N���X
class CWind : public CObjectOrbit
{
public:
	// �e�N�X�`����
	typedef enum
	{
		TEXTURE_NORMAL = 0,	// �ʏ�e�N�X�`��
		TEXTURE_MAX			// ���̗񋓌^�̑���
	}TEXTURE;

	// ��ԗ�
	typedef enum
	{
		STATE_NORMAL = 0,	// �ʏ���
		STATE_DELETE,		// �������
		STATE_MAX			// ���̗񋓌^�̑���
	}STATE;

	// �R���X�g���N�^
	CWind();

	// �f�X�g���N�^
	~CWind();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CWind *Create(const D3DXVECTOR3& rPos);	// ����

	// �����o�ϐ�
	void SetPosition(const D3DXVECTOR3& rPos);	// �ʒu�ݒ�
	D3DXMATRIX *GetPtrMtxWorld(void);			// �}�g���b�N�X�|�C���^�擾

private:
	// �����o�ϐ�
	D3DXMATRIX	m_mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3	m_pos;		// �ʒu
	STATE m_state;			// ���
};

#endif	// _WIND_H_
