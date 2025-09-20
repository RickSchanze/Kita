#pragma once
#include "Inspector.h"

#include "MeshInspector.generated.h"

KCLASS(Inspector = Mesh)
class MeshInspector : public Inspector {
  GENERATED_BODY(MeshInspector)
public:
  virtual void DrawGUI() override;
};
