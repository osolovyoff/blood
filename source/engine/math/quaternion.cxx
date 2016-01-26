#include "quaternion.hxx"

Quaternion::Quaternion()
{
  memset(m_data.bytes, 0, sizeof(Data));
  m_data.bytes[0] = 1;
}

Quaternion::Quaternion(const char* data)
{
  memcpy(m_data.bytes, data, sizeof(Data));
}

Quaternion::Quaternion(const float* data)
{
  memcpy(m_data.values, data, sizeof(Data));
}

Quaternion::Quaternion(float f1, float f2, float f3, float f4)
{
  m_data.values[0] = f1;
  m_data.values[0] = f2;
  m_data.values[0] = f3;
  m_data.values[0] = f4;
}

void Quaternion::operator=(float f)
{
  *this = Quaternion(f, f, f, f);
}

std::string Quaternion::to_string()
{
  std::stringstream ss;
  ss
    << "w:"
    << m_data.values[0]
    << " i:"
    << m_data.values[1]
    << " j:"
    << m_data.values[2]
    << " k:"
    << m_data.values[3];
  return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Quaternion& quat)
{
  const char* beg = quat.m_data.bytes;
  const char* end = beg + sizeof(quat.m_data.bytes);
  while (beg != end)
    os << std::bitset<CHAR_BIT>(*beg++);
  return os;
}