# LLVM-Clang 配置

1. 首先，从官方下载（一般部署在github）clang 13.x win64.exe，下载之后安装，注意添加路径到环境变量里。
2. 然后我们打开vscode,ctrl+·，打开终端，输入clang，如果能正确调用到clang则进行下一步，否则，就是环境变量没设置好，或者需要重新加载vscode的环境。
3. ctrl+shift+p，输入 scan for kits，让它查找clang编译器，如果第2步没问题，肯定能找得到。
4. ctrl+shift+p，输入 edit kits，我们打开json配置文件，找到clang对应的项，如果它的编译器内容是clang.exe clang++.exe，那么一定要改成clang-cl.exe，因为后者才是msvc like的编译器。
5. 打开一个CMake工程，我们ctrl+shift+p，cmake config，让它配置工程，如果出现找不到库之类的，说明环境不行，我们要找到visual studio 20xx专用的终端，打开，然后在终端里输入`code`打开vscode，再配置工程，一般就能正常工作了。