#include "BridgeConfig.h"
#include "Common/Log.h"

#include <string>
#include <sstream>

#include <strings.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xmlsave.h>

namespace
{
  DSB_DECLARE_LOGNAME(BridgeConfig);

  inline xmlChar const* toXmlChar(char const* s)
  {
    return reinterpret_cast<xmlChar const *>(s);
  }

  inline char const* toChar(xmlChar const* s)
  {
    return reinterpret_cast<char const *>(s);
  }

  inline xmlChar const* toXmlChar(bool b)
  {
    return b ? toXmlChar("true") : toXmlChar("false");
  }

  char const* kAlljoynObjectPathPrefix = "/BridgeConfig/AdapterDevices/Device[@Id=\"";
  char const* kAdapterDevices = "/BridgeConfig/AdapterDevices";
  char const* kAlljoynObjectPathSuffix = "\"]";
  char const* kDeviceElementName = "Device";
  char const* kIdAttribute = "Id";
  char const* kVisibleAttribute = "Visible";

  std::string MakePathId(std::string const& id)
  {
    std::stringstream buff;
    buff << kAlljoynObjectPathPrefix;
    buff << id;
    buff << kAlljoynObjectPathSuffix;
    return buff.str();
  }

  xmlChar* GetNodeAttribute(const xmlNodePtr node, char const* attr)
  {
    if (node)
    {
      xmlAttr* attrs = node->properties;
      while (attrs && attrs->name && attrs->children)
      {
        if (strcmp(toChar(attrs->name), attr) == 0)
          return xmlNodeListGetString(node->doc, attrs->children, 1);
        attrs = attrs->next;
      }
    }
    return NULL;
  }

  bool ToBoolean(std::string const& s)
  {
    return strcasecmp(s.c_str(), "true") == 0 ? true : false;
  }

  xmlNodePtr SelectSingleNode(xmlXPathContextPtr ctx, char const* xpath)
  {
    xmlNodePtr node = NULL;

    DSBLOG_INFO("select: %s", xpath);

    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(toXmlChar(xpath), ctx);
    if (xpathObj)
    {
      int n = 0;
      if (xpathObj->nodesetval)
        n = xpathObj->nodesetval->nodeNr;

      if (n == 1)
        node = xpathObj->nodesetval->nodeTab[0];
    }

    return node;
  }

  std::string GetElementText(const xmlDocPtr doc, char const* xpath)
  {
    xmlXPathContextPtr ctx = xmlXPathNewContext(doc);
    if (!ctx)
      return NULL;

    std::string content;

    xmlNodePtr node = SelectSingleNode(ctx, xpath);
    if (node)
    {
      xmlChar* s = xmlNodeGetContent(node);
      content = std::string(toChar(s));
    }

    xmlXPathFreeContext(ctx);
    DSBLOG_INFO("%s == %s", xpath, content.c_str());
    return content;
  }
}

Bridge::BridgeConfig::BridgeConfig()
  : m_doc(NULL)
{
}

Bridge::BridgeConfig::~BridgeConfig()
{
  if (m_doc)
    xmlFreeDoc(m_doc);
}

QStatus Bridge::BridgeConfig::FromFile(std::string const& fileName)
{
  QStatus st = ER_OK;

  m_fileName = fileName;

  xmlInitParser();

  xmlDocPtr doc = xmlParseFile(m_fileName.c_str());
  if (!doc)
  {
    DSBLOG_WARN("failed to parse: %s", m_fileName.c_str());
    st = ER_FAIL;
  }
  if (m_doc)
    xmlFreeDoc(m_doc);

  m_doc = doc;
  return ER_OK;
}

QStatus Bridge::BridgeConfig::ToFile(std::string const& fileName)
{
  if (!m_doc)
    return ER_FAIL;

  char const* outfile = m_fileName.c_str();
  if (!fileName.empty())
    outfile = fileName.c_str();

  // TODO: use TextWriter to actually indent this stuff
//  xmlSaveCtxtPtr ctx = xmlSaveToFilename(outfile, NULL, XML_SAVE_AS_HTML);
//  xmlSaveDoc(ctx, m_doc);
//  xmlSaveClose(ctx);
  xmlKeepBlanksDefault(1);
  xmlSaveFormatFile(outfile, m_doc, 1);

  return ER_OK;
}

QStatus Bridge::BridgeConfig::ToString(std::string& out)
{
  xmlBufferPtr buff = xmlBufferCreate();

  // TODO: use TextWriter to actually indent this stuff
  xmlSaveCtxtPtr ctx = xmlSaveToBuffer(buff, NULL, XML_SAVE_FORMAT);
  xmlSaveDoc(ctx, m_doc);
  xmlSaveClose(ctx);

  out = std::string(toChar(buff->content));
  xmlBufferFree(buff);

  return ER_OK;
}

QStatus Bridge::BridgeConfig::AddObject(DsbObjectConfig const& obj)
{
  xmlXPathContextPtr ctx = xmlXPathNewContext(m_doc);
  if (!ctx)
    return ER_FAIL;

  xmlNodePtr parent = SelectSingleNode(ctx, kAdapterDevices);
  if (parent)
  {
    xmlNodePtr node = xmlNewTextChild(parent, NULL,  toXmlChar(kDeviceElementName),
      toXmlChar(obj.GetDescription().c_str()));
    xmlNewProp(node, toXmlChar(kIdAttribute), toXmlChar(obj.GetId().c_str()));
    xmlNewProp(node, toXmlChar(kVisibleAttribute), toXmlChar(obj.IsVisible()));
  }

  xmlXPathFreeContext(ctx);
  return ER_OK;
}

QStatus Bridge::BridgeConfig::RemoveObject(std::string const& id)
{
  xmlXPathContextPtr ctx = xmlXPathNewContext(m_doc);
  if (!ctx)
    return ER_FAIL;

  xmlNodePtr node = SelectSingleNode(ctx, MakePathId(id).c_str());
  if (node)
  {
    xmlUnlinkNode(node);
    xmlFreeNode(node);
  }

  xmlXPathFreeContext(ctx);
  return ER_OK;
}

QStatus Bridge::BridgeConfig::FindObject(std::string const& id, DsbObjectConfig& obj)
{
  if (!m_doc)
    return ER_FAIL;

  xmlXPathContextPtr ctx = xmlXPathNewContext(m_doc);
  if (!ctx)
    return ER_FAIL;

  std::string path = MakePathId(id);
  xmlNodePtr node = SelectSingleNode(ctx, path.c_str());
  if (!node)
    return ER_BUS_OBJ_NOT_FOUND;

  obj.SetId(path);

  xmlChar* attr = GetNodeAttribute(node, kVisibleAttribute);
  if (attr)
  {
    bool b = strcasecmp(toChar(attr), "true") == 0
      ? true
      : false;
    obj.SetIsVisible(b);
    xmlFree(attr);
  }

  xmlChar* desc = xmlNodeGetContent(node);
  obj.SetDescription(std::string(toChar(desc)));

  xmlXPathFreeContext(ctx);
  return ER_OK;
}

std::string Bridge::BridgeConfig::GetBridgeKeyX() const
{
  return GetElementText(m_doc, "/BridgeConfig/Settings/Bridge/KEYX");
}

std::string Bridge::BridgeConfig::GetDeviceKeyX() const
{
  return GetElementText(m_doc, "/BridgeConfig/Settings/Device/KEYX");
}

std::string Bridge::BridgeConfig::GetDeviceUsername() const
{
  return GetElementText(m_doc, "/BridgeConfig/Settings/Device/USERNAME");
}

std::string Bridge::BridgeConfig::GetDevicePassword() const
{
  return GetElementText(m_doc, "/BridgeConfig/Settings/Device/PASSWORD");
}

std::string Bridge::BridgeConfig::GetDeviceEcdheEcdsaPrivateKey() const
{
  return GetElementText(m_doc, "/BridgeConfig/Settings/Device/ECDHEECDSAPRIVATEKEY");
}

std::string Bridge::BridgeConfig::GetDeviceEcdheEcdsaCertChain() const
{
  return GetElementText(m_doc, "/BridgeConfig/Settings/Device/ECDHEECDSACERTCHAIN");
}

bool Bridge::BridgeConfig::GetDefaultVisibility() const
{
  std::string s = GetElementText(m_doc, "/BridgeConfig/Settings/Device/DefaultVisibility");
  return ToBoolean(s);
}

