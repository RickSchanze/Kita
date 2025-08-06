//
// Created by kita on 25-8-5.
//

#include "INIOutputArchive.h"

#include "Core/Assert.h"
#include "Core/Logging/Logger.hpp"
void INIOutputArchive::SetTargetFile(const Path& FilePath) { mIni.setFileName(FilePath.GetStdString()); }

void INIOutputArchive::BeginObject(const StringView ObjectName) { mCurrentSection = mIni[ObjectName.Data()]; }

void INIOutputArchive::EndObject() {
  ASSERT_MSG(IsSection(mCurrentSection), "Current section is not a valid section");
  if (IsFirstLayerSection(mCurrentSection)) {
    mCurrentSection = {};
  } else {
    mCurrentSection = *static_cast<inicpp::section*>(mCurrentSection.parent());
  }
}

bool INIOutputArchive::IsFirstLayerSection(const inicpp::section& Section) {
  if (Section.parent() != nullptr) {
    if (Section.parent()->parent() != nullptr) {
      return true;
    }
  }
  return false;
}

bool INIOutputArchive::IsSection(const inicpp::section& Section) { return Section.parent() != nullptr; }