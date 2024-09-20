cmake_minimum_required(VERSION 3.20)
project(Crypto_Experiment C)

set(CMAKE_C_STANDARD 99)

set(openssl_INC_DIR E:\\E_drive\\clion\\Environment\\OpenSSL-Win32\\include)
set(openssl_LINK_DIR E:\\E_drive\\clion\\Environment\\OpenSSL-Win32\\lib)
include_directories(${openssl_INC_DIR})
link_directories(${openssl_LINK_DIR})

file (GLOB files *.c */*.c)
foreach (file ${files})
    #string(REGEX REPLACE ".+/(.+)\\..*" "\\1" exe ${file})
    string(REGEX REPLACE ".+/(.+)/(.+)\\..*" "\\1--\\2" exe ${file})
    add_executable (${exe} ${file}
            extern_lib/Wrap/Cpp_lib/cpplib.cpp
            extern_lib/Wrap/wrap.c
            library_file/My_BigNum_GF_cal/My_BigNum_GF_cal.c
            library_file/My_Matrix_cal/My_Matrix_cal.c
            library_file/Classical_Cipher/Classical_Cipher.c
            library_file/conversion/lib_Convert.c
            library_file/DES/DES_enc_dec.c
            library_file/AES/AES_enc_dec.c
            library_file/SM4/SM4_enc_dec.c
            library_file/Stream_Cipher/Stream_Cipher.c
            library_file/RSA_Cipher/RSA_enc_dec.c
            library_file/ECC/ECC_enc_dec.c
            library_file/HashLib/Hash.c
            library_file/Digital_Signature/Digital_Signature.c

            library_file/miracl/hexdump.c
            library_file/miracl/mraes.c
            library_file/miracl/mralloc.c
            library_file/miracl/mrarth0.c
            library_file/miracl/mrarth1.c
            library_file/miracl/mrarth2.c
            library_file/miracl/mrarth3.c
            library_file/miracl/mrbits.c
            library_file/miracl/mrbrick.c
            library_file/miracl/mrcore.c
            library_file/miracl/mrcrt.c
            library_file/miracl/mrcurve.c
            library_file/miracl/mrebrick.c
            library_file/miracl/mrec2m.c
            library_file/miracl/mrecn2.c
            library_file/miracl/mrfast.c
            library_file/miracl/mrgcd.c
            library_file/miracl/mrgcm.c
            library_file/miracl/mrgf2m.c
            library_file/miracl/mrio1.c
            library_file/miracl/mrio2.c
            library_file/miracl/mrjack.c
            library_file/miracl/mrlucas.c
            library_file/miracl/mrmonty.c
            library_file/miracl/mrpower.c
            library_file/miracl/mrprime.c
            library_file/miracl/mrrand.c
            library_file/miracl/mrscrt.c
            library_file/miracl/mrshs256.c
            library_file/miracl/mrshs512.c
            library_file/miracl/mrshs.c
            library_file/miracl/mrsmall.c
            library_file/miracl/mrsroot.c
            library_file/miracl/mrstrong.c
            library_file/miracl/mrxgcd.c
            library_file/miracl/mrzzn2b.c
            library_file/miracl/mrzzn2.c
            library_file/miracl/mrzzn3.c
            library_file/miracl/sm2.c
            library_file/miracl/sm3.c)
    target_link_libraries(${exe} -lm)
    target_link_libraries(${exe} -lgmp)
    target_link_libraries(${exe} -lcrypto)
    #下面是链接.dll库的相关操作，因为这里已经有了My_BigNum_GF_cal的.dll文件，所以在上面就不引用注释掉的两行了
    #link_directories(./library_file/lib_dll)
    #target_link_libraries(${exe} libMy_BigNum_GF_cal.dll)
    #target_link_libraries(${exe} libhello.dll)
    message (\ \ \ \ --\ src/${exe}.c\ will\ be\ compiled\ to\ bin/${exe})
endforeach ()
# 如果你只需要根目录下的 test 文件夹的所有 .cpp 文件
#file (GLOB files test/*.cpp)
# 如果你只有两层目录的话
#file (GLOB files *.cpp */*.cpp)
# 同理，三层的话
#file (GLOB files *.cpp */*.cpp */*/*.cpp)
# 官方提供了一种递归的方法
# 这样在运行框会多一个 CMakeCXXCompilerId，不过无伤大雅
#file (GLOB_RECURSE files *.cpp)

include_directories("/library_file")
link_directories("/library_file")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX FLAGS -fexec-charset=GBK)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#修改clion 内存
MATH(EXPR stack_size "100000000")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}")
