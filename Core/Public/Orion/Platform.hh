#ifndef ORION_PLATFORM_HH
#define ORION_PLATFORM_HH

#include "Orion/Stl/String.hh"
#include <optional>

namespace Orion {

/**
 * Gets the value of an environment variable.
 * @param variable The name of the environment variable to retrieve.
 * @return The value of the environment variable. If the variable doesn't exist
 * std::nullopt is returned.
 */
std::optional<Stl::String> EnvironmentVariable(Stl::StringView variable);

} // namespace Orion

#endif
