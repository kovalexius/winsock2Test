cmake_minimum_required(VERSION 2.8)

add_executable(testWinsock
				./winsockTest.cpp
				)
				
target_link_libraries(testWinsock
-lws2_32
)