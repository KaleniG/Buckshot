#pragma once

namespace Buckshot
{
  typedef enum class MouseCode : uint16_t
  {
    Button0 = 0,
    Button1 = 1,
    Button2 = 2,
    Button3 = 3,
    Button4 = 4,
    Button5 = 5,
    Button6 = 6,
    Button7 = 7,

    ButtonLast = Button7,
    ButtonLeft = Button0,
    ButtonRight = Button1,
    ButtonMiddle = Button2
  } Mouse;

  inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
  {
    os << static_cast<int32_t>(mouseCode);
    return os;
  }
}

#define BS_MOUSE_BUTTON_0      ::Buckshot::Mouse::Button0
#define BS_MOUSE_BUTTON_1      ::Buckshot::Mouse::Button1
#define BS_MOUSE_BUTTON_2      ::Buckshot::Mouse::Button2
#define BS_MOUSE_BUTTON_3      ::Buckshot::Mouse::Button3
#define BS_MOUSE_BUTTON_4      ::Buckshot::Mouse::Button4
#define BS_MOUSE_BUTTON_5      ::Buckshot::Mouse::Button5
#define BS_MOUSE_BUTTON_6      ::Buckshot::Mouse::Button6
#define BS_MOUSE_BUTTON_7      ::Buckshot::Mouse::Button7
#define BS_MOUSE_BUTTON_LAST   ::Buckshot::Mouse::ButtonLast
#define BS_MOUSE_BUTTON_LEFT   ::Buckshot::Mouse::ButtonLeft
#define BS_MOUSE_BUTTON_RIGHT  ::Buckshot::Mouse::ButtonRight
#define BS_MOUSE_BUTTON_MIDDLE ::Buckshot::Mouse::ButtonMiddle