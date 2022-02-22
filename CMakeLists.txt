cmake_minimum_required(VERSION 3.8)
project(cpp-rpc C CXX)




list (APPEND CMAKE_PREFIX_PATH "/home/marcus/Software/grpc/build/install")
list (APPEND PATH "/home/marcus/Software/grpc/build/install/bin")

include (cmake/common.cmake)

message (STATUS ${CMAKE_PREFIX_PATH})
message (STATUS ${PATH})

find_package(Protobuf REQUIRED)
# message(STATUS "Using protobuf ${protobuf_VERSION}")
find_package(gRPC CONFIG REQUIRED)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Proto file
get_filename_component(rg_proto "protos/helloworld.proto" ABSOLUTE)
get_filename_component(rg_proto_path "${rg_proto}" PATH)

# Generated sources
set(rg_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.pb.cc")
set(rg_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.pb.h")
set(rg_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.grpc.pb.cc")
set(rg_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.grpc.pb.h")
add_custom_command(
      OUTPUT "${rg_proto_srcs}" "${rg_proto_hdrs}" "${rg_grpc_srcs}" "${rg_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${rg_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${rg_proto}"
      DEPENDS "${rg_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# rg_grpc_proto
add_library(hw_grpc_proto
  ${rg_grpc_srcs}
  ${rg_grpc_hdrs}
  ${rg_proto_srcs}
  ${rg_proto_hdrs})
target_link_libraries(hw_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})



  # client build
  add_executable(client 
  src/client.cpp
  )

  target_link_libraries(client
  hw_grpc_proto
  # ${_REFLECTION}
  # ${_GRPC_GRPCPP}
  # ${_PROTOBUF_LIBPROTOBUF}
  )


    # server build
    add_executable(server 
    src/server.cpp
    )
  
    target_link_libraries(server
    hw_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF}
    )