#include <raylib.h>

int main(void)
{

	InitWindow(800, 600, "TEST");

	Camera3D camera = {0};
	camera.position = (Vector3){0, 100, 0};
	camera.target = (Vector3){0.5f, 0.0f, 0.0f};	
	camera.up = (Vector3){ 0.5f, 0.0f, 0.0f };
	camera.fovy = 45.0f;

	Texture2D texture = LoadTexture("SA-3Goa.mtl");

	Model model = LoadModel("SA-3Goa.obj");

	model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
	SetTargetFPS(60);
	while(!WindowShouldClose())
	{
		UpdateCamera(&camera, CAMERA_FREE);
		ClearBackground(RAYWHITE);
		BeginDrawing();
		BeginMode3D(camera);
			DrawModel(model, (Vector3){0.0f, 0.0f, 0.0f}, 1, WHITE);
		EndMode3D();
		EndDrawing();
				
	}
	UnloadTexture(texture);
	UnloadModel(model);
	CloseWindow();
}
