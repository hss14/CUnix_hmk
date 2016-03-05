#**1. 简述Android发展历史**

2003年10月，有“Android之父”之称的安迪·鲁宾在美国加州帕洛阿尔托创建了Android科技公司。2005年8月17日， Android被Google收购，全资子公司和所有Android员工都被并入Google。当时没有人知道Google为什么作出这项收购，有许多猜测，后来证明Google正是借助此次收购正式进入移动领域。

在Google，鲁宾领导着一个负责开发基于Linux内核移动操作系统的团队，这个开发项目便是Android操作系统。Google的合作平台为Android提供了广阔的市场，Google给予各大硬件制造商、软件开发商一个灵活可靠的系统升级承诺，并保证将给予它们最新版本的操作系统。

2006年12月，《华尔街日报》和BBC报导了Google有可能进入移动领域的传闻，Google有可能推广移动领域下的各种Google服务，并且将推出一款名为Google电话的自有品牌智能手机。另有传闻指出，按照Google的作风，将有可能呈现出新的智能手机制造商和运营商模式。2007年9月，Google提交了多项移动领域的专利申请。但并没有人知道，Google将会推出的是一款名为Android的智能手机操作系统。更多的猜测是，Google会推出一款像iPhone一样的自有品牌智能手机系列。

2007年11月5日，Google领导成立了开放手持设备联盟（Open Handset Alliance），目的是为了创建一个更加开放自由的移动电话环境。在开放手持设备联盟创建的同一日，联盟对外展示了他们的第一个产品：一部搭载了以Linux 2.6为核心基础的Android操作系统的智能手机。同时，一个负责持续发展Android操作系统的开源代码项目成立了AOSP（Android Open Source Project）。除了开放手持设备联盟之外，Android还拥有全球各地开发人员组成的开源社区来专门负责开发Android应用程序和第三方Android操作系统来延长和扩展Android的功能和性能。

Android操作系统使用开放免费代码许可证，一切代码为公开免费的。Google将Android的大部分以Apache开源条款2.0发布，剩下的Linux内核部分则继承GPLv2许可，AOSP包括了智能手机网络和电话协议栈等智能手机所必需的功能。Google也不断发布问卷和开放修改列表、更新情况和代码来让任何人看到并且提出他们的意见和评论，以便按照用户的要求改进Android操作系统。Android操作系统是完全免费开源的，任何厂商都不须经过Google和开放手持设备联盟的授权随意使用Android操作系统。

开源、联盟，是Android能够快速成长的关键因素。其历次版本更新情况如下：


|Andoid版本|发布日期|代号|中文代号|
|:--|:---|:---|:---|
|Android 1.1||||
|Android 1.5|2009.4.30|Cupcake|纸杯蛋糕|
|Android 1.6|2009.9.15|Donut|甜甜圈|
|Android 2.0/2.1|2009.10.26  |Eclair|松饼|
|Android 2.2|2010.5.20|Froyo|冻酸奶|
|Android 2.3|2010.12.7|Gingerbread|姜饼|
|Android 3.0/3.1/3.2&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|2011.2.2|Honeycomb|蜂巢|
|Android 4.0|2011.10.19&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|Ice Cream Sandwich&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|冰淇淋三明治|
|Android 4.1/4.2/4.3|2012.6.28|Jelly Bean|果冻豆|
|Android 4.4|2013.10.31|Kitkat|奇巧|
|Android 5.0/5.1|2014.10.16|Lollipop|棒棒糖|
|Android 6.0|2015.10.5|Marshmallow|棉花糖|



#**2. Android与其他linux系统的关系**

Android操作系统的核心属于Linux内核的一个分支，具有典型的Linux调度和功能，除此之外，Google为了能让Linux在移动设备上良好地运行，对其进行了修改和扩充。Android去除了Linux中的本地X Window System，也不支持标准的GNU库，这使得Linux平台上的应用程序移植到Android平台上变得困难。2008年，Patrick Brady于Google I/O演讲“Anatomy & Physiology of an Android”，并提出的Android HAL架构图。HAL以*.so档的形式存在，可以把Android framework与Linux kernel隔开，这种中介层的方式使得Android能在移动设备上获得更高的运行效率。这种独特的系统结构被Linux内核开发者Greg Kroah-Hartman和其他核心维护者称赞。Google还在Android的核心中加入了自己开发制作的一个名为“wakelocks”的移动设备电源管理功能，该功能用于管理移动设备的电池性能，但是该功能并没有被加入到Linux内核的主线开放和维护中，因为Linux内核维护者认为Google没有向他们展示这个功能的意图和代码。

2010年2月3日，由于Google在Android核心开发方面和Linux社区方面开发的不同步，Linux内核开发者Greg Kroah-Hartman将Android的驱动程序从Linux内核“状态树”（“staging tree”）上除去。2010年4月，Google宣布将派遣2名开发人员加入Linux内核社区，以便重返Linux内核。2010年9月，Linux内核开发者Rafael J. Wysocki添加了一个修复程序，使得Android的“wakelocks”可以轻松地与主线Linux内核合并。2011年，Linus Torvalds说：“Android的核心和Linux的核心将最终回归到一起，但可能不会是4-5年。”在Linux 3.3中大部分代码的集成完成。

