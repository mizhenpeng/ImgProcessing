#ifndef SCANNER_H
#define SCANNER_H


#include "globaldefine.h"

extern  uint8_t DetectedRingsNum;

extern  snake_detectedRing detectedRings[4];

void FuncRingsDetect(const uint8_t * pucDataStrAddr,int pixStart,int pixEnd,int linStart,int linEnd);

////////////////////////变量声明///////////////////////
//extern uint8_t gucTwoValuThrld;
//extern uint8_t gucRndDetCnt;
//extern HOUGH_AREA_VALU_STR  gstHoughTransValue1;
//extern HOUGH_AREA_VALU_STR  gstHoughTransValue2;
//extern HOUGH_POS_STR        gstHoughPos;
//extern DETC_RING_POS_STR    gastRingPosDet[4];
//extern uint8_t  gucDetRingNum;
//extern DETC_RING_ABC_STR    gstRingPos;
//extern DETC_RING_CEN_STR    gstRingCen;
//extern RING_ROT_CEN_STR     gstRingRotCen;
//extern int	gausPix[4096];
//extern int	gausLin[4096];



//extern uint8_t   guacRecvYBuf[720*588];
//extern uint8_t   guacRecvCbBuf[360*588];
//extern uint8_t   guacRecvCrBuf[360*588];

//extern uint8_t   guacPing0YBuf[720*588];
//extern uint8_t   guacPing0CbBuf[360*588];
//extern uint8_t   guacPing0CrBuf[360*588];

//extern uint8_t   guacPing1YBuf[720*588];
//extern uint8_t   guacPing1CbBuf[360*588];
//extern uint8_t   guacPing1CrBuf[360*588];

//extern uint8_t   guacPang0YBuf[720*588];
//extern uint8_t   guacPang0CbBuf[360*588];
//extern uint8_t   guacPang0CrBuf[360*588];

//extern uint8_t   guacPang1YBuf[720*588];
//extern uint8_t   guacPang1CbBuf[360*588];
//extern uint8_t   guacPang1CrBuf[360*588];

//extern uint8_t   guacL2DataBufPing[720*16];
//extern uint8_t   guacL2DataBufPang[720*16];
//extern uint8_t   guacL2DataBufPingOut[720*16];
//extern uint8_t   guacL2DataBufPangOut[720*16];


////////////////////////函数声明///////////////////////

//void FuncPing0BufInit(void);


//void FuncRingDetAll(const uint8_t * pucDataStrAddr,
//                          int  usPixels,
//                          int  usLines);
//void FuncRingsDetect(void);

//void FuncRingPosDetc(void);

//void FuncGetRingCen(void);
//void FuncRotate(const uint8_t * pucInputData,   /* Input data   */
//                    uint8_t       * pucOutputData,  /* Output data  */
//                    float  fAngle);
//void FuncRotateLitl(const uint8_t * pucInputData,   /* Input data   */
//                         uint8_t       * pucOutputData,  /* Output data  */
//                         float  fAngle);
//uint8_t FuncPortRotate(int  usPix,
//                          int  usLin,
//                          float   fAngle,
//                          int* pusRotPix,
//                          int* pusRotLin);
//uint8_t GetRingRotCen(float  fAngle);



//void FuncLeastSquFitVert(const uint8_t * pucInputData,   /* Input data   */
//                                int      usPixStr,
//                                int      usPixLen,
//                                int      usLinStr,
//                                int      usLinLen);
//void FuncLeastSquFitHori(const uint8_t * pucInputData,   /* Input data   */
//                                int      usPixStr,
//                                int      usPixLen,
//                                int      usLinStr,
//                                int      usLinLen);

//inline void FuncCosSin(int  sInputAngle,
//                            int  *psOutputCos,
//                            int  *psOutputSin);





#endif // SCANNER_H
