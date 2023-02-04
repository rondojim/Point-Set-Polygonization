#include <chrono>

#include "../include/ch2polyline_algo.hpp"
#include "../include/incremental_algo.hpp"
#include "../include/onion_algo.hpp"
#include "../include/utils.hpp"

int main(int argc, char *argv[]) {
  if (argc != 9 && argc != 11 && argc != 13 && argc != 15) {
    fprintf(stderr,
            "Usage: $./to_polygon –i <point set input file> –ο <output "
            "file> –algorithm <incremental/convex_hull/onion> -edge_selection "
            "<1/2/3 | except onion algorithm> -initialization <1a/1b/2a/2b | "
            "only in incremental algorithm> -onion_initialization <1 to 5>\n");
    return -1;
  }

  char input_file[MAXBUF], output_file[MAXBUF];

  ALGORITHM_CHOICE algorithm;
  INIT_OPTION_INCR initialization;
  VISIBLE_EDGE_CHOICE edge_selection;
  INIT_OPTION_ONION onion_initialization;

  bool testing_mode = false;
  if (!parse_arguments(argc, argv, input_file, output_file, &algorithm,
                       &edge_selection, &initialization, &onion_initialization,
                       &testing_mode)) {
    return EXIT_FAILURE;
  }

  int i, x, y;
  char line1[MAXBUF], line2[MAXBUF];
  FILE *fp = fopen(input_file, "r");
  std::vector<Point_2> points;

  if (fp == NULL) {
    fprintf(stderr, "Error opening input file.\n");
    return EXIT_FAILURE;
  }

  read_input(fp, line1, line2, points);

  Polygon_2 polygon_line, convex_hull;
  char str_algo[50];

  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();

  switch (algorithm) {
    case INCREMENTAL_ALGO:
      run_incremental_algo(polygon_line, convex_hull, points, initialization,
                           edge_selection);
      sprintf(str_algo, "incremental");
      break;
    case CONVEX_HULL_ALGO:
      ch2polyline_algo(polygon_line, convex_hull, points, edge_selection);
      sprintf(str_algo, "convex_hull");
      break;
    default:
      onion_algo(polygon_line, convex_hull, points, onion_initialization);
      sprintf(str_algo, "onion_algo");
  }

  end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_ms = (end - start);
  int construction_time =
      (int)(std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                .count());

  if (!testing_mode) {
    print_output(polygon_line, convex_hull, str_algo, edge_selection,
                 initialization, onion_initialization, output_file,
                 construction_time);
  } else {  // Only in testing mode!
    // print some info to check if test case passed
    FILE *rfp = fopen("results.txt", "w");
    fprintf(rfp, "%lu ", polygon_line.size());
    fprintf(rfp, "%c ", (polygon_line.is_simple()) ? ('Y') : ('N'));
    fprintf(rfp, "%lld", (ll)fabs(convex_hull.area()));
  }
  return EXIT_SUCCESS;
}