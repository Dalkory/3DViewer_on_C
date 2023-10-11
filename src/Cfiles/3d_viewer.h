#ifndef SRC_3D_VIEWER_H
#define SRC_3D_VIEWER_H

#define PI 3.141592653589793238462643383279502884197

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct vertexes {
  int rows;
  int cols;
  double** A;
} vertexes_data;

typedef struct facets {
  int rows;
  int* number_of_vertexes;
  int** facets_vertexes;
} facets_data;

typedef struct data {
  unsigned vertex_count;
  unsigned facets_count;
  vertexes_data vertexes_data;
  facets_data facets_data;
} model_data;

typedef struct settings {
  char projection_type[10];
  char line_type[10];
  double line_width;
  char line_color[10];
  char vertexes_paint_type[10];
  double vertexes_size;
  char vertexes_color[10];
  char background_color[10];
} settings_data;

// parser

int parse_obj(char* path, model_data* data);
void count_vertexes_and_facets(FILE* file, model_data* data);
int count_number_vertexes_for_each_facet(FILE* file, model_data* data);
void parse_model_vertexes(FILE* file, model_data* data);
void parse_model_facets(FILE* file, model_data* data);
int allocate_vertexes_array(model_data* data);
int allocate_facets_array(model_data* data);
void free_data(model_data* data);

// affine

int scale_model(model_data* data, double scale_amount);
int move_model(model_data* data, char direction, double movement);
int rotate_model(model_data* data, char axis, double deg_angle);
void rotate_x(model_data* data, double rad_angle);
void rotate_y(model_data* data, double rad_angle);
void rotate_z(model_data* data, double rad_angle);

// saver

void set_default_settings(settings_data* data);
void init_settings(settings_data* data);
int save_settings(settings_data* data);

#endif  // SRC_3D_VIEWER_H
