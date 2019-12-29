#include "internal.h"

#include <exception>
#include <iostream>

namespace {

void LoadCoreFunctions(util::Library &library) {
  using namespace graphics::egl::internal;

  // EGL 1.0
  ChooseConfig =
      library.GetSymbolAddress<ChooseConfigProc>(ChooseConfigProcName);
  // CopyBuffers =
  // library.GetSymbolAddress<CopyBuffersProc>(CopyBuffersProcName);
  CreateContext =
      library.GetSymbolAddress<CreateContextProc>(CreateContextProcName);
  CreatePbufferSurface = library.GetSymbolAddress<CreatePbufferSurfaceProc>(
      CreatePbufferSurfaceProcName);
  // CreatePixmapSurface = library.GetSymbolAddress<CreatePixmapSurfaceProc>(
  //     CreatePixmapSurfaceProcName);
  // CreateWindowSurface = library.GetSymbolAddress<CreateWindowSurfaceProc>(
  //     CreateWindowSurfaceProcName);
  DestroyContext =
      library.GetSymbolAddress<DestroyContextProc>(DestroyContextProcName);
  DestroySurface =
      library.GetSymbolAddress<DestroySurfaceProc>(DestroySurfaceProcName);
  GetConfigAttrib =
      library.GetSymbolAddress<GetConfigAttribProc>(GetConfigAttribProcName);
  GetConfigs = library.GetSymbolAddress<GetConfigsProc>(GetConfigsProcName);
  GetCurrentDisplay = library.GetSymbolAddress<GetCurrentDisplayProc>(
      GetCurrentDisplayProcName);
  GetCurrentSurface = library.GetSymbolAddress<GetCurrentSurfaceProc>(
      GetCurrentSurfaceProcName);
  // GetDisplay = library.GetSymbolAddress<GetDisplayProc>(GetDisplayProcName);
  GetError = library.GetSymbolAddress<GetErrorProc>(GetErrorProcName);
  GetProcAddress =
      library.GetSymbolAddress<GetProcAddressProc>(GetProcAddressProcName);
  Initialize = library.GetSymbolAddress<InitializeProc>(InitializeProcName);
  MakeCurrent = library.GetSymbolAddress<MakeCurrentProc>(MakeCurrentProcName);
  QueryContext =
      library.GetSymbolAddress<QueryContextProc>(QueryContextProcName);
  QueryString = library.GetSymbolAddress<QueryStringProc>(QueryStringProcName);
  QuerySurface =
      library.GetSymbolAddress<QuerySurfaceProc>(QuerySurfaceProcName);
  SwapBuffers = library.GetSymbolAddress<SwapBuffersProc>(SwapBuffersProcName);
  Terminate = library.GetSymbolAddress<TerminateProc>(TerminateProcName);
  WaitGl = library.GetSymbolAddress<WaitGlProc>(WaitGlProcName);
  WaitNative = library.GetSymbolAddress<WaitNativeProc>(WaitNativeProcName);

  // EGL 1.1
  BindTexImage =
      library.GetSymbolAddress<BindTexImageProc>(BindTexImageProcName);
  ReleaseTexImage =
      library.GetSymbolAddress<ReleaseTexImageProc>(ReleaseTexImageProcName);
  SurfaceAttrib =
      library.GetSymbolAddress<SurfaceAttribProc>(SurfaceAttribProcName);
  SwapInterval =
      library.GetSymbolAddress<SwapIntervalProc>(SwapIntervalProcName);

  // EGL 1.2
  BindApi = library.GetSymbolAddress<BindApiProc>(BindApiProcName);
  QueryApi = library.GetSymbolAddress<QueryApiProc>(QueryApiProcName);
  CreatePbufferFromClientBuffer =
      library.GetSymbolAddress<CreatePbufferFromClientBufferProc>(
          CreatePbufferFromClientBufferProcName);
  ReleaseThread =
      library.GetSymbolAddress<ReleaseThreadProc>(ReleaseThreadProcName);

  // EGL 1.3

  // EGL 1.4
  GetCurrentContext = library.GetSymbolAddress<GetCurrentContextProc>(
      GetCurrentContextProcName);

  // EGL 1.5
  CreateSync = library.GetSymbolAddress<CreateSyncProc>(CreateSyncProcName);
  DestroySync = library.GetSymbolAddress<DestroySyncProc>(DestroySyncProcName);
  ClientWaitSync =
      library.GetSymbolAddress<ClientWaitSyncProc>(ClientWaitSyncProcName);
  GetSyncAttrib =
      library.GetSymbolAddress<GetSyncAttribProc>(GetSyncAttribProcName);
  CreateImage = library.GetSymbolAddress<CreateImageProc>(CreateImageProcName);
  DestroyImage =
      library.GetSymbolAddress<DestroyImageProc>(DestroyImageProcName);
  GetPlatformDisplay = library.GetSymbolAddress<GetPlatformDisplayProc>(
      GetPlatformDisplayProcName);
  CreatePlatformWindowSurface =
      library.GetSymbolAddress<CreatePlatformWindowSurfaceProc>(
          CreatePlatformWindowSurfaceProcName);
  CreatePlatformPixmapSurface =
      library.GetSymbolAddress<CreatePlatformPixmapSurfaceProc>(
          CreatePlatformPixmapSurfaceProcName);
  WaitSync = library.GetSymbolAddress<WaitSyncProc>(WaitSyncProcName);
}

void LoadExtensionFunctions() {}

[[noreturn]] void NotLoadedFunction() {
  std::cerr << "Attempted to call an EGL function that is not loaded"
            << std::endl;
  std::terminate();
}

}  // namespace

namespace graphics::egl::internal {

namespace functions {

// EGL 1.0
types::ChooseConfigProc ChooseConfig =
    reinterpret_cast<types::ChooseConfigProc>(NotLoadedFunction);
// types::CopyBuffersProc CopyBuffers =
// reinterpret_cast<types::CopyBuffersProc>(NotLoadedFunction);
types::CreateContextProc CreateContext =
    reinterpret_cast<types::CreateContextProc>(NotLoadedFunction);
types::CreatePbufferSurfaceProc CreatePbufferSurface =
    reinterpret_cast<types::CreatePbufferSurfaceProc>(NotLoadedFunction);
// types::CreatePixmapSurfaceProc CreatePixmapSurface =
// reinterpret_cast<types::CreatePixmapSurfaceProc>(NotLoadedFunction);
// types::CreateWindowSurfaceProc CreateWindowSurface =
// reinterpret_cast<types::CreateWindowSurfaceProc>(NotLoadedFunction);
types::DestroyContextProc DestroyContext =
    reinterpret_cast<types::DestroyContextProc>(NotLoadedFunction);
types::DestroySurfaceProc DestroySurface =
    reinterpret_cast<types::DestroySurfaceProc>(NotLoadedFunction);
types::GetConfigAttribProc GetConfigAttrib =
    reinterpret_cast<types::GetConfigAttribProc>(NotLoadedFunction);
types::GetConfigsProc GetConfigs =
    reinterpret_cast<types::GetConfigsProc>(NotLoadedFunction);
types::GetCurrentDisplayProc GetCurrentDisplay =
    reinterpret_cast<types::GetCurrentDisplayProc>(NotLoadedFunction);
types::GetCurrentSurfaceProc GetCurrentSurface =
    reinterpret_cast<types::GetCurrentSurfaceProc>(NotLoadedFunction);
// types::GetDisplayProc GetDisplay =
// reinterpret_cast<types::GetDisplayProc>(NotLoadedFunction);
types::GetErrorProc GetError =
    reinterpret_cast<types::GetErrorProc>(NotLoadedFunction);
types::GetProcAddressProc GetProcAddress =
    reinterpret_cast<types::GetProcAddressProc>(NotLoadedFunction);
types::InitializeProc Initialize =
    reinterpret_cast<types::InitializeProc>(NotLoadedFunction);
types::MakeCurrentProc MakeCurrent =
    reinterpret_cast<types::MakeCurrentProc>(NotLoadedFunction);
types::QueryContextProc QueryContext =
    reinterpret_cast<types::QueryContextProc>(NotLoadedFunction);
types::QueryStringProc QueryString =
    reinterpret_cast<types::QueryStringProc>(NotLoadedFunction);
types::QuerySurfaceProc QuerySurface =
    reinterpret_cast<types::QuerySurfaceProc>(NotLoadedFunction);
types::SwapBuffersProc SwapBuffers =
    reinterpret_cast<types::SwapBuffersProc>(NotLoadedFunction);
types::TerminateProc Terminate =
    reinterpret_cast<types::TerminateProc>(NotLoadedFunction);
types::WaitGlProc WaitGl =
    reinterpret_cast<types::WaitGlProc>(NotLoadedFunction);
types::WaitNativeProc WaitNative =
    reinterpret_cast<types::WaitNativeProc>(NotLoadedFunction);

// EGL 1.1
types::BindTexImageProc BindTexImage =
    reinterpret_cast<types::BindTexImageProc>(NotLoadedFunction);
types::ReleaseTexImageProc ReleaseTexImage =
    reinterpret_cast<types::ReleaseTexImageProc>(NotLoadedFunction);
types::SurfaceAttribProc SurfaceAttrib =
    reinterpret_cast<types::SurfaceAttribProc>(NotLoadedFunction);
types::SwapIntervalProc SwapInterval =
    reinterpret_cast<types::SwapIntervalProc>(NotLoadedFunction);

// EGL 1.2
types::BindApiProc BindApi =
    reinterpret_cast<types::BindApiProc>(NotLoadedFunction);
types::QueryApiProc QueryApi =
    reinterpret_cast<types::QueryApiProc>(NotLoadedFunction);
types::CreatePbufferFromClientBufferProc CreatePbufferFromClientBuffer =
    reinterpret_cast<types::CreatePbufferFromClientBufferProc>(
        NotLoadedFunction);
types::ReleaseThreadProc ReleaseThread =
    reinterpret_cast<types::ReleaseThreadProc>(NotLoadedFunction);
types::WaitClientProc WaitClient =
    reinterpret_cast<types::WaitClientProc>(NotLoadedFunction);

// EGL 1.4
types::GetCurrentContextProc GetCurrentContext =
    reinterpret_cast<types::GetCurrentContextProc>(NotLoadedFunction);

// EGL 1.5
types::CreateSyncProc CreateSync =
    reinterpret_cast<types::CreateSyncProc>(NotLoadedFunction);
types::DestroySyncProc DestroySync =
    reinterpret_cast<types::DestroySyncProc>(NotLoadedFunction);
types::ClientWaitSyncProc ClientWaitSync =
    reinterpret_cast<types::ClientWaitSyncProc>(NotLoadedFunction);
types::GetSyncAttribProc GetSyncAttrib =
    reinterpret_cast<types::GetSyncAttribProc>(NotLoadedFunction);
types::CreateImageProc CreateImage =
    reinterpret_cast<types::CreateImageProc>(NotLoadedFunction);
types::DestroyImageProc DestroyImage =
    reinterpret_cast<types::DestroyImageProc>(NotLoadedFunction);
types::GetPlatformDisplayProc GetPlatformDisplay =
    reinterpret_cast<types::GetPlatformDisplayProc>(NotLoadedFunction);
types::CreatePlatformWindowSurfaceProc CreatePlatformWindowSurface =
    reinterpret_cast<types::CreatePlatformWindowSurfaceProc>(NotLoadedFunction);
types::CreatePlatformPixmapSurfaceProc CreatePlatformPixmapSurface =
    reinterpret_cast<types::CreatePlatformPixmapSurfaceProc>(NotLoadedFunction);
types::WaitSyncProc WaitSync =
    reinterpret_cast<types::WaitSyncProc>(NotLoadedFunction);

}  // namespace functions

void LoadFunctions(util::Library &library) {
  LoadCoreFunctions(library);
  LoadExtensionFunctions();
}

}  // namespace graphics::egl::internal
