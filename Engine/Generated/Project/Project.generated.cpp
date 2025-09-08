// Auto-generated source file
#include "Project/Project.h"

const Type* Project::GetStaticType() { return TypeOf<Project>(); }
const Type* Project::GetType() { return TypeOf<Project>(); }
void Project::WriteArchive(OutputArchive& Archive) const { 
Archive.WriteType("mProjectPath", mProjectPath); 
Archive.WriteType("mProjectName", mProjectName); 
Archive.WriteType("mPersistentId", mPersistentId); 
} 
void Project::ReadArchive(InputArchive& Archive) { 
Archive.ReadType("mProjectPath", mProjectPath); 
Archive.ReadType("mProjectName", mProjectName); 
Archive.ReadType("mPersistentId", mPersistentId); 
} 
