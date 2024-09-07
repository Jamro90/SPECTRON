#include "raylib_binaries/raygui/src/raygui.h"
#include "gui_maker.h"
#include "calc.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <rcamera.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS_
#ifdef _WIN32
	#define PLATFORM "win32"
#elif _WIN64
	#define PLATFORM "win64"
#elif __linux__
	// at the time only POSIX threading 
	#include <pthread.h>
	#include <unistd.h>
#endif

// Raylib Camera definitions
#define PLAYER_MOVEMENT_SENSITIVITY	0.0f
#define CAMERA_ROTATION_SPEED		0.03f

#define WIDTH				GetScreenWidth()
#define HEIGHT				GetScreenHeight()
#define LIGHT				300000

// SPECTRON - Super Powerfull Engine Computing Tracing Rays Of Numerics
int main(void){
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
	sig.loop_hole = true;
	sig.control = false;
	sig.error_progress = false;

	enum STATE{
		FILE = 1,
		VIEW,
		TOOLS,
		HELP
	};
	
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
	vis.progress = false;
	
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
	radar.x = 10.0;
	radar.y = 10.0;
	radar.z = 0.0;
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
	cam.y = -10;
	cam.z = 10;
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
	bounding bounding = {0};

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

	// data setting
	DATA data = {0};
	data.y_max = DBL_MIN;
	data.y_min = DBL_MAX;
	data.iter = 0;

	float progress = 0.0f;

	Image imag = {0};
	Image plot_polar = {0};
	Image plot_chart = {0};

	uint16_t items = sizeof(data.point.x)/sizeof(data.point.x[0]);
	for(uint16_t i = 0; i < items; ++i){
		data.point.x[i] = 360*i/items;
	}

	// main loop
	while(!WindowShouldClose() && sig.loop_hole){
			
		if(sig.import_message || sig.save || sig.message_status || sig.info_status || sig.new_status || sig.control) GuiLock();
		else GuiUnlock();
		
		radar.lambda = freq2wave(&radar);
		
		// key short cuts check
		// Camera RotationControl <fixing raylib implementation>
		if (IsKeyDown(KEY_Q)) CameraRoll(&camera, CAMERA_ROTATION_SPEED); 
		if (IsKeyDown(KEY_E)) CameraRoll(&camera, -CAMERA_ROTATION_SPEED);

		// shortcuts
			// Exit <ESC>
		if(IsKeyDown(KEY_ESCAPE)) sig.loop_hole = false;	
			// new project <CTRL + N>
		if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_N)) sig.new_status = !sig.new_status;
			// save data <CTRL + S>
		if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) sig.save = !sig.save;
			// info window <CTRL + I>
		if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_I)) sig.info_status = !sig.info_status;
			// help window <CTRL + H>
		if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_H)) sig.message_status = !sig.message_status;
			// show/hide plots
		if(IsKeyPressed(KEY_P)) vis.plot = !vis.plot;
			// show/hide radar 
		if(IsKeyPressed(KEY_R)) vis.radar = !vis.radar;
			// show/hide grid 
		if(IsKeyPressed(KEY_G)) vis.grid = !vis.grid;
			// show/hide gizmo 
		if(IsKeyPressed(KEY_J)) vis.gizmo = !vis.gizmo;
			// show/hide boundingbox 
		if(IsKeyPressed(KEY_B)) vis.box = !vis.box;
			// show/hide model 
		if(IsKeyPressed(KEY_M)) vis.model = !vis.model;
			// show/hide plane wave
		if(IsKeyPressed(KEY_W)) vis.wave = !vis.wave;
		
		if((IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_T)) || sig.control) vis.progress = !vis.progress;

		camera.target = zero_position;	
			
		// camera position/mode update
		UpdateCamera(&camera, cam_mode);
		if((IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) || (GetMouseWheelMove() != 0)) {
			Polar2Cartesian(&cam.x, &cam.y, &cam.z, &cam.distance, &cam.azymuth, &cam.elevation);

			cam.x = camera.position.x;
			cam.y = camera.position.y;
			cam.z = camera.position.z;

			Cartesian2Polar(&cam.x, &cam.y, &cam.z, &cam.distance, &cam.azymuth, &cam.elevation);
		}
		else{
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
		if(IsModelReady(model)){
			boundingBox = GetModelBoundingBox(model);
			bounding.x = boundingBox.max.x - boundingBox.min.x;
			bounding.y = boundingBox.max.y - boundingBox.min.y;
			bounding.z = boundingBox.max.z - boundingBox.min.z;

			if((bounding.x > bounding.y) && (bounding.x > bounding.z)) bounding.ratio = 100*bounding.x/items;
			else if((bounding.y > bounding.x) && (bounding.y > bounding.z)) bounding.ratio = 100*bounding.y/items;
			else if((bounding.z > bounding.x) && (bounding.z > bounding.y)) bounding.ratio = 100*bounding.z/items;
			else bounding.ratio = 100*bounding.x/items;

			if(vis.model){
				DrawModel(model, zero_position, model_scale, GRAY);
				DrawModelWires(model, zero_position, model_scale, BLACK);
			}

			if(vis.box) DrawBoundingBox(boundingBox, DARKBROWN);
		
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

			if(vis.radar){
				DrawSphere((Vector3) {radar.x, radar.y, radar.z}, 0.5, BLUE);	
				DrawSphereWires((Vector3) {radar.x, radar.y, radar.z}, 0.5, 20, 20, BLACK);
			}

			if(vis.wave) DrawCube((Vector3){boundingBox.min.x , boundingBox.min.y , boundingBox.min.z }, 100, 100, 1, (Color){0, 100, 100, 125});
		
			// grid draw
			if(vis.grid && IsModelReady(model)) DrawGrid(2, 10);
			else if(vis.grid) DrawGrid(grid_count, grid_res);

			// draw gizmo
			if(vis.gizmo){
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

		if(IsModelReady(model) && (progress < 100)) DataCounter(&data, &model, &radar, &bounding, &progress);
		else if((sig.control || (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_T))) && (progress == 100)){
			memset(data.point.x, 0, 1024*sizeof(double));
			memset(data.point.y, 0, 1024*sizeof(double));

			data.y_max = DBL_MIN;
			data.y_min = DBL_MAX;
			data.iter = 0;
			progress = 0;
		}

				EndMode3D();

		if(vis.plot && IsImageReady(plot_chart) && IsImageReady(plot_chart)) DrawCharts(&data);

		if(vis.progress && IsModelReady(model)) progressWindow(&vis.progress, &progress);

		if(!IsModelReady(model) && ((IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_T)) || sig.control)){
			sig.error_progress = true; 
			sig.control = true;
		}

			// Title Text
		DrawText("SPECTRON - Super Powerfull Engine Computing Tracing Rays Of Numerics", 10.0f, 10.0f, 10, BLACK); 
		int FileButton = GuiButton(FileBox, "File");
		int ViewButton = GuiButton(ViewBox, "View");
		int ToolsButton = GuiButton(ToolsBox, "Tools");
		int HelpButton = GuiButton(HelpBox, "Help");

			// GUI programing
			switch(panel_state){
				case FILE:
				if(file_status){
					GuiWindowBox(PanelBox, "FILE");
					File(&geo, &model, &sig, data_file, &data);
					break;
				}
				else break;

				case VIEW:
				if(view_status){
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

				case TOOLS:
				if(tools_status){
					GuiPanel(PanelBox, "TOOLS");
					
					RadarSet = Radar_Group(&radar, &geo);
			    
					CamSet = Camera_Group(&cam, &geo);
					
					Object_Group(&import_btn, &material_combo, &geo, name, &model_scale);

					Control_Group(&sig.control, &geo);

					if(import_btn) import_state.windowActive = !import_state.windowActive;

					if(import_state.windowActive) DrawRectangle(0.0f, 0.0f, (float) WIDTH, (float) HEIGHT, Fade(GRAY, 0.8f));

					importWindow(&import_state, &model, model_name, name, &sig.import_message);
					GuiFileDialog(&import_state);

					break;
				}
				else break;

				case HELP:
					if(HelpButton){
						file_status = false;
						view_status = false;
						tools_status = false;
						sig.message_status = !sig.message_status;
					}
					break;
			}
		// Front Buttons
		if(FileButton){
			view_status = false;
			tools_status = false;
			panel_state = FILE;
			file_status = !file_status;
		}

		if(ViewButton){
			file_status = false;
			tools_status = false;
			panel_state = VIEW;
			view_status = !view_status;
		}

		if(ToolsButton){
			file_status = false;
			view_status = false;
			panel_state = TOOLS;
			tools_status = !tools_status;
		}

		if(HelpButton) panel_state = HELP;

		if(sig.new_status) newForSave(&sig.new_status, &sig.save, data_file, &data, &model);
			// save window
		if(sig.save) saveWindow(&sig.save, data_file, &data);
			// info window
		if(sig.info_status) infoWindow(&sig.info_status);
			// help window
		if(sig.message_status) helpWindow(&sig.message_status);
			// invalid model import handler
		if(sig.import_message) import_error_window(&sig.import_message);
			// error window for not importing model
		if(sig.error_progress) errorProgressWindow(&sig.error_progress, &sig.control);
		
		// Radar & Camera polar setting
		if(RadarSet){
			radar.azymuth = cam.azymuth;
			radar.elevation = cam.elevation;
		}
		if(CamSet){
			cam.azymuth = radar.azymuth;
			cam.elevation = radar.elevation;
		}
		
		DrawFPS(10, HEIGHT-20);
		EndDrawing();

		imag = LoadImageFromScreen();
		plot_polar = ImageFromImage(imag, (Rectangle) {90, HEIGHT/20 - 30, 470, HEIGHT/2-50});
		plot_chart = ImageFromImage(imag, (Rectangle) {0, HEIGHT/1.9 - 30, 580, 530});

		if(sig.save && (saveWindow(&sig.save, data_file, &data) == 1)){
			char file_name[1024];
			snprintf(file_name, 1024, "%s/polar.png", data_file);
			ExportImage(plot_polar, file_name);
			snprintf(file_name, 1024, "%s/chart.png", data_file);
			ExportImage(plot_chart, file_name);
		}
	
		UnloadImage(imag);
		UnloadImage(plot_polar);
		UnloadImage(plot_chart);

	} // while true
	//
	// program cleaning
	UnloadModel(model);
	CloseWindow();
	return 0;
}
