cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(spline_fitting)

# Eigen
set(EIGEN_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/SplineFitting/dependencies/eigen)
include_directories(${EIGEN_DIR})

# ANN
set(ANN_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/SplineFitting/dependencies/ann)
include_directories(${ANN_DIR}/include)
link_directories(${ANN_DIR}/lib)

#include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SplineFitting)



set(incs
			SplineFitting/cubic_b_spline.h
			SplineFitting/spline_curve_fitting.h
			SplineFitting/read_write_asc.h
            )

set(srcs    SplineFitting/cubic_b_spline.cpp
            SplineFitting/spline_curve_fitting.cpp
						SplineFitting/read_write_asc.cpp
			)


add_executable(mesh.exe ${incs} ${srcs} mesh.cpp )


target_link_libraries(mesh.exe
			debug ANNd optimized ANN
			)
