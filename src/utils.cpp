#include "../include/utils.hpp"

#include "../include/pick_algo.hpp"

bool parse_arguments(int argc, char *argv[], char *input_file,
                     char *output_file, ALGORITHM_CHOICE *algorithm,
                     VISIBLE_EDGE_CHOICE *edge_selection,
                     INIT_OPTION_INCR *initialization,
                     INIT_OPTION_ONION *onion_initialization,
                     bool *testing_mode) {
  bool incremental_algo = false, convex_hull_algo = false, onion_algo = false;
  bool edge_selection_given = false, initialization_given = false,
       onion_initialization_given = false;

  for (int i = 1; i < argc; i += 2) {
    if (!strcmp(argv[i], "-i")) {
      strcpy(input_file, argv[i + 1]);
    } else if (!strcmp(argv[i], "-o")) {
      strcpy(output_file, argv[i + 1]);
    } else if (!strcmp(argv[i], "-algorithm")) {
      if (!strcmp(argv[i + 1], "incremental")) {
        incremental_algo = true;
        *algorithm = INCREMENTAL_ALGO;
      } else if (!strcmp(argv[i + 1], "convex_hull")) {
        convex_hull_algo = true;
        *algorithm = CONVEX_HULL_ALGO;
      } else if (!strcmp(argv[i + 1], "onion")) {
        onion_algo = true;
        *algorithm = ONION_ALGO;
      } else {
        fprintf(stderr, "Argument error: Invalid algorithm option.\n");
        return false;
      }

    } else if (!strcmp(argv[i], "-edge_selection")) {
      edge_selection_given = true;
      *edge_selection = (VISIBLE_EDGE_CHOICE)(atoi(argv[i + 1]) - 1);
      bool valid_arg = false;

      for (int j = 0; j < 3; ++j) {
        if ((int)*edge_selection == j) {
          valid_arg = true;
        }
      }

      if (!valid_arg) {
        fprintf(stderr,
                "Argument error: Given invalid edge_selection argument.\n");
        return false;
      }
    } else if (!strcmp(argv[i], "-initialization")) {
      initialization_given = true;
      const char *valid_init_values[] = {"1a", "1b", "2a", "2b"};
      bool valid_arg = false;
      for (int j = 0; j < 4; ++j) {
        if (!strcmp(argv[i + 1], valid_init_values[j])) {
          valid_arg = true;
          *initialization = (INIT_OPTION_INCR)(j);
        }
      }

      if (!valid_arg) {
        fprintf(stderr,
                "Argument error: Given invalid initialization argument.\n");
        return false;
      }
    } else if (!strcmp(argv[i], "-onion_initialization")) {
      onion_initialization_given = true;
      *onion_initialization = (INIT_OPTION_ONION)(atoi(argv[i + 1]) - 1);
      bool valid_arg = false;

      for (int j = 0; j < 5; ++j) {
        if (*onion_initialization == j) {
          valid_arg = true;
        }
      }

      if (!valid_arg) {
        fprintf(
            stderr,
            "Argument error: Given invalid onion_initialization argument.\n");
        return false;
      }

    } else if (!strcmp(argv[i], "-t")) {
      *testing_mode = (bool)atoi(argv[i + 1]);
    } else {
      fprintf(stderr, "Argument error: Wrong argument given!\n");
      return false;
    }
  }

  if (!incremental_algo && !convex_hull_algo && edge_selection_given) {
    fprintf(stderr,
            "Argument error: edge_selection argument only "
            "used with the "
            "incremental or the convex hull algorithm.\n");
    return false;
  } else if (initialization_given && !incremental_algo) {
    fprintf(stderr,
            "Argument error: initialization argument only "
            "used with the "
            "incremental algorithm.\n");
    return false;
  } else if (!onion_algo && onion_initialization_given) {
    fprintf(stderr,
            "Argument error: initilization argument only used with the onion "
            "algorithm.\n");
    return false;
  } else if (incremental_algo &&
             (!edge_selection_given || !initialization_given)) {
    fprintf(stderr,
            "Argument error: Incrimental algorithm needs edge selection and "
            "initialization arguments!\n");
    return false;
  } else if (convex_hull_algo && !edge_selection_given) {
    fprintf(stderr,
            "Argument error: Convex hull algorithm needs edge selection "
            "argument!\n");
    return false;
  } else if (onion_algo && !onion_initialization_given) {
    fprintf(stderr,
            "Argument error: Onion algorithm needs onion initilization "
            "argument!\n");
    return false;
  }

  return true;
}

void print_output(Polygon_2 &polygon_line, Polygon_2 &convex_hull,
                  char *str_algo, VISIBLE_EDGE_CHOICE edge_selection,
                  INIT_OPTION_INCR initialization,
                  INIT_OPTION_ONION onion_initialization, char *output_file,
                  int construction_time) {
  FILE *fp = fopen(output_file, "w");
  fprintf(fp, "Polygonization\n");
  for (int i = 0; i < polygon_line.size(); ++i) {
    fprintf(fp, "%d %d\n", (int)polygon_line[i].x(), (int)polygon_line[i].y());
  }

  for (int i = 0; i < polygon_line.size(); ++i) {
    int nxt = (i + 1) % polygon_line.size();
    fprintf(fp, "%d %d %d %d\n", (int)polygon_line[i].x(),
            (int)polygon_line[i].y(), (int)polygon_line[nxt].x(),
            (int)polygon_line[nxt].y());
  }

  fprintf(fp, "Algorithm: %s\n", str_algo);
  if (str_algo[0] != 'o') {
    fprintf(fp, "Edge_selection: %d\n", edge_selection + 1);
  }
  if (str_algo[0] == 'i') {
    fprintf(fp, "Incremental Initialization: %d%c\n", initialization / 2 + 1,
            (initialization % 2) ? 'b' : 'a');
  } else if (str_algo[0] == 'o') {
    fprintf(fp, "Onion Initialization: %d\n", onion_initialization + 1);
  }
  ll area = llabs((ll)polygon_line.area());
  fprintf(fp, "Area: %lld\n", area);
  fprintf(fp, "Pick_calculated_area: %lld\n", pick_algo(polygon_line));
  fprintf(fp, "Ratio: %f\n", (double)area / llabs((ll)convex_hull.area()));
  fprintf(fp, "Construction time: %d ms\n", construction_time);
}

void read_input(FILE *fp, char *line1, char *line2,
                std::vector<Point_2> &points) {
  fgets(line1, MAXBUF, fp);
  fgets(line2, MAXBUF, fp);

  int i, x, y;
  while (fscanf(fp, "%d %d %d", &i, &x, &y) != EOF) {
    points.push_back(Point_2(x, y));
  }
}

static bool compare_option(const Point_2 &lhs, const Point_2 &rhs,
                           INIT_OPTION_INCR initialization) {
  if (initialization == X_DESCENDING) {
    return (lhs.x() == rhs.x()) ? (lhs.y() >= rhs.y()) : (lhs.x() > rhs.x());
  } else if (initialization == X_ASCENDING) {
    return (lhs.x() == rhs.x()) ? (lhs.y() <= rhs.y()) : (lhs.x() < rhs.x());
  } else if (initialization == Y_DESCENDING) {
    return (lhs.y() == rhs.y()) ? (lhs.x() >= rhs.x()) : (lhs.y() > rhs.y());
  } else {
    return (lhs.y() == rhs.y()) ? (lhs.x() <= rhs.x()) : (lhs.y() < rhs.y());
  }
}

static void merge(std::vector<Point_2> &points, int left, int mid, int right,
                  INIT_OPTION_INCR initialization) {
  int subarray_1 = mid - left + 1;
  int subarray_2 = right - mid;

  std::vector<Point_2> left_vec, right_vec;

  for (int i = 0; i < subarray_1; i++) left_vec.push_back(points[left + i]);
  for (int j = 0; j < subarray_2; j++) right_vec.push_back(points[mid + 1 + j]);

  int subarray_1_idx = 0, subarray_2_idx = 0;
  int merged_array_idx = left;

  while (subarray_1_idx < subarray_1 && subarray_2_idx < subarray_2) {
    if (compare_option(left_vec[subarray_1_idx], right_vec[subarray_2_idx],
                       initialization)) {
      points[merged_array_idx] = left_vec[subarray_1_idx];
      subarray_1_idx++;
    } else {
      points[merged_array_idx] = right_vec[subarray_2_idx];
      subarray_2_idx++;
    }
    merged_array_idx++;
  }

  while (subarray_1_idx < subarray_1) {
    points[merged_array_idx] = left_vec[subarray_1_idx];
    subarray_1_idx++;
    merged_array_idx++;
  }

  while (subarray_2_idx < subarray_2) {
    points[merged_array_idx] = right_vec[subarray_2_idx];
    subarray_2_idx++;
    merged_array_idx++;
  }
}

void merge_sort(std::vector<Point_2> &points, int begin, int end,
                INIT_OPTION_INCR initialization) {
  if (begin >= end) return;

  int mid = begin + (end - begin) / 2;
  merge_sort(points, begin, mid, initialization);
  merge_sort(points, mid + 1, end, initialization);
  merge(points, begin, mid, end, initialization);
}

bool do_intersect_corner(Polygon_2 &polygon, Segment_2 segment) {
  for (int i = 0; i < polygon.size(); ++i) {
    int nxt_idx = (i == polygon.size() - 1) ? (0) : (i + 1);  // loop cyclicly
    Segment_2 cur_segment = Segment_2(polygon[i], polygon[nxt_idx]);

    if (do_intersect(cur_segment, segment) &&
        !(cur_segment.source() == segment.target() ||
          cur_segment.target() == segment.target())) {
      return true;
    }
  }
  return false;
}

bool do_intersect_middle(Polygon_2 &polygon, Segment_2 segment,
                         Segment_2 edge_lr) {
  for (int i = 0; i < polygon.size(); ++i) {
    int nxt_idx = (i == polygon.size() - 1) ? (0) : (i + 1);  // loop cyclicly
    Segment_2 cur_segment = Segment_2(polygon[i], polygon[nxt_idx]);

    if (cur_segment == edge_lr) continue;
    if (CGAL::intersection(cur_segment, segment)) {
      return true;
    }
  }
  return false;
}