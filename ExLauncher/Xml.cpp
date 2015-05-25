#include "Xml.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <exception>
#include "Views/LinearPanel.h"
#include "Views/GridView.h"
#include "Views/TabPanel.h"
#include "Views/Selector.h"
#include "Views/State.h"
#include "Views/Image.h"
#include "Views/Label.h"
#include "ThemeManager.h"

using namespace rapidxml;
using namespace std;

View* Xml::LoadView(string filename)
{
	xml_document<> doc;
	ifstream file(filename.c_str());
	stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content(buffer.str());
	View* view = NULL;

	if (content.size() == 0)
		throw runtime_error("not a valid xml file");

	doc.parse<0>(&content[0]);

	xml_node<> * startNode = NULL;
	startNode = doc.first_node();

	if (startNode == NULL)
		throw runtime_error("not a valid Screen");

	view = HandleNode(startNode, NULL);

	return view;
}

View* Xml::HandleNode(xml_node<>* view, View* parent)
{
	View* createdView = NULL;

	if (strcmp(view->name(), "Image") == 0)
	{
		createdView = new Image();
	}
	else if (strcmp(view->name(), "Label") == 0)
	{
		createdView = new Label();
	}
	else if (strcmp(view->name(), "LinearPanel") == 0)
	{
		createdView = new LinearPanel();
	}
	else if (strcmp(view->name(), "GridView") == 0)
	{
		createdView = new GridView();
	}
	else if (strcmp(view->name(), "FramePanel") == 0)
	{
		createdView = new FramePanel();
	}
	else if (strcmp(view->name(), "TabPanel") == 0)
	{
		createdView = new TabPanel();
	}
	else if (strcmp(view->name(), "Selector") == 0)
	{
		createdView = new Selector();
	}
	else if (strcmp(view->name(), "State") == 0)
	{
		createdView = new State();
	}
	else if (strcmp(view->name(), "Item") == 0)
	{
		View* itemTemplate = parent->GetItemTemplate();
		if (itemTemplate == NULL)
			throw runtime_error("item found but no item template");

		createdView = itemTemplate->Copy();
	}
	else
	{
		string msg = "invalid view '";
		msg += view->name();
		msg += "'";

		throw runtime_error(msg.c_str());
	}

	// Parse all arguments and set as properties on the view
	for (const rapidxml::xml_attribute<>* attrib = view->first_attribute(); attrib; attrib = attrib->next_attribute())
	{
		string name = attrib->name();
		string value = attrib->value();

		bool attributeHandled = createdView->SetProperty(name, value);

		if (name == "itemTemplate")
		{
			if (createdView->GetItemTemplate() != NULL)
				throw runtime_error("cannot have multiple item templates");

			string templatePath = ThemeManager::ProcessPath(value);
			createdView->SetItemTemplate(LoadView(templatePath));
			continue;
		}

		if (!attributeHandled)
		{
			string errorMsg = "invalid attribute: ";
			errorMsg = errorMsg + name;

			throw runtime_error(errorMsg.c_str());
		}
	}

	if (createdView == NULL)
	{
		throw runtime_error("view could not be created");
	}

	if (parent != NULL)
		parent->AddChildView(createdView);

	for (xml_node<> * childView = view->first_node(); childView; childView = childView->next_sibling())
	{
		HandleNode(childView, createdView);
	}

	return createdView;
}
