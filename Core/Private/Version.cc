#include "Orion/Version.hh"

#include "Orion/Stl/String.hh"

namespace Orion {

std::string_view Version::Prerelease(std::size_t idx) const noexcept {
  auto iter = Stl::Spliterator(prerelease_, '.');

  std::size_t i = 0;
  while (iter) {
    if (i == idx)
      return *iter;
    ++iter;
    ++i;
  }

  return {};
}

std::string_view Version::BuildMetadata(std::size_t idx) const noexcept {
  auto iter = Stl::Spliterator(build_metadata_, '.');

  std::size_t i = 0;
  while (iter) {
    if (i == idx)
      return *iter;
    ++iter;
    ++i;
  }

  return {};
}

std::ostream &operator<<(std::ostream &stream, const Version &value) {
  stream << value.Major() << '.' << value.Minor();
  if (value.Patch() != 0)
    stream << '.' << value.Patch();
  if (!value.Prerelease().empty())
    stream << '-' << value.Prerelease();
  if (!value.BuildMetadata().empty())
    stream << '+' << value.BuildMetadata();

  return stream;
}

} // namespace Orion
