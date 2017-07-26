#include "harCs.hpp"
#include "Config.hpp"

namespace ConfigMngr
{
	bool Config::loadInitial(std::string const &file)
	{
		

		m_listGroupStack.push_front(this);

		std::ifstream in(file, std::ifstream::in);
		if (!in)
			return false;

		std::string buf;

		while (std::getline(in, buf))
		{
			if ((buf.length() > 2) && (buf[0] != '#') && (buf.find(')') == std::string::npos))
			{
				std::string name;
				std::string value;
				split(buf, name, value, '=');

				for (std::list<Config*>::reverse_iterator it = m_listGroupStack.rbegin(); it != m_listGroupStack.rend(); ++it)
				{
					(*it)->symbolExpand(value);
				}
				envSymbolExpand(value);
				m_listGroupStack.front()->add(name, value);
			}
			if ((buf.length() > 0) && (buf[0] != '#') && (buf.find(')') != std::string::npos))
				m_listGroupStack.pop_front();
		}
		in.close();

		return true;
	}

	void Config::add(std::string name, std::string value)
	{
		m_mapSymbols[name] = value;
	}

	void Config::split(std::string in, std::string &left, std::string &right, char c)
	{
		size_t pos = in.find_first_of(c);

		if (pos == std::string::npos)
		{
			left = in;
			trim(left);
			right = "";
		}
		else if (pos <= 1)
		{
			left = "";
			right = in.substr(pos + 1, std::string::npos);
			trim(right);
		}
		else
		{
			left = in.substr(0, pos - 1);
			right = in.substr(pos + 1, std::string::npos);
			trim(left);
			trim(right);
		}
	}

	void Config::trim(std::string &s)
	{
		while ((s.length() > 1) && ((s[0] == ' ') || (s[0] == '\t')))
			s = s.substr(1, std::string::npos);

		while ((s.length() > 1) &&
			((s[s.length() - 1] == ' ') ||
				(s[s.length() - 1] == '\t') ||
				(s[s.length() - 1] == '\n') ||
				(s[s.length() - 1] == '\r')))
			s = s.substr(0, s.length() - 1);

		if ((s.length() > 1) && (s[0] == '"'))
			s = s.substr(1, std::string::npos);

		if ((s.length() > 1) && (s[s.length() - 1] == '"'))
			s = s.substr(0, s.length() - 1);
	}

	void Config::symbolExpand(std::string &s)
	{
		symbolExpand(m_mapSymbols, s);
	}

	void Config::envSymbolExpand(std::string &s)
	{
		symbolExpand(m_mapEnvSymbols, s);
	}

	void Config::symbolExpand(std::map<std::string, std::string> &symbols, std::string &s)
	{
		bool expanded;

		do
		{
			expanded = false;

			for (std::map<std::string, std::string>::iterator it = m_mapSymbols.begin(); it != m_mapSymbols.end(); ++it)
			{
				std::string search = "%" + it->first + "%";
				std::string replace = it->second;

				size_t pos = s.find(search);

				if (pos != std::string::npos)
				{
					expanded = true;
					s.replace(pos, search.length(), replace);
				}
			}

		} while (expanded);
	}

	std::string Config::getString(std::string const &name)
	{
		auto it = m_mapSymbols.find(name);
		if (it == m_mapSymbols.end())
			return "0";

		return it->second;
	}

	bool Config::getBool(std::string const &name)
	{
		auto res = getString(name);

		if ((res == "yes") ||
			(res == "Yes") ||
			(res == "YES") ||
			(res == "true") ||
			(res == "True") ||
			(res == "TRUE"))
			return true;

		return false;
	}

	int Config::getInt(std::string const &name)
	{
		auto res = getString(name);

		return atoi(res.c_str());
	}

	float Config::getFloat(std::string const &name)
	{
		auto res = getString(name);

		return std::stof(res.c_str());
	}
}