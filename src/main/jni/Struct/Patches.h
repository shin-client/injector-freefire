#pragma once
#include <vector>

#include "Includes/Macros.h"
#include "KittyMemory/MemoryPatch.h"
#include "Offsets.h"
#include "Unity/Il2Cpp.h"

struct PatchInfo {
  MemoryPatch patch;
  bool*       setting;
};

inline std::vector<PatchInfo> patch_infos;

inline void addNewPatch(uintptr_t offset, const char* hexPattern, bool* setting) {
  MemoryPatch newPatch = MemoryPatch::createWithHex(offset, hexPattern);
  if (!newPatch.isValid()) return;
  patch_infos.push_back({newPatch, setting});
}

inline void initPatch() {
  auto assemblyCSharp = IL2CPP::Class::Find("Assembly-CSharp", "", "");
  auto unityEngine    = IL2CPP::Class::Find("UnityEngine", "", "");
  auto mscorlib       = IL2CPP::Class::Find("mscorlib", "", "");

  LOGI("initPatch DONE");
  // // Camera
  // auto cameraClass         = IL2CPP::Class::Find("UnityEngine", "", "Camera");
  // Offset::Camera::get_main = (uintptr_t)cameraClass.GetMethod<void*>("get_main").info->methodPointer;
  // Offset::Camera::WorldToScreenPoint =
  //     (uintptr_t)cameraClass.GetMethod<void*>("WorldToScreenPoint", 1).info->methodPointer;
  // Offset::Camera::get_fieldOfView = (uintptr_t)cameraClass.GetMethod<void*>("get_fieldOfView").info->methodPointer;
  // Offset::Camera::set_fieldOfView = (uintptr_t)cameraClass.GetMethod<void*>("set_fieldOfView", 1).info->methodPointer;

  // // Input
  // auto inputClass                  = IL2CPP::Class::Find("UnityEngine", "", "Input");
  // Offset::Input::get_touchCount    = (uintptr_t)inputClass.GetMethod<void*>("get_touchCount").info->methodPointer;
  // Offset::Input::GetTouch          = (uintptr_t)inputClass.GetMethod<void*>("GetTouch", 1).info->methodPointer;
  // Offset::Input::get_mousePosition = (uintptr_t)inputClass.GetMethod<void*>("get_mousePosition").info->methodPointer;

  // // Screen
  // auto screenClass            = IL2CPP::Class::Find("UnityEngine", "", "Screen");
  // Offset::Screen::get_width   = (uintptr_t)screenClass.GetMethod<void*>("get_width").info->methodPointer;
  // Offset::Screen::get_height  = (uintptr_t)screenClass.GetMethod<void*>("get_height").info->methodPointer;
  // Offset::Screen::get_density = (uintptr_t)screenClass.GetMethod<void*>("get_dpi").info->methodPointer;

  // // Transform
  // auto transformClass = IL2CPP::Class::Find("UnityEngine", "", "Transform");
  // Offset::Transform::GetPosition =
  //     (uintptr_t)transformClass.GetMethod<void*>("get_position_Injected", 1).info->methodPointer;
  // Offset::Transform::SetPosition =
  //     (uintptr_t)transformClass.GetMethod<void*>("set_position_Injected", 1).info->methodPointer;
  // Offset::Transform::get_position = (uintptr_t)transformClass.GetMethod<void*>("get_position", 0).info->methodPointer;
  // Offset::Transform::Position     = (uintptr_t)transformClass.GetMethod<void*>("get_position", 0).info->methodPointer;
  // Offset::Transform::Rotation     = (uintptr_t)transformClass.GetMethod<void*>("get_rotation", 0).info->methodPointer;
  // Offset::Transform::get_forward  = (uintptr_t)transformClass.GetMethod<void*>("get_forward", 0).info->methodPointer;

  // // Component
  // auto componentClass              = IL2CPP::Class::Find("UnityEngine", "", "Component");
  // Offset::Component::get_transform = (uintptr_t)componentClass.GetMethod<void*>("get_transform", 0).info->methodPointer;
  // Offset::Component::Transform     = (uintptr_t)componentClass.GetMethod<void*>("get_transform", 0).info->methodPointer;

  // // Time
  // auto timeClass              = IL2CPP::Class::Find("UnityEngine", "", "Time");
  // Offset::Time::get_deltaTime = (uintptr_t)timeClass.GetMethod<void*>("get_deltaTime", 0).info->methodPointer;

  // // Player
  // auto playerClass                  = IL2CPP::Class::Find("COW.GamePlay", "", "Player");
  // Offset::PlayerSystem::EnemyUpdate = (uintptr_t)playerClass.GetMethod<void*>("LateUpdate", 0).info->methodPointer;
  // Offset::PlayerSystem::Head        = (uintptr_t)playerClass.GetMethod<void*>("GetHeadTF", 0).info->methodPointer;
  // Offset::PlayerSystem::HeadColider =
  //     (uintptr_t)playerClass.GetMethod<void*>("get_HeadCollider", 0).info->methodPointer;
  // Offset::PlayerSystem::MainCam = (uintptr_t)playerClass.GetField<void*>("MainCameraTransform").info->offset;
  // Offset::PlayerSystem::ListPlayer =
  //     (uintptr_t)IL2CPP::Class::Find("COW.GamePlay", "", "NFJPHMKKEBF").GetField<void*>("HOOCHDLKOOG").info->offset;

  // Offset::PlayerStatus::Visible   = (uintptr_t)playerClass.GetMethod<void*>("IsVisible", 0).info->methodPointer;
  // Offset::PlayerStatus::Team      = (uintptr_t)playerClass.GetMethod<void*>("IsLocalTeammate", 0).info->methodPointer;
  // Offset::PlayerStatus::Die       = (uintptr_t)playerClass.GetMethod<void*>("get_IsDieing", 0).info->methodPointer;
  // Offset::PlayerStatus::LocalBool = (uintptr_t)playerClass.GetMethod<void*>("IsLocalPlayer", 0).info->methodPointer;

  // Offset::PlayerHealth::CurHP = (uintptr_t)playerClass.GetMethod<void*>("get_CurHP", 0).info->methodPointer;
  // Offset::PlayerHealth::MaxHP = (uintptr_t)playerClass.GetMethod<void*>("get_MaxHP", 0).info->methodPointer;

  // Offset::PlayerInfo::PlayerName = (uintptr_t)playerClass.GetMethod<void*>("get_NickName", 0).info->methodPointer;

  // Offset::PlayerActions::Aim   = (uintptr_t)playerClass.GetMethod<void*>("SetAimRotation", 1).info->methodPointer;
  // Offset::PlayerActions::Scope = (uintptr_t)playerClass.GetMethod<void*>("get_IsSighting", 0).info->methodPointer;
  // Offset::PlayerActions::Fire  = (uintptr_t)playerClass.GetMethod<void*>("IsFiring", 0).info->methodPointer;

  // Offset::VehicleSystem::CarLocal = (uintptr_t)playerClass.GetMethod<void*>("VehicleIAmIn", 0).info->methodPointer;
  // Offset::VehicleSystem::GetCar   = (uintptr_t)playerClass.GetMethod<void*>("get_IsDriver", 0).info->methodPointer;

  // Offset::GameManagement::Match =
  //     (uintptr_t)IL2CPP::Class::Find("COW", "", "GameFacade").GetMethod<void*>("CurrentMatch", 0).info->methodPointer;
  // Offset::GameManagement::Local = (uintptr_t)IL2CPP::Class::Find("COW", "", "UIHudDetectorController")
  //                                     .GetMethod<void*>("GetLocalPlayer", 0)
  //                                     .info->methodPointer;

  // Offset::PhysicsSystem::Raycast = (uintptr_t)IL2CPP::Class::Find("COW.GamePlay", "", "JEAGCMACNNC")
  //                                      .GetMethod<void*>("PLDCHDBCOBF", 4)
  //                                      .info->methodPointer;

  // Offset::StringSystem::CharGet =
  //     (uintptr_t)IL2CPP::Class::Find("System", "", "String").GetMethod<void*>("get_Chars", 1).info->methodPointer;

  // Offset::ResetGuest::Offset =
  //     (uintptr_t)IL2CPP::Class::Find("COW", "", "GameConfig").GetMethod<void*>("get_ResetGuest", 0).info->methodPointer;

  // Offset::Bypass::Report1 = (uintptr_t)IL2CPP::Class::Find("COW", "", "InteractionPopUtil")
  //                               .GetMethod<void*>("CreateReportPlayer", 3)
  //                               .info->methodPointer;
  // Offset::Bypass::Report2 = (uintptr_t)IL2CPP::Class::Find("COW", "", "UIHudMatchResultItemBaseController")
  //                               .GetMethod<void*>("SetReportData", 4)
  //                               .info->methodPointer;
  // Offset::Bypass::Report3 = (uintptr_t)IL2CPP::Class::Find("COW", "", "UIModelVoice")
  //                               .GetMethod<void*>("get_ReportedPlayerTimeDict", 0)
  //                               .info->methodPointer;
  // Offset::Bypass::Report4 = (uintptr_t)IL2CPP::Class::Find("COW", "", "InteractionPopUtil")
  //                               .GetMethod<void*>("CreateAddToBlackList", 3)
  //                               .info->methodPointer;

  // patchOffset(targetLibName, Offset::PlayerActions::Aim, "C0035FD6", true);
  // patchOffset(targetLibName, Offset::PlayerActions::Scope, "C0035FD6", true);
  // patchOffset(targetLibName, Offset::PlayerActions::Fire, "20008052C0035FD6", true);
}