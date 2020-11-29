#include "Orion/Stl/String.hh"

#include <array>
#include <optional>
#include <sstream>

namespace Orion::Stl {

namespace {

using CodePoint = std::uint_fast32_t;

std::size_t EncodeUtf8CodeUnits(CodePoint code_point,
                                std::span<char8_t, 4> code_units) {
  if (code_point < 0x0080) {
    code_units[0] = static_cast<char8_t>(code_point);
    return 1;
  } else if (code_point < 0x800) {
    code_units[0] =
        static_cast<char8_t>(((code_point >> 6u) & 0b00011111u) | 0b11000000u);
    code_units[1] =
        static_cast<char8_t>((code_point & 0b00111111u) | 0b10000000u);
    return 2;
  } else if (code_point < 0x10000) {
    code_units[0] =
        static_cast<char8_t>(((code_point >> 12u) & 0x0fu) | 0b11100000u);
    code_units[1] =
        static_cast<char8_t>(((code_point >> 6u) & 0x3fu) | 0b10000000u);
    code_units[2] = static_cast<char8_t>((code_point & 0x3fu) | 0b10000000u);
    return 3;
  } else if (code_point < 0x110000) {

    code_units[0] =
        static_cast<char8_t>(((code_point >> 18u) & 0x07u) | 0b11110000u);
    code_units[1] =
        static_cast<char8_t>(((code_point >> 12u) & 0x3fu) | 0b10000000u);
    code_units[2] =
        static_cast<char8_t>(((code_point >> 6u) & 0x3fu) | 0b10000000u);
    code_units[3] = static_cast<char8_t>((code_point & 0x3fu) | 0b10000000u);
    return 4;
  } else
    // Invalid code point
    // TODO: Better encoding exception
    throw std::exception("Invalid code point");
}

std::size_t EncodeUtf16CodeUnits(CodePoint code_point,
                                 std::span<char16_t, 2> code_units) {
  if (code_point < 0xd800) {
    code_units[0] = static_cast<char16_t>(code_point);
    return 1;
  } else if (code_point < 0x110000) {
    auto cp_mod = code_point - 0x10000;
    code_units[0] = static_cast<char16_t>((cp_mod >> 10u) + 0xd800u);
    code_units[1] = static_cast<char16_t>((cp_mod & 0x3ffu) + 0xdc00u);
    return 2;
  } else
    // Invalid code point
    // TODO: Better encoding exception
    throw std::exception("Invalid code point");
}

struct DecodeUtf8State {
  int RemainingBytes = 0;
  CodePoint CurrentCodePoint = 0;
};

std::optional<CodePoint> DecodeUtf8CodeUnit(std::uint8_t code_unit,
                                            DecodeUtf8State &state) {
  if (state.RemainingBytes == 0) {
    // Decoding new character

    if ((code_unit & 0b10000000u) == 0b00000000) {
      // Single-byte code point
      return static_cast<CodePoint>(code_unit);
    } else if ((code_unit & 0b11100000u) == 0b11000000u) {
      // Two-byte code point
      state.RemainingBytes = 1;
      state.CurrentCodePoint = static_cast<CodePoint>(code_unit & 0b00011111u);
    } else if ((code_unit & 0b11110000u) == 0b11100000u) {
      // Three-byte code point
      state.RemainingBytes = 2;
      state.CurrentCodePoint = static_cast<CodePoint>(code_unit & 0b00001111u);
    } else if ((code_unit & 0b11111000u) == 0b11110000u) {
      // Four-byte code point
      state.RemainingBytes = 3;
      state.CurrentCodePoint = static_cast<CodePoint>(code_unit & 0b00000111u);
    } else
      // Invalid code unit
      // TODO: Better exceptions
      throw std::exception("Invalid code unit");

    return {};
  }

  if ((code_unit & 0b11000000u) != 0b10000000u)
    // Invalid code unit
    // TODO: Better exceptions
    throw std::exception("Invalid code unit");

  state.CurrentCodePoint =
      (state.CurrentCodePoint << 6u) | (code_unit & 0b00111111u);
  state.RemainingBytes--;

  if (state.RemainingBytes == 0)
    return state.CurrentCodePoint;

  return {};
}

} // namespace

[[nodiscard]] String DecodeUtf8(std::span<const std::byte> bytes) {
  return String(reinterpret_cast<const char8_t *>(bytes.data()), bytes.size());
}

[[nodiscard]] String DecodeUtf16(std::span<const std::byte> bytes) {
  std::basic_stringstream<char8_t> ss;

  std::span<const std::uint16_t> utf16_code_units{
      reinterpret_cast<const std::uint16_t *>(bytes.data()),
      bytes.size_bytes() / 2};

  std::array<char8_t, 4> utf8_code_units{u8'\0'};
  CodePoint code_point = 0;
  for (auto utf16_code_unit : utf16_code_units) {
    if (utf16_code_unit < 0xd800) {
      // code unit == code point
      if (code_point != 0)
        // TODO: Better encoding error (error character??)
        throw std::exception("Bad surrogate pair");
      else if (utf16_code_unit == 0)
        // Null char, stop decoding
        break;
      code_point = static_cast<CodePoint>(utf16_code_unit);
    } else if (utf16_code_unit < 0xdc00) {
      // First-half of surrogate pair
      if (code_point != 0)
        // TODO: Better encoding error (error character??)
        throw std::exception("Bad surrogate pair");
      code_point = utf16_code_unit - 0xd800;
      code_point <<= 10u;
      continue;
    } else if (utf16_code_unit < 0xf000) {
      // Second-half of surrogate pair
      code_point += utf16_code_unit - 0xdc00;
    } else {
      // Invalid code unit
      throw std::exception("Invalid code unit");
    }

    std::size_t num_units = EncodeUtf8CodeUnits(code_point, utf8_code_units);
    ss.write(utf8_code_units.data(), num_units);
    code_point = 0;
  }

  return ss.str(); // There's an extra copy made here but to fix it we need have
                   // our own string implementation.
}

std::size_t EncodeUtf8(StringView str, std::span<char8_t> buffer,
                       bool zero_terminated) {
  std::memcpy(buffer.data(), str.data(), std::min(buffer.size(), str.size()));

  if (zero_terminated) {
    if (str.size() < buffer.size())
      buffer[str.size()] = u8'\0';
    return str.size() + 1;
  }
  return str.size();
}

std::size_t EncodeUtf16(StringView str, std::span<char16_t> buffer,
                        bool zero_terminated) {
  std::size_t total_length = 0;

  DecodeUtf8State decode_state;
  std::array<char16_t, 2> utf16_code_units{u'\0', u'\0'};
  for (char8_t utf8_code_unit : str) {
    auto code_point_opt = DecodeUtf8CodeUnit(
        static_cast<std::uint8_t>(utf8_code_unit), decode_state);
    if (!code_point_opt)
      continue;

    auto num_units =
        EncodeUtf16CodeUnits(code_point_opt.value(), utf16_code_units);

    if (total_length + num_units < buffer.size()) {
      std::memcpy(buffer.data() + total_length, utf16_code_units.data(),
                  num_units * sizeof(char16_t));
    } else if (total_length + 1 < buffer.size()) {
      buffer[total_length] = utf16_code_units[0];
    }
    total_length += num_units;
  }

  if (zero_terminated) {
    if (total_length + 1 < buffer.size()) {
      buffer[total_length] = u'\0';
    }

    total_length++;
  }

  return total_length;
}

} // namespace Orion::Stl