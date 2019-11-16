#ifndef GETINPUTPARAMETERS_H
#define GETINPUTPARAMETERS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include "TFile.h"

struct Parameters
{
    //Settings
    std::string Directory;
    std::vector < int > Folders;
    std::vector < int > Channels;
    int Filetype=0; // 0 for binary, 1 for ASCII
    int NHeaders=7;
    std::vector < int > Headersize; // bytes
    std::vector < bool > SaveHeaders; // bytes
    int NSamples=208;
    int Samplesize=2; //bytes
    float DynamicRange=2.0; // Volts
    int Resolution=14;
    int Offset=30; // number of samples to calculate the baseline
    int Delt=2; // time between 2 samples, unit: ns
    int Polarity=1; // pulse polarity
    int SavePulses=0;

    bool TimeStamp=0;
    bool SaveTimeStamp=0;

    // float Rejection=0.5;// in coincidence, reject bad shaping pulses

    // Bad pulses options
    bool FilterBad=1;
    bool ZeroSupression=1;
    float MinVoltage=0.05; // Volts
    bool Clipped=1;
    float MaxVoltage=2.0; // Volts
    int SaveBad=0;

    // Piled-up pulses option
    bool FilterPiledup=1;
    bool SavePiledup=0;
    bool SaveSTime=0;
    bool SaveSFrequency=0;

    //energy cut
    std::vector < float_t> CalicoefsPID;
    std::vector < float_t> CalicoefsPHD;
    bool EnergyCut=0;
    float_t EnergyHigh=1000;
    float_t EnergyLoW=0;

    //PSD cut
    std::vector< bool > PSDCut;
    std::vector< float_t > PSDLow;
    std::vector< float_t >  PSDHigh;

    // Coincidences selection
    bool Coincidence=1;
    std::vector < int > CoincidenceChannels;
    float TimeWindow=8; // time window, unit: ns
    int Interpolation=0; // number of interpolation points between two samples
    int TRise = 5; // rise time in number of samples
        // DIACFD settings
    int FilterWidth=1; // width of moving average filter, number of samples
    double Fraction=0.15; // constant fraction F
    int TimeDelay=6; // time delay, unit: ns
    bool SaveDT=1;
    bool TOF=0;
    bool SaveTOF=0;

    // Pulse height distribution
    bool SavePH=0;
    bool PHD=1;
    bool SavePHD=0;
    int PHDBins=200;
    float PHmin=0;
    float PHmax=20;
    // pulse integral distribution options
    bool SavePI=0;
    bool PID=1;
    int PreTrigger=80;
    int PreGate=4;
    int LongGate=120;
    bool SavePID=0;
    int PIDBins=200;
    float PImin=0;
    float PImax=20;

    // Tail integral vs total integral
    bool PSD=1;
    int ShortGate=22;
    int SaveIntegrals=0;
    // plot options
    int PSDXBins=200;
    float PSDXmin=0;
    float PSDXmax=20;
    int PSDYBins=200;
    float PSDYmin=0;
    float PSDYmax=1;
    //  Maximum number of pulses to process
    int MaxNumPulses=1;
};

std::string trim(const std::string& str,
                 const std::string& whitespace = " \t");

std::string removeComments(const std::string& str,
                 const std::string& begChar);

int getInputParameters(std::string filepath, Parameters& setting);

#endif