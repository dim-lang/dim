// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "File.h"
#include "Logging.h"
#include <memory>
#include <mutex>
#include <unordered_map>

namespace fastype {

class FileManager : public Logging {
public:
private:
  std::unordered_map<std::string, std::shared_ptr<File>> fileMapping_;
  std::mutex lock_;
};

} // namespace fastype
