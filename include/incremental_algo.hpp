#pragma once
#include "common.hpp"
#include "utils.hpp"

/**
 * @brief                           Implements the incremental algorithm for
 *                                  point-set polygonization
 *
 * @param polygon_line              Polygon line formed by the incremental algo
 * @param convex_hull               Convex hull formed by the incremental algo
 * @param points                    The point-set given
 * @param initialization            Option for how to sort the point-set
 * @param visible_edge_selection    Option for how to choose the visible edge
 */
void run_incremental_algo(Polygon_2 &polygon_line, Polygon_2 &convex_hull,
                          std::vector<Point_2> &points,
                          INIT_OPTION_INCR initialization,
                          VISIBLE_EDGE_CHOICE visible_edge_selection);