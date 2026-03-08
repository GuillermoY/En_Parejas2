// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"

class Transform;
class Texture;

class Image : public ecs::Component {
public:

	__CMPID_DECL__(ecs::cmp::IMAGE)

		Image();
	Image(const Texture* tex);
	virtual ~Image();

	void setTexture(const Texture* tex) {
		_tex = tex;
	}

	void initComponent() override;
	void render() override;

private:
	Transform* _tr;
	const Texture* _tex;
};