#ifndef DATABANK_H
#define DATABANK_H

#include "globaldefine.h"


extern uint8_t grayImageData[IMG_WIDTH*IMG_HEIGHT];     // 原始灰度图像
extern uint8_t binaryImageData[IMG_WIDTH*IMG_HEIGHT];   // 二值化后图像
extern uint8_t rotatedImageData[IMG_WIDTH*IMG_HEIGHT];  // 存储旋转后图像的数据池，后续旋转操作可能并不需要对所有的数据进行旋转

extern  uint8_t  DetectedRingsNum;

extern  snake_detectedRing detectedRings[4];



//extern uint8_t   guacRecvYBuf[720*588];
//extern uint8_t   guacRecvCbBuf[360*588];
//extern uint8_t   guacRecvCrBuf[360*588];

//extern uint8_t   guacPing0YBuf[720*588];
//extern uint8_t   guacPing0CbBuf[360*588];
//extern uint8_t   guacPing0CrBuf[360*588];

//extern uint8_t   guacPing1YBuf[720*588];
//extern uint8_t   guacPing1CbBuf[360*588];
//extern uint8_t   guacPing1CrBuf[360*588];


//extern uint8_t   guacL2DataBufPing[720*16];
//extern uint8_t   guacL2DataBufPang[720*16];
//extern uint8_t   guacL2DataBufPingOut[720*16];
//extern uint8_t   guacL2DataBufPangOut[720*16];

//extern uint8_t   gaucDecodeData[DECODE_DATA_LEN];
//extern uint8_t   gaucDecodeResult[DECODE_RESULT_LEN];


#endif // DATABANK_H
