#ifndef READDATA_H
#define READDATA_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <string>
#include <stdlib.h>
#include "TFile.h"
#include "events.h"
#include "getInputParameters.h"
//#include "output.h"

// Int_t readTimeStamp(const Parameters &setting,
//                     std::vector<std::vector<UShort_t>> &flags,
//                     std::vector<std::vector<ULong64_t>> &timeStamps);

// Int_t readCoincidences(const Parameters &setting,
//                        const std::vector<std::vector<UInt_t>> &pairs,
//                        std::vector<std::vector<Event>> &coincidentEvents);

// Int_t readEvents(const Parameters &setting,
//                  std::vector<std::vector<Event>> &multichannelevents);

// Int_t readEventsBuf(const Parameters &setting,
//                     std::vector<std::vector<Event>> &multichannelevents);

Int_t getSampleNum(Parameters &setting);

Int_t readData(const Parameters &setting, const UInt_t channelIndex, char *buffer,
               const ULong_t BUFFERSIZE, ULong_t& currentNumber,
               std::vector<Event> &Events, std::vector<std::vector< Float_t > > &Pulses);
// Int_t newPulseHeight(const Parameters &setting,
//                 std::vector< std::vector< Event > > &coincidentEvents);

#endif // READDATA_H
