#include <utility.h>
#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include <analysis.h>
#include <formatio.h>
#include "toolbox.h"
#include "Proiect1.h"
#include <utility.h>
#include <stdlib.h>
#include <advanlys.h>


//==============================================================================
// Constants
#define SAMPLE_RATE		0
#define NPOINTS			1


#define USER_EVENT         1000
#define MYEVENT_CLOSE      (USER_EVENT+1)

//==============================================================================
// Global variables
int waveInfo[2]; //waveInfo[0] = sampleRate
				 //waveInfo[1] = number of elements
double sampleRate = 0.0;
int npoints = 0;
double *waveData = 0;
double *envelopeData = 0;
int filterApplied = 0;

int plotHandle = -1;
int plotHandle2 = -1;

int currentSecond = 0; //pt butoanele prev/next

int isFullView = 1;  // 1 = modul initial complet, 0 = modul pe secunde

					
static int mainPanel = 0;
static int frqPanel = 0;
int showDerivative = 0;   // 0 = normal, 1 = derivatã
int showEnvelope = 0;  // 0 = normal, 1 = afiseazã anvelopa

int allowAutoFilter = 0;   // 0 = nu aplica automat (pânã dupã primul "Aplica")


int main (int argc, char *argv[])
{
	int event = 0;
	int val1 = 0;
	int val2 = 0;
	BOOL quitFlag = FALSE;
	
	
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	
	if ((mainPanel = LoadPanel (0, "Proiect1.uir", MAIN_PANEL)) < 0)
		return -1;
	if ((frqPanel = LoadPanel (0, "Proiect1.uir", FRQ_PANEL)) < 0)
		return -1;
	DisplayPanel (mainPanel);
	while(!quitFlag)
	{
		event = GetUserEvent(1, &val1, &val2);
		switch(event)
		{
			case MYEVENT_CLOSE:
				quitFlag = TRUE;
			break;
		
		}
	}
	DiscardPanel (mainPanel);
	return 0;
}

//functie pentru a ordona valori duble
int CompareDouble(const void *a, const void *b)
{
    double A = *(const double*)a;
    double B = *(const double*)b;
    if (A < B) return -1;
    if (A > B) return 1;
    return 0;
}

double ComputeMedian(double *data, int npoints)
{
    if (data == NULL || npoints <= 0)
        return 0.0;

    double *copy = (double *)malloc(npoints * sizeof(double));
    if (!copy)
        return 0.0;

    // folosim valorile absolute
    for (int i = 0; i < npoints; i++)
        copy[i] = fabs(data[i]);

    qsort(copy, npoints, sizeof(double), CompareDouble);

    double median;
    if (npoints % 2 == 0)
        median = (copy[npoints / 2 - 1] + copy[npoints / 2]) / 2.0;
    else
        median = copy[npoints / 2];

    free(copy);
    return median;
}
void ComputeSkewKurt(double *x, int n, double *skew, double *kurt)
{
    double mean = 0.0, m2 = 0.0, m3 = 0.0, m4 = 0.0;
	//m2 - varianta * n
	//m3 - asimetria
	//m4 - aplatizarea

    for (int i = 0; i < n; i++)
        mean += x[i];
    mean /= n;
    for (int i = 0; i < n; i++)
    {
        double d = x[i] - mean;
        m2 += d*d;
        m3 += d*d*d;
        m4 += d*d*d*d;
    }
	//normalizare
    m2 /= n;  
    m3 /= n;
    m4 /= n;

    *skew = m3 / pow(m2, 1.5);
    *kurt = m4 / (m2 * m2);
}

void UpdateStats(int panel, double *segment, int segmentLength)
{
    double minVal, maxVal, mean, variance, median, skewness, kurtosis;
    int idxMin, idxMax, zeroCross = 0;
    int histValues[50];
    double histBins[50];

    MaxMin1D(segment, segmentLength, &maxVal, &idxMax, &minVal, &idxMin);
    Mean(segment, segmentLength, &mean);
    Variance(segment, segmentLength, &mean, &variance);
    median = ComputeMedian(segment, segmentLength);

    for (int i = 1; i < segmentLength; i++)
        if ((segment[i - 1] < 0 && segment[i] > 0) || (segment[i - 1] > 0 && segment[i] < 0))
            zeroCross++;

    ComputeSkewKurt(segment, segmentLength, &skewness, &kurtosis);

    SetCtrlVal(panel, MAIN_PANEL_NUM_MIN, minVal);
    SetCtrlVal(panel, MAIN_PANEL_NUM_MAX, maxVal);
    SetCtrlVal(panel, MAIN_PANEL_NUM_AVG, mean);
    SetCtrlVal(panel, MAIN_PANEL_NUM_DISPERSIE, variance);
    SetCtrlVal(panel, MAIN_PANEL_NUM_MEDIANA, median);
    SetCtrlVal(panel, MAIN_PANEL_NUM_ZCROSS, zeroCross);
    SetCtrlVal(panel, MAIN_PANEL_NUM_SKEWNESS, skewness);
    SetCtrlVal(panel, MAIN_PANEL_NUM_KURTOSIS, kurtosis);
	SetCtrlVal(panel, MAIN_PANEL_NUM_MIN_IDX, idxMin);
	SetCtrlVal(panel, MAIN_PANEL_NUM_MAX_IDX, idxMax);

    Histogram(segment, segmentLength, minVal, maxVal, histValues, histBins, 50);
    DeleteGraphPlot(panel, MAIN_PANEL_GRAPH_HISTO, -1, VAL_IMMEDIATE_DRAW);
    PlotXY(panel, MAIN_PANEL_GRAPH_HISTO, histBins, histValues, 50, VAL_DOUBLE, VAL_INTEGER,
           VAL_VERTICAL_BAR, VAL_SOLID_CIRCLE, VAL_SOLID, 1, VAL_RED);
}




int CVICALLBACK LoadWaveCB (int panel, int control, int event,
                            void *callbackData, int eventData1, int eventData2)
{
    if (event != EVENT_COMMIT)
        return 0;

    DeleteGraphPlot(panel, MAIN_PANEL_GRAPH_RAW, -1, VAL_IMMEDIATE_DRAW);
    DeleteGraphPlot(panel, MAIN_PANEL_GRAPH_FILTERED, -1, VAL_IMMEDIATE_DRAW);
    DeleteGraphPlot(panel, MAIN_PANEL_GRAPH_HISTO, -1, VAL_IMMEDIATE_DRAW);

    // 1) CITIRE WAVEINFO
    FileToArray("waveInfo.txt", waveInfo, VAL_INTEGER,
                2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);

    sampleRate = waveInfo[0];
    npoints    = waveInfo[1];
    // 2) LIMITARE AUTOMATÃ LA 6 SECUNDE
    double duration = (double)npoints / sampleRate;
    if (duration > 10.0)
        npoints = sampleRate * 6;

    // 3) ALOCARE & CITIRE waveData
    if (waveData) free(waveData);
    waveData = calloc(npoints, sizeof(double));

    FileToArray("waveData.txt", waveData, VAL_DOUBLE,
                npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);

    // 4) AFISARE GRAFIC RAW
    plotHandle = PlotY(panel, MAIN_PANEL_GRAPH_RAW,
                       waveData, npoints, VAL_DOUBLE,
                       VAL_THIN_LINE, VAL_EMPTY_SQUARE,
                       VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);

    // 5) CALCUL STATISTICI PE TOT SEMNALUL 
    UpdateStats(panel, waveData, npoints);
    // 6) HISTOGRAMA
    double minVal, maxVal;
    int idxMin, idxMax;
    double histBins[50];
    int histValues[50];

    MaxMin1D(waveData, npoints, &maxVal, &idxMax, &minVal, &idxMin);

    Histogram(waveData, npoints,
              minVal, maxVal,
              histValues, histBins, 50);

    PlotXY(panel, MAIN_PANEL_GRAPH_HISTO,
           histBins, histValues, 50,
           VAL_DOUBLE, VAL_INTEGER,
           VAL_VERTICAL_BAR, VAL_SOLID_CIRCLE,
           VAL_SOLID, 1, VAL_RED);

    SetCtrlAttribute(panel, MAIN_PANEL_GRAPH_HISTO,
                     ATTR_YMAP_MODE, VAL_LOG);

    SetAxisRange(panel, MAIN_PANEL_GRAPH_HISTO,
                 VAL_AUTOSCALE, VAL_AUTOSCALE,
                 VAL_AUTOSCALE, VAL_AUTOSCALE,
                 VAL_NO_CHANGE, VAL_NO_CHANGE);

    // 7) RESETARE BUTOANE
    isFullView    = 1;
    currentSecond = 0;
    filterApplied = 0;

    SetCtrlVal(panel, MAIN_PANEL_NUM_START, 0);
    SetCtrlVal(panel, MAIN_PANEL_NUM_STOP, (int)(npoints / sampleRate));

    // 8) PREGATESTE FILTRUL PENTRU AUTO-UPDATE
    allowAutoFilter = 0;

    return 0;
}


int CVICALLBACK OnPanelCB (int panel, int event, void *callbackData,
						   int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QueueUserEvent(MYEVENT_CLOSE, panel, MAIN_PANEL);

			break;
	}
	return 0;
}

//la fiecare punct se inlocuieste cu media valorilor din jur
void Filter_Mediere(double *input, double *output, int npoints, int windowSize)
{
    for (int i = 0; i < npoints; i++) {
        double sum = 0.0;
        int count = 0;
        for (int j = i - windowSize / 2; j <= i + windowSize / 2; j++) {
            if (j >= 0 && j < npoints) {
                sum += input[j];
                count++;
            }
        }
        output[i] = sum / count;
    }
}

//filtru de smoothing
void Filter_Ordin1(double *input, double *output, int npoints, double alpha)
{
    output[0] = input[0];
    for (int i = 1; i < npoints; i++) {
        output[i] = (1 - alpha) * output[i - 1] + alpha * input[i];
    }
}

//scoate in evidenta schimbarile rapide din semnal
void ComputeDerivative(double *input, double *output, int npoints, double dt)
{
    if (npoints <= 1) return;

    for (int i = 0; i < npoints - 1; i++) {
        output[i] = (input[i + 1] - input[i]) / dt;
    }
    output[npoints - 1] = output[npoints - 2];  
}



int CVICALLBACK OnApplyFilterCB (int panel, int control, int event,
                                 void *callbackData, int eventData1, int eventData2)
{
    int filterType;
    double alpha;
    int windowSize;
	int color = showEnvelope ? VAL_GREEN : VAL_BLUE;

    if (waveData == NULL || npoints <= 0)
        return 0;   

    switch (event)
    {
        case EVENT_COMMIT:
        {
			allowAutoFilter = 1;
            int startIndex, endIndex, segmentLength;

            if (isFullView == 1) {
                startIndex = 0;
                endIndex   = npoints;
            } else {
                startIndex = currentSecond * (int)sampleRate;
                endIndex   = startIndex + (int)sampleRate;
                if (endIndex > npoints)
                    endIndex = npoints;
            }

            segmentLength = endIndex - startIndex;
            if (segmentLength <= 0)
                return 0;

            double *filtered = calloc(segmentLength, sizeof(double));
            if (!filtered)
                return 0;

            GetCtrlVal(panel, MAIN_PANEL_RING_FILTER, &filterType);

			if (showDerivative == 1)
			{
			    double dt = 1.0 / sampleRate;
			    ComputeDerivative(&waveData[startIndex], filtered, segmentLength, dt);
			}
			else if (showEnvelope == 1)
			{
				if (envelopeData == NULL) {
			        free(filtered);
			        return 0;
			    }
				
			    memcpy(filtered, &envelopeData[startIndex], segmentLength * sizeof(double));

			}

			else
			{
			    if (filterType == 0) {          
			        GetCtrlVal(panel, MAIN_PANEL_NUM_DIMENSION, &windowSize);
			        Filter_Mediere(&waveData[startIndex], filtered, segmentLength, windowSize);
			    } else {                        
			        GetCtrlVal(panel, MAIN_PANEL_NUM_ALPHA, &alpha);
			        Filter_Ordin1(&waveData[startIndex], filtered, segmentLength, alpha);
			    }
			}


            DeleteGraphPlot(panel, MAIN_PANEL_GRAPH_FILTERED, -1, VAL_IMMEDIATE_DRAW);
			
            plotHandle2 = PlotY(panel, MAIN_PANEL_GRAPH_FILTERED,
                                filtered, segmentLength, VAL_DOUBLE,
                                VAL_THIN_LINE, VAL_EMPTY_SQUARE,
                                VAL_SOLID, VAL_CONNECTED_POINTS, color);

			SetAxisRange(panel, MAIN_PANEL_GRAPH_FILTERED,
			             VAL_AUTOSCALE, VAL_AUTOSCALE,
			             VAL_AUTOSCALE, VAL_AUTOSCALE,
			             VAL_NO_CHANGE, VAL_NO_CHANGE);


			filterApplied = 1;
            free(filtered);
            break;
        }
    }
    return 0;
}



int CVICALLBACK OnFilterCB (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	int val = 0;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, MAIN_PANEL_RING_FILTER, &val);

			if(val == 1) {  // Ordin 1
				SetCtrlAttribute(panel, MAIN_PANEL_NUM_ALPHA, ATTR_DIMMED, 0);
				SetCtrlAttribute(panel, MAIN_PANEL_NUM_DIMENSION, ATTR_DIMMED, 1);
			} 
			else if(val == 0) { // Mediere
				SetCtrlAttribute(panel, MAIN_PANEL_NUM_ALPHA, ATTR_DIMMED, 1);
				SetCtrlAttribute(panel, MAIN_PANEL_NUM_DIMENSION, ATTR_DIMMED, 0);
			}

			if (allowAutoFilter)
				OnApplyFilterCB(panel, MAIN_PANEL_BTN_APPLY, EVENT_COMMIT, NULL, 0, 0);

			break;
	}
	return 0;
}
int CVICALLBACK OnDimensionChangeCB (int panel, int control, int event,
                                     void *callbackData, int eventData1, int eventData2)
{
    if (event == EVENT_COMMIT && allowAutoFilter)
        OnApplyFilterCB(panel, MAIN_PANEL_BTN_APPLY, EVENT_COMMIT, NULL, 0, 0);
    return 0;
}

int CVICALLBACK OnAlphaChangeCB (int panel, int control, int event,
                                 void *callbackData, int eventData1, int eventData2)
{
    if (event == EVENT_COMMIT && allowAutoFilter)
        OnApplyFilterCB(panel, MAIN_PANEL_BTN_APPLY, EVENT_COMMIT, NULL, 0, 0);
    return 0;
}

int CVICALLBACK PrevBtnCB (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	int bitMapID;
	char fileName[256];
				
    switch (event)
    {
        case EVENT_COMMIT:
        {
            if (currentSecond == 0)
            {

                break;
            }
            isFullView = 0;

            if (currentSecond > 0)
                currentSecond--;

            int startIndex = currentSecond * (int)sampleRate;
            int endIndex = startIndex + (int)sampleRate;
            if (endIndex > npoints)
                endIndex = npoints;

            int segmentLength = endIndex - startIndex;
            if (plotHandle != -1)
                DeleteGraphPlot(panel, MAIN_PANEL_GRAPH_RAW, plotHandle, VAL_IMMEDIATE_DRAW);

            plotHandle = PlotY(panel, MAIN_PANEL_GRAPH_RAW, &waveData[startIndex], segmentLength, VAL_DOUBLE,
                               VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);

            SetCtrlVal(panel, MAIN_PANEL_NUM_START, currentSecond);
            SetCtrlVal(panel, MAIN_PANEL_NUM_STOP, currentSecond + 1);
			
			UpdateStats(panel, &waveData[startIndex], segmentLength);

			
			if(filterApplied) OnApplyFilterCB(panel, MAIN_PANEL_GRAPH_FILTERED, event, callbackData, eventData1, eventData2);
			
			Rect r;
			int top1, left1, height1, width1;
			int top2, left2, height2, width2;

			GetCtrlBoundingRect(panel, MAIN_PANEL_GRAPH_RAW, &top1, &left1, &height1, &width1);
			GetCtrlBoundingRect(panel, MAIN_PANEL_GRAPH_FILTERED, &top2, &left2, &height2, &width2);
			r.top    = (top1 < top2 ? top1 : top2);
			r.left   = (left1 < left2 ? left1 : left2);

			int bottom = (top1 + height1 > top2 + height2 ? top1 + height1 : top2 + height2);
			int right  = (left1 + width1  > left2 + width2  ? left1 + width1  : left2 + width2);

			r.height = bottom - r.top;
			r.width  = right  - r.left;
			sprintf(fileName, "C:\\Users\\Daria\\OneDrive\\Desktop\\An3\\apd_proiect\\lab1proiect\\output\\grafic_%d_%d_%ld.jpg", currentSecond, currentSecond + 1, time(NULL));
			GetPanelDisplayBitmap(panel, 1, r, &bitMapID);
			SaveBitmapToJPEGFile(bitMapID, fileName, JPEG_PROGRESSIVE, 100);
			DiscardBitmap(bitMapID);

	
            break;
        }
    }
    return 0;
}


int CVICALLBACK NextBtnCB (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	
	int bitMapID;
	char fileName[256];

	switch (event)
	{
		case EVENT_COMMIT:
			
			
			if(isFullView)
			{
			    isFullView = 0;
			    currentSecond = 0;
			}
			
			
			int startIndex = currentSecond * (int)sampleRate;
			int endIndex = startIndex + (int)sampleRate;
			if(endIndex > npoints)
			{
				endIndex = npoints;
			}
			
			int segmentLength = endIndex - startIndex;
			
			if(plotHandle != -1)
			{
				DeleteGraphPlot(panel, MAIN_PANEL_GRAPH_RAW, plotHandle, VAL_IMMEDIATE_DRAW);
			}
			plotHandle = PlotY(panel, MAIN_PANEL_GRAPH_RAW, &waveData[startIndex], segmentLength, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			SetCtrlVal(panel, MAIN_PANEL_NUM_START, currentSecond);
			SetCtrlVal(panel, MAIN_PANEL_NUM_STOP, currentSecond + 1);
			
			UpdateStats(panel, &waveData[startIndex], segmentLength);
			
			
			if(filterApplied) OnApplyFilterCB(panel, MAIN_PANEL_GRAPH_FILTERED, event, callbackData, eventData1, eventData2);
			Rect r;
			int top1, left1, height1, width1;
			int top2, left2, height2, width2;

			GetCtrlBoundingRect(panel, MAIN_PANEL_GRAPH_RAW, &top1, &left1, &height1, &width1);
			GetCtrlBoundingRect(panel, MAIN_PANEL_GRAPH_FILTERED, &top2, &left2, &height2, &width2);
			r.top    = (top1 < top2 ? top1 : top2);
			r.left   = (left1 < left2 ? left1 : left2);

			int bottom = (top1 + height1 > top2 + height2 ? top1 + height1 : top2 + height2);
			int right  = (left1 + width1  > left2 + width2  ? left1 + width1  : left2 + width2);

			r.height = bottom - r.top;
			r.width  = right  - r.left;
			sprintf(fileName, "C:\\Users\\Daria\\OneDrive\\Desktop\\An3\\apd_proiect\\lab1proiect\\output\\grafic_%d_%d_%ld.jpg", currentSecond, currentSecond + 1, time(NULL));
			GetPanelDisplayBitmap(panel, 1, r, &bitMapID);
			SaveBitmapToJPEGFile(bitMapID, fileName, JPEG_PROGRESSIVE, 100);
			DiscardBitmap(bitMapID);


			if(currentSecond  < 5) 
			{
				currentSecond++;

			} else {
				break;
			}
			break;
	}

	return 0;
}

int CVICALLBACK OnResetBtnCB (int panel, int control, int event,
                              void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
        {
            if (waveData == NULL || npoints <= 0) {
                MessagePopup("Reset", "Nu este incarcat niciun fisier.");
                return 0;
            }

            isFullView    = 1;
            currentSecond = 0;

        
            DeleteGraphPlot(panel, MAIN_PANEL_GRAPH_RAW,      -1, VAL_IMMEDIATE_DRAW);
            DeleteGraphPlot(panel, MAIN_PANEL_GRAPH_FILTERED, -1, VAL_IMMEDIATE_DRAW);
            DeleteGraphPlot(panel, MAIN_PANEL_GRAPH_HISTO,    -1, VAL_IMMEDIATE_DRAW);

      
            plotHandle = PlotY(panel, MAIN_PANEL_GRAPH_RAW,
                               waveData, npoints, VAL_DOUBLE,
                               VAL_THIN_LINE, VAL_EMPTY_SQUARE,
                               VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);

            UpdateStats(panel, waveData, npoints);

            
            if(filterApplied) OnApplyFilterCB(panel, MAIN_PANEL_BTN_APPLY, EVENT_COMMIT, NULL, 0, 0);

            SetCtrlVal(panel, MAIN_PANEL_NUM_START, 0);
            SetCtrlVal(panel, MAIN_PANEL_NUM_STOP, (int)(npoints / sampleRate));

            break;
        }
    }
    return 0;
}

int CVICALLBACK OnDerivativeToggleCB (int panel, int control, int event,
                                      void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            GetCtrlVal(panel, control, &showDerivative);
            if (showDerivative) {
                showEnvelope = 0;
                SetCtrlVal(panel, MAIN_PANEL_CHECKBOX_2, 0);
            }
            OnApplyFilterCB(panel, MAIN_PANEL_BTN_APPLY, EVENT_COMMIT, NULL, 0, 0);
            break;
    }
    return 0;
}

int CVICALLBACK OnEnvelopeToggleCB (int panel, int control, int event,
                            void *callbackData, int eventData1, int eventData2)
{
    if (event != EVENT_COMMIT) return 0;

    if (waveData == NULL || npoints <= 0) {
        MessagePopup("Eroare", "Datele semnalului nu sunt incarcate.");
        return 0;
    }


	int count_max = 0, count_min = 0;
	double *peakLoc = NULL, *peakAmp = NULL, *peakDer = NULL;
	double *valleyLoc = NULL, *valleyAmp = NULL, *valleyDer = NULL;
	GetCtrlVal(panel, control, &showEnvelope);
	if(showEnvelope)
	{
		showDerivative = 0;
		SetCtrlVal(panel, MAIN_PANEL_CHECKBOX, 0);
		PeakDetector(waveData, npoints,
		                   0.0,       
		                   3,         
		                   0,         
		                   1,         
		                   1,         
		                   &count_max,
		                   &peakLoc,
		                   &peakAmp,
		                   &peakDer);

		PeakDetector(waveData, npoints,
		                   0.0,
		                   3,
		                   1,         
		                   1,         
		                   1,         
		                   &count_min,
		                   &valleyLoc,
		                   &valleyAmp,
		                   &valleyDer);

	    DeleteGraphPlot(panel, MAIN_PANEL_GRAPH_FILTERED, -1, VAL_NO_DRAW);

	    PlotY(panel, MAIN_PANEL_GRAPH_FILTERED, waveData, npoints, VAL_DOUBLE,
	          VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_GREEN);

	    PlotXY(panel, MAIN_PANEL_GRAPH_FILTERED, peakLoc, peakAmp, count_max,
	           VAL_DOUBLE, VAL_DOUBLE,
	           VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);

	    PlotXY(panel, MAIN_PANEL_GRAPH_FILTERED, valleyLoc, valleyAmp, count_min,
	           VAL_DOUBLE, VAL_DOUBLE,
	           VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_BLUE);
	}

    return 0;
}



int CVICALLBACK OnSwitchPanelCB (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(panel == mainPanel)
			{
				SetCtrlVal(frqPanel, FRQ_PANEL_IDC_SWITCH_PANEL, 1);
				DisplayPanel(frqPanel);
				HidePanel(panel);
			}
			else
			{
				SetCtrlVal(mainPanel, FRQ_PANEL_IDC_SWITCH_PANEL, 0);
				DisplayPanel(mainPanel);
				HidePanel(panel);
			}
			break;
	}
	return 0;
}
