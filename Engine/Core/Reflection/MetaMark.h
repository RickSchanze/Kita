#pragma once
#include "Type.h"
/**
 * 此文件定义了基本的反射标记
 * 要使用 可以用类似
 * class [[AName("B"), ALabel("C")]] D;
 * class [[AName("B")]] [[ALabel("C")]] D; 标记
 * 如果有需要额外标注的属性则单独使用额外标注即可，例如
 * class [[AName("B")]] D; // 不需要Reflected
 * 如果没有则Reflected是必要的
 * class [[AReflected]] D;
 */
#ifdef KITA_PROCESSING_METADATA_MARK
#define ALabel(x) clang::annotate("Label=" #x)
#define AAbstract clang::annotate("Abstract")
#define AName(x) clang::annotate("Name=" #x)
#define AReflected clang::annotate("Reflected")
#define AGetter(x) clang::annotate("Getter=" #x)
#define ASetter(x) clang::annotate("Setter=" #x)
#define ADataMember clang::annotate("DataMember")
#define ATransient clang::annotate("Transient")
#define ACustomSerialization clang::annotate("CustomSerialization")
#define GEN_HEADER(x) <string>
#else
#define ALabel(X, ...) ALabel(X, __VA_ARGS__)
#define AAbstract AAbstract
#define AName(x, ...) AName(x, __VA_ARGS__)
#define AReflected AReflected
#define AGetter(x, ...) AGetter(x, __VA_ARGS__)
#define ASetter(x, ...) ASetter(x, __VA_ARGS__)
#define ADataMember ADataMember
#define ATransient ATransient
#define ACustomSerialization ACustomSerialization
#define GEN_HEADER(x) x
#endif

#define MACRO_COMBINE2_(a, b) a##b
#define MACRO_COMBINE3_(a, b, c) a##b##c
#define MACRO_COMBINE4_(a, b, c, d) a##b##c##d

#define MACRO_COMBINE2(a, b) MACRO_COMBINE2_(a, b)
#define MACRO_COMBINE3(a, b, c) MACRO_COMBINE3_(a, b, c)
#define MACRO_COMBINE4(a, b, c, d) MACRO_COMBINE4_(a, b, c, d)

#ifdef KITA_PROCESSING_METADATA_MARK
#define GENERATED_BODY(Name)
#else
#define GENERATED_BODY(Name) MACRO_COMBINE4(GENERATED_, Name, _, CURRENT_FILE_ID)
#endif