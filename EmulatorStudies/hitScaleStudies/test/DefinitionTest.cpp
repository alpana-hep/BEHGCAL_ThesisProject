#include "Definer.h"
#include "HGCalDefinition.h"

int main() {
  Definer d;
  
  const std::string str1 ("FeMappingV9.txt");
  //const std::string str2 ("double.txt");
  d.readFile(str1);

  std::vector<unsigned> dummy;
  dummy.push_back(123);
  dummy.push_back(456);
  dummy.push_back(789);
  dummy.push_back(1234);
  dummy.push_back(1000);

  SlinkDefinition ld;
  d.defineSlink(dummy,ld);

  ld.print();
  return 0;
};
