#pragma once
#ifndef BE_CLI_VERSION_HPP_
#define BE_CLI_VERSION_HPP_

#include <be/core/macros.hpp>

#define BE_CLI_VERSION_MAJOR 0
#define BE_CLI_VERSION_MINOR 1
#define BE_CLI_VERSION_REV 9

/*!! include('common/version', 'be::cli') !! 6 */
/* ################# !! GENERATED CODE -- DO NOT MODIFY !! ################# */
#define BE_CLI_VERSION (BE_CLI_VERSION_MAJOR * 100000 + BE_CLI_VERSION_MINOR * 1000 + BE_CLI_VERSION_REV)
#define BE_CLI_VERSION_STRING "be::cli " BE_STRINGIFY(BE_CLI_VERSION_MAJOR) "." BE_STRINGIFY(BE_CLI_VERSION_MINOR) "." BE_STRINGIFY(BE_CLI_VERSION_REV)

/* ######################### END OF GENERATED CODE ######################### */

#endif
