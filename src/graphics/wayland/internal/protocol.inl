namespace graphics::wayland::internal {

inline int Buffer::AddListener(const Buffer::Listener *listener, void *data) {
  return static_cast<Proxy *>(this)->AddListener(
      const_cast<void (**)()>(reinterpret_cast<void (*const *)()>(listener)),
      data);
}

inline void Buffer::Destroy() {
  Marshal(kDestroyOpcode);
  static_cast<Proxy *>(this)->Destroy();
}

inline int Callback::AddListener(const Callback::Listener *listener,
                                 void *data) {
  return static_cast<Proxy *>(this)->AddListener(
      const_cast<void (**)()>(reinterpret_cast<void (*const *)()>(listener)),
      data);
}

inline Surface *Compositor::CreateSurface() {
  return reinterpret_cast<Surface *>(
      MarshalConstructor(kCreateSurfaceOpcode, &SurfaceInterface, nullptr));
}

inline Region *Compositor::CreateRegion() {
  return reinterpret_cast<Region *>(
      MarshalConstructor(kCreateRegionOpcode, &RegionInterface, nullptr));
}

inline int DataDevice::AddListener(const DataDevice::Listener *listener,
                                   void *data) {
  return static_cast<Proxy *>(this)->AddListener(
      const_cast<void (**)()>(reinterpret_cast<void (*const *)()>(listener)),
      data);
}

inline void StartDrag(DataSource *source, Surface *origin, Surface *icon, uint32_t serial) {
  
}

inline int DataOffer::AddListener(const DataOffer::Listener *listener,
                                  void *data) {
  return static_cast<Proxy *>(this)->AddListener(
      const_cast<void (**)()>(reinterpret_cast<void (*const *)()>(listener)),
      data);
}

inline void DataOffer::Accept(uint32_t serial, const char *mime_type) {
  Marshal(kAcceptOpcode, serial, mime_type);
}

inline void DataOffer::Receive(const char *mime_type, int32_t fd) {
  Marshal(kReceiveOpcode, mime_type, fd);
}

inline void DataOffer::Destroy() {
  Marshal(kDestroyOpcode);
  static_cast<Proxy *>(this)->Destroy();
}

inline void DataOffer::Finish() { Marshal(kFinishOpcode); }

inline void DataOffer::SetActions(uint32_t dnd_actions,
                                  uint32_t preferred_action) {
  Marshal(kSetActionsOpcode, dnd_actions, preferred_action);
}

inline int DataSource::AddListener(DataSource::Listener *listener, void *data) {
  return static_cast<Proxy *>(this)->AddListener(
      const_cast<void (**)()>(reinterpret_cast<void (*const *)()>(listener)),
      data);
}

inline void DataSource::Offer(const char *mime_type) {
  Marshal(kOfferOpcode, mime_type);
}

inline void DataSource::Destroy() {
  Marshal(kDestroyOpcode);
  static_cast<Proxy *>(this)->Destroy();
}

inline void DataSource::SetActions(uint32_t dnd_actions) {
  Marshal(kSetActionsOpcode, dnd_actions);
}

inline int Display::AddListener(const Display::Listener *listener, void *data) {
  return static_cast<Proxy *>(this)->AddListener(
      const_cast<void (**)()>(reinterpret_cast<void (*const *)()>(listener)),
      data);
}

inline Callback *Display::Sync() {
  Proxy *callback;
  callback = MarshalConstructor(kSyncOpcode, &CallbackInterface, nullptr);
  return reinterpret_cast<Callback *>(callback);
}

inline Registry *Display::GetRegistry() {
  return reinterpret_cast<Registry *>(
      MarshalConstructor(kGetRegistryOpcode, &RegistryInterface, nullptr));
}

inline int Registry::AddListener(const Registry::Listener *listener,
                                 void *data) {
  return static_cast<Proxy *>(this)->AddListener(
      const_cast<void (**)()>(reinterpret_cast<void (*const *)()>(listener)),
      data);
}

inline Proxy *Registry::Bind(uint32_t name, const Interface *interface,
                             uint32_t version) {
  return MarshalConstructorVersioned(kBindOpcode, interface, version, name,
                                     interface->Name, version, nullptr);
}

inline ShmPool *Shm::CreatePool(int32_t fd, int32_t size) {
  return reinterpret_cast<ShmPool *>(
      MarshalConstructor(kCreatePoolOpcode, &ShmPoolInterface, fd, size));
}

inline Buffer *ShmPool::CreateBuffer(int32_t offset, int32_t width,
                                     int32_t height, int32_t stride,
                                     Shm::Format format) {
  return reinterpret_cast<Buffer *>(
      MarshalConstructor(kCreateBufferOpcode, &BufferInterface, nullptr, offset,
                         width, height, stride, format));
}

inline void ShmPool::Destroy() {
  Marshal(kDestroyOpcode);
  static_cast<Proxy *>(this)->Destroy();
}

inline void ShmPool::Resize(int32_t size) { Marshal(kResizeOpcode, size); }

}  // namespace graphics::wayland::internal
