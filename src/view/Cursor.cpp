// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "view/Cursor.h"
#include "view/Window.h"

Cursor::Cursor(Window *window) : window_(window) {}

int Cursor::row() { return window_->cursorPosition().x(); }

void Cursor::setRow(int row) {
  int newRow = std::min(std::max(row, 0), window_->area().height());
  window_->setCursorPosition(newRow, column());
}

int Cursor::column() { return window_->cursorPosition().y(); }

void Cursor::setColumn(int column) {
  int newColumn = std::min(std::max(column, 0), window_->area().width());
  window_->setCursorPosition(row(), newColumn);
}

void Cursor::up(int length) {
  int newRow = std::min(std::max(row() - length, 0), window_->area().height());
  window_->setCursorPosition(newRow, column());
}

void Cursor::down(int length) {
  int newRow = std::min(std::max(row() + length, 0), window_->area().height());
  window_->setCursorPosition(newRow, column());
}

void Cursor::left(int length) {
  int newColumn =
      std::min(std::max(column() - length, 0), window_->area().width());
  window_->setCursorPosition(row(), newColumn);
}

void Cursor::right(int length) {
  int newColumn =
      std::min(std::max(column() + length, 0), window_->area().width());
  window_->setCursorPosition(row(), newColumn);
}

// void Cursor::show() { window_->showCursor(); }

// void Cursor::hide() { window_->hideCursor(); }

// bool Cursor::visible() { return window_->cursorVisible(); }
