//
// Created by kita on 25-8-23.
//

#include "Color.h"

void Color::WriteArchive(OutputArchive& Archive) const {
  Archive.WriteType("R", R());
  Archive.WriteType("G", G());
  Archive.WriteType("B", B());
  Archive.WriteType("A", A());
}

void Color::ReadArchive(InputArchive& Archive) {
  Archive.ReadType("R", R());
  Archive.ReadType("G", G());
  Archive.ReadType("B", B());
  Archive.ReadType("A", A());
}