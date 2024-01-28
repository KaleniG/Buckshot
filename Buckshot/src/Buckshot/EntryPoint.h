#pragma once

#ifdef BS_PLATFORM_WINDOWS

extern Buckshot::Application* Buckshot::CreateApplication();

int main(int argc, char** argv)
{
  Buckshot::Log::Init();
  auto app = Buckshot::CreateApplication();
  app->Run();
  delete app;
}

#endif