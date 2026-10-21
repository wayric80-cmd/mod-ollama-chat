#pragma once
#include <string>
struct OllamaChatRequest;
namespace WovTier
{
enum class Scope { Whisper, Guild, Group, Channel, Say, Yell, Other };
struct Audience
{
    bool realOnline = false, factionAllowed = false, sameGuild = false, sameGroup = false;
    bool inChannel = false, sameMapPhase = false;
    float distance = 0;
};
inline bool CanHear(Scope scope, Audience const& a, float sayRange, float yellRange)
{
    if (!a.realOnline || !a.factionAllowed)
        return false;
    switch (scope)
    {
        case Scope::Guild: return a.sameGuild;
        case Scope::Group: return a.sameGroup;
        case Scope::Channel: return a.inChannel;
        case Scope::Say: return a.sameMapPhase && a.distance <= sayRange;
        case Scope::Yell: return a.sameMapPhase && a.distance <= yellRange;
        default: return false;
    }
}
inline bool UsePremium(bool accountEnabled, bool premiumRecipient, bool premiumAudience,
                       bool rankEnabled, bool hasGuild, unsigned rank, unsigned maxRank,
                       bool realGuildOnline, bool personLane, bool guildChannel)
{
    return (accountEnabled && (premiumRecipient || premiumAudience)) ||
        (rankEnabled && hasGuild && rank <= maxRank && realGuildOnline && (personLane || guildChannel));
}
}
// World thread only: resolve entitlement and audible human audience before queuing HTTP work.
std::string WovPremiumModelRoute(OllamaChatRequest const& request);
