#include "3d_viewer.h"

int main() {
  settings_data set = {};
  model_data DATA = {};
  char path[256] = "../Obj/pyramid.obj";
  int status = parse_obj(path, &DATA);
  printf("status = %d\n", status);
  printf("DATA vertex_count = %u\n", DATA.vertex_count);
  printf("DATA facets_count = %u\n\n", DATA.facets_count);
  printf("DATA vertexes_data\n");
  printf("rows = %d\n", DATA.vertexes_data.rows);
  printf("cols = %d\n", DATA.vertexes_data.cols);
  for (int i = 0; i < DATA.vertexes_data.rows; i++) {
    for (int j = 0; j < DATA.vertexes_data.cols; j++) {
      printf(" %lf  ", DATA.vertexes_data.A[i][j]);
    }
    printf("  \n");
  }
  printf("\n");
  printf("DATA facets_data\n");
  printf("rows = %d\n", DATA.facets_data.rows);
  for (int i = 0; i < DATA.facets_data.rows; i++) {
    printf("Nvert = %d  ||  ", DATA.facets_data.number_of_vertexes[i]);
    for (int j = 0; j < DATA.facets_data.number_of_vertexes[i]; j++) {
      printf(" %d  ", DATA.facets_data.facets_vertexes[i][j]);
    }
    printf("  \n");
  }
  free_data(&DATA);

  init_settings(&set);
  save_settings(&set);
  return 0;
}
