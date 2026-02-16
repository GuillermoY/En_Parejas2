// This file is part of the course TPV2@UCM - Samir Genaim

#include "ImageRenderer.h"

#include "../sdlutils/Texture.h"
#include "Container.h"

ImageRenderer::ImageRenderer(const Texture *img) :
		_img(img) {
}

ImageRenderer::~ImageRenderer() {
}

void ImageRenderer::render(Container *o, float r) {
	SDL_FRect dest = build_sdlfrect(o->getPos(), o->getWidth(), o->getHeight());
	_img->render(dest, r);
}
