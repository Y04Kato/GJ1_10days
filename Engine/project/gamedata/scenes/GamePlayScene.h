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
#include "datas/datas.h"

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

	//シーン終了時の処理
	void SceneEndProcessing();

	void LoadAllBlockData();
	void LoadBlockPopData(int type,int RotateType);
	void SpawnCSVBlock(ModelData ObjModelData, uint32_t ObjTexture, EulerTransform transform, int blockType, Vector3 position,Vector4 color);
	void SelectSpawn(int blockType);
	void Rotate90(std::vector<std::vector<int>>& matrix_);
	//当たり判定まとめ
	void CollisionConclusion();

	void StopConnectedBlocks(Block& block1);

	//SelectCountを進める、0で++、1で--
	void SelectCounter(int selectedBlockType , int countType);

private:
	CitrusJunosEngine* CJEngine_;
	DirectXCommon* dxCommon_;
	ViewProjection viewProjection_;
	TextureManager* textureManager_;

	Audio* audio_;
	SoundData select_;
	SoundData set_;
	SoundData jump_;

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

	//
	std::unique_ptr <CreateSprite> bgSprite_;
	std::unique_ptr <CreateSprite> fadeSprite_;
	EulerTransform allSpriteTransform_;
	EulerTransform allSpriteUVTransform_;
	Vector4 allSpriteMaterial_;

	//
	float fadeAlpha_ = 256.0f;
	bool isfadeIn_ = false;

	//
	uint32_t bgResourceNum_;

	// ブロック発生コマンド
	std::stringstream blockPopCommands;
	std::vector<std::vector<int>> matrix_;
	//ブロック
	std::unordered_map<int, std::vector<std::vector<int>>> blockDataMap_; // ブロックタイプとそのデータを保持
	int selectedBlockType_ = 1; // デフォルトのブロックタイプ
	std::list<Block> blocks_;
	ModelData ObjModelData_;
	uint32_t ObjTexture_;
	WorldTransform world_;//配置指定用の座標
	int RotateType = 0;//Rotate
	const int maxRotateType = 3;// RotateType の範囲を定義
	const int maxType = 12;
	//Editor
	Editors* editors_;
	bool isEditorMode_ = false;

	//押し戻しの倍率
	float pushbackMultiplier_ = 0.05f;
	float pushbackMultiplierBlock_ = 1.01f;

	//Other
	std::random_device seedGenerator;
	bool isGameStart_ = true;//ゲームスタート時の処理

	bool isSetBlock_ = false;//ブロックを出したか否か

	//
	Datas* datas_;

	//
	int pressTimer_ = 0;
	const int pressMaxTime = 10;

	//
	int blockSetCount_[12];
	int blockSetMaxCount_ = 0;

	//
	std::unique_ptr<CreateLine> line_;
	WorldTransform worldTransformLine_[2];
	Vector4 lineMaterial_;
	float lineThickness_ = 0.2f;
};
