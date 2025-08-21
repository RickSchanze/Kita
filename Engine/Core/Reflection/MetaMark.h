#pragma once

#define KCLASS(...)
#define KPROPERTY(...)
#define KFUNCTION(...)
#define KENUM(...)
#define KVALUE(...)
#define KSTRUCT(...)

#define GENERATED_BODY(ClassName) GENERATED_HEADER_##ClassName

inline const char* AvailableClassPropertyNames[] = {"Abstract", "Name", "Label"};
