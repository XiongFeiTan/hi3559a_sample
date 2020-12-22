#ifndef __SVP_SAMPLE_WK_H__
#define __SVP_SAMPLE_WK_H__

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <utility>

#include "hi_type.h"
#include "hi_nnie.h"

#define STRIDE_ALIGN  (16)
#define HI_PI (3.1415926535897932384626433832795)

#define SVP_SAMPLE_MAX(a,b)    (((a) > (b)) ? (a) : (b))
#define SVP_SAMPLE_MIN(a,b)    (((a) < (b)) ? (a) : (b))

#define SVP_SAMPLE_ALIGN32(addr) ((((addr) + 32 - 1)/32)*32)
#define SVP_SAMPLE_ALIGN16(addr) ((((addr) + 16 - 1)/16)*16)

#ifndef SVP_WK_PROPOSAL_WIDTH
#define SVP_WK_PROPOSAL_WIDTH (6)
#endif

#ifndef SVP_WK_COORDI_NUM
#define SVP_WK_COORDI_NUM (4)
#endif

#ifndef SVP_WK_SCORE_NUM
#define SVP_WK_SCORE_NUM (2)
#endif

#ifndef SVP_WK_QUANT_BASE
#define SVP_WK_QUANT_BASE  (0x1000)
#endif

using namespace std;

/*SVP_SAMPLE_FILE_NAME_PAIR first:
 * basic filename, second: filename suffix
 * pair<string, string>
 * */
typedef pair<string, string> SVP_SAMPLE_FILE_NAME_PAIR;

//
typedef struct hiSVP_WK_PARAM_RUNONCE_S
{
    // those below param is shared by all segments in one net.
    //整个网络的所有segments共享的参数，会贯穿整个网络生命周期
    SVP_MEM_INFO_S   stTmpBuf;
    HI_U32 u32TmpBufSize;

    SVP_NNIE_MODEL_S stModel;
    SVP_MEM_INFO_S   stModelBuf;
    HI_U32 u32ModelBufSize;

    // those below param is owned by individual segment.
    //每个segment自己独自拥有的参数
    SVP_MEM_INFO_S astTskBuf[SVP_NNIE_MAX_NET_SEG_NUM];
    HI_U32 au32TaskBufSize[SVP_NNIE_MAX_NET_SEG_NUM];

    SVP_SRC_BLOB_S stSrc[SVP_NNIE_MAX_INPUT_NUM];
    SVP_DST_BLOB_S stDst[SVP_NNIE_MAX_OUTPUT_NUM];
    SVP_BLOB_S stRPN[SVP_NNIE_MAX_OUTPUT_NUM];

    SVP_NNIE_FORWARD_CTRL_S stCtrl[SVP_NNIE_MAX_NET_SEG_NUM];
    SVP_NNIE_FORWARD_WITHBBOX_CTRL_S stBboxCtrl[SVP_NNIE_MAX_NET_SEG_NUM];
}SVP_WK_PARAM_RUNONECE_S;

//config参数
typedef struct hiSVP_WK_CFG_S
{
    const HI_CHAR *pszModelName;//模型名称
    const HI_CHAR *pszPicList;//图片列表

    HI_U32 u32MaxInputNum;//最大输入数量
    HI_U32 u32MaxBboxNum;//最大bbox数量

    HI_U32 u32TopN;//算法中的topN
}SVP_WK_CFG_S;

//分类结果结构体，参数：类别，置信度
typedef struct hiSVP_SAMPLE_CLF_RES_S
{
    HI_U32   u32ClassId;
    HI_U32   u32Confidence;
}SVP_SAMPLE_CLF_RES_S;

//一阶段网络
typedef struct hiSVP_NNIE_ONE_SEG_S
{
    HI_U32 u32TotalImgNum;
    FILE *fpSrc[SVP_NNIE_MAX_INPUT_NUM];
    FILE *fpLabel[SVP_NNIE_MAX_OUTPUT_NUM];

    HI_U32 u32ModelBufSize;
    HI_U32 u32TmpBufSize;

    SVP_NNIE_MODEL_S    stModel;
    SVP_MEM_INFO_S      stModelBuf;
    SVP_MEM_INFO_S      stTmpBuf;

    SVP_MEM_INFO_S      stTskBuf;
    HI_U32 u32TaskBufSize;

    SVP_SRC_BLOB_S astSrc[SVP_NNIE_MAX_INPUT_NUM];
    SVP_DST_BLOB_S astDst[SVP_NNIE_MAX_OUTPUT_NUM];

    SVP_NNIE_FORWARD_CTRL_S stCtrl;

    //memory needed by post-process of getting topN
    SVP_SAMPLE_CLF_RES_S *pstMaxClfIdScore;
    SVP_SAMPLE_CLF_RES_S *pastClfRes[SVP_NNIE_MAX_OUTPUT_NUM];
    HI_U32 au32ClfNum[SVP_NNIE_MAX_OUTPUT_NUM];
}SVP_NNIE_ONE_SEG_S;

typedef struct hiSVP_SAMPLE_RESULT_MEM_HEAD_S
{
    HI_U32 u32Type;
    HI_U32 u32Len;
    /* HI_U32* pu32Mem; */
}SVP_SAMPLE_RESULT_MEM_HEAD_S;

typedef struct hiSVP_NNIE_ONE_SEG_DET_S
{
    HI_U32 u32TotalImgNum;
    FILE *fpSrc[SVP_NNIE_MAX_INPUT_NUM];
    FILE *fpLabel[SVP_NNIE_MAX_OUTPUT_NUM];

    HI_U32 u32ModelBufSize;
    HI_U32 u32TmpBufSize;

    SVP_NNIE_MODEL_S    stModel;
    SVP_MEM_INFO_S      stModelBuf;
    SVP_MEM_INFO_S      stTmpBuf;

    SVP_MEM_INFO_S      stTskBuf;
    HI_U32 u32TaskBufSize;

    SVP_SRC_BLOB_S astSrc[SVP_NNIE_MAX_INPUT_NUM];
    SVP_DST_BLOB_S astDst[SVP_NNIE_MAX_OUTPUT_NUM];

    SVP_NNIE_FORWARD_CTRL_S stCtrl;

    //memory needed by post-process of getting detection result
    HI_S32 *ps32ResultMem;
}SVP_NNIE_ONE_SEG_DET_S;

typedef struct hiSVP_NNIE_MULTI_SEG_S
{
    HI_U32 u32TotalImgNum;
    FILE *fpSrc[SVP_NNIE_MAX_INPUT_NUM];
    FILE *fpLabel[SVP_NNIE_MAX_OUTPUT_NUM];

    HI_U32 u32ModelBufSize;
    HI_U32 u32TmpBufSize;

    SVP_NNIE_MODEL_S    stModel;
    SVP_MEM_INFO_S      stModelBuf;
    SVP_MEM_INFO_S      stTmpBuf;

    // those below param is owned by individual segment.
    SVP_MEM_INFO_S      astTskBuf[SVP_NNIE_MAX_NET_SEG_NUM];
    HI_U32 au32TaskBufSize[SVP_NNIE_MAX_NET_SEG_NUM];

    SVP_SRC_BLOB_S astSrc[SVP_NNIE_MAX_INPUT_NUM];
    SVP_DST_BLOB_S astDst[SVP_NNIE_MAX_OUTPUT_NUM];
    SVP_BLOB_S stRPN[SVP_NNIE_MAX_OUTPUT_NUM];

    SVP_NNIE_FORWARD_CTRL_S astCtrl[SVP_NNIE_MAX_NET_SEG_NUM];
    SVP_NNIE_FORWARD_WITHBBOX_CTRL_S astBboxCtrl[SVP_NNIE_MAX_NET_SEG_NUM];
}SVP_NNIE_MULTI_SEG_S;

typedef struct hiSVP_NNIE_CFG_S
{
    const HI_CHAR *pszModelName;
    const HI_CHAR *paszPicList[SVP_NNIE_MAX_INPUT_NUM];
    const HI_CHAR *paszLabel[SVP_NNIE_MAX_OUTPUT_NUM];

    HI_U32 u32MaxInputNum;
    HI_U32 u32MaxBboxNum;

    HI_U32 u32TopN;
    HI_BOOL bNeedLabel;
}SVP_NNIE_CFG_S;

typedef struct hiSVP_NNIE_NODE_INFO
{
    HI_CHAR layerName[SVP_NNIE_NODE_NAME_LEN];
    HI_U32 segID;
    HI_U32 layerID;
    HI_U32 dstIdx;
}SVP_NNIE_NODE_INFO;

typedef enum hiSVP_SAMPLE_WK_CLF_NET_TYPE_E
{
    SVP_SAMPLE_WK_CLF_NET_LENET         = 0x0,  /*LeNet*/
    SVP_SAMPLE_WK_CLF_NET_ALEXNET       = 0x1,  /*Alexnet*/
    SVP_SAMPLE_WK_CLF_NET_VGG16         = 0x2,  /*Vgg16*/
    SVP_SAMPLE_WK_CLF_NET_GOOGLENET     = 0x3,  /*Googlenet*/
    SVP_SAMPLE_WK_CLF_NET_RESNET50      = 0x4,  /*Resnet50*/
    SVP_SAMPLE_WK_CLF_NET_SQUEEZENET    = 0x5,  /*Squeezenet*/
    SVP_SAMPLE_WK_CLF_NET_MOBILENET     = 0x6,  /*Mobilenet*/

    SVP_SAMPLE_WK_CLF_NET_TYPE_BUTT
}SVP_SAMPLE_WK_CLF_NET_TYPE_E;

typedef enum hiSVP_SAMPLE_WK_DETECT_NET_TYPE_E
{
    SVP_SAMPLE_WK_DETECT_NET_YOLOV1   =  0x0,  /*Yolov1*/
    SVP_SAMPLE_WK_DETECT_NET_YOLOV2   =  0x1,  /*Yolov2*/
    SVP_SAMPLE_WK_DETECT_NET_YOLOV3   =  0x2,  /*Yolov3*/
    SVP_SAMPLE_WK_DETECT_NET_SSD      =  0x3,  /*Ssd*/

    SVP_SAMPLE_WK_DETECT_NET_TYPE_BUTT
}SVP_SAMPLE_WK_DETECT_NET_TYPE_E;

typedef enum hiSVP_SAMPLE_WK_DETECT_NET_FASTER_RCNN_TYPE_E
{
    SVP_SAMPLE_WK_DETECT_NET_FASTER_RCNN_ALEX   =  0x0,  /*fasterrcnn_alexnet*/
    SVP_SAMPLE_WK_DETECT_NET_FASTER_RCNN_VGG16,          /*fasterrcnn_vgg16*/
    SVP_SAMPLE_WK_DETECT_NET_FASTER_RCNN_RES18,          /*fasterrcnn_resnet18*/
    SVP_SAMPLE_WK_DETECT_NET_FASTER_RCNN_RES34,          /*fasterrcnn_resnet34*/
    SVP_SAMPLE_WK_DETECT_NET_FASTER_RCNN_PVANET,         /*fasterrcnn_pvanet*/
    SVP_SAMPLE_WK_DETECT_NET_FASTER_RCNN_DOUBLE_ROI,     /*fasterrcnn_double_roi*/

    SVP_SAMPLE_WK_DETECT_NET_FASTER_RCNN_TYPE_BUTT
}SVP_SAMPLE_WK_DETECT_NET_FASTER_RCNN_TYPE_E;

typedef enum hiSVP_SAMPLE_WK_DETECT_NET_RFCN_TYPE_E
{
    SVP_SAMPLE_WK_DETECT_NET_RFCN_RES50   =  0x0,

    SVP_SAMPLE_WK_DETECT_NET_RFCN_TYPE_BUTT
}SVP_SAMPLE_WK_DETECT_NET_RFCN_TYPE_E;

typedef struct _LSTMRunTimeCtx
{
    HI_U32 *pu32Seqs;
    HI_U32 u32SeqNr;
    HI_U32 u32MaxT;
    HI_U32 u32TotalT;
    HI_U8 u8ExposeHid;
    HI_U8 u8WithStatic;
} SVP_SAMPLE_LSTMRunTimeCtx;

/* classification relative functions */
/* classification with input images and labels, print the top-N result */
HI_S32 SvpSampleCnnClassification(const HI_CHAR *pszModelName, const HI_CHAR *paszPicList[], const HI_CHAR *paszLabel[], HI_S32 s32Cnt=1);
HI_S32 SvpSampleCnnClassificationForword(SVP_NNIE_ONE_SEG_S *pstClfParam, SVP_NNIE_CFG_S *pstClfCfg);
/* One-Segment cnn net mem init */
HI_S32 SvpSampleOneSegCnnInit(SVP_NNIE_CFG_S *pstClfCfg, SVP_NNIE_ONE_SEG_S *pstComParam);
/* One-Segment cnn net mem deinit */
void SvpSampleOneSegCnnDeinit(SVP_NNIE_ONE_SEG_S *pstComParam);

/* detection relative functions */
/* Base-Anchor information initialize */
HI_S32 SvpSampleFasterRCNNAnchorInfoInit(SVP_SAMPLE_WK_DETECT_NET_FASTER_RCNN_TYPE_E netType,
    void* pBaseAnchorInfo);
/* Multi-Segment detection Faster-Rcnn */
HI_S32 SvpSampleWKFasterRCNNRun(const HI_CHAR *pszModel, const HI_CHAR *paszPicList[],
    HI_U8 netType, HI_U32 *pu32DstAlign, HI_S32 s32Cnt=1);
/* Multi-Segment detection Rfcn */
HI_S32 SvpSampleWKRFCNRun(const HI_CHAR *pszModel, const HI_CHAR *paszPicList[],
    HI_U32 *pu32DstAlign,  HI_S32 s32Cnt=1);
/* Multi-Segment net init */
HI_S32 SvpSampleMultiSegCnnInit(SVP_NNIE_CFG_S *pstComCfg, SVP_NNIE_MULTI_SEG_S *pstComParam,
    HI_U32 *pu32SrcAlign = NULL, HI_U32 *pu32DstAlign = NULL);
/* Multi-Segment net deinit */
void SvpSampleMultiSegCnnDeinit(SVP_NNIE_MULTI_SEG_S *pstComParam);

/* One-Segment detection relative functions */
HI_S32 SvpSampleCnnDetectionOneSeg(const HI_CHAR *pszModelName, const HI_CHAR *paszPicList[], const HI_U8 netType, HI_S32 s32Cnt = 1);

HI_S32 SvpSampleCnnDetectionForword(SVP_NNIE_ONE_SEG_DET_S *pstDetParam, SVP_NNIE_CFG_S *pstDetCfg);
/* One-Segment det net init */
HI_S32 SvpSampleOneSegDetCnnInit(SVP_NNIE_CFG_S *pstClfCfg, SVP_NNIE_ONE_SEG_DET_S *pstComfParam,const HI_U8 netType);
/* One-Segment det net  deinit */
void SvpSampleOneSegDetCnnDeinit(SVP_NNIE_ONE_SEG_DET_S *pstComParam);

/* Segmentation */
HI_S32 SvpSampleSegnet(const HI_CHAR *pszModelName, const HI_CHAR *paszPicList[], HI_S32 s32Cnt = 1);

/* RNN relative functions */
#define LSTM_UT_EXPOSE_HID 2
#define LSTM_UT_WITH_STATIC 1
void SvpSampleCreateLSTMCtx(SVP_SAMPLE_LSTMRunTimeCtx *pstCtx, HI_U32 u32SentenceNr, HI_U32 u32BaseFrameNr,
    HI_U8 u8ExposeHid, HI_U8 u8WithStatic);
void SvpSampleDestoryLSTMCtx(SVP_SAMPLE_LSTMRunTimeCtx *pstCtx);
HI_S32 SvpSampleWkLSTM(const HI_CHAR *pszModelName, const HI_CHAR *pszPicList[], HI_U32 u32PicListNum,
    HI_U32 *pu32SrcStride, HI_U32 *pu32DstStride, SVP_SAMPLE_LSTMRunTimeCtx *pstCtx = NULL);

HI_S32 SvpSampleLSTMInit(SVP_NNIE_CFG_S *pstClfCfg, SVP_NNIE_ONE_SEG_S *pstComParam, SVP_SAMPLE_LSTMRunTimeCtx *pstCtx);
HI_S32 SvpSampleLSTMDeinit(SVP_NNIE_ONE_SEG_S *pstComParam);

/************** run sample *******************/

/*Classificacion*/
void SvpSampleCnnClfLenet();
void SvpSampleCnnClfAlexnet();
void SvpSampleCnnClfVgg16();
void SvpSampleCnnClfGooglenet();
void SvpSampleCnnClfResnet50();
void SvpSampleCnnClfSqueezenet();
void SvpSampleCnnClfMobilenet();

/*Detection*/
void SvpSampleRoiDetFasterRCNNAlexnet();
void SvpSampleRoiDetFasterRCNNVGG16();
void SvpSampleRoiDetFasterRCNNResnet18();
void SvpSampleRoiDetFasterRCNNResnet34();
void SvpSampleRoiDetFasterRCNNPvanet();
void SvpSampleRoiDetFasterRCNNDoubleRoi();
void SvpSampleRoiDetRFCNResnet50();
void SvpSampleCnnDetYoloV1();
void SvpSampleCnnDetYoloV2();
void SvpSampleCnnDetYoloV3();
void SvpSampleCnnDetSSD();

/*Segmentation*/
void SvpSampleCnnFcnSegnet();

/*LSTM*/
void SvpSampleRecurrentLSTMFC();
void SvpSampleRecurrentLSTMRelu();

#endif //__SVP_SAMPLE_WK_H__
