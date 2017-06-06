#ifndef GLOBALDEFINE_H
#define GLOBALDEFINE_H


#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <math.h>

using namespace std;

#define  IMG_WIDTH      (366)
#define  IMG_HEIGHT     (321)


#define  WHITE          0xFF
#define  BLACK          0x00

#define  DELTA_LEN_MAX   25
#define  DELTA_LEN_MIN   7

typedef struct {
    int  Pix;  // mark A
    int  Lin;
}snake_point;


typedef struct {

    snake_point pos;

    int  Len;
    int  DetectCount;

}snake_detectedRing;


//typedef struct {

//    snake_point PointA;
//    snake_point PointB;
//    snake_point PointC;




//}snake_capstones;



/////////////////////////宏定义////////////////////////
//#define  DEF_TRUE            1
//#define  DEF_FALSE           0
//#define  PI                  3.1415927
//#define  PI100               314.15927
//#define  PI14                ((PI)/4)      //0.78539815
//#define  PI2                 ((PI)*2)      //6.2831854
//#define  PI34                (((PI)*3)/4) //2.35619445
//#define  PI54                (((PI)*5)/4) //3.92699075
//#define  WHITE               0xff
//#define  BLACK               0x00
//#define  ANGLE_RANGE         200
//#define  DIST_MAX_HOUGH      600 //922
#define  THRE_LOW1_RING_DET  67
#define  THRE_HIGH1_RING_DET 145
#define  THRE_LOW4_RING_DET  34
#define  THRE_HIGH4_RING_DET 45
#define  MIN_LEN_RING_DET    80
#define  BELT_LEN_RING_DET   25
#define  RND_LEN_RING_DET    100
#define  BELT_MIN_RING_DET   7
#define  DETC_RING_NUM       3
////#define  RING_LEN_PIX        90
//#define  RING_LEN_LIN        240
//#define  RING_LIN_ADD        15
//#define  RADIAN_TO_ANGLE     57.29577951
//#define  ANGLE_TO_RADIAN     0.01745329252
//#define  DEF_FAIL            1
//#define  DEF_OK              0
//#define  THRE_LOW_ROT        0.01

//#define  CUL_COM_ANGLE_45    3217
//#define  CUL_COM_SIXTH       5461

//#define  DATA_LEN_DEF        11520
//#define  NUM_LEN_DEF         36
//#define  NUM_DEAL_DEF        16

//#define  PIX_LEN_CEN_DATA    180   // 720/4, 此处数据不明，可能是二维码的尺寸
//#define  LIN_LEN_CEN_DATA    248   //
//#define  PIX_LEN_HALF        ((PIX_LEN_CEN_DATA + 1)/2)
//#define  LIN_LEN_HALF        ((LIN_LEN_CEN_DATA + 1)/2)
//#define  PIX_LEN_ONE_GRID    ((PIX_LEN_CEN_DATA + 4)/8)
//#define  LIN_LEN_ONE_GRID    ((LIN_LEN_CEN_DATA + 5)/10)
//#define  PIX_LEN_HALF_GRID   ((PIX_LEN_CEN_DATA + 8)/16)
//#define  LIN_LEN_HALF_GRID   ((LIN_LEN_CEN_DATA + 10)/20)
//#define  DECODE_DATA_LEN     80
//#define  DECODE_DATA_LIN     10
//#define  DECODE_RESULT_LEN   5



/////////////////////////结构体////////////////////////
//typedef struct
//{
//    int  usValue;
//    int  usDist;
//    int   sAnglNumVert;
//    int   sAnglNumHori;
//    float	fFitPiVert;
//    float	fFitAVert;
//    float	fFitRVert;
//    float   fRadianVert;
//    float	fFitPiHori;
//    float	fFitAHori;
//    float	fFitRHori;
//    float   fRadianHori;
//}HOUGH_AREA_VALU_STR;

//typedef struct
//{
//    int  usPixStr;
//    int  usPixLen;
//    int  usLinStr;
//    int  usLinLen;
//}HOUGH_POS_STR;

//typedef struct
//{
//    int  usLenRndPix; // 水平方向长度
//    int  usLenRndLin; // 垂直方向长度
//    int  usRingLen;
//    int  usRingDetCnt;
//}DETC_RING_POS_STR;



//typedef struct {
//    int  usRingAPix;  // mark A
//    int  usRingALin;
//    int  usRingBPix;  // mark B
//    int  usRingBLin;
//    int  usRingCPix;  // mark C
//    int  usRingCLin;
//    float   lKi;
//    float   lTheta;
//    float   lRadian;
//    int  usRsv;
//}DETC_RING_ABC_STR;

//typedef struct
//{
//    int  usCen0Pix;
//    int  usCen0Lin;
//    int  usCen1Pix;
//    int  usCen1Lin;
//    int  usCen2Pix;
//    int  usCen2Lin;
//}DETC_RING_CEN_STR;

//typedef struct
//{
//    int  usRotCen0Pix;
//    int  usRotCen0Lin;
//    int  usRotCen1Pix;
//    int  usRotCen1Lin;
//    int  usRotCen2Pix;
//    int  usRotCen2Lin;
//}RING_ROT_CEN_STR;



#endif // GLOBALDEFINE_H
