#include "stdafx.h"

#include "HtmlForm.hpp"

#pragma warning ( push )
#pragma warning( disable : 4996 )
#include <boost/algorithm/string.hpp>
#pragma warning ( pop )

#include <vector>

namespace LHP {

HtmlForm::HtmlForm(const Server::Request &req)
{
	std::vector<std::string> tokens;
	boost::split(tokens,req.PostData,boost::is_any_of("&"));

	for (std::vector<std::string>::iterator iter = tokens.begin();iter!=tokens.end();++iter) {
		std::vector<std::string> nv;
		boost::split(nv,(*iter),boost::is_any_of("="));
		_nameValues[UriDecode(nv[0])] = nv.size()>1 ? UriDecode(nv[1]) : "";
	}
}

std::string HtmlForm::GetValue(const std::string &name) const
{
	auto np = _nameValues.find(name);
	if (np!=_nameValues.end()) {
		return np->second;
	}
	else {
		return "";
	}
}

bool HtmlForm::HasValue(const std::string &name) const
{
	return _nameValues.find(name)!=_nameValues.end();
}

const char HEX2DEC[256] = 
{
    /*       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F */
    /* 0 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 1 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 2 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 3 */  0, 1, 2, 3,  4, 5, 6, 7,  8, 9,-1,-1, -1,-1,-1,-1,
    
    /* 4 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 5 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 6 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 7 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    
    /* 8 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 9 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* A */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* B */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    
    /* C */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* D */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* E */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* F */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1
};

std::string HtmlForm::UriDecode(const std::string & sSrc) const
{
   // Note from RFC1630: "Sequences which start with a percent
   // sign but are not followed by two hexadecimal characters
   // (0-9, A-F) are reserved for future extension"

   const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
   const int SRC_LEN = sSrc.length();
   const unsigned char * const SRC_END = pSrc + SRC_LEN;
   // last decodable '%' 
   const unsigned char * const SRC_LAST_DEC = SRC_END - 2;

   char * const pStart = new char[SRC_LEN];
   char * pEnd = pStart;

   while (pSrc < SRC_LAST_DEC)
   {
	  if (*pSrc == '%')
	  {
		 char dec1, dec2;
		 if (-1 != (dec1 = HEX2DEC[*(pSrc + 1)])
			&& -1 != (dec2 = HEX2DEC[*(pSrc + 2)]))
		 {
			*pEnd++ = (dec1 << 4) + dec2;
			pSrc += 3;
			continue;
		 }
	  }

	  *pEnd++ = *pSrc++;
   }

   // the last 2- chars
   while (pSrc < SRC_END)
	  *pEnd++ = *pSrc++;

   std::string sResult(pStart, pEnd);
   delete [] pStart;
   return sResult;
}



}