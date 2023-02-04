#pragma once

#include "common.hpp"
#include "utils.hpp"

/**
 * @brief                   Implementation of the onion algorithm for
 *                          point-set polygonization
 *
 * @param polygon_line      The polygon line formed by the algorithm
 * @param convex_hull       The convex hull that includes the polygon line
 * @param points            The point-set to polygonize
 * @param initialization    Initialization option for the algorithm
 */
void onion_algo(Polygon_2 &polygon_line, Polygon_2 &convex_hull,
                std::vector<Point_2> &points, INIT_OPTION_ONION initialization);