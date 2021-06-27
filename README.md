# CARG
A simple library for parsing command line arguments.

### building
Uses CMake to generate and build library.


## Example

creates an executable that takes the command line keys "-A", "-B" and/or "-C" with a single argument.  

> code
```cpp
#include <carg.h>
#include <iostream>

int main(int argc, char** argv)
{
    // create parser with possible keys "-A", "-B" and "-C"
    carg::ArgParser parser({"-A", "-B", "-C"});
    
    try
    {
        // parse the arguments
        carg::ParsedArgs args = parser.parseArgs(argc, argv);

        // print the arguments sorted by the keys

        std::cout << "-A: ";
        for(carg::Args arg: args["-A"])
        {
            std::cout << "'" << arg.getArg() << "' ";
        }

        std::cout << "\n-B: ";
        for(carg::Args arg: args["-B"])
        {
            std::cout << "'" << arg.getArg() << "' ";
        }

        std::cout << "\n-C: ";
        for(carg::Args arg: args["-C"])
        {
            std::cout << "'" << arg.getArg() << "' ";
        }
    }
    // print error message if the parser could not take the arguments
    catch(carg::CARGException& e)
    {
        std::cout << e.what();
    }
 
 }
```
>output  

*executable name is 'exec.exe'*
```
> exec.exe -A a -B b -C c -A 1 -B 2 -C 3
-A: 'a' '1'
-B: 'b' '2'
-C: 'c' '3'

> exec.exe -A a -B b -C
Expected '1' arguments for key '-C' got '0'
```
