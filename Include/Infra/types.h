#pragma once
#include "clang/ASTMatchers/ASTMatchFinder.h"

#include <set>
#include <string>
#include <vector>

// forward declarations
namespace clang
{
class SourceManager;

namespace tooling
{
class Replacements;
class Replacement;
}   // namespace tooling
}   // namespace clang
namespace Infra
{
// std:: aliases
using string_t = std::string;
using str_t_cr = string_t const&;
using vec_str = std::vector<string_t>;
using set_str = std::set<string_t>;

// clang:: aliases
using callback_t = clang::ast_matchers::MatchFinder::MatchCallback;
using finder_t = clang::ast_matchers::MatchFinder;
using result_t = clang::ast_matchers::MatchFinder::MatchResult;
using replacement_t = clang::tooling::Replacement;
using replacements_t = clang::tooling::Replacements;
using replacements_map_t = std::map<string_t, replacements_t>;
using vec_repl = std::vector<replacement_t>;
using sm_ptr_t = clang::SourceManager*;
using sm_ref_t = clang::SourceManager&;
using sm_cref_t = clang::SourceManager const&;
using sm_cc_ptr_t = clang::SourceManager const* const;
;

}   // namespace Infra