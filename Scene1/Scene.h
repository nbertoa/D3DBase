#pragma once

#include <App/Component.h> 

class Scene : public Component {
public:
	Scene();
	void Update(const float elapsedTime) override;
};
