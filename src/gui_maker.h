#ifndef GUI_MAKER
#define GUI_MAKER

#include "gui_maker.h"
#include "raylib_binaries/raylib/src/raylib.h"

#define RAYGUI_GUI_FILE_DIALOG_IMPLEMENTATION
#include "raylib_binaries/raygui/gui_file_dialog.h"
#include <stdbool.h>
#include "structs.h"
#include "chart_maker.h"
// functions 

// multi-threading 
void *sig_signals(void *arg);
void *vis_signals(void *arg);

// ordinary programing
int import_error_window(int *status);

void importWindow(GuiFileDialogState *import_state, Model *model, char *model_name, char *name, int *message);

int newForSave(int *new_status, bool *status, char *INPUT, DATA *data, Model *model);
	
int infoWindow(int *info_status);
	
int helpWindow(int *message); 

int saveWindow(bool *state, char *INPUT, DATA *data);

void File(Geometry *geometry, Model *model, Signals *sig, char *INPUT, DATA *data);

int Radar_Group(Radar *radar, Geometry *geometry);

int Camera_Group(Cam *cam, Geometry *geometry);

void Object_Group(bool *import_btn, int *material_combo, Geometry *geometry, char *name, float *model_scale);

void Control_Group(bool *sim_btn, Geometry *geometry);

int progressWindow(bool *status, float *progress);

int errorProgressWindow(bool *status, bool *signal);

#endif

