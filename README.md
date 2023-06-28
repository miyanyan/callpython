# callpython
  ��C++�е���python�ű�

## Ŀ��
* ��C++���ɵı�д��python��������
* ���յĳ����ܴ������������python�������ն�
* python��������������ܵ�С

## ����
* [pybind11](https://github.com/pybind/pybind11)�� ͨ�� ```vcpkg install pybind11``` ����
* python3.8�� 3.8 �����һ��֧�� Win7 �İ汾������������python38��windows64λ�汾����ɾ����һЩ����Ҫ�Ŀ�(����```tcl, tk```��, �����Դ���ui�⣬�����õ�)
  ��һЩ����Ҫ���ļ�(icon��test�ļ���)�����������һ�㣬�ڴ��ʱ���԰ѿ������е�ǰ����pythonû�õ��Ŀ�ȫ��ɾ����

## python��Ŀ¼�ṹ
�ڵ���pythonǰ�����˽�һ��python��Ŀ¼�ṹ���˽�python�Ĳ�ͬģ��ķŵ�ʲôλ�ã��ڰ�װpython���ڸ�Ŀ¼���¿��Կ��������ļ�
* python38.dll, python�Ķ�̬���ӿ⣬���ж�python�ĵ��ö����ڴˡ�Ϊ�����ƶ���binĿ¼��
* include�ļ��У�python��capiͷ�ļ�������python��Ҫinclude�����```Python.h```
* libs�ļ���, �ں�python38.lib, ����python��
* DLLs�ļ��У���������������Ķ�̬���ӿ��ļ�
* Lib�ļ��У�python���ڲ�ģ��
* Lib/site-packages�ļ��У��������⼴pip��װ�����Ŀ¼��

## ��дcmake
* ��������vcpkg�����������vcpkg��װ��python(��װpybind11ʱ��ͬʱ��װpython)��������vcpkgǰ���ҵ�python
  ```
  # cmake ����·����������뵱ǰ·��
  # �������ȫ�ֵ�python��ȷ�����������д��ڼ��ɣ������޸�
  set(ORIGINAL_CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})
  set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_DIR}/python ${ORIGINAL_CMAKE_PREFIX_PATH})
  # ֻ����python38�汾
  find_package(Python 3.8 EXACT COMPONENTS Interpreter Development REQUIRED)
  ```
* �� python38.dll��pythonģ�飬python�ű� ���������Ŀ¼
  ```
  add_custom_target(COPY_ASSETS 
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/python/DLLs ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/python/DLLs
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/python/Lib ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/python/Lib
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/python/bin ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/scripts ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/scripts
  )
  add_dependencies(${PROJECT_NAME} COPY_ASSETS)
  ```
* ���target������pybind11��pybind11���Զ�����python
  ```
  target_link_libraries(callpython PRIVATE pybind11::embed)
  ```
## ��дC++���ó���
��```scripts/find_max.py```�б�д�˷ǳ�������dfs��python���������ڳ�����c++�е�����
* ����python�������ĸ�Ŀ¼
  ```
  Py_SetPythonHome(L"./python");
  ```
* ��ʼ��python������ֱ��ʹ��pybind11�ĺ���
  ```
  py::scoped_interpreter guard{};
  ```
* ����pythonģ�飬ע�ⲻ��ʹ�����·��
  ```
  // python��module����ʹ�����·������˶��ڽű� ./scripts/find_max.py�� ��Ҫ�Ƚ�·����ӵ�path��
  py::module::import("sys").attr("path").attr("append")("./scripts");
  // python��ģ�������ļ���
  auto find_max = py::module::import("find_max");
  ```
* ����python������ֱ��ʹ��pybind11�ĺ�������

## ���
�ڲ�ʹ�õ�������������£������������Ŀ¼�Ѿ��߱������л�����������python�������ܴ�С26M��
�������python�ĵ������⣬���轫��Ӧ�⿽����Lib/site-packages�ļ����¡�


## �ο�
https://www.zhihu.com/question/48776632/answer/2336654649