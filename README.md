# callpython
  在C++中调用python脚本

## 目标
* 在C++轻松的编写与python交互代码
* 最终的程序能打包并运行在无python环境的终端
* python的依赖体积尽可能的小

## 依赖
* [pybind11](https://github.com/pybind/pybind11)， 通过 ```vcpkg install pybind11``` 下载
* python3.8， 3.8 是最后一个支持 Win7 的版本。这里内置了python38的windows64位版本，并删除了一些不必要的库(比如```tcl, tk```等, 这是自带的ui库，很少用到)
  和一些不必要的文件(icon、test文件等)。如果更激进一点，在打包时可以把可以运行的前提下python没用到的库全都删除。

## python的目录结构
在调用python前，先了解一下python的目录结构，了解python的不同模块的放到什么位置，在安装python后，在根目录大致可以看到如下文件
* python38.dll, python的动态链接库，所有对python的调用都基于此。为方便移动至bin目录下
* include文件夹，python的capi头文件，调用python需要include这里的```Python.h```
* libs文件夹, 内含python38.lib, 链接python用
* DLLs文件夹，解释器运行所需的动态链接库文件
* Lib文件夹，python的内部模块
* Lib/site-packages文件夹，第三方库即pip安装在这个目录下

## 编写cmake
* 由于引入vcpkg后会优先搜索vcpkg安装的python(安装pybind11时会同时安装python)，在引入vcpkg前先找到python
  ```
  # cmake 搜索路径，这里加入当前路径
  # 如果采用全局的python则确保环境变量中存在即可，无需修改
  set(ORIGINAL_CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})
  set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_DIR}/python ${ORIGINAL_CMAKE_PREFIX_PATH})
  # 只搜索python38版本
  find_package(Python 3.8 EXACT COMPONENTS Interpreter Development REQUIRED)
  ```
* 将 python38.dll，python模块，python脚本 拷贝到输出目录
  ```
  add_custom_target(COPY_ASSETS 
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/python/DLLs ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/python/DLLs
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/python/Lib ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/python/Lib
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/python/bin ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/scripts ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/scripts
  )
  add_dependencies(${PROJECT_NAME} COPY_ASSETS)
  ```
* 添加target并链接pybind11，pybind11会自动链接python
  ```
  target_link_libraries(callpython PRIVATE pybind11::embed)
  ```
## 编写C++调用程序
在```scripts/find_max.py```中编写了非常暴力的dfs的python函数，现在尝试在c++中调用它
* 设置python解析器的根目录
  ```
  Py_SetPythonHome(L"./python");
  ```
* 初始化python，这里直接使用pybind11的函数
  ```
  py::scoped_interpreter guard{};
  ```
* 导入python模块，注意不能使用相对路径
  ```
  // python的module不能使用相对路径，因此对于脚本 ./scripts/find_max.py， 需要先将路径添加到path中
  py::module::import("sys").attr("path").attr("append")("./scripts");
  // python的模块名即文件名
  auto find_max = py::module::import("find_max");
  ```
* 调用python函数，直接使用pybind11的函数即可

## 打包
在不使用第三方库的条件下，编译完后的输出目录已经具备了所有环境，在引入python环境下总大小26M。
如果引入python的第三方库，则需将对应库拷贝到Lib/site-packages文件夹下。


## 参考
https://www.zhihu.com/question/48776632/answer/2336654649