#include "3d_viewer.h"

int parse_obj(char* path, model_data* data) {
  int status = 0;
  FILE* file = fopen(path, "r");
  if (!file) {
    status = 1;
  }
  if (!status) {
    count_vertexes_and_facets(file, data);
    status = allocate_vertexes_array(data);
  }
  if (!status) {
    status = count_number_vertexes_for_each_facet(file, data);
  }
  if (!status) {
    status = allocate_facets_array(data);
  }
  if (!status) {
    parse_model_vertexes(file, data);
    parse_model_facets(file, data);
  }
  if (file) {
    fclose(file);
  }
  return status;
}

void count_vertexes_and_facets(FILE* file, model_data* data) {
  char line[1024];
  rewind(file);
  while (fgets(line, sizeof(line), file) != NULL) {
    if (strncmp(line, "v ", 2) == 0) {
      data->vertex_count++;
    } else if (strncmp(line, "f ", 2) == 0) {
      data->facets_count++;
    }
  }
}

int count_number_vertexes_for_each_facet(FILE* file, model_data* data) {
  int status = 0;
  data->facets_data.number_of_vertexes =
      (int*)calloc(data->facets_count + 1, sizeof(int));
  if (data->facets_data.number_of_vertexes == NULL) {
    status = 1;
  }
  if (!status) {
    char line[1024];
    int i = 1;
    rewind(file);
    while (fgets(line, sizeof(line), file) != NULL) {
      if (strncmp(line, "f ", 2) == 0) {
        char* token = strtok(line + 2, " ");
        int token_count = 0;
        while (token != NULL) {
          if (atof(token)) {
            token_count++;
          }
          token = strtok(NULL, " ");
        }
        data->facets_data.number_of_vertexes[i] = token_count;
        i++;
      }
    }
  }
  return status;
}

void parse_model_vertexes(FILE* file, model_data* data) {
  char line[1024];
  int vertex_index = 1;
  rewind(file);
  while (fgets(line, sizeof(line), file) != NULL &&
         vertex_index <= data->vertexes_data.rows) {
    if (strncmp(line, "v ", 2) == 0) {
      char* token = strtok(line + 2, " ");
      for (int i = 0; i < 3; i++) {
        data->vertexes_data.A[vertex_index][i] = atof(token);
        token = strtok(NULL, " ");
      }
      vertex_index++;
    }
  }
}

void parse_model_facets(FILE* file, model_data* data) {
  char line[1024];
  int facet_index = 1;
  rewind(file);
  while (fgets(line, sizeof(line), file) != NULL &&
         facet_index < data->facets_data.rows) {
    if (strncmp(line, "f ", 2) == 0) {
      int facet_number_of_vertexes =
          data->facets_data.number_of_vertexes[facet_index];
      char* token = strtok(line + 2, " ");
      for (int i = 0; i < facet_number_of_vertexes; i++) {
        data->facets_data.facets_vertexes[facet_index][i] = atoi(token);
        if (data->facets_data.facets_vertexes[facet_index][i] < 0) {
          data->facets_data.facets_vertexes[facet_index][i] +=
              (data->vertex_count + 1);
        }
        token = strtok(NULL, " ");
      }
      facet_index++;
    }
  }
}

int allocate_vertexes_array(model_data* data) {
  int status = 0;
  data->vertexes_data.rows = data->vertex_count + 1;
  data->vertexes_data.cols = 3;
  if (data->vertex_count <= 0) {
    status = 1;
  }
  if (!status) {
    data->vertexes_data.A =
        (double**)calloc(data->vertexes_data.rows, sizeof(double*));
  }
  if (data->vertexes_data.A == NULL) {
    status = 1;
  }
  if (!status) {
    for (int i = 0; i < data->vertexes_data.rows; i++) {
      data->vertexes_data.A[i] =
          (double*)calloc(data->vertexes_data.cols, sizeof(double));
      if (data->vertexes_data.A[i] == NULL) {
        for (int j = 0; j < i; j++) {
          free(data->vertexes_data.A[j]);
        }
        free(data->vertexes_data.A);
        status = 1;
        break;
        ;
      }
    }
  }
  return status;
}

int allocate_facets_array(model_data* data) {
  int status = 0;
  data->facets_data.rows = data->facets_count + 1;
  data->facets_data.facets_vertexes =
      (int**)calloc(data->facets_data.rows, sizeof(int*));
  if (data->facets_data.facets_vertexes == NULL) {
    status = 1;
  }
  if (!status) {
    for (int i = 0; i < data->facets_data.rows; i++) {
      data->facets_data.facets_vertexes[i] =
          (int*)calloc(data->facets_data.number_of_vertexes[i], sizeof(int));
      if (data->facets_data.facets_vertexes[i] == NULL) {
        for (int j = 0; j < i; j++) {
          free(data->facets_data.facets_vertexes[j]);
        }
        free(data->facets_data.facets_vertexes);
        status = 1;
        break;
      }
    }
  }
  return status;
}

void free_data(model_data* data) {
  if (data->vertexes_data.A != NULL) {
    for (int i = 0; i < data->vertexes_data.rows; i++) {
      if (data->vertexes_data.A[i] != NULL) {
        free(data->vertexes_data.A[i]);
      }
    }
    free(data->vertexes_data.A);
    data->vertexes_data.A = NULL;
    data->vertexes_data.rows = 0;
    data->vertexes_data.cols = 0;
  }
  if (data->facets_data.facets_vertexes != NULL) {
    for (int i = 0; i < data->facets_data.rows; i++) {
      if (data->facets_data.facets_vertexes[i] != NULL) {
        free(data->facets_data.facets_vertexes[i]);
      }
    }
    free(data->facets_data.facets_vertexes);
    data->facets_data.facets_vertexes = NULL;
    data->facets_data.rows = 0;
  }
  if (data->facets_data.number_of_vertexes != NULL) {
    free(data->facets_data.number_of_vertexes);
    data->facets_data.number_of_vertexes = NULL;
  }
  data->vertex_count = 0;
  data->facets_count = 0;
}
