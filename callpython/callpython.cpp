#include <iostream>
#include <thread>
#include <vector>

#include <pybind11/embed.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class Pybind11Instance
{
public:
    static Pybind11Instance& getInstance()
    {
        static Pybind11Instance instance;
        return instance;
    }

    // 删除拷贝构造和赋值操作
    Pybind11Instance(const Pybind11Instance&) = delete;
    Pybind11Instance& operator=(const Pybind11Instance&) = delete;

    // 初始化Python解释器
    static bool init()
    {
        try {
            getInstance();
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "Failed to initialize Python: " << e.what() << std::endl;
            return false;
        }
    }

    // 提供一个方便的方法来执行需要GIL的Python操作
    template<typename Func>
    static auto withGIL(Func&& func)
    {
        py::gil_scoped_acquire gil;
        return std::forward<Func>(func)();
    }

private:
    Pybind11Instance()
        : guard_()
        , gil_release_()
    {
    }
    ~Pybind11Instance() = default;

    py::scoped_interpreter guard_;
    py::gil_scoped_release gil_release_;
};

void test_py()
{
    std::cout << "thread id: " << std::this_thread::get_id() << "\n";
    // 获取gil
    Pybind11Instance::withGIL([]() {
        // python的module不能使用相对路径，因此对于脚本 ./scripts/find_max.py， 需要先将路径添加到path中
        py::module::import("sys").attr("path").attr("append")("./scripts");
        // python的模块名即文件名
        auto find_max = py::module::import("find_max");
        auto A = std::vector{1, 2, 3, 4, 5, 6, 7, 8};
        auto B = std::vector{14, 15};
        auto cmp1 = py::cpp_function([](int a, int b) { return a * b; });
        // vector 会自动转换为python的list
        auto ans1 = find_max.attr("find_max")(A, B, cmp1);
        auto anss1 = ans1.cast<py::list>();
        py::print(anss1);

        auto cmp2 = py::cpp_function([](int a, int b) { return a + b; });
        auto ans2 = find_max.attr("find_max")(A, B, cmp2);
        auto anss2 = ans2.cast<py::list>();
        py::print(anss2);

        std::cout
            << "---------------------------------------------cpp-----------------------------------------------\n";
        std::cout << anss1;
    });
}

int main()
{
    // python 根目录
    Py_SetPythonHome(L"./python");
    // 初始化 python
    Pybind11Instance::init();

    auto th = std::thread([]() {
        std::cout << "test py in thread: \n";
        test_py();
    });

    std::cout << "test py in main thread: \n";
    test_py();

    th.join();

    return 0;
}
