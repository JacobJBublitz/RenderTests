#pragma once

#include <cstdint>

#include "util/library.h"

namespace graphics::egl {

namespace consts {

// EGL 1.0
constexpr unsigned int kAlphaSize = 0x3021;
constexpr unsigned int kBadAccess = 0x3002;
constexpr unsigned int kBadAlloc = 0x3003;
constexpr unsigned int kBadAttribute = 0x3004;
constexpr unsigned int kBadConfig = 0x3005;
constexpr unsigned int kBadContext = 0x3006;
constexpr unsigned int kBadCurrentSurface = 0x3007;
constexpr unsigned int kBadDisplay = 0x3008;
constexpr unsigned int kBadMatch = 0x3009;
constexpr unsigned int kBadNativePixmap = 0x300A;
constexpr unsigned int kBadNativeWindow = 0x300B;
constexpr unsigned int kBadParameter = 0x300C;
constexpr unsigned int kBadSurface = 0x300D;
constexpr unsigned int kBlueSize = 0x3022;
constexpr unsigned int kBufferSize = 0x3020;
constexpr unsigned int kConfigCaveat = 0x3027;
constexpr unsigned int kConfigId = 0x3028;
constexpr unsigned int kCoreNativeEngine = 0x305B;
constexpr unsigned int kDepthSize = 0x3025;
constexpr int32_t kDontCare = -1;
constexpr unsigned int kDraw = 0x3059;
constexpr unsigned int kExtensions = 0x3055;
constexpr unsigned int kFalse = 0;
constexpr unsigned int kGreenSize = 0x3023;
constexpr unsigned int kHeight = 0x3056;
constexpr unsigned int kLargestPbuffer = 0x3058;
constexpr unsigned int kLevel = 0x3029;
constexpr unsigned int kMaxPbufferHeight = 0x302A;
constexpr unsigned int kMaxPbufferPixels = 0x302B;
constexpr unsigned int kMaxPbufferWidth = 0x302C;
constexpr unsigned int kNativeRenderable = 0x302D;
constexpr unsigned int kNativeVisualId = 0x302E;
constexpr unsigned int kNativeVisualType = 0x302F;
constexpr unsigned int kNone = 0x3038;
constexpr unsigned int kNonConformantConfig = 0x3051;
constexpr unsigned int kNotInitialized = 0x3001;
constexpr void *kNoContext = nullptr;
constexpr void *kNoDisplay = nullptr;
constexpr void *kNoSurface = nullptr;
constexpr unsigned int kPbufferBit = 0x0001;
constexpr unsigned int kPixmapBit = 0x0002;
constexpr unsigned int kRead = 0x305A;
constexpr unsigned int kRedSize = 0x3024;
constexpr unsigned int kSamples = 0x3031;
constexpr unsigned int kSampleBuffers = 0x3032;
constexpr unsigned int kSlowConfig = 0x3050;
constexpr unsigned int kStencilSize = 0x3026;
constexpr unsigned int kSuccess = 0x3000;
constexpr unsigned int kSurfaceType = 0x3033;
constexpr unsigned int kTransparentBlueValue = 0x3035;
constexpr unsigned int kTransparentGreenValue = 0x3036;
constexpr unsigned int kTransparentRedValue = 0x3037;
constexpr unsigned int kTransparentRgb = 0x3052;
constexpr unsigned int kTransparentType = 0x3034;
constexpr unsigned int kTrue = 1;
constexpr unsigned int kVendor = 0x3053;
constexpr unsigned int kVersion = 0x3054;
constexpr unsigned int kWidth = 0x3057;
constexpr unsigned int kWindowBit = 0x0004;

// EGL 1.1
constexpr unsigned int kBackBuffer = 0x3084;
constexpr unsigned int kBindToTextureRgb = 0x3039;
constexpr unsigned int kBindToTextureRgba = 0x303A;
constexpr unsigned int kContextLost = 0x300E;
constexpr unsigned int kMinSwapInterval = 0x303B;
constexpr unsigned int kMaxSwapInterval = 0x303C;
constexpr unsigned int kMipmapTexture = 0x3082;
constexpr unsigned int kMipmapLevel = 0x3083;
constexpr unsigned int kNoTexture = 0x305C;
constexpr unsigned int kTexture2d = 0x305F;
constexpr unsigned int kTextureFormat = 0x3080;
constexpr unsigned int kTextureRgb = 0x305D;
constexpr unsigned int kTextureRgba = 0x305E;
constexpr unsigned int kTextureTarget = 0x3081;

// EGL 1.2
constexpr unsigned int kAlphaFormat = 0x3088;
constexpr unsigned int kAlphaFormatNonpre = 0x308B;
constexpr unsigned int kAlphaFormatPre = 0x308C;
constexpr unsigned int kAlphaMaskSize = 0x303E;
constexpr unsigned int kBufferPreserved = 0x3094;
constexpr unsigned int kBufferDestroyed = 0x3095;
constexpr unsigned int kClientApis = 0x308D;
constexpr unsigned int kColorspace = 0x3087;
constexpr unsigned int kColorspaceSrgb = 0x3089;
constexpr unsigned int kColorspaceLinear = 0x308A;
constexpr unsigned int kColorBufferType = 0x303F;
constexpr unsigned int kContextClientType = 0x3097;
constexpr unsigned int kDisplayScaling = 10000;
constexpr unsigned int kHorizontalResolution = 0x3090;
constexpr unsigned int kLuminanceBuffer = 0x308F;
constexpr unsigned int kLuminanceSize = 0x303D;
constexpr unsigned int kOpenGlEsBit = 0x0001;
constexpr unsigned int kOpenVgBit = 0x0002;
constexpr unsigned int kOpenGlEsApi = 0x30A0;
constexpr unsigned int kOpenVgApi = 0x30A1;
constexpr unsigned int kOpenVgImage = 0x3096;
constexpr unsigned int kPixelAspectRatio = 0x3092;
constexpr unsigned int kRenderableType = 0x3040;
constexpr unsigned int kRenderBuffer = 0x3086;
constexpr unsigned int kRgbBuffer = 0x308E;
constexpr unsigned int kSingleBuffer = 0x3085;
constexpr unsigned int kSwapBehavior = 0x3093;
constexpr int32_t kUnknown = -1;
constexpr unsigned int kVerticalResolution = 0x3091;

// EGL 1.3
constexpr unsigned int kConformant = 0x3042;
constexpr unsigned int kContextClientVersion = 0x3098;
constexpr unsigned int kMatchNativePixmap = 0x3041;
constexpr unsigned int kOpenGlEs2Bit = 0x0004;
constexpr unsigned int kVgAlphaFormat = 0x3088;
constexpr unsigned int kVgAlphaFormatNonpre = 0x308B;
constexpr unsigned int kVgAlphaFormatPre = 0x308C;
constexpr unsigned int kVgAlphaFormatPreBit = 0x0040;
constexpr unsigned int kVgColorspace = 0x3087;
constexpr unsigned int kVgColorspaceSrgb = 0x3089;
constexpr unsigned int kVgColorspaceLinear = 0x308A;
constexpr unsigned int kVgColorspaceLinearBit = 0x0020;

// EGL 1.4
constexpr void *kDefaultDisplay = nullptr;
constexpr unsigned int kMultisampleResolveBoxBit = 0x0200;
constexpr unsigned int kMultisampleResolve = 0x3099;
constexpr unsigned int kMultisampleResolveDefault = 0x309A;
constexpr unsigned int kMultisampleResolveBox = 0x309B;
constexpr unsigned int kOpenGlApi = 0x30A2;
constexpr unsigned int kOpenGlBit = 0x0008;
constexpr unsigned int kSwapBehaviorPreservedBit = 0x0400;

// EGL 1.5
constexpr unsigned int kContextMajorVersion = 0x3098;
constexpr unsigned int kContextMinorVersion = 0x30FB;
constexpr unsigned int kContextOpenGlProfileMask = 0x30FD;
constexpr unsigned int kContextOpenGlResetNotificationStrategy = 0x31BD;
constexpr unsigned int kNoResetNotification = 0x31BE;
constexpr unsigned int kLoseContextOnReset = 0x31BF;
constexpr unsigned int kContextOpenGlCoreProfileBit = 0x00000001;
constexpr unsigned int kContextOpenGlCompatibilityProfileBit = 0x00000002;
constexpr unsigned int kContextOpenGlDebug = 0x31B0;
constexpr unsigned int kContextOpenGlForwardCompatible = 0x31B1;
constexpr unsigned int kContextOpenGlRobustAccess = 0x31B2;
constexpr unsigned int kOpenGlEs3Bit = 0x00000040;
constexpr unsigned int kClEventHandle = 0x309C;
constexpr unsigned int kSyncClEvent = 0x30FE;
constexpr unsigned int kSyncClEventComplete = 0x30FF;
constexpr unsigned int kSyncPriorCommandsComplete = 0x30F0;
constexpr unsigned int kSyncType = 0x30F7;
constexpr unsigned int kSyncStatus = 0x30F1;
constexpr unsigned int kSyncCondition = 0x30F8;
constexpr unsigned int kSignaled = 0x30F2;
constexpr unsigned int kUnsignaled = 0x30F3;
constexpr unsigned int kSyncFlushCommandsBit = 0x0001;
constexpr unsigned long long kForever = 0xFFFFFFFFFFFFFFFFull;
constexpr unsigned int kTimeoutExpired = 0x30F5;
constexpr unsigned int kConditionSatisfied = 0x30F6;
constexpr void *kNoSync = nullptr;
constexpr unsigned int kSyncFence = 0x30F9;
constexpr unsigned int kGlColorSpace = 0x309D;
constexpr unsigned int kGlColorspaceSrgb = 0x3089;
constexpr unsigned int kGlColorspaceLinear = 0x308A;
constexpr unsigned int kGlRenderbuffer = 0x30B9;
constexpr unsigned int kGlTexture2d = 0x30B1;
constexpr unsigned int kGlTextureLevel = 0x30BC;
constexpr unsigned int kGlTexture3d = 0x30B2;
constexpr unsigned int kGlTextureZoffset = 0x30BD;
constexpr unsigned int kGlTextureCubeMapPositiveX = 0x30B3;
constexpr unsigned int kGlTextureCubeMapNegativeX = 0x30B4;
constexpr unsigned int kGlTextureCubeMapPositiveY = 0x30B5;
constexpr unsigned int kGlTextureCubeMapNegativeY = 0x30B6;
constexpr unsigned int kGlTextureCubeMapPositiveZ = 0x30B7;
constexpr unsigned int kGlTextureCubeMapNegativeZ = 0x30B8;
constexpr unsigned int kImagePreserved = 0x30D2;
constexpr void *kNoImage = nullptr;

}  // namespace consts

namespace functions {

namespace names {

// EGL 1.0
constexpr const char *ChooseConfigProcName = "eglChooseConfig";
constexpr const char *CopyBuffersProcName = "eglCopyBuffers";
constexpr const char *CreateContextProcName = "eglCreateContext";
constexpr const char *CreatePbufferSurfaceProcName = "eglCreatePbufferSurface";
constexpr const char *CreatePixmapSurfaceProcName = "eglCreatePixmapSurface";
constexpr const char *CreateWindowSurfaceProcName = "eglCreateWindowSurface";
constexpr const char *DestroyContextProcName = "eglDestroyContext";
constexpr const char *DestroySurfaceProcName = "eglDestroySurface";
constexpr const char *GetConfigAttribProcName = "eglGetConfigAttrib";
constexpr const char *GetConfigsProcName = "eglGetConfigs";
constexpr const char *GetCurrentDisplayProcName = "eglGetCurrentDisplay";
constexpr const char *GetCurrentSurfaceProcName = "eglGetCurrentSurface";
constexpr const char *GetDisplayProcName = "eglGetDisplay";
constexpr const char *GetErrorProcName = "eglGetError";
constexpr const char *GetProcAddressProcName = "eglGetProcAddress";
constexpr const char *InitializeProcName = "eglInitialize";
constexpr const char *MakeCurrentProcName = "eglMakeCurrent";
constexpr const char *QueryContextProcName = "eglQueryContext";
constexpr const char *QueryStringProcName = "eglQueryString";
constexpr const char *QuerySurfaceProcName = "eglQuerySurface";
constexpr const char *SwapBuffersProcName = "eglSwapBuffers";
constexpr const char *TerminateProcName = "eglTerminate";
constexpr const char *WaitGlProcName = "eglWaitGL";
constexpr const char *WaitNativeProcName = "eglWaitNative";

// EGL 1.1
constexpr const char *BindTexImageProcName = "eglBindTexImage";
constexpr const char *ReleaseTexImageProcName = "eglReleaseTexImage";
constexpr const char *SurfaceAttribProcName = "eglSurfaceAttrib";
constexpr const char *SwapIntervalProcName = "eglSwapInterval";

// EGL 1.2
constexpr const char *BindApiProcName = "eglBindAPI";
constexpr const char *QueryApiProcName = "eglQueryAPI";
constexpr const char *CreatePbufferFromClientBufferProcName =
    "eglCreatePbufferFromClientBuffer";
constexpr const char *ReleaseThreadProcName = "eglReleaseThread";

// EGL 1.3

// EGL 1.4
constexpr const char *GetCurrentContextProcName = "eglGetCurrentContext";

// EGL 1.5
constexpr const char *CreateSyncProcName = "eglCreateSync";
constexpr const char *DestroySyncProcName = "eglDestroySync";
constexpr const char *ClientWaitSyncProcName = "eglClientWaitSync";
constexpr const char *GetSyncAttribProcName = "eglGetSyncAttrib";
constexpr const char *CreateImageProcName = "eglCreateImage";
constexpr const char *DestroyImageProcName = "eglDestroyImage";
constexpr const char *GetPlatformDisplayProcName = "eglGetPlatformDisplay";
constexpr const char *CreatePlatformWindowSurfaceProcName =
    "eglCreatePlatformWindowSurface";
constexpr const char *CreatePlatformPixmapSurfaceProcName =
    "eglCreatePlatformPixmapSurface";
constexpr const char *WaitSyncProcName = "eglWaitSync";

}  // namespace names

namespace types {

#if defined(_WIN32)
#define EGL_APIENTRY __stdcall
#else
#define EGL_APIENTRY
#endif

// EGL 1.0
using ChooseConfigProc = unsigned int(EGL_APIENTRY *)(
    void *dpy, const int32_t *attrib_list, void **configs, int32_t config_size,
    int32_t *num_config);
// using CopyBuffersProc = unsigned int(EGL_APIENTRY *)(
//     void *dpy, void *surface, EGLNativePixmapType target);
using CreateContextProc = void *(EGL_APIENTRY *)(void *dpy, void *config,
                                                 void *share_context,
                                                 const int32_t *attrib_list);
using CreatePbufferSurfaceProc =
    void *(EGL_APIENTRY *)(void *dpy, void *config, const int32_t *attrib_list);
// using CreatePixmapSurfaceProc =
//     void *(EGL_APIENTRY *)(void *dpy, void *config, EGLNativePixmapType
//     pixmap,
//                            const int32_t *attrib_list);
// using CreateWindowSurfaceProc =
//     void *(EGL_APIENTRY *)(void *dpy, void *config, EGLNativeWindowType win,
//                            const int32_t *attrib_list);
using DestroyContextProc = unsigned int(EGL_APIENTRY *)(void *dpy, void *ctx);
using DestroySurfaceProc = unsigned int(EGL_APIENTRY *)(void *dpy,
                                                        void *surface);
using GetConfigAttribProc = unsigned int(EGL_APIENTRY *)(void *dpy,
                                                         void *config,
                                                         int32_t attribute,
                                                         int32_t *value);
using GetConfigsProc = unsigned int(EGL_APIENTRY *)(void *dpy, void **configs,
                                                    int32_t config_size,
                                                    int32_t *num_config);
using GetCurrentDisplayProc = void *(EGL_APIENTRY *)();
using GetCurrentSurfaceProc = void *(EGL_APIENTRY *)(int32_t readdraw);
// using GetDisplayProc = void *(EGL_APIENTRY *)(EGLNativeDisplayType
// display_id);
using GetErrorProc = int32_t(EGL_APIENTRY *)();
using GetProcAddressProc = uintptr_t(EGL_APIENTRY *)(const char *procname);
using InitializeProc = unsigned int(EGL_APIENTRY *)(void *dpy, int32_t *major,
                                                    int32_t *minor);
using MakeCurrentProc = unsigned int(EGL_APIENTRY *)(void *dpy, void *draw,
                                                     void *read, void *ctx);
using QueryContextProc = unsigned int(EGL_APIENTRY *)(void *dpy, void *ctx,
                                                      int32_t attribute,
                                                      int32_t *value);
using QueryStringProc = const char *(EGL_APIENTRY *)(void *dpy, int32_t name);
using QuerySurfaceProc = unsigned int(EGL_APIENTRY *)(void *dpy, void *surface,
                                                      int32_t attribute,
                                                      int32_t *value);
using SwapBuffersProc = unsigned int(EGL_APIENTRY *)(void *dpy, void *surface);
using TerminateProc = unsigned int(EGL_APIENTRY *)(void *dpy);
using WaitGlProc = unsigned int(EGL_APIENTRY *)();
using WaitNativeProc = unsigned int(EGL_APIENTRY *)(int32_t engine);

// EGL 1.1
using BindTexImageProc = unsigned int(EGL_APIENTRY *)(void *dpy, void *surface,
                                                      int32_t buffer);
using ReleaseTexImageProc = unsigned int(EGL_APIENTRY *)(void *dpy,
                                                         void *surface,
                                                         int32_t buffer);
using SurfaceAttribProc = unsigned int(EGL_APIENTRY *)(void *dpy, void *surface,
                                                       int32_t attribute,
                                                       int32_t value);
using SwapIntervalProc = unsigned int(EGL_APIENTRY *)(void *dpy,
                                                      int32_t interval);

// EGL 1.2
using BindApiProc = unsigned int(EGL_APIENTRY *)(unsigned int api);
using QueryApiProc = unsigned int(EGL_APIENTRY *)();
using CreatePbufferFromClientBufferProc =
    void *(EGL_APIENTRY *)(void *dpy, unsigned int buftype, void *buffer,
                           void *config, const int32_t *attrib_list);
using ReleaseThreadProc = unsigned int(EGL_APIENTRY *)();
using WaitClientProc = unsigned int(EGL_APIENTRY *)();

// EGL 1.4
using GetCurrentContextProc = void *(EGL_APIENTRY *)();

// EGL 1.5
using CreateSyncProc = void *(EGL_APIENTRY *)(void *dpy, unsigned int type,
                                              const intptr_t *attrib_list);
using DestroySyncProc = unsigned int(EGL_APIENTRY *)(void *dpy, void *sync);
using ClientWaitSyncProc = int(EGL_APIENTRY *)(void *dpy, void *sync,
                                               int32_t flags, uint64_t timeout);
using GetSyncAttribProc = unsigned int(EGL_APIENTRY *)(void *dpy, void *sync,
                                                       int32_t attribute,
                                                       intptr_t *value);
using CreateImageProc = void *(EGL_APIENTRY *)(void *dpy, void *ctx,
                                               unsigned int target,
                                               void *buffer,
                                               const intptr_t *attrib_list);
using DestroyImageProc = unsigned int(EGL_APIENTRY *)(void *dpy, void *image);
using GetPlatformDisplayProc =
    void *(EGL_APIENTRY *)(unsigned int platform, void *native_display,
                           const intptr_t *attrib_list);
using CreatePlatformWindowSurfaceProc =
    void *(EGL_APIENTRY *)(void *dpy, void *config, void *native_window,
                           const intptr_t *attrib_list);
using CreatePlatformPixmapSurfaceProc =
    void *(EGL_APIENTRY *)(void *dpy, void *config, void *native_pixmap,
                           const intptr_t *attrib_list);
using WaitSyncProc = unsigned int(EGL_APIENTRY *)(void *dpy, void *sync,
                                                  int32_t flags);

#undef EGL_APIENTRY

}  // namespace types

// EGL 1.0
extern types::ChooseConfigProc ChooseConfig;
// extern types::CopyBuffersProc CopyBuffers;
extern types::CreateContextProc CreateContext;
extern types::CreatePbufferSurfaceProc CreatePbufferSurface;
// extern types::CreatePixmapSurfaceProc CreatePixmapSurface;
// extern types::CreateWindowSurfaceProc CreateWindowSurface;
extern types::DestroyContextProc DestroyContext;
extern types::DestroySurfaceProc DestroySurface;
extern types::GetConfigAttribProc GetConfigAttrib;
extern types::GetConfigsProc GetConfigs;
extern types::GetCurrentDisplayProc GetCurrentDisplay;
extern types::GetCurrentSurfaceProc GetCurrentSurface;
// extern types::GetDisplayProc GetDisplay;
extern types::GetErrorProc GetError;
extern types::GetProcAddressProc GetProcAddress;
extern types::InitializeProc Initialize;
extern types::MakeCurrentProc MakeCurrent;
extern types::QueryContextProc QueryContext;
extern types::QueryStringProc QueryString;
extern types::QuerySurfaceProc QuerySurface;
extern types::SwapBuffersProc SwapBuffers;
extern types::TerminateProc Terminate;
extern types::WaitGlProc WaitGl;
extern types::WaitNativeProc WaitNative;

// EGL 1.1
extern types::BindTexImageProc BindTexImage;
extern types::ReleaseTexImageProc ReleaseTexImage;
extern types::SurfaceAttribProc SurfaceAttrib;
extern types::SwapIntervalProc SwapInterval;

// EGL 1.2
extern types::BindApiProc BindApi;
extern types::QueryApiProc QueryApi;
extern types::CreatePbufferFromClientBufferProc CreatePbufferFromClientBuffer;
extern types::ReleaseThreadProc ReleaseThread;
extern types::WaitClientProc WaitClient;

// EGL 1.4
extern types::GetCurrentContextProc GetCurrentContext;

// EGL 1.5
extern types::CreateSyncProc CreateSync;
extern types::DestroySyncProc DestroySync;
extern types::ClientWaitSyncProc ClientWaitSync;
extern types::GetSyncAttribProc GetSyncAttrib;
extern types::CreateImageProc CreateImage;
extern types::DestroyImageProc DestroyImage;
extern types::GetPlatformDisplayProc GetPlatformDisplay;
extern types::CreatePlatformWindowSurfaceProc CreatePlatformWindowSurface;
extern types::CreatePlatformPixmapSurfaceProc CreatePlatformPixmapSurface;
extern types::WaitSyncProc WaitSync;

}  // namespace functions

namespace extensions {

namespace KHR_client_get_all_proc_addresses {

constexpr const char *Name = "EGL_KHR_client_get_all_proc_addresses";

}  // namespace KHR_client_get_all_proc_addresses

namespace KHR_get_all_proc_addresses {

constexpr const char *Name = "EGL_KHR_get_all_proc_addresses";

}  // namespace KHR_get_all_proc_addresses

namespace KHR_platform_android {

constexpr const char *Name = "EGL_KHR_platform_android";
constexpr unsigned int PlatformAndroidKhr = 0x3141;

}  // namespace KHR_platform_android

namespace KHR_platform_gbm {

constexpr const char *Name = "EGL_KHR_platform_gbm";
constexpr unsigned int PlatformGbmKhr = 0x31D7;

}  // namespace KHR_platform_gbm

namespace KHR_platform_wayland {

constexpr const char *Name = "EGL_KHR_platform_wayland";
constexpr unsigned int PlatformWaylandKhr = 0x31D8;

}  // namespace KHR_platform_wayland

namespace KHR_platform_x11 {

constexpr const char *Name = "EGL_KHR_platform_x11";
constexpr unsigned int PlatformX11Khr = 0x31D5;
constexpr unsigned int PlatformX11ScreenKhr = 0x31D6;

}  // namespace KHR_platform_x11

}  // namespace extensions

void LoadFunctions(util::Library &library);

}  // namespace graphics::egl
