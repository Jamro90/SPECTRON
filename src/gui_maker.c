#include "gui_maker.h"
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui/src/raygui.h"
#undef RAYGUI_IMPLEMENTATION
#define GUI_FILE_DIALOG_IMPLEMENTATION
#include "raygui/gui_file_dialog.h"
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#ifdef _WIN32
	#define PLATFORM "win32"
#elif _WIN64
	#include <direct.h>
	#define PLATFORM "win64"
	#define MKDIR(x) mkdir(x)
#elif __linux__
	#define PLATFORM "linux"
	#define MKDIR(x) mkdir(x, 0777)
#endif

#define WIDTH  GetScreenWidth()
#define HEIGHT GetScreenHeight()

int import_error_window(int *status)
{
	GuiUnlock();
	Rectangle window = {(float) WIDTH/8, (float) HEIGHT/8, (float) WIDTH/4, (float) HEIGHT/4};
	int gui = GuiMessageBox(window, GuiIconText(ICON_GEAR_EX, "Invalid model extension!"), TextFormat("%s", "Supported extension: .obj"), "Ok");

	if( (gui == 0) || (gui == 1) ) *status = 0;
	GuiLock();
	return gui;
}

void importWindow(GuiFileDialogState *import_state, Model *model, char *model_name, char *name, int *message)
{
	if(import_state->SelectFilePressed)
	{
		if(  (IsFileExtension(import_state->fileNameText, ".obj")) )
		{
			strcpy(model_name, TextFormat("%s/%s", import_state->dirPathText, import_state->fileNameText));
			strcpy(name, TextFormat("%s", import_state->fileNameText));
			UnloadModel(*model);
			*model = LoadModel(model_name);
		}
		else
		{ 
			*message = 1;
			import_error_window(message);
		}
		import_state->SelectFilePressed = false;
	}
}
// INFO window properities
int infoWindow(int *status)
{
	GuiUnlock();
	*status = 1;
	Rectangle window = {(float) WIDTH/4, (float) HEIGHT/4, (float) WIDTH/2, (float) HEIGHT/2};
	DrawRectangle(0, 0, WIDTH, HEIGHT, Fade(RAYWHITE, 0.8f));
	int gui = GuiMessageBox(window, GuiIconText(ICON_FILETYPE_INFO, "INFO"), "Properieties", "Ok");
	
	if((gui == 0) || (gui == 1)) *status = 0;
	GuiLock();
	return gui;
}
// HELP window properities
int helpWindow(int *message, Font *font)
{
	GuiUnlock();
	Rectangle window = {(float) WIDTH/4, (float) HEIGHT/4, (float) WIDTH/2, (float) HEIGHT/2};
	DrawRectangle(0, 0, WIDTH, HEIGHT, Fade(RAYWHITE, 0.8f));
	int gui = GuiMessageBox(window, GuiIconText(ICON_HELP, "HELP"), "SPECTRON - Super Powerfull Engine Computing Tracing Rays Of Numerics", "Ok");

// Text section
	DrawTextEx(*font, "Short cuts keys:", (Vector2){WIDTH/4 + 10, HEIGHT/4 + 40}, 20, 0, BLACK);

	DrawTextEx(*font, TextFormat("\t\tNew Project"), (Vector2){WIDTH/4 + 10, HEIGHT/4 + 70}, 20, 0, BLACK);
	DrawTextEx(*font, "<N + Ctrl>", (Vector2){WIDTH/4 + 300, HEIGHT/4 + 70}, 20, 0, BLACK);

	DrawTextEx(*font, TextFormat("\t\tSave data"), (Vector2){WIDTH/4 + 10, HEIGHT/4 + 100}, 20, 0, BLACK);
	DrawText("<S + Ctrl>", WIDTH/4 + 300, HEIGHT/4 + 100, 20, BLACK);

	DrawTextEx(*font, TextFormat("\t\tInfo"), (Vector2){WIDTH/4 + 10, HEIGHT/4 + 130}, 20, 0, BLACK);
	DrawTextEx(*font, "<I + Ctrl>", (Vector2){WIDTH/4 + 300, HEIGHT/4 + 130}, 20, 0, BLACK);
	
	DrawTextEx(*font, TextFormat("\t\tExit program"), (Vector2){WIDTH/4 + 10, HEIGHT/4 + 160}, 20, 0, BLACK);
	DrawTextEx(*font, "<ESC>", (Vector2){WIDTH/4 + 320, HEIGHT/4 + 160}, 20, 0, BLACK);
	
	DrawTextEx(*font, TextFormat("\t\tToggle grid"), (Vector2){WIDTH/4 + 10, HEIGHT/4 + 190}, 20, 0, BLACK);
	DrawTextEx(*font, "<G + Ctrl>", (Vector2){WIDTH/4 + 300, HEIGHT/4 + 190}, 20, 0, BLACK);

	DrawTextEx(*font, TextFormat("\t\tShow help"), (Vector2){WIDTH/4 + 10, HEIGHT/4 + 220}, 20, 0, BLACK);
	DrawTextEx(*font, "<H + Ctrl>", (Vector2){WIDTH/4 + 300, HEIGHT/4 + 220}, 20, 0, BLACK);

	DrawTextEx(*font, TextFormat("\t\tImport model"), (Vector2){WIDTH/4 + 10, HEIGHT/4 + 250}, 20, 0, BLACK);
	DrawTextEx(*font, "<P + Ctrl>", (Vector2){WIDTH/4 + 300, HEIGHT/4 + 250}, 20, 0, BLACK);

	DrawTextEx(*font, TextFormat("\t\tToggle camera mode"), (Vector2){WIDTH/4 + 10, HEIGHT/4 + 280}, 20, 0, BLACK);
	DrawTextEx(*font, "<Y + Ctrl>", (Vector2){WIDTH/4 + 300, HEIGHT/4 + 280}, 20, 0, BLACK);

	if((gui == 0) || (gui == 1)) *message = 0;
	GuiLock();
	return gui;
}

// SAVE window properties
int saveWindow(bool *state, char *INPUT, Font *font)
{
	GuiUnlock();
	DrawRectangle(0, 0, WIDTH, HEIGHT, Fade(RAYWHITE, 0.8f));
	int gui = GuiTextInputBox((Rectangle){(float) WIDTH/2 - 60, (float) HEIGHT/2 - 60, 120.0, 120.0}, GuiIconText(ICON_FILE_SAVE, "Save data"), "File name:", "Save;Cancel", INPUT, 255, NULL);
	
	if(gui == 1)
	{
		DrawText(TextFormat("%s", INPUT), 200, 200, 30, GREEN);

		if(!strcmp("linux", PLATFORM))
		{
			//mkdir(INPUT, 0777);
			MKDIR(INPUT);
			char file_name[1024];
			sprintf(file_name, "%s/%s.txt", INPUT, INPUT);
			FILE *file = fopen(file_name, "w");
			fprintf(file, "%s has been saved correctly!", INPUT);
			fclose(file);
		}
		else if(!(strcmp("win32", PLATFORM) || strcmp("win64", PLATFORM)))
		{
			MKDIR(INPUT);
		}
		else exit(1);

		*state = false;
	}
	
	if((gui == 0) || (gui == 2))
	{
		DrawText("Save Terminated!", 200, 200, 30, RED);
		*state = false;
		//strcpy(INPUT, "\0"); 
	}

	GuiLock();
	return gui;
}

// File panel
void File(float *panel_width, float *btn_width, float *btn_height, int *pad_y, Model *model, Image *image, Texture2D *image2D, bool *status, int *info_status, char *INPUT, Font *font)
{
	
	int NewButton = GuiButton((Rectangle) {(float) WIDTH - (*panel_width + *btn_width)/2, *pad_y, *btn_width, *btn_height}, GuiIconText(ICON_FILE_NEW, "[N]ew Project"));
	
	int SaveButton = GuiButton((Rectangle) {(float) WIDTH - (*panel_width + *btn_width)/2, (*pad_y)*2, *btn_width, *btn_height}, GuiIconText(ICON_FILE_SAVE, "[S]ave Data"));
	
	if(SaveButton)
	{
		*status = true;
		saveWindow(status, INPUT, font);
	}

	int InfoButton = GuiButton((Rectangle) {(float) WIDTH - (*panel_width + *btn_width)/2, (*pad_y)*3, *btn_width, *btn_height}, GuiIconText(ICON_INFO, "[I]nfo"));

	if(InfoButton) infoWindow(info_status);

	int ExitButton = GuiButton((Rectangle) {(float) WIDTH - (*panel_width + *btn_width)/2, (*pad_y)*4, *btn_width, *btn_height}, GuiIconText(ICON_EXIT, "Exit [ESC]"));
	
	if(ExitButton)
	{
		UnloadImage(*image);
		UnloadTexture(*image2D);
		UnloadModel(*model);
		CloseWindow();
		exit(0);
	}

}
// Tools panel
int Radar_Group(float *x_radar, float *y_radar, float *z_radar, float *distance_radar, float *azymuth_radar, float *elevation_radar, int *radar_combo, float *lambda, float *freq, float *group_width, float *panel_width, float *slider_width)
{
	int CamSet = 0;
		// Radar Group
	GuiGroupBox((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/2, 50.0, *group_width, 260.0}, "Radar Settings");

	GuiLabel((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/2.5, 70.0, *group_width, 20.0}, "Radar Position");

	*radar_combo = GuiComboBox((Rectangle){(float) WIDTH - (*group_width + *panel_width)/4, 65.0, *group_width/2.5, 20.0}, "Cartesian;Polar", *radar_combo);
	
	if (*radar_combo == 0)
	{
			// coordinats 
		Cartesian2Polar(x_radar, y_radar, z_radar, distance_radar, azymuth_radar, elevation_radar);
	
		*x_radar = GuiSlider((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/2.4, 95.0, *slider_width, 20.0}, "X: ", TextFormat("%.2f m", *x_radar), *x_radar, -100, 100);
		
		*y_radar = GuiSlider((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/2.4, 135.0, *slider_width, 20.0}, "Y: ", TextFormat("%.2f m", *y_radar), *y_radar, -100, 100);
			
	 	*z_radar = GuiSlider((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/2.4, 175.0, *slider_width, 20.0}, "Z: ", TextFormat("%.2f m", *z_radar), *z_radar, -100, 100);
	
		GuiSetState(STATE_DISABLED);
		CamSet = GuiButton((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/6.6, 215.0f, 80.0f, 20.0f}, "CamSet");
		GuiSetState(STATE_NORMAL);

	}
	else if (*radar_combo == 1)
	{
			// coordinats 
		Polar2Cartesian(x_radar, y_radar, z_radar, distance_radar, azymuth_radar, elevation_radar);
		
		*distance_radar = GuiSlider((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/2.4, 95.0, *slider_width, 20.0}, "Distance", TextFormat("%.2f m", *distance_radar), *distance_radar, 0, 100);
		
		*azymuth_radar = GuiSlider((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/2.4, 135.0, *slider_width, 20.0}, "Azymuth", TextFormat("%.2f °", *azymuth_radar), *azymuth_radar, 0, 360);
			
		*elevation_radar = GuiSlider((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/2.4, 175.0, *slider_width, 20.0}, "Elevation", TextFormat("%.2f °", *elevation_radar), *elevation_radar, -90, 90);
		
		CamSet = GuiButton((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/6.6, 215.0f, 80.0f, 20.0f}, "CamSet");
		}


	GuiLabel((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/2.5, 215.0, *group_width, 20.0}, "Radar Params");
	
	*freq = GuiSlider((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/2.4, 240.0, *slider_width, 20.0}, "Frequency", TextFormat("%.2f", *freq), *freq, 1, 999.99);

	*lambda = GuiSlider((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/2.4, 280.0, *slider_width, 20.0}, "Lambda", TextFormat("%.2f", *lambda), *lambda, 1, 999.99);

	return CamSet;
}

int Camera_Group(float *x_camera, float *y_camera, float *z_camera, float *distance_camera, float *azymuth_camera, float *elevation_camera, int *camera_combo, float *group_width, float *panel_width, float *slider_width)
{
	int RadarSet = 0;
	// Camera Group	
	GuiGroupBox((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/2, 340.0, *group_width, 220.0}, "Camera Settings");

	GuiLabel((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/2.5, 360.0, *group_width, 20.0}, "Camera Position");
	
	*camera_combo = GuiComboBox((Rectangle){(float) WIDTH - (*group_width + *panel_width)/4, 355.0, *group_width/2.5, 20.0}, "Cartesian;Polar", *camera_combo);
	
	if(*camera_combo == 0)
	{
		Cartesian2Polar(x_camera, y_camera, z_camera, distance_camera, azymuth_camera, elevation_camera);

		*x_camera = GuiSlider((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/2.4, 400.0, *slider_width, 20.0}, "X: ", TextFormat("%.2f m", *x_camera), *x_camera, -100, 100);
		
		*y_camera = GuiSlider((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/2.4, 440.0, *slider_width, 20.0}, "Y: ", TextFormat("%.2f m", *y_camera), *y_camera, -100, 100);
			
		*z_camera = GuiSlider((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/2.4, 480.0, *slider_width, 20.0}, "Z: ", TextFormat("%.2f m", *z_camera), *z_camera, -100, 100);
	
		GuiSetState(STATE_DISABLED);
		RadarSet = GuiButton((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/6.6, 520.0f, 80.0f, 20.0f}, "RadarSet");
		GuiSetState(STATE_NORMAL);
	}
	else if(*camera_combo == 1)
	{
		Polar2Cartesian(x_camera, y_camera, z_camera, distance_camera, azymuth_camera, elevation_camera);

		*distance_camera = GuiSlider((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/2.4, 400.0, *slider_width, 20.0}, "Distance", TextFormat("%.2f m", *distance_camera), *distance_camera, 0, 100);
		
		*azymuth_camera = GuiSlider((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/2.4, 440.0, *slider_width, 20.0}, "Azymuth", TextFormat("%.2f °", *azymuth_camera), *azymuth_camera, 0, 360);
			
		*elevation_camera = GuiSlider((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/2.4, 480.0, *slider_width, 20.0}, "Elevation", TextFormat("%.2f °", *elevation_camera), *elevation_camera, -90, 90);
	
		RadarSet = GuiButton((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/6.6, 520.0f, 80.0f, 20.0f}, "RadarSet");
	}


	return RadarSet;
}

void Object_Group(bool *import_btn, int *material_combo, float *group_width, float *panel_width, float *slider_width, char *name, float *model_scale)
{
			// Object Group
	GuiGroupBox((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/2, 580.0, *group_width, 140.0}, "Object Settings");
		
	GuiLabel((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/2.2, 600.0, *group_width/2, 20.0}, name);

	*import_btn = GuiButton((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/3.5, 600.0, *group_width/2, 20.0}, "Import");
	
	GuiLabel((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/2.5, 640.0, *group_width, 20.0}, "Material");

	*material_combo = GuiComboBox((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/3.5, 640.0, *group_width/2, 20.0}, "PEC;Al;Cu", *material_combo);

	*model_scale = GuiSlider((Rectangle) {(float) WIDTH - (*group_width + *panel_width)/2.4, 680.0, *slider_width, 20.0}, "Scale", TextFormat("%.2f", *model_scale), *model_scale, 0.1, 1);
}

