# 川麻牌效率向听分析助手

基于 C++/Qt6 的川麻手牌向听与切牌效率分析工具。

## 功能

- 图形界面选择手牌（筒/条/万）
- 定缺花色选择
- 14 张手牌切牌分析：向听数、有效进张、进张种类
- 最优切牌高亮排序

## 环境安装

| 组件 | 版本 | 路径 |
|---|---|---|
| CMake | 4.3.3 | `C:\Program Files\CMake\bin` |
| Qt | 6.8.2 MinGW | `%USERPROFILE%\Qt\6.8.2\mingw_64` |
| MinGW | 13.1.0 | `%USERPROFILE%\Qt\Tools\mingw1310_64` |

## 快速开始

**编译：**

```bat
scripts\build.bat
```

**运行：**

```bat
scripts\run.bat
```

或直接双击 `build\SichuanMahjongAssistant.exe`。

## 手动构建步骤

```powershell
$env:Path = "$env:USERPROFILE\Qt\Tools\mingw1310_64\bin;$env:USERPROFILE\Qt\6.8.2\mingw_64\bin;C:\Program Files\CMake\bin;" + $env:Path
cmake -B build -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="$env:USERPROFILE\Qt\6.8.2\mingw_64" -DCMAKE_BUILD_TYPE=Release
cmake --build build -j 4
& "$env:USERPROFILE\Qt\6.8.2\mingw_64\bin\windeployqt.exe" build\SichuanMahjongAssistant.exe
.\build\SichuanMahjongAssistant.exe
```

也可使用 Qt Creator 直接打开 `CMakeLists.txt`。

## 项目结构

```
src/core/       核心算法（递归分解向听、效率计算）
src/analyzer/   分析器面向对象封装
src/gui/        Qt 主窗口
```

## 规则说明

- 仅筒/条/万 108 张
- 必须缺一门（定缺）
- 支持标准形与七对
