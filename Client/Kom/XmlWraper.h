#pragma once
#include <libxml/tree.h>
#include <libxml/parser.h>

class XMLDocument
{
public:
	XMLDocument()
		:doc(NULL)
	{
	}

	~XMLDocument()
	{
		if(doc)
		{
			xmlFreeDoc(doc);
			doc = NULL;
		}
	}

	void ReadMemory(const char* data,int length)
	{
		if(doc)
		{
			xmlFreeDoc(doc);
			doc = NULL;
		}

		doc = xmlReadMemory(data,length,NULL,NULL,0);
	}

	bool IsOpen(){return doc != NULL;}

	xmlNode * GetRootElement(){return xmlDocGetRootElement(doc);}
protected:
	xmlDoc * doc;
};