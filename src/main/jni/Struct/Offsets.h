#pragma once

#include <cstdint>

namespace Offset {
  namespace Camera {
    uint64_t get_main = 0x0;
    uint64_t WorldToScreenPoint = 0x0;
    uint64_t get_fieldOfView = 0x0;
    uint64_t set_fieldOfView = 0x0;
  }
  namespace Input {
    uint64_t get_touchCount = 0x0;
    uint64_t GetTouch = 0x0;
    uint64_t get_mousePosition = 0x0;
  }
  namespace Screen {
    uint64_t get_width = 0x0;
    uint64_t get_height = 0x0;
    uint64_t get_density = 0x0;
  }
  namespace Transform {
    uint64_t GetPosition = 0x0;
    uint64_t SetPosition = 0x0;
    uint64_t get_position = 0x0;
    uint64_t Position = 0x0;
    uint64_t Rotation = 0x0;
    uint64_t get_forward = 0x0;
  }
  namespace Component {
    uint64_t get_transform = 0x0;
    uint64_t Transform = 0x0;
  }
  namespace Time {
    uint64_t get_deltaTime = 0x0;
  }
  namespace PlayerSystem {
    uint64_t ListPlayer = 0x0;
    uint64_t EnemyUpdate = 0x0;
    uint64_t MainCam = 0x0;
    uint64_t Head = 0x0;
    uint64_t HeadColider = 0x0;
  }
  namespace PlayerStatus {
    uint64_t Visible = 0x0;
    uint64_t Team = 0x0;
    uint64_t Die = 0x0;
    uint64_t LocalBool = 0x0;
  }
  namespace PlayerHealth {
    uint64_t CurHP = 0x0;
    uint64_t MaxHP = 0x0;
  }
  namespace PlayerInfo {
    uint64_t PlayerName = 0x0;
  }
  namespace PlayerActions {
    uint64_t Aim = 0x0;
    uint64_t Scope = 0x0;
    uint64_t Fire = 0x0;
  }
  namespace VehicleSystem {
    uint64_t CarLocal = 0x0;
    uint64_t GetCar = 0x0;
  }
  namespace GameManagement {
    uint64_t Match = 0x0;
    uint64_t Local = 0x0;
  }
  namespace PhysicsSystem {
    uint64_t Raycast = 0x0;
  }
  namespace StringSystem {
    uint64_t CharGet = 0x0;
  }
  namespace ResetGuest {
    uint64_t Offset = 0x0;
  }
  namespace Bypass {
    uint64_t Report1 = 0x0;
    uint64_t Report2 = 0x0;
    uint64_t Report3 = 0x0;
    uint64_t Report4 = 0x0;
  }
}