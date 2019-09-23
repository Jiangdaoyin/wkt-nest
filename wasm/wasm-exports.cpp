#include <emscripten.h>

#include <cstdlib>
#include <string>
#include <sstream>

#include <boost/geometry/io/svg/svg_mapper.hpp>
#include "wkt-nest/wktio.hpp"
#include "wkt-nest/bbpack.hpp"

using namespace wktnest;

const size_t r_size = 1048576; // 1MiB
char result[r_size];

extern "C" char* EMSCRIPTEN_KEEPALIVE nest(char* p)
{
  try {
    nesting_opts opts;
    opts.sorting = SORTING::HEIGHT;

    std::stringstream in(p);

    std::optional<box_t> b = read_box(in);
    if (!b) {
      strcpy(result, "error: No bin box provided");
      return &result[0];
    }
    std::vector<polygon_t> ps = read_polygons(in);

    auto fit = bbpack::fit(*b, ps, opts);

    std::stringstream out;
    {
      // Declare a stream and an SVG mapper
      boost::geometry::svg_mapper<point_t> mapper(out, 400, 400);

      // Add geometries such that all these geometries fit on the map
      mapper.add(*b);
      /*
        for (auto item : fit)
        if (item.bin)
        mapper.add(item.bbox);
      */
      mapper.map(*b, "fill-opacity:0.5;fill:rgb(153,204,0);stroke:rgb(153,204,0);stroke-width:2", 5);

      for (auto item : fit)
        if (item.bin) {
          // Draw the geometries on the SVG map, using a specific SVG style
          //mapper.map(item.bbox, "fill-opacity:0.2;fill:rgb(51,51,153);stroke:rgb(51,51,153);stroke-width:1");
          mapper.map(item.polygon, "fill-opacity:0.3;fill:rgb(212,0,0);stroke:rgb(212,0,0);stroke-width:2");
        }

      // Destructor of map will be called - adding </svg>
      // Destructor of stream will be called, closing the file
    }

    return strncpy(result, out.str().c_str(), r_size);
  }
  catch (std::exception &ex)
  {
    strcpy(result, ex.what());
    return result;
  }
}