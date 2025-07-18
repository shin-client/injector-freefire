
#include <android/log.h>
#include <unistd.h>

#include <cstring>
#include <map>
#include <string>
#include <vector>

#include "il2Cpp.h"
#include "xdl/xdl.h"

// ===================================================================================
// PHẦN 1: CÁC CON TRỎ HÀM TOÀN CỤC (BỘ NÃO)
// ===================================================================================

namespace {
  // Các con trỏ này sẽ lưu địa chỉ của các hàm IL2CPP sau khi được tìm thấy
  // Đây là một cải tiến về hiệu năng, vì chúng ta chỉ cần tìm địa chỉ một lần duy nhất.

  // Hàm quản lý Domain và Assembly
  void* (*il2cpp_domain_get)();
  const Il2CppAssembly** (*il2cpp_domain_get_assemblies)(const void* domain, size_t* size);
  const Il2CppImage* (*il2cpp_assembly_get_image)(const Il2CppAssembly* assembly);
  const char* (*il2cpp_image_get_name)(const Il2CppImage* image);

  // Hàm quản lý Class và Type
  Il2CppClass* (*il2cpp_class_from_name)(const Il2CppImage* image, const char* namespaze, const char* name);
  const Il2CppType* (*il2cpp_class_get_type)(Il2CppClass* klass);
  const char* (*il2cpp_type_get_name)(const Il2CppType* type);

  // Hàm quản lý Method
  const MethodInfo* (*il2cpp_class_get_method_from_name_ptr)(Il2CppClass* klass, const char* name, int argsCount);
  const char* (*il2cpp_method_get_name)(const MethodInfo* method);
  const ParameterInfo* (*il2cpp_method_get_param)(const MethodInfo* method, uint32_t index);

  // Hàm quản lý Field
  FieldInfo* (*il2cpp_class_get_field_from_name)(Il2CppClass* klass, const char* name);
  size_t (*il2cpp_field_get_offset)(const FieldInfo* field);
  void (*il2cpp_field_static_get_value)(FieldInfo* field, void* value);
  void (*il2cpp_field_static_set_value)(FieldInfo* field, void* value);

  // Hàm quản lý Object và String
  Il2CppObject* (*il2cpp_object_new)(Il2CppClass* klass);
  Il2CppString* (*il2cpp_string_new)(const char* str);
  void* (*il2cpp_array_new)(Il2CppClass* elementTypeInfo, il2cpp_array_size_t length);

  // Hàm gọi runtime
  Il2CppObject* (*il2cpp_runtime_invoke)(const MethodInfo* method, void* obj, void** params, Il2CppObject** exc);
}

// ===================================================================================
// PHẦN 2: HÀM KHỞI TẠO CHÍNH (ATTACH)
// ===================================================================================

void Il2CppAttach(const char* name) {
  // Mở file libil2cpp.so
  void* handle = xdl_open(name, 0);
  if (!handle) {
    // Lặp lại nếu chưa thành công, vì game có thể chưa nạp thư viện
    while (!handle) {
      handle = xdl_open(name, 0);
      sleep(1);
    }
  }

  // Sử dụng xdl_sym để tìm địa chỉ của từng hàm và gán vào con trỏ tương ứng
  // Đây là phần "nạp não" cho thư viện của bạn.
  il2cpp_domain_get = (decltype(il2cpp_domain_get))xdl_sym(handle, "il2cpp_domain_get", nullptr);
  il2cpp_domain_get_assemblies =
      (decltype(il2cpp_domain_get_assemblies))xdl_sym(handle, "il2cpp_domain_get_assemblies", nullptr);
  il2cpp_assembly_get_image =
      (decltype(il2cpp_assembly_get_image))xdl_sym(handle, "il2cpp_assembly_get_image", nullptr);
  il2cpp_image_get_name  = (decltype(il2cpp_image_get_name))xdl_sym(handle, "il2cpp_image_get_name", nullptr);
  il2cpp_class_from_name = (decltype(il2cpp_class_from_name))xdl_sym(handle, "il2cpp_class_from_name", nullptr);
  il2cpp_class_get_type  = (decltype(il2cpp_class_get_type))xdl_sym(handle, "il2cpp_class_get_type", nullptr);
  il2cpp_type_get_name   = (decltype(il2cpp_type_get_name))xdl_sym(handle, "il2cpp_type_get_name", nullptr);
  il2cpp_class_get_method_from_name_ptr =
      (decltype(il2cpp_class_get_method_from_name_ptr))xdl_sym(handle, "il2cpp_class_get_method_from_name", nullptr);
  il2cpp_method_get_name  = (decltype(il2cpp_method_get_name))xdl_sym(handle, "il2cpp_method_get_name", nullptr);
  il2cpp_method_get_param = (decltype(il2cpp_method_get_param))xdl_sym(handle, "il2cpp_method_get_param", nullptr);
  il2cpp_class_get_field_from_name =
      (decltype(il2cpp_class_get_field_from_name))xdl_sym(handle, "il2cpp_class_get_field_from_name", nullptr);
  il2cpp_field_get_offset = (decltype(il2cpp_field_get_offset))xdl_sym(handle, "il2cpp_field_get_offset", nullptr);
  il2cpp_field_static_get_value =
      (decltype(il2cpp_field_static_get_value))xdl_sym(handle, "il2cpp_field_static_get_value", nullptr);
  il2cpp_field_static_set_value =
      (decltype(il2cpp_field_static_set_value))xdl_sym(handle, "il2cpp_field_static_set_value", nullptr);
  il2cpp_object_new     = (decltype(il2cpp_object_new))xdl_sym(handle, "il2cpp_object_new", nullptr);
  il2cpp_string_new     = (decltype(il2cpp_string_new))xdl_sym(handle, "il2cpp_string_new", nullptr);
  il2cpp_array_new      = (decltype(il2cpp_array_new))xdl_sym(handle, "il2cpp_array_new", nullptr);
  il2cpp_runtime_invoke = (decltype(il2cpp_runtime_invoke))xdl_sym(handle, "il2cpp_runtime_invoke", nullptr);

  // Không cần đóng handle vì chúng ta sẽ cần nó trong suốt quá trình chạy
  // xdl_close(handle);
}

// ===================================================================================
// PHẦN 3: CÁC HÀM TIỆN ÍCH C-STYLE (BACKEND CHO CÁC LỚP WRAPPER)
// ===================================================================================

const char* Il2CppString::CString() {
  // Chuyển đổi chuỗi UTF-16 của Il2Cpp sang UTF-8
  std::u16string u16_str(reinterpret_cast<const char16_t*>(&this->start_char), this->length);
  // Sử dụng C++11 wstring_convert để chuyển đổi
  thread_local static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
  return convert.to_bytes(u16_str).c_str();
}

Il2CppString* Il2CppString::Create(const char* s) {
  if (il2cpp_string_new) {
    return il2cpp_string_new(s);
  }
  return nullptr;
}

void* Il2CppGetImageByName(const char* image_name) {
  if (!il2cpp_domain_get_assemblies) return nullptr;

  size_t                 size;
  const Il2CppAssembly** assemblies = il2cpp_domain_get_assemblies(il2cpp_domain_get(), &size);
  for (int i = 0; i < size; ++i) {
    const Il2CppImage* img      = il2cpp_assembly_get_image(assemblies[i]);
    const char*        img_name = il2cpp_image_get_name(img);
    if (strcmp(img_name, image_name) == 0) {
      return (void*)img;
    }
  }
  return nullptr;
}

void* Il2CppGetClassType(const char* image, const char* namespaze, const char* clazz) {
  // Sử dụng map để cache kết quả, tăng tốc độ cho các lần gọi sau
  static std::map<std::string, void*> cache;
  std::string                         key = std::string(image) + "/" + namespaze + "/" + clazz;
  if (cache.count(key) > 0) {
    return cache[key];
  }

  void* img = Il2CppGetImageByName(image);
  if (!img) {
    // LOGE("Can't find image %s!", image);
    return nullptr;
  }

  void* klass = il2cpp_class_from_name((const Il2CppImage*)img, namespaze, clazz);
  if (!klass) {
    // LOGE("Can't find class %s::%s in image %s!", namespaze, clazz, image);
    return nullptr;
  }

  cache[key] = klass;
  return klass;
}

// Cung cấp các hàm này để các lớp wrapper trong Il2Cpp.h có thể sử dụng
void* il2cpp_class_get_field_from_name_c(void* klass, const char* name) {
  if (::il2cpp_class_get_field_from_name) {
    return ::il2cpp_class_get_field_from_name((Il2CppClass*)klass, name);
  }
  return nullptr;
}

void* il2cpp_class_get_method_from_name(void* klass, const char* name, int argsCount) {
  if (il2cpp_class_get_method_from_name_ptr) {
    return (void*)il2cpp_class_get_method_from_name_ptr((Il2CppClass*)klass, name, argsCount);
  }
  return nullptr;
}

// Các hàm tiện ích khác có thể được thêm vào đây nếu cần
void* Il2CppCreateClassInstance(const char* image, const char* namespaze, const char* clazz) {
  void* klass = Il2CppGetClassType(image, namespaze, clazz);
  if (!klass) return nullptr;
  return il2cpp_object_new((Il2CppClass*)klass);
}

void* Il2CppCreateArray(const char* image, const char* namespaze, const char* clazz, size_t length) {
  void* klass = Il2CppGetClassType(image, namespaze, clazz);
  if (!klass) return nullptr;
  return il2cpp_array_new((Il2CppClass*)klass, length);
}

void Il2CppGetStaticFieldValue(const char* image, const char* namespaze, const char* clazz, const char* name,
                               void* output) {
  void* klass = Il2CppGetClassType(image, namespaze, clazz);
  if (!klass) return;
FieldInfo* field = (FieldInfo*)il2cpp_class_get_field_from_name_c((Il2CppClass*)klass, name);
  if (!field) return;
  il2cpp_field_static_get_value(field, output);
}

void Il2CppSetStaticFieldValue(const char* image, const char* namespaze, const char* clazz, const char* name,
                               void* value) {
  void* klass = Il2CppGetClassType(image, namespaze, clazz);
  if (!klass) return;
  FieldInfo* field = il2cpp_class_get_field_from_name((Il2CppClass*)klass, name);
  if (!field) return;
  il2cpp_field_static_set_value(field, value);
}

// Cập nhật hàm này để trả về con trỏ hàm thực thi
void* Il2CppGetMethodOffset(const char* image, const char* namespaze, const char* clazz, const char* name,
                            int argsCount) {
  void* klass = Il2CppGetClassType(image, namespaze, clazz);
  if (!klass) return nullptr;
  const MethodInfo* method = il2cpp_class_get_method_from_name_ptr((Il2CppClass*)klass, name, argsCount);
  if (!method) return nullptr;
  return (void*)method->methodPointer;
}

size_t Il2CppGetFieldOffset(const char* image, const char* namespaze, const char* clazz, const char* name) {
  void* klass = Il2CppGetClassType(image, namespaze, clazz);
  if (!klass) return -1;
  FieldInfo* field = il2cpp_class_get_field_from_name((Il2CppClass*)klass, name);
  if (!field) return -1;
  return il2cpp_field_get_offset(field);
}
