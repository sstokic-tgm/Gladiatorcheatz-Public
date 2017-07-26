#pragma once

#include <string>
#include <map>
#include <list>

namespace ConfigMngr
{
	class Config
	{

	public:

		Config() = default;
		~Config() = default;

		bool loadInitial(std::string const &file);

		std::string getString(std::string const &name);
		bool getBool(std::string const &name);
		int getInt(std::string const &name);
		float getFloat(std::string const &name);

		inline std::map<std::string, std::string> &getSymbols()
		{
			return m_mapSymbols;
		}

	private:

		void add(std::string name, std::string value);
		void split(std::string in, std::string &left, std::string &right, char c);
		void trim(std::string &s);
		void symbolExpand(std::string &s);
		void symbolExpand(std::map<std::string, std::string> &symbols, std::string &s);
		void envSymbolExpand(std::string &s);

		std::map<std::string, std::string> m_mapSymbols;
		std::map<std::string, std::string> m_mapEnvSymbols;
		std::list<Config*> m_listGroupStack;
	};
}