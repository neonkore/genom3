
set(genutils_SRCS
  ast.cpp
  interpreter.cpp
  idltype.cpp
  dumptypes.cpp
  idlvisitor.cpp
  cvisitor.cpp
#  cppvisitor.cpp
)

add_library(genutils ${genutils_SRCS})
# install(TARGETS genparser LIBRARY DESTINATION lib)
