#include "class.h"

void dummyFunction() {
  std::cout << "This should not be called!" << std::endl;
}

int main(int argc, char** argv) {
  // In Production:
  std::cout << "-----Unit Test----" << std::endl;
  // The Sen environment create the component and trigger its registration
  MySenComponent myComp;
  myComp.registered();

  // In Unit tests:
  std::cout << std::endl << std::endl << "-----In Production----" << std::endl << std::endl;

  // In the test fixture we create the mock interface and the unit to be tested.
  // We connect to a dummy function dummyFunction that will never be called in the test.
  // It would be called if a Sen environment would be running and the onXXChange callback
  // of the interface would be called.
  // In the context of the unit test, we just want to make sure that on change itself has been
  // called. Not that it is calling the Callback it is been given to.
  DataInterfaceMock* mock = new DataInterfaceMock();
  MyUnit myTestedObject([&](OnChangedCb&& onChanged) { onChanged(SenCallback{[&]() { dummyFunction(); }}); });

  // This is the logic that happens in the unit and is being tested
  // (the unit self retrieve its interface and connects appropriately)
  myTestedObject.setInterface(mock);
  myTestedObject.connect();

  // We assert that OnChange has been called via a simple call counter
  std::cout << "OnChangeCallCounter: " << mock->onchangedCallCounter << std::endl;
  if (mock->onchangedCallCounter > 0U) {
    std::cout << "TEST SUCESS" << std::endl;
  }

  return 0;
}
