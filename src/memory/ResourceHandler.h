// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include <boost/core/noncopyable.hpp>

namespace fastype {

typedef void (*ResourceHandler)(void *data);

class ResourceHolder : public boost::noncopyable {
public:
  ResourceHolder();
  ResourceHolder(ResourceHandler handler, void *resource);
  virtual ~ResourceHolder();

  virtual ResourceHandler &handler();
  virtual void setHandler(ResourceHandler handler);
  virtual void *&resource();
  virtual void setResource(void *resource);

protected:
  ResourceHandler handler_;
  void *resource_;
};

} // namespace fastype
