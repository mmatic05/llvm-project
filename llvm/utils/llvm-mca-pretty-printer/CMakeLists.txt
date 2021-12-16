if (LLVM_INCLUDE_UTILS AND LLVM_INCLUDE_TOOLS)
  add_custom_command(
    OUTPUT ${LLVM_TOOLS_BINARY_DIR}/llvm-mca-pretty-printer
    DEPENDS ${LLVM_MAIN_SRC_DIR}/utils/llvm-mca-pretty-printer/llvm-mca-pretty-printer.py
    DEPENDS llvm-mca
    COMMAND ${CMAKE_COMMAND} -E copy ${LLVM_MAIN_SRC_DIR}/utils/llvm-mca-pretty-printer/llvm-mca-pretty-printer.py ${LLVM_TOOLS_BINARY_DIR}/llvm-mca-pretty-printer
    COMMENT "Copying llvm-mca-pretty-printer into ${LLVM_TOOLS_BINARY_DIR}"
    )
  add_custom_target(llvm-mca-pretty-printer ALL
    DEPENDS ${LLVM_TOOLS_BINARY_DIR}/llvm-mca-pretty-printer
    )
  if (NOT LLVM_BUILD_TOOLS)
    set_target_properties(llvm-mca-pretty-printer PROPERTIES EXCLUDE_FROM_ALL ON)
  endif()
  set_target_properties(llvm-mca-pretty-printer PROPERTIES FOLDER "Tools")
endif()
