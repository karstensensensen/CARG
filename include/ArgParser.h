#pragma once

namespace carg
{
	 
	// A class containing data passed to a key
	class Args
	{
		std::vector<std::string> m_args;

		static const std::regex m_natural_regex;
		static const std::regex m_real_regex;
		static const std::vector<std::regex> m_bool_regex;

	public:

		Args(const std::vector<std::string>& args) : m_args(args) {}
		Args(std::vector<std::string>&& args) : m_args(std::move(args)) {}

		// Returns the argument at a given index
		const std::string& getArg(size_t index = 0) const{ return m_args[index]; }

		// get = convert argument to type if possible and return the value, else throw 
		// is = check if the argument can be converted

		long long getNumber(size_t index = 0) const;
		bool isNumber(size_t index = 0) const;
		double getReal(size_t index = 0) const;
		bool isReal(size_t index = 0) const;
		bool getBool(size_t index = 0) const;
		bool isBool(size_t index = 0) const;

		// Returns which regex matches the index argument first
		// S = takes std::string as argument instead of std::regex

		size_t getOption(const std::vector<std::regex>& regexes, size_t index = 0) const;
		size_t getOptionS(const std::vector<std::string>& regexes, size_t index = 0) const;
		bool hasOption(const std::vector<std::regex>& regexes, size_t index = 0) const;
		bool hasOptionS(const std::vector<std::string>& regexes, size_t index = 0) const;
	};

	// What types the command line arguments can be converted to
	enum class CMDTypes
	{
		String,
		Natural,
		Real,
		Bool
	};

	// Properties for arguments to a given key
	class ArgProp
	{
		// Checks wether the arguments passed can be converted to the given type
		std::vector<CMDTypes> m_arg_types;
		// What seperates the arguments when passing it to a key
		char m_seperator;
		// Range of possible instances
		std::pair<unsigned int, unsigned int> m_instances;

	public:
		ArgProp(size_t arg_count = 1, CMDTypes type = CMDTypes::String, char seperator = ' ', unsigned int min_instances = 0, unsigned int max_instances = UINT_MAX)
			: m_arg_types(arg_count, type), m_seperator(seperator), m_instances(min_instances, max_instances) {
			assert(min_instances <= max_instances);
		};

		ArgProp(const std::vector<CMDTypes>& arg_types, char seperator = ' ', unsigned int min_instances = 0, unsigned int max_instances = UINT_MAX)
			: m_arg_types(arg_types), m_seperator(seperator), m_instances(min_instances, max_instances) {
			assert(min_instances <= max_instances);
		};

		int argCount()  const{ return (int) m_arg_types.size(); }
		CMDTypes argType(size_t arg_i) const { return m_arg_types[arg_i]; }
		char seperator() const { return m_seperator; }
		std::pair<unsigned int, unsigned int> instanceRange() const{ return m_instances; };
	};

	typedef std::map<std::string, std::vector<Args>> ParsedArgs;

	/*
	* A class that defines how to parse the incoming arguments from the commandline.
	* Use addArg and removeArg to change how arguments will be parsed after construction
	*/
	class ArgParser
	{
		std::map<std::string, ArgProp> m_arg_map;
	public:
		ArgParser() = default;
		// Takes a vector of keys and properties
		ArgParser(const std::map<std::string, ArgProp>& arg_map) : m_arg_map(arg_map) {};
		// Takes a vector of keys and sets their properties to default
		ArgParser(const std::vector<std::string>& keys);

		// Appends a key and property to the argument map
		void addArg(std::string key, ArgProp properties = ArgProp());
		// Removes a key and propertie to the argument map
		void removeArg(std::string key);

		// Parses the arguments given
		// Returns a map of keys and their arguments
		ParsedArgs parseArgs(int argc, char** argv);
	};
}

