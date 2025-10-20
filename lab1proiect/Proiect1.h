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
#define  MAIN_PANEL_BTN_NEXT              6       /* control type: command, callback function: (none) */
#define  MAIN_PANEL_BTN_PREV              7       /* control type: command, callback function: (none) */
#define  MAIN_PANEL_NUM_START             8       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_STOP              9       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_RING_FILTER           10      /* control type: ring, callback function: OnFilterCB */
#define  MAIN_PANEL_NUM_KURTOSIS          11      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_SKEWNESS          12      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_HISTOGRAM         13      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_MEDIANA           14      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_ZCROSS            15      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_AVG               16      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_DISPERSIE         17      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_MAX_IDX           18      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_MAX               19      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_MIN_IDX           20      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_MIN               21      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_TEXTMSG               22      /* control type: textMsg, callback function: (none) */
#define  MAIN_PANEL_NUM_ALPHA             23      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_DIMENSION         24      /* control type: numeric, callback function: (none) */


     /* Control Arrays: */

#define  CTRLARRAY                        1

     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK LoadWaveCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnApplyFilterCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFilterCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif