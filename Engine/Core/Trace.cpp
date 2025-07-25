//
// Created by Tencent_GO on 25-7-25.
//

#include "Trace.h"
#include "cpptrace/cpptrace.hpp"

String Trace::GenerateTraceString(Size Skip) {
  const auto MyTrace = cpptrace::generate_trace(Skip);
  return MyTrace.to_string();
}