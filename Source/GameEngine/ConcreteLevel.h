#pragma once

#include "ILevel.h"
#include "ConcreteSpriteFactory.h"

using namespace GameEngine;


class ConcreteLevel : public ILevel
{
public:
    ConcreteLevel();
	void LoadAssets() override;
	void Update(float dt) override;
	void Render() override;
	void Enter() override;
	void Exit() override;
};