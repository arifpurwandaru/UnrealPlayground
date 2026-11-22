#pragma once

#include "DrawDebugHelpers.h"


#define DRAW_SPHERE(Location, color) if (GetWorld()) { DrawDebugSphere(GetWorld(), Location, 25.f, 12, color, true); } 

#define DRAW_SQUARE(Location, color) if (GetWorld()) { DrawDebugBox(GetWorld(), Location, FVector(25.f, 25.f, 25.f), color, true); }

#define DRAW_CAPSULE(Location, color) if (GetWorld()) { DrawDebugCapsule(GetWorld(), Location, 50.f, 25.f, FQuat::Identity, color, true); }

#define DRAW_LINE(Start, End, color) if (GetWorld()) { DrawDebugLine(GetWorld(), Start, End, color, true, -1.f, 0, 1.f); }

#define DRAW_VECTOR(startLocation, endLocation, color) { \
	if (GetWorld()) { \
		DrawDebugLine(GetWorld(), startLocation, endLocation, color, true, -1.f, 0, 1.f); \
		DrawDebugPoint(GetWorld(), endLocation, 15.f, color, true); \
	} \
}