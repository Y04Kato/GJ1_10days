#pragma once
#include "Vector.h"

class Datas {
public:
	static Datas* GetInstance();

	void Initialize();

	void SetStageNum(const int stageNum) { stageNum_ = stageNum; }
	const int& GetStageNum() { return stageNum_; }

private:
	Datas() = default;
	~Datas() = default;
	Datas(const Datas& obj) = default;
	Datas& operator=(const Datas& obj) = default;

	int stageNum_ = 0;
};