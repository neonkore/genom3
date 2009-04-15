
set(genutils_SRCS
  ast.cpp
  interpreter.cpp
)

add_library(genutils ${genutils_SRCS})
# install(TARGETS genparser LIBRARY DESTINATION lib)
