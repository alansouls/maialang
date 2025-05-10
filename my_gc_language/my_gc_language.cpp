#include "my_gc_language.h"
#include "gc.h"
#include "interpreter.h"

using namespace MaiaLang;

int main() {
  GC::printInfo();

  Interpreter interpreter;

  try {
    // interpreter.execute("test.maia", "let string c = \"Hello, World! Maia
    // lang\";\nlet int d = 5;\nprintln(\"Hello, World - with MaiaLang!\");");
    interpreter.execute("test.maia",
                        "println(\"Hello World - with aaa MaiaLang!\");");
  } catch (const std::runtime_error &err) {
    std::cerr << err.what() << '\n';
    return -1;
  }

  GC::collect();

  GC::printInfo();
}
