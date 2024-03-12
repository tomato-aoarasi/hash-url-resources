# Hash URL Resources
只有输入HASH值才能得到特定的资源<br>

## 使用编译器
gcc 13.2.1

## C++标准是多少?
C++20

## C++用了哪些库?

<pre><code>- jsoncpp
- trantor
- yaml-cpp
- spdlog
- Drogon
- nlohmann_json</code></pre>

※使用了PkgConfig

## 怎么编译

<pre><code>$ mkdir build && cd build
$ cmake ..
$ make
</code></pre>

项目最终会生成到<b>bin</b>目录

<h2>平台</h2>
<ui>
<li>Linux</li>
</ui>