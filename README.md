# TinyPhotoshop核心模块解析

## 简介
此次的开发目标实现一个简易的图像编辑器，支持多种格式的图像的**保存**，**读取**，**修改**。支持**图层**,支持类似于`.psd`的工程文件的保存及打开。主要开发框架为QT，openGL。主要分为4个模块：
* 前端界面：基于QT绘制的GUI界面
* 图像编辑区：调用opengl API生成的对图像数据进行显示和修改的主界面
* 核心数据结构：定义了点(`BasicPoint`)、图层(`BasicLayer`)、图像(`BasicImage`)的多种数据结构用于实现图层，图像变换（包括旋转、镜面、裁剪、图层混合、滤镜等）
* 图像读取及保存模块：支持多种图像格式（bmp、png、jpg、自定义结构）的读取、保存  


## 项目亮点
* 文件的读取和保存以及图像编辑未使用第三方图像库、根据图像类型解析
* 支持工程文件的打开和保存、用一个哈希表存储项目中生成的对象的名称及地址，将对象及其状态按自定义格式进行存储。重新读取后可以恢复工程现场。
* 自定义图形库实现了带透明度的图层功能、支持图层的增删查改、支持图层命名及图层融合、支持高效的图形变换、支持图层的选取


## 变换模块简介
### 文件结构
```txt
basicDS.h
basicDS.cpp
utils.h
utils.cpp
```
`basicDS.h`中声明了三个类，`BasicPoint`，`BasicLayer`以及`BasicImage`，为该项目核心数据结构。`utils.h`为辅助函数及错误处理函数。
#### RS::BasicPoint
```cpp
class BasicPoint
{
protected:
    uint8_t red,green,blue,alpha;
public:
    BasicPoint(){red=0;green=0;blue=0;alpha=0;}
    BasicPoint(const uint8_t red, const uint8_t green, const uint8_t blue,
        const uint8_t alpha=0):red(red),green(green),blue(blue),alpha(alpha){}
    BasicPoint(const uint32_t data);//use a uint32 to initial
    virtual ~BasicPoint(){};
    virtual void reInit();
    virtual void reInit(const uint32_t data);
    virtual void display(); 
    ...
};
```
用于表示32位真彩色图的像素点。成员变量red、green、blue和alpha表示图像的四个通道，取值为0-255。默认初始值设置为`{0,0,0,0}`。
#### RS::BasicLayer
```cpp
class BasicLayer
{
protected:
    pointMatrix datamatrix;
    std::string name;
    uint16_t layerNumber;
    bool Valid;
public:
    ...

    virtual bool rightRotate();
    virtual bool leftRotate();
    virtual bool upDownReverse();
    virtual bool leftRightReverse();
    ...

    virtual bool squareRotate();
    virtual bool rectangleRotate();

    ...
    
    BasicLayer& operator=(const BasicLayer& layer);
};
```
该类为基本的图层对象，数据以二维数组形式存在于`datamatrix`中，每个图层有一个`name`属性用来表示名字，还会有一位`Valid`表示该图层是否有效。  
其成员函数包括了裁剪、旋转、翻转。在接受到前端信号之后会触发函数改变datamatrix。
#### RS::BasicImage
```cpp
class BasicImage
{
protected:
    std::vector<RS::BasicLayer> layers;
    std::string name;
    std::unordered_map<std::string,uint16_t> nameToIndex;
    uint16_t validLayer;
    uint16_t totalLayer;
    uint16_t current;
    bool indexOK(const uint16_t index){return index>=0&&index<layers.size();}
public:
    ...

    virtual const std::string& getImageName() const;
    virtual RS::BasicLayer& getLayer(const std::string& name);
    virtual RS::BasicLayer& getLayer(const uint16_t index);
    virtual RS::BasicLayer& getCurrentLayer();

    virtual bool insert(const RS::BasicLayer& aLayer);
    virtual bool insert(const std::string& name,const dataBuffer& buffer);
    virtual bool remove(const uint16_t index);
    virtual bool remove(const std::string& name);
    virtual bool duplicate(const uint16_t index);
    virtual bool duplicate(const std::string& name);
    virtual bool swap(const uint16_t index1,const uint16_t index2);
    virtual bool swap(const std::string& name1,const std::string& name2);
    virtual bool updateLayer(const std::string& name,const dataBuffer& buffer);
    virtual bool updateLayer(const uint16_t index,const dataBuffer& buffer);

    virtual bool taylor(const std::string& name,const std::vector<uint16_t>& array);
    virtual bool taylor(const uint16_t index,const std::vector<uint16_t>& array);

    virtual bool mergeLayer(const std::string& name1,const std::string& name2);
    virtual bool mergeLayer(const uint16_t index1,const uint16_t index2);
...
};
```
该类可被实例化为图像对象。一个图像包含多个图层，用`vector`存储，图像中还包含一个哈希表，用来保存`图像名称`和`下标`的对应关系，每个图像都有一个名字。该类支持图层的复制、删除、重命名、重生成、顺序交换、裁剪、融合等功能。
#### 用法示例
```cpp
#include "basicDS.h"
int main()
{
    //数据块初始化（用来模拟从文件中读取的数据）
    dataBuffer haha(1024,rowData(1024,0xffffffff));
    dataBuffer baba={{0x1f2f3f4f,0x22222222,0xffffffff},
                     {0x33333333,0x44444444,0x3f3f3f3f},
                     {0x11111111,0x22222222,0xffffffff},
                     {0x33333333,0x44444444,0x3f3f3f3f}};
    dataBuffer caca(2048,rowData(2048,0x3f3f3f3f));



    //初始化图层并重命名
    RS::BasicLayer l1(1024,1024);
    l1.setLayerName("doubi 1");

    RS::BasicLayer l2(2048,2048);
    l2.setLayerName("doubi 2");

    RS::BasicLayer l3(40,80);
    l3.setLayerName("doubi 3");

    RS::BasicLayer l4(baba);//用数据块初始化
    l4.setLayerName("doubi 4");

    //显示图层l4的数据信息
    l4.displayData();

    std::cout<<std::endl;

    //上下反转图层并输出该图层的数据信息
    l4.upDownReverse();
    l4.displayData();

    //左右翻转图层并测试该图层的数据信息
    l4.leftRightReverse();
    std::cout<<std::endl;
    l4.displayData();

    //右旋图层
    l4.rightRotate();
    std::cout<<std::endl;
    l4.displayData();

    std::cout<<std::endl;

    //左旋图层
    l4.leftRotate();
    l4.displayData();

    //设置该图层无效
    l4.setInvalid();

    //初始化一个空的图像对象
    RS::BasicImage img;

    //插入图层
    img.insert(l1);
    img.insert(l2);
    img.insert(l3);
    img.insert(l4);

    //显示图层的NAME-Index对应关系
    img.displayHash();

    std::cout<<std::endl;

    //用来保存用于裁剪的坐标信息，此处意思为左上角坐标（30，30），右下角坐标（60，60）
    std::vector<uint16_t> array={30,30,60,60};

    //裁剪图层“doubi 1”
    img.taylor("doubi 1",array);

    //用数据块更新图层
    img.updateLayer("doubi 4",caca);

    //复制并插入新图层
    img.duplicate("doubi 4");
    img.duplicate("doubi 3");

    //交换图层位置
    img.swap("doubi 1","doubi 2");

    //删除图层
    img.remove("doubi 1");
    img.remove("doubi 2");
    img.remove("doubi 3");
    img.remove("doubi 4");
    
    //显示整个图像的信息
    img.display();
}
```


## 展望
* 根据项目需求和个人目的进行技术选型。假设需要快速开发高效产出，选取成熟的第三方框架和类库；若想提高个人编码能力，自行查找相关算法进行实现封装。在该调试过程中来优化代码效率，减少代码的冗余，收敛出好的代码风格。
* 熟悉内存layout对于代码水平至关重要。熟悉对象在内存中存放的位置，熟悉变量的生存周期，熟悉指针对于内存的每个地址以及每个地址对应的没一位的操作，可以做到随心所欲。
* 熟悉x86计算机的小端特性，能将一个整形变量的字节读取顺序和内存位置相对应，几乎可以解决大部分的内存操作问题，准确找到自己想要的数据并修改。
* 熟悉隐式类型转换，使用强制类型转换控制过程，避免数据的丢失和错误补位。
* 熟悉const、static等关键词，做足够的参数类型检查
* 熟悉打开后的文件在内存中的layout，将文件字节码与各类型的buffer相对应。
* 增强代码的复用和封装，设置私有成员函数，减少无用接口的暴露
* 写c系代码的感觉就是和内存在打交道，如果能够映射每条语句过后内存究竟发生了什么样的变化，是否会引起系统调用等，可以有效提高程序的执行效率。

