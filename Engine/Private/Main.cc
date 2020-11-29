#include <filesystem>
#include <iostream>

#include "Orion/BuildInfo.hh"
#include "Orion/Platform.hh"

int main(int argc, char **argv) {
  //  std::cout << "Orion version: " << Orion::kVersion << "\n";
  //
  //  std::filesystem::path appdata_dir =
  //      Orion::EnvironmentVariable(u8"APPDATA").value();
  //
  //  std::cout << "APPDATA: " << appdata_dir << "\n";
  //
  //  std::filesystem::path program_path{argv[0]};
  //  std::filesystem::path program_dir = program_path.parent_path();
  //
  //  std::cout << "Program directory: " << program_dir << "\n";

  auto str = Orion::Stl::DecodeWide(L"16\u00A3\n");
  auto narrow = Orion::Stl::EncodeNarrow(str);
  std::cout << narrow.data();
  auto wide = Orion::Stl::EncodeWide(str);
  std::wcout << wide.data();

  // Read installation configuration file
  // This config file is located in the same directory as the executable.

  // Run pre-configuration file initializaton
  // Orion::Setup();

  // Read configuration file
  // TODO: Read configuration file

  // Initiaize core subsystems
  // Orion::InputSubsystem input_subsystem;
  // Orion::RendererSubsystem renderer_subsystem;

  // return Orion::Entry(argc, argv);

  return 0;
}
