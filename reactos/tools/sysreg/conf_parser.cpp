/* $Id$
 *
 * PROJECT:     System regression tool for ReactOS
 * LICENSE:     GPL - See COPYING in the top level directory
 * FILE:        tools/sysreg/conf_parser.h
 * PURPOSE:     configuration parser
 * PROGRAMMERS: Johannes Anderwald (johannes.anderwald at sbox tugraz at)
 */

#include "conf_parser.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>

namespace Sysreg_
{
	using std::ifstream;
//---------------------------------------------------------------------------------------
	ConfigParser::ConfigParser()
	{

	}

//---------------------------------------------------------------------------------------
	ConfigParser::~ConfigParser()
	{
		m_Map.clear ();
	}

//---------------------------------------------------------------------------------------
	bool ConfigParser::parseFile(char * FileName)
	{
		FILE * file = fopen(FileName, "rt");
		if (!file)
		{
			cerr << "Error: ConfigParser::parseFile failed to open configuration file " << FileName << endl;
			return false;
		}
		bool ret = false;
		while (!feof(file))
		{
			char buffer[500];
			char * buf;

			buf = fgets(buffer, sizeof(buffer) / sizeof(char), file);
			if (buf)
			{
				if (buffer[0] != ';')
				{
					string s_buffer = string(buffer);
					string::size_type ws_pos = s_buffer.find_first_of ("=");

					if (ws_pos != string::npos && ws_pos > 0 && ws_pos < s_buffer.size())
					{
						string name = s_buffer.substr (0, ws_pos);
						string value = s_buffer.substr (ws_pos + 1, s_buffer.size() - ws_pos - 2);
/*
						if (value[value.length () -1] == 0xA)
						{
							// remove newline char
							value[value.length ()-1] = '\0';
						}
*/
//                        cerr << "name: "<< name << "value: "<< value << "|" << endl;
						m_Map[name] = value;
						ret = true;
					}
				}
			}
		}
		fclose(file);
		return ret;
	}

//---------------------------------------------------------------------------------------
	bool ConfigParser::getStringValue(string &ConfVariable, string &ConfValue)
	{
		ConfigMap::iterator it = m_Map.find (ConfVariable);
		if (it == m_Map.end ())
		{
			//cerr << "ConfigParser::getValue failed to find " << ConfVariable << endl;
			return false;
		}
		
		ConfValue = it->second;
		return true;
	}
//----------------------------------------------------------------------------------------
    bool ConfigParser::getDoubleValue(string ConfVariable, double & ConfValue)
    {
		ConfigMap::iterator it = m_Map.find (ConfVariable);
		if (it == m_Map.end ())
		{
			cerr << "ConfigParser::getValue failed to find " << ConfVariable << endl;
			return false;
		}

        ConfValue = strtod(it->second.c_str(), NULL);
        return true;
    }
//-----------------------------------------------------------------------------------------
    bool ConfigParser::getIntValue(string ConfVariable, long int & ConfValue)
    {
		ConfigMap::iterator it = m_Map.find (ConfVariable);
		if (it == m_Map.end ())
		{
			cerr << "ConfigParser::getValue failed to find " << ConfVariable << endl;
			return false;
		}

        ConfValue = strtol(it->second.c_str(), NULL, 10);
        return true;
    }

} // end of namespace Sysreg_
