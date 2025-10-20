#include <utility.h>
#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include <analysis.h>
#include <formatio.h>
#include "toolbox.h"
#include "Proiect1.h"

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
int plotHandle = -1;
int plotHandle2 = -1;


static int mainPanel = 0;

int main (int argc, char *argv[])
{
	int event = 0;
	int val1 = 0;
	int val2 = 0;
	BOOL quitFlag = FALSE;
	
	
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((mainPanel = LoadPanel (0, "Proiect1.uir", MAIN_PANEL)) < 0)
		return -1;
	DisplayPanel (mainPanel);
	while(!quitFlag)
	{
		event = GetUserEvent(1, &val1, &val2);
		switch(event)
		{
			case MYEVENT_CLOSE:
				//event
				quitFlag = TRUE;
			break;
		
		}
	}
	DiscardPanel (mainPanel);
	return 0;
}

int CVICALLBACK LoadWaveCB (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	
	double maxVal = 0.0;
	double minVal = 0.0;
	int maxIndex = 0;
	int minIndex = 0;
	double mean = 0.0;
	double variance = 0.0;
	double median = 0.0;
	int zeroCross = 0;
	double skewness = 0.0;
	double kurtosis = 0.0;
	double histBins[50];
	int histValues[50];
	switch (event)
	{
		case EVENT_COMMIT:
			//executa script python pentru conversia unui fisierului .wav in .txt
			LaunchExecutable("python main.py");
			
			//astept sa fie generate cele doua fisiere (modificati timpul daca este necesar
			Delay(4);
			
			//incarc informatiile privind rata de esantionare si numarul de valori
			FileToArray("waveInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[SAMPLE_RATE];
			npoints = waveInfo[NPOINTS];
			
			double duration = (double)npoints / sampleRate;
			if (duration > 10.0) {
		    	npoints = sampleRate * 6;
			}
			//alocare memorie pentru numarul de puncte
			waveData = (double *) calloc(npoints, sizeof(double));
			
			//incarcare din fisierul .txt in memorie (vector)
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			if (plotHandle != -1)
				DeleteGraphPlot(panel, MAIN_PANEL_GRAPH_FILTERED, plotHandle, VAL_IMMEDIATE_DRAW);
			//afisare pe grapf
			plotHandle = PlotY(panel, MAIN_PANEL_GRAPH_RAW, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			MaxMin1D(waveData, npoints, &maxVal, &maxIndex, &minVal, &minIndex);
			SetCtrlVal(panel, MAIN_PANEL_NUM_MIN, minVal);
			SetCtrlVal(panel, MAIN_PANEL_NUM_MIN_IDX, minIndex);
			SetCtrlVal(panel, MAIN_PANEL_NUM_MAX, maxVal);
			SetCtrlVal(panel, MAIN_PANEL_NUM_MAX_IDX, maxIndex);
			
			Mean(waveData, npoints, &mean);
			SetCtrlVal(panel, MAIN_PANEL_NUM_AVG, mean);
			
			Variance(waveData, npoints, &mean, &variance);
			SetCtrlVal(panel, MAIN_PANEL_NUM_DISPERSIE, variance);
			
			Median(waveData, npoints, &median);
			SetCtrlVal(panel, MAIN_PANEL_NUM_MEDIANA, median);
			
		    for (int i = 1; i < npoints; i++) {
		        if ((waveData[i-1] < 0 && waveData[i] > 0) || (waveData[i-1] > 0 && waveData[i] < 0))
		            zeroCross++;
		    }
			SetCtrlVal(panel, MAIN_PANEL_NUM_ZCROSS, zeroCross);
			
			Moment(waveData, 256, 3, &skewness);
			Moment(waveData, 256, 4, &kurtosis);
			SetCtrlVal(panel, MAIN_PANEL_NUM_SKEWNESS, skewness);
			SetCtrlVal(panel, MAIN_PANEL_NUM_KURTOSIS, kurtosis);
			
			Histogram(waveData, npoints, minVal, maxVal, histValues, histBins, 50);
			SetCtrlVal(panel, MAIN_PANEL_NUM_HISTOGRAM, (double)histValues[0]);
			
			OnFilterCB(panel, MAIN_PANEL_RING_FILTER, EVENT_COMMIT, NULL, 0, 0);
			break;
	}
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


void Filter_Ordin1(double *input, double *output, int npoints, double alpha)
{
    output[0] = input[0];
    for (int i = 1; i < npoints; i++) {
        output[i] = (1 - alpha) * output[i - 1] + alpha * input[i];
    }
}


int CVICALLBACK OnApplyFilterCB (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	
	int filterType;
    double alpha;
    int windowSize;
    double *filtered = calloc(npoints, sizeof(double));
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal(panel, MAIN_PANEL_RING_FILTER, &filterType);
			
			if (filterType == 0) { // Mediere
				GetCtrlVal(panel, MAIN_PANEL_NUM_DIMENSION, &windowSize);
                Filter_Mediere(waveData, filtered, npoints, windowSize);
			} else {                // Ordin I
				GetCtrlVal(panel, MAIN_PANEL_NUM_ALPHA, &alpha);
                Filter_Ordin1(waveData, filtered, npoints, alpha);
			}
			if (plotHandle2 != -1)
				DeleteGraphPlot(panel, MAIN_PANEL_GRAPH_FILTERED, plotHandle2, VAL_IMMEDIATE_DRAW);
			if (waveData != NULL)
				plotHandle2 = PlotY(panel, MAIN_PANEL_GRAPH_FILTERED, filtered, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
			
			break;
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
			if(val == 1) {
				SetCtrlAttribute(panel, MAIN_PANEL_NUM_ALPHA, ATTR_DIMMED, !val);
				SetCtrlAttribute(panel, MAIN_PANEL_NUM_DIMENSION, ATTR_DIMMED, val);
			} else if(val == 0) {
				SetCtrlAttribute(panel, MAIN_PANEL_NUM_ALPHA, ATTR_DIMMED, !val);
				SetCtrlAttribute(panel, MAIN_PANEL_NUM_DIMENSION, ATTR_DIMMED, val);
			}
			break;
	}
	return 0;
}
