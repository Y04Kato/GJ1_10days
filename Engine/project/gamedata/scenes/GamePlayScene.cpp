#include "GamePlayScene.h"
#include "components/utilities/globalVariables/GlobalVariables.h"
#include "iostream"
void GamePlayScene::Initialize() {
	CJEngine_ = CitrusJunosEngine::GetInstance();
	dxCommon_ = DirectXCommon::GetInstance();
	textureManager_ = TextureManager::GetInstance();

	//Input
	input_ = Input::GetInstance();

	//Audio
	audio_ = Audio::GetInstance();
	select_ = audio_->SoundLoad("project/gamedata/resources/SE/Select.mp3");
	set_ = audio_->SoundLoad("project/gamedata/resources/SE/Set.mp3");
	jump_ = audio_->SoundLoad("project/gamedata/resources/SE/Jump.mp3");

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

	//FadeSprite
	fadeSprite_ = std::make_unique <CreateSprite>();

	fadeSprite_->Initialize(Vector2{ 10.0f,10.0f }, bgResourceNum_);
	fadeSprite_->SetAnchor(Vector2{ 0.5f,0.5f });
	fadeSprite_->SetTextureInitialSize();

	//Player初期化
	player_ = std::make_unique<Player>();
	playerModel_.reset(Model::CreateModel("project/gamedata/resources/block", "block.obj"));
	playerModel_->SetDirectionalLightFlag(true, 3);
	player_->Initialize(playerModel_.get());

	//配置カーソル用
	model_.reset(Model::CreateModel("project/gamedata/resources/block", "block.obj"));
	worldTransformModel_.Initialize();
	modelMaterial_ = { 1.0f,0.2f,0.2f,0.2f };
	model_->SetDirectionalLightFlag(true, 0);
	worldTransformModel_.translation_.num[1] = 10.0f;//配置の高さ

	//ブロックのモデル&テクスチャ読み込み
	ObjModelData_ = model_->LoadModelFile("project/gamedata/resources/block", "block.obj");
	ObjTexture_ = textureManager_->Load(ObjModelData_.material.textureFilePath);

	//Editors
	editors_ = Editors::GetInstance();
	editors_->Initialize();
	editors_->SetModels(ObjModelData_, ObjTexture_);
	editors_->AddGroupName((char*)"DemoStage");
	LoadAllBlockData();

	datas_ = Datas::GetInstance();
}

void GamePlayScene::Update() {
	//ステージ初期設定
	if (isGameStart_ == true) {
		GameStartProcessing();
	}

	if (input_->TriggerKey(DIK_Q)) {
		for (Obj obj : editors_->GetObj()) {
			if (obj.type == "Spawn") {
				player_->SetTranslate(Vector3{ obj.world.GetWorldPos().num[0],obj.world.GetWorldPos().num[1] + 2.0f,obj.world.GetWorldPos().num[2] });
			}
		}
		player_->SetVelocity(Vector3{ 0.0f,0.0f,0.0f });

		blocks_.clear();
	}

	//ImGui
	ImGui::Begin("PlayScene");
	ImGui::Checkbox("isEditorMode", &isEditorMode_);
	ImGui::Text("PlayerRespawn:[Q]key");
	ImGui::Text("Player & Block Move:[A]or[D]key");
	ImGui::Text("PlayerJump:[Space]key");
	ImGui::Text("TogglePlayerOperationModes:[E]key");
	if (isPlayerOperationModes_ == true) {
		ImGui::Text("PlayerOperationModes:True");
	}
	else {
		ImGui::Text("PlayerOperationModes:False");
	}
	ImGui::End();

	//フェード用
	if (isfadeIn_ == false) {
		fadeAlpha_ -= 4.0f;
		if (fadeAlpha_ <= 0.0f) {
			fadeAlpha_ = 0.0f;
		}
	}
	else if (isfadeIn_ == true) {
		fadeAlpha_ += 4.0f;
		if (fadeAlpha_ >= 256.0f) {
			SceneEndProcessing();
		}
	}

	//EditorMode
	if (isEditorMode_ == true) {

	}
	else {//EditorsModeではない時
		//Player操作モードチェンジ
		if (input_->TriggerKey(DIK_E)) {
			if (isPlayerOperationModes_ == true) {
				isPlayerOperationModes_ = false;
			}
			else {
				isPlayerOperationModes_ = true;
			}
		}

		//Player操作モードなら
		if (isPlayerOperationModes_ == true) {
			player_->Move();
		}
		else {

			//配置地点操作
			if (input_->TriggerKey(DIK_A)) {
				worldTransformModel_.translation_.num[0] -= 1.0001f;
			}
			if (input_->TriggerKey(DIK_D)) {
				worldTransformModel_.translation_.num[0] += 1.0001f;
			}

			ImGui::Begin("Block Type Selector");
			ImGui::Text("Reset : R");
			ImGui::Text("TypeSelect : W,S");
			ImGui::Text("Rotate : 1(-90),2(+90) \nangle : %d", RotateType * 90);
			// ドロップダウンでブロックタイプを選択
			if (ImGui::BeginCombo("Block Type", std::to_string(selectedBlockType).c_str()))
			{
				for (int type = 1; type <= maxType; ++type)
				{
					bool isSelected = (selectedBlockType == type);
					if (ImGui::Selectable(std::to_string(type).c_str(), isSelected))
					{
						selectedBlockType = type;
					}
					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			// ブロックタイプに応じてデータをロードするボタン
			if (ImGui::Button("Load Block || DIK_SPACE")) {
				LoadBlockPopData(selectedBlockType, RotateType);
			}
			//SpaceでそのTypeのブロックをSpawn
			if (input_->TriggerKey(DIK_SPACE) && isSetBlock_ == false) {
				LoadBlockPopData(selectedBlockType, RotateType);
				isSetBlock_ = true;
			}
			// DIK_2 が押されたときに RotateType を増加させる
			if (input_->TriggerKey(DIK_2)) {
				RotateType = (RotateType + 1) % (maxRotateType + 1);
			}
			// DIK_1 が押されたときに RotateType を減少させる
			if (input_->TriggerKey(DIK_1)) {
				RotateType = (RotateType - 1 + (maxRotateType + 1)) % (maxRotateType + 1);
			}
			//Type変更
			if (input_->TriggerKey(DIK_W) && selectedBlockType <= maxType - 1) {
				selectedBlockType++;
			}
			if (input_->TriggerKey(DIK_S) && selectedBlockType >= 2) {
				selectedBlockType--;
			}
			//Reset
			if (input_->TriggerKey(DIK_R)) {
				blocks_.clear();
			}
			ImGui::End();
		}
	}

	//Block落下
	for (Block& block : blocks_) {
		if (block.isFloorOrBlockHit == false) {
			block.world.translation_.num[1] -= 0.5f;
		}
	}

	//Playerが落下した時戻す
	if (player_->GetWorldTransform().translation_.num[1] <= -30.0f) {
		player_->SetTranslate(Vector3{ 0.0f,0.0f,0.0f });
		player_->SetVelocity(Vector3{ 0.0f,0.0f,0.0f });
	}

	//World更新
	worldTransformModel_.UpdateMatrix();

	//Player更新
	player_->Update();

	//Edirots更新
	editors_->Update();

	//当たり判定処理
	CollisionConclusion();

	for (Block& block : blocks_) {
		block.world.UpdateMatrix();
	}

	//Camera
	debugCamera_->Update();

	//View更新
	viewProjection_.translation_ = debugCamera_->GetViewProjection()->translation_;
	viewProjection_.rotation_ = debugCamera_->GetViewProjection()->rotation_;
	viewProjection_.UpdateMatrix();
}

void GamePlayScene::Draw() {
#pragma region 背景スプライト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard2D);
	bgSprite_->Draw(allSpriteTransform_, allSpriteUVTransform_, allSpriteMaterial_);

#pragma endregion

#pragma region 3Dオブジェクト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard3D);

	//Player
	player_->Draw(viewProjection_);

	//Block
	for (Block& block : blocks_) {
		block.model.Draw(block.world, viewProjection_, block.material);
	}

	//
	editors_->Draw(viewProjection_);

	//
	model_->Draw(worldTransformModel_, viewProjection_, modelMaterial_);

#pragma endregion

#pragma region Skinningモデル描画
	CJEngine_->renderer_->Draw(PipelineType::Skinning);

	editors_->DrawSkin(viewProjection_);

#pragma endregion

#pragma region パーティクル描画
	CJEngine_->renderer_->Draw(PipelineType::Particle);

#pragma endregion

#pragma region 前景スプライト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard2D);

#pragma endregion
}

void GamePlayScene::DrawUI() {
#pragma region 前景スプライト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard2D);


	fadeSprite_->Draw(allSpriteTransform_, allSpriteUVTransform_, Vector4{ 0.0f,0.0f,0.0f,fadeAlpha_ / 256.0f });
#pragma endregion
}

void GamePlayScene::DrawPostEffect() {
	CJEngine_->renderer_->Draw(PipelineType::PostProcess);
}

void GamePlayScene::Finalize() {
	blocks_.clear();
	editors_->Finalize();

}

void GamePlayScene::GameStartProcessing() {
	//ステージ選択を適用
	if (datas_->GetStageNum() == 0) {
		editors_->SetGroupName((char*)"Stage1");
	}
	if (datas_->GetStageNum() == 1) {
		editors_->SetGroupName((char*)"Stage2");
	}
	if (datas_->GetStageNum() == 2) {
		editors_->SetGroupName((char*)"Stage3");
	}
	if (datas_->GetStageNum() == 3) {
		editors_->SetGroupName((char*)"Stage4");
	}
	if (datas_->GetStageNum() == 4) {
		editors_->SetGroupName((char*)"Stage5");
	}

	editors_->Update();

	for (Obj obj : editors_->GetObj()) {
		if (obj.type == "Spawn") {
			player_->SetTranslate(Vector3{ obj.world.GetWorldPos().num[0],obj.world.GetWorldPos().num[1] + 1.0f,obj.world.GetWorldPos().num[2]});
		}
	}

	isGameStart_ = false;
}

void GamePlayScene::SceneEndProcessing() {
	blocks_.clear();
	editors_->Finalize();

	isSetBlock_ = false;
	isfadeIn_ = false;
	isGameStart_ = true;
	fadeAlpha_ = 256.0f;

	sceneNo = CLEAR_SCENE;
}

void GamePlayScene::CollisionConclusion() {
	// Playerと床の当たり判定
	bool isCollisionFloor = false; //衝突があったかどうかを記録するフラグ
	bool isCollidingFromSideOnFloor = false; //横から衝突しているかどうかを記録するフラグ

	OBB playerOBB;
	playerOBB = CreateOBBFromEulerTransform(EulerTransform(player_->GetWorldTransform().scale_, player_->GetWorldTransform().rotation_, player_->GetWorldTransform().translation_));

	for (Obj obj : editors_->GetObj()) {
		if (obj.type == "Floor") {
			OBB objOBB;
			objOBB = CreateOBBFromEulerTransform(EulerTransform(obj.world.scale_, obj.world.rotation_, obj.world.translation_));

			if (IsCollision(playerOBB, objOBB)) {
				Vector3 closestPoint = objOBB.center;
				Vector3 d = playerOBB.center - objOBB.center;

				//最近接点の計算
				for (int i = 0; i < 3; ++i) {
					float dist = Dot(d, objOBB.orientation[i]);
					dist = std::fmax(-objOBB.size.num[i], std::min(dist, objOBB.size.num[i]));
					closestPoint += objOBB.orientation[i] * dist;
				}

				Vector3 difference = playerOBB.center - closestPoint;
				float distance = Length(difference);

				if (distance > 0.0f) {
					Vector3 pushDirection = Normalize(difference);
					Vector3 pushAmount = pushDirection * (playerOBB.size.num[0] - distance) * pushbackMultiplierBlock_;

					//プレイヤーが上に乗っているかどうかを判断
					if (std::abs(pushDirection.num[1]) > std::abs(pushDirection.num[0]) &&
						std::abs(pushDirection.num[1]) > std::abs(pushDirection.num[2])) {

						player_->SetFloorPos(obj.world);
						isCollisionFloor = true;
					}
					else { //横方向からの衝突
						isCollidingFromSideOnFloor = true;
						player_->SetIsReflection(true);

						//押し戻し処理
						playerOBB.center += pushAmount;
						player_->SetTranslate(playerOBB.center);

						player_->SetVelocity(ComputeOBBRepulsion(playerOBB, player_->GetVelocity(), objOBB, 1.0f));

						audio_->SoundPlayWave(jump_, 0.1f, false);
					}
				}
			}
		}

		if (obj.type == "Goal" && fadeAlpha_ == 0.0f) {
			OBB objOBB;
			objOBB = CreateOBBFromEulerTransform(EulerTransform(obj.world.scale_, obj.world.rotation_, obj.world.translation_));

			if (IsCollision(playerOBB, objOBB)) {
				isfadeIn_ = true;
			}
		}
	}

	// 衝突判定の結果を反映
	player_->SetIsFloorHit(isCollisionFloor);
	player_->SetIsCollidingFromSide(isCollidingFromSideOnFloor);


	//PlayerとBlockの当たり判定
	bool isCollisionDetected = false;//衝突があったかどうかを記録するフラグ
	bool isStandingOnBlock = false;//上に乗っているかどうかを記録するフラグ
	bool isCollidingFromSide = false;//横から衝突しているかどうかを記録するフラグ
	for (Block& block : blocks_) {
		OBB blockOBB = CreateOBBFromEulerTransform(EulerTransform(block.world.scale_, block.world.rotation_, block.world.translation_));
		//衝突判定
		if (IsCollision(playerOBB, blockOBB)) {
			Vector3 closestPoint = blockOBB.center;
			Vector3 d = playerOBB.center - blockOBB.center;

			//最近接点の計算
			for (int i = 0; i < 3; ++i) {
				float dist = Dot(d, blockOBB.orientation[i]);
				dist = std::fmax(-blockOBB.size.num[i], std::min(dist, blockOBB.size.num[i]));
				closestPoint += blockOBB.orientation[i] * dist;
			}

			Vector3 difference = playerOBB.center - closestPoint;
			float distance = Length(difference);

			if (distance > 0.0f) {
				Vector3 pushDirection = Normalize(difference);
				Vector3 pushAmount = pushDirection * (playerOBB.size.num[0] - distance) * pushbackMultiplierBlock_;

				//プレイヤーが上に乗っているかどうかを判断
				if (std::abs(pushDirection.num[1]) > std::abs(pushDirection.num[0]) &&
					std::abs(pushDirection.num[1]) > std::abs(pushDirection.num[2])) {

					isStandingOnBlock = true;

					//押し戻し処理
					playerOBB.center += pushAmount;
					player_->SetTranslate(playerOBB.center);

					player_->SetVelocity(ComputeOBBRepulsion(playerOBB, player_->GetVelocity(), blockOBB, 0.2f));

					isCollisionDetected = true;
				}
				else {//横方向からの衝突
					isCollidingFromSide = true;
					player_->SetIsReflection(true);

					//押し戻し処理
					playerOBB.center += pushAmount;
					player_->SetTranslate(playerOBB.center);

					player_->SetVelocity(ComputeOBBRepulsion(playerOBB, player_->GetVelocity(), blockOBB, 1.0f));

					isCollisionDetected = true;

					audio_->SoundPlayWave(jump_, 0.1f, false);
				}
			}
		}
	}

	//衝突判定の結果を反映
	player_->SetIsBlockHit(isCollisionDetected);
	player_->SetIsStandingOnHit(isStandingOnBlock);
	player_->SetIsCollidingFromSide(isCollidingFromSide);

	//Block同士の当たり判定
	for (Block& block1 : blocks_) {
		if (block1.isFloorOrBlockHit) continue;

		OBB block1OBB = CreateOBBFromEulerTransform(EulerTransform(block1.world.scale_, block1.world.rotation_, block1.world.translation_));

		for (Block& block2 : blocks_) {
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

				if (isSetBlock_ == true) {
					audio_->SoundPlayWave(set_, 0.1f, false);
				}

				block1.isFloorOrBlockHit = true;
				StopConnectedBlocks(block1);
				isSetBlock_ = false;
			}
		}
	}

	//Blockと床の当たり判定
	for (Block& block : blocks_) {
		OBB blockOBB;
		blockOBB = CreateOBBFromEulerTransform(EulerTransform(block.world.scale_, block.world.rotation_, block.world.translation_));
		for (Obj obj : editors_->GetObj()) {
			if (obj.type == "Floor") {
				OBB objOBB;
				objOBB = CreateOBBFromEulerTransform(EulerTransform(Vector3{ obj.world.scale_.num[0]*0.98f,obj.world.scale_.num[1] *1.005f,obj.world.scale_.num[2] }, obj.world.rotation_, obj.world.translation_));
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

					if (isSetBlock_ == true) {
						audio_->SoundPlayWave(set_, 0.1f, false);
					}

					block.isFloorOrBlockHit = true;
					StopConnectedBlocks(block);
					isSetBlock_ = false;
				}
			}
		}
	}
}

void GamePlayScene::LoadAllBlockData() {
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
			currentBlockType = std::stoi(line);//1桁または2桁の数字を整数として読み込む
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

void GamePlayScene::LoadBlockPopData(int type, int RotateType) {
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

void GamePlayScene::SelectSpawn(int blockType) {
	EulerTransform trans;
	trans.translate = Vector3{ worldTransformModel_.translation_.num[0] - 2.0002f ,worldTransformModel_.translation_.num[1] ,worldTransformModel_.translation_.num[2] };
	trans.rotate = worldTransformModel_.rotation_;
	trans.scale = worldTransformModel_.scale_;
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

void GamePlayScene::SpawnCSVBlock(ModelData ObjModelData, uint32_t ObjTexture, EulerTransform transform, int blockType, Vector3 position, Vector4 color) {
	Block block;
	block.model.Initialize(ObjModelData_, ObjTexture_);
	block.model.SetDirectionalLightFlag(true, 3);
	block.world.Initialize();
	//オフセットを設定（例: 2.0fを掛けて間隔を広げる）
	Vector3 offsetPosition = position;
	offsetPosition.num[0] *= 2.000001f;//X軸方向の間隔を広げる
	offsetPosition.num[1] *= 2.000001f;//Y軸方向の間隔を広げる

	block.world.translation_ = transform.translate + offsetPosition;//位置を追加
	block.world.rotation_ = transform.rotate;
	block.world.scale_ = transform.scale;

	block.material = color;

	block.isFloorOrBlockHit = false;
	block.blockType = blockType;
	blocks_.push_back(block);
}

//90度回転（時計回り）
void GamePlayScene::Rotate90(std::vector<std::vector<int>>& matrix_) {
	std::vector<std::vector<int>> rotatedMatrix(3, std::vector<int>(3, 0));
	for (int y = 0; y < 3; ++y) {
		for (int x = 0; x < 3; ++x) {
			rotatedMatrix[x][2 - y] = matrix_[y][x];
		}
	}
	matrix_ = rotatedMatrix;
}

void GamePlayScene::StopConnectedBlocks(Block& block1) {
	//接触したブロック以外のブロックも全て停止する
	for (Block& block2 : blocks_) {
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
			StopConnectedBlocks(block2);//再帰的に関連ブロックを停止させる
		}
	}
}
