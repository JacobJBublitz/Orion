#include "Orion/Platform.hh"

#include <cstdlib>
#include <cstring>

namespace Orion {

namespace {

constexpr std::size_t kEnvBufferSize = 64;

} // namespace

std::optional<Stl::String> EnvironmentVariable(Stl::StringView variable) {
  char name_stack_buffer[kEnvBufferSize];

  std::vector<char> name_heap_buffer;

  const char *name_ptr;

  if (Stl::EncodeNarrow(variable, name_stack_buffer, true) <= kEnvBufferSize) {
    // Name fit into stack-allocated buffer
    name_ptr = name_stack_buffer;
  } else {
    // Store the encoded name in the heap
    name_heap_buffer = Stl::EncodeNarrow(variable, true);
    name_ptr = name_heap_buffer.data();
  }

  const char *value = getenv(name_ptr);
  if (value) {
    return Stl::DecodeNarrow({value, std::strlen(value)});
  } else {
    return std::nullopt;
  }
}

} // namespace Orion