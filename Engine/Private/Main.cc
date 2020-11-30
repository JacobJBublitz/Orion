#include <filesystem>
#include <iostream>

#include "Orion/BuildInfo.hh"
#include "Orion/Platform.hh"

int main(int argc, char **argv) {
  std::cout << "Orion version: " << Orion::kVersion << "\n";

  std::filesystem::path home_dir;
  if (auto res = Orion::EnvironmentVariable(u8"HOME")) {
    home_dir = *res;
  } else {
    throw std::runtime_error("Unable to determine user home directory");
  }

  std::filesystem::path config_dir;
  if (auto res = Orion::EnvironmentVariable(u8"XDG_CONFIG_HOME")) {
    config_dir = *res;
  } else {
    config_dir = home_dir / ".config";
  }

  std::cout << "User Home: " << home_dir << "\n"
            << "Config Dir: " << config_dir << "\n";

  //  std::filesystem::path appdata_dir =
  //      Orion::EnvironmentVariable(u8"APPDATA").value();
  //
  //  std::cout << "APPDATA: " << appdata_dir << "\n";
  //
  //  std::filesystem::path program_path{argv[0]};
  //  std::filesystem::path program_dir = program_path.parent_path();
  //
  //  std::cout << "Program directory: " << program_dir << "\n";

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
