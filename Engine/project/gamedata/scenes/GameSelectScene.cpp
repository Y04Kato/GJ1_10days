#include"GameSelectScene.h"

void GameSelectScene::Initialize() {
	//CJEngine
	CJEngine_ = CitrusJunosEngine::GetInstance();
	dxCommon_ = DirectXCommon::GetInstance();
	textureManager_ = TextureManager::GetInstance();

	//Input
	input_ = Input::GetInstance();

	//Audio
	audio_ = Audio::GetInstance();

	// デバッグカメラの初期化
	debugCamera_ = DebugCamera::GetInstance();
	debugCamera_->initialize();

	viewProjection_.Initialize();

	//
	bgResourceNum_ = textureManager_->Load("project/gamedata/resources/bg/paper.png");

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
}

void GameSelectScene::Update() {
	ImGui::Begin("SelectScene");
	ImGui::Text("PlayScene:1 key");
	ImGui::Text("TitleScene:2 key");
	ImGui::End();

	if (input_->TriggerKey(DIK_1)) {
		sceneNo = GAME_SCENE;
	}
	if (input_->TriggerKey(DIK_2)) {
		sceneNo = TITLE_SCENE;
	}

	//Camera
	debugCamera_->Update();

	//View更新
	viewProjection_.translation_ = debugCamera_->GetViewProjection()->translation_;
	viewProjection_.rotation_ = debugCamera_->GetViewProjection()->rotation_;
	viewProjection_.UpdateMatrix();
}

void GameSelectScene::Draw() {
#pragma region 背景スプライト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard2D);
	bgSprite_->Draw(allSpriteTransform_, allSpriteUVTransform_, allSpriteMaterial_);

#pragma endregion
}

void GameSelectScene::DrawUI() {
#pragma region 前景スプライト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard2D);

#pragma endregion
}

void GameSelectScene::DrawPostEffect() {
	CJEngine_->renderer_->Draw(PipelineType::PostProcess);
}

void GameSelectScene::Finalize() {

}