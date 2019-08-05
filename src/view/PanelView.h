// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "View.h"
#include "boost/noncopyable.hpp"
#include <memory>
#include <vector>

namespace fastype {

class PanelView : public View, boost::noncopyable {
public:
  PanelView();
  PanelView(std::shared_ptr<View> parent);
  virtual ~PanelView();

  virtual std::string name() const;
  virtual void setName(const std::string &name);
  virtual std::shared_ptr<View> parent() const;
  virtual void setParent(std::shared_ptr<View> parent);
  virtual enum ViewType type() const;

  int size() const;
  bool vertical() const;   // children from up to down
  bool horizontal() const; // children from left to right

  std::shared_ptr<View> get(int index) const;
  void append(std::shared_ptr<View> c);
  void insert(int index, std::shared_ptr<View> c);
  void remove(int index);
  int find(const std::string &name) const;

private:
  std::shared_ptr<View> parent_;
  std::vector<std::shared_ptr<View>> children_;
};

} // namespace fastype
