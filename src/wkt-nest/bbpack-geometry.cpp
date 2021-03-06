#include "wkt-nest/bbpack-geometry.hpp"

#include <boost/geometry.hpp>

namespace bg = boost::geometry;

using namespace bbpack::geometry;

flt_t bbpack::geometry::area(const box_t& b) {
  return bg::area(b);
}
flt_t bbpack::geometry::area(const polygon_t& b) {
  return bg::area(b);
}

void bbpack::geometry::centroid(const box_t& b, point_t& c) {
  return bg::centroid(b, c);
}
const bg::strategy::centroid::bashein_detmer<point_t,point_t,crd_t> centroid_strategy;
void bbpack::geometry::centroid(const polygon_t& b, point_t& c) {
  return bg::centroid(b, c, centroid_strategy);
}

bool bbpack::geometry::intersects(const box_t& i1, const box_t& i2) {
  return bg::intersects(i1, i2);
}
bool bbpack::geometry::intersects(const box_t& i1, const polygon_t& i2) {
  return bg::intersects(i1, i2);
}
bool bbpack::geometry::intersects(const polygon_t& i1, const polygon_t& i2) {
  return bg::intersects(i1, i2);
}

polygon_t bbpack::geometry::buffer(const polygon_t& p, crd_t buffer_distance) {
  if (buffer_distance==0)
    return p;

  bg::model::multi_polygon<polygon_t> mpol;
  // Declare strategies
  const int points_per_circle = 8;
  bg::strategy::buffer::distance_symmetric<crd_t> distance(buffer_distance);
  boost::geometry::strategy::buffer::join_round join(points_per_circle);
  //boost::geometry::strategy::buffer::join_miter join;
  boost::geometry::strategy::buffer::end_flat end;
  boost::geometry::strategy::buffer::point_circle circle(points_per_circle);
  boost::geometry::strategy::buffer::side_straight side;
  bg::buffer(p, mpol, distance, side, join, end, circle);
  polygon_t r;
  bg::convert(mpol[0].outer(), r);
  return r;
}

polygon_t bbpack::geometry::convert(const box_t& b) {
  polygon_t p;
  bg::convert(b, p);
  return p;
}

polygon_t bbpack::geometry::union_(const polygon_t& p1, const polygon_t& p2) {
  bg::model::multi_polygon<polygon_t> mpol;
  bg::union_(p1, p2, mpol);
  if (mpol.size()==0)
    return p2;
  polygon_t r;
  bg::convert(mpol[0].outer(), r);
  return r;
}

flt_t bbpack::geometry::union_area(const polygon_t& p1, const polygon_t& p2) {
  bg::model::multi_polygon<polygon_t> mpol;
  bg::union_(p1, p2, mpol);
  return bg::area(mpol);
}

polygon_t bbpack::geometry::convex_hull(const polygon_t& p1, const polygon_t& p2) {
  bg::model::multi_polygon<polygon_t> mpol;
  mpol.push_back(p1);
  mpol.push_back(p2);
  polygon_t r;
  bg::convex_hull(mpol, r);
  return r;
}

flt_t bbpack::geometry::distance(const point_t& pnt, const polygon_t& poly) {
  return bg::comparable_distance(pnt, poly);
}
