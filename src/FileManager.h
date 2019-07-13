// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "File.h"
#include <memory>
#include <unordered_map>

namespace fastype {

class FileManager {

private:
  std::unordered_map<std::string, std::shared_ptr<File>> fileMapping_;
};

} // namespace fastype
