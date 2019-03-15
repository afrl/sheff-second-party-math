add_executable(secondpartymath_dev source/main.cpp)
target_link_libraries(secondpartymath_dev PRIVATE SecondPartyMath::SecondPartyMath)

include(${CMAKE_SOURCE_DIR}/cmake/Utilities.cmake)
assign_source_group("second_party_math" ${SECONDPARTYMATH_HEADER_LIST})
assign_source_group("second_party_math" ${SECONDPARTYMATH_SOURCE_LIST})
