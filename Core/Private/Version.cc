#include "Orion/Version.hh"

#include "Orion/Stl/String.hh"

#include <sstream>

namespace Orion {

namespace {

constexpr char kSeparatorChar = '.';
constexpr char kPrereleaseSeparatorChar = '-';
constexpr char kBuildMetadataSeparatorChar = '+';

constexpr std::size_t kInitialReadBufferCapacity = 128;

std::optional<int> StringToInt(std::string_view str) {
  int value = 0;
  for (char c : str) {
    int v = c - '0';
    if (v < 0 || v > 9)
      return {};
    value *= 10;
    value += v;
  }

  return value;
}

} // namespace

Version::Version(int major, int minor, int patch, std::string_view prerelease,
                 std::string_view build_metadata)
    : major_(major), minor_(minor), patch_(patch),
      string_storage_(prerelease.size() + build_metadata.size()),
      build_metadata_offset_(prerelease.size()) {
  std::memcpy(string_storage_.data(), prerelease.data(), prerelease.size());
  std::memcpy(string_storage_.data() + build_metadata_offset_,
              build_metadata.data(), build_metadata.size());
}

std::optional<Version> Version::Parse(std::string_view str) {
  std::stringstream ss;
  ss << str;
  return Read(ss);
}

std::optional<Version> Version::Read(std::istream &stream) {
  std::vector<char> buffer;
  buffer.reserve(kInitialReadBufferCapacity);

  auto read_until_separator = [&stream, &buffer](bool sep, bool prerelease_sep,
                                                 bool build_metadata_sep) {
    char c;
    stream.read(&c, 1);
    while ((!sep || c != kSeparatorChar) &&
           (!prerelease_sep || c != kPrereleaseSeparatorChar) &&
           (!build_metadata_sep || c != kBuildMetadataSeparatorChar) &&
           stream.good()) {
      buffer.push_back(c);
      stream.read(&c, 1);
    }

    return c;
  };

  read_until_separator(true, false, false);
  int major;
  if (auto opt = StringToInt({buffer.data(), buffer.size()}))
    major = *opt;
  else
    return {};
  buffer.clear();

  read_until_separator(true, false, false);
  int minor;
  if (auto opt = StringToInt({buffer.data(), buffer.size()}))
    minor = *opt;
  else
    return {};
  buffer.clear();

  char last_sep = read_until_separator(false, true, true);
  int patch;
  if (auto opt = StringToInt({buffer.data(), buffer.size()}))
    patch = *opt;
  else
    return {};
  buffer.clear();

  std::size_t prerelease_size = 0;
  if (last_sep == kPrereleaseSeparatorChar) {
    last_sep = read_until_separator(false, false, true);
    prerelease_size = buffer.size();
  }

  if (last_sep == kBuildMetadataSeparatorChar) {
    read_until_separator(false, false, false);
  }

  return Version{major, minor, patch, std::move(buffer), prerelease_size};
}

std::string_view Version::Prerelease(std::size_t idx) const noexcept {
  auto iter = Stl::Spliterator(Prerelease(), kSeparatorChar);

  iter.Skip(idx);
  return iter.Next();
}

std::string_view Version::BuildMetadata(std::size_t idx) const noexcept {
  auto iter = Stl::Spliterator(BuildMetadata(), kSeparatorChar);

  iter.Skip(idx);
  return iter.Next();
}
void Version::Write(std::ostream &stream, bool include_build_metadata) const {
  stream << major_ << kSeparatorChar << minor_ << kSeparatorChar << patch_;
  if (!Prerelease().empty())
    stream << kPrereleaseSeparatorChar << Prerelease();
  if (!BuildMetadata().empty() && include_build_metadata)
    stream << kBuildMetadataSeparatorChar << BuildMetadata();
}

std::ostream &operator<<(std::ostream &stream, const Version &value) {
  value.Write(stream, true);

  return stream;
}

} // namespace Orion
