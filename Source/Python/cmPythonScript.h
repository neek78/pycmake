/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include <filesystem>
#include <iostream>

#include <pybind11/embed.h>

class cmMakefile;
class cmPythonRaw;

class cmPythonScript
{
public:
  cmPythonScript(
      cmMakefile& makefile,
      const std::filesystem::path& path, 
      const std::filesystem::path& filename);

  bool ParseScript();
  bool RunScript();

private:
  void LoadModule();
  std::string Mangle(const std::filesystem::path& fullPath) const;

  bool CheckRequiredCommands(const cmPythonRaw& raw);

  pybind11::module_ ClientModule;
  cmMakefile& Makefile;
  //const std::filesystem::path Path;
  //const std::filesystem::path Filename;
  const std::filesystem::path FullPath;
  const std::string MangledModName;
};
