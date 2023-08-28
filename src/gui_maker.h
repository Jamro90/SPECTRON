#ifndef GUI_MAKER
#define GUI_MAKER

#include "gui_maker.h"
#include "raylib/src/raylib.h"

#define RAYGUI_GUI_FILE_DIALOG_IMPLEMENTATION
#include "raygui/gui_file_dialog.h"
#include <stdbool.h>

// structures for simple code syntax

// signals for windows
typedef struct{
	int message_status;
	int new_status;
	int info_status;
	int import_message;
	bool save;
}Signals;

// radar properities
typedef struct{
	float x;
	float y;
	float z;
	float distance;
	float azymuth;
	float elevation;
	int combo;
	float lambda;
	float freq;

}Radar;

// camera view properities
typedef struct{
	float x;
	float y;
	float z;
	float distance;
	float azymuth;
	float elevation;
	int combo;
}Cam;

typedef struct{
	float panel_width;
	float btn_width;
	float btn_height;
	int pad_y; 
	float group_width;
	float slider_width;
}Geometry;

// functions 

int import_error_window(int *status);

void importWindow(GuiFileDialogState *import_state, Model *model, char *model_name, char *name, int *message);

int newForSave(int *new_status, bool *status, char *INPUT, Font *font);
	
int infoWindow(int *info_status);
	
int helpWindow(int *message, Font *font);

int saveWindow(bool *state, char *INPUT, Font *font);

void File(Geometry *geometry, Model *model, Image *image, Texture2D *image2D, Signals *sig, char *INPUT, Font *font);

int Radar_Group(Radar *radar, Geometry *geometry);

int Camera_Group(Cam *cam, Geometry *geometry);

void Object_Group(bool *import_btn, int *material_combo, Geometry *geometry, char *name, float *model_scale);

#endif

