#include "XmlUtils.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

void xmlWrite(pt::ptree& tree, const std::string& filename) {
  pt::write_xml(filename, tree);
}
