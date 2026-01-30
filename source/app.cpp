#include <fmt/format.h>

struct node
{
    ~node() = default;
    std::string describe() const { return "I am a node"; }
};

#include "factorys.hpp"

#define RUNTIME_VISUALIZER_IMPLEMENTATION
#include <runtime-visualizer.hpp>

#include <chrono>
#include <iostream>
#include <thread>
using namespace std::chrono_literals;

int main(int argc, char* argv[])
{
    std::ignore = argc;
    std::ignore = argv;
    std::locale::global(std::locale("zh_CN.UTF-8"));

    std::cout << "Hello, World! 测试中文" << std::endl;

    node_factorys node_fs;
    node_fs.register_group_from_absolute_path("tmp/创建", []() -> std::shared_ptr<node> { return nullptr; });
    node_fs.register_group_from_absolute_path("临时/节点/创建", []() -> std::shared_ptr<node> { return nullptr; });
    node_fs.register_group_from_absolute_path("创建", []() -> std::shared_ptr<node> { return nullptr; });
    runtime_visualizer viz;
    viz.initialize();
    viz.main_render([&]() {
        ImGui::Begin("Hello, world!");
        ImGui::Text("测试中文");
        ImGui::End();

        if (node_fs.root_factory->size() == 0)
        {
            ImGui::Text("没有可用的节点工厂");
        }
        else
        {
            node_fs.for_each(
                [](std::vector<std::string> stack, node_factorys::stack_status status, bool menu_status, std::shared_ptr<factory_group<std::function<std::shared_ptr<node>()>>> groups) {
                    if (menu_status == false)
                        return false;
                    if (status == node_factorys::stack_status::begin)
                        return ImGui::BeginMenu(stack.back().c_str());
                    else if (status == node_factorys::stack_status::content)
                    {
                        if (ImGui::MenuItem(stack.back().c_str()))
                            groups->factory_opt.value()();
                    }
                    else if (status == node_factorys::stack_status::end)
                        ImGui::EndMenu();
                    return false;
                });
        }
    });

    viz.wait_exit();
    return 0;
}