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

	// デバッグカメラの初期化
	debugCamera_ = DebugCamera::GetInstance();
	debugCamera_->initialize();

	viewProjection_.Initialize();

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
}

void GamePlayScene::Update() {
	//ステージ初期設定
	if (isGameStart_ == true) {
		GameStartProcessing();
	}

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_Q)) {
		player_->SetTranslate(worldTransformModel_.translation_);
		player_->SetVelocity(Vector3{ 0.0f,0.0f,0.0f });
	}
#endif // _DEBUG

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

	ImGui::Begin("Block Type Selector");

	// ドロップダウンでブロックタイプを選択
	if (ImGui::BeginCombo("Block Type", std::to_string(selectedBlockType).c_str()))
	{
		for (int type = 1; type <= 11; ++type)
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
	if (ImGui::Button("Load Block || DIK_9"))
	{
		LoadBlockPopData(selectedBlockType);
	}
	if (input_->TriggerKey(DIK_9)) {
		LoadBlockPopData(selectedBlockType);
	}
	if (input_->TriggerKey(DIK_W) && selectedBlockType <= 10) {
		selectedBlockType++;
	}
	if (input_->TriggerKey(DIK_S) && selectedBlockType >= 2 ) {
		selectedBlockType--;
	}

	ImGui::End();

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
			//Block配置
			if (input_->TriggerKey(DIK_0)) {
				EulerTransform trans;
				trans.translate = worldTransformModel_.translation_;
				trans.rotate = worldTransformModel_.rotation_;
				trans.scale = worldTransformModel_.scale_;
				SpawnBlock(ObjModelData_, ObjTexture_, trans);
			}

			//配置地点操作
			if (input_->PressKey(DIK_A)) {
				worldTransformModel_.translation_.num[0] -= 1.0f;
			}
			if (input_->PressKey(DIK_D)) {
				worldTransformModel_.translation_.num[0] += 1.0f;
			}
			/*if (input_->TriggerKey(DIK_A)) {
				worldTransformModel_.translation_.num[0] -= 2.0f;
			}
			if (input_->TriggerKey(DIK_D)) {
				worldTransformModel_.translation_.num[0] += 2.0f;
			}*/
		}
	}

	//Block落下
	for (Block& block : blocks_) {
		if (block.isFloorOrBlockHit == false) {
			block.world.translation_.num[1] -= 0.5f;
		}
	}

	//World更新
	worldTransformModel_.UpdateMatrix();
	for (Block& block : blocks_) {
		block.world.UpdateMatrix();
	}

	//Player更新
	player_->Update();

	//Edirots更新
	editors_->Update();

	//当たり判定処理
	CollisionConclusion();

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
	editors_->SetGroupName((char*)"DemoStage");

	isGameStart_ = false;
}

void GamePlayScene::SpawnBlock(ModelData ObjModelData, uint32_t ObjTexture, EulerTransform transform) {
	Block block;
	block.model.Initialize(ObjModelData_, ObjTexture_);
	block.model.SetDirectionalLightFlag(true, 3);

	block.world.Initialize();
	block.world.translation_ = transform.translate;
	block.world.rotation_ = transform.rotate;
	block.world.scale_ = transform.scale;

	block.material = { 1.0f,1.0f,1.0f,1.0f };

	block.isFloorOrBlockHit = false;

	blocks_.push_back(block);
}

void GamePlayScene::CollisionConclusion() {
	//Playerと床の当たり判定
	OBB playerOBB;
	playerOBB = CreateOBBFromEulerTransform(EulerTransform(player_->GetWorldTransform().scale_, player_->GetWorldTransform().rotation_, player_->GetWorldTransform().translation_));
	for (Obj obj : editors_->GetObj()) {
		if (obj.type == "Floor") {
			OBB objOBB;
			objOBB = CreateOBBFromEulerTransform(EulerTransform(obj.world.scale_, obj.world.rotation_, obj.world.translation_));
			if (IsCollision(playerOBB, objOBB)) {
				//押し戻し処理
				//playerOBBからobjOBBの最近接点を計算
				Vector3 closestPoint = objOBB.center;
				Vector3 d = playerOBB.center - objOBB.center;

				//objOBBの各軸方向における距離を計算し、最近接点を求める
				for (int i = 0; i < 3; ++i) {
					float dist = Dot(d, objOBB.orientation[i]);
					dist = std::fmax(-objOBB.size.num[i], std::fmin(dist, objOBB.size.num[i]));
					closestPoint += objOBB.orientation[i] * dist;
				}

				//playerOBBの中心とobjOBBの最近接点の距離を計算
				Vector3 direction = playerOBB.center - closestPoint;
				float distance = Length(direction);
				float overlap = std::fmax(0.0f, Length(playerOBB.size) - distance);//重なりを計算

				if (overlap > 0.0f) {
					//押し戻すための修正ベクトルを計算
					Vector3 correction = Normalize(direction) * overlap * pushbackMultiplier_;
					playerOBB.center += correction;//playerOBBを押し戻す
					player_->SetTranslate(playerOBB.center);
					player_->SetIsFloorHit(true);
				}
			}
			else {
				player_->SetIsFloorHit(false);
			}
		}
	}

	//PlayerとBlockの当たり判定
	bool isCollisionDetected = false; //衝突があったかどうかを記録するフラグ
	for (Block& block : blocks_) {
		OBB blockOBB;
		blockOBB = CreateOBBFromEulerTransform(EulerTransform(block.world.scale_, block.world.rotation_, block.world.translation_));
		if (IsCollision(playerOBB, blockOBB)) {
			//押し戻し処理
			//playerOBBからblockOBBの最近接点を計算
			Vector3 closestPoint = blockOBB.center;
			Vector3 d = playerOBB.center - blockOBB.center;

			//blockOBBの各軸方向における距離を計算し、最近接点を求める
			for (int i = 0; i < 3; ++i) {
				float dist = Dot(d, blockOBB.orientation[i]);
				dist = std::fmax(-blockOBB.size.num[i], std::fmin(dist, blockOBB.size.num[i]));
				closestPoint += blockOBB.orientation[i] * dist;
			}

			//playerOBBの中心とblockOBBの最近接点の距離を計算
			Vector3 direction = playerOBB.center - closestPoint;
			float distance = Length(direction);
			float overlap = std::fmax(0.0f, Length(playerOBB.size) - distance);//重なりを計算

			if (overlap > 0.0f) {
				//押し戻すための修正ベクトルを計算
				Vector3 correction = Normalize(direction) * overlap * pushbackMultiplier_;
				playerOBB.center += correction;//playerOBBを押し戻す
				player_->SetTranslate(playerOBB.center);
				player_->SetIsBlockHit(true);
				isCollisionDetected = true;
			}
		}
	}

	//全てのブロックとの判定が終了後に、衝突がなかった場合にのみfalseを設定
	player_->SetIsBlockHit(isCollisionDetected);

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
					block1.world.translation_ = block1OBB.center;
				}

				block1.isFloorOrBlockHit = true;
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
						block.world.translation_ = blockOBB.center;
						block.isFloorOrBlockHit = true;
					}
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
		// 行の最初の文字が数字かどうかを確認
		if (isdigit(line[0])) {
			currentBlockType = std::stoi(line); // 1桁または2桁の数字を整数として読み込む
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
				currentBlockType = -1; // ブロック定義の終わり
			}
		}
	}
	file.close();
}

void GamePlayScene::LoadBlockPopData(int type) {
	auto it = blockDataMap_.find(type);
	if (it != blockDataMap_.end()) {
		matrix_ = it->second;
		SelectSpawn(type);
	}
	else {
		// 指定されたブロックタイプが存在しない場合の処理
		assert(false && "Block type not found in CSV data");
	}
}

void GamePlayScene::SelectSpawn(int blockType) {
	EulerTransform trans;
	trans.translate = worldTransformModel_.translation_;
	trans.rotate = worldTransformModel_.rotation_;
	trans.scale = worldTransformModel_.scale_;

	for (int y = 0; y < 3; ++y) { // 行方向
		for (int x = 0; x < 3; ++x) { // 列方向
			if (matrix_[y][x] == 1) {
				Vector3 position;
				position.num[0] = static_cast<float>(x); // X座標
				position.num[1] = static_cast<float>(2 - y); // Y座標
				position.num[2] = 0.0f;
				SpawnCSVBlock(ObjModelData_, ObjTexture_, trans, blockType, position);
			}
		}
	}
}

void GamePlayScene::SpawnCSVBlock(ModelData ObjModelData, uint32_t ObjTexture, EulerTransform transform, int blockType, Vector3 position) {
	Block block;
	block.model.Initialize(ObjModelData_, ObjTexture_);
	block.model.SetDirectionalLightFlag(true, 3);
	block.world.Initialize();
	// オフセットを設定（例: 2.0fを掛けて間隔を広げる）
	Vector3 offsetPosition = position;
	offsetPosition.num[0] *= 2.0f; // X軸方向の間隔を広げる
	offsetPosition.num[1] *= 2.0f; // Y軸方向の間隔を広げる

	block.world.translation_ = transform.translate + offsetPosition; // 位置を追加
	block.world.rotation_ = transform.rotate;
	block.world.scale_ = transform.scale;

	block.material = { 1.0f,1.0f,1.0f,1.0f };
	block.isFloorOrBlockHit = false;
	block.blockType = blockType;
	blocks_.push_back(block);
}