#pragma once

#ifndef DWORD
#define DWORD unsigned int
#endif

#include <dlfcn.h>

#include "Struct/Obfuscate.h"
#include "Unity/Il2Cpp.h"

typedef Il2CppClass *(*class_from_name_t)(const Il2CppImage *assembly, const char *name_space, const char *name);
typedef MethodInfo *(*class_get_method_from_name_t)(Il2CppClass *klass, const char *name, int paramcount);
typedef Il2CppDomain *(*domain_get_t)();
typedef const Il2CppAssembly **(*domain_get_assemblies_t)(const Il2CppDomain *domain, size_t *size);
typedef const Il2CppImage *(*assembly_get_image_t)(const Il2CppAssembly *assembly);
typedef const Il2CppAssembly *(*domain_assembly_open_t)(Il2CppDomain *domain, const char *name);
typedef FieldInfo *(*class_get_field_from_name_t)(Il2CppClass *klass, const char *name);

class LoadClass {
  void *get_il2cpp() {
    void *mod = 0;
    while (!mod) {
      mod = dlopen(OBFUSCATE("libil2cpp.so"), 4);
    }
    return mod;
  }

  Il2CppClass *GetClassFromName(const char *name_space, const char *type_name) {
    auto domain_get = (domain_get_t)dlsym(get_il2cpp(), OBFUSCATE("il2cpp_domain_get"));
    auto dom        = domain_get();
    if (!dom) {
      return nullptr;
    }
    size_t assemb_count;
    auto   domain_get_assemblies =
        (domain_get_assemblies_t)dlsym(get_il2cpp(), OBFUSCATE("il2cpp_domain_get_assemblies"));
    const Il2CppAssembly **allAssemb = domain_get_assemblies(dom, &assemb_count);
    auto assembly_get_image = (assembly_get_image_t)dlsym(get_il2cpp(), OBFUSCATE("il2cpp_assembly_get_image"));
    auto class_from_name    = (class_from_name_t)dlsym(get_il2cpp(), OBFUSCATE("il2cpp_class_from_name"));
    for (int i = 0; i < assemb_count; i++) {
      auto assemb = allAssemb[i];
      auto img    = assembly_get_image(assemb);
      if (!img) {
        continue;
      }
      auto klass = class_from_name(img, name_space, type_name);
      if (klass) {
        namespaze_txt = name_space;
        clazz_txt     = type_name;
        dllname_txt   = img->name;
        dll           = img;
        return klass;
      }
    }
    return nullptr;
  }

 public:
  const Il2CppImage *dll;
  Il2CppClass       *thisclass;
  const char        *namespaze_txt;
  const char        *clazz_txt;
  const char        *dllname_txt;

  LoadClass(const char *namespaze, const char *clazz) {
    thisclass = nullptr;
    do {
      thisclass = GetClassFromName(namespaze, clazz);
    } while (!thisclass);
  }

  LoadClass(const char *namespaze, const char *clazz, const char *dllname) {
    auto domain_assembly_open = (domain_assembly_open_t)dlsym(get_il2cpp(), OBFUSCATE("il2cpp_domain_assembly_open"));
    auto assembly_get_image   = (assembly_get_image_t)dlsym(get_il2cpp(), OBFUSCATE("il2cpp_assembly_get_image"));
    auto domain_get           = (domain_get_t)dlsym(get_il2cpp(), OBFUSCATE("il2cpp_domain_get"));
    auto class_from_name      = (class_from_name_t)dlsym(get_il2cpp(), OBFUSCATE("il2cpp_class_from_name"));
    dll                       = assembly_get_image(domain_assembly_open(domain_get(), dllname));
    thisclass                 = nullptr;
    do {
      thisclass = class_from_name(dll, namespaze, clazz);
    } while (!thisclass);
  }

  FieldInfo *GetFieldInfoByName(const char *name) {
    auto class_get_field_from_name =
        (class_get_field_from_name_t)dlsym(get_il2cpp(), OBFUSCATE("il2cpp_class_get_field_from_name"));
    return class_get_field_from_name(thisclass, name);
  }

  // template <typename T>
  // Field<T> GetFieldByName(const char *name) {
  //   return Field<T>(GetFieldInfoByName(name), (void *)thisclass);
  // }

  DWORD GetFieldOffset(const char *name) { return GetFieldInfoByName(name)->offset; }

  DWORD GetFieldOffset(FieldInfo *filed) { return filed->offset; }

  MethodInfo *GetMethodInfoByName(const char *name, int paramcount) {
    auto class_get_method_from_name =
        (class_get_method_from_name_t)dlsym(get_il2cpp(), OBFUSCATE("il2cpp_class_get_method_from_name"));
    if (thisclass)
      return class_get_method_from_name(thisclass, name, paramcount);
    else
      return nullptr;
  }

  DWORD GetMethodOffsetByName(const char *name, int paramcount) {
    auto res = GetMethodInfoByName(name, paramcount);
    if (res)
      return (DWORD)GetMethodInfoByName(name, paramcount)->methodPointer;
    else
      return 0;
  }
};

void *get_Method(const char *str) {
  void *(*il2cpp_resolve_icall_0)(const char *str) = nullptr;
  void *h                                          = nullptr;
  while (!h) {
    h = dlopen(OBFUSCATE("libil2cpp.so"), 4);
  }
  do {
    il2cpp_resolve_icall_0 = (void *(*)(const char *))dlsym(h, OBFUSCATE("il2cpp_resolve_icall"));
  } while (!il2cpp_resolve_icall_0);
  return il2cpp_resolve_icall_0(str);
}

#define InitResolveFunc(x, y) *reinterpret_cast<void **>(&x) = get_Method(y)
#define InitFunc(x, y) \
  if (y != 0) *reinterpret_cast<void **>(&x) = (void *)(y)
#define FieldBN(myfield, type, inst, nameSpacec, clazzz, fieldName, key)                            \
  Field<type> myfield = (new LoadClass(OBFUSCATE_KEY(nameSpacec, key), OBFUSCATE_KEY(clazzz, key))) \
                            ->GetFieldByName<type>(OBFUSCATE_KEY(fieldName, key));                  \
  myfield.clazz = inst
