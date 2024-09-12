#include"GameTitleScene.h"

void GameTitleScene::Initialize(){
	//CJEngine
	CJEngine_ = CitrusJunosEngine::GetInstance();
	dxCommon_ = DirectXCommon::GetInstance();
	textureManager_ = TextureManager::GetInstance();

	//Input
	input_ = Input::GetInstance();

	//Audio
	audio_ = Audio::GetInstance();
	bgm_ = audio_->SoundLoad("project/gamedata/resources/SE/BGM.mp3");
	select_ = audio_->SoundLoad("project/gamedata/resources/SE/Select.mp3");
	set_ = audio_->SoundLoad("project/gamedata/resources/SE/Set.mp3");
	audio_->SoundPlayWave(bgm_, 0.1f, true);

	// デバッグカメラの初期化
	debugCamera_ = DebugCamera::GetInstance();
	debugCamera_->initialize();

	viewProjection_.Initialize();

	//配置カーソル用
	model_.reset(Model::CreateModel("project/gamedata/resources/TextObj", "TitleText.obj"));
	worldTransformModel_.Initialize();
	modelMaterial_ = { 0.6f,0.3f,0.3f,1.0f };
	model_->SetDirectionalLightFlag(true, 0);
	worldTransformModel_.translation_.num[1] = 10.0f;//配置の高さ
	worldTransformModel_.scale_ = { 12.0f,12.0f,5.0f };

	//ブロックのモデル&テクスチャ読み込み
	ObjModelData_ = model_->LoadModelFile("project/gamedata/resources/block", "block.obj");
	ObjTexture_ = textureManager_->Load(ObjModelData_.material.textureFilePath);

	//Editors
	editors_ = Editors::GetInstance();
	editors_->Initialize();
	editors_->SetModels(ObjModelData_, ObjTexture_);
	editors_->AddGroupName((char*)"TitleStage");
	LoadAllBlockData();

	//
	titleResourceNum_ = textureManager_->Load("project/gamedata/resources/ui/title.png");
	bgResourceNum_ = textureManager_->Load("project/gamedata/resources/bg/paper.png");
	sphereResourceNum_ = textureManager_->Load("project/gamedata/resources/bg/sphere.png");
	startResourceNum_ = textureManager_->Load("project/gamedata/resources/ui/pressSpace.png");

	//スプライト
	allSpriteMaterial_ = { 1.0f,1.0f,1.0f,1.0f };
	allSpriteTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{1280 / 2.0f,720 / 2.0f,0.0f} };
	allSpriteUVTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	//TitleSprite
	titleSprite_ = std::make_unique <CreateSprite>();

	titleSprite_->Initialize(Vector2{ 10.0f,10.0f }, titleResourceNum_);
	titleSprite_->SetAnchor(Vector2{ 0.5f,0.5f });
	titleSprite_->SetTextureInitialSize();

	//BGSprite
	bgSprite_ = std::make_unique <CreateSprite>();

	bgSprite_->Initialize(Vector2{ 10.0f,10.0f }, bgResourceNum_);
	bgSprite_->SetAnchor(Vector2{ 0.5f,0.5f });
	bgSprite_->SetTextureInitialSize();

	//SphereSprite
	sphereSprite_ = std::make_unique <CreateSprite>();

	sphereSpriteTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{974.0f,514.0f,0.0f} };
	sphereSize_ = { 0.0f,0.0f };
	sphereSprite_->Initialize(sphereSize_, sphereResourceNum_);
	sphereSprite_->SetAnchor(Vector2{ 0.5f,0.5f });

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
	selectedBlockType_ = 11;
}

void GameTitleScene::Update(){
	//ステージ初期設定
	if (isSceneStart_ == true) {
		SceneStartProcessing();
	}

	ImGui::Begin("TitleScene");
	ImGui::Checkbox("isEditorMode", &isEditorMode_);
	ImGui::End();

	//EditorMode
	if (isEditorMode_ == true) {

	}
	else {//EditorsModeではない時

		if (isSetBlock_ == false) {
			if (input_->TriggerKey(DIK_SPACE)) {
				LoadBlockPopData(selectedBlockType_, RotateType);
				isSetBlock_ = true;
				audio_->SoundPlayWave(select_, 0.1f, false);
			}
		}
	}

	//ブロックが設置されたら
	if (isNextScene_ == true) {
		sphereSize_.num[0] += 35.0f;
		sphereSize_.num[1] += 35.0f;
		sphereSpriteTransform_.rotate.num[2] += 0.05f;
		sphereSprite_->SetSize(sphereSize_);

		fadeAlpha_ += 4;
	}
	else {
		fadeAlpha_ -= 4;
		if (fadeAlpha_ <= 0) {
			fadeAlpha_ = 0;
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
	if (sphereSize_.num[0] >= 2500.0f) {
		if (fadeAlpha_ >= 256) {
			fadeAlpha_ = 256;
			SceneEndProcessing();
			//SELECTシーンへ移行
			sceneNo = SELECT_SCENE;
		}
	}

	//Block落下
	for (TestBlock& block : blocks_) {
		if (block.isFloorOrBlockHit == false) {
			block.world.translation_.num[1] -= 0.5f;
		}
	}

	//World更新
	worldTransformModel_.UpdateMatrix();

	//Edirots更新
	editors_->Update();

	//当たり判定処理
	CollisionConclusion();

	for (TestBlock& block : blocks_) {
		block.world.UpdateMatrix();
	}

	//Camera
	debugCamera_->Update();

	//View更新
	viewProjection_.translation_ = debugCamera_->GetViewProjection()->translation_;
	viewProjection_.rotation_ = debugCamera_->GetViewProjection()->rotation_;
	viewProjection_.UpdateMatrix();
}

void GameTitleScene::Draw(){
#pragma region 背景スプライト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard2D);
	bgSprite_->Draw(allSpriteTransform_, allSpriteUVTransform_, allSpriteMaterial_);

#pragma endregion

#pragma region 3Dオブジェクト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard3D);

	//Block
	for (TestBlock& block : blocks_) {
		block.model.Draw(block.world, viewProjection_, block.material);
	}

	//
	editors_->Draw(viewProjection_);

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

void GameTitleScene::DrawUI() {
#pragma region 前景スプライト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard2D);
	//titleSprite_->Draw(allSpriteTransform_, allSpriteUVTransform_, allSpriteMaterial_);
	startSprite_->Draw(allSpriteTransform_, allSpriteUVTransform_, Vector4{ allSpriteMaterial_.num[0],allSpriteMaterial_.num[1],allSpriteMaterial_.num[2],spriteAlpha_ / 256.0f });

	if (isNextScene_ == true) {
		//sphereSprite_->Draw(sphereSpriteTransform_, allSpriteUVTransform_, allSpriteMaterial_);
	}

	fadeSprite_->Draw(allSpriteTransform_, allSpriteUVTransform_, Vector4{ 0.0f,0.0f,0.0f,fadeAlpha_ / 256.0f });

#pragma endregion
}

void GameTitleScene::DrawPostEffect() {
	CJEngine_->renderer_->Draw(PipelineType::PostProcess);
}

void GameTitleScene::Finalize() {
	blocks_.clear();
	editors_->Finalize();
}

void GameTitleScene::SceneStartProcessing() {
	editors_->SetGroupName((char*)"TitleStage");

	debugCamera_->SetCamera({ 0.0f,0.0f,-45.0f }, { 0.0f,0.0f,0.0f });

	isSceneStart_ = false;
}

void GameTitleScene::SceneEndProcessing() {
	blocks_.clear();
	editors_->Finalize();
	sphereSize_ = { 0.0f,0.0f };

	isNextScene_ = false;
	isSceneStart_ = true;
	isSetBlock_ = false;
}

void GameTitleScene::CollisionConclusion() {
	//Block同士の当たり判定
	for (TestBlock& block1 : blocks_) {
		if (block1.isFloorOrBlockHit) continue;

		OBB block1OBB = CreateOBBFromEulerTransform(EulerTransform(block1.world.scale_, block1.world.rotation_, block1.world.translation_));

		for (TestBlock& block2 : blocks_) {
			if (&block1 == &block2 || !block2.isFloorOrBlockHit) continue;

			OBB block2OBB = CreateOBBFromEulerTransform(EulerTransform(block2.world.scale_, block2.world.rotation_, block2.world.translation_));

			if (IsCollision(block1OBB, block2OBB)) {
				//押し戻し処理
				//blockOBBからobjOBBの最近接点を計算
				Vector3 closestPoint = block2OBB.center;
				Vector3 d = block1OBB.center - block2OBB.center;

				//objOBBの各軸方向における距離を計算し、最近接点を求める
				for (int i = 0; i < 3; ++i) {
					float dist = Dot(d, block2OBB.orientation[i]);
					dist = std::fmax(-block2OBB.size.num[i], std::fmin(dist, block2OBB.size.num[i]));
					closestPoint += block2OBB.orientation[i] * dist;
				}

				//blockOBBの中心とobjOBBの最近接点の距離を計算
				Vector3 direction = block1OBB.center - closestPoint;
				float distance = Length(direction);
				float overlap = std::fmax(0.0f, Length(block1OBB.size) - distance);//重なりを計算

				if (overlap > 0.0f) {
					//押し戻すための修正ベクトルを計算
					Vector3 correction = Normalize(direction) * overlap * pushbackMultiplier_;
					block1OBB.center += correction;//blockOBBを押し戻す
					block1.world.translation_.num[1] = block1OBB.center.num[1];
				}

				block1.isFloorOrBlockHit = true;
				StopConnectedBlocks(block1);
			}
		}
	}

	//Blockと床の当たり判定
	for (TestBlock& block : blocks_) {
		OBB blockOBB;
		blockOBB = CreateOBBFromEulerTransform(EulerTransform(block.world.scale_, block.world.rotation_, block.world.translation_));
		for (Obj obj : editors_->GetObj()) {
			if (obj.type == "Floor") {
				OBB objOBB;
				objOBB = CreateOBBFromEulerTransform(EulerTransform(obj.world.scale_, obj.world.rotation_, obj.world.translation_));
				if (IsCollision(blockOBB, objOBB)) {
					//押し戻し処理
					//blockOBBからobjOBBの最近接点を計算
					Vector3 closestPoint = objOBB.center;
					Vector3 d = blockOBB.center - objOBB.center;

					//objOBBの各軸方向における距離を計算し、最近接点を求める
					for (int i = 0; i < 3; ++i) {
						float dist = Dot(d, objOBB.orientation[i]);
						dist = std::fmax(-objOBB.size.num[i], std::fmin(dist, objOBB.size.num[i]));
						closestPoint += objOBB.orientation[i] * dist;
					}

					//blockOBBの中心とobjOBBの最近接点の距離を計算
					Vector3 direction = blockOBB.center - closestPoint;
					float distance = Length(direction);
					float overlap = std::fmax(0.0f, Length(blockOBB.size) - distance);//重なりを計算

					if (overlap > 0.0f) {
						//押し戻すための修正ベクトルを計算
						Vector3 correction = Normalize(direction) * overlap * pushbackMultiplier_;
						blockOBB.center += correction;//blockOBBを押し戻す
						block.world.translation_.num[1] = blockOBB.center.num[1];
					}

					block.isFloorOrBlockHit = true;
					StopConnectedBlocks(block);
					isNextScene_ = true;
					audio_->SoundPlayWave(set_, 0.1f, false);
				}
			}
		}
	}
}

void GameTitleScene::LoadAllBlockData() {
	std::ifstream file("project/gamedata/resources/csv/BlockType.csv");
	assert(file.is_open());

	std::string line;
	int currentBlockType = -1;

	while (getline(file, line)) {
		if (line.empty()) {
			continue;
		}
		//行の最初の文字が数字かどうかを確認
		if (isdigit(line[0])) {
			currentBlockType = std::stoi(line); //1桁または2桁の数字を整数として読み込む
			matrix_ = std::vector<std::vector<int>>(3, std::vector<int>(3, 0));
			continue;
		}

		if (currentBlockType != -1) {
			for (int y = 0; y < 3; ++y) {
				if (line.find(",") != std::string::npos) {
					std::istringstream line_stream(line);
					std::string value;

					for (int x = 0; x < 3; ++x) {
						if (getline(line_stream, value, ',')) {
							matrix_[y][x] = std::atoi(value.c_str());
						}
					}
				}
				if (!getline(file, line)) {
					break;
				}
			}
			blockDataMap_[currentBlockType] = matrix_;

			if (line == "}") {
				currentBlockType = -1;//ブロック定義の終わり
			}
		}
	}
	file.close();
}

void GameTitleScene::LoadBlockPopData(int type, int RotateType) {
	auto it = blockDataMap_.find(type);
	if (it != blockDataMap_.end()) {
		matrix_ = it->second;
		//RotateTypeに応じて回転処理を行う
		for (int i = 0; i < RotateType; ++i) {
			Rotate90(matrix_);
		}
		SelectSpawn(type);
	}
	else {
		//指定されたブロックタイプが存在しない場合の処理
		assert(false && "Block type not found in CSV data");
	}
}

void GameTitleScene::SelectSpawn(int blockType) {
	EulerTransform trans;
	trans.translate = Vector3{ 15.0f,10.0f,0.0f};
	trans.rotate = worldTransformModel_.rotation_;
	trans.scale = Vector3{ 1.0f,1.0f,1.0f };
	std::mt19937 randomEngine(seedGenerator());
	std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
	Vector4 color = { distColor(randomEngine),distColor(randomEngine) ,distColor(randomEngine) ,1.0f };
	for (int y = 0; y < 3; ++y) {//行方向
		for (int x = 0; x < 3; ++x) {//列方向
			if (matrix_[y][x] == 1) {
				Vector3 position;
				position.num[0] = static_cast<float>(x);//X座標
				position.num[1] = static_cast<float>(2 - y);//Y座標
				position.num[2] = 0.0f;
				SpawnCSVBlock(ObjModelData_, ObjTexture_, trans, blockType, position, color);
			}
		}
	}
}

void GameTitleScene::SpawnCSVBlock(ModelData ObjModelData, uint32_t ObjTexture, EulerTransform transform, int blockType, Vector3 position, Vector4 color) {
	TestBlock block;
	block.model.Initialize(ObjModelData_, ObjTexture_);
	block.model.SetDirectionalLightFlag(true, 3);
	block.world.Initialize();
	//オフセットを設定（例: 2.0fを掛けて間隔を広げる）
	Vector3 offsetPosition = position;
	offsetPosition.num[0] *= 2.0f;//X軸方向の間隔を広げる
	offsetPosition.num[1] *= 2.0f;//Y軸方向の間隔を広げる

	block.world.translation_ = transform.translate + offsetPosition;//位置を追加
	block.world.rotation_ = transform.rotate;
	block.world.scale_ = transform.scale;

	block.material = color;

	block.isFloorOrBlockHit = false;
	block.blockType = blockType;
	blocks_.push_back(block);
}

//90度回転（時計回り）
void GameTitleScene::Rotate90(std::vector<std::vector<int>>& matrix_) {
	std::vector<std::vector<int>> rotatedMatrix(3, std::vector<int>(3, 0));
	for (int y = 0; y < 3; ++y) {
		for (int x = 0; x < 3; ++x) {
			rotatedMatrix[x][2 - y] = matrix_[y][x];
		}
	}
	matrix_ = rotatedMatrix;
}

void GameTitleScene::StopConnectedBlocks(TestBlock& block1) {
	//接触したブロック以外のブロックも全て停止する
	for (TestBlock& block2 : blocks_) {
		if (block2.isFloorOrBlockHit || &block1 == &block2) {
			continue;
		}

		//block1とblock2の当たり判定を行う
		OBB block2OBB = CreateOBBFromEulerTransform(EulerTransform(block2.world.scale_, block2.world.rotation_, block2.world.translation_));
		OBB block1OBB = CreateOBBFromEulerTransform(EulerTransform(block1.world.scale_, block1.world.rotation_, block1.world.translation_));

		if (IsCollision(block1OBB, block2OBB)) {
			//接触しているならblock2も停止させる
			block2.isFloorOrBlockHit = true;
			block2.world.translation_.num[1] = block2OBB.center.num[1];
			StopConnectedBlocks(block2);  //再帰的に関連ブロックを停止させる
		}
	}
}
