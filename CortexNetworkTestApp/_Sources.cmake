file(GLOB_RECURSE sources CONFIGURE_DEPENDS
  "src/*.c"
  "src/*.cpp"
)

file(GLOB_RECURSE headers CONFIGURE_DEPENDS
  "src/*.h"
  "src/*.hpp"
)
