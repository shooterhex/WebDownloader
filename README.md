# WedDownload

## 项目描述 ##
神经网络是目前热门的研究领域，大部分神经网络的实现较为抽象，难以清楚知道神经网络结构以及中间计算过程。因此我们希望通过对神经网络进行可视化处理，让用户能够直观地了解到所构建的神经网络结构以及中间计算过程，使其能够直观方便地调整神经网络结构与参数。

## 团队描述 ##
小组名称：web内容下载

小组成员：李智，郑无恙，田文杰

选题：简单web内容下载
### 项目管理 ###

版本控制：git, github

开发环境：QT 5.14.2

编程语言：C++17

框架结构：MVVM

文档编写：markdown

### 项目提出 ###


### 功能实现 ###

#### 内容下载

· 输入URL，保存目录

· 选择文件类型

· 下载

#### 下载进度显示

· 显示任务队列

· 在状态栏中显示当前任务相关状态


#### 界面与功能展示

主界面

此处应插入图片

## 具体实现

### 框架描述 ###

框架整体采用MVVM模式，将view层与model层分离，通过view model连接，实现多人协作。

### 实现过程 ###

第一轮迭代：做出主窗口，实现url，目录的输入，点击按钮，获取html内容，保存到内部变量中

第二轮迭代：增加保存文件类型选择（html/文字/图片），完成实际文件保存

第三轮迭代：实现多页面连续下载。

第四轮迭代：实现状态栏显示当前任务状态

第五轮迭代：增加任务窗口，显示任务列表

第六轮迭代：实现任务排队，多任务下载

### 计算实现 ###

根据图结构对神经元进行拓扑排序，再顺序计算各神经元上的值。

## 项目分工 ##

View：田文杰

Model：郑无恙

ViewModel : 李智