// Copyright 2019- <rsc-lang>
// Apache License Version 2.0

#pragma once

class Approximate {
public:
  // equal
  static bool eq(float a, float b, float epsilon = 0.00001f);
  static bool eq(double a, double b, double epsilon = 0.000000000001);

  // greater than
  static bool gt(float a, float b, float epsilon = 0.00001f);
  static bool gt(double a, double b, double epsilon = 0.000000000001);

  // less than
  static bool lt(float a, float b, float epsilon = 0.00001f);
  static bool lt(double a, double b, double epsilon = 0.000000000001);
};
