#pragma once

#include <cstdint>

#include "core.h"
#include "util.h"

namespace graphics::wayland::internal {

struct Buffer;
struct Callback;
struct Compositor;
struct DataDevice;
struct DataDeviceManager;
struct DataOffer;
struct DataSource;
struct Display;
struct Keyboard;
struct Output;
struct Pointer;
struct Region;
struct Registry;
struct Seat;
struct Shell;
struct ShellSurface;
struct Shm;
struct ShmPool;
struct SubCompositor;
struct Surface;
struct Touch;

inline namespace interfaces {

extern const Interface DisplayInterface;
extern const Interface RegistryInterface;
extern const Interface CallbackInterface;
extern const Interface CompositorInterface;
extern const Interface ShmPoolInterface;
extern const Interface ShmInterface;
extern const Interface BufferInterface;
extern const Interface DataOfferInterface;
extern const Interface DataSourceInterface;
extern const Interface DataDeviceInterface;
extern const Interface DataDeviceManagerInterface;
extern const Interface ShellInterface;
extern const Interface ShellSurfaceInterface;
extern const Interface SurfaceInterface;
extern const Interface SeatInterface;
extern const Interface PointerInterface;
extern const Interface KeyboardInterface;
extern const Interface TouchInterface;
extern const Interface OutputInterface;
extern const Interface RegionInterface;
extern const Interface SubCompositorInterface;
extern const Interface SubSurfaceInterface;

}  // namespace interfaces

struct Display final : public Proxy {
  enum class Error {
    kInvalidObject = 0,
    kInvalidMethod = 1,
    kNoMemory = 2,
    kErrorImplementation = 3,
  };

  struct Listener {
    using ErrorFunc = void (*)(void *data, Display *display, void *object_id,
                               uint32_t code, const char *message);
    using DeleteIdFunc = void (*)(void *data, Display *display, uint32_t id);

    ErrorFunc Error;
    DeleteIdFunc DeleteId;
  };
  static_assert(sizeof(Listener) == sizeof(void (*)()) * 2);

  int AddListener(const Listener *listener, void *data);

  Callback *Sync();
  Registry *GetRegistry();

 private:
  static constexpr uint32_t kSyncOpcode = 0;
  static constexpr uint32_t kGetRegistryOpcode = 1;
};
static_assert(sizeof(Display) == 1);

struct Registry final : public Proxy {
  struct Listener {
    using GlobalFunc = void (*)(void *data, Registry *registry, uint32_t name,
                                const char *interface, uint32_t version);
    using GlobalRemoveFunc = void (*)(void *data, Registry *registry,
                                      uint32_t name);

    GlobalFunc Global;
    GlobalRemoveFunc GlobalRemove;
  };
  static_assert(sizeof(Listener) == sizeof(void (*)()) * 2);

  int AddListener(const Listener *listener, void *data);

  Proxy *Bind(uint32_t name, const Interface *interface, uint32_t version);

 private:
  static constexpr uint32_t kBindOpcode = 0;
};
static_assert(sizeof(Registry) == 1);

struct Callback final : public Proxy {
  struct Listener {
    using DoneFunc = void (*)(void *data, Callback *callback,
                              uint32_t callback_data);

    DoneFunc Done;
  };
  static_assert(sizeof(Listener) == sizeof(void (*)()));

  int AddListener(const Listener *listener, void *data);
};
static_assert(sizeof(Callback) == 1);

struct Compositor final : public Proxy {
  Surface *CreateSurface();
  Region *CreateRegion();

 private:
  static constexpr uint32_t kCreateSurfaceOpcode = 0;
  static constexpr uint32_t kCreateRegionOpcode = 1;
};
static_assert(sizeof(Compositor) == 1);

struct Shm final : public Proxy {
  enum class Error {
    kInvalidFormat = 0,
    kInvalidStride = 1,
    kInvalidFd = 2,
  };
  enum class Format {
    kArgb8888 = 0,
    kXrgb8888 = 1,
    kC8 = 0x20203843,
    kRgb332 = 0x38424752,
    kBgr233 = 0x38524742,
    kXrgb4444 = 0x32315258,
    kXbgr4444 = 0x32314258,
    kRgbx4444 = 0x32315852,
    kBgrx4444 = 0x32315842,
    kArgb4444 = 0x32315241,
    kAbgr4444 = 0x32314241,
    kRgba4444 = 0x32314152,
    kBgra4444 = 0x32314142,
    kXrgb1555 = 0x35315258,
    kXbgr1555 = 0x35314258,
    kRgbx5551 = 0x35315852,
    kBgrx5551 = 0x35315842,
    kArgb1555 = 0x35315241,
    kAbgr1555 = 0x35314241,
    kRgba5551 = 0x35314152,
    kBgra5551 = 0x35314142,
    kRgb565 = 0x36314752,
    kBgr565 = 0x36314742,
    kRgb888 = 0x34324752,
    kBgr888 = 0x34324742,
    kXbgr8888 = 0x34324258,
    kRgbx8888 = 0x34325852,
    kBgrx8888 = 0x34325842,
    kAbgr8888 = 0x34324241,
    kRgba8888 = 0x34324152,
    kBgra8888 = 0x34324142,
    kXrgb2101010 = 0x30335258,
    kXbgr2101010 = 0x30334258,
    kRgbx1010102 = 0x30335852,
    kBgrx1010102 = 0x30335842,
    kArgb2101010 = 0x30335241,
    kAbgr2101010 = 0x30334241,
    kRgba1010102 = 0x30334152,
    kBgra1010102 = 0x30334142,
    kYuyv = 0x56595559,
    kYvyu = 0x55595659,
    kUyvy = 0x59565955,
    kVyuy = 0x59555956,
    kAyuv = 0x56555941,
    kNv12 = 0x3231564e,
    kNv21 = 0x3132564e,
    kNv16 = 0x3631564e,
    kNv61 = 0x3136564e,
    kYuv410 = 0x39565559,
    kYvu410 = 0x39555659,
    kYuv411 = 0x31315559,
    kYvu411 = 0x31315659,
    kYuv420 = 0x32315559,
    kYvu420 = 0x32315659,
    kYuv422 = 0x36315559,
    kYvu422 = 0x36315659,
    kYuv444 = 0x34325559,
    kYvu444 = 0x34325659
  };
  static_assert(sizeof(Format) == sizeof(uint32_t));

  struct Listener {
    using FormatFunc = void (*)(void *data, Shm *shm, Format format);

    FormatFunc Format;
  };
  static_assert(sizeof(Listener) == sizeof(void (*)()));

  int AddListener(const Listener *listener, void *data);

  ShmPool *CreatePool(int32_t fd, int32_t size);

 private:
  static constexpr uint32_t kCreatePoolOpcode = 0;
};
static_assert(sizeof(Shm) == 1);

struct ShmPool final : public Proxy {
  Buffer *CreateBuffer(int32_t offset, int32_t width, int32_t height,
                       int32_t stride, Shm::Format format);
  void Destroy();
  void Resize(int32_t size);

 private:
  static constexpr uint32_t kCreateBufferOpcode = 0;
  static constexpr uint32_t kDestroyOpcode = 1;
  static constexpr uint32_t kResizeOpcode = 2;
};
static_assert(sizeof(ShmPool) == 1);

struct Buffer final : public Proxy {
  struct Listener {
    using ReleaseFunc = void (*)(void *data, Buffer *buffer);

    ReleaseFunc Release;
  };
  static_assert(sizeof(Listener) == sizeof(void (*)()));

  int AddListener(const Listener *listener, void *data);

  void Destroy();

 private:
  static constexpr uint32_t kDestroyOpcode = 1;
};
static_assert(sizeof(Buffer) == 1);

struct DataOffer final : public Proxy {
  enum class Error {
    kInvalidFinish = 0,
    kInvalidActionMask = 1,
    kInvalidAction = 2,
    kInvalidOffer = 3
  };

  struct Listener {
    using OfferFunc = void (*)(void *data, DataOffer *offer,
                               const char *mime_type);
    using SourceActionsFunc = void (*)(void *data, DataOffer *offer,
                                       uint32_t source_actions);
    using ActionFunc = void (*)(void *data, DataOffer *offer,
                                uint32_t dnd_action);

    OfferFunc Offer;
    SourceActionsFunc SourceActions;
    ActionFunc Action;
  };
  static_assert(sizeof(Listener) == sizeof(void (*)()) * 3);

  int AddListener(const Listener *listener, void *data);

  void Accept(uint32_t serial, const char *mime_type);
  void Receive(const char *mime_type, int32_t fd);
  void Destroy();
  void Finish();
  void SetActions(uint32_t dnd_actions, uint32_t preferred_action);

 private:
  static constexpr uint32_t kAcceptOpcode = 0;
  static constexpr uint32_t kReceiveOpcode = 1;
  static constexpr uint32_t kDestroyOpcode = 2;
  static constexpr uint32_t kFinishOpcode = 3;
  static constexpr uint32_t kSetActionsOpcode = 4;
};
static_assert(sizeof(DataOffer) == 1);

struct DataSource final : public Proxy {
  enum class Error { kInvalidActionMask = 0, kInvalidSource = 1 };

  struct Listener {
    using TargetFunc = void (*)(void *data, DataSource *source,
                                const char *mime_type);
    using SendFunc = void (*)(void *data, DataSource *source,
                              const char *mime_type, int32_t fd);
    using CancelledFunc = void (*)(void *data, DataSource *source);
    using DndDropPerformedFunc = void (*)(void *data, DataSource *source);
    using DndFinishedFunc = void (*)(void *data, DataSource *source);
    using ActionFunc = void (*)(void *data, DataSource *source,
                                uint32_t dnd_action);

    TargetFunc Target;
    SendFunc Send;
    CancelledFunc Cancelled;
    DndDropPerformedFunc DndDropPerformed;
    DndFinishedFunc DndFinished;
    ActionFunc Action;
  };
  static_assert(sizeof(Listener) == sizeof(void (*)()) * 6);

  int AddListener(Listener *listener, void *data);

  void Offer(const char *mime_type);
  void Destroy();
  void SetActions(uint32_t dnd_actions);

 private:
  static constexpr uint32_t kOfferOpcode = 0;
  static constexpr uint32_t kDestroyOpcode = 1;
  static constexpr uint32_t kSetActionsOpcode = 2;
};
static_assert(sizeof(DataSource) == 1);

struct DataDevice final : public Proxy {
  enum class Error { kRole = 0 };

  struct Listener {
    using DataOfferFunc = void (*)(void *data, DataDevice *device,
                                   DataOffer *id);
    using EnterFunc = void (*)(void *data, DataDevice *device, uint32_t serial,
                               Surface *surface, Fixed x, Fixed y,
                               DataOffer *id);
    using LeaveFunc = void (*)(void *data, DataDevice *device);
    using MotionFunc = void (*)(void *data, DataDevice *device, uint32_t time,
                                Fixed x, Fixed y);
    using DropFunc = void (*)(void *data, DataDevice *device);
    using SelectionFunc = void (*)(void *data, DataDevice *device,
                                   DataOffer *id);

    DataOfferFunc DataOffer;
    EnterFunc Enter;
    LeaveFunc Leave;
    MotionFunc Motion;
    DropFunc Drop;
    SelectionFunc Selection;
  };
  static_assert(sizeof(Listener) == sizeof(void (*)()) * 6);

  int AddListener(const Listener *listener, void *data);

  void StartDrag(DataSource *source, Surface *origin, Surface *icon,
                 uint32_t serial);
  void SetSelection(DataSource *source, uint32_t serial);
  void Release();

 private:
  static constexpr uint32_t kStartDragOpcode = 0;
  static constexpr uint32_t kSetSelectionOpcode = 1;
  static constexpr uint32_t kReleaseOpcode = 2;
};
static_assert(sizeof(DataDevice) == 1);

}  // namespace graphics::wayland::internal

#include "protocol.inl"
