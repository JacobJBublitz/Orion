#ifndef ORION_STL_STRING_HH
#define ORION_STL_STRING_HH

#include <string_view>

namespace Orion::Stl {

template <typename CharT> class Spliterator {
public:
  using CharType = CharT;
  using StringType = std::basic_string_view<CharT>;

  constexpr Spliterator(StringType str, CharType separator) noexcept
      : str_(str), separator_string_(), separator_char_(separator) {}

  constexpr Spliterator(StringType str, StringType separator) noexcept
      : str_(str), separator_string_(separator), separator_char_('\0') {}

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

} // namespace Orion::Stl

#endif
