#pragma once

#include "Bools.h"
#include "Dobby/dobby.h"
#include "KittyMemory/KittyInclude.h"
#include "Logger.h"
#include "Offsets.h"
#include "Unity/MonoString.h"
#include "Unity/Quaternion.h"
#include "obfuscate.h"
#include "Unity/Vector3.h"

extern uintptr_t g_Il2cppBase;

// ============================================================================
// BYPASS HOOK FUNCTIONS
// ============================================================================

static void *(*orig_CreateReportPlayer)(void *util, void *player, int reason) = nullptr;

static void *hook_CreateReportPlayer(void *util, void *player, int reason) {
  if (g_OtherConfig->AntiReport) {
    return nullptr;
  }
  return orig_CreateReportPlayer(util, player, reason);
}

static void *(*orig_SetReportData)(void *controller, void *data1, void *data2, void *data3, void *data4) = nullptr;

static void *hook_SetReportData(void *controller, void *data1, void *data2, void *data3, void *data4) {
  if (g_OtherConfig->AntiReport) {
    return nullptr;
  }
  return orig_SetReportData(controller, data1, data2, data3, data4);
}

static void *(*orig_CreateAddToBlackList)(void *util, void *player, int reason) = nullptr;

static void *hook_CreateAddToBlackList(void *util, void *player, int reason) {
  if (g_OtherConfig->AntiReport) {
    return nullptr;
  }
  return orig_CreateAddToBlackList(util, player, reason);
}

// ============================================================================
// UNITY ENGINE FUNCTIONS
// ============================================================================

// Camera Functions
void *get_main() { return reinterpret_cast<void *(__fastcall *)()>(Offset::Camera::get_main)(); }

static void *Camera_main() {
  void *(*_Camera_main)(void *nuls) = (void *(*)(void *))(Offset::Camera::get_main);
  return _Camera_main(nullptr);
}

float get_fieldOfView() {
  return reinterpret_cast<float(__fastcall *)(void *)>(Offset::Camera::get_fieldOfView)(get_main());
}

void *set_fieldOfView(float value) {
  return reinterpret_cast<void *(__fastcall *)(void *, float)>(Offset::Camera::set_fieldOfView)(get_main(), value);
}

static Vector3 WorldToScreenPoint(void *WorldCam, Vector3 WorldPos) {
  Vector3 (*_WorldToScreenScene)(void *Camera, Vector3 position) =
      (Vector3 (*)(void *, Vector3))(Offset::Camera::WorldToScreenPoint);
  return _WorldToScreenScene(WorldCam, WorldPos);
}

// Transform Functions
static void Transform_INTERNAL_SetPosition(void *player, Vector3 inn) {
  void (*Transform_INTERNAL_SetPosition)(void *transform, Vector3 in) =
      (void (*)(void *, Vector3))(Offset::Transform::SetPosition);
  Transform_INTERNAL_SetPosition(player, inn);
}

static Quaternion GetRotation(void *player) {
  Quaternion (*_GetRotation)(void *players) = (Quaternion (*)(void *))(Offset::Transform::Rotation);
  return _GetRotation(player);
}

static Vector3 Transform_GetPosition(void *player) {
  Vector3 out = Vector3::zero();
  void (*_Transform_GetPosition)(void *transform, Vector3 *out) =
      (void (*)(void *, Vector3 *))(Offset::Transform::GetPosition);
  _Transform_GetPosition(player, &out);
  return out;
}

Vector3 get_position(void *player) {
  Vector3 (*_get_position)(void *players) = (Vector3 (*)(void *))(Offset::Transform::Position);
  return _get_position(player);
}

static void *Component_GetTransform(void *player) {
  void *(*_Component_GetTransform)(void *component) = (void *(*)(void *))(Offset::Component::Transform);
  return _Component_GetTransform(player);
}

static Vector3 GetForward(void *player) {
  Vector3 (*_GetForward)(void *players) = (Vector3 (*)(void *))(Offset::Transform::get_forward);
  return _GetForward(player);
}

Vector3 getPosition(void *transform) { return get_position(Component_GetTransform(transform)); }

// Screen Functions
int get_width() { return reinterpret_cast<int(__fastcall *)()>(Offset::Screen::get_width)(); }

int get_height() { return reinterpret_cast<int(__fastcall *)()>(Offset::Screen::get_height)(); }

float get_density() { return reinterpret_cast<float(__fastcall *)()>(Offset::Screen::get_density)(); }

// Physics Functions
static bool Physics_Raycast(Vector3 camLocation, Vector3 headLocation, unsigned int LayerID, void *collider) {
  bool (*_Physics_Raycast)(Vector3 camLocation, Vector3 headLocation, unsigned int LayerID, void *collider) =
      (bool (*)(Vector3, Vector3, unsigned int, void *))(Offset::PhysicsSystem::Raycast);
  return _Physics_Raycast(camLocation, headLocation, LayerID, collider);
}

// ============================================================================
// GAME SPECIFIC FUNCTIONS
// ============================================================================

// Game Management
static void *Curent_Match() {
  void *(*_Curent_Match)(void *nuls) = (void *(*)(void *))(Offset::GameManagement::Match);
  return _Curent_Match(NULL);
}

static void *GetLocalPlayer(void *Game) {
  void *(*_GetLocalPlayer)(void *match) = (void *(*)(void *))(Offset::GameManagement::Local);
  return _GetLocalPlayer(Game);
}

static bool (*orig_ResetGuest)();

static bool fake_ResetGuest() { return true; }

void Patch_ResetGuest_Hook() {
  void *resetGuestAddr = (void *)(Offset::ResetGuest::Offset);
  LOGD("resetGuestAddr: %p", resetGuestAddr);
  DobbyHook(resetGuestAddr, (dobby_dummy_func_t)fake_ResetGuest, (dobby_dummy_func_t *)&orig_ResetGuest);
}

// Player Status Functions
static bool get_isVisible(void *player) {
  bool (*_get_isVisible)(void *players) = (bool (*)(void *))(Offset::PlayerStatus::Visible);
  return _get_isVisible(player);
}

static bool get_isLocalTeam(void *player) {
  bool (*_get_isLocalTeam)(void *players) = (bool (*)(void *))(Offset::PlayerStatus::Team);
  return _get_isLocalTeam(player);
}

static bool get_IsDieing(void *player) {
  bool (*_get_die)(void *players) = (bool (*)(void *))(Offset::PlayerStatus::Die);
  return _get_die(player);
}

static bool IsLocal(void *player) {
  bool (*_isMe)(void *players) = (bool (*)(void *))(Offset::PlayerStatus::LocalBool);
  return _isMe(player);
}

static bool get_IsSighting(void *player) {
  bool (*_get_IsSighting)(void *players) = (bool (*)(void *))(Offset::PlayerActions::Scope);
  return _get_IsSighting(player);
}

static bool get_IsFiring(void *player) {
  bool (*_get_IsFiring)(void *players) = (bool (*)(void *))(Offset::PlayerActions::Fire);
  return _get_IsFiring(player);
}

// Player Health Functions
static int GetHp(void *player) {
  int (*_GetHp)(void *players) = (int (*)(void *))(Offset::PlayerHealth::CurHP);
  return _GetHp(player);
}

static int get_MaxHP(void *enemy) {
  int (*_get_MaxHP)(void *player) = (int (*)(void *))(Offset::PlayerHealth::MaxHP);
  return _get_MaxHP(enemy);
}

// Player Position Functions
static void *GetHeadPositions(void *player) {
  void *(*_GetHeadPositions)(void *players) = (void *(*)(void *))(Offset::PlayerSystem::Head);
  return _GetHeadPositions(player);
}

static void *Player_GetHeadCollider(void *player) {
  void *(*_Player_GetHeadCollider)(void *players) = (void *(*)(void *))(Offset::PlayerSystem::HeadColider);
  return _Player_GetHeadCollider(player);
}

static Vector3 GetHeadPosition(void *player) { return get_position(GetHeadPositions(player)); }

static Vector3 CameraMain(void *player) {
  return get_position(*(void **)((uint64_t)player + Offset::PlayerSystem::MainCam));
}

// Player Action Functions
static void set_aim(void *player, Quaternion look) {
  void (*_set_aim)(void *players, Quaternion lock) = (void (*)(void *, Quaternion))(Offset::PlayerActions::Aim);
  _set_aim(player, look);
}

// Vehicle Functions
static void *GetLocalCar(void *playerCar) {
  void *(*_Player_get_local)(void *Player) = (void *(*)(void *))(Offset::VehicleSystem::CarLocal);
  return _Player_get_local(playerCar);
}

static bool IsDriver(void *player) {
  bool (*_IsDriver)(void *players) = (bool (*)(void *))(Offset::VehicleSystem::GetCar);
  return _IsDriver(player);
}

// Player Info Functions
static MonoString *get_NickName(void *player) {
  MonoString *(*_get_NickName)(void *players) = (MonoString * (*)(void *))(Offset::PlayerInfo::PlayerName);
  return _get_NickName(player);
}

// Better UTF-8 support function
wchar_t get_Chars(MonoString *str, int index) {
  wchar_t (*_get_Chars)(MonoString *str, int index) = (wchar_t (*)(MonoString *, int))(Offset::StringSystem::CharGet);
  return _get_Chars(str, index);
}

// ============================================================================
// ANTI-REPORT SYSTEM SETUP
// ============================================================================

// Anti-Report Setup Function
void SetupAntiReport() {
  LOGI("Setting up Anti-Report system...");

  // Hook CreateReportPlayer
  if (Offset::Bypass::Report1) {
    LOGD("Hooking CreateReportPlayer at: %p", (void *)Offset::Bypass::Report1);
    orig_CreateReportPlayer = (void *(*)(void *, void *, int))Offset::Bypass::Report1;
    DobbyHook((void *)Offset::Bypass::Report1, (void *)hook_CreateReportPlayer, (void **)&orig_CreateReportPlayer);
    LOGI("CreateReportPlayer hook installed successfully");
  } else {
    LOGE("Failed to find CreateReportPlayer address");
  }

  // Hook SetReportData
  if (Offset::Bypass::Report2) {
    LOGD("Hooking SetReportData at: %p", (void *)Offset::Bypass::Report2);
    orig_SetReportData = (void *(*)(void *, void *, void *, void *, void *))Offset::Bypass::Report2;
    DobbyHook((void *)Offset::Bypass::Report2, (void *)hook_SetReportData, (void **)&orig_SetReportData);
    LOGI("SetReportData hook installed successfully");
  } else {
    LOGE("Failed to find SetReportData address");
  }

  // Hook CreateAddToBlackList
  if (Offset::Bypass::Report4) {
    LOGD("Hooking CreateAddToBlackList at: %p", (void *)Offset::Bypass::Report4);
    orig_CreateAddToBlackList = (void *(*)(void *, void *, int))Offset::Bypass::Report4;
    DobbyHook((void *)Offset::Bypass::Report4, (void *)hook_CreateAddToBlackList, (void **)&orig_CreateAddToBlackList);
    LOGI("CreateAddToBlackList hook installed successfully");
  } else {
    LOGE("Failed to find CreateAddToBlackList address");
  }

  LOGI("Anti-Report system setup completed");
}