// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Global.h"
#include "Config.h"
#include <cstdlib>
#include <string>

namespace fastype {

const char *Global::ProjectName = PROJECT_NAME;
const char *Global::ProjectVerion = PROJECT_VERSION;
const char *Global::ProjectVerionMajor = PROJECT_VERSION_MAJOR;
const int Global::ProjectVerionMajorNumber = std::atoi(PROJECT_VERSION_MAJOR);
const char *Global::ProjectVerionMinor = PROJECT_VERSION_MINOR;
const int Global::ProjectVerionMinorNumber = std::atoi(PROJECT_VERSION_MINOR);
const char *Global::ProjectVerionPatch = PROJECT_VERSION_PATCH;
const int Global::ProjectVerionPatchNumber = std::atoi(PROJECT_VERSION_PATCH);
const char *Global::FastypeVersion = PROJECT_NAME " " PROJECT_VERSION;

} // namespace fastype
