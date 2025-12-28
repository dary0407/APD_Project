
//==============================================================================
//
// Title:		Frequency.c
// Purpose:		A short description of the implementation.
//
// Created on:	11/5/2020 at 5:01:06 PM by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "Frequency.h"
#include "Proiect1.h"
#include <analysis.h>


//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

//==============================================================================
// Static functions

//==============================================================================
// Global variables
int g_frqOffset = 0; //ofset pt domeniul frecventa
//==============================================================================
// Global functions


int CVICALLBACK OnFrequencyPanelCB (int panel, int event, void *callbackData,
									int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:
            if (waveData != NULL && npoints > 0) 
            {
   
                DeleteGraphPlot(panel, FRQ_PANEL_GRAPH_INIT, -1, VAL_IMMEDIATE_DRAW);
  
                PlotY(panel, FRQ_PANEL_GRAPH_INIT, waveData, npoints, VAL_DOUBLE, 
                      VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
            }

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface(0);
			break;
	}
	return 0;
}

int CVICALLBACK OnNextCB (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	int N_points;
	int windowType;
	double *xArray = NULL;
	double *autoSpectrum = NULL;
	double *convertedSpectrum = NULL;
	WindowConst wind;
	double dt, df;
	double frequencyPeak, powerPeak;
	char unit[32] = "";
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, FRQ_PANEL_NPOINTS, &N_points);
			GetCtrlVal(panel, FRQ_PANEL_WINDOW, &windowType);

	
			if (waveData == NULL || N_points <= 0) {
                //MessagePopup("Eroare", "Datele nu sunt incarcate sau N_points este invalid.");
                return 0;
            }
			
			if (g_frqOffset + N_points > npoints) {
				MessagePopup("Info", "Am ajuns la sfarsitul datelor. Resetare la inceput.");
				g_frqOffset = 0;
				return 0; 
			}

			xArray = calloc(N_points, sizeof(double));
			autoSpectrum = calloc(N_points / 2, sizeof(double));
			convertedSpectrum = calloc(N_points / 2, sizeof(double));

			if (!xArray || !autoSpectrum || !convertedSpectrum) {
			    MessagePopup("Eroare", "Alocare memorie esuata.");
			    goto Cleanup; 
			}
			
			memcpy(xArray, waveData + g_frqOffset, N_points * sizeof(double));
			
			dt = 1.0 / sampleRate;
			
			ScaledWindowEx(xArray, N_points, windowType, 0.0, &wind);

			AutoPowerSpectrum(xArray, N_points, dt, autoSpectrum, &df);
			
			PowerFrequencyEstimate(autoSpectrum, N_points/2, 0.0, wind, df, 0, &frequencyPeak, &powerPeak);
			
			SetCtrlVal(panel, FRQ_PANEL_FREQ_PEAK, frequencyPeak);
			SetCtrlVal(panel, FRQ_PANEL_POWER_PEAK, powerPeak);  

			SpectrumUnitConversion(autoSpectrum, N_points/2, 0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df, wind, convertedSpectrum, unit);
			
			DeleteGraphPlot(panel, FRQ_PANEL_FREQ_GRAPH, -1, VAL_IMMEDIATE_DRAW);
			PlotY(panel, FRQ_PANEL_FREQ_GRAPH, convertedSpectrum, N_points / 2, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
			
            g_frqOffset += N_points;
            
			Cleanup:
				if (xArray) free(xArray);
				if (autoSpectrum) free(autoSpectrum);
				if (convertedSpectrum) free(convertedSpectrum);
				
			break;
	}
	return 0;
}

int CVICALLBACK OnPrevCB (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
    int N_points;
    switch (event)
    {
        case EVENT_COMMIT:
            GetCtrlVal(panel, FRQ_PANEL_NPOINTS, &N_points);
			
            g_frqOffset -= N_points;

            if (g_frqOffset < 0) g_frqOffset = 0;
       
            OnNextCB(panel, FRQ_PANEL_COMMANDBUTTON_4 , EVENT_COMMIT, 0, 0, 0);
            break;
    }
    return 0;
}

int CVICALLBACK OnTimerCB (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
    {
        case EVENT_TIMER_TICK:
            OnNextCB(panel, FRQ_PANEL_COMMANDBUTTON_4, EVENT_COMMIT, 0, 0, 0);
            break;
    }
    return 0;
}

int CVICALLBACK OnApply (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}
