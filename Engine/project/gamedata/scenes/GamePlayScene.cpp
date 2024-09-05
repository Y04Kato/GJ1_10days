#include "GamePlayScene.h"
#include "components/utilities/globalVariables/GlobalVariables.h"

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
}

void GamePlayScene::Update() {

	//ImGui
	ImGui::Begin("PlayScene");
	ImGui::Checkbox("isEditorMode", &isEditorMode_);
	ImGui::DragFloat3("", viewProjection_.rotation_.num);
	ImGui::End();

	//EditorMode
	if (isEditorMode_ == true) {
		
	}
	else {//EditorsModeではない時
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

	//ブロック描画
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
	//ブロック同士の当たり判定
	for (Block& block1 : blocks_) {
		if (block1.isFloorOrBlockHit) continue;

		OBB block1OBB = CreateOBBFromEulerTransform(EulerTransform(block1.world.scale_, block1.world.rotation_, block1.world.translation_));

		for (Block& block2 : blocks_) {
			if (&block1 == &block2 || !block2.isFloorOrBlockHit) continue;

			OBB block2OBB = CreateOBBFromEulerTransform(EulerTransform(block2.world.scale_, block2.world.rotation_, block2.world.translation_));

			if (IsCollision(block1OBB, block2OBB)) {
				Vector3 closestPoint = block2OBB.center;
				Vector3 d = block1OBB.center - block2OBB.center;

				for (int i = 0; i < 3; ++i) {
					float dist = Dot(d, block2OBB.orientation[i]);
					dist = std::fmax(-block2OBB.size.num[i], std::fmin(dist, block2OBB.size.num[i]));
					closestPoint += block2OBB.orientation[i] * dist;
				}

				Vector3 direction = block1OBB.center - closestPoint;
				float distance = Length(direction);
				float overlap = std::fmax(0.0f, Length(block1OBB.size) - distance); // 重なりを計算

				if (overlap > 0.0f) {
					Vector3 correction = Normalize(direction) * overlap * pushbackMultiplier_;
					block1OBB.center += correction;  // blockOBBを押し戻す
					block1.world.translation_ = block1OBB.center;
				}

				block1.isFloorOrBlockHit = true;
			}
		}
	}

	//ブロックと床の当たり判定
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
						// 押し戻すための修正ベクトルを計算
						Vector3 correction = Normalize(direction) * overlap * pushbackMultiplier_;
						blockOBB.center += correction;  // blockOBBを押し戻す
						block.world.translation_ = blockOBB.center;
						block.isFloorOrBlockHit = true;
					}
				}
			}
		}
	}
}