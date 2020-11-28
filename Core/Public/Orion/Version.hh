#ifndef ORION_VERSION_HH
#define ORION_VERSION_HH

#include <optional>
#include <ostream>
#include <string_view>
#include <vector>

namespace Orion {

/**
 * Follows the Semantic Versioning Specification version 2.0.0.
 *
 * @see <a href="https://semver.org/spec/v2.0.0.html">Semantic Versioning 2.0.0
 * Specification</a>
 */
class Version {
public:
  Version(int major, int minor, int patch, std::string_view prerelease = {},
          std::string_view build_metadata = {});

  Version(const Version &other) = default;

  static std::optional<Version> Parse(std::string_view str);

  static std::optional<Version> Read(std::istream &stream);

  [[nodiscard]] constexpr int Major() const noexcept { return major_; }
  [[nodiscard]] constexpr int Minor() const noexcept { return minor_; }

  [[nodiscard]] constexpr int Patch() const noexcept { return patch_; }

  /**
   * Gets the pre-release component of the version.
   * <p>
   * If no pre-release component exists an empty string is returned.
   * @return The pre-release component or an empty string if the component does
   * not exist.
   */
  [[nodiscard]] inline std::string_view Prerelease() const noexcept {
    return {string_storage_.data(), build_metadata_offset_};
  }
  [[nodiscard]] std::string_view Prerelease(std::size_t idx) const noexcept;
  /**
   * Gets the build metadata of the version.
   * <p>
   * If no build metadata exists an empty string is returned.
   * @return The build metadata or an empty string if no build metadata exists.
   */
  [[nodiscard]] inline std::string_view BuildMetadata() const noexcept {
    return {string_storage_.data() + build_metadata_offset_,
            string_storage_.size() - build_metadata_offset_};
  }
  [[nodiscard]] std::string_view BuildMetadata(std::size_t idx) const noexcept;

  void Write(std::ostream &stream, bool include_build_metadata = true) const;

private:
  int major_;
  int minor_;
  int patch_;
  std::vector<char> string_storage_;
  std::size_t build_metadata_offset_;

  Version(int major, int minor, int patch, std::vector<char> &&string_storage,
          std::size_t build_metadata_offset)
      : major_(major), minor_(minor), patch_(patch),
        string_storage_(string_storage),
        build_metadata_offset_(build_metadata_offset) {}
};

std::ostream &operator<<(std::ostream &stream, const Version &value);

} // namespace Orion

#endif
