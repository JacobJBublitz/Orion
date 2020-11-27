#ifndef ORION_VERSION_HH
#define ORION_VERSION_HH

#include <ostream>
#include <string_view>

namespace Orion {

class Version {
  int major_;
  int minor_;
  int patch_;
  std::string_view prerelease_;
  std::string_view build_metadata_;

public:
  constexpr Version(int major, int minor, int patch,
                    std::string_view prerelease = {},
                    std::string_view build_metadata = {}) noexcept
      : major_(major), minor_(minor), patch_(patch), prerelease_(prerelease),
        build_metadata_(build_metadata) {}

  constexpr Version(const Version &other) noexcept = default;

  [[nodiscard]] constexpr int Major() const noexcept { return major_; }
  [[nodiscard]] constexpr int Minor() const noexcept { return minor_; }

  [[nodiscard]] constexpr int Patch() const noexcept { return patch_; }
  [[nodiscard]] constexpr std::string_view Prerelease() const noexcept {
    return prerelease_;
  }
  [[nodiscard]] std::string_view Prerelease(std::size_t idx) const noexcept;
  [[nodiscard]] constexpr std::string_view BuildMetadata() const noexcept {
    return build_metadata_;
  }
  [[nodiscard]] std::string_view BuildMetadata(std::size_t idx) const noexcept;

  void WriteToStream(std::ostream &stream,
                     bool include_build_metadata = true) const;
};

std::ostream &operator<<(std::ostream &stream, const Version &value);

} // namespace Orion

#endif
