#pragma once

#include "protocol.h"

class Generator {
 public:
  virtual ~Generator();

  virtual void Generate(const Protocol &protocol) = 0;
};
