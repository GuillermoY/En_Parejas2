// This file is part of the course TPV2@UCM - Samir Genaim


#pragma once

// Components list - must have at least one element
//

struct Transform;
struct Image;
struct RectangleViewer;
struct PaddleCtrl;
struct GameState;

#define _CMPS_LIST_ \
	Transform, \
	Image, \
	RectangleViewer, \
	PaddleCtrl, \
	GameState

// Groups list - must have at least one element
//
#define _GRPS_LIST_ \
	PADDLES

// Handlers list - must have at least one element
//
#define _HDLRS_LIST_ \
	BALL

// Systems list - must have at least one element
//

class BallSystem;
class PaddlesSystem;
class RenderSystem;
class GameCtrlSystem;
class CollisionsSystem;

#define _SYS_LIST_ \
	BallSystem, \
	PaddlesSystem, \
	RenderSystem, \
	GameCtrlSystem, \
	CollisionsSystem

