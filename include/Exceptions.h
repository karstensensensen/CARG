#pragma once

namespace carg
{
	// Base carg exception class
	class CARGException : public std::exception
	{
	protected:
		std::string err_msg;
		
		CARGException() : err_msg("Carg exception was thrown with no error msg") {};
		CARGException(std::string msg) : err_msg(msg) {}

	public:
		virtual const char* what() const throw()
		{
			return err_msg.c_str();
		}

	};

	// Thrown if there are too few arguments for a key
	class NotEnoughArgs : public CARGException
	{
	public:
		NotEnoughArgs() : CARGException("Got an insuficcient number of arguments") {}
		NotEnoughArgs(std::string key, int expected_count, int arg_count)
			: CARGException("")
		{
			err_msg += "Expected '" + std::to_string(expected_count) + "' arguments for key '" + key + "' got '" + std::to_string(arg_count) + '\'';
		}
	};

	// Thrown if there are to few or to many instances of a key
	class InvalidInstanceCount : public CARGException
	{
	public:
		InvalidInstanceCount() : CARGException("Got an invalid number of keys") {}
		InvalidInstanceCount(std::string key, std::pair<unsigned int, unsigned int> range, unsigned int instances) : CARGException("")
		{
			if(range.first != range.second)
				err_msg += "expected between '" + std::to_string(range.first) + "' and '" + std::to_string(range.second) + "' instances of key '" + key + "' got '" + std::to_string(instances) + '\'';
			else if(range.first == 1)
				err_msg += "expected '1' instance of key '" + key + "' got '" + std::to_string(instances) + '\'';
			else	
				err_msg += "expected '" + std::to_string(range.first) + "' instances of key '" + key + "' got '" + std::to_string(instances) + '\'';

		}
	};

	// thrown if an argument has no key
	// will never be thrown if an empty [" "] key exists
	class NoKeyFound : public CARGException
	{
	public:
		NoKeyFound() : CARGException("Could not find a key for the given argument") {}
		NoKeyFound(std::string argument) : CARGException("")
		{
			err_msg += "No key found for the argument '" + argument + '\'';
		}
	};

	// Thrown if an argument passed to a key is invalid
	class InvalidCArg: public CARGException
	{
	protected:
		using CARGException::err_msg;
	public:
		InvalidCArg(): CARGException("Invalid command line argument was passed") {};
		InvalidCArg(std::string msg) : CARGException(msg) {}

		virtual const char* what() const throw()
		{
			return err_msg.c_str();
		}
	};
	
	// Thrown if an argument cannot be converted to a natural number (long long)
	class NotANatural: public InvalidCArg
	{
	public:
		NotANatural(): InvalidCArg("Argument passed was not a natural number") {}
		NotANatural(std::string invalid_arg) : InvalidCArg("")
		{
			err_msg += "\"\" is not a natural number";
			err_msg.insert(err_msg.begin() + 1, invalid_arg.begin(), invalid_arg.end());
		}

		virtual const char* what() const throw()
		{
			return err_msg.c_str();
		}
	};

	// Thrown if an argument cannot be converted to a real number (double)
	class NotAReal : public InvalidCArg
	{
	public:
		NotAReal() : InvalidCArg("Argument passed was not a real (floating point) number") {}
		NotAReal(std::string invalid_arg) : InvalidCArg("")
		{
			err_msg += "\"\" is not a real (floating point) number";
			err_msg.insert(err_msg.begin() + 1, invalid_arg.begin(), invalid_arg.end());
		}

		virtual const char* what() const throw()
		{
			return err_msg.c_str();
		}
	};

	// Thrown if an argument cannot be converted to a boolean value
	class NotABool : public InvalidCArg
	{
	public:
		NotABool() : InvalidCArg("Argument passed was not a boolean value") {}
		NotABool(std::string invalid_arg) : InvalidCArg("")
		{
			err_msg += "\"\" is not a boolean value";
			err_msg.insert(err_msg.begin() + 1, invalid_arg.begin(), invalid_arg.end());
		}

		virtual const char* what() const throw()
		{
			return err_msg.c_str();
		}
	};

	// Thrown if an argument cannot be matched with any of the given options
	class NoValidOption : public InvalidCArg
	{
	public:
		NoValidOption() : InvalidCArg("Argument passed could not be matched to passed regexes") {};
		NoValidOption(std::string invalid_arg) : InvalidCArg("")
		{
			err_msg += "\"\" could not be matched to passed regexes";
			err_msg.insert(err_msg.begin() + 1, invalid_arg.begin(), invalid_arg.end());
		}

		virtual const char* what() const throw()
		{
			return err_msg.c_str();
		}
	};
}

