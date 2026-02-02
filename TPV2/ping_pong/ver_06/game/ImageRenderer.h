// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "RenderComponent.h"

class Texture;

class ImageRenderer: public RenderComponent {
public:
	ImageRenderer(const Texture *img);
	virtual ~ImageRenderer();
	void render(Container *o) override;
private:
	const Texture *_img;
};

