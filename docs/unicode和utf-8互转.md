# ASCII码
在计算机内部, 所有的信息最终都表示为一个二进制的字符串.每一个二进制位(bit)有0和1两种状态, 因此八个二进制位就可以组合出 256种状态, 这被称为一个字节(byte).

也就是说, 一个字节一共可以用来表示256种不同的状态, 每一个状态对应一个符号, 就是256个符号, 从 0000000到11111111

ASCII码一共规定了128个字符的编码, 比如空格"SPACE"是32(二进制00100000), 大写的字母A是65(二进制01000001)

这128个符号(包括32个不能打印出来的控制符号), 只占用了一个字节的后面7位, 最前面的1位统一规定为0

# 非ASCII编码
英语用128个符号编码就够了, 但是用来表示其他语言, 128个符号是不够的. 比如, 在法语中, 字母上方有注音符号, 它就无法用ASCII码表示.

于是, 一些欧洲国家就决定, 利用字节中闲置的最高位编入新的符号. 比如, 法语中的é的编码为130(二进制10000010).这样一来, 这些欧洲国家使用的编码体系, 可以表示最多256个符号

但是, 这里又出现了新的问题. 不同的国家有不同的字母, 因此, 哪怕它们都使用256个符号的编码方式, 代表的字母却不一样
- 比如, 130在法语编码中代表了é, 在希伯来语编码中却代表了字母Gimel (?), 在俄语编码中又会代表另一个符号

但是不管怎样, 所有这些编码方式中, 0-127表示的符号是一样的, 不一样的只是128-255的这一段

至于亚洲国家的文字, 使用的符号就更多了, 汉字就多达10万左右. 一个字节只能表示256种符号, 肯定是不够的, 就必须使用多个字节表达一个符号
- 比如, 简体中文常见的编码方式是GB2312, 使用两个字节表示一个汉字, 所以理论上最多可以表示256x256=65536个符号

# Unicode
## Unicode的定义
世界上存在着多种编码方式, 同一个二进制数字可以被解释成不同的符号.因此, 要想打开一个文本文件, 就必须知道它的编码方式, 否则用错误的编码方式解读, 就会出现乱码.

为什么电子邮件常常出现乱码?就是因为发信人和收信人使用的编码方式不一样.

可以想象, 如果有一种编码, 将世界上所有的符号都纳入其中. 每一个符号都给予一个独一无二的编码, 那么乱码问题就会消失. 这就是Unicode, 就像它的名字都表示的, 这是一种所有符号的编码.

Unicode也是一种字符编码方法, 不过它是由国际组织设计, 可以容纳全世界所有语言文字的编码方案. 

Unicode的学名是"Universal Multiple-Octet Coded Character Set",简称为UCS. UCS可以看作是"Unicode Character Set"的缩写.

Unicode当然是一个很大的集合, 现在的规模可以容纳100多万个符号. 每个符号的编码都不一样
- 比如, U+0639表示阿拉伯字母Ain, U+0041表示英语的大写字母A, U+4E25表示汉字"严"
- 具体的符号对应表, 可以查询unicode.org, 或者专门的汉字对应表.

## Unicode的问题
Unicode只是一个符号集, 它只规定了符号的二进制代码, 却没有规定这个二进制代码应该如何存储"

比如, 汉字"严"的unicode是十六进制数4E25, 转换成二进制数足足有15位(100111000100101), 也就是说这个符号的表示至少需要2个字节. 表示其他更大的符号,可能需要3个字节或者4个字节, 甚至更多

这里就有两个严重的问题, 第一个问题是, 如何才能区别unicode和ascii?计算机怎么知道三个字节表示一个符号, 而不是分别表示三个符号呢?
- 第二个问题是, 我们已经知道,英文字母只用一个字节表示就够了
- 如果unicode统一规定, 每个符号用三个或四个字节表示, 那么每个英文字母前都必然有二到三个字节是0, 这对于存储来说是极大的浪费,文本文件的大小会因此大出二三倍, 这是无法接受的

它们造成的结果是:
- 出现了unicode的多种存储方式, 也就是说有许多种不同的二进制格式,可以用来表示unicode
- unicode在很长一段时间内无法推广, 直到互联网的出现

## UTF-8
### 概述
互联网的普及, 强烈要求出现一种统一的编码方式.UTF-8就是在互联网上使用最广的一种unicode的实现方式. 其他实现方式还包括UTF-16和UTF-32, 不过在互联网上基本不用.重复一遍, 这里的关系是, UTF-8是Unicode的实现方式之一

UTF-8最大的一个特点, 就是它是一种变长的编码方式. 它可以使用1~6个字节表示一个符号, 根据不同的符号而变化字节长度.

### UTF-8的编码规则
UTF-8的编码规则很简单, 只有两条:
- 对于单字节的符号, 字节的第一位设为0, 后面7位为这个符号的unicode码. 因此对于英语字母, UTF-8编码和ASCII码是相同的.
- 对于n字节的符号(n>1), 第一个字节的前n位都设为1, 第n+1位设为0, 后面字节的前两位一律设为10. 剩下的没有提及的二进制位, 全部为这个符号的unicode码.

```
| Unicode 符号范围(16进制)| UTF-8编码方式(2进制)
| -----------------------|--------------------------------------------------------------
| 0000 0000  - 0000 007F |                                                    0xxx xxxx
| 0000 0000  - 0000 07FF |                                          110x xxxx 10xx xxxx
| 0000 0000  - 0000 FFFF |                                1110 xxxx 10xx xxxx 10xx xxxx
| 0001 0000  - 0010 FFFF |                      1111 0xxx 10xx xxxx 10xx xxxx 10xx xxxx
| 0020 0000  - 03FF FFFF |            1111 10xx 10xx xxxx 10xx xxxx 10xx xxxx 10xx xxxx
| 0400 0000  - 7FFF FFFF |  1111 110x 10xx xxxx 10xx xxxx 10xx xxxx 10xx xxxx 10xx xxxx    
```

例子：以汉字"严"为例, 演示如何实现UTF-8编码
- 已知"严"的unicode是4E25(1001110 00100101), 根据上表, 可以发现4E25处在第三行的范围内(0000 0800 - 0000 FFFF), 因此"严"的UTF-8编码需要三个字节, 即格式是
"1110xxxx 10xxxxxx 10xxxxxx".
- 然后, 从"严"的最后一个二进制位开始, 依次从后向前填入格式中的x, 多出的位补0.
- 这样就得到了, "严"的UTF-8编码是 "11100100 10111000 10100101", 转换成十六进制就是E4B8A5.

# Little endian 和 Big endian
## 概述
上面提到，Unicode码可以采用UCS-2格式直接存储
- 以汉字"严"为例, Unicode码是4E25，需要用两个字节存储, 一个字节是4E, 另一个字节是25
- 存储的时候, 4E在前,25在后, 就是Big endian方式; 25在前, 4E在后, 就是Little endian方式.

因此, 第一个字节在前, 就是"大头方式"(Big endian), 第二个字节在前就是"小头方式"(Little endian).

## 计算机怎么知道某一个文件到底采用哪一种方式编码?(零宽度非换行空格(FEFF))
Unicode规范中定义, 每一个文件的最前面分别加入一个表示编码顺序的字符, 这个字符的名字叫做"零宽度非换行空格"(ZERO WIDTH NO-BREAK SPACE), 用FEFF表示. 

这正好是两个字节, 而且FF比FE大1.

## Unicode与UTF-8之间的转换
### 将一个字符的Unicode编码转换成UTF-8编码.
```
template <class Uint16ContainerConIter>
void static unicodeToUtf8(Uint16ContainerConIter begin, Uint16ContainerConIter end, std::string& res)
{
    res.clear();
    uint16_t ui;

    while (begin != end) {
        ui = *begin;

        if (ui <= 0x7f) {
            // U-00000000 - U-0000007F:  0xxxxxxx
            res += char(ui);
        } else if (ui <= 0x7ff) {
            // U-00000080 - U-000007FF:  110xxxxx 10xxxxxx
            res += char(((ui >> 6) & 0x1f) | 0xc0);
            res += char((ui & 0x3f) | 0x80);
        } else {
            //  * U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            res += char(((ui >> 12) & 0x0f) | 0xe0);
            res += char(((ui >> 6) & 0x3f) | 0x80);
            res += char((ui & 0x3f) | 0x80);
        }

        begin++;
    }
}
```
### 将一个字符的UTF8编码转换成Unicode编码
```
template <class Uint16Container>
bool static Utf8ToUnicode(const char* const str, size_t len, Uint16Container& vec)
{
    if (!str) {
        return false;
    }
    char ch1, ch2;
    uint16_t tmp;
    vec.clear();

    for (size_t i = 0; i < len;) {
        if (!(str[i] & 0x80)) {     // 0xxxxxxx
            vec.push_back((str[i] & 0x7f));
            i++;
        } else if ((uint8_t)str[i] <= 0xdf && i + 1 < len) {    // 110xxxxxx
            ch1 = (str[i] >> 2) & 0x07;
            ch2 = (str[i + 1] & 0x3f) | ((str[i] & 0x03) << 6);
            tmp = (((uint16_t(ch1) & 0x00ff) << 8) | (uint16_t(ch2) & 0x00ff));
            vec.push_back(tmp);
            i += 2;
        } else if ((uint8_t)str[i] <= 0xef && i + 2 < len) {
            // 方法一
            // ch1 = ((uint8_t)str[i] << 4) | ((str[i + 1] >> 2) & 0x0f );
            // ch2 = (((uint8_t)str[i + 1] << 6) & 0xc0) | (str[i+2] & 0x3f);
            // tmp = (((uint16_t(ch1) & 0x00ff ) << 8) | (uint16_t(ch2) & 0x00ff));
            
            // 方法二
            tmp = (uint8_t)(str[i]) & 0x0f;

            tmp <<= 6;
            tmp |= (uint8_t)(str[i + 1]) & 0x3f;

            tmp <<= 6;
            tmp |= (uint8_t)(str[i + 2]) & 0x3f;

            vec.push_back(tmp);
            i += 3;
        } else {
            return false;
        }
    }

    return true;
}
```