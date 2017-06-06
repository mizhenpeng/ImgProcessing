#ifndef PROCESSING_H
#define PROCESSING_H

#include "DataBank.h"
#include "binarize.h"
#include "Scanner.h"
#include "Decoder.h"


extern uint8_t grayImageData[IMG_WIDTH*IMG_HEIGHT];     // 原始灰度图像
extern uint8_t binaryImageData[IMG_WIDTH*IMG_HEIGHT];   // 二值化后图像
extern uint8_t rotatedImageData[IMG_WIDTH*IMG_HEIGHT];  // 存储旋转后图像的数据池，后续旋转操作可能并不需要对所有的数据进行旋转



void FuncDataInit(void);

void FuncProcessing(void);


//void FuncGet2DCode(const uint8_t * pucInputData1,
//                           const uint8_t * pucInputData0);


#endif // PROCESSING_H
