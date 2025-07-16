#pragma once
#ifndef DWORD
#define DWORD unsigned int
#endif

#include "Unity/Il2Cpp.h"
#include "obfuscate.h"

// template <typename T>
// class Field : FieldInfo {
//   bool statik;

//   bool CheckStatic() {
//     if ((type->attrs & 0x10) == 0) return false;

//     if ((type->attrs & 0x40) != 0) return false;

//     if (thread_static = offset == -1) LOGI(OBFUSCATE("thread static fields is not supported!"));

//     return true;
//   }

//   bool CheckStaticFieldInfo(FieldInfo *fieldInfo) {
//     if ((fieldInfo->type->attrs & 0x10) == 0) return false;

//     if ((fieldInfo->type->attrs & 0x40) != 0) return false;

//     if (fieldInfo->offset == -1) LOGI(OBFUSCATE("thread static fields is not supported!"));

//     return true;
//   }

//  public:
//   bool  init;
//   bool  thread_static;
//   void *clazz;

//   Field(FieldInfo *thiz, void *clas = NULL) {
//     if (!CheckStaticFieldInfo(thiz)) clazz = clas;
//     init = (thiz != NULL);
//     if (init) {
//       parent = thiz->parent;
//       offset = thiz->offset;
//       name   = thiz->name;
//       token  = thiz->token;
//       type   = thiz->type;
//       statik = CheckStatic();
//     }
//   }

//   DWORD get_offset() { return offset; }

//   T get() {
//     if (!init || thread_static) return T();
//     if (statik) {
//       return *(T *)((uint64_t)parent->static_fields + offset);
//     }
//     return *(T *)((uint64_t)clazz + offset);
//   }

//   void set(T val) {
//     if (!init || thread_static) return;
//     if (statik) {
//       *(T *)((uint64_t)parent->static_fields + offset) = val;
//       return;
//     }
//     *(T *)((uint64_t)clazz + offset) = val;
//   }

//   operator T() { return get(); }

//   void operator=(T val) { set(val); }

//   bool operator==(Field other) {
//     if (!init) return false;
//     return (type == other.type && parent == other.parent && offset == other.offset && name == other.name &&
//             token == other.token);
//   }

//   T operator()() { return get(); }
// };
