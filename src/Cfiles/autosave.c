#include "3d_viewer.h"

void set_default_settings(settings_data* data) {
  strcpy(data->projection_type, "parallel");
  strcpy(data->line_type, "solid");
  data->line_width = 1;
  strcpy(data->line_color, "#FFFFFF");
  strcpy(data->vertexes_paint_type, "round");
  data->vertexes_size = 1;
  strcpy(data->vertexes_color, "#FFFFFF");
  strcpy(data->background_color, "#000000");
}

void init_settings(settings_data* data) {
  set_default_settings(data);
  FILE* file = fopen("settings.cfg", "r");
  if (file) {
    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL) {
      if (sscanf(line, "Projection_type: %9s", data->projection_type) == 1) {
        if (strcmp(data->projection_type, "parallel") != 0 &&
            strcmp(data->projection_type, "central") != 0) {
          strcpy(data->projection_type, "parallel");
        }
      } else if (sscanf(line, "Line_type: %9s", data->line_type) == 1) {
        if (strcmp(data->line_type, "solid") != 0 &&
            strcmp(data->line_type, "dotted") != 0) {
          strcpy(data->line_type, "solid");
        }
      } else if (sscanf(line, "Line_width: %lf", &data->line_width) == 1) {
        if (data->line_width <= 0 || data->line_width > 100) {
          data->line_width = 1;
        }
      } else if (sscanf(line, "Line_color: %9s", data->line_color) == 1) {
        if (strlen(data->line_color) != 7) {
          strcpy(data->line_color, "#FFFFFF");
        }
      } else if (sscanf(line, "Vertexes_paint_type: %9s",
                        data->vertexes_paint_type) == 1) {
        if (strcmp(data->vertexes_paint_type, "round") != 0 &&
            strcmp(data->vertexes_paint_type, "square") != 0 &&
            strcmp(data->vertexes_paint_type, "none") != 0) {
          strcpy(data->vertexes_paint_type, "round");
        }
      } else if (sscanf(line, "Vertexes_size: %lf", &data->vertexes_size) ==
                 1) {
        if (data->vertexes_size <= 0 || data->vertexes_size > 100) {
          data->vertexes_size = 1;
        }
      } else if (sscanf(line, "Vertexes_color: %9s", data->vertexes_color) ==
                 1) {
        if (strlen(data->vertexes_color) != 7) {
          strcpy(data->vertexes_color, "#FFFFFF");
        }
      } else if (sscanf(line, "Background_color: %9s",
                        data->background_color) == 1) {
        if (strlen(data->background_color) != 7) {
          strcpy(data->background_color, "#000000");
        }
      }
    }
    fclose(file);
  }
}

int save_settings(settings_data* data) {
  int status = 0;
  FILE* file = fopen("settings.cfg", "w");
  if (file == NULL) {
    status = 1;
  }
  if (!status) {
    fprintf(file, "Projection_type: %s\n", data->projection_type);
    fprintf(file, "Line_type: %s\n", data->line_type);
    fprintf(file, "Line_width: %lf\n", data->line_width);
    fprintf(file, "Line_color: %s\n", data->line_color);
    fprintf(file, "Vertexes_paint_type: %s\n", data->vertexes_paint_type);
    fprintf(file, "Vertexes_size: %lf\n", data->vertexes_size);
    fprintf(file, "Vertexes_color: %s\n", data->vertexes_color);
    fprintf(file, "Background_color: %s\n", data->background_color);
  }
  fclose(file);
  return status;
}
