// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

struct Transform;
struct FramedImage;
struct Health;
struct Immunity;
struct MiracleFood;

#define _CMPS_LIST_ \
	Transform, \
	FramedImage, \
	Health, \
	Immunity, \
	MiracleFood

#define _GRPS_LIST_ \
	FOOD, \
	GHOSTS

#define _HDLRS_LIST_ \
	PACMAN

class RenderSystem;
class PacManSystem;
class FoodSystem;
class GhostSystem;
class ImmunitySystem;
class CollisionsSystem;

#define _SYS_LIST_ \
	RenderSystem, \
	PacManSystem, \
	FoodSystem, \
	GhostSystem, \
	ImmunitySystem, \
	CollisionsSystem