#ifndef _XML_H_
#define _XML_H_

/*********************************************/

#include "ViewSystem/View.h"
#include "RapidXml/rapidxml.hpp"

/*********************************************/

class Xml
{
public:
	View* LoadView(std::string filename);
	View* HandleNode(rapidxml::xml_node<>* view, View* parent);
};

/*********************************************/

#endif
