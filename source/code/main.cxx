#include "quaternion.hxx"

int main()
{
  Quaternion q;
  q = { 1.0f, 2.0f, 3.0f, 4.0f };

  std::cout << q << std::endl;
  std::cout << q.to_string() << std::endl;
  return 0;
}
