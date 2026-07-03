# R8ASM

基于[ROT-8](https://gitea.starcells.ltd/starcells/ROT-8.git)的类汇编语言\
和用于汇编类汇编语言的汇编器

## 规范

### Unit与Cell

在R8ASM中,从[0]开始,每2个Cells将被划为一个Unit.每个Unit中前一个Cell用于存储主要数据,而后一个Cell则作为缓冲区存在.这在一定程度上解决了ROT-8缺乏寄存器的问题.\
macro参数的注释中出现Unit或UnitBit前缀时,即表示其指向的数据作为Unit的前一个参数地址被传递,这意味着该Unit的缓冲区可能(几乎一定)会被清空.
>当然往奇数Cell中存数据本来就是不推荐的行为.如果要进行不太在意性能的计算,用封装好的,更安全的宏直接计算会更合适.\
>除非你知道自己在做什么.

### stp与btp在[],%macro和%repeat内的守恒

如果stp与btp在[]内数量相等,就可以假定[]中的部分不会对dataptr造成影响,也就可以实现dataptr在编译期的静态分析.\
stp与btp的守恒使得dataptr的静态分析变得相当简单,也使anchor和tp——原本几乎不可能存在的随机访问——成为可能.\
macro和repeat内同理,并且这样宏就不会破坏调用前的上下文.
>当然对macro,最方便的肯定是在开头anchor,末尾再tp回去.

## 全局常量

| 常量 | 含义 |
| --- | --- |
| _CELL_SIZE | 每个Cell的bit数 |
| _CELL_MAX | 基于_CELL_SIZE推算出的,Cell中数据的最大值 |

## Builtins

| Builtin | 含义 |
| ------- | ---- |
| .anchor s | 创建一个全局常量s,其值为创建该常量时dataptr所在的位置 |
| .tp s | 将dataptr移动至s指定的位置 |
| .xor s | 对dataptr指向的Cell xor stoi(s) |
| .loop | 基于dataptr指向的Cell的下一个Cell中,由LSB开始的连续的1的个数进行循环(具有破坏性,将清零下一Cell中的这些1) |
| .endloop | 结束由.loop发起的循环 |

>.loop的示例:当*(dataptr++)为115(0b11010111)时,循环将进行3次.结束后该Cell将变为26(0b11010)\
>因此这种可变的循环每层最多可执行_CELL_SIZE次

## Parses

| Parse | 含义 |
| --- | --- |
| %macro s1 s2 | 创建具有stoi(s2)个参数的,名为s1的宏. |
| %endmacro | 结束对该宏的声明 |
| %repeat s | 将该段代码重复插入stoi(s)次 |
| %endrepeat | 结束重复的代码段 |
| %include s | 将s作为依赖引入 |
| %var s1 s2 | 创建值为stoi(s2)的,名为s1的变量 |

> 在macro内,%0表示"当前宏的参数个数". 可以以%n的形式获取参数, n $ \in[ 1, $ stoi(s2) $ ] $

## 标准库

各个宏的具体含义与用法在对应的r8asm文件中基本都有提及,此处不做过多介绍.\
命名上的规则如下:

| 前缀 | 含义 |
| --- | --- |
| bit_unsafe | 最基本的层级,对指定Cell的LSB进行单bit运算且具有破坏性(会清除参数的LSB) |
| bit | 安全的宏,对指定的Unit中,前一个Cell的LSB进行单bit运算,但不会破坏其LSB |
| unsafe | 对指定的整个Cell进行运算,具有破坏性(会清除该Cell) |
| 无前缀 | 安全的宏,对指定Unit中的前一个Cell进行运算,不会破坏该Cell |

>正如规范中提到的,即使安全宏不会破坏前一个Cell,也会使后一个Cell被清空以作为寄存器.\
>这是因为安全宏的具体实现就是简单粗暴地把数据拷贝进不同Unit的"后一个Cell"进行计算.\
>这一点在and.r8asm中体现得尤其明显.

## 流程

read_src(r8asm文件) => R8Src\
r8asm_preprocess(R8Src) => macros\
expand_macros(macros) => builtin\
r8asm_tape_out(builtin) => bytecode\
ROT8_BC_TO_CHAR(bytecode) => tape

## 示例

可以用add宏计像这样计算255+255+1(在 _CELL_SIZE = 8时,结果为 Co = 1, S = 255)并验证其安全性:

```nasm
%include asm/add.r8asm

%var A 0
%var B 2
%var Ci 4
%var S 6
%var Co 8

.tp A
.xor 255
.tp B
.xor 255
.tp Ci
.xor 1

add A B Ci S Co

.tp A
out
.tp B
out
.tp Ci
out
.tp S
out
.tp Co
out
```

>r8asm的语法很大程度上参考了nasm,因此这里用nasm的Markdown代码块蹭一下语法高亮

## 构建

```sh
mkdir build
cd build
cmake ..
make -j$(nproc)
```

## 用法

```sh
  r8asm_test asm
```

## TODO

1. 像正经汇编一样的sections结构
2. 各种边界检查/语法检查
