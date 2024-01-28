#pragma once

#ifdef BS_PLATFORM_WINDOWS

extern Buckshot::Application* Buckshot::CreateApplication();

int main(int argc, char** argv)
{
  auto app = Buckshot::CreateApplication();
  app->Run();
  delete app;
}

#endif