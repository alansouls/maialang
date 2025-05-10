#include "my_gc_language.h"
#include "gc.h"
#include "interpreter.h"

using namespace MaiaLang;

int main() {
  GC::printInfo();

  Interpreter interpreter;

  try {
    interpreter.execute(
        "test.maia",
        "println(\"Hello World - with aaa MaiaLang!\", \"owdkaodk\");");
  } catch (const std::runtime_error &err) {
    std::cerr << err.what() << '\n';
    return -1;
  }

  GC::collect();

  GC::printInfo();
}
