#include "gui_maker.h"
#include "calc.h"
#include <math.h>
#include <stdio.h>
#include <time.h> // only for <test>
#include <stdlib.h>

#define _CRT_SECURE_NO_WARNINGS_
#define CHART_ARRAY 2048

#ifdef _WIN32
	#define PLATFORM "win32"
#elif _WIN64
	#define PLATFORM "win64"
#elif __linux__
	#define PLATFORM "linux"
#endif

#define WIDTH  GetScreenWidth()
#define HEIGHT GetScreenHeight()

// SPECTRON - Super Powerfull Engine Computing Tracing Rays Of Numerics
int main(int argc, char **argv)
{
	InitWindow(WIDTH, HEIGHT, "SPECTRON");
	// program set
		// status for windows & objects
	Signals sig;
	sig.import_message = 0;
	sig.save = 0;
	sig.message_status = 0 ;
	sig.info_status = 0;
	sig.new_status = 0;

	bool file_status = false;
	bool view_status = false;
	bool tools_status = false;

		// object visibility
	float check_width = 20.0;
	bool gizmo_visibility = false;
	bool box_visibility = false;
	bool model_visibility = true;
	bool radar_visibility = false;
	bool grid_visibility = false;
	bool plot_visibility = false;
	
	int panel_state = 0;
	int material_combo = 0;
	int CamSet = 0;
	int RadarSet = 0;

	// widgets parameters
	Geometry geo;
	geo.panel_width = WIDTH * 0.2;
	geo.btn_width = 170.0f;
	geo.btn_height = 50.0f;
	geo.pad_y = geo.btn_height + 10;
	geo.group_width = WIDTH * 0.18;
	geo.slider_width = WIDTH * 0.1;

	// widgets preprogram
	Rectangle PanelBox = {(float) WIDTH - geo.panel_width, 0.0f, (float) WIDTH - 10, (float) HEIGHT};
	Rectangle FileBox = {20.0f, check_width, geo.btn_width, geo.btn_height};
	Rectangle ViewBox = {200.0f, check_width, geo.btn_width, geo.btn_height};
	Rectangle ToolsBox = {380.0f, check_width, geo.btn_width, geo.btn_height};
	Rectangle HelpBox = {560.0f, check_width, geo.btn_width, geo.btn_height};
		// variables for computing cordinates
		// radar
	Radar radar;
	radar.x = 1.0;
	radar.y = 2.0;
	radar.z = 2.0;
	radar.distance = 1;
	radar.azymuth = 0.0f;
	radar.elevation = 0.0f;
	radar.combo = 0;
	radar.lambda = 1.0f;
	radar.freq = 1.0f;
		// camera
	Cam cam;
	cam.x = 0;
	cam.y = -5;
	cam.z = 5;
	cam.distance = 100;
	cam.azymuth = 100;
	cam.elevation = 100;
	cam.combo = 0;

	// Camera 3D PreSetting
	int cam_mode = CAMERA_THIRD_PERSON;
	Camera3D camera = {0};
	camera.position = (Vector3){cam.x, cam.y, cam.z};
	camera.target = (Vector3){0.0f, 0.0f, 0.0f};	
	camera.up = (Vector3){ 0.0f, 0.0f, 1.0f };
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;

		// object
	GuiFileDialogState import_state = InitGuiFileDialog(GetWorkingDirectory());	
	bool import_btn = false;
	Model model = {0};
	char model_name[512];
	char name[512] = "File name";
	float model_scale = 1.0f;

		//gizmo settings
	float gizmo_x = 0;
	float gizmo_y = 0;
	float gizmo_z = 0;
	int segments = 16;
	float cylinder_d = 0.3;
	float cylinder_h = 4;
	float cone_d = 0.6;
	float cone_h = 2 + cylinder_h;

		// config
	int grid_count = 100;
	float grid_res = 0.1;
	char *data_file = {0};
	
		// Vectors & Positions
	Vector3 zero_position = { 0.0f, 0.0f, 0.0f };

	// random seed for <test>
	srand(time(NULL));
	// FPS set
	SetTargetFPS(60);

	// memory for image to plot conversion
	Image image;
	Texture2D image2D;
	const char *font_name = "Roboto_font/RobotoMonoNerdFont-Regular.ttf";
	Font font = LoadFont(font_name);

	// main loop
	while(!WindowShouldClose())
	{
		if(sig.import_message || sig.save || sig.message_status || sig.info_status || sig.new_status) GuiLock();
		else GuiUnlock();
			// image to plot
	//	image = LoadImage("general.png");
	//	image2D = LoadTextureFromImage(image);

		// key shot cuts check
			// Exit <ESC>
		if(IsKeyDown(KEY_ESCAPE))
		{
			WindowShouldClose();
			UnloadModel(model);
			UnloadImage(image);
			UnloadTexture(image2D);	
			UnloadFont(font);
			exit(0);
		}
			// plot visibility <Ctrl + P>
		if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_P)) plot_visibility = !plot_visibility;     
			// grid enable/disable  <Ctrl + G>
		if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_G)) grid_visibility = !grid_visibility;
			// info window enable/disable <Ctrl + I>
		if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_I)) sig.info_status = !sig.info_status;
			// save data <Ctrl + S>
		if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) sig.save = !sig.save;
			// help <Ctrl + H>
		if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_H)) sig.message_status = !sig.message_status;

		// camera position/mode update
		if((IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) || (GetMouseWheelMove() != 0)) 
		{
			UpdateCamera(&camera, cam_mode);
			camera.target = (Vector3){0.0f, 0.0f, 0.0f};	
			cam.x = camera.position.x;
			cam.y = camera.position.y;
			cam.z = camera.position.z;

			Cartesian2Polar(&cam.x, &cam.y, &cam.z, &cam.distance, &cam.azymuth, &cam.elevation);
			Polar2Cartesian(&cam.x, &cam.y, &cam.z, &cam.distance, &cam.azymuth, &cam.elevation);
		}
		else
		{
			camera.target = (Vector3){0.0f, 0.0f, 0.0f};	
			cam.x = camera.position.x;
			cam.y = camera.position.y;
			cam.z = camera.position.z;
			Polar2Cartesian(&cam.x, &cam.y, &cam.z, &cam.distance, &cam.azymuth, &cam.elevation);
			Cartesian2Polar(&cam.x, &cam.y, &cam.z, &cam.distance, &cam.azymuth, &cam.elevation);
		}

		// drawing objects
		BeginDrawing();

			ClearBackground(RAYWHITE);
        			// 3D Mode	
		BeginMode3D(camera);
		if(IsModelReady(model))	
		{
			BoundingBox boundingBox = GetModelBoundingBox(model);
			if(model_visibility)
			{
				DrawModel(model, zero_position, model_scale, GRAY);
				DrawModelWires(model, zero_position, model_scale, BLACK);
			}

			if(box_visibility)
			{
				DrawBoundingBox(boundingBox, DARKBROWN);
			}

			// gizmo lock in corrner of model
			gizmo_x = boundingBox.min.x;
			gizmo_y = boundingBox.min.y;
			gizmo_z = boundingBox.min.z;
		}
			if(radar_visibility) 
			{
				//DrawSphere((Vector3) {x, y, z}, 1.0, BLUE);	
		/*		DrawCylinderEx((Vector3){radar.x, radar.y, radar.z}, 
					       (Vector3){radar.x + 10 * cos(radar.azymuth) * sin(radar.elevation),
							 radar.y + 10 * cos(radar.elevation) * sin(radar.azymuth),
							 radar.z + 10 * cos(radar.azymuth) * sin(radar.azymuth)},
							 0, cylinder_d, segments, DARKBLUE);
			*/
			}
			// grid draw
			if(grid_visibility) DrawGrid(grid_count, grid_res);

			// draw gizmo
			if(gizmo_visibility)
			{
				// X axis RED
				DrawCylinderEx( (Vector3) {gizmo_x, gizmo_y, gizmo_z}, (Vector3) {cylinder_h + gizmo_x, gizmo_y, gizmo_z}, cylinder_d, cylinder_d, segments, RED);
				DrawCylinderWiresEx( (Vector3) {gizmo_x, gizmo_y, gizmo_z}, (Vector3) {cylinder_h + gizmo_x, gizmo_y, gizmo_z}, cylinder_d, cylinder_d, 0, BLACK);
				DrawCylinderEx( (Vector3) {cylinder_h + gizmo_x, gizmo_y, gizmo_z}, (Vector3) {cone_h + gizmo_x, gizmo_y, gizmo_z}, cone_d, 0.0f, segments, RED);
				DrawCylinderWiresEx( (Vector3) {cylinder_h + gizmo_x, gizmo_y, gizmo_z}, (Vector3) {cone_h + gizmo_x, gizmo_y, gizmo_z}, cone_d, 0.0f, 0, BLACK);
				// Y axis GREEN
				DrawCylinderEx( (Vector3) {gizmo_x, gizmo_y, gizmo_z}, (Vector3) {gizmo_x, cylinder_h + gizmo_y, gizmo_z}, cylinder_d, cylinder_d, 0, GREEN);
				DrawCylinderWiresEx( (Vector3) {gizmo_x, gizmo_y, gizmo_z}, (Vector3) {gizmo_x, cylinder_h + gizmo_y, gizmo_z}, cylinder_d, cylinder_d, 0, BLACK);
				DrawCylinderEx( (Vector3) {gizmo_x, cylinder_h + gizmo_y, gizmo_z}, (Vector3) {gizmo_x, cone_h + gizmo_y, gizmo_z}, cone_d, 0.0f, segments, GREEN);
				DrawCylinderWiresEx( (Vector3) {gizmo_x, cylinder_h + gizmo_y, gizmo_z}, (Vector3) {gizmo_x, cone_h + gizmo_y, gizmo_z}, cone_d, 0.0f, 0, BLACK);
				// Z axis BLUE
				DrawCylinderEx( (Vector3) {gizmo_x, gizmo_y, gizmo_z}, (Vector3) {gizmo_x, gizmo_y, cylinder_h + gizmo_z}, cylinder_d, cylinder_d, segments, BLUE);
				DrawCylinderWiresEx( (Vector3) {gizmo_x, gizmo_y, gizmo_z}, (Vector3) {gizmo_x, gizmo_y, cylinder_h + gizmo_z}, cylinder_d, cylinder_d, 0, BLACK);
				DrawCylinderEx( (Vector3) {gizmo_x, gizmo_y, cylinder_h + gizmo_z}, (Vector3) {gizmo_x, gizmo_y, cone_h + gizmo_z}, cone_d, 0.0f, segments, BLUE);
				DrawCylinderWiresEx( (Vector3) {gizmo_x, gizmo_y, cylinder_h + gizmo_z}, (Vector3) {gizmo_x, gizmo_y, cone_h + gizmo_z}, cone_d, 0.0f, 0, BLACK);
			}

		EndMode3D();

				// plot visibility
		if(plot_visibility) DrawTextureEx(image2D, (Vector2) {WIDTH * .01, HEIGHT * .58}, 0.0f, 0.75f, WHITE);

			// Title Text
			DrawText("SPECTRON - Super Powerfull Engine Computing Tracing Rays Of Numerics", 10.0f, 10.0f, 10, BLACK); 
			int FileButton = GuiButton(FileBox, "File");
			int ViewButton = GuiButton(ViewBox, "View");
			int ToolsButton = GuiButton(ToolsBox, "Tools");
			int HelpButton = GuiButton(HelpBox, "Help");

			// GUI programing
			switch(panel_state)
			{
				case 1:
				if(file_status)
				{
					GuiWindowBox(PanelBox, "FILE");
					File(&geo, &model, &image, &image2D, &sig, data_file, &font);
					break;
				}
				else break;

				case 2:
				if(view_status)
				{
					GuiPanel(PanelBox, "VIEW");
					GuiGroupBox((Rectangle) {(float) WIDTH - (geo.group_width + geo.panel_width)/2, 50.0, geo.group_width, 60.0}, "Object visibility");
					// check boxes
					model_visibility = GuiCheckBox((Rectangle) {(float) WIDTH - (geo.group_width + geo.panel_width)/2.4, 70.0f, check_width, 20.0f}, "Model", model_visibility);
					box_visibility = GuiCheckBox((Rectangle) {(float) WIDTH - (geo.group_width + geo.panel_width)/3.5, 70.0f, check_width, 20.0f}, "Box", box_visibility);
					radar_visibility = GuiCheckBox((Rectangle) {(float) WIDTH - (geo.group_width + geo.panel_width)/6.6, 70.0f, check_width, 20.0f}, "Radar", radar_visibility);

					GuiGroupBox((Rectangle) {(float) WIDTH - (geo.group_width + geo.panel_width)/2, 140.0, geo.group_width, 60.0}, "Sceen visibility");
					grid_visibility = GuiCheckBox((Rectangle) {(float) WIDTH - (geo.group_width + geo.panel_width)/2.4, 160.0f, check_width, 20.0f}, "Grid", grid_visibility);
					gizmo_visibility = GuiCheckBox((Rectangle) {(float) WIDTH - (geo.group_width + geo.panel_width)/3.5, 160.0f, check_width, 20.0f}, "Gizmo", gizmo_visibility);

					GuiGroupBox((Rectangle) {(float) WIDTH - (geo.group_width + geo.panel_width)/2, 230.0, geo.group_width, 60.0}, "Data visibility");
					plot_visibility = GuiCheckBox((Rectangle) {(float) WIDTH - (geo.group_width + geo.panel_width)/3.5, 250.0f, check_width, 20.0f}, "plot", plot_visibility);

					break;
				}
				else break;

				case 3:
				if(tools_status)
				{
					GuiPanel(PanelBox, "TOOLS");
					
					RadarSet = Radar_Group(&radar, &geo);
			    
					CamSet = Camera_Group(&cam, &geo);
					
					Object_Group(&import_btn, &material_combo, &geo, name, &model_scale);

					if(import_btn)
					{
						import_state.windowActive = !import_state.windowActive;
					}

					if(import_state.windowActive)
					{
						DrawRectangle(0.0f, 0.0f, (float) WIDTH, (float) HEIGHT, Fade(GRAY, 0.8f));
					}
					importWindow(&import_state, &model, model_name, name, &sig.import_message);
					GuiFileDialog(&import_state);

					break;
				}
				else break;

				case 4:
					if(HelpButton)
					{
						file_status = false;
						view_status = false;
						tools_status = false;
						sig.message_status = !sig.message_status;
					}
					break;
			}
		// Front Buttons
		if(FileButton) 
		{
			view_status = false;
			tools_status = false;
			panel_state = 1;
			file_status = !file_status;
		}
		if(ViewButton) 
		{
			file_status = false;
			tools_status = false;
			panel_state = 2;
			view_status = !view_status;
		}

		if(ToolsButton) 
		{
			file_status = false;
			view_status = false;
			panel_state = 3;
			tools_status = !tools_status;
		}

		if(HelpButton) panel_state = 4;

		if(sig.new_status) newForSave(&sig.new_status, &sig.save, data_file, &font);
			// save window
		if(sig.save) saveWindow(&sig.save, data_file, &font);
			// info window
		if(sig.info_status) infoWindow(&sig.info_status);
			// help window
		if(sig.message_status) helpWindow(&sig.message_status, &font);
			// invalid model import handler
		if(sig.import_message) import_error_window(&sig.import_message);

		// Radar & Camera polar setting
		if(RadarSet)
		{
			radar.azymuth = cam.azymuth;
			radar.elevation = cam.elevation;
		}
		if(CamSet)
		{
			cam.azymuth = radar.azymuth;
			cam.elevation = radar.elevation;
		}

		
		EndDrawing();
	
		UnloadTexture(image2D);	
		UnloadImage(image);
		}

	UnloadTexture(image2D);	
	UnloadImage(image);


	// program cleaning
	UnloadModel(model);
	CloseWindow();

	return 0;
}
