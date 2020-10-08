#include "type.h"

namespace codeana {
namespace kernel {
namespace util {


bool ISIDBEGIN(char x) {return ((x <= 'z' && x >= 'a') || (x <= 'Z' && x >= 'A') || (x == '_'));}
bool ISIDCHAR(char x) {return ((x <= '9' && x >= '0') || (x <= 'z' && x >= 'a') || (x <= 'Z' && x >= 'A') || (x == '_'));}
bool ISBLANK(char x) {return ((x == '\n') || (x == '\t') || (x == ' '));}

} // util
} // namespace kernel
} // namespace codeana
