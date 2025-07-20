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
  // Camera
  Offset::Camera::get_main = (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"),
                                                              OBFUSCATE("Camera"), OBFUSCATE("get_main"));
  Offset::Camera::WorldToScreenPoint = (uintptr_t)Il2CppGetMethodOffset(
      OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Camera"), OBFUSCATE("WorldToScreenPoint"), 1);
  Offset::Camera::get_fieldOfView = (uintptr_t)Il2CppGetMethodOffset(
      OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Camera"), OBFUSCATE("get_fieldOfView"));
  Offset::Camera::set_fieldOfView = (uintptr_t)Il2CppGetMethodOffset(
      OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Camera"), OBFUSCATE("set_fieldOfView"), 1);

  // Input
  Offset::Input::get_touchCount = (uintptr_t)Il2CppGetMethodOffset(
      OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Input"), OBFUSCATE("get_touchCount"));
  Offset::Input::GetTouch = (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"),
                                                             OBFUSCATE("Input"), OBFUSCATE("GetTouch"), 1);
  Offset::Input::get_mousePosition = (uintptr_t)Il2CppGetMethodOffset(
      OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Input"), OBFUSCATE("get_mousePosition"));

  // Screen
  Offset::Screen::get_width   = (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"),
                                                                 OBFUSCATE("Screen"), OBFUSCATE("get_width"));
  Offset::Screen::get_height  = (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"),
                                                                 OBFUSCATE("Screen"), OBFUSCATE("get_height"));
  Offset::Screen::get_density = (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"),
                                                                 OBFUSCATE("Screen"), OBFUSCATE("get_dpi"));

  // Transform
  Offset::Transform::GetPosition =
      (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"),
                                       OBFUSCATE("get_position_Injected"), 1);
  Offset::Transform::SetPosition =
      (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"),
                                       OBFUSCATE("set_position_Injected"), 1);
  Offset::Transform::get_position = (uintptr_t)Il2CppGetMethodOffset(
      OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_position"), 0);
  Offset::Transform::Position = (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"),
                                                                 OBFUSCATE("Transform"), OBFUSCATE("get_position"), 0);
  Offset::Transform::Rotation = (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"),
                                                                 OBFUSCATE("Transform"), OBFUSCATE("get_rotation"), 0);
  Offset::Transform::get_forward = (uintptr_t)Il2CppGetMethodOffset(
      OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_forward"), 0);

  // Component
  Offset::Component::get_transform = (uintptr_t)Il2CppGetMethodOffset(
      OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Component"), OBFUSCATE("get_transform"), 0);

  // Time
  Offset::Time::get_deltaTime = (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"),
                                                                 OBFUSCATE("Time"), OBFUSCATE("get_deltaTime"), 0);

  // Player
  Offset::PlayerSystem::EnemyUpdate = (uintptr_t)Il2CppGetMethodOffset(
      OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("LateUpdate"), 0);
  Offset::PlayerSystem::Head = (uintptr_t)Il2CppGetMethodOffset(
      OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetHeadTF"), 0);
  Offset::PlayerSystem::HeadColider =
      (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"),
                                       OBFUSCATE("get_HeadCollider"), 0);
  Offset::PlayerSystem::MainCam =
      (uintptr_t)Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"),
                                      OBFUSCATE("MainCameraTransform"));
  Offset::PlayerSystem::ListPlayer = (uintptr_t)Il2CppGetFieldOffset(
      OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("NFJPHMKKEBF"), OBFUSCATE("HOOCHDLKOOG"));

  Offset::PlayerStatus::Visible = (uintptr_t)Il2CppGetMethodOffset(
      OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("IsVisible"), 0);
  Offset::PlayerStatus::Team =
      (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"),
                                       OBFUSCATE("IsLocalTeammate"), 0);
  Offset::PlayerStatus::Die = (uintptr_t)Il2CppGetMethodOffset(
      OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_IsDieing"), 0);
  Offset::PlayerStatus::LocalBool = (uintptr_t)Il2CppGetMethodOffset(
      OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("IsLocalPlayer"), 0);

  Offset::PlayerHealth::CurHP = (uintptr_t)Il2CppGetMethodOffset(
      OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_CurHP"), 0);
  Offset::PlayerHealth::MaxHP = (uintptr_t)Il2CppGetMethodOffset(
      OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_MaxHP"), 0);

  Offset::PlayerInfo::PlayerName = (uintptr_t)Il2CppGetMethodOffset(
      OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_NickName"), 0);

  Offset::PlayerActions::Aim = (uintptr_t)Il2CppGetMethodOffset(
      OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("SetAimRotation"), 1);
  Offset::PlayerActions::Scope = (uintptr_t)Il2CppGetMethodOffset(
      OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_IsSighting"), 0);
  Offset::PlayerActions::Fire = (uintptr_t)Il2CppGetMethodOffset(
      OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("IsFiring"), 0);

  Offset::VehicleSystem::CarLocal = (uintptr_t)Il2CppGetMethodOffset(
      OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("VehicleIAmIn"), 0);
  Offset::VehicleSystem::GetCar = (uintptr_t)Il2CppGetMethodOffset(
      OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_IsDriver"), 0);

  Offset::GameManagement::Match = (uintptr_t)Il2CppGetMethodOffset(
      OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("CurrentMatch"), 0);
  Offset::GameManagement::Local =
      (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"),
                                       OBFUSCATE("UIHudDetectorController"), OBFUSCATE("GetLocalPlayer"), 0);

  Offset::PhysicsSystem::Raycast =
      (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"),
                                       OBFUSCATE("JEAGCMACNNC"), OBFUSCATE("PLDCHDBCOBF"), 4);

  Offset::StringSystem::CharGet = (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("mscorlib.dll"), OBFUSCATE("System"),
                                                                   OBFUSCATE("String"), OBFUSCATE("get_Chars"), 1);

  Offset::Bypass::Report1 =
      (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"),
                                       OBFUSCATE("InteractionPopUtil"), OBFUSCATE("CreateReportPlayer"), 3);
  Offset::Bypass::Report2 =
      (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"),
                                       OBFUSCATE("UIHudMatchResultItemBaseController"), OBFUSCATE("SetReportData"), 4);
  Offset::Bypass::Report3 =
      (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("UIModelVoice"),
                                       OBFUSCATE("get_ReportedPlayerTimeDict"), 0);
  Offset::Bypass::Report4 =
      (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"),
                                       OBFUSCATE("InteractionPopUtil"), OBFUSCATE("CreateAddToBlackList"), 3);

  Offset::GameManagement::ResetGuest = (uintptr_t)Il2CppGetMethodOffset(
      OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameConfig"), OBFUSCATE("get_ResetGuest"));

  // patchOffset(targetLibName, Offset::PlayerActions::Aim, "C0035FD6", true);
  // patchOffset(targetLibName, Offset::PlayerActions::Scope, "C0035FD6", true);
  // patchOffset(targetLibName, Offset::PlayerActions::Fire, "20008052C0035FD6", true);
}