#include "Processing.h"


/////////////////////////宏定义////////////////////////
//#define  FUNC_RING_DET0    FuncRingDet0
//#define  FUNC_RING_DET1    FuncRingDet1
//#define  FUNC_RING_DET2    FuncRingDet2
//#define  FUNC_RING_DET3    FuncRingDet3
//#define  FUNC_RING_DET4    FuncRingDet4
//#define  FUNC_RING_DET5    FuncRingDet5
//#define  FUNC_RING_DET6    FuncRingDet6


////////////////////////全局变量////////////////////////
//uint8_t  gucTwoValuThrld = 0x80;//0x8d;
//uint8_t  gucRndDetCnt;
//HOUGH_AREA_VALU_STR  gstHoughTransValue1;
//HOUGH_AREA_VALU_STR  gstHoughTransValue2;
//HOUGH_POS_STR        gstHoughPos;
//DETC_RING_POS_STR    gastRingPosDet[4];
//uint8_t  gucDetRingNum = 0;
//DETC_RING_ABC_STR    gstRingPos;
//DETC_RING_CEN_STR    gstRingCen;
//RING_ROT_CEN_STR     gstRingRotCen;


//uint8_t   guacHoughTransArea[0x200000];
//int	gausPix[4096];
//int	gausLin[4096];




/********************************************
函数功能:FuncDataInit函数实现，初始化函数
函数输入:
函数输出:
*********************************************/
void FuncDataInit(void)
{
    memset((void *)grayImageData, 0x00, sizeof(grayImageData));
    memset((void *)binaryImageData, 0x00, sizeof(binaryImageData));
    memset((void *)rotatedImageData, 0x00, sizeof(rotatedImageData));

}


/********************************************
函数功能:FuncProcessPing函数实现
函数输入:
函数输出:
*********************************************/
void FuncProcessing(void)
{


    // 二值化操作
    uint8_t * bindata =qr_binarize(grayImageData,IMG_WIDTH,IMG_HEIGHT);
    memcpy(binaryImageData,bindata,sizeof(uint8_t)*IMG_WIDTH*IMG_HEIGHT);
    free(bindata);


    //图像3个特征圆检测
    FuncRingsDetect(binaryImageData,0,IMG_WIDTH,0,IMG_HEIGHT);

//    //待添加: 因为已经定位了三个特征圆，下一步可以对数据进行剪裁，只保留有用的数据区域


//    //二维码检测
//    FuncGet2DCode((const uint8_t *)guacPing1YBuf,
//                  (const uint8_t *)guacPing0YBuf);
}




///********************************************
//函数功能:FuncGet2DCode函数实现,二维码检测
//函数输入:
//函数输出:
//*********************************************/
//void FuncGet2DCode(const uint8_t * pucInputData1,
//                           const uint8_t * pucInputData0)
//{
//    float   lAbsTheta;
//    uint8_t   ucGetRotFlg;

//    if (DETC_RING_NUM == gucDetRingNum) // 如果所有的3个特征环都被有效识别
//    {
//        gucDetRingNum = 0; // 计数器归零

//        //计算三个特征圆的圆心坐标
//        FuncGetRingCen();
//        //获取图像坐标
//        FuncRingPosDetc();

//        lAbsTheta = fabs(gstRingPos.lTheta);
//        if ((lAbsTheta - 10.0) < THRE_LOW_ROT) // 如果当前图像的偏转角度在10度范围内，则直接执行最小二乘
//        {

//            //最小二乘检测Vert
//            FuncLeastSquFitVert((const uint8_t *)pucInputData1,
//                                gastRingPosDet[1].usLenRndPix,
//                                gastRingPosDet[1].usRingLen,
//                                (gstRingPos.usRingBLin + (gastRingPosDet[1].usRingLen>>1) + RING_LIN_ADD),
//                                RING_LEN_LIN);

//            //最小二乘检测Hori
//            FuncLeastSquFitHori((const uint8_t *)pucInputData1,
//                                (gastRingPosDet[1].usLenRndPix + gastRingPosDet[1].usRingLen + RING_LIN_ADD),
//                                RING_LEN_LIN,
//                                gastRingPosDet[1].usLenRndLin - (gastRingPosDet[1].usRingLen>>1),
//                                gastRingPosDet[1].usRingLen);


//            //整幅图像旋转
//            FuncRotateLitl((const uint8_t *)pucInputData1,
//                           (uint8_t *)pucInputData0,
//                           -gstHoughTransValue1.fRadianVert);
//            //特殊点旋转，主要用于判断旋转后，三个特征圆环还是不是在有效区间内
//            ucGetRotFlg = GetRingRotCen(-gstHoughTransValue1.fRadianVert);

//            if (DEF_OK == ucGetRotFlg)
//            {
//                //获取二维码数据
//                GetCodeData((const uint8_t *)pucInputData0);
//                DeCode(gaucDecodeData,DECODE_DATA_LIN,gaucDecodeResult);
//            }

//        }
//        else if ((lAbsTheta - 10.0) >= THRE_LOW_ROT)  // 如果图像角度太偏，最小二乘方法的精度不太够，则先进行旋转
//        {

//            // 粗旋转过程可以只截取直线段的有效数据进行运算

//            //图像旋转
//            FuncRotate((const uint8_t *)pucInputData1,
//                       (uint8_t *)pucInputData0,
//                       -gstRingPos.lRadian);
//            //特殊点旋转
//            ucGetRotFlg = GetRingRotCen(-gstRingPos.lRadian);

//            if (DEF_OK == ucGetRotFlg)
//            {

//                //最小二乘检测Vert
//                FuncLeastSquFitVert((const uint8_t *)pucInputData0,
//                                    gstHoughPos.usPixStr,
//                                    gstHoughPos.usPixLen,
//                                    gstHoughPos.usLinStr,
//                                    RING_LEN_LIN);
//                //最小二乘检测Hori
//                FuncLeastSquFitHori((const uint8_t *)pucInputData1,
//                                    (gstHoughPos.usPixStr + gstHoughPos.usPixLen + RING_LIN_ADD),
//                                    RING_LEN_LIN,
//                                    gstHoughPos.usLinStr - gstHoughPos.usPixLen - RING_LIN_ADD,
//                                    gstHoughPos.usPixLen);

//                //整幅图像旋转, pucInputData0为旋转后的数据
//                FuncRotate((const uint8_t *)pucInputData1,
//                           (uint8_t *)pucInputData0,
//                           -gstHoughTransValue1.fRadianVert);
//                //特殊点旋转，用于判断特征圆环还是否在有效区域内
//                ucGetRotFlg = GetRingRotCen(-gstHoughTransValue1.fRadianVert);

//                if (DEF_OK == ucGetRotFlg)
//                {
//                    //获取二维码数据
//                    GetCodeData((const uint8_t *)pucInputData0);
//                    DeCode(gaucDecodeData,DECODE_DATA_LIN,gaucDecodeResult);
//                }
//            }
//            else
//            {
//                //err
//            }
//        }
//        gucDetRingNum = 0;
//        memset(gastRingPosDet, 0x00, sizeof(gastRingPosDet));
//        memset(&gstHoughPos, 0x00, sizeof(gstHoughPos));
//        memset(&gstRingPos, 0x00, sizeof(gstRingPos));
//        memset(&gstRingCen, 0x00, sizeof(gstRingCen));
//        memset(&gstRingRotCen, 0x00, sizeof(gstRingRotCen));
//    }
//    else
//    {
//        gucDetRingNum = 0;
//        memset(gastRingPosDet, 0x00, sizeof(gastRingPosDet));
//        memset(&gstHoughPos, 0x00, sizeof(gstHoughPos));
//        memset(&gstRingPos, 0x00, sizeof(gstRingPos));
//        memset(&gstRingCen, 0x00, sizeof(gstRingCen));
//        memset(&gstRingRotCen, 0x00, sizeof(gstRingRotCen));
//    }
//}
