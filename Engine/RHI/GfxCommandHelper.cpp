//
// Created by kita on 25-9-3.
//

#include "GfxCommandHelper.h"

GfxCommandSyncHandle GfxCommandHelper::CopyAsync(RHIBuffer* Source, Int32 SourceSize, Int32 SourceOffset, RHIBuffer* Dest, Int32 DestSize, Int32 DestOffset) {}

GfxCommandSyncHandle::GfxCommandSyncHandle() {}

GfxCommandSyncHandle::~GfxCommandSyncHandle() { WaitAll(); }
