#define RAYGUI_IMPLEMENTATION
#include "raygui/src/raygui.h"
#undef RAYGUI_IMPLEMENTATION
#define GUI_FILE_DIALOG_IMPLEMENTATION
#include "raygui/gui_file_dialog.h"

void importWindow(GuiFileDialogState *import_state, Model *model, char *model_name, char *name)
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
		import_state->SelectFilePressed = false;
	}
}

// HELP window properities
int helpWindow(int *message)
{
	Rectangle window = {(float) GetScreenWidth()/4, (float) GetScreenHeight()/4, (float) GetScreenWidth()/2, (float) GetScreenHeight()/2};
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.8f));
	int gui = GuiMessageBox(window, GuiIconText(ICON_FILETYPE_INFO, "HELP"), "SPECTRON - Super Powerfull Engine Computing Tracing Rays Of Numerics", "Ok");

	// Text section
	DrawText("Short cuts keys:", GetScreenWidth()/4 + 10, GetScreenHeight()/4 + 40, 20, BLACK);

	DrawText(TextFormat("\t\tNew Project"), GetScreenWidth()/4 + 10, GetScreenHeight()/4 + 70, 20, BLACK);
	DrawText("<N + Ctrl>", GetScreenWidth()/4 + 300, GetScreenHeight()/4 + 70, 20, BLACK);

	DrawText(TextFormat("\t\tSave data"), GetScreenWidth()/4 + 10, GetScreenHeight()/4 + 100, 20, BLACK);
	DrawText("<S + Ctrl>", GetScreenWidth()/4 + 300, GetScreenHeight()/4 + 100, 20, BLACK);

	DrawText(TextFormat("\t\tInfo"), GetScreenWidth()/4 + 10, GetScreenHeight()/4 + 130, 20, BLACK);
	DrawText("<I + Ctrl>", GetScreenWidth()/4 + 300, GetScreenHeight()/4 + 130, 20, BLACK);
	
	DrawText(TextFormat("\t\tExit program"), GetScreenWidth()/4 + 10, GetScreenHeight()/4 + 160, 20, BLACK);
	DrawText("<ESC>", GetScreenWidth()/4 + 320, GetScreenHeight()/4 + 160, 20, BLACK);
	
	DrawText(TextFormat("\t\tToggle grid"), GetScreenWidth()/4 + 10, GetScreenHeight()/4 + 190, 20, BLACK);
	DrawText("<G + Ctrl>", GetScreenWidth()/4 + 300, GetScreenHeight()/4 + 190, 20, BLACK);

	DrawText(TextFormat("\t\tShow help"), GetScreenWidth()/4 + 10, GetScreenHeight()/4 + 220, 20, BLACK);
	DrawText("<H + Ctrl>", GetScreenWidth()/4 + 300, GetScreenHeight()/4 + 220, 20, BLACK);

	DrawText(TextFormat("\t\tImport model"), GetScreenWidth()/4 + 10, GetScreenHeight()/4 + 250, 20, BLACK);
	DrawText("<P + Ctrl>", GetScreenWidth()/4 + 300, GetScreenHeight()/4 + 250, 20, BLACK);

	DrawText(TextFormat("\t\tToggle camera mode"), GetScreenWidth()/4 + 10, GetScreenHeight()/4 + 280, 20, BLACK);
	DrawText("<Y + Ctrl>", GetScreenWidth()/4 + 300, GetScreenHeight()/4 + 280, 20, BLACK);

	if((gui == 0) || (gui == 1)) *message = 0;

	return gui;
}

// SAVE window properties
int saveWindow(int *state, char *INPUT)
{
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.8f));
	int gui = GuiTextInputBox((Rectangle){(float) GetScreenWidth()/2 - 60, (float) GetScreenHeight()/2 - 60, 120.0, 120.0}, GuiIconText(ICON_FILE_SAVE, "Save data"), "File name:", "Save;Cancel", INPUT, 255, NULL);
	
	if(gui == 1)
	{
		DrawText(TextFormat("%s", INPUT), 200, 200, 30, GREEN);
//		mkdir(INPUT, 0777);
		*state = false;
	}
	
	if((gui == 0) || (gui == 2))
	{
		DrawText("Save Terminated!", 200, 200, 30, RED);
		*state = false;
		//*INPUT = "\0";
	}

	return gui;
}

// File panel
void File(float *panel_width, float *btn_width, float *btn_height, int *pad_y, Model *model)
{
	int NewButon = GuiButton((Rectangle) {(float) GetScreenWidth() - (*panel_width + *btn_width)/2, *pad_y, *btn_width, *btn_height}, GuiIconText(ICON_FILE_NEW, "[N]ew Project"));
	
	int SaveButton = GuiButton((Rectangle) {(float) GetScreenWidth() - (*panel_width + *btn_width)/2, (*pad_y)*2, *btn_width, *btn_height}, GuiIconText(ICON_FILE_SAVE, "[S]ave Data"));
	
	int InfoButton = GuiButton((Rectangle) {(float) GetScreenWidth() - (*panel_width + *btn_width)/2, (*pad_y)*3, *btn_width, *btn_height}, GuiIconText(ICON_INFO, "[I]nfo"));

	int ExitButton = GuiButton((Rectangle) {(float) GetScreenWidth() - (*panel_width + *btn_width)/2, (*pad_y)*4, *btn_width, *btn_height}, GuiIconText(ICON_EXIT, "Exit [ESC]"));
	
	if(ExitButton)
	{
		UnloadModel(*model);
		CloseWindow();
	}

}
// Tools panel
void Radar_Group(float *x_radar, float *y_radar, float *z_radar, float *distance_radar, float *azymuth_radar, float *elevation_radar, int *radar_combo, float *lambda, float *freq, float *group_width, float *panel_width, float *slider_width)
{
		// Radar Group
	GuiGroupBox((Rectangle) {(float) GetScreenWidth() - (*group_width + *panel_width)/2, 50.0, *group_width, 260.0}, "Radar Settings");

	GuiLabel((Rectangle) {(float) GetScreenWidth() - (*group_width + *panel_width)/2.5, 70.0, *group_width, 20.0}, "Radar Position");

	*radar_combo = GuiComboBox((Rectangle){(float) GetScreenWidth() - (*group_width + *panel_width)/4, 65.0, *group_width/2.5, 20.0}, "Cartesian;Polar", *radar_combo);
	
	if (*radar_combo == 0)
	{
		Cartesian2Polar(x_radar, y_radar, z_radar, distance_radar, azymuth_radar, elevation_radar);
		
		*x_radar = GuiSlider((Rectangle) {(float) GetScreenWidth() - (*group_width + *panel_width)/2.4, 95.0, *slider_width, 20.0}, "X: ", TextFormat("%.2f", *x_radar), *x_radar, -100, 100);
		
		*y_radar = GuiSlider((Rectangle) {(float) GetScreenWidth() - (*group_width + *panel_width)/2.4, 135.0, *slider_width, 20.0}, "Y: ", TextFormat("%.2f", *y_radar), *y_radar, -100, 100);
			
		*z_radar = GuiSlider((Rectangle) {(float) GetScreenWidth() - (*group_width + *panel_width)/2.4, 175.0, *slider_width, 20.0}, "Z: ", TextFormat("%.2f", *z_radar), *z_radar, -100, 100);
	
	}
	else if (*radar_combo == 1)
	{
		Polar2Cartesian(x_radar, y_radar, z_radar, distance_radar, azymuth_radar, elevation_radar);
		
		*distance_radar = GuiSlider((Rectangle) {(float) GetScreenWidth() - (*group_width + *panel_width)/2.4, 95.0, *slider_width, 20.0}, "Distance", TextFormat("%.2f", *distance_radar), *distance_radar, 0, 100);
		
		*azymuth_radar = GuiSlider((Rectangle) {(float) GetScreenWidth() - (*group_width + *panel_width)/2.4, 135.0, *slider_width, 20.0}, "Azymuth", TextFormat("%.2f", *azymuth_radar), *azymuth_radar, 0, 360);
			
		*elevation_radar = GuiSlider((Rectangle) {(float) GetScreenWidth() - (*group_width + *panel_width)/2.4, 175.0, *slider_width, 20.0}, "Elevation", TextFormat("%.2f", *elevation_radar), *elevation_radar, -90, 90);
	}

	GuiLabel((Rectangle) {(float) GetScreenWidth() - (*group_width + *panel_width)/2.5, 215.0, *group_width, 20.0}, "Radar Params");
	
	*freq = GuiSlider((Rectangle) {(float) GetScreenWidth() - (*group_width + *panel_width)/2.4, 240.0, *slider_width, 20.0}, "Frequency", TextFormat("%.2f", *freq), *freq, 1, 999.99);

	*lambda = GuiSlider((Rectangle) {(float) GetScreenWidth() - (*group_width + *panel_width)/2.4, 280.0, *slider_width, 20.0}, "Lambda", TextFormat("%.2f", *lambda), *lambda, 1, 999.99);
}

void Camera_Group(float *x_camera, float *y_camera, float *z_camera, float *distance_camera, float *azymuth_camera, float *elevation_camera, int *camera_combo, float *group_width, float *panel_width, float *slider_width)
{
	// Camera Group	
	GuiGroupBox((Rectangle) {(float) GetScreenWidth() - (*group_width + *panel_width)/2, 340.0, *group_width, 180.0}, "Camera Settings");

	GuiLabel((Rectangle) {(float) GetScreenWidth() - (*group_width + *panel_width)/2.5, 360.0, *group_width, 20.0}, "Camera Position");
	
	*camera_combo = GuiComboBox((Rectangle){(float) GetScreenWidth() - (*group_width + *panel_width)/4, 355.0, *group_width/2.5, 20.0}, "Cartesian;Polar", *camera_combo);
	
	if(*camera_combo == 0)
	{
		Cartesian2Polar(x_camera, y_camera, z_camera, distance_camera, azymuth_camera, elevation_camera);

		*x_camera = GuiSlider((Rectangle) {(float) GetScreenWidth() - (*group_width + *panel_width)/2.4, 400.0, *slider_width, 20.0}, "X: ", TextFormat("%.2f", *x_camera), *x_camera, -100, 100);
		
		*y_camera = GuiSlider((Rectangle) {(float) GetScreenWidth() - (*group_width + *panel_width)/2.4, 440.0, *slider_width, 20.0}, "Y: ", TextFormat("%.2f", *y_camera), *y_camera, -100, 100);
			
		*z_camera = GuiSlider((Rectangle) {(float) GetScreenWidth() - (*group_width + *panel_width)/2.4, 480.0, *slider_width, 20.0}, "Z: ", TextFormat("%.2f", *z_camera), *z_camera, -100, 100);
	}
	else if(*camera_combo == 1)
	{
		Polar2Cartesian(x_camera, y_camera, z_camera, distance_camera, azymuth_camera, elevation_camera);

		*distance_camera = GuiSlider((Rectangle) {(float) GetScreenWidth() - (*group_width + *panel_width)/2.4, 400.0, *slider_width, 20.0}, "Distance", TextFormat("%.2f", *distance_camera), *distance_camera, -100, 100);
		
		*azymuth_camera = GuiSlider((Rectangle) {(float) GetScreenWidth() - (*group_width + *panel_width)/2.4, 440.0, *slider_width, 20.0}, "Azymuth", TextFormat("%.2f", *azymuth_camera), *azymuth_camera, 0, 360);
			
		*elevation_camera = GuiSlider((Rectangle) {(float) GetScreenWidth() - (*group_width + *panel_width)/2.4, 480.0, *slider_width, 20.0}, "Elevation", TextFormat("%.2f", *elevation_camera), *elevation_camera, -90, 90);
	}
}

void Object_Group(bool *import_btn, int *material_combo, float *group_width, float *panel_width, char *name)
{
			// Object Group
	GuiGroupBox((Rectangle) {(float) GetScreenWidth() - (*group_width + *panel_width)/2, 540.0, *group_width, 100.0}, "Object Settings");
		
	GuiLabel((Rectangle) {(float) GetScreenWidth() - (*group_width + *panel_width)/2.2, 560.0, *group_width/2, 20.0}, name);

	*import_btn = GuiButton((Rectangle) {(float) GetScreenWidth() - (*group_width + *panel_width)/3.5, 560.0, *group_width/2, 20.0}, "Import");
	
	GuiLabel((Rectangle) {(float) GetScreenWidth() - (*group_width + *panel_width)/2.5, 600.0, *group_width, 20.0}, "Material");

	*material_combo = GuiComboBox((Rectangle) {(float) GetScreenWidth() - (*group_width + *panel_width)/3.5, 600.0, *group_width/2, 20.0}, "PEC;Al;Cu", *material_combo);

}

