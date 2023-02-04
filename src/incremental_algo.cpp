#include "../include/incremental_algo.hpp"

static bool is_visible(Polygon_2 &polygon, Point_2 &new_point,
                       Point_2 &left_point, Point_2 &right_point) {
  Point_2 middle_point = CGAL::midpoint(left_point, right_point);

  if (!do_intersect_corner(polygon, Segment_2(new_point, left_point)) &&
      !do_intersect_middle(polygon, Segment_2(new_point, middle_point),
                           Segment_2(left_point, right_point)) &&
      !do_intersect_corner(polygon, Segment_2(new_point, right_point))) {
    return true;
  }
  return false;
}

static void expand_convex_hull(Polygon_2 &convex_hull,
                               std::vector<Segment_2> &red_segments,
                               Point_2 new_point) {
  for (int j = 0; j < convex_hull.size(); ++j) {
    int nxt_idx = (j == convex_hull.size() - 1) ? (0) : (j + 1);
    Point_2 left_point = convex_hull[j];
    Point_2 right_point = convex_hull[nxt_idx];

    if (is_visible(convex_hull, new_point, left_point, right_point)) {
      red_segments.push_back(Segment_2(left_point, right_point));
    }
  }

  Point_2 purple_point_1 = red_segments[0].source();
  Point_2 purple_point_2 = red_segments[red_segments.size() - 1].target();

  Polygon_2 expanded_convex_hull;
  bool reached_purple_point_1 = false;
  bool reached_purple_point_2 = false;

  for (int i = 0; i < convex_hull.size(); ++i) {
    if (i == 0 && convex_hull[i] == purple_point_2) {
      expanded_convex_hull.push_back(convex_hull[i]);
      continue;
    }

    if (convex_hull[i] == purple_point_1) {
      reached_purple_point_1 = true;
      expanded_convex_hull.push_back(convex_hull[i]);
      expanded_convex_hull.push_back(new_point);
    } else if (convex_hull[i] == purple_point_2) {
      reached_purple_point_2 = true;
      expanded_convex_hull.push_back(convex_hull[i]);
    } else if (reached_purple_point_1 == false ||
               reached_purple_point_2 == true) {
      expanded_convex_hull.push_back(convex_hull[i]);
    }
  }

  convex_hull = expanded_convex_hull;
}

static void expand_polygon_line(Polygon_2 &polygon_line,
                                std::vector<Segment_2> &red_segments,
                                Point_2 new_point,
                                VISIBLE_EDGE_CHOICE initialization) {
  int poly_idx;
  int red_idx = 0;
  for (int i = 0; i < polygon_line.size(); ++i) {
    if (red_segments[red_idx].source() == polygon_line[i]) {
      poly_idx = i;
      break;
    }
  }

  ll min_area = -1, max_area = -1;
  ll polygon_area = polygon_line.area();
  int poly_idx_min_area = -1, poly_idx_max_area = -1;
  while (true) {
    int nxt_idx = (poly_idx == polygon_line.size() - 1) ? (0) : (poly_idx + 1);
    Segment_2 poly_segment =
        Segment_2(polygon_line[poly_idx], polygon_line[nxt_idx]);

    ll cur_area =
        CGAL::area(polygon_line[poly_idx], polygon_line[nxt_idx], new_point);

    // check if the current segment of polygon line is red (i.e., on the ch)
    if (poly_segment == red_segments[red_idx]) {
      if (initialization == RANDOM_EDGE) {
        polygon_line.insert(polygon_line.begin() + poly_idx + 1, new_point);
        return;
      }
      if (cur_area < min_area || min_area == -1) {
        min_area = cur_area;
        poly_idx_min_area = poly_idx;
      }
      if (cur_area > max_area || max_area == -1) {
        max_area = cur_area;
        poly_idx_max_area = poly_idx;
      }
    } else {  // or walk the chain of polygon line segments under the red
              // segment (of ch)
      if (is_visible(polygon_line, new_point, polygon_line[poly_idx],
                     polygon_line[nxt_idx])) {
        if (initialization == RANDOM_EDGE) {
          polygon_line.insert(polygon_line.begin() + poly_idx + 1, new_point);
          return;
        }
        if (cur_area < min_area || min_area == -1) {
          min_area = cur_area;
          poly_idx_min_area = poly_idx;
        }
        if (cur_area > max_area || max_area == -1) {
          max_area = cur_area;
          poly_idx_max_area = poly_idx;
        }
      }
    }

    // if reached the end of red segment(s)
    if (poly_segment.target() == red_segments[red_idx].target()) {
      if (++red_idx == red_segments.size()) {
        break;
      }
    }
    poly_idx++;
  }

  if (initialization == MAX_AREA) {
    polygon_line.insert(polygon_line.begin() + poly_idx_max_area + 1,
                        new_point);
  } else {
    polygon_line.insert(polygon_line.begin() + poly_idx_min_area + 1,
                        new_point);
  }
}

void run_incremental_algo(Polygon_2 &polygon_line, Polygon_2 &convex_hull,
                          std::vector<Point_2> &points,
                          INIT_OPTION_INCR initialization,
                          VISIBLE_EDGE_CHOICE visible_edge_choice) {
  merge_sort(points, 0, points.size() - 1, initialization);

  for (int i = 0; i < 3; ++i) {
    convex_hull.push_back(points[i]);
    polygon_line.push_back(points[i]);
  }

  for (int i = 3; i < points.size(); ++i) {
    Point_2 new_point = points[i];
    std::vector<Segment_2> red_segments;

    expand_convex_hull(convex_hull, red_segments, new_point);
    expand_polygon_line(polygon_line, red_segments, new_point,
                        visible_edge_choice);
  }
}
