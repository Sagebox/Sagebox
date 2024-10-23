
// ***************************************************
// CCounter.h -- SageBox Neural Network 7-Bit Counter
// ***************************************************
//
// See notes in CCounter.cpp for more information
//

#pragma once

#if !defined(_CSixBitCounter_h__)
#define _CSixBitCounter_h__
#include "CSageBox.h"
#include "CBarGraph.h"
#include "CDevNN.h"

// Main CounterNN class, derived from CDevNN
class CCounterNN : protected CDevNN
{
public:
    static constexpr int        kBits           = 7;            // Number of bits for the counter.
                                                                // Past 7 bits may take a while and will probably need additions to the nodes
                                                                // in the layer structure.

private:

    // All of our window controls

    CWindow                   * m_cWin              = nullptr;  // Main Counter Window
    CWindow                   * m_cGraphWin         = nullptr;  // Bar-Chart window
    CWindow                   * m_cResultsWin       = nullptr;  // Text Results window
    CButton                   * m_cButton           = nullptr;  // Start/Stop button
    CTextWidget               * m_cTextIteration    = nullptr;  // Text Widgets for output messages
    CTextWidget               * m_cTextPercent      = nullptr;  // --> These allow fire-and-forget output, since we 
    CTextWidget               * m_cTextError        = nullptr;  //     don't have to manage the display.
    CTextWidget               * m_cTextTrend        = nullptr;
    CTextWidget               * m_cTextLR           = nullptr;
    CBarGraph                   m_cBarGraph;                    // Bar Graph Class to print bar chart bars

    // Some constant values 

    static constexpr int          m_iMaxEpochs      = -1;                       // Run forever (until TrainUpdate() quits automatically)
    static constexpr int          m_iInputs         = kBits;                    // kBits bits of input
    static constexpr int          m_iOutputs        = kBits;                    // kBits bits of output
    static constexpr int          m_iDataSets       = 1 << kBits;               // 0-127 in 7 bits
    static constexpr int          m_iMaxValues      = m_iOutputs*m_iDataSets;
    static constexpr int          m_iTrainResolution= 25;                       // Train Update() will only execute very 25 epochs
    static constexpr int          kBarWidth         = 7;                        // Width of Bar Graph bars
    static constexpr int          kWinWidth         = (1 << kBits)*kBarWidth;   // Width of the Bar Graph Window, based on Bar Size and number of possible outputs
    static constexpr int          kWinHeight        = 310;
    static constexpr int          kWinX             = 20;
    static constexpr int          kWinY             = 50; 
    static constexpr RGBColor_t   krgbGraphWin1     = {0,0,0};                  // Top Gradient color for clear Graph Window
    static constexpr RGBColor_t   krgbGraphWin2     = {64,64,64};               // Bottom Gradient color for clear Graph Window
    static constexpr RGBColor_t   rgbBarGraph[2]    = { {0,255,0}, {0,255,255} };   // Alternate colors for the bar graphs
    static constexpr const char * sColorWrong       = "{r}";                    // Shortcut for Red when printing text
    static constexpr const char * sColorCorrect     = "{g}";                    // Shortcut for Green when printing text


    int     m_iOutValues[m_iDataSets];                      // Output values in decimal

    double  m_fTrainInputs[m_iInputs*m_iDataSets];          // Training Inputs
    double  m_fTrainOutputs[m_iInputs*m_iDataSets];         // Training Outputs

    cnnErr TrainUpdate(const char * & sMsg) override;     // Override for TrainUpdate() called after every Epoch in the neural network
    void UpdateValueBox();
    void InitUI();
    CString RunNeuralNetwork();
public:
    CCounterNN(unsigned int uiRandSeed,CDevNN::stLayerInput_t * stLayers);
    void CreateDataMap();
    void Go(CWindow & cWin);
    static int main();

};

#endif // _CSixBitCounter_h__