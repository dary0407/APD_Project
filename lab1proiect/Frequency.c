
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
#include <utility.h>
#include <advanlys.h>



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

			SetCtrlAttribute(panel, FRQ_PANEL_TIMER, ATTR_ENABLED, 1);
			break;
		case EVENT_LOST_FOCUS:

			SetCtrlAttribute(panel, FRQ_PANEL_TIMER, ATTR_ENABLED, 0);
			break;
		case EVENT_CLOSE:
			QueueUserEvent(MYEVENT_CLOSE, panel, FRQ_PANEL);
			break;
	}
	return 0;
}

int CVICALLBACK OnTimerCB (int panel, int control, int event,
                           void *callbackData, int eventData1, int eventData2)
{
    int N_points, windowType, filterType;
    double *xArray = NULL, *yFiltered = NULL;
    double *autoSpectrum = NULL, *convertedSpectrum = NULL;
    double *autoSpecFilt = NULL, *convSpecFilt = NULL;
    
    // Coeficien?i ?i variabile de stare pentru filtrele "Old-style"
    double a[1024] = {0}, b[1024] = {0};
    static double x1_st[1024] = {0}, y1_st[1024] = {0};
    int na = 1024, nb = 1024;
    
    WindowConst wind, windFilt;
    double dt, df, dfFilt;
    double frequencyPeak, powerPeak;
    char unit[32] = "";
    int imghandle;
    char filename[512]; // Dimensiune mai mare pentru c?i lungi

    switch (event)
    {
        case EVENT_TIMER_TICK:
            // 1. Preluare parametri din interfa??
            GetCtrlVal(panel, FRQ_PANEL_NPOINTS, &N_points);
            GetCtrlVal(panel, FRQ_PANEL_WINDOW, &windowType);
            GetCtrlVal(panel, FRQ_PANEL_RING_3, &filterType);

            if (waveData == NULL || N_points <= 0) return 0;
            
            // Verificare limit? buffer
            if (g_frqOffset + N_points > npoints) {
                MessagePopup("Info", "Resetare la începutul datelor.");
                g_frqOffset = 0;
                return 0; 
            }

            // 2. Alocare memorie
            xArray = calloc(N_points, sizeof(double));
            yFiltered = calloc(N_points, sizeof(double));
            autoSpectrum = calloc(N_points / 2, sizeof(double));
            convertedSpectrum = calloc(N_points / 2, sizeof(double));
            autoSpecFilt = calloc(N_points / 2, sizeof(double));
            convSpecFilt = calloc(N_points / 2, sizeof(double));

            if (!xArray || !yFiltered || !autoSpectrum || !autoSpecFilt) goto Cleanup;
            
            // Copiere date curente
            memcpy(xArray, waveData + g_frqOffset, N_points * sizeof(double));
            dt = 1.0 / sampleRate;

            // --- 3. PROCESARE SPECTRU INI?IAL ---
            ScaledWindowEx(xArray, N_points, windowType, 0.0, &wind);
            AutoPowerSpectrum(xArray, N_points, dt, autoSpectrum, &df);
            SpectrumUnitConversion(autoSpectrum, N_points/2, 0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df, wind, convertedSpectrum, unit);

            // --- 4. FILTRARE IIR (Old-style) ---
            // Folosim frecven?e sigure pentru a evita erorile de memorie
            if (filterType == 0) {
                InvCh_Coef(0, 2, sampleRate, 1000.0, 4000.0, 40.0, a, na, b, nb);
            } else {
                Elp_Coef(0, 2, sampleRate, 1000.0, 4000.0, 0.5, 40.0, a, na, b, nb);
            }
            
            // Aplic?m filtrarea pe xArray
            IIRFiltering(xArray, (ssize_t)N_points, a, y1_st, na, b, x1_st, nb, yFiltered);

            // --- 5. PROCESARE SPECTRU FILTRAT ---
            ScaledWindowEx(yFiltered, N_points, windowType, 0.0, &windFilt);
            AutoPowerSpectrum(yFiltered, N_points, dt, autoSpecFilt, &dfFilt);
            SpectrumUnitConversion(autoSpecFilt, N_points/2, 0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, dfFilt, windFilt, convSpecFilt, unit);
            
            // Estimare Peak pe spectrul filtrat
            PowerFrequencyEstimate(autoSpecFilt, N_points/2, 0.0, windFilt, dfFilt, 0, &frequencyPeak, &powerPeak);
            SetCtrlVal(panel, FRQ_PANEL_FREQ_PEAK, frequencyPeak);
            SetCtrlVal(panel, FRQ_PANEL_POWER_PEAK, powerPeak);

            // --- 6. ACTUALIZARE GRAFICE ---
            DeleteGraphPlot(panel, FRQ_PANEL_FREQ_GRAPH, -1, VAL_IMMEDIATE_DRAW);
            PlotY(panel, FRQ_PANEL_FREQ_GRAPH, convertedSpectrum, N_points / 2, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
            
            DeleteGraphPlot(panel, FRQ_PANEL_GRAPH_FILTER, -1, VAL_IMMEDIATE_DRAW);
            PlotY(panel, FRQ_PANEL_GRAPH_FILTER, yFiltered, N_points, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
            
            DeleteGraphPlot(panel, FRQ_PANEL_GRAPH_FREQ2, -1, VAL_IMMEDIATE_DRAW);
            PlotY(panel, FRQ_PANEL_GRAPH_FREQ2, convSpecFilt, N_points / 2, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);

            // --- 7. SALVARE IMAGINI (Calea ta specific?) ---
            long timestamp = (long)time(NULL);

            // Salvare Spectru Ini?ial
            GetCtrlDisplayBitmap(panel, FRQ_PANEL_FREQ_GRAPH, 1, &imghandle);
            sprintf(filename, "C:\\Users\\andre\\Documents\\GitHub\\APD_Project\\lab1proiect\\output\\spectru_init_N%d_%ld.jpg", N_points, timestamp);
            SaveBitmapToJPEGFile(imghandle, filename, JPEG_PROGRESSIVE, 100);
            DiscardBitmap(imghandle);

            // Salvare Spectru Filtrat
            GetCtrlDisplayBitmap(panel, FRQ_PANEL_GRAPH_FREQ2, 1, &imghandle);
            sprintf(filename, "C:\\Users\\andre\\Documents\\GitHub\\APD_Project\\lab1proiect\\output\\spectru_filt_N%d_%ld.jpg", N_points, timestamp);
            SaveBitmapToJPEGFile(imghandle, filename, JPEG_PROGRESSIVE, 100);
            DiscardBitmap(imghandle);
			
			GetCtrlDisplayBitmap(panel, FRQ_PANEL_GRAPH_FILTER, 1, &imghandle);
			sprintf(filename, "C:\\Users\\andre\\Documents\\GitHub\\APD_Project\\lab1proiect\\output\\semnal_filt_N%d_%ld.jpg", N_points, timestamp);
			SaveBitmapToJPEGFile(imghandle, filename, JPEG_PROGRESSIVE, 100);
			DiscardBitmap(imghandle);

            g_frqOffset += N_points;

        Cleanup:
            if (xArray) free(xArray);
            if (yFiltered) free(yFiltered);
            if (autoSpectrum) free(autoSpectrum);
            if (convertedSpectrum) free(convertedSpectrum);
            if (autoSpecFilt) free(autoSpecFilt);
            if (convSpecFilt) free(convSpecFilt);
            break;
    }
    return 0;
}





