// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "ResourceHandler.h"

namespace fastype {

ResourceHolder::ResourceHolder() : handler_(nullptr), resource_(nullptr) {}

ResourceHolder::ResourceHolder(ResourceHandler handler, void *resource)
    : handler_(handler), resource_(resource) {}

ResourceHolder::~ResourceHolder() {
  if (handler_) {
    handler_(resource_);
  }
  handler_ = nullptr;
  resource_ = nullptr;
}

ResourceHandler &ResourceHolder::handler() { return handler_; }
void ResourceHolder::setHandler(ResourceHandler handler) { handler_ = handler; }
void *&ResourceHolder::resource() { return resource_; }
void ResourceHolder::setResource(void *resource) { resource_ = resource; }

} // namespace fastype
