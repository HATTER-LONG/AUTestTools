#pragma once

#include "Infra/types.h"

namespace MyFunction
{
/**\brief Include directories for standard headers */
namespace
{
#ifdef CLANG_ANALYSIS_ARGS_APPEND
Infra::string_t const CLANG_ARGS2APPEND(CLANG_ANALYSIS_ARGS_APPEND);
#else
Infra::string_t const CLANG_ARGS2APPEND;
#endif

#ifdef COMPILE_COMMANDS_FILEPATH
Infra::string_t const COMPILECOMMANDS_INFOFILE(COMPILE_COMMANDS_FILEPATH);
#else
Infra::string_t const COMPILECOMMANDS_INFOFILE;
#endif

#ifdef TESTCODE_FILENAMEANDFULLPATH
Infra::string_t const TESTCODEFILE(TESTCODE_FILENAMEANDFULLPATH);
#else
Infra::string_t const TESTCODE_FILE;
#endif

} // namespace
} // namespace MyFunction