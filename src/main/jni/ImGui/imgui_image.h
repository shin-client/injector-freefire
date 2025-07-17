#include "imgui.h"
#include "imgui/stb_image.h"

struct TextureInfo {
  ImTextureID textureId;
  int         x;
  int         y;
  int         w;
  int         h;
};

TextureInfo CreateTexture(const unsigned char *buf, int len) {
  TextureInfo    image;
  unsigned char *image_data = stbi_load_from_memory(buf, len, &image.w, &image.h, NULL, 4);
  if (image_data == NULL) perror("File not found!");
  GLuint image_texture;
  glGenTextures(1, &image_texture);
  glBindTexture(GL_TEXTURE_2D, image_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.w, image.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
  stbi_image_free(image_data);
  image.textureId = (ImTextureID)image_texture;
  return image;
}