// Copyright 2019- <collie.org>
// Apache License Version 2.0

#pragma once

class Approximate {
public:
  static bool eq(float a, float b, float epsilon = 0.00001f);
  static bool eq(double a, double b, double epsilon = 0.000000000001);

  static bool gt(float a, float b, float epsilon = 0.00001f);
  static bool gt(double a, double b, double epsilon = 0.000000000001);

  static bool lt(float a, float b, float epsilon = 0.00001f);
  static bool lt(double a, double b, double epsilon = 0.000000000001);
};
