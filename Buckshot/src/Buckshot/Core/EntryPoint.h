#pragma once

#ifdef BS_PLATFORM_WINDOWS

extern Buckshot::Application* Buckshot::CreateApplication();

int main(int argc, char** argv)
{
  Buckshot::Log::Init();

  BS_PROFILE_BEGIN_SESSION("Startup", "BuckshotProfile-Startup.json");
  auto app = Buckshot::CreateApplication();
  BS_PROFILE_END_SESSION();

  BS_PROFILE_BEGIN_SESSION("Runtime", "BuckshotProfile-Runtime.json");
  app->Run();
  BS_PROFILE_END_SESSION();
  
  BS_PROFILE_BEGIN_SESSION("Shutdown", "BuckshotProfile-Shutdown.json");
  delete app;
  BS_PROFILE_END_SESSION();
}

#endif