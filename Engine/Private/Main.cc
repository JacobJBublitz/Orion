#include <filesystem>
#include <iostream>

#include "Orion/BuildInfo.hh"

int main(int argc, char **argv) {
  std::cout << "Orion version: " << Orion::kVersion << "\n";

  std::filesystem::path program_path{argv[0]};
  std::filesystem::path program_dir = program_path.parent_path();

  std::cout << "Program directory: " << program_dir << "\n";

  // Read installation configuration file
  // This config file is located in the same directory as the executable.

  // Run pre-configuration file initializaton
  // Orion::Setup();

  // Read configuration file
  // TODO: Read configuration file

  // Initiaize core subsystems
  // Orion::InputSubsystem input_subsystem;
  // Orion::RendererSubsystem renderer_subsystem;

  std::cout << "Hello, world!\n";
  // return Orion::Entry(argc, argv);

  return 0;
}
