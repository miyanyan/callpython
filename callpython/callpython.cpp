#include <vector>
#include <iostream>

#include <pybind11/embed.h>
#include <pybind11/stl.h>

namespace py = pybind11;

int main()
{
    // python 根目录
    Py_SetPythonHome(L"./python");
    // 初始化 python
    py::scoped_interpreter guard{};

    // python的module不能使用相对路径，因此对于脚本 ./scripts/find_max.py， 需要先将路径添加到path中
    py::module::import("sys").attr("path").attr("append")("./scripts");
    // python的模块名即文件名
    auto find_max = py::module::import("find_max");
    auto A = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8 };
    auto B = std::vector{ 14, 15 };
    auto cmp1 = py::cpp_function([](int a, int b) {return a * b; });
    // vector 会自动转换为python的list
    auto ans1 = find_max.attr("find_max")(A, B, cmp1);
    auto anss1 = ans1.cast<py::list>();
    py::print(anss1);

    auto cmp2 = py::cpp_function([](int a, int b) {return a + b; });
    auto ans2 = find_max.attr("find_max")(A, B, cmp2);
    auto anss2 = ans2.cast<py::list>();
    py::print(anss2);

    std::cin.get();
    
    return 0;
}
