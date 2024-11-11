#include "lib.h"

#include "version.h"

int version() {
  std::stringstream strValue;
  strValue << PROJECT_VERSION;
  unsigned int intValue;
  strValue >> intValue;
	return intValue;
}
