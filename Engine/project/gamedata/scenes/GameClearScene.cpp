#include"GameClearScene.h"

void GameClearScene::Initialize() {
	//CJEngine
	CJEngine_ = CitrusJunosEngine::GetInstance();
	dxCommon_ = DirectXCommon::GetInstance();
	textureManager_ = TextureManager::GetInstance();

	//Input
	input_ = Input::GetInstance();

	//Audio
	audio_ = Audio::GetInstance();
	select_ = audio_->SoundLoad("project/gamedata/resources/SE/Select.mp3");
	set_ = audio_->SoundLoad("project/gamedata/resources/SE/Set.mp3");

	// デバッグカメラの初期化
	debugCamera_ = DebugCamera::GetInstance();
	debugCamera_->initialize();

	viewProjection_.Initialize();

	//配置カーソル用
	model_.reset(Model::CreateModel("project/gamedata/resources/TextObj", "ClearText.obj"));
	worldTransformModel_.Initialize();
	modelMaterial_ = { 0.6f,0.3f,0.3f,1.0f };
	model_->SetDirectionalLightFlag(true, 0);
	worldTransformModel_.translation_.num[1] = 10.0f;//配置の高さ
	worldTransformModel_.scale_ = { 12.0f,12.0f,5.0f };

	//
	bgResourceNum_ = textureManager_->Load("project/gamedata/resources/bg/paper.png");
	startResourceNum_ = textureManager_->Load("project/gamedata/resources/ui/pressSpace.png");

	//スプライト
	allSpriteMaterial_ = { 1.0f,1.0f,1.0f,1.0f };
	allSpriteTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{1280 / 2.0f,720 / 2.0f,0.0f} };
	allSpriteUVTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	//BGSprite
	bgSprite_ = std::make_unique <CreateSprite>();

	bgSprite_->Initialize(Vector2{ 10.0f,10.0f }, bgResourceNum_);
	bgSprite_->SetAnchor(Vector2{ 0.5f,0.5f });
	bgSprite_->SetTextureInitialSize();

	//StartSprite
	startSprite_ = std::make_unique <CreateSprite>();

	startSprite_->Initialize(Vector2{ 10.0f,10.0f }, startResourceNum_);
	startSprite_->SetAnchor(Vector2{ 0.5f,0.5f });
	startSprite_->SetTextureInitialSize();

	//FadeSprite
	fadeSprite_ = std::make_unique <CreateSprite>();

	fadeSprite_->Initialize(Vector2{ 10.0f,10.0f }, bgResourceNum_);
	fadeSprite_->SetAnchor(Vector2{ 0.5f,0.5f });
	fadeSprite_->SetTextureInitialSize();
}

void GameClearScene::Update() {
	//ステージ初期設定
	if (isSceneStart_ == true) {
		SceneStartProcessing();
	}

	ImGui::Begin("Clearcene");

	ImGui::End();

	if (input_->TriggerKey(DIK_SPACE)) {
		audio_->SoundPlayWave(select_, 0.1f, false);
		isfadeIn_ = true;
	}

	//フェード用
	if (isfadeIn_ == false) {
		fadeAlpha_ -= 4.0f;
		if (fadeAlpha_ <= 0.0f) {
			fadeAlpha_ = 0.0f;
		}
	}
	else if (isfadeIn_ == true) {//ステージ決定時
		fadeAlpha_ += 4.0f;
		if (fadeAlpha_ >= 256.0f) {
			SceneEndProcessing();
		}
	}

	//UI点滅用
	if (changeAlpha_ == false) {
		spriteAlpha_ -= 8;
		if (spriteAlpha_ <= 0) {
			changeAlpha_ = true;
		}
	}
	else if (changeAlpha_ == true) {
		spriteAlpha_ += 8;
		if (spriteAlpha_ >= 256) {
			changeAlpha_ = false;
		}
	}

	//画面を埋めるサイズになったら
	if (fadeAlpha_ >= 256) {
		fadeAlpha_ = 256;
		SceneEndProcessing();
		//SELECTシーンへ移行
		sceneNo = SELECT_SCENE;
	}

	//World更新
	worldTransformModel_.UpdateMatrix();

	//Camera
	debugCamera_->Update();

	//View更新
	viewProjection_.translation_ = debugCamera_->GetViewProjection()->translation_;
	viewProjection_.rotation_ = debugCamera_->GetViewProjection()->rotation_;
	viewProjection_.UpdateMatrix();
}

void GameClearScene::Draw() {
#pragma region 背景スプライト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard2D);
	bgSprite_->Draw(allSpriteTransform_, allSpriteUVTransform_, allSpriteMaterial_);

#pragma endregion

#pragma region 3Dオブジェクト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard3D);

	//
	model_->Draw(worldTransformModel_, viewProjection_, modelMaterial_);

#pragma endregion

#pragma region パーティクル描画
	CJEngine_->renderer_->Draw(PipelineType::Particle);

#pragma endregion

#pragma region 前景スプライト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard2D);

#pragma endregion
}

void GameClearScene::DrawUI() {
#pragma region 前景スプライト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard2D);
	startSprite_->Draw(allSpriteTransform_, allSpriteUVTransform_, Vector4{ allSpriteMaterial_.num[0],allSpriteMaterial_.num[1],allSpriteMaterial_.num[2],spriteAlpha_ / 256.0f });

	fadeSprite_->Draw(allSpriteTransform_, allSpriteUVTransform_, Vector4{ 0.0f,0.0f,0.0f,fadeAlpha_ / 256.0f });

#pragma endregion
}

void GameClearScene::DrawPostEffect() {
	CJEngine_->renderer_->Draw(PipelineType::PostProcess);
}

void GameClearScene::Finalize() {

}

void GameClearScene::SceneStartProcessing() {
	debugCamera_->SetCamera({ 0.0f,0.0f,-45.0f }, { 0.0f,0.0f,0.0f });

	isSceneStart_ = false;
}

void GameClearScene::SceneEndProcessing() {
	isSceneStart_ = true;
	isfadeIn_ = false;
	fadeAlpha_ = 256.0f;
}