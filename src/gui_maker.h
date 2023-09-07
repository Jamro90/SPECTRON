#ifndef GUI_MAKER
#define GUI_MAKER

#include "gui_maker.h"
#include "raylib_binaries/raylib/src/raylib.h"

#define RAYGUI_GUI_FILE_DIALOG_IMPLEMENTATION
#include "raylib_binaries/raygui/gui_file_dialog.h"
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
	char lambda_fix[8];
	float freq;
	int freq_fix;

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

typedef struct{
	bool gizmo;
	bool box;
	bool model;
	bool radar;
	bool grid;
	bool plot;
}VIS;

typedef struct{
	float x;
	float y;
	float z;
	float cylinder_d;
	float cylinder_h;
	float cone_d;
	float cone_h;
	int segments;
	Vector3 start;
	Vector3 endRed;
	Vector3 endGreen;
	Vector3 endBlue;
}Gizmo;
// functions 

int import_error_window(int *status);

void importWindow(GuiFileDialogState *import_state, Model *model, char *model_name, char *name, int *message);

int newForSave(int *new_status, bool *status, char *INPUT);
	
int infoWindow(int *info_status);
	
int helpWindow(int *message, Font *font);

int saveWindow(bool *state, char *INPUT);

void File(Geometry *geometry, Model *model, Image *image, Texture2D *image2D, Signals *sig, char *INPUT, Font *font);

int Radar_Group(Radar *radar, Geometry *geometry);

int Camera_Group(Cam *cam, Geometry *geometry);

void Object_Group(bool *import_btn, int *material_combo, Geometry *geometry, char *name, float *model_scale);

#endif

