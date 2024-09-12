#include"GameSelectScene.h"

//フレームごとの時間計測用
std::chrono::high_resolution_clock::time_point lastFrameTime;

void GameSelectScene::Initialize() {
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

	//
	model_[0].reset(Model::CreateModel("project/gamedata/resources/DemoViewStageObj", "DemoViewStage1.obj"));
	model_[1].reset(Model::CreateModel("project/gamedata/resources/DemoViewStageObj", "DemoViewStage2.obj"));
	model_[2].reset(Model::CreateModel("project/gamedata/resources/DemoViewStageObj", "DemoViewStage3.obj"));
	model_[3].reset(Model::CreateModel("project/gamedata/resources/DemoViewStageObj", "DemoViewStage4.obj"));
	model_[4].reset(Model::CreateModel("project/gamedata/resources/DemoViewStageObj", "DemoViewStage5.obj"));
	model_[5].reset(Model::CreateModel("project/gamedata/resources/TextObj", "EXITText.obj"));
	for (int i = 0; i < 6; i++) {
		worldTransformModel_[i].Initialize();
		worldTransformModel_[i].scale_ = { 0.1f,0.1f,0.1f };
		model_[i]->SetDirectionalLightFlag(true, 3);
	}
	modelMaterial_ = { 1.0f,1.0f,1.0f,1.0f };

	worldTransformModel_[0].translation_ = { -5.0f,0.0f,-10.0f };
	worldTransformModel_[1].translation_ = { 5.0f,0.0f,-10.0f };
	worldTransformModel_[2].translation_ = { 10.0f,0.0f,0.0f };
	worldTransformModel_[3].translation_ = { 5.0f,0.0f,10.0f };
	worldTransformModel_[4].translation_ = { -5.0f,0.0f,10.0f };
	worldTransformModel_[5].translation_ = { -10.0f,1.0f,0.0f };

	worldTransformModel_[0].rotation_ = { 0.0f,0.0f,0.0f };
	worldTransformModel_[1].rotation_ = { 0.0f,0.0f,0.0f };
	worldTransformModel_[2].rotation_ = { 0.0f,-1.57f,0.0f };
	worldTransformModel_[3].rotation_ = { 0.0f,-3.14f,0.0f };
	worldTransformModel_[4].rotation_ = { 0.0f,-3.14f,0.0f };
	worldTransformModel_[5].rotation_ = { 0.0f,1.57f,0.0f };

	worldTransformModel_[5].scale_ = { 1.0f,1.0f,1.0f };

	hexVertices_[0].position = { -5.0f,0.0f,-18.5f };
	hexVertices_[1].position = { 5.0f,0.0f,-18.5f };
	hexVertices_[2].position = { 18.5f,0.0f,0.0f };
	hexVertices_[3].position = { 5.0f,0.0f,18.5f };
	hexVertices_[4].position = { -5.0f,0.0f,18.5f };
	hexVertices_[5].position = { -18.5f,0.0f,0.0f };

	//
	datas_ = Datas::GetInstance();
	datas_->Initialize();

	lastFrameTime = std::chrono::high_resolution_clock::now();
}

void GameSelectScene::Update() {
	ImGui::Begin("SelectScene");
	ImGui::Text("CurrentVertexIndex %d", currentVertexIndex);
	ImGui::DragFloat3("", &worldTransformModel_[0].rotation_.num[0]);
	ImGui::End();

	//シーン初期設定
	if (isGameStart_ == true) {
		GameStartProcessing();
	}

	//Aキーが押されたら左側（反時計回り）に移動
	if (input_->TriggerKey(DIK_A)) {
		audio_->SoundPlayWave(select_, 0.1f, false);
		currentVertexIndex = (currentVertexIndex + 5) % 6;//左へ移動（インデックスを減少）
		if (currentVertexIndex == 1) {
			currentRotationY += rotationStep;//π/2右回転
		}
		if (currentVertexIndex == 2) {
			currentRotationY += rotationStep;//π/2右回転
		}
		if (currentVertexIndex == 4) {
			currentRotationY += rotationStep;//π/2右回転
		}
		if (currentVertexIndex == 5) {
			currentRotationY += rotationStep;//π/2右回転
		}
		MoveCameraToVertex(currentVertexIndex, 0.05f, currentRotationY);
	}

	//Dキーが押されたら右側（時計回り）に移動
	if (input_->TriggerKey(DIK_D)) {
		audio_->SoundPlayWave(select_, 0.1f, false);
		currentVertexIndex = (currentVertexIndex + 1) % 6;//右へ移動（インデックスを増加）
		if (currentVertexIndex == 2) {
			currentRotationY -= rotationStep;//π/2左回転
		}
		if (currentVertexIndex == 3) {
			currentRotationY -= rotationStep;//π/2左回転
		}
		if (currentVertexIndex == 5) {
			currentRotationY -= rotationStep;//π/2左回転
		}
		if (currentVertexIndex == 0) {
			currentRotationY -= rotationStep;//π/2左回転
		}
		MoveCameraToVertex(currentVertexIndex, 0.05f, currentRotationY);
	}

	//決定
	if (input_->TriggerKey(DIK_SPACE)) {
		audio_->SoundPlayWave(set_, 0.1f, false);
		datas_->SetStageNum(currentVertexIndex);
		isfadeIn_ = true;
	}

	//頂点の浮遊アニメーションとY座標の補正を更新
	CalculateDeltaTime();
	UpdateVertices(deltaTime);

	//UI点滅用
	if (changeAlpha_ == false) {
		spriteAlpha_ -= 8.0f;
		if (spriteAlpha_ <= 0.0f) {
			changeAlpha_ = true;
		}
	}
	else if (changeAlpha_ == true) {
		spriteAlpha_ += 8.0f;
		if (spriteAlpha_ >= 256.0f) {
			changeAlpha_ = false;
		}
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

	for (int i = 0; i < 6; i++) {
		worldTransformModel_[i].UpdateMatrix();
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

#pragma region 3Dオブジェクト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard3D);

	for (int i = 0; i < 6; i++) {
		model_[i]->Draw(worldTransformModel_[i], viewProjection_, modelMaterial_);
	}

#pragma endregion
}

void GameSelectScene::DrawUI() {
#pragma region 前景スプライト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard2D);
	startSprite_->Draw(allSpriteTransform_, allSpriteUVTransform_, Vector4{ allSpriteMaterial_.num[0],allSpriteMaterial_.num[1],allSpriteMaterial_.num[2],spriteAlpha_ / 256.0f });

	fadeSprite_->Draw(allSpriteTransform_, allSpriteUVTransform_, Vector4{ 0.0f,0.0f,0.0f,fadeAlpha_ / 256.0f });

#pragma endregion
}

void GameSelectScene::DrawPostEffect() {
	CJEngine_->renderer_->Draw(PipelineType::PostProcess);
}

void GameSelectScene::Finalize() {

}

void GameSelectScene::GameStartProcessing() {
	debugCamera_->SetCamera({ -5.0f,1.0f,-18.5f }, { 0.0f,0.0f,0.0f });

	isGameStart_ = false;
}

void GameSelectScene::SceneEndProcessing() {
	currentVertexIndex = 0;
	currentRotationY = 0.0f;
	isfadeIn_ = false;
	isGameStart_ = true;
	fadeAlpha_ = 256.0f;
	debugCamera_->SetCamera({ 0.0f,0.0f,-45.0f }, { 0.0f,0.0f,0.0f });

	if (datas_->GetStageNum() == 5) {
		sceneNo = TITLE_SCENE;
	}
	else {
		sceneNo = GAME_SCENE;
	}
}

void GameSelectScene::MoveCameraToVertex(int vertexIndex, float timerSpeed, float rotationY) {
	Vector3 translation = hexVertices_[vertexIndex].position;
	translation.num[1] += 1;
	Vector3 rotation = { 0.0f, rotationY, 0.0f };//必要に応じてカメラの回転を指定
	debugCamera_->MovingCamera(translation, rotation, timerSpeed);
}

void GameSelectScene::UpdateVertices(float deltaTime) {
	for (int i = 0; i < 6; ++i) {
		if (i == currentVertexIndex) {
			//選ばれている頂点は浮遊
			hexVertices_[i].oscillationTimer += deltaTime;
			worldTransformModel_[i].translation_.num[1] = oscillationAmplitude * std::sin(oscillationSpeed * hexVertices_[i].oscillationTimer);
			worldTransformModel_[i].rotation_.num[1] += 0.01f;
		}
		else {
			//選ばれていない頂点はY=0に戻る
			if (worldTransformModel_[i].translation_.num[1] != 0.0f) {
				//線形補間で徐々にY=0に戻す
				float direction = (worldTransformModel_[i].translation_.num[1] > 0.0f) ? -1.0f : 1.0f;
				worldTransformModel_[i].translation_.num[1] += direction * returnSpeed * deltaTime;

				//0に近づける処理（Y=0を超えないように調整）
				if (std::abs(worldTransformModel_[i].translation_.num[1]) < returnSpeed * deltaTime) {
					worldTransformModel_[i].translation_.num[1] = 0.0f;
				}
			}

			if (worldTransformModel_[0].rotation_.num[1] != 0.0f && 0 != currentVertexIndex) {
				//線形補間で徐々にY=0に戻す
				float direction = (worldTransformModel_[0].rotation_.num[1] > 0.0f) ? -1.0f : 1.0f;
				worldTransformModel_[0].rotation_.num[1] += direction * returnSpeed * deltaTime;

				//0に近づける処理（Y=0を超えないように調整）
				if (std::abs(worldTransformModel_[0].rotation_.num[1]) < returnSpeed * deltaTime) {
					worldTransformModel_[0].rotation_.num[1] = 0.0f;
				}
			}

			if (worldTransformModel_[1].rotation_.num[1] != 0.0f && 1 != currentVertexIndex) {
				//線形補間で徐々にY=0に戻す
				float direction = (worldTransformModel_[1].rotation_.num[1] > 0.0f) ? -1.0f : 1.0f;
				worldTransformModel_[1].rotation_.num[1] += direction * returnSpeed * deltaTime;

				//0に近づける処理（Y=0を超えないように調整）
				if (std::abs(worldTransformModel_[1].rotation_.num[1]) < returnSpeed * deltaTime) {
					worldTransformModel_[1].rotation_.num[1] = 0.0f;
				}
			}

			if (worldTransformModel_[2].rotation_.num[1] != -1.57f && 2 != currentVertexIndex) {
				//線形補間で徐々にY=-1.57に戻す
				float direction = (worldTransformModel_[2].rotation_.num[1] > -1.57f) ? -1.0f : 1.0f;
				worldTransformModel_[2].rotation_.num[1] += direction * returnSpeed * deltaTime;

				//-1.57に近づける処理（Y=-1.57を超えないように調整）
				if (std::abs(worldTransformModel_[2].rotation_.num[1] - (-1.57f)) < returnSpeed * deltaTime) {
					worldTransformModel_[2].rotation_.num[1] = -1.57f;
				}
			}

			if (worldTransformModel_[3].rotation_.num[1] != -3.14f && 3 != currentVertexIndex) {
				//線形補間で徐々にY=-3.14に戻す
				float direction = (worldTransformModel_[3].rotation_.num[1] > -3.14f) ? -1.0f : 1.0f;
				worldTransformModel_[3].rotation_.num[1] += direction * returnSpeed * deltaTime;

				//-3.14に近づける処理（Y=-3.14を超えないように調整）
				if (std::abs(worldTransformModel_[3].rotation_.num[1] - (-3.14f)) < returnSpeed * deltaTime) {
					worldTransformModel_[3].rotation_.num[1] = -3.14f;
				}
			}

			if (worldTransformModel_[4].rotation_.num[1] != -3.14f && 4 != currentVertexIndex) {
				//線形補間で徐々にY=-3.14に戻す
				float direction = (worldTransformModel_[4].rotation_.num[1] > -3.14f) ? -1.0f : 1.0f;
				worldTransformModel_[4].rotation_.num[1] += direction * returnSpeed * deltaTime;

				//-3.14に近づける処理（Y=-3.14を超えないように調整）
				if (std::abs(worldTransformModel_[4].rotation_.num[1] - (-3.14f)) < returnSpeed * deltaTime) {
					worldTransformModel_[4].rotation_.num[1] = -3.14f;
				}
			}

			if (worldTransformModel_[5].rotation_.num[1] != 1.57f && 5 != currentVertexIndex) {
				//線形補間で徐々にY=1.57に戻す
				float direction = (worldTransformModel_[5].rotation_.num[1] > 1.57f) ? -1.0f : 1.0f;
				worldTransformModel_[5].rotation_.num[1] += direction * returnSpeed * deltaTime;

				//1.57に近づける処理（Y=-1.57を超えないように調整）
				if (std::abs(worldTransformModel_[5].rotation_.num[1] - (1.57f)) < returnSpeed * deltaTime) {
					worldTransformModel_[5].rotation_.num[1] = 1.57f;
				}
			}
		}
	}

}

void GameSelectScene::CalculateDeltaTime() {
	//現在の時間を取得
	auto currentTime = std::chrono::high_resolution_clock::now();

	//前回のフレームとの時間差を計算 (秒単位に変換)
	std::chrono::duration<float> elapsedTime = currentTime - lastFrameTime;
	deltaTime = elapsedTime.count(); //秒での経過時間

	//現在の時間を次のフレーム用に保存
	lastFrameTime = currentTime;
}