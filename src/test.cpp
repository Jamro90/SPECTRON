/**********************************************************************************************
*
*   raylib-extras, examples-cpp * examples for Raylib in C++
*
*   LICENSE: MIT
*
*   Copyright (c) 2023 Jeffery Myers
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*
**********************************************************************************************/

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <vector>
#include <list>

Matrix ModelTransform = MatrixIdentity();
Mesh Cube;
Material CubeMat;

Camera3D ViewCamera = { 0 };

RayCollision Collision = { 0 };
Ray CastRay = { 0 };

void GameInit()
{
	Cube = GenMeshCube(5, 2, 1);
	CubeMat = LoadMaterialDefault();
	CubeMat.maps[MATERIAL_MAP_ALBEDO].color = RED;

	ViewCamera.fovy = 45;
	ViewCamera.up.y = 1;
	ViewCamera.position.y = 1;
	ViewCamera.position.z = -5;
}

bool GameUpdate()
{
	ModelTransform = MatrixMultiply(ModelTransform, MatrixRotateY(GetFrameTime() * DEG2RAD * 10));

	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		CastRay = GetMouseRay(GetMousePosition(), ViewCamera);
	}
	else
	{
		CastRay.position = Vector3{ -5, 0.5f, -5 };
		CastRay.direction = Vector3{ 1, 0, 1 };
	}

	Collision = GetRayCollisionMesh(CastRay, Cube, ModelTransform);
	return true;
}

void GameDraw()
{
	BeginDrawing();
	ClearBackground(DARKGRAY);

	BeginMode3D(ViewCamera);

	DrawMesh(Cube, CubeMat, ModelTransform);

	if (Collision.hit)
	{
		rlDisableDepthTest();
		DrawLine3D(CastRay.position, Collision.point, PURPLE);

		Vector3 reflection = Vector3Reflect(CastRay.direction, Collision.normal);

		Vector3 newEP = Vector3Add(Collision.point, Vector3Scale(reflection, 5));

		DrawLine3D(newEP, Collision.point, YELLOW);
		rlDrawRenderBatchActive();
		rlEnableDepthTest();
	}

	EndMode3D();

	DrawFPS(0, 0);

	if (!Collision.hit)
	{
		DrawText("No hit", 300, 20, 20, MAROON);
	}

	EndDrawing();
}

int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(1280, 800, "Example");
	SetTargetFPS(144);

	GameInit();

	while (!WindowShouldClose())
	{
		if (!GameUpdate())
			break;
		
		GameDraw();
	}

	CloseWindow();
	return 0;
}
