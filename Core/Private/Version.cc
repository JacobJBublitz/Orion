#include "Orion/Version.hh"

#include "Orion/Stl/String.hh"

namespace Orion {

namespace {

constexpr char kSeparatorChar = '.';
constexpr char kPrereleaseSeparatorChar = '-';
constexpr char kBuildMetadataSeparatorChar = '+';

} // namespace

std::string_view Version::Prerelease(std::size_t idx) const noexcept {
  auto iter = Stl::Spliterator(prerelease_, kSeparatorChar);

  iter.Skip(idx);
  return iter.Next();
}

std::string_view Version::BuildMetadata(std::size_t idx) const noexcept {
  auto iter = Stl::Spliterator(build_metadata_, kSeparatorChar);

  iter.Skip(idx);
  return iter.Next();
}
void Version::WriteToStream(std::ostream &stream,
                            bool include_build_metadata) const {
  stream << major_ << kSeparatorChar << minor_ << kSeparatorChar << patch_;
  if (!prerelease_.empty())
    stream << kPrereleaseSeparatorChar << prerelease_;
  if (!build_metadata_.empty() && include_build_metadata)
    stream << kBuildMetadataSeparatorChar << build_metadata_;
}

std::ostream &operator<<(std::ostream &stream, const Version &value) {
  value.WriteToStream(stream, true);

  return stream;
}

} // namespace Orion
