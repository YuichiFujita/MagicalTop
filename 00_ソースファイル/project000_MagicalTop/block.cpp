//============================================================
//
//	�u���b�N���� [block.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "block.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�}�N����`
//************************************************************
#define BLOCK_SETUP_TXT	"data\\TXT\\block.txt"	// �Z�b�g�A�b�v�e�L�X�g���΃p�X
#define BLOCK_PRIO	(1)	// �u���b�N�̗D�揇��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CObjectMeshCube::MeshCube CBlock::m_blockInfo = {};			// �u���b�N�X�e�[�^�X���
CBlock::Type CBlock::m_aTypeInfo[CBlock::TYPE_MAX] = {};	// �e�N�X�`����ޏ��

//************************************************************
//	�q�N���X [CBlock] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CBlock::CBlock() : CObjectMeshCube(CObject::LABEL_BLOCK, BLOCK_PRIO)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CBlock::~CBlock()
{

}

//============================================================
//	����������
//============================================================
HRESULT CBlock::Init(void)
{
	// �I�u�W�F�N�g���b�V���L���[�u�̏�����
	if (FAILED(CObjectMeshCube::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CBlock::Uninit(void)
{
	// �I�u�W�F�N�g���b�V���L���[�u�̏I��
	CObjectMeshCube::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CBlock::Update(void)
{
	// �I�u�W�F�N�g���b�V���L���[�u�̍X�V
	CObjectMeshCube::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CBlock::Draw(void)
{
	// �I�u�W�F�N�g���b�V���L���[�u�̕`��
	CObjectMeshCube::Draw();
}

//============================================================
//	�u���b�N�̃X�e�[�^�X�擾����
//============================================================
CBlock::MeshCube CBlock::GetBlockInfo(void)
{
	// �u���b�N�̃X�e�[�^�X����Ԃ�
	return m_blockInfo;
}

//============================================================
//	��������
//============================================================
CBlock *CBlock::Create
(
	const TYPE type,		// ���
	const D3DXVECTOR3& rPos	// �ʒu
)
{
	// �|�C���^��錾
	CBlock *pBlock = NULL;	// �u���b�N�����p

	if (UNUSED(pBlock))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pBlock = new CBlock;	// �u���b�N
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pBlock))
	{ // �m�ۂɐ������Ă���ꍇ

		if (SUCCEEDED(pBlock->GetResult()))
		{ // �I�u�W�F�N�g�̏��ݒ�ɐ��������ꍇ

			// �u���b�N�̏�����
			if (FAILED(pBlock->Init()))
			{ // �������Ɏ��s�����ꍇ

				// �������J��
				delete pBlock;
				pBlock = NULL;

				// ���s��Ԃ�
				return NULL;
			}

			// �e�N�X�`��������
			pBlock->BindTexture(m_aTypeInfo[type].texture);

			// �ʒu��ݒ�
			pBlock->SetPosition(rPos);

			// ������ݒ�
			pBlock->SetRotation(m_blockInfo.rot);

			// �傫����ݒ�
			pBlock->SetScaling(m_blockInfo.size);

			// �L���[�u�F��ݒ�
			pBlock->SetCubeColor(m_blockInfo.aCol[CUBECOL_CUBE]);

			// �����F��ݒ�
			pBlock->SetBorderColor(m_blockInfo.aCol[CUBECOL_BORDER]);

			// �����̏�Ԃ�ݒ�
			if (FAILED(pBlock->SetBorderState(m_blockInfo.bordState)))
			{ // �������̐ݒ�Ɏ��s�����ꍇ

				// �������J��
				delete pBlock;
				pBlock = NULL;

				// ���s��Ԃ�
				return NULL;
			}

			// �����̑�����ݒ�
			pBlock->SetBorderThick(m_blockInfo.fBordThick);

			// �e�N�X�`���̏�Ԃ�ݒ�
			pBlock->SetTextureState(m_aTypeInfo[type].state);

			// �e�N�X�`���̕�����X��ݒ�
			pBlock->SetTexturePatternX(m_blockInfo.aTexPart[CUBEPART_X]);

			// �e�N�X�`���̕�����Y��ݒ�
			pBlock->SetTexturePatternY(m_blockInfo.aTexPart[CUBEPART_Y]);

			// �e�N�X�`���̕�����Z��ݒ�
			pBlock->SetTexturePatternZ(m_blockInfo.aTexPart[CUBEPART_Z]);

			// �J�����O��ݒ�
			pBlock->SetCulling(m_blockInfo.cull);

			// ���C�e�B���O��ݒ�
			pBlock->SetLighting(m_blockInfo.bLight);

			// �m�ۂ����A�h���X��Ԃ�
			return pBlock;
		}
		else
		{ // �I�u�W�F�N�g�̏��ݒ�Ɏ��s�����ꍇ

			// �������J��
			delete pBlock;
			pBlock = NULL;

			// ���s��Ԃ�
			return NULL;
		}
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
void CBlock::LoadSetup(void)
{
	// �ϐ���錾
	FACETEX texType;			// ��ނ̓ǂݍ��ݑ���p
	int nCurrentType = 0;		// ���݂̎�ނ̓ǂݍ��ݐ�
	int nNumTexture = 0;		// �e�N�X�`���̓ǂݍ��ݐ��̑���p
	int nCurrentTexture = 0;	// ���݂̃e�N�X�`���ǂݍ��ݐ�
	int nLight = 0;				// ���C�e�B���O��ON/OFF�̕ϊ��p
	int nEnd = 0;				// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^
	int *pTextureID = NULL;		// �e�N�X�`���C���f�b�N�X�|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(BLOCK_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			// �e�N�X�`���̓Ǎ�
			if (strcmp(&aString[0], "TEXTURELOAD") == 0)
			{ // �ǂݍ��񂾕����� TEXTURELOAD �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_TEXTURELOAD �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "NUM_TEXTURE") == 0)
					{ // �ǂݍ��񂾕����� NUM_TEXTURE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nNumTexture);	// �e�N�X�`���t�@�C������ǂݍ���

						if (UNUSED(pTextureID))
						{ // �|�C���^���g�p����Ă��Ȃ��ꍇ

							// �e�N�X�`���t�@�C�����ێ������������m��
							pTextureID = new int[nNumTexture];

							// ��O����
							assert(USED(pTextureID));	// ��g�p��
						}
						else { assert(false); }	// �g�p��
					}
					else if (strcmp(&aString[0], "TEXTURE_FILENAME") == 0)
					{ // �ǂݍ��񂾕����� TEXTURE_FILENAME �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%s", &aString[0]);	// �e�N�X�`���t�@�C������ǂݍ���

						// ��O����
						assert(nCurrentTexture < nNumTexture);	// �e�N�X�`���t�@�C���I�[�o�[

						// �e�N�X�`����o�^
						pTextureID[nCurrentTexture] = pTexture->Regist(&aString[0]);

						// �e�N�X�`���ǂݍ��ݐ������Z
						nCurrentTexture++;
					}
				} while (strcmp(&aString[0], "END_TEXTURELOAD") != 0);	// �ǂݍ��񂾕����� END_TEXTURELOAD �ł͂Ȃ��ꍇ���[�v
			}

			// ��{���̐ݒ�
			else if (strcmp(&aString[0], "BASICSET") == 0)
			{ // �ǂݍ��񂾕����� BASICSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_BASICSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					// ��{���̐ݒ�
					if (strcmp(&aString[0], "ROT") == 0)
					{ // �ǂݍ��񂾕����� ROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &m_blockInfo.rot.x);	// X������ǂݍ���
						fscanf(pFile, "%f", &m_blockInfo.rot.y);	// Y������ǂݍ���
						fscanf(pFile, "%f", &m_blockInfo.rot.z);	// Z������ǂݍ���
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // �ǂݍ��񂾕����� SIZE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &m_blockInfo.size.x);	// X�傫����ǂݍ���
						fscanf(pFile, "%f", &m_blockInfo.size.y);	// Y�傫����ǂݍ���
						fscanf(pFile, "%f", &m_blockInfo.size.z);	// Z�傫����ǂݍ���
					}

					// �������̐ݒ�
					else if (strcmp(&aString[0], "BORDER") == 0)
					{ // �ǂݍ��񂾕����� BORDER �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &m_blockInfo.bordState);	// �����̎g�p�󋵂�ǂݍ���
					}
					else if (strcmp(&aString[0], "THICK") == 0)
					{ // �ǂݍ��񂾕����� THICK �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &m_blockInfo.fBordThick);	// �����̑�����ǂݍ���
					}

					// �\�����̐ݒ�
					else if (strcmp(&aString[0], "CULL") == 0)
					{ // �ǂݍ��񂾕����� CULL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &m_blockInfo.cull);	// �J�����O�󋵂�ǂݍ���
					}
					else if (strcmp(&aString[0], "LIGHT") == 0)
					{ // �ǂݍ��񂾕����� LIGHT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nLight);		// ���C�e�B���O��ON/OFF��ǂݍ���

						// �ǂݍ��񂾒l��bool�^�ɕϊ�
						m_blockInfo.bLight = (nLight == 0) ? true : false;
					}

					// �F�̐ݒ�
					if (strcmp(&aString[0], "COLORSET") == 0)
					{ // �ǂݍ��񂾕����� COLORSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_COLORSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "CUBE") == 0)
							{ // �ǂݍ��񂾕����� CUBE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_blockInfo.aCol[CUBECOL_CUBE].r);	// �L���[�u�F��ǂݍ���
								fscanf(pFile, "%f", &m_blockInfo.aCol[CUBECOL_CUBE].g);	// �L���[�u�F��ǂݍ���
								fscanf(pFile, "%f", &m_blockInfo.aCol[CUBECOL_CUBE].b);	// �L���[�u�F��ǂݍ���
								fscanf(pFile, "%f", &m_blockInfo.aCol[CUBECOL_CUBE].a);	// �L���[�u�����x��ǂݍ���
							}
							else if (strcmp(&aString[0], "BORDER") == 0)
							{ // �ǂݍ��񂾕����� BORDER �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_blockInfo.aCol[CUBECOL_BORDER].r);	// �����F��ǂݍ���
								fscanf(pFile, "%f", &m_blockInfo.aCol[CUBECOL_BORDER].g);	// �����F��ǂݍ���
								fscanf(pFile, "%f", &m_blockInfo.aCol[CUBECOL_BORDER].b);	// �����F��ǂݍ���
								fscanf(pFile, "%f", &m_blockInfo.aCol[CUBECOL_BORDER].a);	// ����蓧���x��ǂݍ���
							}
						} while (strcmp(&aString[0], "END_COLORSET") != 0);	// �ǂݍ��񂾕����� END_COLORSET �ł͂Ȃ��ꍇ���[�v
					}

					// �e�N�X�`���������̐ݒ�
					if (strcmp(&aString[0], "TEXPARTSET") == 0)
					{ // �ǂݍ��񂾕����� TEXPARTSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_TEXPARTSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "TEXPART_X") == 0)
							{ // �ǂݍ��񂾕����� TEXPART_X �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_blockInfo.aTexPart[CUBEPART_X].x);	// ���E�̉���������ǂݍ���
								fscanf(pFile, "%f", &m_blockInfo.aTexPart[CUBEPART_X].y);	// ���E�̏c��������ǂݍ���
							}
							else if (strcmp(&aString[0], "TEXPART_Y") == 0)
							{ // �ǂݍ��񂾕����� TEXPART_Y �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_blockInfo.aTexPart[CUBEPART_Y].x);	// �㉺�̉���������ǂݍ���
								fscanf(pFile, "%f", &m_blockInfo.aTexPart[CUBEPART_Y].y);	// �㉺�̏c��������ǂݍ���
							}
							else if (strcmp(&aString[0], "TEXPART_Z") == 0)
							{ // �ǂݍ��񂾕����� TEXPART_Z �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_blockInfo.aTexPart[CUBEPART_Z].x);	// �O��̉���������ǂݍ���
								fscanf(pFile, "%f", &m_blockInfo.aTexPart[CUBEPART_Z].y);	// �O��̏c��������ǂݍ���
							}
						} while (strcmp(&aString[0], "END_TEXPARTSET") != 0);	// �ǂݍ��񂾕����� END_TEXPARTSET �ł͂Ȃ��ꍇ���[�v
					}
				} while (strcmp(&aString[0], "END_BASICSET") != 0);	// �ǂݍ��񂾕����� END_BASICSET �ł͂Ȃ��ꍇ���[�v
			}

			// �e�N�X�`�����̐ݒ�
			else if (strcmp(&aString[0], "TEXTURESET") == 0)
			{ // �ǂݍ��񂾕����� TEXTURESET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_TEXTURESET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "BLOCKSET") == 0)
					{ // �ǂݍ��񂾕����� BLOCKSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_BLOCKSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							// �e�N�X�`���̎�ނ̐ݒ�
							if (strcmp(&aString[0], "TEXALL") == 0)
							{ // �ǂݍ��񂾕����� TEXALL �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &texType.All);	// �S�ʂ̃e�N�X�`����ǂݍ���

								// ��O����
								assert(nCurrentType < TYPE_MAX);	// �e�N�X�`����ރI�[�o�[

								// �e�N�X�`����o�^
								m_aTypeInfo[nCurrentType].texture = FACETEX(pTextureID[texType.All]);

								// �e�N�X�`����Ԃ�ݒ�
								m_aTypeInfo[nCurrentType].state = TEXSTATE_ONE;	// ����̃e�N�X�`�����g�p

								// ��ޓǂݍ��ݐ������Z
								nCurrentType++;
							}
							else if (strcmp(&aString[0], "TEXSET") == 0)
							{ // �ǂݍ��񂾕����� TEXSET �̏ꍇ

								do
								{ // �ǂݍ��񂾕����� END_TEXSET �ł͂Ȃ��ꍇ���[�v

									// �t�@�C�����當�����ǂݍ���
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "TEXLEFT") == 0)
									{ // �ǂݍ��񂾕����� TEXLEFT �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%d", &texType.Left);		// ���ʂ̃e�N�X�`����ǂݍ���
									}
									else if (strcmp(&aString[0], "TEXRIGHT") == 0)
									{ // �ǂݍ��񂾕����� TEXRIGHT �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%d", &texType.Right);	// �E�ʂ̃e�N�X�`����ǂݍ���
									}
									else if (strcmp(&aString[0], "TEXBOTTOM") == 0)
									{ // �ǂݍ��񂾕����� TEXBOTTOM �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%d", &texType.Bottom);	// ���ʂ̃e�N�X�`����ǂݍ���
									}
									else if (strcmp(&aString[0], "TEXTOP") == 0)
									{ // �ǂݍ��񂾕����� TEXTOP �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%d", &texType.Top);		// ��ʂ̃e�N�X�`����ǂݍ���
									}
									else if (strcmp(&aString[0], "TEXNEAR") == 0)
									{ // �ǂݍ��񂾕����� TEXNEAR �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%d", &texType.Near);		// �O�ʂ̃e�N�X�`����ǂݍ���
									}
									else if (strcmp(&aString[0], "TEXFAR") == 0)
									{ // �ǂݍ��񂾕����� TEXFAR �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%d", &texType.Far);		// ��ʂ̃e�N�X�`����ǂݍ���
									}
								} while (strcmp(&aString[0], "END_TEXSET") != 0);	// �ǂݍ��񂾕����� END_TEXSET �ł͂Ȃ��ꍇ���[�v

								// ��O����
								assert(nCurrentType < TYPE_MAX);	// �e�N�X�`����ރI�[�o�[

								// �e�N�X�`����o�^
								m_aTypeInfo[nCurrentType].texture = FACETEX
								( // ����
									pTextureID[texType.Left],	// ��
									pTextureID[texType.Right],	// �E
									pTextureID[texType.Bottom],	// ��
									pTextureID[texType.Top],	// ��
									pTextureID[texType.Near],	// �O
									pTextureID[texType.Far]		// ��
								);

								// �e�N�X�`����Ԃ�ݒ�
								m_aTypeInfo[nCurrentType].state = TEXSTATE_SELECT;	// ���ꂼ��̃e�N�X�`�����g�p

								// ��ޓǂݍ��ݐ������Z
								nCurrentType++;
							}
						} while (strcmp(&aString[0], "END_BLOCKSET") != 0);	// �ǂݍ��񂾕����� END_BLOCKSET �ł͂Ȃ��ꍇ���[�v
					}
				} while (strcmp(&aString[0], "END_TEXTURESET") != 0);		// �ǂݍ��񂾕����� END_TEXTURESET �ł͂Ȃ��ꍇ���[�v
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		if (USED(pTextureID))
		{ // �|�C���^���g�p���̏ꍇ

			// �������J��
			delete[] pTextureID;
		}
		else { assert(false); }	// ��g�p��

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�u���b�N�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}
