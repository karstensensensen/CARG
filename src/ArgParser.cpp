#include "pch.h"
#include "ArgParser.h"
#include "Exceptions.h"

namespace carg
{

	const std::regex Args::m_natural_regex = std::regex("^(-?[0-9]+)$");
	const std::regex Args::m_real_regex = std::regex("^(-?[0-9]+)(\\.?[0-9]+)?$");
	const std::array<std::regex, 2> Args::m_bool_regex = { std::regex("^[f/F]alse$"), std::regex("^[t/T]rue$") };

	long long Args::getNumber(size_t index) const
	{
		if (!isNumber(index))
			throw NotANatural(getArg(index));
		
		return std::stoll(getArg(index));
	}

	bool Args::isNumber(size_t index) const
	{
		return std::regex_match(getArg(index), m_natural_regex);
	}

	double Args::getReal(size_t index) const
	{
		if (!isReal(index))
			throw NotAReal(getArg(index));

		return std::stod(getArg(index));
	}

	bool Args::isReal(size_t index) const
	{
		return std::regex_match(getArg(index), m_real_regex);
	}

	bool Args::getBool(size_t index) const
	{
		if(!isBool(index)) throw NotABool(getArg(index));

		return (bool)getOption(m_bool_regex, index);
	}
	
	bool Args::isBool(size_t index) const
	{
		return hasOption(m_bool_regex, index);
	}

	size_t Args::getOption(const std::vector<std::regex>& regexes, size_t index) const
	{
		const std::string& arg = getArg(index);
		for (size_t i = 0; i < regexes.size(); i++)
			if (std::regex_match(arg, regexes[i])) return i;

		throw NoValidOption(arg);
	}

	size_t Args::getOptionS(const std::vector<std::string>& regexes, size_t index) const
	{
		const std::string& arg = getArg(index);
		for (size_t i = 0; i < regexes.size(); i++)
			if (std::regex_match(arg, std::regex(regexes[i]))) return i;

		throw NoValidOption(arg);
	}

	bool Args::hasOption(const std::vector<std::regex>& regexes, size_t index) const
	{
		const std::string& arg = getArg(index);
		for (const auto& regex: regexes)
			if (std::regex_match(arg, regex)) return true;
		return false;
	}

	bool Args::hasOptionS(const std::vector<std::string>& regexes, size_t index) const
	{
		const std::string& arg = getArg(index);
		for (const auto& regex : regexes)
			if (std::regex_match(arg, std::regex(regex))) return true;
		return false;
	}

	ArgParser::ArgParser(const std::vector<std::string>& keys)
	{
		for (const std::string& key : keys)
		{
			m_arg_map.emplace(key, ArgProp());
		}
	}

	void ArgParser::addArg(std::string key, ArgProp properties)
	{
		m_arg_map.emplace(key, properties);
	}

	void ArgParser::removeArg(std::string key)
	{
		m_arg_map.erase(key);
	}

	ParsedArgs ArgParser::parseArgs(int argc, char** argv)
	{
		int arg = 1;

		ParsedArgs parsed_args;

		for (const auto& arg_pair : m_arg_map)
			parsed_args.emplace(arg_pair.first, std::vector<Args>());

		for(; arg < argc; arg++)
		{
			bool found_key = false;
			for (const auto& [key, props]: m_arg_map)
			{
				std::regex reg_test("^" + key);
				if (std::regex_search(argv[arg], reg_test))
				{
					std::vector<std::string> args;
					args.reserve(props.argCount());
					if (props.seperator() == ' ')
					{
						for (int i = 0; i < props.argCount(); i++)
						{
							arg++;

							if (arg == argc) throw NotEnoughArgs(key, props.argCount(), i);

							for (const auto& [other_key, _] : m_arg_map)
								if(argv[arg] == other_key)  throw NotEnoughArgs(key, props.argCount(), i);

							args.push_back(argv[arg]);
						}
					}
					else
					{
						char* argv_begin = argv[arg];
						char* argv_end = argv[arg] + strlen(argv[arg]);

						if (std::count(argv_begin, argv_end, props.seperator()) != props.argCount())
						{
							throw NotEnoughArgs(key, props.argCount(), (int) std::count(argv_begin, argv_end, props.seperator()));
						}

						for (char* i = argv_begin + key.size(); i < argv_end; i++)
						{
							i = strchr(i, props.seperator()) + 1;
							char* next_i = strchr(i, props.seperator());
							next_i = next_i != nullptr ? next_i : argv_end;
							args.push_back(std::string(i, next_i));
							i = next_i;
						}

					}

					size_t args_indx = parsed_args[key].size();
					parsed_args[key].push_back(Args(std::move(args)));

					for (int i = 0; i < props.argCount(); i++)
					{
						const Args& args_elem = parsed_args[key][args_indx];

						// check if arguments passed has the correct types
						switch (props.argType(i))
						{
						case CMDTypes::Natural:
							if (!args_elem.isNumber(i)) throw NotANatural(args_elem.getArg(i));
							break;
						case CMDTypes::Real:
							if (!args_elem.isReal(i)) throw NotAReal(args_elem.getArg(i));
							break;
						case CMDTypes::Bool:
							if (!args_elem.isBool(i)) throw NotABool(args_elem.getArg(i));
							break;
						}
					}

					found_key = true;
					break;
				}
			}

			if(!found_key)
				if (m_arg_map.find(" ") == m_arg_map.end())
					throw NoKeyFound(argv[arg]);
				else
					parsed_args[" "].push_back(Args({argv[arg]}));

		}

		// check if the number of keys are within their instance ranges
		for (const auto& [key, args] : parsed_args)
		{
			if (m_arg_map[key].instanceRange().first > args.size() || m_arg_map[key].instanceRange().second < args.size())
				throw InvalidInstanceCount(key, m_arg_map[key].instanceRange(), (int) args.size());
		}

		return parsed_args;
	}



}
