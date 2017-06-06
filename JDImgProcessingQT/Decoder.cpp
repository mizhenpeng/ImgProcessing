#include "Decoder.h"




///********************************************
//函数功能:FuncDataGet函数实现
//函数输出:
//*********************************************/
//void FuncDataGet(const uint8_t * pucInputDataY,   /* Input data   */
//                      const uint8_t * pucInputDataCb,   /* Input data   */
//                      const uint8_t * pucInputDataCr,   /* Input data   */
//                      uint8_t	    * pucOutputDataY,   /* Output data  */
//                      uint8_t       * pucOutputDataCb,  /* Output data  */
//                      uint8_t       * pucOutputDataCr,  /* Output data  */
//                      int      usNumLine,
//                      int      usNumPixel)
//{
//    int  i;
//    int  usNum12Line;
//    //int  usNum12Pixel;

//    usNum12Line = usNumLine >> 1;
//    //usNum12Pixel = usNumPixel >> 1;

//    /*打开一个数据拷贝的数据通路*/
//    //DAT_open(DAT_CHA0, DAT_PRI_LOW, DAT_OPEN_2D);

//    for(i = 0; i < usNum12Line; i++)
//    {
//        /*传送临时Y缓冲区*/
//        memcpy((void *)(pucOutputDataY + 2 * i * usNumPixel),
//               (void *)(pucInputDataY + i * usNumPixel),
//                 usNumPixel);

//    }

//    for(i = usNum12Line; i < usNumLine; i++)
//    {
//        /*传送临时Y缓冲区*/
//        memcpy((void *)(pucOutputDataY + (2 * (i - usNum12Line) + 1) * usNumPixel),
//               (void *)(pucInputDataY + i * usNumPixel),
//                 usNumPixel);

//    }

//    //DAT_wait(DAT_XFRID_WAITALL);

//}
///*********************************************************
//* 函数名称: GetHafByte
//* 函数输入:
//* 函数输入:
//**********************************************************/
//void GetHafByte(uint8_t  ucShift,
//                    uint8_t  (*pucInputBuff)[7],
//                    uint8_t  *pucOutputBuff)
//{
//    int   i;
//    uint8_t  ucTmp7Bit = 0;

//    for(i = 6; i > 2; i--)
//    {
//        ucTmp7Bit |= ((*(*(pucInputBuff + ucShift) + i))&1)<<(i-3);
//    }

//    *(pucOutputBuff + ucShift) = ucTmp7Bit;
//}
///*********************************************************
//* 函数名称: GetCodeData ,获取二维码数据
//* 函数输入:
//* 函数输入:
//**********************************************************/
//void GetCodeData(const uint8_t * pucInputData)
//{
//    int  usPix,usLin;
//    int  usCenPix;
//    int  usCenLin;
//    int  usPixStr;
//    int  usPixEnd;
//    int  usLinStr;
//    int  usLinEnd;
//    int  usTmpSum;
//    int  usTmpSum1, usTmpSum2, usTmpSum3;
//    int  usCnt;
//    uint8_t   aucValue[9];

//    usCnt = 0;
//    //gaucDecodeData
//    memset((void *)gaucDecodeData, 0xff, sizeof(gaucDecodeData));
//    // 计算中心
//    usCenPix = (gstRingRotCen.usRotCen0Pix + gstRingRotCen.usRotCen2Pix)>>1;
//    usCenLin = (gstRingRotCen.usRotCen0Lin + gstRingRotCen.usRotCen2Lin)>>1;

//    // 二维码矩形区域的四个顶点
//    usPixStr = usCenPix - PIX_LEN_HALF + PIX_LEN_HALF_GRID;
//    usPixEnd = usCenPix + PIX_LEN_HALF;// - PIX_LEN_HALF_GRID;
//    usLinStr = usCenLin - LIN_LEN_HALF + LIN_LEN_HALF_GRID;
//    usLinEnd = usCenLin + LIN_LEN_HALF;// - LIN_LEN_HALF_GRID;

//    // LIN_LEN_ONE_GRID 为垂直方向步进长度
//    for (usLin = usLinStr; usLin < usLinEnd; usLin += LIN_LEN_ONE_GRID)
//    {
//        // PIX_LEN_ONE_GRID 为水平方向步进长度
//        for (usPix = usPixStr; usPix < usPixEnd; usPix += PIX_LEN_ONE_GRID)
//        {
//            // 取3x3矩阵块
//            aucValue[0] = (*(uint8_t *)(pucInputData + (usLin - 2)*PIXELS_NUM + (usPix - 2)));
//            aucValue[1] = (*(uint8_t *)(pucInputData + (usLin - 2)*PIXELS_NUM + usPix));
//            aucValue[2] = (*(uint8_t *)(pucInputData + (usLin - 2)*PIXELS_NUM + (usPix + 2)));
//            aucValue[3] = (*(uint8_t *)(pucInputData + usLin*PIXELS_NUM + (usPix - 2)));
//            aucValue[4] = (*(uint8_t *)(pucInputData + usLin*PIXELS_NUM + usPix));
//            aucValue[5] = (*(uint8_t *)(pucInputData + usLin*PIXELS_NUM + (usPix + 2)));
//            aucValue[6] = (*(uint8_t *)(pucInputData + (usLin + 2)*PIXELS_NUM + (usPix - 2)));
//            aucValue[7] = (*(uint8_t *)(pucInputData + (usLin + 2)*PIXELS_NUM + usPix));
//            aucValue[8] = (*(uint8_t *)(pucInputData + (usLin + 2)*PIXELS_NUM + (usPix + 2)));

//            usTmpSum1 = aucValue[0] + aucValue[1] + aucValue[2];
//            usTmpSum2 = aucValue[3] + aucValue[4] + aucValue[5];
//            usTmpSum3 = aucValue[6] + aucValue[7] + aucValue[8];

//            usTmpSum = usTmpSum1 + usTmpSum2 + usTmpSum3;

//            if (usTmpSum > WHITE*6) // 如果至少有7个白块
//            {
//                gaucDecodeData[usCnt] = 0; // 编码0
//            }
//            else if (usTmpSum < WHITE*3) // 如果至多有2个白块
//            {
//                gaucDecodeData[usCnt] = 1; // 编码1
//            }
//            else
//            {
//                //
//            }
//            usCnt += 1;
//        }
//    }
//}
///*********************************************************
//* 函数名称: DeCode  ,获取二维码
//* 函数输入:
//* 函数输入:
//**********************************************************/

//void DeCode(uint8_t  *pucInputBuff,
//                uint8_t  ucInputLen,
//                uint8_t  *pucOutputBuff)
//{
//    uint8_t   i, j, k;
//    uint8_t  ucSum = 0;
//    uint8_t  aucBuff[20] = {0};
//    uint8_t  aucDeCodeOutPut[16][7];
//    uint8_t  aucPswitch[8];
//    uint8_t  aucSfollow[16][3];
//    const uint8_t  aucMonitorHTran[7][3] = {{1,1,1},
//                                          {1,1,0},
//                                          {1,0,1},
//                                          {0,1,1},
//                                          {1,0,0},
//                                          {0,1,0},
//                                          {0,0,1}};

//    memset((void *)gaucDecodeResult, 0x00, sizeof(gaucDecodeResult));

//    for(i = 0; i < ucInputLen; i++)
//    {
//        for(j = 0; j < 7; j++)
//        {
//            aucDeCodeOutPut[i][6 - j] = pucInputBuff[8*i + j];
//        }
//    }

//    for(i = 0; i < ucInputLen; i++)  //计算伴随式
//    {
//        for(k = 0; k < 3; k++)
//        {
//            for(j = 0; j < 7; j++)
//            {
//                ucSum += (aucDeCodeOutPut[i][6-j] * aucMonitorHTran[j][k]);
//            }

//            if (1 ==  (ucSum&1))
//            {
//                ucSum = 1;
//            }
//            else
//            {
//                ucSum = 0;
//            }

//            aucSfollow[i][2-k] = ucSum;
//            ucSum = 0;
//        }
//    }

//    for(i = 0; i < ucInputLen; i++)
//    {
//        aucPswitch[i] = 4*aucSfollow[i][2] + 2*aucSfollow[i][1] + aucSfollow[i][0] + 1;
//        switch(aucPswitch[i])    /*根据伴随式判断接收码，并译码*/
//        {
//            case 1:
//                 {
//                    GetHafByte(i,aucDeCodeOutPut,aucBuff);
//                     break;
//                 }
//            case 2:
//                {
//                     aucDeCodeOutPut[i][0]=1^aucDeCodeOutPut[i][0];
//                     GetHafByte(i,aucDeCodeOutPut,aucBuff);
//                     break;
//                }
//            case 3:
//                {
//                     aucDeCodeOutPut[i][1]=1^aucDeCodeOutPut[i][1];
//                     GetHafByte(i,aucDeCodeOutPut,aucBuff);
//                     break;
//                }
//            case 4:
//                {
//                     aucDeCodeOutPut[i][3]=1^aucDeCodeOutPut[i][3];
//                     GetHafByte(i,aucDeCodeOutPut,aucBuff);
//                     break;
//                }
//            case 5:
//                {
//                     aucDeCodeOutPut[i][2]=1^aucDeCodeOutPut[i][2];
//                     GetHafByte(i,aucDeCodeOutPut,aucBuff);
//                     break;
//                }
//            case 6:
//                {
//                     aucDeCodeOutPut[i][4]=1^aucDeCodeOutPut[i][4];
//                     GetHafByte(i,aucDeCodeOutPut,aucBuff);
//                     break;
//                }
//            case 7:
//                {
//                     aucDeCodeOutPut[i][5]=1^aucDeCodeOutPut[i][5];
//                     GetHafByte(i,aucDeCodeOutPut,aucBuff);
//                     break;
//                }
//            case 8:
//                {
//                     aucDeCodeOutPut[i][6]=1^aucDeCodeOutPut[i][6];
//                     GetHafByte(i,aucDeCodeOutPut,aucBuff);
//                     break;
//                }
//        }
//    }

//    for(i = 0; i < (ucInputLen>>1); i++)
//    {
//        *(pucOutputBuff + i) = aucBuff[i<<1] + (aucBuff[(i<<1)+1]<<4);
//    }

//}
