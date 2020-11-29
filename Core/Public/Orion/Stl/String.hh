#ifndef ORION_STL_STRING_HH
#define ORION_STL_STRING_HH

#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace Orion::Stl {

using String = std::basic_string<char8_t>;
using StringView = std::basic_string_view<char8_t>;

template <typename CharT> class Spliterator {
public:
  using CharType = CharT;
  using StringType = std::basic_string_view<CharT>;

  constexpr Spliterator(StringType str, CharType separator) noexcept
      : str_(str), separator_string_(), separator_char_(separator) {}

  constexpr Spliterator(StringType str, StringType separator) noexcept
      : str_(str), separator_string_(separator), separator_char_('\0') {}

  [[nodiscard]] constexpr bool HasNext() const noexcept {
    return !str_.empty();
  }

  StringType Next() noexcept {
    auto str = **this;
    ++(*this);
    return str;
  }

  bool Skip(std::size_t count) noexcept {
    for (std::size_t i = 0; i < count; ++i)
      if (!HasNext())
        return false;
      else
        Next();
    return true;
  }

  constexpr operator bool() const noexcept { return !str_.empty(); }

  StringType operator*() const noexcept {
    if (!*this)
      return {};

    std::size_t idx;
    if (!separator_string_.empty()) {
      idx = str_.find(separator_string_);
    } else {
      idx = str_.find(separator_char_);
    }

    return str_.substr(0, idx);
  }

  Spliterator &operator++() noexcept {
    if (!*this)
      return *this;

    std::size_t idx;
    std::size_t sep_len;
    if (!separator_string_.empty()) {
      idx = str_.find(separator_string_);
      sep_len = separator_string_.size();
    } else {
      idx = str_.find(separator_char_);
      sep_len = 1;
    }

    if (idx == std::string_view::npos)
      str_ = {};
    else
      str_ = str_.substr(idx + sep_len);

    return *this;
  }

  Spliterator operator++(int) noexcept {
    Spliterator result(*this);
    ++(*this);
    return result;
  }

private:
  StringType str_;
  StringType separator_string_;
  CharType separator_char_;
};

[[nodiscard]] String DecodeUtf8(std::span<const std::byte> bytes);
[[nodiscard]] inline String DecodeUtf8(std::span<const char8_t> chars) {
  return DecodeUtf8(std::as_bytes(chars));
}
[[nodiscard]] String DecodeUtf16(std::span<const std::byte> bytes);
[[nodiscard]] inline String DecodeUtf16(std::span<const char16_t> chars) {
  return DecodeUtf16(std::as_bytes(chars));
}
[[nodiscard]] String DecodeUtf32(std::span<const std::byte> bytes);
[[nodiscard]] inline String DecodeUtf32(std::span<const char32_t> chars) {
  return DecodeUtf32(std::as_bytes(chars));
}
[[nodiscard]] String DecodeNarrow(std::span<const std::byte> bytes);
[[nodiscard]] inline String DecodeNarrow(std::span<const char> chars) {
  return DecodeNarrow(std::as_bytes(chars));
}
[[nodiscard]] String DecodeWide(std::span<const std::byte> bytes);
[[nodiscard]] inline String DecodeWide(std::span<const wchar_t> chars) {
  return DecodeWide(std::as_bytes(chars));
}

std::size_t EncodeUtf8(StringView str, std::span<char8_t> buffer,
                       bool zero_terminated = true);
[[nodiscard]] inline std::vector<char8_t>
EncodeUtf8(StringView str, bool zero_terminated = true) {
  std::vector<char8_t> result;

  std::size_t size = EncodeUtf8(str, {}, zero_terminated);
  result.resize(size);

  EncodeUtf8(str, result, zero_terminated);

  return result;
}
std::size_t EncodeUtf16(StringView str, std::span<char16_t> buffer,
                        bool zero_terminated = true);
[[nodiscard]] inline std::vector<char16_t>
EncodeUtf16(StringView str, bool zero_terminated = true) {
  std::vector<char16_t> result;

  std::size_t size = EncodeUtf16(str, {}, zero_terminated);
  result.resize(size);

  EncodeUtf16(str, result, zero_terminated);

  return result;
}
std::size_t EncodeUtf32(StringView str, std::span<char32_t> buffer,
                        bool zero_terminated = true);
[[nodiscard]] inline std::vector<char32_t>
EncodeUtf32(StringView str, bool zero_terminated = true) {
  std::vector<char32_t> result;

  std::size_t size = EncodeUtf32(str, {}, zero_terminated);
  result.resize(size);

  EncodeUtf32(str, result, zero_terminated);

  return result;
}
std::size_t EncodeNarrow(StringView str, std::span<char> buffer,
                         bool zero_terminated = true);
[[nodiscard]] inline std::vector<char>
EncodeNarrow(StringView str, bool zero_terminated = true) {
  std::vector<char> result;

  std::size_t size = EncodeNarrow(str, {}, zero_terminated);
  result.resize(size);

  EncodeNarrow(str, result, zero_terminated);

  return result;
}
std::size_t EncodeWide(StringView str, std::span<wchar_t> buffer,
                       bool zero_terminated = true);
[[nodiscard]] inline std::vector<wchar_t>
EncodeWide(StringView str, bool zero_terminated = true) {
  std::vector<wchar_t> result;

  std::size_t size = EncodeWide(str, {}, zero_terminated);
  result.resize(size);

  EncodeWide(str, result, zero_terminated);

  return result;
}

// Narrow chars should always be convertible to char8_t
static_assert(sizeof(char) == sizeof(char8_t),
              "char cannot be converted to char8_t.");

[[nodiscard]] inline String DecodeNarrow(std::span<const std::byte> bytes) {
  return DecodeUtf8(bytes);
}

inline std::size_t EncodeNarrow(StringView str, std::span<char> buffer,
                                bool zero_terminated) {
  return EncodeUtf8(str,
                    {reinterpret_cast<char8_t *>(buffer.data()), buffer.size()},
                    zero_terminated);
}

#ifdef _WINDOWS
static_assert(sizeof(wchar_t) == sizeof(char16_t),
              "wchar_t cannot be converted to char16_t.");

[[nodiscard]] inline String DecodeWide(std::span<const std::byte> bytes) {
  return DecodeUtf16(bytes);
}

inline std::size_t EncodeWide(StringView str, std::span<wchar_t> buffer,
                              bool zero_terminated) {
  return EncodeUtf16(
      str, {reinterpret_cast<char16_t *>(buffer.data()), buffer.size()},
      zero_terminated);
}
#else
static_assert(sizeof(wchar_t) == sizeof(char32_t),
              "wchar_t cannot be converted to char32_t.");

[[nodiscard]] inline String DecodeWide(std::span<const std::byte> bytes) {
  return DecodeUtf32(bytes);
}

inline std::size_t EncodeWide(StringView str, std::span<wchar_t> buffer,
                              bool zero_terminated) {
  return EncodeUtf32(
      str, {reinterpret_cast<char32_t *>(buffer.data()), buffer.size()},
      zero_terminated);
}
#endif

} // namespace Orion::Stl

#endif
