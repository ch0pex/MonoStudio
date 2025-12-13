#pragma once

#include "rflect3d/config/base_config.hpp"
#include "rflect3d/config/parser.hpp"
#include "rflect3d/core/error/expected.hpp"

#include <boost/program_options.hpp>
#include <rfl/toml.hpp>
#include <rfl/toml/load.hpp>
#include <rfl/toml/write.hpp>

//
#include <filesystem>
#include <print>
#include <sstream>

namespace rflect::po {

namespace detail {

template<config::Game T>
inline std::string help_message(boost::program_options::options_description const& desc) {
  std::stringstream ss {};
  ss << desc << "\n";
  ss << "##########################\n";
  ss << "#      TOML config       #\n";
  ss << "##########################\n";
  ss << rfl::toml::write(config::example<T>());
  return ss.str();
}

} // namespace detail

template<config::Game T>
err::expected<T> parse_options(std::span<char*> const args) try {
  namespace po = boost::program_options;
  po::options_description desc("Allowed options");
  desc.add_options() //
      ("help,h", "Show help message") //
      ("config,c", po::value<std::string>(), "Path to configuration file");

  po::variables_map vm;
  po::store(po::parse_command_line(static_cast<int>(args.size()), args.data(), desc), vm);
  po::notify(vm);

  if (vm.contains("help")) {
    std::println("{}", detail::help_message<T>(desc));
    std::exit(0);
  }

  if (not vm.contains("config")) {
    std::println("No program options were provided");
    return err::unexpected(detail::help_message<T>(desc));
  }

  std::filesystem::path const config_file = vm["config"].as<std::string>();
  return config::parse_file<T>(config_file);
}
catch (boost::program_options::error const& e) {
  return err::unexpected("Error parsing command line: {}\n", e.what());
}

} // namespace rflect::po
