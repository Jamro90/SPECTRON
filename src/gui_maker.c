#include "gui_maker.h"
#include "chart_maker.h"
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include "raylib_binaries/raylib/src/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raylib_binaries/raygui/src/raygui.h"
#undef RAYGUI_IMPLEMENTATION
#define GUI_FILE_DIALOG_IMPLEMENTATION
#include "raylib_binaries/raygui/gui_file_dialog.h"

#ifdef _WIN32
	#include <direct.h>
	#define PLATFORM "win32"
	#define MKDIR(x) mkdir(x)
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
	DrawRectangle(0, 0, WIDTH, HEIGHT, Fade(RAYWHITE, 0.8f));
	Rectangle window = {(float) WIDTH/8, (float) HEIGHT/8, (float) WIDTH/4, (float) HEIGHT/4};
	int gui = GuiMessageBox(window, GuiIconText(ICON_GEAR_EX, "Invalid model extension!"), TextFormat("%s", "Supported extension: .obj"), "Ok");

	if( (gui == 0) || (gui == 1) ) *status = 0;
	GuiLock();
	return gui;
}

void importWindow(GuiFileDialogState *import_state, Model *model, char *model_name, char *name, int *message)
{
	GuiUnlock();
	if(import_state->SelectFilePressed)
	{
		if(  (IsFileExtension(import_state->fileNameText, ".obj")) )
		{
			strcpy(model_name, TextFormat("%s/%s", import_state->dirPathText, import_state->fileNameText));
			strcpy(name, TextFormat("%s", import_state->fileNameText));
			UnloadModel(*model);
			*model = LoadModel(model_name);
			GuiLock();
		}
		else
		{ 
			*message = 1;
			import_error_window(message);
		}
		import_state->SelectFilePressed = false;
		GuiUnlock();
	}
}

// window for asking to save unsaved data
int newForSave(int *new_status, bool *status, char *INPUT, DATA *data)
{	
	GuiUnlock();
	*new_status = 1;
	Rectangle window = {(float) WIDTH/4, (float) HEIGHT/4, (float) WIDTH/2, (float) HEIGHT/2};
	DrawRectangle(0, 0, WIDTH, HEIGHT, Fade(RAYWHITE, 0.8f));
	int gui = GuiMessageBox(window, GuiIconText(ICON_WAVE, ""), "Do you want to save?", "Yes;No");
		
	if(gui == 1)
	{
		*status = true;
		*new_status = 0;
		saveWindow(status, INPUT, data);
	}
	else if(gui == 0 || gui == 2) *new_status = 0;
	else *new_status = 1;

	GuiLock();

	return gui;
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
	int gui = GuiMessageBox(window, GuiIconText(ICON_HELP, "HELP"), NULL, "Ok");

// Text section
	DrawTextEx(*font, "Short cuts keys:", (Vector2){WIDTH/4 + 10, HEIGHT/4 + 40}, 20, 0, BLACK);

	DrawTextEx(*font, TextFormat("\t\tNew Project"), (Vector2){WIDTH/4 + 10, HEIGHT/4 + 70}, 20, 0, BLACK);
	DrawTextEx(*font, "<Ctrl + N>", (Vector2){WIDTH/4 + 300, HEIGHT/4 + 70}, 20, 0, BLACK);

	DrawTextEx(*font, TextFormat("\t\tSave data"), (Vector2){WIDTH/4 + 10, HEIGHT/4 + 100}, 20, 0, BLACK);
	DrawTextEx(*font, "<Ctrl + S>", (Vector2){WIDTH/4 + 300, HEIGHT/4 + 100}, 20, 0, BLACK);

	DrawTextEx(*font, TextFormat("\t\tInfo"), (Vector2){WIDTH/4 + 10, HEIGHT/4 + 130}, 20, 0, BLACK);
	DrawTextEx(*font, "<Ctrl + I>", (Vector2){WIDTH/4 + 300, HEIGHT/4 + 130}, 20, 0, BLACK);
	
	DrawTextEx(*font, TextFormat("\t\tExit program"), (Vector2){WIDTH/4 + 10, HEIGHT/4 + 160}, 20, 0, BLACK);
	DrawTextEx(*font, "<ESC>", (Vector2){WIDTH/4 + 320, HEIGHT/4 + 160}, 20, 0, BLACK);
	
	DrawTextEx(*font, TextFormat("\t\tToggle grid"), (Vector2){WIDTH/4 + 10, HEIGHT/4 + 190}, 20, 0, BLACK);
	DrawTextEx(*font, "<Ctrl + G>", (Vector2){WIDTH/4 + 300, HEIGHT/4 + 190}, 20, 0, BLACK);
	DrawTextEx(*font, TextFormat("\t\tShow help"), (Vector2){WIDTH/4 + 10, HEIGHT/4 + 220}, 20, 0, BLACK);
	DrawTextEx(*font, "<Ctrl + H>", (Vector2){WIDTH/4 + 300, HEIGHT/4 + 220}, 20, 0, BLACK);

	DrawTextEx(*font, TextFormat("\t\tImport model"), (Vector2){WIDTH/4 + 10, HEIGHT/4 + 250}, 20, 0, BLACK);
	DrawTextEx(*font, "<Ctrl + P>", (Vector2){WIDTH/4 + 300, HEIGHT/4 + 250}, 20, 0, BLACK);

	DrawTextEx(*font, TextFormat("\t\tToggle camera mode"), (Vector2){WIDTH/4 + 10, HEIGHT/4 + 280}, 20, 0, BLACK);
	DrawTextEx(*font, "<Ctrl + Y>", (Vector2){WIDTH/4 + 300, HEIGHT/4 + 280}, 20, 0, BLACK);

	if((gui == 0) || (gui == 1)) *message = 0;
	GuiLock();
	return gui;
}

// SAVE window properties
int saveWindow(bool *state, char *INPUT, DATA *data)
{
	GuiUnlock();
	DrawRectangle(0, 0, WIDTH, HEIGHT, Fade(RAYWHITE, 0.8f));
	int gui = GuiTextInputBox((Rectangle){(float) WIDTH/2 - 60, (float) HEIGHT/2 - 60, 120.0, 120.0}, GuiIconText(ICON_FILE_SAVE, "Save data"), "File name:", "Save;Cancel", INPUT, 255, NULL);
	
	if(gui == 1)
	{
		if(!strcmp("linux", PLATFORM))
		{
			//mkdir(INPUT, 0777);
			MKDIR(INPUT);

			char file_name[1024];
			snprintf(file_name, 1024, "%s/%s.txt", INPUT, INPUT);
			FILE *file = fopen(file_name, "w");
			for(size_t i = 0; i < sizeof(data->x)/sizeof(data->x[0]); ++i)
			{
				fprintf(file, "%lf\t%lf\n", data->x[i], data->y[i]);
			}
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
		*state = false;
	}

	GuiLock();
	return gui;
}

// File panel
void File(Geometry *geometry, Model *model, Image *image, Texture2D *image2D, Signals *sig, char *INPUT, DATA *data, Font *font)
{
	
	int NewButton = GuiButton((Rectangle) {(float) WIDTH - (geometry->panel_width + geometry->btn_width)/2, geometry->pad_y, geometry->btn_width, geometry->btn_height}, GuiIconText(ICON_FILE_NEW, "[N]ew Project"));
	
	switch(NewButton)
	{
		// with saved prompt
		case 1:
			newForSave(&sig->new_status, &sig->save, INPUT, data);
		// just make new	
		case 2:
			UnloadModel(*model);
			UnloadTexture(*image2D);
			UnloadImage(*image);
			break;
	}

	int SaveButton = GuiButton((Rectangle) {(float) WIDTH - (geometry->panel_width + geometry->btn_width)/2, (geometry->pad_y)*2, geometry->btn_width, geometry->btn_height}, GuiIconText(ICON_FILE_SAVE, "[S]ave Data"));
	
	if(SaveButton)
	{
		sig->save = true;
		saveWindow(&sig->save, INPUT, data);
	}

	int InfoButton = GuiButton((Rectangle) {(float) WIDTH - (geometry->panel_width + geometry->btn_width)/2, (geometry->pad_y)*3, geometry->btn_width, geometry->btn_height}, GuiIconText(ICON_INFO, "[I]nfo"));

	if(InfoButton) infoWindow(&sig->info_status);

	int ExitButton = GuiButton((Rectangle) {(float) WIDTH - (geometry->panel_width + geometry->btn_width)/2, (geometry->pad_y)*4, geometry->btn_width, geometry->btn_height}, GuiIconText(ICON_EXIT, "Exit [ESC]"));
	
	if(ExitButton)
	{
		UnloadImage(*image);
		UnloadTexture(*image2D);
		UnloadModel(*model);
		UnloadFont(*font);
		CloseWindow();
		exit(0);
	}

}
// Tools panel
int Radar_Group(Radar *radar, Geometry *geometry)
{
	int CamSet = 0;
		// Radar Group
	GuiGroupBox((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/2, 50.0, geometry->group_width, 270.0}, "Radar Settings");

	GuiLabel((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/2.5, 70.0, geometry->group_width, 20.0}, "Radar Position");

	radar->combo = GuiComboBox((Rectangle){(float) WIDTH - (geometry->group_width + geometry->panel_width)/4, 65.0, geometry->group_width/2.5, 20.0}, "Cartesian;Polar", radar->combo);
	
	if (radar->combo == 0)
	{
			// coordinats 
	
		radar->x = GuiSlider((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/2.4, 95.0, geometry->slider_width, 20.0}, "X: ", TextFormat("%.2f m", radar->x), radar->x, -1e3, 1e3);
		
		radar->y = GuiSlider((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/2.4, 135.0, geometry->slider_width, 20.0}, "Y: ", TextFormat("%.2f m", radar->y), radar->y, -1e3, 1e3);
			
	 	radar->z = GuiSlider((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/2.4, 175.0, geometry->slider_width, 20.0}, "Z: ", TextFormat("%.2f m", radar->z), radar->z, -1e3, 1e3);
	
		GuiSetState(STATE_DISABLED);
		CamSet = GuiButton((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/6.6, 215.0f, 80.0f, 20.0f}, "CamSet");
		GuiSetState(STATE_NORMAL);

	}
	else if (radar->combo == 1)
	{
			// coordinats 
		
		radar->distance = GuiSlider((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/2.4, 95.0, geometry->slider_width, 20.0}, "Distance", TextFormat("%.2f m", radar->distance), radar->distance, 0, 1732.05);
		
		radar->azymuth = GuiSlider((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/2.4, 135.0, geometry->slider_width, 20.0}, "Azymuth", TextFormat("%.2f %s", radar->azymuth, "\u00B0"), radar->azymuth, 0, 360);
			
		radar->elevation = GuiSlider((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/2.4, 175.0, geometry->slider_width, 20.0}, "Elevation", TextFormat("%.2f %s", radar->elevation, "\u00B0"), radar->elevation, -90, 90);
		
		CamSet = GuiButton((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/6.6, 215.0f, 80.0f, 20.0f}, "CamSet");
		}


	GuiLabel((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/2.5, 235.0, geometry->group_width, 20.0}, "Radar Params");
	
	radar->freq = GuiSlider((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/2.4, 260.0, geometry->slider_width, 20.0}, "Frequency", TextFormat("%.2f", radar->freq), radar->freq, 1, 999.99);

	radar->freq_fix = GuiComboBox((Rectangle){(float) WIDTH - (geometry->group_width + geometry->panel_width)/6.5, 260.0, geometry->group_width/4.5, 20.0}, "MHz;GHz;THz", radar->freq_fix);

	char lambda_text[128];
	snprintf(lambda_text, sizeof(float), "%f", radar->lambda);
	
	GuiLabel((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/2 + 10, 290.0, geometry->group_width, 20.0}, "Wave length");

	GuiLabel((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/5, 290.0, geometry->group_width, 20.0}, lambda_text);
	
	GuiLabel((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/6.5, 290.0, geometry->group_width, 20.0}, radar->lambda_fix);

	return CamSet;
}

int Camera_Group(Cam *cam, Geometry *geometry)
{
	int RadarSet = 0;
	// Camera Group	
	GuiGroupBox((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/2, 340.0, geometry->group_width, 220.0}, "Camera Settings");

	GuiLabel((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/2.5, 360.0, geometry->group_width, 20.0}, "Camera Position");
	
	cam->combo = GuiComboBox((Rectangle){(float) WIDTH - (geometry->group_width + geometry->panel_width)/4, 355.0, geometry->group_width/2.5, 20.0}, "Cartesian;Polar", cam->combo);
	
	if(cam->combo == 0)
	{
		cam->x = GuiSlider((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/2.4, 400.0, geometry->slider_width, 20.0}, "X: ", TextFormat("%.2f m", cam->x), cam->x, -100, 100);
		
		cam->y = GuiSlider((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/2.4, 440.0, geometry->slider_width, 20.0}, "Y: ", TextFormat("%.2f m", cam->y), cam->y, -100, 100);
			
		cam->z = GuiSlider((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/2.4, 480.0, geometry->slider_width, 20.0}, "Z: ", TextFormat("%.2f m", cam->z), cam->z, -100, 100);
	
		GuiSetState(STATE_DISABLED);
		RadarSet = GuiButton((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/6.6, 520.0f, 80.0f, 20.0f}, "RadarSet");
		GuiSetState(STATE_NORMAL);
	}
	else if(cam->combo == 1)
	{
		cam->distance = GuiSlider((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/2.4, 400.0, geometry->slider_width, 20.0}, "Distance", TextFormat("%.2f m", cam->distance), cam->distance, 0, 173.21);
		
		cam->azymuth = GuiSlider((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/2.4, 440.0, geometry->slider_width, 20.0}, "Azymuth", TextFormat("%.2f %s", cam->azymuth, "\u00B0"), cam->azymuth, 0, 359.99);
			
		cam->elevation = GuiSlider((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/2.4, 480.0, geometry->slider_width, 20.0}, "Elevation", TextFormat("%.2f %s", cam->elevation, "\u00B0"), cam->elevation, -90, 90);
	
		RadarSet = GuiButton((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/6.6, 520.0f, 80.0f, 20.0f}, "RadarSet");
	}


	return RadarSet;
}

void Object_Group(bool *import_btn, int *material_combo, Geometry *geometry, char *name, float *model_scale)
{
			// Object Group
	GuiGroupBox((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/2, 580.0, geometry->group_width, 140.0}, "Object Settings");
		
	GuiLabel((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/2.2, 600.0, geometry->group_width/2, 20.0}, name);

	*import_btn = GuiButton((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/3.5, 600.0, geometry->group_width/2, 20.0}, "Import");
	
	GuiLabel((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/2.5, 640.0, geometry->group_width, 20.0}, "Material");

	*material_combo = GuiComboBox((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/3.5, 640.0, geometry->group_width/2, 20.0}, "PEC;Al;Cu", *material_combo);

	*model_scale = GuiSlider((Rectangle) {(float) WIDTH - (geometry->group_width + geometry->panel_width)/2.4, 680.0, geometry->slider_width, 20.0}, "Scale", TextFormat("%.2f", *model_scale), *model_scale, 0.1, 1);
}

