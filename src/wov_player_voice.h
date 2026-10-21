#pragma once
#include <algorithm>
#include <cctype>
#include <regex>
#include <string>

namespace Wov
{
// Reject narrator-style self references, including split-reply continuations.
// Introductions ("I'm Bob") and mentions of other characters still pass.
inline bool IsSelfNarration(std::string const& text, std::string const& name)
{
    if (name.empty())
        return false;
    std::string escaped;
    for (char ch : name)
    {
        if (std::string(".^$|()[]{}*+?\\").find(ch) != std::string::npos)
            escaped += '\\';
        escaped += ch;
    }
    std::regex const self("\\b" + escaped +
        "(?:(?:'|’)s\\s+(?:in|at|is|was|just|got|going|looking|level)\\b|\\s+(?:is|was|has|just|got|went|hit|died|needs|wants|says|feels|reached|will|does|did)\\b)",
        std::regex::icase);
    return std::regex_search(text, self);
}
}
