/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include <vector>
#include <string>

class cmExecutionStatus;

template<typename T>
using CommandFunction = bool (*) (const std::vector<T>& params, cmExecutionStatus& status);

using StdCommandFunction = CommandFunction<std::string>;

