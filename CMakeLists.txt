cmake_minimum_required(VERSION 3.1)

set(project_name my-project) ## rename your project here

project(${project_name})

set(CMAKE_CXX_STANDARD 11)

add_library(${project_name}-lib
        src/controller/StaticController.hpp
        src/controller/UserController.hpp
        src/controller/ProjectController.hpp
        src/controller/ComponentController.hpp
        src/controller/UploadController.hpp
        src/controller/MyController.hpp
        src/controller/AuthController.hpp
        src/db/UserDb.hpp
        src/db/ProjectDb.hpp
        src/db/ComponentDb.hpp
        src/db/Db.hpp
        src/dto/PageDto.hpp
        src/dto/StatusDto.hpp
        src/dto/UserDto.hpp
        src/dto/ProjectDto.hpp
        src/dto/ComponentRecordDto.hpp
        src/dto/AuthDto.hpp
        src/service/BaseService.hpp
        src/service/AuthService.cpp
        src/service/AuthService.hpp
        src/service/UserService.cpp
        src/service/UserService.hpp
        src/service/ProjectService.cpp
        src/service/ProjectService.hpp
        src/service/ComponentService.cpp
        src/service/ComponentService.hpp
        src/AppComponent.hpp
        src/DatabaseComponent.hpp
        src/SwaggerComponent.hpp
        src/ErrorHandler.cpp
        src/ErrorHandler.hpp
        src/auth/JWT.hpp
        src/auth/JWT.cpp
        src/auth/AuthHandler.hpp
        src/auth/AuthHandler.cpp
        src/interceptor/AuthInterceptor.hpp
        src/interceptor/AuthInterceptor.cpp)

# link libs

# 可以找到 vcpkg 中的包
INCLUDE_DIRECTORIES(
  E:/ljh/software/vcpkg/installed/x64-windows/include
)

# 连接 vcpkg 中的包
LINK_DIRECTORIES(
    E:/ljh/software/vcpkg/installed/x64-windows/lib/
)


# 编译目录
target_include_directories(${project_name}-lib PUBLIC src)

# find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(oatpp          1.3.0 REQUIRED)
find_package(oatpp-swagger  1.3.0 REQUIRED)
find_package(oatpp-sqlite   1.3.0 REQUIRED)
find_package(oatpp-openssl      1.3.0 REQUIRED)
find_package(OpenSSL   REQUIRED)

# https://github.com/microsoft/vcpkg/issues/10626
# find_package(jwt-cpp REQUIRED PATHS E:/ljh/software/vcpkg/packages/jwt-cpp_x64-windows/include/jwt-cpp)

target_link_libraries(${project_name}-lib
        PUBLIC unofficial::sqlite3::sqlite3
        # Oat++
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-swagger
        PUBLIC oatpp::oatpp-sqlite

        # third-party
        # PUBLIC jwt-cpp::jwt-cpp
        # PUBLIC picojson
        PUBLIC OpenSSL::SSL
        PUBLIC OpenSSL::Crypto
)

# 依赖的一些相关路径
add_definitions(
        ## define path to swagger-ui static resources folder
        -DOATPP_SWAGGER_RES_PATH="${oatpp-swagger_INCLUDE_DIRS}/../bin/oatpp-swagger/res"

        ## SQLite database file
        -DDATABASE_FILE="${CMAKE_CURRENT_SOURCE_DIR}/db.sqlite"
        ## SQLite database test file
        -DTESTDATABASE_FILE="${CMAKE_CURRENT_SOURCE_DIR}/test-db.sqlite"

        ## Path to database migration scripts
        -DDATABASE_MIGRATIONS="${CMAKE_CURRENT_SOURCE_DIR}/sql"

        ## Path to save the upload file
        -DUPLOADFILEPATH="${CMAKE_CURRENT_SOURCE_DIR}/upload.jpg"
)

## add executables

add_executable(${project_name}-exe src/App.cpp)
target_link_libraries(${project_name}-exe ${project_name}-lib)


add_dependencies(${project_name}-exe ${project_name}-lib)

# add_executable(${project_name}-test
#         test/tests.cpp
#         test/app/TestClient.hpp
#         test/app/TestDatabaseComponent.hpp
#         test/app/TestComponent.hpp
#         test/UserControllerTest.hpp
#         test/UserControllerTest.cpp)
# target_link_libraries(${project_name}-test ${project_name}-lib)

# add_dependencies(${project_name}-test ${project_name}-lib)

# TARGET_LINK_LIBRARIES(${project_name}-lib 
# oatpp::oatpp oatpp-swagger oatpp-sqlite
# )

# set_target_properties(${project_name}-lib ${project_name}-exe ${project_name}-test PROPERTIES
#         CXX_STANDARD 11
#         CXX_EXTENSIONS OFF
#         CXX_STANDARD_REQUIRED ON
# )

enable_testing()
add_test(project-tests ${project_name}-test)
