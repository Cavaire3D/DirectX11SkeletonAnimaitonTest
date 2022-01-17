//=============================================================================
//
// �G���W������ [Engine.cpp]
// Author : GP11B183 28 �N �B�� (�p���@�C�^)
//
//=============================================================================
#include "Engine.h"
#include <tchar.h>
#include <atlstr.h>

//=============================================================================
// �G���W������������
//=============================================================================
bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	timer.Start();

	if (!this->render_window.Initialize(this, hInstance, window_title, window_class, width, height))
		return false;

	if (!gfx.Initialize(this->render_window.GetHWND(), width, height))
		return false;
	
	//change input setting
	//LoadKeyboardLayout(_T("0x0409"), KLF_ACTIVATE);

	HKL defaultLanguageLayout = LoadKeyboardLayout(L"0x04090409", KLF_SUBSTITUTE_OK);
	DWORD dwThreadID = GetCurrentThreadId();
	HKL hCurKeyboard = GetKeyboardLayout(dwThreadID);
	if (hCurKeyboard != defaultLanguageLayout) 
	{
		UINT i;
		HKL hklCurrent;
		UINT uLayouts;
		HKL * lpList;

		uLayouts = GetKeyboardLayoutList(0, NULL);
		lpList = (HKL*)malloc(uLayouts * sizeof(HKL));
		uLayouts = GetKeyboardLayoutList(uLayouts, lpList);

		for (i = 0; i < uLayouts; i++)
		{
			hklCurrent = *(lpList + i);
			if (hklCurrent == defaultLanguageLayout)
			{
				ActivateKeyboardLayout(hklCurrent, 0);
			}
		}
	}
}

//=============================================================================
// ���b�Z�[�W����
//=============================================================================
bool Engine::ProcessMessages()
{
	return this->render_window.ProcessMessages();
}

//=============================================================================
// �X�V����
//=============================================================================
void Engine::Update()
{
	// ���Ԃ̏�����
	float dt = timer.GetMilisecondsElapsed();
	timer.Restart();

	auto cameratype = gfx.Camera3D.cameratype;

	//gfx.gameObject.AdjustRotation(XMFLOAT3(0, 0.001, 0));

	// �L�[�{�[�h�̓ǂݍ���
	while (!keyboard.CharBufferIsEmpty())
	{
		unsigned char ch = keyboard.ReadChar();
	}

	while (!keyboard.KeyBufferIsEmpty())
	{
		KeyboardEvent kbe = keyboard.ReadKey();
		unsigned char keycode = kbe.GetKeyCode();
		if (kbe.IsPress())
		{
			if (keycode == 'Q')
			{
				auto index = gfx.gameObject.GetCurrentAnimationClipIndex();
				//auto max = gfx.gameObject.GetAnimationClipSize();
				if (index == 4)
				{
					gfx.gameObject.PlayAnimation(0, AnimationPlayStyle::PlayOnce);
				}
			}
			if (keycode == 'E')
			{
				auto index = gfx.gameObject.GetCurrentAnimationClipIndex();
				if (index != 4 && index != 5)
				{
					gfx.gameObject.PlayAnimation(5, AnimationPlayStyle::PlayOnce);//
				}
			}
			if (keycode == 'P')
			{
				auto playerblade = gfx.gameObject.GetBladeCollsionObject();
				auto playerbody = gfx.gameObject.GetBodyCollsionObject();
				auto enemyblade = gfx.enemy.GetBladeCollsionObject();
				auto enemybody = gfx.enemy.GetBodyCollsionObject();
				playerblade->debugmeshflag = !playerblade->debugmeshflag;
				playerbody->debugmeshflag = !playerbody->debugmeshflag;
				enemyblade->debugmeshflag = !enemyblade->debugmeshflag;
				enemybody->debugmeshflag = !enemybody->debugmeshflag;
			}
			if (keycode == 'O')
			{
				gfx.showImgui = !gfx.showImgui;
			}
			if (keycode == 'I')
			{
				gfx.gameObject.ShowBones();
				gfx.enemy.ShowBones();
			}
			if (keycode == 'U')
			{
				gfx.skybox.ChangeSkyBoxSRV();
				gfx.InitializeIBLStatus();
			}
			if (keycode == 'R')
			{
				gfx.ResetGame();
			}
		}
	}

	 //�}�E�X�̓ǂݍ���
	while (!mouse.EventBufferIsEmpty())
	{
		MouseEvent me = mouse.ReadEvent();

		if (mouse.IsRightDown())
		{
			if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
			{
				this->gfx.Camera3D.AdjustRotation((float)me.GetPosY() * 0.005f, (float)me.GetPosX() * 0.005f, 0);
			}
		}
	}

	float Camera3DSpeed = 0.06f;

	//�Q�[���I�u�W�F�N�g�ɂ��Ă̏���
	gfx.enemy.Update(dt, gfx.Camera3D.GetViewMatrix() *gfx.Camera3D.GetProjectionMatrix());
	gfx.gameObject.Update(dt, gfx.Camera3D.GetViewMatrix() *gfx.Camera3D.GetProjectionMatrix());

	if (keyboard.KeyIsPressed('Y'))
	{
		gfx.gameObject.SetSkeletonDebugFlag();
	}

	if (cameratype == 0)
	{
		if (keyboard.KeyIsPressed('W'))
		{
			this->gfx.Camera3D.AdjustPosition(this->gfx.Camera3D.GetForwardVector() * Camera3DSpeed * dt);
		}
		if (keyboard.KeyIsPressed('S'))
		{
			this->gfx.Camera3D.AdjustPosition(this->gfx.Camera3D.GetBackwardVector() * Camera3DSpeed * dt);
		}
		if (keyboard.KeyIsPressed('A'))
		{
			this->gfx.Camera3D.AdjustPosition(this->gfx.Camera3D.GetLeftVector() * Camera3DSpeed * dt);
		}
		if (keyboard.KeyIsPressed('D'))
		{
			this->gfx.Camera3D.AdjustPosition(this->gfx.Camera3D.GetRightVector() * Camera3DSpeed * dt);
		}
		if (keyboard.KeyIsPressed(VK_SPACE))
		{
			this->gfx.Camera3D.AdjustPosition(0.0f, Camera3DSpeed * dt, 0.0f);
		}
		if (keyboard.KeyIsPressed('Z'))
		{
			this->gfx.Camera3D.AdjustPosition(0.0f, -Camera3DSpeed * dt, 0.0f);
		}
	}
	else if (cameratype == 1)
	{
		//gfx.Camera3D.SetLookAtPos(gfx.gameObject.GetPositionFloat3());
	}

	if (keyboard.KeyIsPressed('V')) {
		gfx.Camera3D.ChangeFocusMode(1, &gfx.gameObject);
	}
	

	if (keyboard.KeyIsPressed('C'))
	{
		XMVECTOR lightPosition = this->gfx.Camera3D.GetPositionVector();
		lightPosition += this->gfx.Camera3D.GetForwardVector();
		this->gfx.light.SetPosition(lightPosition);
		this->gfx.light.SetRotation(this->gfx.Camera3D.GetRotationFloat3());
	}

	//collision
	//�Փ˔���
	auto playerblade = gfx.gameObject.GetBladeCollsionObject();
	auto playerbody = gfx.gameObject.GetBodyCollsionObject();
	auto enemyblade = gfx.enemy.GetBladeCollsionObject();
	auto enemybody = gfx.enemy.GetBodyCollsionObject();

	DirectX::ContainmentType coresult = playerblade->aabb.Contains(enemybody->aabb);
	if (precoreslut == 0 && coresult != 0)
	{
		//hit

		gfx.enemy.hp -= 30;
		if (gfx.enemy.hp >= 0)
		{
			gfx.enemy.PlayAnimation(1, AnimationPlayStyle::PlayOnce);
		}
		else
		{
			auto index = gfx.enemy.GetCurrentAnimationClipIndex();
			if (index != 7)
			{
				gfx.start_dissolve_animation = true;
				gfx.enemy.PlayAnimation(7, AnimationPlayStyle::PlayOnce);
			}
		}
	}
	precoreslut = coresult;

	if (gfx.start_dissolve_animation)
		gfx.dissolveTheradhold += gfx.increaserate;
}

//=============================================================================
// �`�揈��
//=============================================================================
void Engine::RenderFrame()
{
	this->gfx.RenderFrame();
}
