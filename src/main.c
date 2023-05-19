#include "calc.h"
#include "gui_widgets.h"

#define _CRT_SECURE_NO_WARNINGS_

#ifdef _WIN32
	#define PLATFORM "win32"
#elif _WIN64
	#define PLATFORM "win64"
#elif __linux__
	#define PLATFORM "linux"
#endif

// SPECTRON - Super Powerfull Engine Computing Tracing Rays Of Numerics
int main(int argc, char **argv)
{
	// main variables 
	int SCREEN_WIDTH = GetScreenWidth();
	int SCREEN_HEIGHT = GetScreenHeight();

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SPECTRON");
	// program set
		// status for windows & objects
	int message_status = 0;
	bool error = false;
	int save = 0;
	bool grid = true;
	int panel_state = 0;
	int radar_combo = 0;
	int camera_combo = 0;
	int material_combo = 0;

	float btn_width = 170.0f;
	float btn_height = 50.0f;
	float panel_width = GetScreenWidth() * 0.2;
	float group_width = GetScreenWidth() * 0.18;
	float slider_width = GetScreenWidth() * 0.1;
	int pad_y = btn_height + 10;

	Rectangle PanelBox = {(float) GetScreenWidth() - panel_width, 0.0f, (float) GetScreenWidth() - 10, (float) GetScreenHeight()};
	Rectangle FileBox = {20.0f, 20.0f, btn_width, btn_height};
	Rectangle ViewBox = {20.0f, 80.0f, btn_width, btn_height};
	Rectangle ToolsBox = {20.0f, 140.0f, btn_width, btn_height};
	Rectangle HelpBox = {20.0f, 200.0f, btn_width, btn_height};
		// variables for computing cordinates
		// radar
	float x_radar = 1.0;
	float y_radar = 2.0;
	float z_radar = 2.0;
	float distance_radar = 1;
	float azymuth_radar = 0.0f;
	float elevation_radar = 0.0f;
	float freq = 1.0f;
	float lambda = 1.0f;
		// camera
	float x_camera = 0;
	float y_camera = -5;
	float z_camera = 5;
	float distance_camera = 100;
	float azymuth_camera = 100;
	float elevation_camera = 100;

	// Camera 3D PreSetting
	int cam_mode = CAMERA_THIRD_PERSON;
	Camera3D camera = {0};
	camera.position = (Vector3){x_camera, y_camera, z_camera};
	camera.target = (Vector3){0.0f, 0.0f, 0.0f};	
	camera.up = (Vector3){ 0.0f, 0.0f, 1.0f };
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;

		// object
	GuiFileDialogState import_state = InitGuiFileDialog(GetWorkingDirectory());	
	bool import_btn = false;
	Model model = {0};
	char model_name[512];
	char name[512];

		// config
	int grid_count = 100;
	float grid_res = 0.1;
	char data_file[256] = {0};

	
	Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
	SetTargetFPS(60);

	// main loop
	while(!WindowShouldClose())
	{	
		// key shot cuts check
			// grid enable/disable  <Ctrl + G>
		((IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_G))) ? toggle(&grid) : NULL;
			// save data <Ctrl + S>
		if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) save = !save;
			// help <Ctrl + H>
		if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_H)) message_status = !message_status;				
		// camera position/mode update
		if((IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) || (GetMouseWheelMove() != 0)) 
		{
			UpdateCamera(&camera, cam_mode);
			x_camera = camera.position.x;
			y_camera = camera.position.y;
			z_camera = camera.position.z;

			Cartesian2Polar(&x_camera, &y_camera, &z_camera, &distance_camera, &azymuth_camera, &elevation_camera);
			Polar2Cartesian(&x_camera, &y_camera, &z_camera, &distance_camera, &azymuth_camera, &elevation_camera);
		}
		else
		{
			x_camera = camera.position.x;
			y_camera = camera.position.y;
			z_camera = camera.position.z;
			Polar2Cartesian(&x_camera, &y_camera, &z_camera, &distance_camera, &azymuth_camera, &elevation_camera);
			Cartesian2Polar(&x_camera, &y_camera, &z_camera, &distance_camera, &azymuth_camera, &elevation_camera);
		}

		// drawing objects
		BeginDrawing();

			ClearBackground(RAYWHITE);
        			// 3D Mode	
		BeginMode3D(camera);
		if(IsModelReady(model))	DrawModel(model, cubePosition, 1.0f, GRAY);
                	DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);
			DrawSphere((Vector3) {x_radar, y_radar, z_radar}, 1.0, BLUE);		
			// grid draw
			if(grid) DrawGrid(grid_count, grid_res);
		EndMode3D();
       	
			// Title Text
			DrawText("SPECTRON - Super Powerfull Engine Computing Tracing Rays Of Numerics", 10.0f, 10.0f, 10, BLACK);
			DrawFPS(SCREEN_WIDTH * .9, 10);
			int FileButton = GuiButton(FileBox, "File");
			int ViewButton = GuiButton(ViewBox, "View");
			int ToolsButton = GuiButton(ToolsBox, "Tools");
			int HelpButton = GuiButton(HelpBox, "Help");

			// GUI programing
			switch(panel_state)
			{
				case 1:
					GuiWindowBox(PanelBox, "FILE");
					File(&panel_width, &btn_width, &btn_height, &pad_y, &model);
					break;
				case 2:
					GuiPanel(PanelBox, "View");
					break;
				case 3:
					GuiPanel(PanelBox, "Tools");
					Radar_Group(&x_radar, &y_radar, &z_radar, &distance_radar, &azymuth_radar, &elevation_radar, &radar_combo, &lambda, &freq, &group_width, &panel_width, &slider_width);
					Camera_Group(&camera.position.x, &camera.position.y, &camera.position.z, &distance_camera, &azymuth_camera, &elevation_camera, &camera_combo, &group_width, &panel_width, &slider_width);
					
					strcmp(name, "") ? "File name" : name;
					Object_Group(&import_btn, &material_combo, &group_width, &panel_width, name);

					if(import_btn)
					{
						//UnloadModel(model);
						toggle(&import_state.windowActive);
					}
						if(import_state.windowActive)
						{
							DrawRectangle(0.0f, 0.0f, (float) GetScreenWidth(), (float) GetScreenHeight(), Fade(GRAY, 0.8f));
						}
					importWindow(&import_state, &model, model_name, name);
					GuiFileDialog(&import_state);

					
					break;
				case 4:
					if(HelpButton) message_status = !message_status;
					break;
	
			}
		// Front Buttons
		if(FileButton) panel_state = 1;
		if(ViewButton) panel_state = 2;
		if(ToolsButton) panel_state = 3;
		if(HelpButton) panel_state = 4;

			// help window
		if(message_status) helpWindow(&message_status);
			// save window
		if(save) saveWindow(&save, data_file);

		EndDrawing();
	}
	
	// program cleaning
	UnloadModel(model);
	CloseWindow();

	return 0;
}
