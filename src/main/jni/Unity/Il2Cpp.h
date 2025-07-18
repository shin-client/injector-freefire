#pragma once

#include <dlfcn.h>
#include <inttypes.h>
#include <stdio.h>

#include <codecvt>
#include <locale>
#include <string>

// ===================================================================================
// PHẦN 1: CÁC ĐỊNH NGHĨA CƠ BẢN VÀ CẤU TRÚC IL2CPP CHO UNITY 2022.2+
// ===================================================================================

typedef void (*Il2CppMethodPointer)();
typedef int32_t il2cpp_array_size_t;

struct Il2CppObject;
struct Il2CppClass;
struct MethodInfo;
struct FieldInfo;
struct Il2CppImage;
struct Il2CppAssembly;
struct Il2CppDomain;
struct Il2CppString;
struct Il2CppArrayBounds;
struct ParameterInfo;

// Cấu trúc Il2CppType hiện đại, sử dụng bitfield
struct Il2CppType {
  union {
    void*             dummy;
    Il2CppClass*      klass;
    const Il2CppType* type;
    void*             array;
    // ...
  } data;

  unsigned int attrs : 16;
  unsigned int type : 8;
  unsigned int num_mods : 6;
  unsigned int byref : 1;
  unsigned int pinned : 1;
};

struct Il2CppObject {
  Il2CppClass* klass;
  void*        monitor;
};

// Cấu trúc Il2CppArray ĐÚNG
template <typename T>
struct Il2CppArray {
  Il2CppObject        obj;
  Il2CppArrayBounds*  bounds;
  il2cpp_array_size_t max_length;
  T                   m_Items[1];  // Kỹ thuật flexible array member

  int getLength() { return max_length; }

  T* getPointer() { return (T*)m_Items; }

  T& operator[](int i) { return m_Items[i]; }

  const T& operator[](int i) const { return m_Items[i]; }
};

struct Il2CppString {
  Il2CppObject object;
  int32_t      length;
  uint16_t     start_char;

  const char*          CString();
  static Il2CppString* Create(const char* s);
};

struct VirtualInvokeData {
  Il2CppMethodPointer methodPtr;
  const MethodInfo*   method;
};

// Cấu trúc Il2CppClass hiện đại (một struct duy nhất)
struct Il2CppClass {
  const Il2CppImage* image;
  void*              gc_desc;
  const char*        name;
  const char*        namespaze;
  Il2CppType         byval_arg;
  Il2CppType         this_arg;
  Il2CppClass*       element_class;
  Il2CppClass*       castClass;
  Il2CppClass*       declaringType;
  Il2CppClass*       parent;
  void*              generic_class;
  const void*        typeMetadataHandle;
  void*              interopData;
  Il2CppClass*       klass;
  FieldInfo*         fields;
  const void*        events;
  const void*        properties;
  const MethodInfo** methods;
  Il2CppClass**      nestedTypes;
  Il2CppClass**      implementedInterfaces;
  void*              interfaceOffsets;
  void*              static_fields;
  void*              rgctx_data;
  Il2CppClass**      typeHierarchy;
  void*              unity_user_data;
  uint32_t           initializationExceptionGCHandle;
  uint32_t           cctor_started;
  uint32_t           cctor_finished_or_no_cctor;
  size_t             cctor_thread;
  const void*        genericContainerHandle;
  uint32_t           instance_size;
  uint32_t           actualSize;
  uint32_t           element_size;
  int32_t            native_size;
  uint32_t           static_fields_size;
  uint32_t           thread_static_fields_size;
  int32_t            thread_static_fields_offset;
  uint32_t           flags;
  uint32_t           token;
  uint16_t           method_count;
  uint16_t           property_count;
  uint16_t           field_count;
  uint16_t           event_count;
  uint16_t           nested_type_count;
  uint16_t           vtable_count;
  uint16_t           interfaces_count;
  uint16_t           interface_offsets_count;
  uint8_t            typeHierarchyDepth;
  uint8_t            genericRecursionDepth;
  uint8_t            rank;
  uint8_t            minimumAlignment;
  uint8_t            naturalAligment;
  uint8_t            packingSize;
  uint8_t            bitflags;
  VirtualInvokeData  vtable[255];  // Giữ nguyên để đơn giản, nhưng cần cẩn thận
};

struct ParameterInfo {
  const char*       name;
  int32_t           position;
  uint32_t          token;
  const Il2CppType* parameter_type;
};

// Cấu trúc MethodInfo hiện đại
struct MethodInfo {
  Il2CppMethodPointer  methodPointer;
  Il2CppMethodPointer  virtualMethodPointer;
  void*                invoker_method;
  const char*          name;
  Il2CppClass*         klass;
  const Il2CppType*    return_type;
  const ParameterInfo* parameters;
  void*                rgctx_data;
  void*                methodDefinition;
  void*                genericMethod;
  void*                genericContainer;
  uint32_t             token;
  uint16_t             flags;
  uint16_t             iflags;
  uint16_t             slot;
  uint8_t              parameters_count;
  uint8_t              is_generic : 1;
  uint8_t              is_inflated : 1;
  uint8_t              wrapper_type : 1;
  uint8_t              is_marshaled_from_native : 1;
};

struct FieldInfo {
  const char*       name;
  const Il2CppType* type;
  Il2CppClass*      parent;
  int32_t           offset;
  uint32_t          token;
};

// ===================================================================================
// PHẦN 2: CÁC LỚP WRAPPER C++ ĐÃ ĐƯỢC CẢI TIẾN VÀ CẬP NHẬT
// ===================================================================================

// Khai báo trước các hàm C-style
void* Il2CppGetClassType(const char* image, const char* namespaze, const char* clazz);
void* il2cpp_class_get_field_from_name_c(void* klass, const char* name);
void* il2cpp_class_get_method_from_name(void* klass, const char* name, int argsCount);

namespace IL2CPP {
  // Khai báo trước các lớp wrapper
  struct Method;
  template <typename Ret>
  struct MethodT;
  struct Field;
  template <typename T>
  struct FieldT;

  struct Class {
    Il2CppClass* klass;

    Class(Il2CppClass* ptr = nullptr) : klass(ptr) {}

    static Class Find(const char* image, const char* namespaze, const char* name) {
      return Class((Il2CppClass*)Il2CppGetClassType(image, namespaze, name));
    }

    bool IsValid() const { return klass != nullptr; }

    template <typename T>
    FieldT<T> GetField(const char* name);

    template <typename Ret>
    MethodT<Ret> GetMethod(const char* name, int argsCount = 0);
  };

  struct Field {
    FieldInfo*    info;
    Il2CppObject* instance;

    void* GetPointer() const {
      if (!info) return nullptr;
      // Sử dụng trường 'attrs' từ Il2CppType hiện đại
      if (info->type->attrs & 0x0010 /* FIELD_ATTRIBUTE_STATIC */) {
        uintptr_t static_fields = (uintptr_t)info->parent->static_fields;
        return (void*)(static_fields + info->offset);
      }
      if (!instance) return nullptr;
      return (void*)((uintptr_t)instance + info->offset);
    }
  };

  template <typename T>
  struct FieldT : public Field {
    FieldT(FieldInfo* f = nullptr, Il2CppObject* i = nullptr) {
      info     = f;
      instance = i;
    }

    FieldT& operator[](void* obj) {
      instance = (Il2CppObject*)obj;
      return *this;
    }

    T Get() const {
      T* ptr = (T*)GetPointer();
      if (ptr) return *ptr;
      return T{};
    }

    void Set(T value) const {
      T* ptr = (T*)GetPointer();
      if (ptr) *ptr = value;
    }

    operator T() const { return Get(); }

    void operator=(T value) const { Set(value); }
  };

  struct Method {
    MethodInfo*   info;
    Il2CppObject* instance;
  };

  template <typename Ret>
  struct MethodT : public Method {
    MethodT(MethodInfo* m = nullptr, Il2CppObject* i = nullptr) {
      info     = m;
      instance = i;
    }

    MethodT& operator[](void* obj) {
      instance = (Il2CppObject*)obj;
      return *this;
    }

    // Cập nhật hàm Call cho quy ước gọi hàm của Unity hiện đại (truyền MethodInfo*)
    template <typename... Args>
    Ret Call(Args... args) const {
      if (!info || !info->methodPointer) {
        // Trả về giá trị mặc định nếu là kiểu void thì không làm gì
        if constexpr (std::is_same_v<Ret, void>)
          return;
        else
          return Ret{};
      }

      // Ép kiểu con trỏ hàm về đúng chữ ký, thêm 'const MethodInfo*' ở cuối
      auto func = (Ret (*)(void*, Args..., const MethodInfo*))info->methodPointer;

      if (info->flags & 0x0010 /* METHOD_ATTRIBUTE_STATIC */) {
        return func(nullptr, args..., info);
      } else {
        if (!instance) {
          if constexpr (std::is_same_v<Ret, void>)
            return;
          else
            return Ret{};
        }
        return func(instance, args..., info);
      }
    }

    template <typename... Args>
    Ret operator()(Args... args) const {
      return Call(args...);
    }
  };

  // Triển khai các hàm của Class sau khi FieldT và MethodT đã được định nghĩa
  template <typename T>
  FieldT<T> Class::GetField(const char* name) {
    if (!klass) return {nullptr, nullptr};
    FieldInfo* fieldInfo = (FieldInfo*)il2cpp_class_get_field_from_name_c(klass, name);
    return {fieldInfo, nullptr};
  }

  template <typename Ret>
  MethodT<Ret> Class::GetMethod(const char* name, int argsCount) {
    if (!klass) return {nullptr, nullptr};
    MethodInfo* methodInfo = (MethodInfo*)il2cpp_class_get_method_from_name(klass, name, argsCount);
    return {methodInfo, nullptr};
  }
}

// ===================================================================================
// PHẦN 3: KHAI BÁO CÁC HÀM TIỆN ÍCH C-STYLE
// ===================================================================================

void   Il2CppAttach(const char* name = "libil2cpp.so");
void*  Il2CppGetImageByName(const char* image);
void*  Il2CppGetClassType(const char* image, const char* namespaze, const char* clazz);
void*  Il2CppCreateClassInstance(const char* image, const char* namespaze, const char* clazz);
void*  Il2CppCreateArray(const char* image, const char* namespaze, const char* clazz, size_t length);
void   Il2CppGetStaticFieldValue(const char* image, const char* namespaze, const char* clazz, const char* name,
                                 void* output);
void   Il2CppSetStaticFieldValue(const char* image, const char* namespaze, const char* clazz, const char* name,
                                 void* value);
void*  Il2CppGetMethodOffset(const char* image, const char* namespaze, const char* clazz, const char* name,
                             int argsCount = 0);
void*  Il2CppGetMethodOffset(const char* image, const char* namespaze, const char* clazz, const char* name, char** args,
                             int argsCount);
size_t Il2CppGetFieldOffset(const char* image, const char* namespaze, const char* clazz, const char* name);
size_t Il2CppGetStaticFieldOffset(const char* image, const char* namespaze, const char* clazz, const char* name);
bool   Il2CppIsAssembliesLoaded();
