
set(genutils_SRCS
  ast.cpp
  interpreter.cpp
  idltype.cpp
  dumptypes.cpp
)

add_library(genutils ${genutils_SRCS})
# install(TARGETS genparser LIBRARY DESTINATION lib)
