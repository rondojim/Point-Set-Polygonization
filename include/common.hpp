#pragma once

#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Convex_hull_traits_adapter_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Line_2.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_2_algorithms.h>
#include <CGAL/Polygon_set_2.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Simple_polygon_visibility_2.h>
#include <CGAL/Triangulation_2.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/intersections.h>
#include <CGAL/property_map.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <algorithm>
#include <numeric>
#include <vector>

typedef long long ll;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point_2;
typedef CGAL::Line_2<K> Line_2;
typedef CGAL::Segment_2<K> Segment_2;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef CGAL::Polygon_set_2<K> Polygon_set_2;

typedef CGAL::Convex_hull_traits_adapter_2<
    K, CGAL::Pointer_property_map<Point_2>::type>
    Convex_hull_traits_2;

const int MAXBUF = 1024;

// Initialisation option for incremental algorithm
enum INIT_OPTION_INCR { X_DESCENDING, X_ASCENDING, Y_DESCENDING, Y_ASCENDING };

// Possible choice for next visible edge
enum VISIBLE_EDGE_CHOICE { RANDOM_EDGE, MIN_AREA, MAX_AREA };

// Algorithm choice
enum ALGORITHM_CHOICE { INCREMENTAL_ALGO, CONVEX_HULL_ALGO, ONION_ALGO };

// Initialisation option for onion algorithm
enum INIT_OPTION_ONION {
  RANDOM_POINT,
  MIN_X_POINT,
  MAX_X_POINT,
  MIN_Y_POINT,
  MAX_Y_POINT
};