#pragma once
#include "Iscene.h"
#include "components/audio/Audio.h"
#include "components/input/Input.h"
#include "components/3d/WorldTransform.h"
#include "components/3d/ViewProjection.h"
#include "TextureManager.h"
#include "components/2d/CreateTriangle.h"
#include "components/2d/CreateSprite.h"
#include "components/2d/CreateParticle.h"
#include "components/3d/CreateLine.h"
#include "components/3d/CreateSphere.h"
#include "components/3d/CreateSkyBox.h"
#include "components/3d/Model.h"
#include "components/debugcamera/DebugCamera.h"
#include "levelDataLoader/LevelDataLoader.h"
#include "postEffect/PostEffect.h"
#include "editors/Editors.h"

#include "components/utilities/globalVariables/GlobalVariables.h"

#include "player/Player.h"

struct Block {
	Model model;
	WorldTransform world;
	Vector4 material;
	bool isFloorOrBlockHit;
	int blockType;
};

class GamePlayScene :public Iscene {
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void DrawUI() override;
	void DrawPostEffect() override;
	void Finalize() override;

	//ゲーム開始時の処理
	void GameStartProcessing();

	//ブロックの生成
	void SpawnBlock(ModelData ObjModelData, uint32_t ObjTexture, EulerTransform transform);

	void LoadBlockPopData(int type);
	void SpawnCSVBlock(ModelData ObjModelData, uint32_t ObjTexture, EulerTransform transform, int blockType, Vector3 position);
	void SelectSpawn(int blockType);

	//当たり判定まとめ
	void CollisionConclusion();

private:
	CitrusJunosEngine* CJEngine_;
	DirectXCommon* dxCommon_;
	ViewProjection viewProjection_;
	TextureManager* textureManager_;

	Audio* audio_;
	Input* input_;
	DebugCamera* debugCamera_;

	//Player
	std::unique_ptr<Player> player_;
	std::unique_ptr<Model> playerModel_;
	bool isPlayerOperationModes_ = true;//Playerを操作しているか

	//
	std::unique_ptr<Model> model_;
	WorldTransform worldTransformModel_;
	Vector4 modelMaterial_;

	// ブロック発生コマンド
	std::stringstream blockPopCommands;
	std::vector<std::vector<int>> matrix_;
	//ブロック
	std::list<Block> blocks_;
	ModelData ObjModelData_;
	uint32_t ObjTexture_;
	WorldTransform world_;//配置指定用の座標

	//Editor
	Editors* editors_;
	bool isEditorMode_ = false;

	//押し戻しの倍率
	float pushbackMultiplier_ = 0.001f;

	//Other
	bool isGameStart_ = true;//ゲームスタート時の処理
};
