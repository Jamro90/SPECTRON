#include "gui_maker.h"
#include <stdio.h>
#include <time.h> // only for <test>
#include <stdlib.h>
#include "calc.h"
//#include "chart_maker.h"
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
	int message_status = 0;
	int info_status = 0;
	int import_message = 0;
	bool save = 0;

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
	int radar_combo = 0;
	int camera_combo = 0;
	int material_combo = 0;
	int CamSet = 0;
	int RadarSet = 0;

	// widgets parameters
	float btn_width = 170.0f;
	float btn_height = 50.0f;
	float panel_width = WIDTH * 0.2;
	float group_width = WIDTH * 0.18;
	float slider_width = WIDTH * 0.1;
	int pad_y = btn_height + 10;

	// widgets preprogram
	Rectangle PanelBox = {(float) WIDTH - panel_width, 0.0f, (float) WIDTH - 10, (float) HEIGHT};
	Rectangle FileBox = {20.0f, check_width, btn_width, btn_height};
	Rectangle ViewBox = {200.0f, check_width, btn_width, btn_height};
	Rectangle ToolsBox = {380.0f, check_width, btn_width, btn_height};
	Rectangle HelpBox = {560.0f, check_width, btn_width, btn_height};
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
	char data_file[256] = {0};
	
		// Vectors & Positions
	Vector3 zero_position = { 0.0f, 0.0f, 0.0f };

		// charts variables
//	PLFLT x_chart[CHART_ARRAY];
//	PLFLT y_chart[CHART_ARRAY];

	// random seed for <test>
	srand(time(NULL));
	// FPS set
	SetTargetFPS(60);

	// memory for image to plot conversion
	Image image;
	Texture2D image2D;
	const char *font_name = "Roboto_font/RobotoMonoNerdProto_Regular.ttf";
	Font font = LoadFont(font_name);

	// main loop
	while(!WindowShouldClose())
	{
		if(import_message || save || message_status || info_status) GuiLock();
		else GuiUnlock();
/*
		for(int i = 0; i < CHART_ARRAY; ++i)
		{
			x_chart[i] = (PLFLT) i;
			y_chart[i] = (PLFLT) rand() ;
		}
*/
	// plotting using plplot <test>
//		GeneralChart(&x_chart, &y_chart);
		
			// image to plot
		image = LoadImage("general.png");
		image2D = LoadTextureFromImage(image);

		// key shot cuts check
			// Exit <ESC>
		if(IsKeyDown(KEY_ESCAPE))
		{
			WindowShouldClose();
			UnloadModel(model);
			UnloadImage(image);
			UnloadTexture(image2D);	
			exit(0);
		}
			// plot visibility <Ctrl + P>
		((IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_P))) ? toggle(&plot_visibility) : NULL;	
			// grid enable/disable  <Ctrl + G>
		((IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_G))) ? toggle(&grid_visibility) : NULL;
		((IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_I))) ? info_status = !info_status : NULL;
			// save data <Ctrl + S>
		(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) ? save = !save : NULL;
			// help <Ctrl + H>
		(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_H)) ? message_status = !message_status : NULL; 

		// camera position/mode update
		if((IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) || (GetMouseWheelMove() != 0)) 
		{
			UpdateCamera(&camera, cam_mode);
			camera.target = (Vector3){0.0f, 0.0f, 0.0f};	
			x_camera = camera.position.x;
			y_camera = camera.position.y;
			z_camera = camera.position.z;

			Cartesian2Polar(&x_camera, &y_camera, &z_camera, &distance_camera, &azymuth_camera, &elevation_camera);
			Polar2Cartesian(&x_camera, &y_camera, &z_camera, &distance_camera, &azymuth_camera, &elevation_camera);
		}
		else
		{
			camera.target = (Vector3){0.0f, 0.0f, 0.0f};	
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
			if(radar_visibility) DrawSphere((Vector3) {x_radar, y_radar, z_radar}, 1.0, BLUE);		

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
					File(&panel_width, &btn_width, &btn_height, &pad_y, &model, &image, &image2D, &save, &info_status, &data_file);
					break;
				}
				else break;

				case 2:
				if(view_status)
				{
					GuiPanel(PanelBox, "VIEW");
					GuiGroupBox((Rectangle) {(float) WIDTH - (group_width + panel_width)/2, 50.0, group_width, 60.0}, "Object visibility");
					// check boxes
					model_visibility = GuiCheckBox((Rectangle) {(float) WIDTH - (group_width + panel_width)/2.4, 70.0f, check_width, 20.0f}, "Model", model_visibility);
					box_visibility = GuiCheckBox((Rectangle) {(float) WIDTH - (group_width + panel_width)/3.5, 70.0f, check_width, 20.0f}, "Box", box_visibility);
					radar_visibility = GuiCheckBox((Rectangle) {(float) WIDTH - (group_width + panel_width)/6.6, 70.0f, check_width, 20.0f}, "Radar", radar_visibility);

					GuiGroupBox((Rectangle) {(float) WIDTH - (group_width + panel_width)/2, 140.0, group_width, 60.0}, "Sceen visibility");
					grid_visibility = GuiCheckBox((Rectangle) {(float) WIDTH - (group_width + panel_width)/2.4, 160.0f, check_width, 20.0f}, "Grid", grid_visibility);
					gizmo_visibility = GuiCheckBox((Rectangle) {(float) WIDTH - (group_width + panel_width)/3.5, 160.0f, check_width, 20.0f}, "Gizmo", gizmo_visibility);

					GuiGroupBox((Rectangle) {(float) WIDTH - (group_width + panel_width)/2, 230.0, group_width, 60.0}, "Data visibility");
					plot_visibility = GuiCheckBox((Rectangle) {(float) WIDTH - (group_width + panel_width)/3.5, 250.0f, check_width, 20.0f}, "plot", plot_visibility);

					break;
				}
				else break;

				case 3:
				if(tools_status)
				{
					GuiPanel(PanelBox, "TOOLS");
					
					RadarSet = Radar_Group(&x_radar, &y_radar, &z_radar, &distance_radar, &azymuth_radar, &elevation_radar, &radar_combo, &lambda, &freq, &group_width, &panel_width, &slider_width);
					CamSet = Camera_Group(&camera.position.x, &camera.position.y, &camera.position.z, &distance_camera, &azymuth_camera, &elevation_camera, &camera_combo, &group_width, &panel_width, &slider_width);
					
					Object_Group(&import_btn, &material_combo, &group_width, &panel_width, &slider_width, name, &model_scale);

					if(import_btn)
					{
						toggle(&import_state.windowActive);
					}

					if(import_state.windowActive)
					{
						DrawRectangle(0.0f, 0.0f, (float) WIDTH, (float) HEIGHT, Fade(GRAY, 0.8f));
					}
					importWindow(&import_state, &model, model_name, name, &import_message);
					GuiFileDialog(&import_state);

					break;
				}
				else break;

				case 4:
					if(HelpButton) message_status = !message_status;
					break;
			}
		// Front Buttons
		if(FileButton) 
		{
			panel_state = 1;
			file_status = !file_status;
		}
		if(ViewButton) 
		{
			panel_state = 2;
			view_status = !view_status;
		}

		if(ToolsButton) 
		{
			panel_state = 3;
			tools_status = !tools_status;
		}

		if(HelpButton) panel_state = 4;

			// save window
		if(save) saveWindow(&save, data_file);
			// info window
		if(info_status) infoWindow(&info_status);
			// help window
		if(message_status) helpWindow(&message_status);
			// invalid model import handler
		if(import_message) import_error_window(&import_message);

		// Radar & Camera polar setting
		if(RadarSet)
		{
			azymuth_radar = azymuth_camera;
			elevation_radar = elevation_camera;
		}
		if(CamSet)
		{
			azymuth_camera = azymuth_radar;
			elevation_camera = elevation_radar;
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
