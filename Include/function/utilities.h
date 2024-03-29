#pragma once

#include <string>
namespace MyFunction
{
/**
 * @brief Include directories for standard headers
 *
 */
#ifdef COMPILE_COMMANDS_FILEPATH
std::string const COMPILECOMMANDS_INFOFILE(COMPILE_COMMANDS_FILEPATH);
#else
std::string const COMPILECOMMANDS_INFOFILE;
#endif

#ifdef TESTCODE_FULLPATH
std::string const TRAININGCODE_FILEPATH(TESTCODE_FULLPATH);
#else
std::string const TRAININGCODE_FILEPATH;
#endif
}   // namespace MyFunction