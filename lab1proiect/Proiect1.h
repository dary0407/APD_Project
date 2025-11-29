/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  MAIN_PANEL                       1       /* callback function: OnPanelCB */
#define  MAIN_PANEL_COMMANDBUTTON         2       /* control type: command, callback function: LoadWaveCB */
#define  MAIN_PANEL_GRAPH_RAW             3       /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_GRAPH_FILTERED        4       /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_BTN_APPLY             5       /* control type: command, callback function: OnApplyFilterCB */
#define  MAIN_PANEL_BTN_NEXT              6       /* control type: command, callback function: NextBtnCB */
#define  MAIN_PANEL_BTN_PREV              7       /* control type: command, callback function: PrevBtnCB */
#define  MAIN_PANEL_NUM_START             8       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_STOP              9       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_RING_FILTER           10      /* control type: ring, callback function: OnFilterCB */
#define  MAIN_PANEL_NUM_KURTOSIS          11      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_SKEWNESS          12      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_MEDIANA           13      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_ZCROSS            14      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_AVG               15      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_DISPERSIE         16      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_MAX_IDX           17      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_MAX               18      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_MIN_IDX           19      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_MIN               20      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_TEXTMSG               21      /* control type: textMsg, callback function: (none) */
#define  MAIN_PANEL_NUM_ALPHA             22      /* control type: numeric, callback function: OnAlphaChangeCB */
#define  MAIN_PANEL_NUM_DIMENSION         23      /* control type: numeric, callback function: OnDimensionChangeCB */
#define  MAIN_PANEL_GRAPH_HISTO           24      /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_RESET_BUTTON          25      /* control type: command, callback function: OnResetBtnCB */
#define  MAIN_PANEL_CHECKBOX              26      /* control type: radioButton, callback function: OnDerivativeToggleCB */
#define  MAIN_PANEL_CHECKBOX_2            27      /* control type: radioButton, callback function: OnEnvelopeToggleCB */


     /* Control Arrays: */

#define  CTRLARRAY                        1

     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK LoadWaveCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK NextBtnCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnAlphaChangeCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnApplyFilterCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnDerivativeToggleCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnDimensionChangeCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnEnvelopeToggleCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFilterCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnResetBtnCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PrevBtnCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif