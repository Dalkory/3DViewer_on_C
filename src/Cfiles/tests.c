#include <check.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "3d_viewer.h"

#define ACC 1e-5f

START_TEST(parse_test_1) {
  int ref_count_vertexes = 5;
  int ref_count_facets = 8;
  model_data parse_data = {};
  int ref_result = 0;
  int result = parse_obj("Obj/pyramid.obj", &parse_data);
  ck_assert_int_eq(result, ref_result);
  ck_assert_int_eq(parse_data.vertex_count, ref_count_vertexes);
  ck_assert_int_eq(parse_data.facets_count, ref_count_facets);
  ck_assert_ptr_ne(parse_data.vertexes_data.A, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.number_of_vertexes, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.facets_vertexes, NULL);
  float ref_vertexes[][3] = {{},
                             {0.0, 1.0, 0.0},
                             {-1.0, 0.0, -1.0},
                             {1.0, 0.0, -1.0},
                             {1.0, 0.0, 1.0},
                             {-1.0, 0.0, 1.0}};
  unsigned int ref_facets[][3] = {{},        {1, 2, 3}, {1},
                                  {1},       {1, 3, 4}, {1, 4, 5},
                                  {1, 5, 2}, {3, 2, 4}, {2, 5, 4}};
  for (int i = 1; i <= ref_count_vertexes; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_float_eq_tol(parse_data.vertexes_data.A[i][j],
                             ref_vertexes[i][j], ACC);
    }
  }
  for (int i = 1; i <= ref_count_facets; i++) {
    for (int j = 0; j < parse_data.facets_data.number_of_vertexes[i]; j++) {
      ck_assert_int_eq(parse_data.facets_data.facets_vertexes[i][j],
                       ref_facets[i][j]);
    }
  }
  free_data(&parse_data);
}
END_TEST

START_TEST(parse_test_2) {
  int ref_count_vertexes = 15662;
  int ref_count_facets = 29340;
  model_data parse_data = {};
  int ref_result = 0;
  int result = parse_obj("Obj/ball.obj", &parse_data);
  ck_assert_int_eq(result, ref_result);
  ck_assert_int_eq(parse_data.vertex_count, ref_count_vertexes);
  ck_assert_int_eq(parse_data.facets_count, ref_count_facets);
  ck_assert_ptr_ne(parse_data.vertexes_data.A, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.number_of_vertexes, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.facets_vertexes, NULL);
  free_data(&parse_data);
}
END_TEST

START_TEST(parse_test_3) {
  int ref_count_vertexes = 3441;
  int ref_count_facets = 5535;
  model_data parse_data = {};
  int ref_result = 0;
  int result = parse_obj("Obj/skull.obj", &parse_data);
  ck_assert_int_eq(result, ref_result);
  ck_assert_int_eq(parse_data.vertex_count, ref_count_vertexes);
  ck_assert_int_eq(parse_data.facets_count, ref_count_facets);
  ck_assert_ptr_ne(parse_data.vertexes_data.A, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.number_of_vertexes, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.facets_vertexes, NULL);
  free_data(&parse_data);
}
END_TEST

START_TEST(parse_test_4) {
  model_data parse_data = {};
  int ref_result = 1;
  int result = parse_obj("Obj/non-exist.obj", &parse_data);
  ck_assert_int_eq(result, ref_result);
}
END_TEST

START_TEST(affine_test_1) {
  model_data parse_data = {};
  int ref_result = 0;
  int ref_count_vertexes = 5;
  int result = parse_obj("Obj/pyramid.obj", &parse_data);
  ck_assert_int_eq(result, ref_result);
  ck_assert_ptr_ne(parse_data.vertexes_data.A, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.number_of_vertexes, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.facets_vertexes, NULL);
  float ref_vertexes[][3] = {{},
                             {1.0, 1.0, 0.0},
                             {0.0, 0.0, -1.0},
                             {2.0, 0.0, -1.0},
                             {2.0, 0.0, 1.0},
                             {0.0, 0.0, 1.0}};
  move_model(&parse_data, 'x', 1.0);
  for (int i = 1; i <= ref_count_vertexes; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_float_eq_tol(parse_data.vertexes_data.A[i][j],
                             ref_vertexes[i][j], ACC);
    }
  }
  free_data(&parse_data);
}
END_TEST

START_TEST(affine_test_2) {
  model_data parse_data = {};
  int ref_result = 0;
  int ref_count_vertexes = 5;
  int result = parse_obj("Obj/pyramid.obj", &parse_data);
  ck_assert_int_eq(result, ref_result);
  ck_assert_ptr_ne(parse_data.vertexes_data.A, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.number_of_vertexes, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.facets_vertexes, NULL);
  float ref_vertexes[][3] = {{},
                             {1.31563, 1.0, 0.0},
                             {0.31563, 0.0, -1.0},
                             {2.31563, 0.0, -1.0},
                             {2.31563, 0.0, 1.0},
                             {0.31563, 0.0, 1.0}};
  move_model(&parse_data, 'x', 1.31563);
  for (int i = 1; i <= ref_count_vertexes; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_float_eq_tol(parse_data.vertexes_data.A[i][j],
                             ref_vertexes[i][j], ACC);
    }
  }
  free_data(&parse_data);
}
END_TEST

START_TEST(affine_test_3) {
  model_data parse_data = {};
  int ref_result = 0;
  int ref_count_vertexes = 5;
  int result = parse_obj("Obj/pyramid.obj", &parse_data);
  ck_assert_int_eq(result, ref_result);
  ck_assert_ptr_ne(parse_data.vertexes_data.A, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.number_of_vertexes, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.facets_vertexes, NULL);
  float ref_vertexes[][3] = {{},
                             {0.0, 2.0, 0.0},
                             {-1.0, 1.0, -1.0},
                             {1.0, 1.0, -1.0},
                             {1.0, 1.0, 1.0},
                             {-1.0, 1.0, 1.0}};
  move_model(&parse_data, 'y', 1.0);
  for (int i = 1; i <= ref_count_vertexes; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_float_eq_tol(parse_data.vertexes_data.A[i][j],
                             ref_vertexes[i][j], ACC);
    }
  }
  free_data(&parse_data);
}
END_TEST

START_TEST(affine_test_4) {
  model_data parse_data = {};
  int ref_result = 0;
  int ref_count_vertexes = 5;
  int result = parse_obj("Obj/pyramid.obj", &parse_data);
  ck_assert_int_eq(result, ref_result);
  ck_assert_ptr_ne(parse_data.vertexes_data.A, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.number_of_vertexes, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.facets_vertexes, NULL);
  float ref_vertexes[][3] = {{},
                             {0.0, 2.563113, 0.0},
                             {-1.0, 1.563113, -1.0},
                             {1.0, 1.563113, -1.0},
                             {1.0, 1.563113, 1.0},
                             {-1.0, 1.563113, 1.0}};
  move_model(&parse_data, 'y', 1.563113);
  for (int i = 1; i <= ref_count_vertexes; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_float_eq_tol(parse_data.vertexes_data.A[i][j],
                             ref_vertexes[i][j], ACC);
    }
  }
  free_data(&parse_data);
}
END_TEST

START_TEST(affine_test_5) {
  model_data parse_data = {};
  int ref_result = 0;
  int ref_count_vertexes = 5;
  int result = parse_obj("Obj/pyramid.obj", &parse_data);
  ck_assert_int_eq(result, ref_result);
  ck_assert_ptr_ne(parse_data.vertexes_data.A, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.number_of_vertexes, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.facets_vertexes, NULL);
  float ref_vertexes[][3] = {{},
                             {0.0, 1.0, 1.0},
                             {-1.0, 0.0, 0.0},
                             {1.0, 0.0, 0.0},
                             {1.0, 0.0, 2.0},
                             {-1.0, 0.0, 2.0}};
  move_model(&parse_data, 'z', 1.0);
  for (int i = 1; i <= ref_count_vertexes; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_float_eq_tol(parse_data.vertexes_data.A[i][j],
                             ref_vertexes[i][j], ACC);
    }
  }
  free_data(&parse_data);
}
END_TEST

START_TEST(affine_test_6) {
  model_data parse_data = {};
  int ref_result = 0;
  int ref_count_vertexes = 5;
  int result = parse_obj("Obj/pyramid.obj", &parse_data);
  ck_assert_int_eq(result, ref_result);
  ck_assert_ptr_ne(parse_data.vertexes_data.A, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.number_of_vertexes, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.facets_vertexes, NULL);
  float ref_vertexes[][3] = {{},
                             {0.0, 1.0, 1.872612},
                             {-1.0, 0.0, 0.872612},
                             {1.0, 0.0, 0.872612},
                             {1.0, 0.0, 2.872612},
                             {-1.0, 0.0, 2.872612}};
  move_model(&parse_data, 'z', 1.872612);
  for (int i = 1; i <= ref_count_vertexes; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_float_eq_tol(parse_data.vertexes_data.A[i][j],
                             ref_vertexes[i][j], ACC);
    }
  }
  free_data(&parse_data);
}
END_TEST

START_TEST(affine_test_7) {
  model_data parse_data = {};
  int ref_result = 0;
  int ref_count_vertexes = 5;
  int result = parse_obj("Obj/pyramid.obj", &parse_data);
  ck_assert_int_eq(result, ref_result);
  ck_assert_ptr_ne(parse_data.vertexes_data.A, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.number_of_vertexes, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.facets_vertexes, NULL);
  float ref_vertexes[][3] = {{},
                             {0.0, 1.0, 0.0},
                             {-1.0, 0.0, -1.0},
                             {1.0, 0.0, -1.0},
                             {1.0, 0.0, 1.0},
                             {-1.0, 0.0, 1.0}};
  rotate_model(&parse_data, 'x', 360);
  for (int i = 1; i <= ref_count_vertexes; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_float_eq_tol(parse_data.vertexes_data.A[i][j],
                             ref_vertexes[i][j], ACC);
    }
  }
  free_data(&parse_data);
}
END_TEST

START_TEST(affine_test_8) {
  model_data parse_data = {};
  int ref_result = 0;
  int ref_count_vertexes = 5;
  int result = parse_obj("Obj/pyramid.obj", &parse_data);
  ck_assert_int_eq(result, ref_result);
  ck_assert_ptr_ne(parse_data.vertexes_data.A, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.number_of_vertexes, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.facets_vertexes, NULL);
  float ref_vertexes[][3] = {{},
                             {0.0, -1.0, 0.0},
                             {-1.0, 0.0, 1.0},
                             {1.0, 0.0, 1.0},
                             {1.0, 0.0, -1.0},
                             {-1.0, 0.0, -1.0}};
  rotate_model(&parse_data, 'x', 180);
  for (int i = 1; i <= ref_count_vertexes; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_float_eq_tol(parse_data.vertexes_data.A[i][j],
                             ref_vertexes[i][j], ACC);
    }
  }
  free_data(&parse_data);
}
END_TEST

START_TEST(affine_test_9) {
  model_data parse_data = {};
  int ref_result = 0;
  int ref_count_vertexes = 5;
  int result = parse_obj("Obj/pyramid.obj", &parse_data);
  ck_assert_int_eq(result, ref_result);
  ck_assert_ptr_ne(parse_data.vertexes_data.A, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.number_of_vertexes, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.facets_vertexes, NULL);
  float ref_vertexes[][3] = {{},
                             {0.0, -1.0, 0.0},
                             {-1.0, 0.0, 1.0},
                             {1.0, 0.0, 1.0},
                             {1.0, 0.0, -1.0},
                             {-1.0, 0.0, -1.0}};
  rotate_model(&parse_data, 'x', -180);
  for (int i = 1; i <= ref_count_vertexes; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_float_eq_tol(parse_data.vertexes_data.A[i][j],
                             ref_vertexes[i][j], ACC);
    }
  }
  free_data(&parse_data);
}
END_TEST

START_TEST(affine_test_10) {
  model_data parse_data = {};
  int ref_result = 0;
  int ref_count_vertexes = 5;
  int result = parse_obj("Obj/pyramid.obj", &parse_data);
  ck_assert_int_eq(result, ref_result);
  ck_assert_ptr_ne(parse_data.vertexes_data.A, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.number_of_vertexes, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.facets_vertexes, NULL);
  float ref_vertexes[][3] = {{},
                             {0.0, 1.0, 0.0},
                             {-1.0, 0.0, -1.0},
                             {1.0, 0.0, -1.0},
                             {1.0, 0.0, 1.0},
                             {-1.0, 0.0, 1.0}};
  rotate_model(&parse_data, 'y', 360);
  for (int i = 1; i <= ref_count_vertexes; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_float_eq_tol(parse_data.vertexes_data.A[i][j],
                             ref_vertexes[i][j], ACC);
    }
  }
  free_data(&parse_data);
}
END_TEST

START_TEST(affine_test_11) {
  model_data parse_data = {};
  int ref_result = 0;
  int ref_count_vertexes = 5;
  int result = parse_obj("Obj/pyramid.obj", &parse_data);
  ck_assert_int_eq(result, ref_result);
  ck_assert_ptr_ne(parse_data.vertexes_data.A, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.number_of_vertexes, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.facets_vertexes, NULL);
  float ref_vertexes[][3] = {{},
                             {0.0, 1.0, 0.0},
                             {1.0, 0.0, 1.0},
                             {-1.0, 0.0, 1.0},
                             {-1.0, 0.0, -1.0},
                             {1.0, 0.0, -1.0}};
  rotate_model(&parse_data, 'y', 180);
  for (int i = 1; i <= ref_count_vertexes; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_float_eq_tol(parse_data.vertexes_data.A[i][j],
                             ref_vertexes[i][j], ACC);
    }
  }
  free_data(&parse_data);
}
END_TEST

START_TEST(affine_test_12) {
  model_data parse_data = {};
  int ref_result = 0;
  int ref_count_vertexes = 5;
  int result = parse_obj("Obj/pyramid.obj", &parse_data);
  ck_assert_int_eq(result, ref_result);
  ck_assert_ptr_ne(parse_data.vertexes_data.A, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.number_of_vertexes, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.facets_vertexes, NULL);
  float ref_vertexes[][3] = {{},
                             {0.0, 1.0, 0.0},
                             {1.0, 0.0, 1.0},
                             {-1.0, 0.0, 1.0},
                             {-1.0, 0.0, -1.0},
                             {1.0, 0.0, -1.0}};
  rotate_model(&parse_data, 'y', -180);
  for (int i = 1; i <= ref_count_vertexes; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_float_eq_tol(parse_data.vertexes_data.A[i][j],
                             ref_vertexes[i][j], ACC);
    }
  }
  free_data(&parse_data);
}
END_TEST

START_TEST(affine_test_13) {
  model_data parse_data = {};
  int ref_result = 0;
  int ref_count_vertexes = 5;
  int result = parse_obj("Obj/pyramid.obj", &parse_data);
  ck_assert_int_eq(result, ref_result);
  ck_assert_ptr_ne(parse_data.vertexes_data.A, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.number_of_vertexes, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.facets_vertexes, NULL);
  float ref_vertexes[][3] = {{},
                             {0.0, 1.0, 0.0},
                             {-1.0, 0.0, -1.0},
                             {1.0, 0.0, -1.0},
                             {1.0, 0.0, 1.0},
                             {-1.0, 0.0, 1.0}};
  rotate_model(&parse_data, 'z', 360);
  for (int i = 1; i <= ref_count_vertexes; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_float_eq_tol(parse_data.vertexes_data.A[i][j],
                             ref_vertexes[i][j], ACC);
    }
  }
  free_data(&parse_data);
}
END_TEST

START_TEST(affine_test_14) {
  model_data parse_data = {};
  int ref_result = 0;
  int ref_count_vertexes = 5;
  int result = parse_obj("Obj/pyramid.obj", &parse_data);
  ck_assert_int_eq(result, ref_result);
  ck_assert_ptr_ne(parse_data.vertexes_data.A, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.number_of_vertexes, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.facets_vertexes, NULL);
  float ref_vertexes[][3] = {{},
                             {0.0, 1.0, 0.0},
                             {1.0, 0.0, 1.0},
                             {-1.0, 0.0, 1.0},
                             {-1.0, 0.0, -1.0},
                             {1.0, 0.0, -1.0}};
  rotate_model(&parse_data, 'z', 180);
  for (int i = 1; i <= ref_count_vertexes; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_float_eq_tol(parse_data.vertexes_data.A[i][j],
                             ref_vertexes[i][j], ACC);
    }
  }
  free_data(&parse_data);
}
END_TEST

START_TEST(affine_test_15) {
  model_data parse_data = {};
  int ref_result = 0;
  int ref_count_vertexes = 5;
  int result = parse_obj("Obj/pyramid.obj", &parse_data);
  ck_assert_int_eq(result, ref_result);
  ck_assert_ptr_ne(parse_data.vertexes_data.A, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.number_of_vertexes, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.facets_vertexes, NULL);
  float ref_vertexes[][3] = {{},
                             {0.0, 1.0, 0.0},
                             {1.0, 0.0, 1.0},
                             {-1.0, 0.0, 1.0},
                             {-1.0, 0.0, -1.0},
                             {1.0, 0.0, -1.0}};
  rotate_model(&parse_data, 'z', -180);
  for (int i = 1; i <= ref_count_vertexes; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_float_eq_tol(parse_data.vertexes_data.A[i][j],
                             ref_vertexes[i][j], ACC);
    }
  }
  free_data(&parse_data);
}
END_TEST

START_TEST(affine_test_16) {
  model_data parse_data = {};
  int ref_result = 0;
  int ref_count_vertexes = 5;
  int result = parse_obj("Obj/pyramid.obj", &parse_data);
  ck_assert_int_eq(result, ref_result);
  ck_assert_ptr_ne(parse_data.vertexes_data.A, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.number_of_vertexes, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.facets_vertexes, NULL);
  float ref_vertexes[][3] = {{},
                             {0.0, 0.5, 0.0},
                             {-0.5, 0.0, -0.5},
                             {0.5, 0.0, -0.5},
                             {0.5, 0.0, 0.5},
                             {-0.5, 0.0, 0.5}};
  scale_model(&parse_data, 0.5);
  for (int i = 1; i <= ref_count_vertexes; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_float_eq_tol(parse_data.vertexes_data.A[i][j],
                             ref_vertexes[i][j], ACC);
    }
  }
  free_data(&parse_data);
}
END_TEST

START_TEST(affine_test_17) {
  model_data parse_data = {};
  int ref_result = 0;
  int ref_count_vertexes = 5;
  int result = parse_obj("Obj/pyramid.obj", &parse_data);
  ck_assert_int_eq(result, ref_result);
  ck_assert_ptr_ne(parse_data.vertexes_data.A, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.number_of_vertexes, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.facets_vertexes, NULL);
  float ref_vertexes[][3] = {{},
                             {0.0, 2.0, 0.0},
                             {-2.0, 0.0, -2.0},
                             {2.0, 0.0, -2.0},
                             {2.0, 0.0, 2.0},
                             {-2.0, 0.0, 2.0}};
  scale_model(&parse_data, 2);
  for (int i = 1; i <= ref_count_vertexes; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_float_eq_tol(parse_data.vertexes_data.A[i][j],
                             ref_vertexes[i][j], ACC);
    }
  }
  free_data(&parse_data);
}
END_TEST

START_TEST(affine_test_18) {
  model_data parse_data = {};
  int ref_result = 0;
  int result = parse_obj("Obj/pyramid.obj", &parse_data);
  ck_assert_int_eq(result, ref_result);
  ck_assert_ptr_ne(parse_data.vertexes_data.A, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.number_of_vertexes, NULL);
  ck_assert_ptr_ne(parse_data.facets_data.facets_vertexes, NULL);
  ref_result = 1;
  result = scale_model(&parse_data, -2);
  ck_assert_int_eq(result, ref_result);
  free_data(&parse_data);
}
END_TEST

Suite* Viewer() {
  Suite* s = suite_create("tests");
  TCase* tc_parse = tcase_create("parse_tests");
  TCase* tc_affine = tcase_create("affine_tests");
  tcase_add_test(tc_parse, parse_test_1);
  tcase_add_test(tc_parse, parse_test_2);
  tcase_add_test(tc_parse, parse_test_3);
  tcase_add_test(tc_parse, parse_test_4);
  tcase_add_test(tc_affine, affine_test_1);
  tcase_add_test(tc_affine, affine_test_2);
  tcase_add_test(tc_affine, affine_test_3);
  tcase_add_test(tc_affine, affine_test_4);
  tcase_add_test(tc_affine, affine_test_5);
  tcase_add_test(tc_affine, affine_test_6);
  tcase_add_test(tc_affine, affine_test_7);
  tcase_add_test(tc_affine, affine_test_8);
  tcase_add_test(tc_affine, affine_test_9);
  tcase_add_test(tc_affine, affine_test_10);
  tcase_add_test(tc_affine, affine_test_11);
  tcase_add_test(tc_affine, affine_test_12);
  tcase_add_test(tc_affine, affine_test_13);
  tcase_add_test(tc_affine, affine_test_14);
  tcase_add_test(tc_affine, affine_test_15);
  tcase_add_test(tc_affine, affine_test_16);
  tcase_add_test(tc_affine, affine_test_17);
  tcase_add_test(tc_affine, affine_test_18);
  suite_add_tcase(s, tc_parse);
  suite_add_tcase(s, tc_affine);

  return s;
}

void run_testcase(Suite* testcase) {
  SRunner* sr = srunner_create(testcase);
  putchar('\n');
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);
  srunner_free(sr);
}

int main() {
  run_testcase(Viewer());
  return 0;
}
