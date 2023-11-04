#include "raylib_binaries/raygui/src/raygui.h"
#include "gui_maker.h"
#include "calc.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <rcamera.h>

#define _CRT_SECURE_NO_WARNINGS_
#ifdef _WIN32
	#define PLATFORM "win32"
#elif _WIN64
	#define PLATFORM "win64"
#elif __linux__
	#define PLATFORM "linux"
#endif

// Raylib Camera definitions
#define PLAYER_MOVEMENT_SENSITIVITY 0.0f
#define CAMERA_ROTATION_SPEED	    0.03f

#define WIDTH  GetScreenWidth()
#define HEIGHT GetScreenHeight()
#define LIGHT  300000

// SPECTRON - Super Powerfull Engine Computing Tracing Rays Of Numerics
int main(void)
{
	InitWindow(WIDTH, HEIGHT, "SPECTRON");
	// program set
		// status for windows & objects
	// flags
	SetConfigFlags(FLAG_VSYNC_HINT);
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	// 
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
	VIS vis;
	vis.gizmo = false;
	vis.box = false;
	vis.model = true;
	vis.radar = false;
	vis.grid = false;
	vis.plot = false;
	vis.wave = false;
	
	int panel_state = 0;
	int material_combo = 0;
	int CamSet = 0;
	int RadarSet = 0;

	// widgets parameters
	Geometry geo;
	geo.panel_width = WIDTH * 0.25;
	geo.btn_width = 120.0f;
	geo.btn_height = 30.0f;
	geo.pad_y = geo.btn_height + 10;
	geo.group_width = WIDTH * 0.22;
	geo.slider_width = WIDTH * 0.1;

	// widgets preprogram
	Rectangle PanelBox = {(float) WIDTH - geo.panel_width, 0.0f, (float) WIDTH - 10, (float) HEIGHT};
	Rectangle FileBox = {700.0f, check_width, geo.btn_width, geo.btn_height};
	Rectangle ViewBox = {850.0f, check_width, geo.btn_width, geo.btn_height};
	Rectangle ToolsBox = {1000.0f, check_width, geo.btn_width, geo.btn_height};
	Rectangle HelpBox = {1150.0f, check_width, geo.btn_width, geo.btn_height};
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
	radar.freq_fix = 1;
	radar.freq = 1.0f;
	radar.lambda = freq2wave(&radar);
	sprintf(radar.lambda_fix, "m");

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
	char model_name[2048];
	char name[2048] = "File name";
	float model_scale = 1.0f;

	BoundingBox boundingBox = {0};
		//gizmo settings
	Gizmo gizmo;
	gizmo.x = 0;
	gizmo.y = 0;
	gizmo.z = 0;
	gizmo.segments = 16;
	gizmo.cylinder_d = 0.3;
	gizmo.cylinder_h = 4;
	gizmo.cone_d = 0.6;
	gizmo.cone_h = 2 + gizmo.cylinder_h;
	gizmo.start = (Vector3) {gizmo.x, gizmo.y, gizmo.z};
	gizmo.endRed = (Vector3) {gizmo.cylinder_h + gizmo.x, gizmo.y, gizmo.z};
	gizmo.endGreen = (Vector3) {gizmo.x, gizmo.cylinder_h + gizmo.y, gizmo.z};
	gizmo.endBlue = (Vector3) {gizmo.x, gizmo.y, gizmo.cylinder_h + gizmo.z};
		// config
	int grid_count = 100;
	float grid_res = 0.1;
	char data_file[512];
	
		// Vectors & Positions
	Vector3 zero_position = { 0.0f, 0.0f, 0.0f };

	// FPS set
	SetTargetFPS(60);

	const char *font_name = "Roboto_font/RobotoMonoNerdFont-Regular.ttf";
	Font font = LoadFont(font_name);

	DATA data;

	data.x_max = DBL_MIN;
	data.x_min = DBL_MAX;

	for(size_t i = 0; i < sizeof(data.x)/sizeof(data.x[0]); ++i)
	{
		data.x[i] = 360*i/(sizeof(data.x)/sizeof(data.x[0]));
		if(data.x[i] < data.x_min) data.x_min = data.x[i];
		if(data.x[i] > data.x_max) data.x_max = data.x[i];
	}
	// main loop
	while(!WindowShouldClose())
	{
		if(sig.import_message || sig.save || sig.message_status || sig.info_status || sig.new_status) GuiLock();
		else GuiUnlock();
		
		radar.lambda = freq2wave(&radar);
		
		// key short cuts check
		// Camera RotationControl <fixing raylib implementation>
		if (IsKeyDown(KEY_Q)) CameraRoll(&camera, CAMERA_ROTATION_SPEED); 
		if (IsKeyDown(KEY_E)) CameraRoll(&camera, -CAMERA_ROTATION_SPEED);

			// Exit <ESC>
		if(IsKeyDown(KEY_ESCAPE))
		{
			WindowShouldClose();
			UnloadModel(model);
			UnloadFont(font);
			exit(0);
		}
			// plot visibility <Ctrl + P>
		if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_P)) vis.plot = !vis.plot;     
			// grid enable/disable  <Ctrl + G>
		if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_G)) vis.grid = !vis.grid;
			// info window enable/disable <Ctrl + I>
		if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_I)) sig.info_status = !sig.info_status;
			// new project (clear) <Ctrl + N>
		if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_N)) sig.new_status = !sig.new_status;
			// save data <Ctrl + S>
		if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) sig.save = !sig.save;
			// help <Ctrl + H>
		if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_H)) sig.message_status = !sig.message_status;
		if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_W)) vis.wave = !vis.wave;

		UpdateCamera(&camera, cam_mode);
		camera.target = zero_position;	
			
		// camera position/mode update
		if((IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) || (GetMouseWheelMove() != 0)) 
		{

			Polar2Cartesian(&cam.x, &cam.y, &cam.z, &cam.distance, &cam.azymuth, &cam.elevation);
			cam.x = camera.position.x;
			cam.y = camera.position.y;
			cam.z = camera.position.z;

			Cartesian2Polar(&cam.x, &cam.y, &cam.z, &cam.distance, &cam.azymuth, &cam.elevation);
		}
		else
		{
			if(cam.combo == 0) Cartesian2Polar(&cam.x, &cam.y, &cam.z, &cam.distance, &cam.azymuth, &cam.elevation);
			else Polar2Cartesian(&cam.x, &cam.y, &cam.z, &cam.distance, &cam.azymuth, &cam.elevation);
			camera.position.x = cam.x;
			camera.position.y = cam.y;
			camera.position.z = cam.z;
		}

				// drawing objects
		BeginDrawing();

		ClearBackground(RAYWHITE);
        			// 3D Mode	
		BeginMode3D(camera);
		if(IsModelReady(model))	
		{
	
			boundingBox = GetModelBoundingBox(model);
			if(vis.model)
			{
				DrawModel(model, zero_position, model_scale, GRAY);
				DrawModelWires(model, zero_position, model_scale, BLACK);
			}

			if(vis.box)
			{
				DrawBoundingBox(boundingBox, DARKBROWN);
			}

			// gizmo lock in corrner of model
			gizmo.x = boundingBox.min.x;
			gizmo.y = boundingBox.min.y;
			gizmo.z = boundingBox.min.z;
			gizmo.start = (Vector3) {gizmo.x, gizmo.y, gizmo.z};
			gizmo.endRed = (Vector3) {gizmo.cylinder_h + gizmo.x, gizmo.y, gizmo.z};
			gizmo.endGreen = (Vector3) {gizmo.x, gizmo.cylinder_h + gizmo.y, gizmo.z};
			gizmo.endBlue = (Vector3) {gizmo.x, gizmo.y, gizmo.cylinder_h + gizmo.z};

		}

		if(radar.combo == 0) Cartesian2Polar(&radar.x, &radar.y, &radar.z, &radar.distance, &radar.azymuth, &radar.elevation);
		else Polar2Cartesian(&radar.x, &radar.y, &radar.z, &radar.distance, &radar.azymuth, &radar.elevation);

			if(vis.radar) 
			{
						//DrawSphere((Vector3) {x, y, z}, 1.0, BLUE);	
				DrawCylinderEx(	(Vector3){radar.x, radar.y, radar.z}, 
						(Vector3){(radar.x+1)*cos(radar.azymuth * PI/180),
							  (radar.y+1)*sin(radar.azymuth * PI/180),
							  (radar.z+1)*sin(radar.elevation * PI/180)},
						0, gizmo.cylinder_d, gizmo.segments,
						(Color){0x10, 0x4F, 0xCC, 0x55});
			}
			if(vis.wave)
			{
				DrawCube((Vector3){boundingBox.min.x , boundingBox.min.y , boundingBox.min.z }, 100, 100, 1, (Color){0, 100, 100, 125});
			}
			// grid draw
			if(vis.grid && IsModelReady(model)) DrawGrid(2, 10);
			else if(vis.grid) DrawGrid(grid_count, grid_res);

			// draw gizmo
			if(vis.gizmo)
			{
				// X axis RED
				DrawCylinderEx(gizmo.start, gizmo.endRed, gizmo.cylinder_d, gizmo.cylinder_d, gizmo.segments, RED);
				DrawCylinderWiresEx(gizmo.start, gizmo.endRed, gizmo.cylinder_d, gizmo.cylinder_d, 0, BLACK);
				DrawCylinderEx(gizmo.endRed, (Vector3) {gizmo.cone_h + gizmo.x, gizmo.y, gizmo.z}, gizmo.cone_d, 0.0f, gizmo.segments, RED);
				DrawCylinderWiresEx(gizmo.endRed, (Vector3) {gizmo.cone_h + gizmo.x, gizmo.y, gizmo.z}, gizmo.cone_d, 0.0f, 0, BLACK);
				// Y axis GREEN
				DrawCylinderEx(gizmo.start, gizmo.endGreen, gizmo.cylinder_d, gizmo.cylinder_d, 0, GREEN);
				DrawCylinderWiresEx(gizmo.start, gizmo.endGreen, gizmo.cylinder_d, gizmo.cylinder_d, 0, BLACK);
				DrawCylinderEx(gizmo.endGreen, (Vector3) {gizmo.x, gizmo.cone_h + gizmo.y, gizmo.z}, gizmo.cone_d, 0.0f, gizmo.segments, GREEN);
				DrawCylinderWiresEx(gizmo.endGreen, (Vector3) {gizmo.x, gizmo.cone_h + gizmo.y, gizmo.z}, gizmo.cone_d, 0.0f, 0, BLACK);
				// Z axis BLUE
				DrawCylinderEx(gizmo.start, gizmo.endBlue, gizmo.cylinder_d, gizmo.cylinder_d, gizmo.segments, BLUE);
				DrawCylinderWiresEx(gizmo.start, gizmo.endBlue, gizmo.cylinder_d, gizmo.cylinder_d, 0, BLACK);
				DrawCylinderEx(gizmo.endBlue, (Vector3) {gizmo.x, gizmo.y, gizmo.cone_h + gizmo.z}, gizmo.cone_d, 0.0f, gizmo.segments, BLUE);
				DrawCylinderWiresEx(gizmo.endBlue, (Vector3) {gizmo.x, gizmo.y, gizmo.cone_h + gizmo.z}, gizmo.cone_d, 0.0f, 0, BLACK);
			}

		EndMode3D();

		DataCounter(&data);

		if(vis.plot)
		{
			PowerChart(&data); 
			PolarChart(&data);
		}

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
					File(&geo, &model, &sig, data_file, &data, &font);
					break;
				}
				else break;

				case 2:
				if(view_status)
				{
					GuiPanel(PanelBox, "VIEW");
					GuiGroupBox((Rectangle) {(float) WIDTH - (geo.group_width + geo.panel_width)/2, 50.0, geo.group_width, 60.0}, "Object visibility");
					// check boxes
					vis.model = GuiCheckBox((Rectangle) {(float) WIDTH - (geo.group_width + geo.panel_width)/2.4, 70.0f, check_width, 20.0f}, "Model", vis.model);
					vis.box = GuiCheckBox((Rectangle) {(float) WIDTH - (geo.group_width + geo.panel_width)/3.5, 70.0f, check_width, 20.0f}, "Box", vis.box);
					vis.radar = GuiCheckBox((Rectangle) {(float) WIDTH - (geo.group_width + geo.panel_width)/6.6, 70.0f, check_width, 20.0f}, "Radar", vis.radar);

					GuiGroupBox((Rectangle) {(float) WIDTH - (geo.group_width + geo.panel_width)/2, 140.0, geo.group_width, 60.0}, "Sceen visibility");
					vis.grid = GuiCheckBox((Rectangle) {(float) WIDTH - (geo.group_width + geo.panel_width)/2.4, 160.0f, check_width, 20.0f}, "Grid", vis.grid);
					vis.gizmo = GuiCheckBox((Rectangle) {(float) WIDTH - (geo.group_width + geo.panel_width)/3.5, 160.0f, check_width, 20.0f}, "Gizmo", vis.gizmo);

					GuiGroupBox((Rectangle) {(float) WIDTH - (geo.group_width + geo.panel_width)/2, 230.0, geo.group_width, 60.0}, "Data visibility");
					vis.plot = GuiCheckBox((Rectangle) {(float) WIDTH - (geo.group_width + geo.panel_width)/3.5, 250.0f, check_width, 20.0f}, "plot", vis.plot);

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

		if(sig.new_status) newForSave(&sig.new_status, &sig.save, data_file, &data, &model);
			// save window
		if(sig.save) saveWindow(&sig.save, data_file, &data);
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
		DrawFPS(10, HEIGHT-20);
	
		}

	UnloadFont(font);
	// program cleaning
	UnloadModel(model);
	CloseWindow();
	return 0;
}
