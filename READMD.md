# Hot Reload Tool

一个基于 **C++20** 实现的轻量级资源热加载与配置管理工具。  
项目支持资源目录扫描、JSON/YAML 配置解析、文件变更检测与热更新流程，主要用于模拟客户端开发中常见的资源调试与配置管理场景。

---

## 项目简介

在客户端开发过程中，配置文件和文本资源经常需要频繁修改。如果每次修改后都要重启程序，会显著增加调试成本。  
本项目围绕这一问题，实现了一个最小可运行的资源热加载工具，完成了以下基础能力：

- 递归扫描指定目录下的资源文件
- 识别并加载文本、JSON、YAML 三类资源
- 保存资源元信息并提供统一查询接口
- 基于文件修改时间检测资源变更
- 在程序运行期间自动重新加载变更资源
- 通过主循环轮询模拟热更新流程

该项目主要用于练习以下内容：

- C++20 工程组织与模块划分
- `std::filesystem` 文件系统操作
- JSON / YAML 配置解析
- 资源缓存与统一访问接口设计
- 热加载与轮询机制实现

---

## 技术栈

- **C++20**
- **CMake**
- **STL**
- **std::filesystem**
- **nlohmann/json**
- **yaml-cpp**

---

## 已实现功能

### 1. 资源目录扫描
- 支持递归扫描指定目录
- 识别支持的文件类型：
  - `.txt`
  - `.json`
  - `.yaml`
  - `.yml`

### 2. 资源元信息管理
- 为每个资源记录：
  - 文件路径
  - 资源类型
  - 最近修改时间

### 3. 统一资源存储
- 提供统一的 `ResourceStore`
- 支持：
  - 文本资源加载与查询
  - JSON 配置加载与查询
  - YAML 配置加载与查询

### 4. 文件变更检测
- 基于 `last_write_time` 检测文件变化
- 支持识别：
  - 新增
  - 修改
  - 删除

### 5. 热加载流程
- 程序运行期间周期性轮询文件状态
- 在检测到资源变化后自动重新加载
- 避免调试时频繁重启程序

### 6. 基础运行验证
- 支持初始化加载验证
- 支持资源热更新流程验证
- 支持通过 CMake 独立构建和运行

---

## 项目结构

```text
hot_reload_tool/
├─ CMakeLists.txt
├─ README.md
├─ data/
│  ├─ configs/
│  │  ├─ app.json
│  │  └─ render.yaml
│  └─ resources/
│     └─ welcome.txt
├─ include/
│  └─ hotreload/
│     ├─ types.hpp
│     ├─ directory_scanner.hpp
│     ├─ resource_store.hpp
│     ├─ file_watcher.hpp
│     └─ hot_reload_app.hpp
└─ src/
   ├─ types.cpp
   ├─ directory_scanner.cpp
   ├─ resource_store.cpp
   ├─ file_watcher.cpp
   ├─ hot_reload_app.cpp
   └─ main.cpp