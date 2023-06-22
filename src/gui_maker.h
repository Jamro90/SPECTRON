#ifndef GUI_MAKER
#define GUI_MAKER

#include "gui_maker.h"
#include <raylib.h>
//#define RAYGUI_IMPLEMENETATION
//#include "raygui/src/raygui.h"
//#undef RAYGUI_IMPLEMENTATION
#define RAYGUI_GUI_FILE_DIALOG_IMPLEMENTATION
#include "raygui/gui_file_dialog.h"
#include <stdbool.h>

int import_error_window(int *status);

void importWindow(GuiFileDialogState *import_state, Model *model, char *model_name, char *name, int *message);

int helpWindow(int *message);

int saveWindow(int *state, char *INPUT);

void File(float *panel_width, float *btn_width, float *btn_height, int *pad_y, Model *model, Image *image, Texture2D *image2D);

int Radar_Group(float *x_radar, float *y_radar, float *z_radar, float *distance_radar, float *azymuth_radar, float *elevation_radar, int *radar_combo, float *lambda, float *freq, float *group_width, float *panel_width, float *slider_width);

int Camera_Group(float *x_camera, float *y_camera, float *z_camera, float *distance_camera, float *azymuth_camera, float *elevation_camera, int *camera_combo, float *group_width, float *panel_width, float *slider_width);

void Object_Group(bool *import_btn, int *material_combo, float *group_width, float *panel_width, float *slider_width, char *name, float *model_scale);

#endif
