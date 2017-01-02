#include "pch.hpp"
#include "app_name.hpp"

#include <be/util/paths.hpp>

namespace be {
namespace cli {
namespace {

S find_app_name() {
   S name = util::special_path(util::SpecialPath::process_image).stem().generic_string();
   if (name.empty()) {
      name = util::default_special_path(util::SpecialPath::process_image).stem().generic_string();
   }
   return name;
}

} // be::cli::()

const S& app_name() {
   static S name = find_app_name();
   return name;
}

} // be::cli
} // be
