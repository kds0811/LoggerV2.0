#include "Logger.h"
#include <iostream>
#include <format>


struct TestStruct
{
  //std::string name{"KURWA"};
};

int main()
{
  int a         = 7;
  int* ptr      = &a;
  void* ptrVoid = nullptr;

  LOG_WARNING(" dwdwdw");

  LOG_WARNING(" Vector X -", 1.0, " --- Is  ", true);
  LOG_WARNING(" poiner - ", ptr, " ");
  LOG_MESSAGE("QUAT - W ", 0.56564, " X -  ", 0.12321, " Y - ", 1, " Z - ", 0.777777777777);
  LOG_MESSAGE("  ", ' ', "wdw", " ", nullptr, -556494949);
  LOG_MESSAGE("Void PTR", ptrVoid, &ptrVoid, 1525555.005255555555555555);
  LOG_MESSAGE();
  LOG_MESSAGE("");
  LOG_MESSAGE("Empty string test: ", "");

  double d     = 3.14;
  double* ptrD = &d;

  LOG_MESSAGE(" Double pointer - ", ptrD);
  LOG_MESSAGE(" Nullptr test - ", nullptr);
  LOG_MESSAGE(" Void pointer - ", static_cast<void*>(ptrD));
  LOG_MESSAGE(" Int max - ", std::numeric_limits<int>::max);
  LOG_MESSAGE(" Int min - ", std::numeric_limits<int>::min);
  LOG_MESSAGE(" Float epsilon - ", std::numeric_limits<float>::epsilon());
  LOG_MESSAGE(" Double infinity - ", std::numeric_limits<double>::infinity());
  LOG_MESSAGE(" Negative zero - ", -0.0);
  LOG_MESSAGE(" String literal - ", "Hello, World!");
  LOG_MESSAGE(" StdString - ", std::string("Hello, World!"));
  LOG_MESSAGE(" Special chars - ", "Newline:\nTab:\tQuote:\"");
  LOG_MESSAGE(" True test - ", true);
  LOG_MESSAGE(" False test - ", false);
  LOG_MESSAGE(" Expression test - ", 42 > 0);
  LOG_MESSAGE(" Expression test - ", 0 == 1);
  LOG_MESSAGE(" Char test - ", 'A');
  LOG_MESSAGE(" Wide char test - ", L'Ω');
  LOG_MESSAGE(" Special char test - ", '\t', " ddd");
  LOG_MESSAGE(" Non-printable char test - ", '\x01', " END");

  std::string stre("dwqdwq");
  LOG_MESSAGE(stre);

  std::unique_ptr<TestStruct> testUnique     = std::make_unique<TestStruct>();
  std::unique_ptr<TestStruct> testUniqueNull = nullptr;
  LOG_MESSAGE(" test unique - ", testUnique.get());
  LOG_MESSAGE(" test unique - ", testUniqueNull.get());

  LOG_ERROR(" Adress W - ", L" D:\\Logger\\Logger");
  LOG_ERROR(" Adress  - ", " D:\\Logger\\Logger");

  std::wstring path = L" D:\\boxyboi\\License";
  LOG_MESSAGE(path);

  LOG_MESSAGE(" C++20 format test - ", std::format("Formatted: {}", 42));


  return 0;
}