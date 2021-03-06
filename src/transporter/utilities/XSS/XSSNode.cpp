#include "transporter.h"

XSSNode::XSSNode( str key,XSSNode* parent,str value,str type, str ndx )
{
	this->parent = parent;
	this->key = key;
	this->value = value;
	this->ndx = ndx;
	this->type = type;
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

str XSSNode::getKey() const
{
	return this->key;
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

str XSSNode::getValue() const
{
	return this->value;
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

str XSSNode::getNdx() const
{
	return this->ndx;
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

str XSSNode::getType() const
{
	return this->type;
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

i32 XSSNode::getValueAsInt() const
{
	return StrToInt(this->value);
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

f32 XSSNode::getValueAsFloat() const
{
	return StrToFloat(this->value);
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

bit XSSNode::getValueAsBool() const
{
	if( value == "NO" || value == "NULL" || value == "FALSE" ||
		value == "no" || value == "null" || value == "false" ||
		value == "0"  || value == "nil"  || value.empty())
	{
		return false;
	}
	else
	{
		return true;
	}
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

void XSSNode::setValue( str value )
{
	this->value = value;
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

void XSSNode::addConfig( xss_item item, str parentStr )
{
	if(xss_count(item))
	{
		str key = xss_arg(item);
		if(!parentStr.empty())
		{
			key = parentStr+"."+xss_arg(item);
		}

		addConfigNode(item,key);
		for(u32 i=0 ; i<xss_count(item) ; i++)
		{
			addConfig(xss_child(item,i),key);
		}
	}
	else
	{
		str varName = str(xss_arg(item));
		str key = varName;
		if(!parentStr.empty())
		{
			key = parentStr+"."+varName;
		}		

		XSSNode* config = new XSSNode(key,this, str(xss_value(item)),
												str(xss_type(item)),
												str(xss_ndx(item)));
		configs.push_back(config);
		configMap[key] = config;
	}
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

XSSNode* XSSNode::operator[]( str keyName ) const
{
	return get(keyName);
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

XSSNode* XSSNode::get( str keyName,bool upRecursive ) const
{
	str keyStr = keyName;
	XSSNode* nodeParent = this->parent;
	do 
	{
		std::map<str,XSSNode*>::const_iterator it = configMap.find(keyStr);
		if(it != configMap.end())
		{	
			return it->second;
		}	

		str upkey = strupr((char*)keyStr.c_str());
		it = configMap.find(upkey);
		if(it != configMap.end())
		{	
			return it->second;
		}

		str lowkey = strlwr((char*)keyStr.c_str());
		it = configMap.find(lowkey);
		if(it != configMap.end())
		{	
			return it->second;
		}
		return NULL;

		if(nodeParent)
		{
			keyStr = nodeParent->getKey()+"."+keyStr;
			nodeParent = nodeParent->getParent();
		}
	} 
	while (nodeParent && upRecursive);
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

void XSSNode::addConfigNode( xss_item item, str nodeKey )
{
	if(xss_count(item))
	{
		assert(!nodeKey.empty());
		XSSNode* config = new XSSNode(nodeKey,this,
			                          str(xss_value(item)),
									  str(xss_type(item)),
									  str(xss_ndx(item)));
		configs.push_back(config);
		configMap[nodeKey] = config;

		for(u32 i=0 ; i<xss_count(item) ; i++)
		{
			config->addConfig(xss_child(item,i),"");
		}
	}
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

XSSNode* XSSNode::getChild( u32 index ) const
{
	if(index < configs.size())
	{
		return configs.at(index);
	}
	return NULL;
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

u32 XSSNode::countChild() const
{
	return configs.size();
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

XSSNode* XSSNode::getParent() const
{
	return parent;
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

bit XSSNode::loadConfig( str fileName )
{
	xss cfg = xss_loadfrom_file(NULL,(i8*)fileName.c_str());
	if(cfg)
	{
		for(u32 i=0 ; i<xss_count(cfg) ; i++)
		{
			addConfig(xss_child(cfg,i),"");
		}
		xss_destroy(cfg);
		return true;
	}
	return false;
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

void XSSNode::clearConfig()
{
	std::vector<XSSNode*>::iterator it = configs.begin();
	for(it ; it!=configs.end() ; it++)
	{
		XSSNode* cfg = *it;	
		str key = cfg->getKey();
		assert(!key.empty());
		configMap.erase(key);
		delete cfg;
	}
	configs.clear();
	configMap.clear();
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

XSSNode::~XSSNode()
{
	clearConfig();
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

XSSNode::XSSNode()
{
	this->parent = NULL;
	this->key = "";
	this->value = "";
	this->type = "";
	this->ndx = "";
}