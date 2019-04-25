#ifndef XMLUTILS_H_
#define XMLUTILS_H_

#include <boost/property_tree/ptree.hpp>
#include <string>

namespace pt = boost::property_tree;

void XmlWrite(pt::ptree& tree, const std::string& filename);

#endif // XMLUTILS_H_
