// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "interface/Parentable.h"

ParentableImpl::ParentableImpl(Ast *parentableImpl)
    : parentableImpl_(parentableImpl) {}

Ast *&ParentableImpl::parent() { return parentableImpl_; }

Ast *ParentableImpl::parent() const { return parentableImpl_; }

