#include "TitleScene.h"
#include "GameManager.h"

#include "GameScene/GameScene.h"
#include "Input.h"
#include "SampleScene/SampleScene.h"
#include <numbers>
TitleScene::TitleScene() {}

void TitleScene::Initialize() {
	playerModel_ = std::unique_ptr<Model>();
	uint32_t model = ModelManager::GetInstance()->LoadObject("Resources/GameResources/Player", "Player.obj");
	playerModel_.reset(Model::Create(model));

	uint32_t blackTextureHandle =
		TextureManager::GetInstance()->LoadTexture("Resources/Black.png");
	blackSprite_.reset(Sprite::Create(blackTextureHandle, { 0.0f, 0.0f }));

	playerWorldTransform_.Initialize();
	playerWorldTransform_.scale_ = { 1.0f, 1.0f, 1.0f };
	playerWorldTransform_.rotate_ = { 0.0f, 0.0f, 0.0f };
	playerWorldTransform_.translate_ = { 0.0f, 0.0f, 0.0f };



	playerVelocity_ = { 0.0f,0.0f,0.0f };

	// 生成
	skydome_ = new Skydome();

	// フォルダの名前を指定してね
	// 読み込むファイルと同じフォルダ名にしないと✕
	/////skydomeModel_ = Model::CreateFromOBJ("CelestialSphere", true);

	// テクスチャ読み込み
	// skydomeTextureHandle_ = TextureManager::Load("CelestialSphere/SpaceSphere.png");

	// 天球の初期化
	skydome_->Initialize();

	// タイトル
	titleLogoTexture = TextureManager::LoadTexture("Resources/Title/Logo/TitleLogo.png");
	// スプライトの生成
	titleLogoSprite_ = Sprite::Create(titleLogoTexture, { 0.0f, 0.0f });

	startHandle_ = TextureManager::GetInstance()->LoadTexture("Resources/Title/Logo/Start.png");
	start_ = Sprite::Create(startHandle_, { 0.0f, 0.0f });
	

	playerTranslate_ = { 0.0f, 0.0f, -30.0f };
	// cameraTranslate_ = { 0.0f,0.0f,0.0f };
	cameraRotate_ = { 0.0f, 0.0f, 0.0f };
	Camera::GetInstance()->SetTranslate(playerTranslate_);
	Camera::GetInstance()->SetRotate(cameraRotate_);

	// BGM
	bgm_ = Audio::GetInstance();
	bgmHandle_ = bgm_->LoadWave("Resources/Audio/Title/Music/Title.wav");
	bgm_->PlayWave(bgmHandle_, true);

	// DecideSE
	decideSE_ = Audio::GetInstance();
	seHandle_ = decideSE_->LoadWave("Resources/Audio/Title/Music/StartSE.wav");

	isFadeIn_ = true;
}


void TitleScene::Update(GameManager* gameManager) {
	// デバッグ用
	skydome_->Update();

	playerWorldTransform_.rotate_ = Add(playerWorldTransform_.rotate_, { 0.0f,playerModelRotate_,0.0f });
	playerWorldTransform_.translate_ = Add(playerWorldTransform_.translate_, playerVelocity_);
	playerWorldTransform_.Update();

	blackSprite_->SetTransparency(transparency_);

	const float RADIUS = 30.0f;

	/*theta_ = 0.001f;
	cameraTranslate_.z = RADIUS*std::sinf(theta_)+ initialCamerTranslate_;
	cameraTranslate_.x = RADIUS*std::cosf(theta_);
	const float INITIAL_CAMERA_ROTATE = float(std::numbers::pi) / 2.0f;
	cameraRotate_.y = -theta_;*/

	Camera::GetInstance()->SetTranslate(playerTranslate_);
	Camera::GetInstance()->SetRotate(cameraRotate_);

	const float TRANSPARENCY_INTERVAL = 0.05f;

	if (isFadeIn_ == true) {
		transparency_ -= TRANSPARENCY_INTERVAL;

		if (transparency_ < 0.0f) {
			isFadeIn_ = false;
		}
	}

	if (isFadeIn_ == false && isStart_ == false) {
		if (Input::GetInstance()->GetJoystickState(joyState_)) {
			if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
				triggerButtonTime_ += 1;
			}
		}
		if ((joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) {
			triggerButtonTime_ = 0;
		}

		//Playerのアニメーション
		playerMoveTheta_ += 0.1f;
		const float offset = 0.2f;
		playerWorldTransform_.translate_.y = float(std::sinf(playerMoveTheta_)) * offset;


		flashTime_ += 1;
		if (flashTime_ > 0) {
			if (flashTime_ > 0 && flashTime_ <= 30) {
				start_->SetInvisible(false);
			}
			if (flashTime_ > 30 && flashTime_ <= 60) {
				start_->SetInvisible(true);
			}
			if (flashTime_ > 60) {
				flashTime_ = 0;
			}
		}

		// スペースかBボタンでスタート
		if ((Input::GetInstance()->IsTriggerKey(DIK_SPACE) == true) ||
			triggerButtonTime_ == 1) {

			decideSE_->PlayWave(seHandle_, false);
			bgm_->StopWave(bgmHandle_);

			isStart_ = true;
		}
	}

	if (isStart_ == true) {
		fastFlashTime_ += 1;
		// 高速点滅処理
		const int FLASH_INTERVAL = 2;
		if (fastFlashTime_ > FLASH_INTERVAL * 0) {
			if (fastFlashTime_ > FLASH_INTERVAL * 0 && fastFlashTime_ <= FLASH_INTERVAL * 1) {
				start_->SetInvisible(false);
			}
			if (fastFlashTime_ > FLASH_INTERVAL * 1 && fastFlashTime_ <= FLASH_INTERVAL * 2) {
				start_->SetInvisible(true);
			}
			if (fastFlashTime_ > FLASH_INTERVAL * 2 && fastFlashTime_ <= FLASH_INTERVAL * 3) {
				start_->SetInvisible(false);
			}
			if (fastFlashTime_ > FLASH_INTERVAL * 3 && fastFlashTime_ <= FLASH_INTERVAL * 4) {
				start_->SetInvisible(true);
			}
			if (fastFlashTime_ > FLASH_INTERVAL * 4 && fastFlashTime_ <= FLASH_INTERVAL * 5) {
				start_->SetInvisible(false);
			}
			if (fastFlashTime_ > FLASH_INTERVAL * 5 && fastFlashTime_ <= FLASH_INTERVAL * 6) {
				start_->SetInvisible(true);
			}
			if (fastFlashTime_ > FLASH_INTERVAL * 6 && fastFlashTime_ <= FLASH_INTERVAL * 7) {
				start_->SetInvisible(false);
			}
			if (fastFlashTime_ > FLASH_INTERVAL * 7 && fastFlashTime_ <= FLASH_INTERVAL * 8) {
				start_->SetInvisible(true);
			}
			if (fastFlashTime_ > FLASH_INTERVAL * 8) {

				isPlayerModelMove_ = true;



			}
		}

		if (isPlayerModelMove_ == true) {

			playerModelRotate_ += 0.005f;


			//反対向きに向いて加速する
			if (playerWorldTransform_.rotate_.y > float(std::numbers::pi)) {
				playerWorldTransform_.rotate_.y = float(std::numbers::pi);
				//向いたら加速
				playerVelocity_.z += accel_;


				playerModelMoveTime_ += 1;
				if (playerModelMoveTime_ > 80) {
					transparency_ += TRANSPARENCY_INTERVAL;
				}

			}


		}


		if (transparency_ > 1.0f) {
			loadingTime_ += 1;
			if (loadingTime_ > 60 * 2) {
				gameManager->ChangeScene(new GameScene());
			}
		}
	}
}

void TitleScene::Draw() {
	playerModel_->Draw(playerWorldTransform_, camera_);
	skydome_->Draw(camera_);

	start_->Draw();

	// スプライト
	titleLogoSprite_->Draw();

	blackSprite_->Draw();
}

TitleScene::~TitleScene() {
	delete skydome_;
	delete titleLogoSprite_;
	delete start_;
}





