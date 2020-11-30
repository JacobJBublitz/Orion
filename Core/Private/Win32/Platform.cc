#include "Orion/Platform.hh"

#include <system_error>

extern "C" {
#include <Windows.h>
}

namespace Orion {

namespace {

constexpr std::size_t kEnvBufferSize = 64;

} // namespace

std::optional<Stl::String> EnvironmentVariable(Stl::StringView variable) {
  wchar_t name_stack_buffer[kEnvBufferSize];
  wchar_t value_stack_buffer[kEnvBufferSize];

  std::vector<wchar_t> name_heap_buffer;
  std::vector<wchar_t> value_heap_buffer;

  const wchar_t *name_ptr;

  if (Stl::EncodeWide(variable, name_stack_buffer, true) <= kEnvBufferSize) {
    // Name fit into stack-allocated buffer
    name_ptr = name_stack_buffer;
  } else {
    // Store the encoded name in the heap
    name_heap_buffer = Stl::EncodeWide(variable, true);
    name_ptr = name_heap_buffer.data();
  }

  DWORD result =
      GetEnvironmentVariableW(name_ptr, value_stack_buffer, kEnvBufferSize);
  if (result == 0) {
    DWORD err = GetLastError();
    if (err == ERROR_ENVVAR_NOT_FOUND)
      return {};
    throw std::system_error(err, std::system_category(),
                            "Failed to get environment variable");
  }

  if (result <= kEnvBufferSize) {
    // Value fit into stack-allocated buffer
    return Stl::DecodeWide({value_stack_buffer, result});
  }

  // Value didn't fit into stack-allocated buffer
  value_heap_buffer.resize(result);

  result = GetEnvironmentVariableW(name_ptr, value_heap_buffer.data(), result);
  if (result == 0) {
    DWORD err = GetLastError();
    if (err == ERROR_ENVVAR_NOT_FOUND)
      return {};
    throw std::system_error(err, std::system_category(),
                            "Failed to get environment variable");
  }

  return Stl::DecodeWide(value_heap_buffer);
}

} // namespace Orion