//============================================================
//
//	�n���P�[���w�b�_�[ [hurricane.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _HURRICANE_H_
#define _HURRICANE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectTornado.h"

//************************************************************
//	�N���X��`
//************************************************************
// �n���P�[���N���X
class CHurricane : public CObjectTornado
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
		STATE_MAX			// ���̗񋓌^�̑���
	}STATE;

	// �R���X�g���N�^
	CHurricane();

	// �f�X�g���N�^
	~CHurricane();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CHurricane *Create(const D3DXVECTOR3& rPos);	// ����

private:
	// �����o�ϐ�
	STATE m_state;	// ���
};

#endif	// _HURRICANE_H_
