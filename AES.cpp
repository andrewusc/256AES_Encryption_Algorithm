#include <tchar.h>
#include <stdio.h>
#include "memory.h"
#include <iostream>
#include <string>
#include <algorithm>
#include "iterator"
#include <Windows.h>
using namespace std;
//sbox初始化矩阵
const unsigned char Sbox[16][16] =
{
      {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
      {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
      {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
      {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
      {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
      {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
      {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
      {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
      {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
      {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
      {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
      {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
      {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
      {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
      {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
      {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}
};

//sBOX的逆矩阵
const unsigned char iSbox[16][16] =    // populate the iSbox matrix
{
   
      {0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb},
      {0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb},
      {0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e},
      {0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25},
      {0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92},
      {0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84},
      {0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06},
      {0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b},
      {0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73},
      {0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e},
      {0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b},
      {0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4},
      {0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f},
      {0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef},
      {0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61},
      {0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d}
};


const unsigned char Rcon[9]= {0x00,0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};//定义轮常量,这里定义了8个，实际上只用到了7个 
bool progress=true;   //true为加密过程，false为解密过程 为函数复用提供了可能 
int clearColumns = 4;      //明文矩阵列数 
int keyColumns = 8 ;       //密钥矩阵列数 
int roundCount = 14;       //加密轮数 
unsigned char *key;         
unsigned char input[16];   //128位明文 
unsigned char original_key[32];//原始256位密钥

typedef struct ByteS
{
    unsigned char keyList[4];
} ByteColumns;
ByteColumns *keyList;
LPBYTE State[4];



//循环左移一个字节
unsigned char *ShiftWord(unsigned char *hexnum )
{
    unsigned temp=hexnum[0];
    for(int i=0; i<3; i++)
    {
        hexnum[i]=hexnum[(i+1)%4];
    }
    hexnum[3]=temp;
    return hexnum;
}

//字符映射 
unsigned char *Map( unsigned char *hexnum )
{
    unsigned char *result = new unsigned char[4];
    //右移4位，获得高4位，与0x0f进行与操作，获得低4位
    result[0] = Sbox[ hexnum[0] >> 4][ hexnum[0] & 0x0f ];
    result[1] = Sbox[ hexnum[1] >> 4][ hexnum[1] & 0x0f ];
    result[2] = Sbox[ hexnum[2] >> 4][ hexnum[2] & 0x0f ];
    result[3] = Sbox[ hexnum[3] >> 4][ hexnum[3] & 0x0f ];
    delete[] hexnum;
    return result;
}


//每调用函数一次做一轮亦或操作，共14轮 
void AddRoundKey( int round )
{
    for (int r = 0; r < 4; ++r)   //
    {
        for (int c = 0; c < 4; ++c)
        {
            State[r][c] = (unsigned char) ( (int)State[r][c] ^ (int)keyList[(round*4)+c].keyList[r] );
        }
    }
    if(progress)
    {
       if(round!=0)
       {
            for(int i=0; i<4; i++)
            {
                for(int j=0; j<4; j++)
               {
                   int temp=State[j][i];
                   if(temp<16)
                       cout<<'0';
                   cout<<hex<<temp<<' ';
               }
            }
            cout<<endl;
         }
    }
    else{
    	
    	for(int i=0;i<4;i++)
    	{
    		for(int j=0;j<4;j++)
    		{
    			int temp=State[j][i];
    			if(temp<16)
    				cout<<'0';
    			cout<<hex<<temp<<' ';
    		}
    	}
    	cout<<endl;
    }
}


//字节替换，用sBox的字节替换明文
void SubBytes()
{
    for (int r = 0; r < 4; ++r)
    {
        for (int c = 0; c < 4; ++c)
        {
            State[r][c] = Sbox[ (State[r][c] >> 4)][ (State[r][c] & 0x0f) ];
        }
    }
}

//逆字节替换，用isBox的字节替换明文
void AdSubBytes()
{
    for (int r = 0; r < 4; ++r)
    {
        for (int c = 0; c < 4; ++c)
        {
            State[r][c] = iSbox[ (State[r][c] >> 4)][ (State[r][c] & 0x0f) ];
        }
    }
}


//行变换
void ShiftRows()
{
    unsigned char temp[4][4];
    for (int r = 0; r < 4; ++r)
    {
        for (int c = 0; c < 4; ++c)
        {
            temp[r][c] = State[r][c];
        }
    }

    for (int r = 1; r < 4; ++r)
    {
        for (int c = 0; c < 4; ++c)
        {
            State[r][c] = temp[ r][ (c + r) % clearColumns ];
        }
    }
}

//逆行变换
void AdShiftRows()
{
    unsigned char temp[4][4];
    for (int r = 0; r < 4; ++r)  // copy State into temp[]
    {
        for (int c = 0; c < 4; ++c)
        {
            temp[r][c] = State[r][c];
        }
    }
    for (int r = 1; r < 4; ++r)  // shift temp into State
    {
        for (int c = 0; c < 4; ++c)
        {
            State[r][ (c + r) % clearColumns ] = temp[r][c];
        }
    }
}


//---------------定义16进制的乘法运算---------------
static unsigned char Mul01(unsigned char h)
{
    return h;
}
static unsigned char Mul02(unsigned char h)
{
    if (h < 0x80)
        return (unsigned char)(int)(h <<1);
    else
        return (unsigned char)( (int)(h << 1) ^ (int)(0x1b) );
}
static unsigned char Mul03(unsigned char h)
{
    return (unsigned char) ( (int)Mul02(h) ^ (int)h );
}
static unsigned char Mul09(unsigned char h)
{
    return (unsigned char)( (int)Mul02(Mul02(Mul02(h))) ^
                            (int)h );
}
static unsigned char Mul0b(unsigned char h)
{
    return (unsigned char)( (int)Mul02(Mul02(Mul02(h))) ^
                            (int)Mul02(h) ^
                            (int)h );
}

static unsigned char Mul0d(unsigned char h)
{
    return (unsigned char)( (int)Mul02(Mul02(Mul02(h))) ^
                            (int)Mul02(Mul02(h)) ^
                            (int)(h) );
}

static unsigned char Mul0e(unsigned char h)
{
    return (unsigned char)( (int)Mul02(Mul02(Mul02(h))) ^
                            (int)Mul02(Mul02(h)) ^
                            (int)Mul02(h) );
}
//---------------定义16进制的乘法运算---------------




//混淆列变换
void MixColumns()
{
    unsigned char temp[4][4];
    for (int r = 0; r < 4; ++r)  // copy State into temp[]
    {
        for (int c = 0; c < 4; ++c)
        {
            temp[r][c] = State[r][c];
        }
    }

    for (int c = 0; c < 4; ++c)
    {
        State[0][c] = (unsigned char) ( (int)Mul02(temp[0][c]) ^ (int)Mul03(temp[1][c]) ^
                                        (int)Mul01(temp[2][c]) ^ (int)Mul01(temp[3][c]) );
        State[1][c] = (unsigned char) ( (int)Mul01(temp[0][c]) ^ (int)Mul02(temp[1][c]) ^
                                        (int)Mul03(temp[2][c]) ^ (int)Mul01(temp[3][c]) );
        State[2][c] = (unsigned char) ( (int)Mul01(temp[0][c]) ^ (int)Mul01(temp[1][c]) ^
                                        (int)Mul02(temp[2][c]) ^ (int)Mul03(temp[3][c]) );
        State[3][c] = (unsigned char) ( (int)Mul03(temp[0][c]) ^ (int)Mul01(temp[1][c]) ^
                                        (int)Mul01(temp[2][c]) ^ (int)Mul02(temp[3][c]) );
    }
}

//逆混淆列变换
void decMixColumns()
{
    unsigned char temp[4][4];
    for (int r = 0; r < 4; ++r)  // copy State into temp[]
    {
        for (int c = 0; c < 4; ++c)
        {
            temp[r][c] = State[r][c];
        }
    }

    for (int c = 0; c < 4; ++c)
    {
        State[0][c] = (unsigned char) ( (int)Mul0e(temp[0][c]) ^ (int)Mul0b(temp[1][c]) ^
                                        (int)Mul0d(temp[2][c]) ^ (int)Mul09(temp[3][c]) );
        State[1][c] = (unsigned char) ( (int)Mul09(temp[0][c]) ^ (int)Mul0e(temp[1][c]) ^
                                        (int)Mul0b(temp[2][c]) ^ (int)Mul0d(temp[3][c]) );
        State[2][c] = (unsigned char) ( (int)Mul0d(temp[0][c]) ^ (int)Mul09(temp[1][c]) ^
                                        (int)Mul0e(temp[2][c]) ^ (int)Mul0b(temp[3][c]) );
        State[3][c] = (unsigned char) ( (int)Mul0b(temp[0][c]) ^ (int)Mul0d(temp[1][c]) ^
                                        (int)Mul09(temp[2][c]) ^ (int)Mul0e(temp[3][c]) );
    }
}

//密钥扩展函数
void KeyExpansion(int textColumns, int keyColumns, int roundCount ){      
    unsigned char *temp = new unsigned char[4];
    for( int row = keyColumns; row < textColumns *(roundCount + 1); ++ row )
    {
        //存储当前列的上一列内容，便于后面的异或操作
        temp[0] = keyList[row-1].keyList[0];
        temp[1] = keyList[row-1].keyList[1];
        temp[2] = keyList[row-1].keyList[2];
        temp[3] = keyList[row-1].keyList[3];
        if (row % keyColumns == 0)
        {
            //如果为第0列，则分别进行，循环左移，Sbox映射，以及与轮常量异或操作，此时temp的存储的是上一轮，最后一列的数值
            temp = Map(ShiftWord(temp));
            temp[0] = (unsigned char)( (int)temp[0] ^ (int)Rcon[row/keyColumns]);
            temp[1] = (unsigned char)( (int)temp[1]);
            temp[2] = (unsigned char)( (int)temp[2] );
            temp[3] = (unsigned char)( (int)temp[3] );
        }
        else if ( keyColumns > 6 && (row % keyColumns == 4) )
        {
            //如果是第0列，但是不能整除8，则只需要进行Sbox映射即可
            temp = Map(temp);
        }
        //其余行列，则是上一轮当前列的值，与temp中存储的值进行异或
        keyList[row].keyList[0] = (unsigned char) ( (int)keyList[row-keyColumns].keyList[0] ^ (int)temp[0] );
        keyList[row].keyList[1] = (unsigned char) ( (int)keyList[row-keyColumns].keyList[1] ^ (int)temp[1] );
        keyList[row].keyList[2] = (unsigned char) ( (int)keyList[row-keyColumns].keyList[2] ^ (int)temp[2] );
        keyList[row].keyList[3] = (unsigned char) ( (int)keyList[row-keyColumns].keyList[3] ^ (int)temp[3] );
    }
}

//加密函数 
void Cipher( unsigned char *tempkey,unsigned char *input, unsigned char *result_cipher )   
{
    //根据用户输入，为密钥分配并初始化其内容
    key = new unsigned char[keyColumns * 4];
    memcpy( key, tempkey, keyColumns * 4 );
    keyList = new ByteColumns[clearColumns * (roundCount+1)];
    //初始化，前8列的轮转密钥序列(由用户指定)
    for( int row = 0; row < keyColumns; ++row )
    {
        keyList[row].keyList[0] = key[4*row];
        keyList[row].keyList[1] = key[4*row+1];
        keyList[row].keyList[2] = key[4*row+2];
        keyList[row].keyList[3] = key[4*row+3];
    }
   

   KeyExpansion(clearColumns, keyColumns,roundCount );   //密钥扩展 


    for(int i=0; i<4; i++)
    {
        State[i] = NULL;
    }
    if (State[0] == NULL)
    {
        for(int i=0; i<4; i++)
        {
            State[i] = new unsigned char[clearColumns];
        }
    }
    for (int i = 0; i < (4 * clearColumns); ++i)
    {
        State[i%4][i/4] = input[i];
    }
    AddRoundKey(0);
    
    // 加密轮数循环（13轮）
    for (int round = 1; round <= (roundCount - 1); ++round)
    {
        SubBytes();
        ShiftRows();
        MixColumns();
        AddRoundKey(round);
    }

    SubBytes();
    ShiftRows();
    cout<<"加密结果为： "<<endl; 
    AddRoundKey(14);     //最后一轮，即第十四轮 

    for ( int i = 0; i < (4 * clearColumns); ++i)
    {
        result_cipher[i] = State[i % 4][ i / 4];
    }
}

//解密函数 
void DeCipher( unsigned char *input, unsigned char *result_decipher )
{
    progress=false;
    if (State[0] == NULL)
    {
        for(int i=0; i<4; i++)
        {
            State[i] = new unsigned char[clearColumns];
        }
    }
    int i;
    for (i = 0; i < (4 * clearColumns); ++i)
    {
        State[i % 4][ i / 4] = input[i];
    }

    AddRoundKey(roundCount);

    for (int round = roundCount-1; round >= 1; --round)
    {
        AdShiftRows();
        AdSubBytes();
        AddRoundKey(round);
        decMixColumns();
    }

    AdShiftRows();
    AdSubBytes();
    cout<<"解密结果为： "<<endl; 
    AddRoundKey(0);
    //将解密结果输出到output数组中
    for (i = 0; i < (4 * clearColumns); ++i)
    {
        result_decipher[i] = State[i % 4][ i / 4];
    }
}

//输入控制函数 
void Input(int task){
     if(task == 1){
                  cout<<"请输入明文:";
                  for(int k=0; k<16; k++)
                  {
                    char a,b;
                       cin>>a>>b;
                       int a1,b1;
                       if(a-'0'>9)
                             a1=a-'a'+10;
                       if(a-'0'<=9)
                             a1=a-'0';
                       if(b-'0'>9)
                             b1=b-'a'+10;
                       if(b-'0'<=9)
                             b1=b-'0';
                      input[k]=a1*16+b1;
                  }
     }
     
     if(task == 2){
                 cout<<"请输入密钥:";
                 for(int k=0; k<32; k++)
                 {
                    char a,b;
                    cin>>a>>b;
                    int a1,b1;
                    if(a-'0'>9)
                               a1=a-'a'+10;
                    if(a-'0'<=9)
                                a1=a-'0';
                    if(b-'0'>9)
                               b1=b-'a'+10;
                    if(b-'0'<=9)
                                b1=b-'0';
                    original_key[k]=a1*16+b1;
                  }
     }
}



int main()
{
    int test = 1 ;
    cout<<"---------------AES加密算法---------------"<<endl;
    while(test){
                  Input( 1 ) ;  //输入明文 
                  Input( 2 ) ;  //输入密钥 
                  unsigned char result[17];
                  cout<<"---------------将输入明文加密---------------"<<endl;

                  Cipher(original_key,input,result);
                  cout<<endl<<"---------------将加密结果解密---------------"<<endl;
                  //Input( 3 ) ;    //输入密文 
                  cout<<"解密过程如下所示："<<endl;
                  DeCipher(result, result); 
                  system("pause");
                  cout<<"(0)退出  (1)继续测试:  "; 
                  cin>>test;
     }
                  return 0;
}
