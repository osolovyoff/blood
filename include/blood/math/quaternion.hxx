#pragma once
#ifndef tesla_quaternion_hxx__
#define tesla_quaternion_hxx__

#include <bitset>
#include <cstring>
#include <climits>
#include <sstream>
#include <iostream> // TODO DELETE
#include <initializer_list>

class Quaternion
{
public:
  Quaternion();
  Quaternion(const char* data);
  Quaternion(const float* data);
  Quaternion(float, float, float, float);

  void operator=(float);
  std::string to_string();


  friend std::ostream& operator<<(std::ostream& os, const Quaternion& quat);

private:
  union Data
  {
    char  bytes[16];
    float values[4];
  }m_data;
};

std::ostream& operator<<(std::ostream& os, const Quaternion& quat);

#endif // tesla_quaternion_hxx__