//程序详细说明位于同文件夹下.log文件内

/*
 * 程序中各个设备使用的针脚预设为：
 * 液晶面板按照接口接入SDA-Buchse和SCL接口 同时提供5V供电和接地
 * 扬声器 PIN 11
 * 摩尔斯电码读入 点 PIN 4
 * 摩尔斯电码读入 横 PIN 5
 * 摩尔斯电码读入 断点（分割字母） PIN 6
 * 摩尔斯电码读入 空格 PIN 7
 * 可以在#define命令下更改
*/

#include<Wire.h>
#include<LiquidCrystal_I2C.h>

#ifdef __AVR__
#include<avr/power.h>
#endif
//检查Atmel AVR系微处理器的兼容性

#define LC_I2C_Address 0x3f
//数据接收地址可为0x27 0x3f 0x20
#define LC_I2C_HorizontalLength 16
#define LC_I2C_VerticalLength 2
//定义液晶面板的数据接收地址 以及长宽各有多少显示模块（需按照液晶面板数据调整）
LiquidCrystal_I2C Lcd( LC_I2C_Address , LC_I2C_HorizontalLength , LC_I2C_VerticalLength );
//声明液晶显示面板的大小以及数据接收地址

#define Tone_PIN 11
#define MorseCode_Input_ShortMark_PIN 4
#define MorseCode_Input_LongMark_PIN 5
#define MorseCode_Input_AlphabetBreakDown_PIN 6
#define MorseCode_Input_Space_PIN 7
//定义输出控制接口

#define MorseCode_ShortMark_Length 200
#define MorseCode_LongMark_Length 600
//定义摩尔斯电码点与横的持续时长
//程序预设定为200ms和600ms

#define Key_PressingDown LOW
//预设定按钮按下状态时的状态值为0

struct MorseCode_BinaryTree{
    char Root;
    MorseCode_BinaryTree *Lbranch;
    MorseCode_BinaryTree *Rbranch;
};
//构建二叉树的结构体 用于存放摩尔斯密码的密码表

/*
char MorseCode_Pre_Set[31] = { '$' , 'E' , 'I' , 'S' , 'H' , 'V' , 'U' , 'F' , '/' , 'A' , 'R' , 'L' , '/' , 'W' , 'P' , 'J' , 'T' , 'N' , 'D' , 'B' , 'X' , 'K' , 'C' , 'Y' , 'M' , 'G' , 'Z' , 'Q' , 'O' , '/' , '/' };
char MorseCode_Pre_Set[100] = { ' ' , '$' , 'E' , 'I' , 'S' , 'H' , '/' , '/' , 'V' , '/' , '/' , 'U' , 'F' , '/' , '/' , '/' , 'A' , 'R' , 'L' , '/' , '/' , '/' , 'W' , 'P' , '/' , '/' , 'J' , '/' , '/' , 'T' , 'N' , 'D' , 'B' , '/' , '/' , 'X' , '/' , '/' , 'K' , 'C' , '/' , '/' , 'Y' , '/' , 'M' , 'G' , '/' , '/' , 'Q' , '/' , '/' , 'O' , '/' , '/' };
*/

char MorseCode_Pre_Set[100] = { ' ' , '$' , 'E' , 'I' , 'S' , 'H' , '/' , '/' , 'V' , '/' , '/' , 'U' , 'F' , '/' , '/' , '/' , 'A' , 'R' , 'L' , '/' , '/' , '/' , 'W' , 'P' , '/' , '/' , 'J' , '/' , '/' , 'T' , 'N' , 'D' , 'B' , '/' , '/' , 'X' , '/' , '/' , 'K' , 'C' , '/' , '/' , 'Y' , '/' , '/' , 'M' , 'G' , 'Z' , '/' , '/' , 'Q' , '/' , '/' , 'O' , '/' , '/' };
//摩尔斯密码表的二叉树的前根遍历 用于在二叉树写入函数BinaryTree_Create中对根节点和所有叶结点赋值

int Pre_Set_Counter = 0;
//二叉树写入位数计数器

void BinaryTree_Create( MorseCode_BinaryTree *tree ){
    Pre_Set_Counter++;
    //写入位数计数器自加1
    tree -> Root = MorseCode_Pre_Set[Pre_Set_Counter];
    if ( tree -> Root == '/' )
    {
        return;
        //判断是否为空节点 是则返回父节点
    }
    else
    {
        tree -> Lbranch = new MorseCode_BinaryTree;
        tree -> Rbranch = new MorseCode_BinaryTree;
        //新建该叶结点的左右子节点的结构体 设置该叶结点的左右子树指针的指向地址
        BinaryTree_Create( tree -> Lbranch );
        //处理左子节点并递归处理左子节点的左右子树
        BinaryTree_Create( tree -> Rbranch );
        //处理右子节点并递归处理右子节点的左右子树
    }
}
//二叉树构建函数 用于将摩尔斯密码的密码表存入二叉树

MorseCode_BinaryTree *MorseCode_MainStorage = new MorseCode_BinaryTree;
//摩尔斯密码密码表根节点

char MorseCode_ReadIn_Data[100];
//摩尔斯电码读入记录器

#define MorseCode_SoundFrequency 650
//定义摩尔斯电码按下时 播放声音的频率 预设定为650Hz

/*
#define AlphabetBreakDown_SoundFrequency 800
#define Space_SoundFrequency 725
*/

void MorseCode_ShortMark_Playback(){
    tone( Tone_PIN , MorseCode_SoundFrequency );
    delay( MorseCode_ShortMark_Length );
    noTone( Tone_PIN );
    return;
}
//扬声器播放摩尔斯电码点的声音

void MorseCode_LongMark_Playback(){
    tone( Tone_PIN , MorseCode_SoundFrequency );
    delay( MorseCode_LongMark_Length );
    noTone( Tone_PIN );
    return;
}
//扬声器播放摩尔斯电码横的声音

long SearchEngine_Counter;
//解码器内核搜索深度记录器

char Decode_Path_SearchEngine( MorseCode_BinaryTree *tree ){
    if ( SearchEngine_Counter == strlen( MorseCode_ReadIn_Data ) )
    {
        return tree -> Root;
        //搜索深度等于读入电码长度 搜索完毕 返回该叶结点的数据
    }
    if ( MorseCode_ReadIn_Data[SearchEngine_Counter] == '.' )
    {
        SearchEngine_Counter++;
        return Decode_Path_SearchEngine( tree -> Lbranch );
        //读入电码下一位为点 继续搜索左子树 深度+1
    }
    if ( MorseCode_ReadIn_Data[SearchEngine_Counter] == '-' )
    {
        SearchEngine_Counter++;
        return Decode_Path_SearchEngine( tree -> Rbranch );
        //读入电码下一位为横 继续搜索右子树 深度+1
    }
}
//解码器内核 负责按照读入的电码寻找具体字母 搜索方式类似于深度优先搜索（DFS）

int EndLine_Counter;
//换行计数器 记录在一行内具体输出了多少个字母

void MorseCode_Decoder(){
    if ( strlen( MorseCode_ReadIn_Data ) == 0 )
    {
        Serial.println( "You haven't given any Morse Code yet" );
        return;
    }
    //判定是否有读入数据
    if ( strlen( MorseCode_ReadIn_Data ) > 4 )
    {
        Serial.println( "This code is too long, please press the Break key after every letters" );
        return;
    }
    //判读读入电码长度是否越界
    SearchEngine_Counter = 0;
    //搜索深度计数器归零
    char SearchResult = Decode_Path_SearchEngine( MorseCode_MainStorage );
    //字符变量SearchResult用于记录解码结果
    if ( SearchResult == '/' )
    {
        Serial.println( "Wrong Morse Code, this Code cannot be found in the Codelist" );
        return;
    }
    //判定读入电码是否合法
    Lcd.print( SearchResult );
    EndLine_Counter++;
    //只有在有输出数据的时候才要将换行计数器+1
    return;
}

void setup(){
    Lcd.init();
    Lcd.clear();
    Lcd.backlight();
    Lcd.setCursor( 0 , 0 );
    //液晶面板初始化
    pinMode( MorseCode_Input_ShortMark_PIN , INPUT_PULLUP );
    pinMode( MorseCode_Input_LongMark_PIN , INPUT_PULLUP );
    pinMode( MorseCode_Input_AlphabetBreakDown_PIN , INPUT_PULLUP );
    pinMode( MorseCode_Input_Space_PIN , INPUT_PULLUP );
    //声明程序所用的PIN 4~7为输入接口 输入模式为INPUT_PULLUP
    Serial.begin( 9600 );
    //设置串口通信频率
    BinaryTree_Create( MorseCode_MainStorage );
    //写入摩尔斯密码密码表
    strcpy( MorseCode_ReadIn_Data , "" );
    //初始化电码读入记录器
    EndLine_Counter = 0;
    //换行计数器归零
}

void loop(){
    if ( digitalRead( MorseCode_Input_ShortMark_PIN ) == Key_PressingDown )
    {
        strcat( MorseCode_ReadIn_Data , "." );
        MorseCode_ShortMark_Playback();
    }
    if ( digitalRead( MorseCode_Input_LongMark_PIN ) == Key_PressingDown )
    {
        strcat( MorseCode_ReadIn_Data , "-" );
        MorseCode_LongMark_Playback();
    }
    //将读入数据以“."和“-”的形式储存进MorseCode_ReadIn_Data中
    if ( digitalRead( MorseCode_Input_AlphabetBreakDown_PIN ) == Key_PressingDown )
    {
        MorseCode_Decoder();
        //触发断点 对前面读入的电码进行解码
        //delay( 200 );
        strcpy( MorseCode_ReadIn_Data , "" );
        //重置电码读入记录器
        if ( EndLine_Counter == LC_I2C_HorizontalLength )
        {
            Lcd.setCursor( 1 , 0 );
        }
        //当光标到达液晶面板横向最右侧 则换行
        delay( 100 );
        while ( digitalRead( MorseCode_Input_AlphabetBreakDown_PIN ) == Key_PressingDown );
        //判断断点按键是否仍然处于按下状态 若仍然被按下则保持不结束if语句
        //因为主程序不断的在被循环 若不对结束断电按键判断加以限制就会出现一次按下被多次读取的问题 而这是要被避免的
        //而点横读入因为有播放音量的等待时间所以一般不用加以限制
    }
    //当断点按键被按下 触发解码器
    if ( digitalRead( MorseCode_Input_Space_PIN ) == Key_PressingDown )
    {
        Lcd.print( " " );
        //打印空格
        EndLine_Counter++;
        if ( EndLine_Counter == LC_I2C_HorizontalLength )
        {
            Lcd.setCursor( 1 , 0 );
        }
        //当光标到达液晶面板最右侧 则换行
        delay( 100 );
        while ( digitalRead( MorseCode_Input_Space_PIN ) == Key_PressingDown );
        //限制if语句的结束条件（同行数220~222）
    }
}