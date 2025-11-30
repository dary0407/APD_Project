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

#define  FRQ_PANEL                        1       /* callback function: OnFrequencyPanelCB */
#define  FRQ_PANEL_IDC_SWITCH_PANEL       2       /* control type: binary, callback function: OnSwitchPanelCB */

#define  MAIN_PANEL                       2       /* callback function: OnPanelCB */
#define  MAIN_PANEL_IDC_SWITCH_PANEL      2       /* control type: binary, callback function: OnSwitchPanelCB */
#define  MAIN_PANEL_COMMANDBUTTON         3       /* control type: command, callback function: LoadWaveCB */
#define  MAIN_PANEL_GRAPH_RAW             4       /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_GRAPH_FILTERED        5       /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_BTN_APPLY             6       /* control type: command, callback function: OnApplyFilterCB */
#define  MAIN_PANEL_BTN_NEXT              7       /* control type: command, callback function: NextBtnCB */
#define  MAIN_PANEL_BTN_PREV              8       /* control type: command, callback function: PrevBtnCB */
#define  MAIN_PANEL_NUM_START             9       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_STOP              10      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_RING_FILTER           11      /* control type: ring, callback function: OnFilterCB */
#define  MAIN_PANEL_NUM_KURTOSIS          12      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_SKEWNESS          13      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_MEDIANA           14      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_ZCROSS            15      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_AVG               16      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_DISPERSIE         17      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_MAX_IDX           18      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_MAX               19      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_MIN_IDX           20      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_MIN               21      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_TEXTMSG               22      /* control type: textMsg, callback function: (none) */
#define  MAIN_PANEL_NUM_ALPHA             23      /* control type: numeric, callback function: OnAlphaChangeCB */
#define  MAIN_PANEL_NUM_DIMENSION         24      /* control type: numeric, callback function: OnDimensionChangeCB */
#define  MAIN_PANEL_GRAPH_HISTO           25      /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_RESET_BUTTON          26      /* control type: command, callback function: OnResetBtnCB */
#define  MAIN_PANEL_CHECKBOX              27      /* control type: radioButton, callback function: OnDerivativeToggleCB */
#define  MAIN_PANEL_CHECKBOX_2            28      /* control type: radioButton, callback function: OnEnvelopeToggleCB */


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
int  CVICALLBACK OnFrequencyPanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnResetBtnCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSwitchPanelCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PrevBtnCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif