#pragma once

namespace Buckshot
{
  typedef enum class KeyCode : uint16_t
  {
    Space = 32,
    Apostrophe = 39, /* ' */
    Comma = 44, /* , */
    Minus = 45, /* - */
    Period = 46, /* . */
    Slash = 47, /* / */

    D0 = 48, /* 0 */
    D1 = 49, /* 1 */
    D2 = 50, /* 2 */
    D3 = 51, /* 3 */
    D4 = 52, /* 4 */
    D5 = 53, /* 5 */
    D6 = 54, /* 6 */
    D7 = 55, /* 7 */
    D8 = 56, /* 8 */
    D9 = 57, /* 9 */

    Semicolon = 59, /* ; */
    Equal = 61, /* = */

    A = 65,
    B = 66,
    C = 67,
    D = 68,
    E = 69,
    F = 70,
    G = 71,
    H = 72,
    I = 73,
    J = 74,
    K = 75,
    L = 76,
    M = 77,
    N = 78,
    O = 79,
    P = 80,
    Q = 81,
    R = 82,
    S = 83,
    T = 84,
    U = 85,
    V = 86,
    W = 87,
    X = 88,
    Y = 89,
    Z = 90,

    LeftBracket = 91,  /* [ */
    Backslash = 92,  /* \ */
    RightBracket = 93,  /* ] */
    GraveAccent = 96,  /* ` */

    World1 = 161, /* non-US #1 */
    World2 = 162, /* non-US #2 */

    /* Function keys */
    Escape = 256,
    Enter = 257,
    Tab = 258,
    Backspace = 259,
    Insert = 260,
    Delete = 261,
    Right = 262,
    Left = 263,
    Down = 264,
    Up = 265,
    PageUp = 266,
    PageDown = 267,
    Home = 268,
    End = 269,
    CapsLock = 280,
    ScrollLock = 281,
    NumLock = 282,
    PrintScreen = 283,
    Pause = 284,
    F1 = 290,
    F2 = 291,
    F3 = 292,
    F4 = 293,
    F5 = 294,
    F6 = 295,
    F7 = 296,
    F8 = 297,
    F9 = 298,
    F10 = 299,
    F11 = 300,
    F12 = 301,
    F13 = 302,
    F14 = 303,
    F15 = 304,
    F16 = 305,
    F17 = 306,
    F18 = 307,
    F19 = 308,
    F20 = 309,
    F21 = 310,
    F22 = 311,
    F23 = 312,
    F24 = 313,
    F25 = 314,

    /* Keypad */
    KP0 = 320,
    KP1 = 321,
    KP2 = 322,
    KP3 = 323,
    KP4 = 324,
    KP5 = 325,
    KP6 = 326,
    KP7 = 327,
    KP8 = 328,
    KP9 = 329,
    KPDecimal = 330,
    KPDivide = 331,
    KPMultiply = 332,
    KPSubtract = 333,
    KPAdd = 334,
    KPEnter = 335,
    KPEqual = 336,

    LeftShift = 340,
    LeftControl = 341,
    LeftAlt = 342,
    LeftSuper = 343,
    RightShift = 344,
    RightControl = 345,
    RightAlt = 346,
    RightSuper = 347,
    Menu = 348
  } Key;

  inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
  {
    os << static_cast<int32_t>(keyCode);
    return os;
  }
}

#define BS_KEY_SPACE           ::Buckshot::Key::Space
#define BS_KEY_APOSTROPHE      ::Buckshot::Key::Apostrophe    /* ' */
#define BS_KEY_COMMA           ::Buckshot::Key::Comma         /* , */
#define BS_KEY_MINUS           ::Buckshot::Key::Minus         /* - */
#define BS_KEY_PERIOD          ::Buckshot::Key::Period        /* . */
#define BS_KEY_SLASH           ::Buckshot::Key::Slash         /* / */
#define BS_KEY_0               ::Buckshot::Key::D0
#define BS_KEY_1               ::Buckshot::Key::D1
#define BS_KEY_2               ::Buckshot::Key::D2
#define BS_KEY_3               ::Buckshot::Key::D3
#define BS_KEY_4               ::Buckshot::Key::D4
#define BS_KEY_5               ::Buckshot::Key::D5
#define BS_KEY_6               ::Buckshot::Key::D6
#define BS_KEY_7               ::Buckshot::Key::D7
#define BS_KEY_8               ::Buckshot::Key::D8
#define BS_KEY_9               ::Buckshot::Key::D9
#define BS_KEY_SEMICOLON       ::Buckshot::Key::Semicolon     /* ; */
#define BS_KEY_EQUAL           ::Buckshot::Key::Equal         /* = */
#define BS_KEY_A               ::Buckshot::Key::A
#define BS_KEY_B               ::Buckshot::Key::B
#define BS_KEY_C               ::Buckshot::Key::C
#define BS_KEY_D               ::Buckshot::Key::D
#define BS_KEY_E               ::Buckshot::Key::E
#define BS_KEY_F               ::Buckshot::Key::F
#define BS_KEY_G               ::Buckshot::Key::G
#define BS_KEY_H               ::Buckshot::Key::H
#define BS_KEY_I               ::Buckshot::Key::I
#define BS_KEY_J               ::Buckshot::Key::J
#define BS_KEY_K               ::Buckshot::Key::K
#define BS_KEY_L               ::Buckshot::Key::L
#define BS_KEY_M               ::Buckshot::Key::M
#define BS_KEY_N               ::Buckshot::Key::N
#define BS_KEY_O               ::Buckshot::Key::O
#define BS_KEY_P               ::Buckshot::Key::P
#define BS_KEY_Q               ::Buckshot::Key::Q
#define BS_KEY_R               ::Buckshot::Key::R
#define BS_KEY_S               ::Buckshot::Key::S
#define BS_KEY_T               ::Buckshot::Key::T
#define BS_KEY_U               ::Buckshot::Key::U
#define BS_KEY_V               ::Buckshot::Key::V
#define BS_KEY_W               ::Buckshot::Key::W
#define BS_KEY_X               ::Buckshot::Key::X
#define BS_KEY_Y               ::Buckshot::Key::Y
#define BS_KEY_Z               ::Buckshot::Key::Z
#define BS_KEY_LEFT_BRACKET    ::Buckshot::Key::LeftBracket   /* [ */
#define BS_KEY_BACKSLASH       ::Buckshot::Key::Backslash     /* \ */
#define BS_KEY_RIGHT_BRACKET   ::Buckshot::Key::RightBracket  /* ] */
#define BS_KEY_GRAVE_ACCENT    ::Buckshot::Key::GraveAccent   /* ` */
#define BS_KEY_WORLD_1         ::Buckshot::Key::World1        /* non-US #1 */
#define BS_KEY_WORLD_2         ::Buckshot::Key::World2        /* non-US #2 */

/* Function keys */
#define BS_KEY_ESCAPE          ::Buckshot::Key::Escape
#define BS_KEY_ENTER           ::Buckshot::Key::Enter
#define BS_KEY_TAB             ::Buckshot::Key::Tab
#define BS_KEY_BACKSPACE       ::Buckshot::Key::Backspace
#define BS_KEY_INSERT          ::Buckshot::Key::Insert
#define BS_KEY_DELETE          ::Buckshot::Key::Delete
#define BS_KEY_RIGHT           ::Buckshot::Key::Right
#define BS_KEY_LEFT            ::Buckshot::Key::Left
#define BS_KEY_DOWN            ::Buckshot::Key::Down
#define BS_KEY_UP              ::Buckshot::Key::Up
#define BS_KEY_PAGE_UP         ::Buckshot::Key::PageUp
#define BS_KEY_PAGE_DOWN       ::Buckshot::Key::PageDown
#define BS_KEY_HOME            ::Buckshot::Key::Home
#define BS_KEY_END             ::Buckshot::Key::End
#define BS_KEY_CAPS_LOCK       ::Buckshot::Key::CapsLock
#define BS_KEY_SCROLL_LOCK     ::Buckshot::Key::ScrollLock
#define BS_KEY_NUM_LOCK        ::Buckshot::Key::NumLock
#define BS_KEY_PRINT_SCREEN    ::Buckshot::Key::PrintScreen
#define BS_KEY_PAUSE           ::Buckshot::Key::Pause
#define BS_KEY_F1              ::Buckshot::Key::F1
#define BS_KEY_F2              ::Buckshot::Key::F2
#define BS_KEY_F3              ::Buckshot::Key::F3
#define BS_KEY_F4              ::Buckshot::Key::F4
#define BS_KEY_F5              ::Buckshot::Key::F5
#define BS_KEY_F6              ::Buckshot::Key::F6
#define BS_KEY_F7              ::Buckshot::Key::F7
#define BS_KEY_F8              ::Buckshot::Key::F8
#define BS_KEY_F9              ::Buckshot::Key::F9
#define BS_KEY_F10             ::Buckshot::Key::F10
#define BS_KEY_F11             ::Buckshot::Key::F11
#define BS_KEY_F12             ::Buckshot::Key::F12
#define BS_KEY_F13             ::Buckshot::Key::F13
#define BS_KEY_F14             ::Buckshot::Key::F14
#define BS_KEY_F15             ::Buckshot::Key::F15
#define BS_KEY_F16             ::Buckshot::Key::F16
#define BS_KEY_F17             ::Buckshot::Key::F17
#define BS_KEY_F18             ::Buckshot::Key::F18
#define BS_KEY_F19             ::Buckshot::Key::F19
#define BS_KEY_F20             ::Buckshot::Key::F20
#define BS_KEY_F21             ::Buckshot::Key::F21
#define BS_KEY_F22             ::Buckshot::Key::F22
#define BS_KEY_F23             ::Buckshot::Key::F23
#define BS_KEY_F24             ::Buckshot::Key::F24
#define BS_KEY_F25             ::Buckshot::Key::F25

/* Keypad */
#define BS_KEY_KP_0            ::Buckshot::Key::KP0
#define BS_KEY_KP_1            ::Buckshot::Key::KP1
#define BS_KEY_KP_2            ::Buckshot::Key::KP2
#define BS_KEY_KP_3            ::Buckshot::Key::KP3
#define BS_KEY_KP_4            ::Buckshot::Key::KP4
#define BS_KEY_KP_5            ::Buckshot::Key::KP5
#define BS_KEY_KP_6            ::Buckshot::Key::KP6
#define BS_KEY_KP_7            ::Buckshot::Key::KP7
#define BS_KEY_KP_8            ::Buckshot::Key::KP8
#define BS_KEY_KP_9            ::Buckshot::Key::KP9
#define BS_KEY_KP_DECIMAL      ::Buckshot::Key::KPDecimal
#define BS_KEY_KP_DIVIDE       ::Buckshot::Key::KPDivide
#define BS_KEY_KP_MULTIPLY     ::Buckshot::Key::KPMultiply
#define BS_KEY_KP_SUBTRACT     ::Buckshot::Key::KPSubtract
#define BS_KEY_KP_ADD          ::Buckshot::Key::KPAdd
#define BS_KEY_KP_ENTER        ::Buckshot::Key::KPEnter
#define BS_KEY_KP_EQUAL        ::Buckshot::Key::KPEqual

#define BS_KEY_LEFT_SHIFT      ::Buckshot::Key::LeftShift
#define BS_KEY_LEFT_CONTROL    ::Buckshot::Key::LeftControl
#define BS_KEY_LEFT_ALT        ::Buckshot::Key::LeftAlt
#define BS_KEY_LEFT_SUPER      ::Buckshot::Key::LeftSuper
#define BS_KEY_RIGHT_SHIFT     ::Buckshot::Key::RightShift
#define BS_KEY_RIGHT_CONTROL   ::Buckshot::Key::RightControl
#define BS_KEY_RIGHT_ALT       ::Buckshot::Key::RightAlt
#define BS_KEY_RIGHT_SUPER     ::Buckshot::Key::RightSuper
#define BS_KEY_MENU            ::Buckshot::Key::Menu