/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include "cmConfigure.h" // IWYU pragma: keep

#include <string>

#include <cm/optional>

#include <cm3p/curl/curl.h>

cm::optional<int> cmCurlParseTLSVersion(std::string const& tls_version);
std::string cmCurlSetCAInfo(::CURL* curl, const std::string& cafile = {});
std::string cmCurlSetNETRCOption(::CURL* curl, const std::string& netrc_level,
                                 const std::string& netrc_file);
std::string cmCurlFixFileURL(std::string url);
