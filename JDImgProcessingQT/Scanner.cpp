#include "Scanner.h"



///********************************************
//函数功能:FuncPing0BufInit函数实现
//函数输入:
//函数输出:
//*********************************************/
//void FuncPing0BufInit(void)
//{
//    memset((void *)guacPing0YBuf, 0x00, sizeof(guacPing0YBuf));
//    memset((void *)guacPing0CbBuf, 0x00, sizeof(guacPing0CbBuf));
//    memset((void *)guacPing0CrBuf, 0x00, sizeof(guacPing0CrBuf));
//}




///********************************************
//函数功能:FuncRingDetAll函数实现
//函数输入:
//函数输出:
//*********************************************/
//void FuncRingDetAll(const uint8_t *  pucDataStrAddr,
//                          int  usPixels,
//                          int  usLines)
//{
//    int	i;

//    /*传送临时缓冲区*/
//    memcpy((void *)(guacL2DataBufPing),
//           (void *)(pucDataStrAddr),
//             DATA_LEN_DEF);

//    for(i = 0; i < NUM_LEN_DEF; i++)
//    {
//        if (0 == (i & 0x01))
//        {
//            /*传送临时缓冲区*/
//            memcpy((void *)(guacL2DataBufPang),
//                   (void *)(pucDataStrAddr + ((i + 1) * DATA_LEN_DEF)),
//                     DATA_LEN_DEF);

//            FuncRingDet((const uint8_t *)guacL2DataBufPing,
//                        PIXELS_NUM,
//                        (NUM_DEAL_DEF*i));
//        }
//        else
//        {

//            /*传送临时缓冲区*/
//            memcpy((void *)(guacL2DataBufPing),
//                     (void *)(pucDataStrAddr + ((i + 1) * DATA_LEN_DEF)),
//                     DATA_LEN_DEF);

//            FuncRingDet((const uint8_t *)guacL2DataBufPang,
//                        PIXELS_NUM,
//                        (NUM_DEAL_DEF*i));
//        }
//    }

//}



/********************************************
函数功能:FuncRingDet函数实现,图像轮廓检测
函数输入:
函数输出:
*********************************************/
void FuncRingsDetect(const uint8_t * pucDataStrAddr,int pixStart,int pixEnd,int linStart,int linEnd)
{
    int	 usPix,usLin;

    uint8_t	 uacData[6];
    int	 usDataSumTmp0;
    int	 usDataSumTmp1;
    int	 uasPi[4];
    uint8_t	 n;
    uint8_t	 ucDistNum=0;
    int	 usDistPix;
    int	 usDistLin;
    int	 ucDetFlg = 0;
    bool	 ucRingFlg = false;
    bool	 ucLinMinusFlg = false;
    int	 usLen[5] = {0,0,0,0,0};

    DetectedRingsNum=0;
    memset(detectedRings, 0x00, sizeof(detectedRings));

    usPix = 3;
    for (usLin = linStart; usLin < linEnd; usLin++) // 行
    {
        if (true == ucLinMinusFlg)
        {
            usLin--;  // 维持在本行，本行可能存在圆形标识符
            ucLinMinusFlg = false;
        }

        if (usPix > (pixEnd - 2))  // 本行搜索完成，进入下一行
        {
            usPix = 3;
            ucDetFlg = 0; // 重置搜索标识符
            memset(usLen, 0x00, sizeof(usLen));
        }

        switch (ucDetFlg)
        {
            case 0 :
            {
                memset(usLen, 0x00, sizeof(usLen));
                for (; usPix < pixEnd - 1; usPix++)
                {
                    // 此处效率不高，可以改进，这个运算pucDataStrAddr + usLin*pixEnd可以合并
                    uacData[0] = (*(uint8_t *)(pucDataStrAddr + usLin*pixEnd + usPix - 3));
                    uacData[1] = (*(uint8_t *)(pucDataStrAddr + usLin*pixEnd + usPix - 2));
                    uacData[2] = (*(uint8_t *)(pucDataStrAddr + usLin*pixEnd + usPix - 1));
                    uacData[3] = (*(uint8_t *)(pucDataStrAddr + usLin*pixEnd + usPix));
                    uacData[4] = (*(uint8_t *)(pucDataStrAddr + usLin*pixEnd + usPix + 1));
                    uacData[5] = (*(uint8_t *)(pucDataStrAddr + usLin*pixEnd + usPix + 2));

                    usDataSumTmp0 = uacData[0] + uacData[1] + uacData[2];
                    usDataSumTmp1 = uacData[3] + uacData[4] + uacData[5];

                    if (((WHITE*3) == usDataSumTmp0)&&(BLACK == usDataSumTmp1))
                      //&&(usPix < pixEnd - MIN_LEN_RING_DET))
                    {
                        // 如果遇到3块白+3块黑的

                        ucDetFlg = 1;
                        ucLinMinusFlg = true; // 检测到可能是圆形标识符
                        break;
                    }
                }
            }
            break;
            case 1 : // BLACK 0
            {
                // 继续往后搜索，步长为2
                for (; usPix < pixEnd - 1; usPix += 2)
                {
                    uacData[0] = (*(uint8_t *)(pucDataStrAddr + usLin*pixEnd + usPix));
                    uacData[1] = (*(uint8_t *)(pucDataStrAddr + usLin*pixEnd + usPix + 1));

                    usDataSumTmp0 = uacData[0] + uacData[1];
                    if (usDataSumTmp0 < (WHITE + 1)) // 如果小于256，可能是黑白相间，但肯定不是全白
                    {
                        usLen[0] += 2;  // 黑块长度增加2
                    }

                    if ((WHITE<<1) == usDataSumTmp0)
                    {
                        ucDetFlg = 2; // 检测到白块部分，进入下一个判别环节
                        //usPix--;
                        if ((usLen[0] > DELTA_LEN_MAX)  //如果累积的黑块长度大于25或小于7，则认为不是圆形标记符
                          ||(usLen[0] < DELTA_LEN_MIN))
                        {
                            ucDetFlg = 0;
                        }
                        ucLinMinusFlg = true; // 仍然维持在本行，继续从case0开始搜索
                        break;
                    }

                    if (usLen[0] > DELTA_LEN_MAX) // 如果黑块长度已经大于25
                    {
                        ucDetFlg = 0;  // 圆形标识符检测失败
                        ucLinMinusFlg = true; // 仍然维持在本行，继续从case0开始搜索
                        break;
                    }

                }
            }
            break;
            case 2 :  // WHITE 1
            {
                for (; usPix < pixEnd - 1; usPix += 2) // 继续往后搜索，步进为2
                {
                    // pucDataStrAddr + usLin*pixEnd 可以化简

                    uacData[0] = (*(uint8_t *)(pucDataStrAddr + usLin*pixEnd + usPix));
                    uacData[1] = (*(uint8_t *)(pucDataStrAddr + usLin*pixEnd + usPix + 1));

                    usDataSumTmp0 = uacData[0] + uacData[1];
                    if (usDataSumTmp0 > (WHITE - 1)) // 如果大于254，可能是黑白相间，但肯定不是全黑
                    {
                        usLen[1] += 2; // 白块长度增加2
                    }

                    if (BLACK == usDataSumTmp0)
                    {
                        uasPi[0] = (usLen[1]*100)/usLen[0]; // white1与black0长度做比值*100

                        if ((uasPi[0] > THRE_LOW1_RING_DET)  // 如果比值大于67且小于145
                          &&(uasPi[0] < THRE_HIGH1_RING_DET))
                        {
                            ucDetFlg = 3; // 白块辨别成功，进入下一步
                            //usPix--;
                            if ((usLen[1] > BELT_LEN_RING_DET) //如果白块宽度大于25或者小于7，则识别失败
                              ||(usLen[1] < BELT_MIN_RING_DET))
                            {
                                ucDetFlg = 0;  //  继续从case0开始搜索
                            }
                            ucLinMinusFlg = true; // 仍然维持在本行
                            break;
                        }
                        else
                        {
                            ucDetFlg = 0;     // 圆形标识符检测失败
                            ucLinMinusFlg = true; // 仍然维持在本行，继续从case0开始搜索
                            break;
                        }
                    }

                    if (usLen[1] > BELT_LEN_RING_DET) // 如果白块的长度也已经大于25了
                    {
                        ucDetFlg = 0;        // 圆形标识符检测失败
                        ucLinMinusFlg = true;    // 仍然维持在本行，继续从case0开始搜索
                        break;
                    }
                }
            }
            break;
            case 3 :  // BLACK 2
            {
                for (; usPix < pixEnd - 1; usPix += 2)  //步进2
                {
                    uacData[0] = (*(uint8_t *)(pucDataStrAddr + usLin*pixEnd + usPix));
                    uacData[1] = (*(uint8_t *)(pucDataStrAddr + usLin*pixEnd + usPix + 1));

                    usDataSumTmp0 = uacData[0] + uacData[1];
                    if (usDataSumTmp0 < (WHITE + 1)) // 如果不是全白
                    {
                        usLen[2] += 2; // 黑块长度增2
                    }

                    if ((WHITE<<1) == usDataSumTmp0)  // 如果是白的
                    {
                        uasPi[1] = (usLen[2]*10)/usLen[0]; // black2与black0长度做比值*10

                        if ((uasPi[1] > THRE_LOW4_RING_DET) // 如果比值大于34且小于45
                          &&(uasPi[1] < THRE_HIGH4_RING_DET))
                        {
                            ucDetFlg = 4; // 黑块识别成功，进入下一个环节
                            //usPix--;
                            if ((usLen[2] > (BELT_LEN_RING_DET<<2)) // 如果黑块的长度大于
                              ||(usLen[2] < (BELT_MIN_RING_DET<<2))) // 或者小于
                            {
                                ucDetFlg = 0; // 检测失败，仍然维持在本行，继续从case0开始搜索
                            }
                            ucLinMinusFlg = true;
                            break;
                        }
                        else
                        {
                            ucDetFlg = 0;
                            ucLinMinusFlg = true;  // 检测失败，仍然维持在本行，继续从case0开始搜索
                            break;
                        }
                    }

                    if (usLen[2] > RND_LEN_RING_DET) //如果黑块长度大于100
                    {
                        ucDetFlg = 0;
                        ucLinMinusFlg = true;  // 检测失败，仍然维持在本行，继续从case0开始搜索
                        break;
                    }
                }
            }
            break;
            case 4 :  // WHITE 3
            {
                for (; usPix < pixEnd - 1; usPix += 2) // 步进2
                {
                    uacData[0] = (*(uint8_t *)(pucDataStrAddr + usLin*pixEnd + usPix));
                    uacData[1] = (*(uint8_t *)(pucDataStrAddr + usLin*pixEnd + usPix + 1));

                    usDataSumTmp0 = uacData[0] + uacData[1];
                    if (usDataSumTmp0 > (WHITE - 1)) // 如果不是全黑
                    {
                        usLen[3] += 2; //白块长度加2
                    }

                    if (BLACK == usDataSumTmp0)  // 如果检测到全黑
                    {
                        uasPi[2] = (usLen[3]*100)/usLen[0];  // white3与black0长度做比值*100

                        if ((uasPi[2] > THRE_LOW1_RING_DET) // 如果比值在范围内
                          &&(uasPi[2] < THRE_HIGH1_RING_DET))
                        {
                            ucDetFlg = 5; // 检测成功，进入下一步
                            //usPix--;
                            if ((usLen[3] > BELT_LEN_RING_DET) // 如果白块长度过长或过短，则检测失败
                              ||(usLen[3] < BELT_MIN_RING_DET))
                            {
                                ucDetFlg = 0;
                            }
                            ucLinMinusFlg = true;
                            break;
                        }
                        else
                        {
                            ucDetFlg = 0;  // 如果比值不在范围内，则判断检测失败
                            ucLinMinusFlg = true;
                            break;
                        }
                    }

                    if (usLen[3] > BELT_LEN_RING_DET) // 如果白块长度过长，则检测失败
                    {
                        ucDetFlg = 0;
                        ucLinMinusFlg = true;
                        break;
                    }
                }
            }
            break;
            case 5 :  // BLACK 4
            {
                for (; usPix < pixEnd - 1; usPix += 2)  // 步进2
                {
                    uacData[0] = (*(uint8_t *)(pucDataStrAddr + usLin*pixEnd + usPix));
                    uacData[1] = (*(uint8_t *)(pucDataStrAddr + usLin*pixEnd + usPix + 1));

                    usDataSumTmp0 = uacData[0] + uacData[1];
                    if (usDataSumTmp0 < (WHITE + 1)) // 如果非全白
                    {
                        usLen[4] += 2;  // 黑块长度增2
                    }

                    if ((WHITE<<1) == usDataSumTmp0) // 如果检测到全白
                    {
                        uasPi[3] = (usLen[4]*100)/usLen[0];  // black4与black0长度做比值*100

                        if ((uasPi[3] > THRE_LOW1_RING_DET)
                          &&(uasPi[3] < THRE_HIGH1_RING_DET)) // 如果比值在范围内
                        {
                            ucDetFlg = 6; // 辨识成功，进入下一步
                            //usPix--;
                            if ((usLen[4] > BELT_LEN_RING_DET)
                              ||(usLen[4] < BELT_MIN_RING_DET)) // 如果黑块过短或过长，则检测失败
                            {
                                ucDetFlg = 0;
                            }
                            ucLinMinusFlg = true;
                            break;
                        }
                        else
                        {
                            ucDetFlg = 0;
                            ucLinMinusFlg = true; // 比值不在范围内，检测失败
                            break;
                        }
                    }

                    if (usLen[4] > BELT_LEN_RING_DET)  // 如果黑块过长，则检测失败
                    {
                        ucDetFlg = 0;
                        ucLinMinusFlg = true;
                        break;
                    }
                }
            }
            break;
            case 6 :  // Detc ring，经过以上检测步骤，成功检测出圆环
            {
                // 圆环截面的总长度
                detectedRings[DetectedRingsNum].Len = usLen[4] + usLen[3]  \
                                                        + usLen[2] + usLen[1]  \
                                                        + usLen[0];
                //消除域内重复，
                // DetectedRingsNum为检测到的总圆环的数目
                // detectedRings存储所有已经检测到的圆环
                for (n= 0; n < DetectedRingsNum; n++)
                {
                    // 当前圆环距所有已知圆环的水平像素距离
                    usDistPix = abs(usPix - 1 - detectedRings[DetectedRingsNum].Len - detectedRings[n].pos.Pix);
                    // 当前圆环距所有已知圆环的垂直像素距离
                    usDistLin = abs(usLin - detectedRings[n].pos.Lin);

                    if ((usDistPix + usDistLin) > MIN_LEN_RING_DET) // 如果像素距离和大于80
                    {
                        ucDistNum += 1;  // 问题，ucDistNum没有赋初值，是不是有问题??
                    }
                    else
                    {
                        detectedRings[n].DetectCount += 1; // 当前圆环与第n个圆环为同一圆环，被检测到的次数加1
                        // 通过统计检测次数，可以估计圆环的高度
                    }
                }
                // 对于以上循环的分析，如果当前DetectedRingsNum=0；则以上操作只会将usRingDetCnt的计数加一
                // 如果DetectedRingsNum=2，则当前圆环会与Num=1,Num=0的圆环做比较，每次比较成功ucDistNum += 1
                // 因此，最终ucDistNum=2.



                if (ucDistNum == DetectedRingsNum) // 如果当前圆环与所有已知圆环均可有效区分
                {
                    ucRingFlg = true;
                }
                ucDistNum = 0; // 重置计数器

                if ((detectedRings[DetectedRingsNum].Len < (pixEnd>>2)) // 如果当前圆环的长度小于1/4总长度
                  &&(true == ucRingFlg)) // 且能与其他圆环有效区分
                {
                    ucRingFlg = false; // 重置圆环判别标记位

                    // 记录当前圆环的行列像素坐标
                    detectedRings[DetectedRingsNum].pos.Pix = usPix - 1 - detectedRings[DetectedRingsNum].Len;
                    detectedRings[DetectedRingsNum].pos.Lin = usLin;
                    detectedRings[DetectedRingsNum].DetectCount += 1; // 检测次数加1

                    DetectedRingsNum++; // 圆环数目加1

                    ucDetFlg = 0; // 重置特征判别标记位
                }
                else
                {
                    ucDetFlg = 0;
                    ucLinMinusFlg = true; // 圆环特征判别错误，检测失败
                    break;
                }
            }
            break;
            default :
            {
                //error;
                ucDetFlg = 0;
            }
        }
    }

}



///********************************************
//函数功:FuncRingPosDetc函数实现,获取图像坐标
//函数输入:
//函数输出:
//*********************************************/
//void FuncRingPosDetc(void)
//{
//    int  ulLinMinus01,ulLinMinus02,ulLinMinus12;
//    int  ulPixMinus01,ulPixMinus02,ulPixMinus12;
//    int  ulLenSqu01,ulLenSqu12,ulLenSqu02;
//    int  ulLinPixSum0,ulLinPixSum1,ulLinPixSum2;
//    float   lTmp;
//    int   sPixTmp,sLinTmp;

//    ulLinMinus01 = abs(gstRingCen.usCen0Lin - gstRingCen.usCen1Lin);
//    ulPixMinus01 = abs(gstRingCen.usCen0Pix - gstRingCen.usCen1Pix);
//    ulLinMinus02 = abs(gstRingCen.usCen0Lin - gstRingCen.usCen2Lin);
//    ulPixMinus02 = abs(gstRingCen.usCen0Pix - gstRingCen.usCen2Pix);
//    ulLinMinus12 = abs(gstRingCen.usCen1Lin - gstRingCen.usCen2Lin);
//    ulPixMinus12 = abs(gstRingCen.usCen1Pix - gstRingCen.usCen2Pix);

//    ulLenSqu01 = ulLinMinus01*ulLinMinus01 + ulPixMinus01*ulPixMinus01;
//    ulLenSqu02 = ulLinMinus02*ulLinMinus02 + ulPixMinus02*ulPixMinus02;
//    ulLenSqu12 = ulLinMinus12*ulLinMinus12 + ulPixMinus12*ulPixMinus12;

//    if ((ulLenSqu01 > ulLenSqu02)&&(ulLenSqu01 > ulLenSqu12))  // (90--180]
//    {
//        gstRingPos.usRingBLin = gstRingCen.usCen2Lin;
//        gstRingPos.usRingBPix = gstRingCen.usCen2Pix;
//        sLinTmp = fabs(gstRingCen.usCen0Lin - gstRingCen.usCen1Lin);
//        sPixTmp = gstRingCen.usCen0Pix - gstRingCen.usCen1Pix;
//        gstRingPos.lKi = (float)sLinTmp/sPixTmp;
//        lTmp = atanf(gstRingPos.lKi);
//        gstRingPos.lTheta = RADIAN_TO_ANGLE * lTmp+ 135.0;
//        gstRingPos.lRadian = lTmp + PI34;
//    }
//    else if ((ulLenSqu02 > ulLenSqu01)&&(ulLenSqu02 > ulLenSqu12))
//    {
//        gstRingPos.usRingBLin = gstRingCen.usCen1Lin;
//        gstRingPos.usRingBPix = gstRingCen.usCen1Pix;

//        ulLinPixSum1 = gstRingCen.usCen1Lin + gstRingCen.usCen1Pix;
//        ulLinPixSum0 = gstRingCen.usCen0Lin + gstRingCen.usCen0Pix;
//        ulLinPixSum2 = gstRingCen.usCen2Lin + gstRingCen.usCen2Pix;

//        if ((ulLinPixSum1 < ulLinPixSum2) && (gstRingCen.usCen1Pix <= gstRingCen.usCen0Pix)
//         && (gstRingCen.usCen1Pix <= gstRingCen.usCen2Pix))
//        {
//            sLinTmp = fabs(gstRingCen.usCen0Lin - gstRingCen.usCen2Lin);
//            sPixTmp = gstRingCen.usCen0Pix - gstRingCen.usCen2Pix;
//            gstRingPos.lKi = (float)sLinTmp/sPixTmp;
//            lTmp = atanf(gstRingPos.lKi);
//            if (sPixTmp >= 0)  // (0--45]
//            {
//                gstRingPos.lTheta = RADIAN_TO_ANGLE * lTmp - 45.0;
//                gstRingPos.lRadian = lTmp - PI14;
//            }
//            else               // (45--90]
//            {
//                gstRingPos.lTheta = RADIAN_TO_ANGLE * lTmp + 135.0;
//                gstRingPos.lRadian = lTmp + PI34;
//            }
//        }
//        else if ((ulLinPixSum1 > ulLinPixSum0) && (gstRingCen.usCen1Pix >= gstRingCen.usCen0Pix)
//              && (gstRingCen.usCen1Pix >= gstRingCen.usCen2Pix))
//        {
//            sLinTmp = fabs(gstRingCen.usCen0Lin - gstRingCen.usCen2Lin);
//            sPixTmp = gstRingCen.usCen0Pix - gstRingCen.usCen2Pix;
//            gstRingPos.lKi = (float)sLinTmp/sPixTmp;
//            lTmp = atanf(gstRingPos.lKi);
//            if (sPixTmp >= 0)  // ((-180)--(-135))
//            {
//                gstRingPos.lTheta = RADIAN_TO_ANGLE * lTmp - 225.0;
//                gstRingPos.lRadian = lTmp - PI54;
//            }
//            else			   // [(-135)--(-90)]
//            {
//                gstRingPos.lTheta = RADIAN_TO_ANGLE * lTmp - 45.0;
//                gstRingPos.lRadian = lTmp - PI14;
//            }
//        }
//    }
//    else if ((ulLenSqu12 > ulLenSqu02)&&(ulLenSqu12 > ulLenSqu02)) // ((-90)--0]
//    {
//        gstRingPos.usRingBLin = gstRingCen.usCen0Lin;
//        gstRingPos.usRingBPix = gstRingCen.usCen0Pix;
//        sLinTmp = fabs(gstRingCen.usCen1Lin - gstRingCen.usCen2Lin);
//        sPixTmp = gstRingCen.usCen1Pix - gstRingCen.usCen2Pix;
//        gstRingPos.lKi = (float)sLinTmp/sPixTmp;
//        lTmp = atanf(gstRingPos.lKi);
//        gstRingPos.lTheta = RADIAN_TO_ANGLE * lTmp - 45.0;
//        gstRingPos.lRadian = lTmp - PI14;
//    }
//}
///********************************************
//函数功能:FuncGetRingCen函数实现,计算坐标
//函数输入:
//函数输出:
//*********************************************/
//void FuncGetRingCen(void)
//{
//    int  usTmp0,usTmp1,usTmp2;

//    usTmp2 = gastRingPosDet[2].usRingDetCnt;
//    usTmp1 = gastRingPosDet[1].usRingDetCnt;
//    usTmp0 = gastRingPosDet[0].usRingDetCnt;

//    // 三个特征环的圆心位置
//    gstRingCen.usCen0Lin = gastRingPosDet[0].usLenRndLin + (usTmp0>>1); // 上边界+半径
//    gstRingCen.usCen0Pix = gastRingPosDet[0].usLenRndPix + (gastRingPosDet[0].usRingLen>>1); // 左边界+半径

//    gstRingCen.usCen1Lin = gastRingPosDet[1].usLenRndLin + (usTmp1>>1);
//    gstRingCen.usCen1Pix = gastRingPosDet[1].usLenRndPix + (gastRingPosDet[1].usRingLen>>1);

//    gstRingCen.usCen2Lin = gastRingPosDet[2].usLenRndLin + (usTmp2>>1);
//    gstRingCen.usCen2Pix = gastRingPosDet[2].usLenRndPix + (gastRingPosDet[2].usRingLen>>1);
//}



///********************************************
//函数功能:FuncRotate函数实现,图像旋转
//函数输入:
//函数输出:
//*********************************************/
//void FuncRotate(const uint8_t * pucInputData,   /* Input data   */
//                     uint8_t * pucOutputData,  /* Output data  */
//                     float  fAngle)
//{
//    int   lTmp0,lTmp1;
//    int	lRotPixTmp,lRotLinTmp;
//    int  usPix,usLin;
//    int   sRotPix,sRotLin;
//    int   sCosAngle,sSinAngle;


//    sCosAngle = (int)(32768*cosf(PI2 - fAngle));
//    sSinAngle = (int)(32768*sinf(PI2 - fAngle));

//    lTmp0 = ((int)(PIXELS_NUM-1)*(32768-sCosAngle) - (int)(LINES_NUM-1)*sSinAngle)>>1;
//    lTmp1 = ((int)(PIXELS_NUM-1)*sSinAngle + (int)(LINES_NUM-1)*(32768-sCosAngle))>>1;

//    memset((void *)pucOutputData, 0xff, 576*720);

//    for(usLin = 0; usLin < LINES_NUM; usLin++)
//    {
//        #pragma MUST_ITERATE(90,90,8);
//        for(usPix = 0; usPix < PIXELS_NUM; usPix++)
//        {
//            //ucPort = pucInputData[usLin*PIXELS_NUM + usPix];
//            //if (BLACK == ucPort)
//            {
//                lRotPixTmp = ((int)usPix*sCosAngle + (int)usLin*sSinAngle + lTmp0);
//                lRotLinTmp = ((int)usLin*sCosAngle - (int)usPix*sSinAngle + lTmp1);

//                sRotPix = (int)(lRotPixTmp>>15);
//                sRotLin = (int)(lRotLinTmp>>15);

//                //判断图像范围内
//                if((sRotPix >= 0) && (sRotPix < PIXELS_NUM)
//                && (sRotLin >= 0) && (sRotLin < LINES_NUM))
//                {
//                    //传送亮度信号
//                    *(uint8_t*)(pucOutputData + usLin*PIXELS_NUM + usPix) = \
//                    *(uint8_t*)(pucInputData + sRotLin*PIXELS_NUM + sRotPix);
//                }
//                else
//                {
//                    *(uint8_t*)(pucOutputData + usLin*PIXELS_NUM + usPix) = 0xFF;
//                }

//            }
//        }
//    }

//}
///********************************************
//函数功能:FuncRotateLitl函数实现,图像旋转
//函数输入:
//函数输出:
//*********************************************/
//void FuncRotateLitl(const uint8_t * pucInputData,   /* Input data   */
//                         uint8_t * pucOutputData,  /* Output data  */
//                         float  fAngle)
//{
//    int   lTmp0,lTmp1;
//    int	lRotPixTmp,lRotLinTmp;
//    int  usPix,usLin;
//    int   sRotPix,sRotLin;
//    int   sCosAngle,sSinAngle;
//    int  usCenPix0;
//    int  usCenLin0;
//    int  usPixStr0;
//    int  usPixEnd0;
//    int  usLinStr0;
//    int  usLinEnd0;
//    //int  usCnt;

//    // 粗略获取二维码中心位置
//    usCenPix0 = (gstRingCen.usCen0Pix + gstRingCen.usCen2Pix)>>1;
//    usCenLin0 = (gstRingCen.usCen0Lin + gstRingCen.usCen2Lin)>>1;

//    // 获取二维码的大致区域，其中:
//    //
//    usPixStr0 = usCenPix0 - PIX_LEN_HALF - PIX_LEN_ONE_GRID;
//    usPixEnd0 = usCenPix0 + PIX_LEN_HALF + PIX_LEN_ONE_GRID;
//    usLinStr0 = usCenLin0 - LIN_LEN_HALF - LIN_LEN_ONE_GRID;
//    usLinEnd0 = usCenLin0 + LIN_LEN_HALF + LIN_LEN_ONE_GRID;

//    sCosAngle = (int)(32768*cosf(PI2 - fAngle));
//    sSinAngle = (int)(32768*sinf(PI2 - fAngle));

//    lTmp0 = ((int)(PIXELS_NUM-1)*(32768-sCosAngle) - (int)(LINES_NUM-1)*sSinAngle)>>1;
//    lTmp1 = ((int)(PIXELS_NUM-1)*sSinAngle + (int)(LINES_NUM-1)*(32768-sCosAngle))>>1;

//    memset((void *)pucOutputData, 0xff, 576*720);

//    for(usLin = usLinStr0; usLin < usLinEnd0; usLin++)
//    {
//        //#pragma MUST_ITERATE(90,90,8);
//        for(usPix = usPixStr0; usPix < usPixEnd0; usPix++)
//        {
//            //ucPort = pucInputData[usLin*PIXELS_NUM + usPix];
//            //if (BLACK == ucPort)
//            {
//                // 以lTmp0，lTmp1为中心进行平移
//                lRotPixTmp = ((int)usPix*sCosAngle + (int)usLin*sSinAngle + lTmp0);
//                lRotLinTmp = ((int)usLin*sCosAngle - (int)usPix*sSinAngle + lTmp1);

//                sRotPix = (int)(lRotPixTmp>>15); //数据换算回真实数据
//                sRotLin = (int)(lRotLinTmp>>15);

//                //判断图像范围内
//                if((sRotPix >= 0) && (sRotPix < PIXELS_NUM)
//                && (sRotLin >= 0) && (sRotLin < LINES_NUM))
//                {
//                    //传送亮度信号，返回旋转后的数据
//                    *(uint8_t*)(pucOutputData + usLin*PIXELS_NUM + usPix) = \
//                    *(uint8_t*)(pucInputData + sRotLin*PIXELS_NUM + sRotPix);
//                }
//                else
//                {
//                    *(uint8_t*)(pucOutputData + usLin*PIXELS_NUM + usPix) = 0xFF;
//                }
//            }
//        }
//    }
//}
///********************************************
//函数功能:FuncRotate函数实现
//函数输入:
//函数输出:
//*********************************************/
//uint8_t FuncPortRotate(int  usPix,   /* Input data   */
//                           int  usLin,   /* Input data   */
//                           float   fAngle,
//                           int* pusRotPix, /* Output data   */
//                           int* pusRotLin) /* Output data   */
//{

//    float   fCosAngle,fSinAngle;
//    float   fTmp0,fTmp1;
//    float   fRotPixTmp,fRotLinTmp;

//    fCosAngle = cosf(PI2 - fAngle);
//    fSinAngle = sinf(PI2 - fAngle);

//    fTmp0 = 0.5*(PIXELS_NUM-1)*(1-fCosAngle) + 0.5*(LINES_NUM-1)*fSinAngle;
//    fTmp1 = -0.5*(PIXELS_NUM-1)*fSinAngle + 0.5*(LINES_NUM-1)*(1-fCosAngle);

//    fRotPixTmp = (int)(usPix*fCosAngle - usLin*fSinAngle + fTmp0 + 0.5);
//    fRotLinTmp = (int)(usLin*fCosAngle + usPix*fSinAngle + fTmp1 + 0.5);

//    //判断是否在原图范围内
//    if((fRotPixTmp >= 0) && (fRotPixTmp < PIXELS_NUM)
//    && (fRotLinTmp >= 0) && (fRotLinTmp < LINES_NUM))
//    {
//        *pusRotPix = (int)fRotPixTmp;
//        *pusRotLin = (int)fRotLinTmp;
//        return DEF_OK;
//    }
//    else
//    {
//        return DEF_FAIL;
//    }

//}
///********************************************
//函数功能:GetRingRotCen函数实现,特殊点旋转
//函数输入:
//函数输出:
//*********************************************/
//uint8_t GetRingRotCen(float  fAngle)
//{
//    uint8_t  ucTmp0 = 0;
//    uint8_t  ucTmp1 = 0;
//    uint8_t  ucTmp2 = 0;

//    ucTmp0 = FuncPortRotate(gstRingCen.usCen0Pix,
//                            gstRingCen.usCen0Lin,
//                            fAngle,
//                            &(gstRingRotCen.usRotCen0Pix),
//                            &(gstRingRotCen.usRotCen0Lin));

//    ucTmp1 = FuncPortRotate(gstRingCen.usCen1Pix,
//                            gstRingCen.usCen1Lin,
//                            fAngle,
//                            &(gstRingRotCen.usRotCen1Pix),
//                            &(gstRingRotCen.usRotCen1Lin));

//    ucTmp2 = FuncPortRotate(gstRingCen.usCen2Pix,
//                            gstRingCen.usCen2Lin,
//                            fAngle,
//                            &(gstRingRotCen.usRotCen2Pix),
//                            &(gstRingRotCen.usRotCen2Lin));

//    if (DEF_OK == (ucTmp0 + ucTmp1 + ucTmp2))
//    {
//        return DEF_OK;
//    }
//    else
//    {
//        return DEF_FAIL;
//    }

//}




///*********************************************************
//* 函数名称: FuncCosSin
//* 函数输入:
//* 函数输入:
//**********************************************************/

//inline void FuncCosSin(int  sInputAngle,            // l(16,12)
//                            int  *psOutputCos,       // l(16,15)
//                            int  *psOutputSin)
//{
//    int   x_2_32;          //x^2,sint 32
//    int   x_2_16;          //x^2,sint 16
//    int   x_2_sixth_16;    //1/3!*x^2 sint 16
//    int   x_3_16;          //x^3,sint 16
//    int   sCosTmp;
//    int   sSinTmp;

//    sCosTmp = 0x7fff;
//    sSinTmp = (int)(sInputAngle << 3);

//    x_2_32 = sInputAngle * sInputAngle;
//    sCosTmp = (int)(sCosTmp - (x_2_32 >> 10));

//    x_2_16 = (int)(x_2_32 >> 9);
//    x_2_sixth_16 = (int)((x_2_16 * CUL_COM_SIXTH) >> 15);
//    x_3_16 = (int)((x_2_sixth_16 * sInputAngle) >> 12);
//    sSinTmp = (int)(sSinTmp - x_3_16);

//    sCosTmp = (int)(sCosTmp + (((int)x_2_16 * x_2_sixth_16) >> 17));

//    *psOutputCos = sCosTmp;
//    *psOutputSin = sSinTmp;
//}

///********************************************
//函数功能:FuncLeastSquFitVert函数实现,最小二乘检测
//函数输入:
//函数输出:
//*********************************************/
//void FuncLeastSquFitVert(const uint8_t * pucInputData,   /* Input data   */
//                                int      usPixStr,
//                                int      usPixLen,
//                                int      usLinStr,
//                                int      usLinLen)
//{

//    float	 ulSumPixSqu;
//    float	 ulSumPixSumPix;
//    float	 ulSumPixLin;
//    float	 ulMeanPix;
//    float	 ulMeanLin;
//    float	 ulSumMeanPixSqu;
//    float	 ulSumMeanLinSqu;
//    float	 ulSumPixSquN;
//    float	 lSumMeanPix;
//    float	 lSumMeanLin;
//    float	 lTemp;
//    float	 lPiTmp;
//    float	 lATmp;
//    float	 fPixSqu2;
//    float	 fLinSqu2;
//    float	 fA;
//    float	 fPi;
//    float	 fR;
//    float	 fTmp;
//    float	 fAnglNum;
//    int	 ulSumPix;
//    int	 ulSumLin;
//    int	 i,j,k;
//    int	 usCnt;
//    uint8_t	 ucPort;

//    usCnt = 0;
//    ulSumPix = 0;
//    ulSumLin = 0;
//    ulSumPixSqu = 0;
//    ulSumPixLin = 0;
//    lSumMeanPix = 0;
//    lSumMeanLin = 0;
//    ulSumMeanPixSqu = 0;
//    ulSumMeanLinSqu = 0;
//    memset((void *)gausPix, 0x00, sizeof(gausPix));
//    memset((void *)gausLin, 0x00, sizeof(gausLin));

//    for (j = usLinStr; j < (usLinLen + usLinStr); j++)
//    {
//        for (i = usPixStr; i < (usPixLen + usPixStr); i++)
//        {
//            ucPort = pucInputData[j*PIXELS_NUM + i];

//            //二值图
//            if (BLACK == ucPort)
//            {
//                ulSumPix += j;
//                ulSumLin += i;
//                ulSumPixSqu += (int)j * j;
//                ulSumPixLin += (int)i * j;

//                gausPix[usCnt] = j;
//                gausLin[usCnt] = i;

//                usCnt += 1;
//                usCnt &= 0x0fff;
//            }
//        }
//    }

//    ulMeanPix = (float)ulSumPix/usCnt;
//    ulMeanLin = (float)ulSumLin/usCnt;

//    for(k = 0; k < usCnt; k++)
//    {
//        lSumMeanPix += (float)(gausPix[k] - ulMeanPix);
//        lSumMeanLin += (float)(gausLin[k] - ulMeanLin);
//        ulSumMeanPixSqu += (float)((gausPix[k] - ulMeanPix)*(gausPix[k] - ulMeanPix));
//        ulSumMeanLinSqu += (float)((gausPix[k] - ulMeanLin)*(gausPix[k] - ulMeanLin));
//    }

//    fPixSqu2 = sqrtf(ulSumMeanPixSqu);
//    fLinSqu2 = sqrtf(ulSumMeanLinSqu);

//    ulSumPixSumPix = (float)ulSumPix * ulSumPix;
//    ulSumPixSquN = (float)usCnt * ulSumPixSqu;
//    lTemp = (float)(ulSumPixSquN - ulSumPixSumPix);
//    lATmp = (float)((float)ulSumLin * ulSumPixSqu - (float)ulSumPix * ulSumPixLin);
//    lPiTmp = (float)((float)usCnt * ulSumPixLin - (float)ulSumPix * ulSumLin);
//    fPi = (float)lPiTmp/lTemp;
//    fA = (float)lATmp/lTemp;
//    gstHoughTransValue1.fFitPiVert = fPi;
//    gstHoughTransValue1.fFitAVert = fA;

//    fTmp = atanf(fPi);
//    fAnglNum = RADIAN_TO_ANGLE * fTmp;

//    gstHoughTransValue1.sAnglNumVert = (int)(fAnglNum*10);
//    gstHoughTransValue1.fRadianVert = fTmp;

//    fR = (float)(lSumMeanPix * lSumMeanLin)/(fPixSqu2 * fLinSqu2);
//    gstHoughTransValue1.fFitRVert = fR;

//}
///********************************************
//函数功能:FuncLeastSquFitHori函数实现,最小二乘检测
//函数输入:
//函数输出:
//*********************************************/
//void FuncLeastSquFitHori(const uint8_t * pucInputData,   /* Input data   */
//                                int      usPixStr,
//                                int      usPixLen,
//                                int      usLinStr,
//                                int      usLinLen)
//{
//    float	 ulSumPixSqu;
//    float	 ulSumPixSumPix;
//    float	 ulSumPixLin;
//    float	 ulMeanPix;
//    float	 ulMeanLin;
//    float	 ulSumMeanPixSqu;
//    float	 ulSumMeanLinSqu;
//    float	 ulSumPixSquN;
//    float	 lSumMeanPix;
//    float	 lSumMeanLin;
//    float	 lTemp;
//    float	 lPiTmp;
//    float	 lATmp;
//    float	 fPixSqu2;
//    float	 fLinSqu2;
//    float	 fA;
//    float	 fPi;
//    float	 fR;
//    float	 fTmp;
//    float	 fAnglNum;
//    int	 ulSumPix;
//    int	 ulSumLin;
//    int	 i,j,k;
//    int	 usCnt;
//    uint8_t	 ucPort;

//    usCnt = 0;
//    ulSumPix = 0;
//    ulSumLin = 0;
//    ulSumPixSqu = 0;
//    ulSumPixLin = 0;
//    lSumMeanPix = 0;
//    lSumMeanLin = 0;
//    ulSumMeanPixSqu = 0;
//    ulSumMeanLinSqu = 0;
//    memset((void *)gausPix, 0x00, sizeof(gausPix));
//    memset((void *)gausLin, 0x00, sizeof(gausLin));

//    for (j = usLinStr; j < (usLinLen + usLinStr); j++)
//    {
//        for (i = usPixStr; i < (usPixLen + usPixStr); i++)
//        {
//            ucPort = pucInputData[j*PIXELS_NUM + i];

//            //二值图
//            if (BLACK == ucPort)
//            {
//                ulSumPix += i;
//                ulSumLin += j;
//                ulSumPixSqu += (int)i * i;
//                ulSumPixLin += (int)i * j;

//                gausPix[usCnt] = i;
//                gausLin[usCnt] = j;

//                usCnt += 1;
//                usCnt &= 0x0fff;
//            }
//        }
//    }

//    ulMeanPix = (float)ulSumPix/usCnt;
//    ulMeanLin = (float)ulSumLin/usCnt;

//    for(k = 0; k < usCnt; k++)
//    {
//        lSumMeanPix += (float)(gausPix[k] - ulMeanPix);
//        lSumMeanLin += (float)(gausLin[k] - ulMeanLin);
//        ulSumMeanPixSqu += (float)((gausPix[k] - ulMeanPix)*(gausPix[k] - ulMeanPix));
//        ulSumMeanLinSqu += (float)((gausPix[k] - ulMeanLin)*(gausPix[k] - ulMeanLin));
//    }

//    fPixSqu2 = sqrtf(ulSumMeanPixSqu);
//    fLinSqu2 = sqrtf(ulSumMeanLinSqu);

//    ulSumPixSumPix = (float)ulSumPix * ulSumPix;
//    ulSumPixSquN = (float)usCnt * ulSumPixSqu;
//    lTemp = (float)(ulSumPixSquN - ulSumPixSumPix);
//    lATmp = (float)((float)ulSumLin * ulSumPixSqu - (float)ulSumPix * ulSumPixLin);
//    lPiTmp = (float)((float)usCnt * ulSumPixLin - (float)ulSumPix * ulSumLin);
//    fPi = (float)lPiTmp/lTemp;
//    fA = (float)lATmp/lTemp;
//    gstHoughTransValue1.fFitPiHori = fPi;
//    gstHoughTransValue1.fFitAHori = fA;

//    fTmp = atan(fPi);
//    fAnglNum = RADIAN_TO_ANGLE * fTmp;

//    gstHoughTransValue1.sAnglNumHori = (int)(fAnglNum*10);
//    gstHoughTransValue1.fRadianHori = fTmp;

//    fR = (float)(lSumMeanPix * lSumMeanLin)/(fPixSqu2 * fLinSqu2);
//    gstHoughTransValue1.fFitRHori = fR;

//}

/////////////////////// end //////////////////////////


