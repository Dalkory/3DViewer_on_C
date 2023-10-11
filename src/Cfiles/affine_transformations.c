#include "3d_viewer.h"

int scale_model(model_data* data, double scale_amount) {
  int status = 0;
  if (scale_amount < 0.0001) {
    status = 1;
  }
  if (!status) {
    for (size_t i = 1; i <= data->vertex_count; i++) {
      for (size_t j = 0; j < 3; j++) {
        data->vertexes_data.A[i][j] *= scale_amount;
      }
    }
  }
  return status;
}

int move_model(model_data* data, char direction,
               double movement) {  // direction (x, y, z)
  int d = -1, status = 0;
  if (direction == 'x') {
    d = 0;
  } else if (direction == 'y') {
    d = 1;
  } else if (direction == 'z') {
    d = 2;
  }
  status = d == -1 ? 1 : 0;
  if (!status) {
    for (size_t i = 1; i <= data->vertex_count; i++) {
      data->vertexes_data.A[i][d] += movement;
    }
  }
  return status;
}

int rotate_model(model_data* data, char axis,
                 double deg_angle) {  // axis (x, y, z), angle in deg
  double rad_angle = deg_angle * PI / 180;
  int status = 0;
  if (axis == 'x') {
    rotate_x(data, rad_angle);
  } else if (axis == 'y') {
    rotate_y(data, rad_angle);
  } else if (axis == 'z') {
    rotate_z(data, rad_angle);
  } else {
    status = 1;
  }
  return status;
}

void rotate_x(model_data* data, double rad_angle) {
  for (size_t i = 1; i <= data->vertex_count; i++) {
    double original_y = data->vertexes_data.A[i][1];
    double original_z = data->vertexes_data.A[i][2];
    data->vertexes_data.A[i][1] =
        original_y * cos(rad_angle) + original_z * sin(rad_angle);
    data->vertexes_data.A[i][2] =
        -original_y * sin(rad_angle) + original_z * cos(rad_angle);
  }
}

void rotate_y(model_data* data, double rad_angle) {
  for (size_t i = 1; i <= data->vertex_count; i++) {
    double original_x = data->vertexes_data.A[i][0];
    double original_z = data->vertexes_data.A[i][2];
    data->vertexes_data.A[i][0] =
        original_x * cos(rad_angle) + original_z * sin(rad_angle);
    data->vertexes_data.A[i][2] =
        -original_x * sin(rad_angle) + original_z * cos(rad_angle);
  }
}

void rotate_z(model_data* data, double rad_angle) {
  for (size_t i = 1; i <= data->vertex_count; i++) {
    double original_x = data->vertexes_data.A[i][0];
    double original_z = data->vertexes_data.A[i][2];
    data->vertexes_data.A[i][0] =
        original_x * cos(rad_angle) - original_z * sin(rad_angle);
    data->vertexes_data.A[i][2] =
        original_x * sin(rad_angle) + original_z * cos(rad_angle);
  }
}
