#include "readData.h"

// read the data, calculate the baseline, convert to voltage, calculate height and heightIndex.

// Int_t readTimeStamp( const Parameters &setting, std::vector< std::vector<UShort_t> > &flags, std::vector< std::vector<ULong64_t> > &timeStamps)
// {
//     std::string filepath;
//     ULong64_t timestamptemp;
//     UShort_t flagTemp;
//     std::vector<UShort_t> flagstemp;
//     std::vector<ULong64_t> timestampstemp;
//     //std::vector<std::string> filepaths;

//     //read the data
//     UInt_t channelIndex(0);
//     for (auto channel : setting.CoincidenceChannels)
//     {
//         for (int j = setting.Folders[0]; j <= setting.Folders[1];)
//         {
//             filepath = setting.Directory + '/' + std::to_string(j) + '/' + "channel" + std::to_string(channel) + ".bin";
//             // std::cout << filepaths[i-setting.NFolders[0]] << std::endl;
//             // std::cout << filepaths[i-setting.NFolders[0]].length() << std::endl;

//             //binary
//             if (setting.Filetype == 0)
//             {
//                 std::ifstream fileptr;
//                 fileptr.open(filepath, std::ios::in | std::ios::binary);
//                 std::cout << "Opening binary file: " << filepath << std::endl;
//                 if (!fileptr.is_open())
//                 {
//                     std::cout << "Cannot open file! " << filepath << std::endl;
//                     exit(1);
//                 }
//                 else
//                 {
//                     std::cout << " Open success !\n" << " Reading data: " << std::endl;
//                 }

//                 //Bool_t badflag=false;
//                 int currentNumber = 0;
//                 //fileptr.ignore(10000000*230);
//                 while (!fileptr.eof() && currentNumber < setting.MaxNumPulses)
//                 {
//                     fileptr.ignore(setting.Headersize[0] + setting.Headersize[1]); // board number and channel number
//                     fileptr.read((char *)&(timestamptemp), setting.Headersize[2]); // fine time stamp
//                     timestampstemp.push_back(timestamptemp);
//                     //fileptr.ignore(8);
//                     fileptr.ignore(setting.Headersize[3] + setting.Headersize[4]); // energy
//                     // fileptr.read((char *)&(flagTemp), setting.Headersize[5]); // ignore flags
//                     // flagstemp.push_back(flagTemp);
//                     fileptr.ignore(setting.Headersize[5]); // ignore flags
//                     fileptr.ignore(setting.Headersize[6]); // total number of samples
//                     fileptr.ignore(setting.Samplesize * setting.NSamples); // samples
//                     //fileptr.ignore(10*230);

//                     currentNumber ++;
//                 }
//                 fileptr.close();
//                 j++; //TBD

//                 std::cout << currentNumber << " input pulses in channel" << std::to_string(channel)<< std::endl;
//             }
//             // ASCII
//             else if (setting.Filetype == 1)
//             {
//                 /* read data */
//                 // TODO
//             }
//         }
//         flags.push_back(flagstemp);
//         flagstemp.clear();
//         timeStamps.push_back(timestampstemp);
//         timestampstemp.clear();
        
//     }

//     return 0;
// }

// Int_t readCoincidences(const Parameters &setting,
//                 const std::vector<std::vector<UInt_t>> &pairs,
//                 std::vector<std::vector<Event>> &coincidentEvents)
// {
//     std::string filepath;
//     ULong64_t timestamptemp;
//     std::vector<ULong64_t> timestampsTemp;
//     UShort_t EnergyLong;
//     std::vector<UShort_t> EnergyLongTemp;
//     UShort_t EnergyShort;
//     std::vector<UShort_t> EnergyShortTemp;
//     UShort_t sample;
//     std::vector<UShort_t> samples(setting.NSamples);
//     std::vector<std::vector<UShort_t> > pulsesTemp;
//     //std::vector<Float_t> voltageTemp(setting.NSamples);
//     //std::vector<std::vector<Float_t>> voltagesTemp;
//     std::vector<Event> eventsTemp;
//     UInt_t totalheadersize = std::accumulate(setting.Headersize.begin(),setting.Headersize.end(),0); // total header size
//     UInt_t tailheadersize = std::accumulate(setting.Headersize.begin() + 3,setting.Headersize.end(),0); // size of Energy(ch) + Energy Short(ch) + Flags + Number of Samples
//     UInt_t eventsize = totalheadersize + setting.NSamples * setting.Samplesize;
    
//     Int_t PreTrig = setting.PreTrigger / setting.Delt;
//     Int_t PreGate = setting.PreGate / setting.Delt;
//     PreGate = PreTrig - PreGate;
//     if(PreGate < 0) PreGate = 0;
//     Int_t LongGate = setting.LongGate  / setting.Delt;
//     LongGate += PreGate;
//     // if(LongGate > setting.NSamples) LongGate = setting.NSamples;
//     Int_t ShortGate = setting.ShortGate / setting.Delt;
//     ShortGate += PreGate;
//     // if(ShortGate > setting.NSamples) ShortGate = setting.NSamples;


//     //read the data
//     UInt_t channelIndex(0);
//     for (auto channel : setting.CoincidenceChannels)
//     {
//         std::vector<Event> events;

//         // std::string waveformname = "output/waveform_CH" + std::to_string(channel) + ".txt";
//         // std::ofstream waveformfile(waveformname);//save first 100 waveforms
        
//         for (int j = setting.Folders[0]; j <= setting.Folders[1]; j++)
//         {
//             filepath = setting.Directory + '/' + std::to_string(j) + '/' + "channel" + std::to_string(channel) + ".bin";
//             // std::cout << filepaths[i-setting.NFolders[0]] << std::endl;
//             // std::cout << filepaths[i-setting.NFolders[0]].length() << std::endl;

//             //binary
//             if (setting.Filetype == 0)
//             {
//                 std::ifstream fileptr;
//                 fileptr.open(filepath, std::ios::in | std::ios::binary);
//                 std::cout << "Opening binary file: " << filepath << std::endl;
//                 if (!fileptr.is_open())
//                 {
//                     std::cout << "Cannot open file! " << filepath << std::endl;
//                     exit(1);
//                 }
//                 else
//                 {
//                     std::cout << " Open success !"<< std::endl;
//                     std::cout << " Reading data: "<< std::endl;
//                 }

//                 //Bool_t badflag=false;

//                 UInt_t currentNumber = 0;
//                 UInt_t offset = pairs[currentNumber][channelIndex];
//                 //std::ofstream samplefile("rawsamples"+std::to_string(channel));
//                 while (!fileptr.eof())
//                 {
//                     //read headers
//                     // for (int k = 0; k != setting.NHeaders && !fileptr.eof(); k++)
//                     // {
//                     //     fileptr.read((char *)&buffer, sizeof(UInt_t));
//                     //     header[k] = buffer;
//                     // }
//                     fileptr.ignore(offset * eventsize);
//                     // ignore headers
//                     //fileptr.ignore(setting.Headersize);
//                     fileptr.ignore(setting.Headersize[0] + setting.Headersize[1]);
//                     fileptr.read((char *)&(timestamptemp), setting.Headersize[2]);
//                     fileptr.read((char *)&(EnergyLong), setting.Headersize[3]);
//                     fileptr.read((char *)&(EnergyShort), setting.Headersize[4]);
                    
//                     timestampsTemp.push_back(timestamptemp);
//                     EnergyLongTemp.push_back(EnergyLong);
//                     EnergyShortTemp.push_back(EnergyShort);
//                     fileptr.ignore(setting.Headersize[5] + setting.Headersize[6]);


//                     //read samples
//                     for (int k = 0; k != setting.NSamples && !fileptr.eof(); k++)
//                     {
//                         fileptr.read((char *)&sample, sizeof(UShort_t));
//                         samples[k] = sample;
//                     }
//                     if (samples.size() < setting.NSamples)
//                         continue;
//                     pulsesTemp.push_back(samples);
//                     // for(int k=0;k != setting.NSamples;k++)
//                     // {
//                     //     samplefile << samples[k] << '\t';
//                     // }
//                     // samplefile << '\n';

//                     currentNumber ++;
//                     if(currentNumber < pairs.size())
//                     {
//                         offset = pairs[currentNumber][channelIndex] - pairs[currentNumber-1][channelIndex] - 1;
//                     }
                    
//                     else
//                     {
//                         break;
//                     }
                    
//                 }
//                 fileptr.close();
//                 //samplefile.close();
//             }
//             // ASCII
//             else if (setting.Filetype == 1)
//             {
//                 /* read data */
//             }

//             if (pulsesTemp.size() < pairs.size() ||
//                 timestampsTemp.size() != pulsesTemp.size())
//             {
//                 std::cout << timestampsTemp.size() << '\t' << pulsesTemp.size()<< '\t' << pairs.size()<<" read failed !" << std::endl;
//                 exit(1);
//             }
            
//             else
//             {
//                 std::cout << " Read success !" << std:: endl;
//             }
            

//             const UInt_t PulseNum = pulsesTemp.size();

//             Float_t baseline(0);
//             Float_t heightTemp(0);
//             // UShort_t heightIndex(0);

//             std::cout << "Converting the unit to Volts..." << std::endl;
//             const Double_t convertConst = setting.DynamicRange / (pow(2, setting.Resolution) - 1);
//             for (Int_t index = 0; index < PulseNum; index++)
//             {
//                 Event newPulse;
//                 newPulse.timeStampHeader = timestampsTemp[index];
//                 newPulse.ergLong = EnergyLongTemp[index];
//                 newPulse.egrShort = EnergyShortTemp[index];
//                 samples = pulsesTemp[index];

//                 // if (index < 100)
//                 // {
//                 //     for (int k = 0; k < setting.NSamples; k++)
//                 //     {
//                 //         waveformfile << samples[k] << '\t';
//                 //     }
//                 //     waveformfile << '\n';
//                 // }
                

//                 //calculate the baseline.
//                 baseline = 0;
//                 for (int k = 0; k < setting.Offset && k < setting.NSamples; k++)
//                 {
//                     baseline += samples[k];
//                 }
//                 baseline = baseline / setting.Offset;

//                 //convert to voltage
//                 for (int k = 0; k < setting.NSamples; k++)
//                 {
//                     heightTemp = (baseline - samples[k]) * convertConst; // fabs(samples[k] - baseline) * convertConst;
//                     newPulse.voltage.push_back(heightTemp);
//                     // newPulse.samples.push_back(samples[k]);
//                     //std::cout << newPulse.voltage[k] <<std::endl;
//                 }

//                 // calculate pulse height and height index, total integral and energy
//                 newPulse.heightindex = std::max_element(newPulse.voltage.begin(), newPulse.voltage.end()) - newPulse.voltage.begin();
//                 newPulse.height = *(newPulse.voltage.begin() + newPulse.heightindex);
//                 // for (int j = newPulse.heightindex - setting.PreGate; j <= newPulse.heightindex + setting.LongGate && j < setting.NSamples; j++)
//                 // {
//                 //     newPulse.totalIntegral += newPulse.voltage[j];
//                 // }
//                 for (int j = PreGate; j < LongGate && j < setting.NSamples; j++)
//                 {
//                     newPulse.totalIntegral +=  newPulse.voltage[j];
//                 }
                
//                 for (int j = PreGate; j < ShortGate && j < setting.NSamples; j++)
//                 {
//                     newPulse.tailIntegral += newPulse.voltage[j];
//                 }
//                 newPulse.tailIntegral = newPulse.totalIntegral - newPulse.tailIntegral;

//                 // energy calibration
//                 if (setting.CalicoefsPHD[channelIndex] != 1)
//                 {
//                     newPulse.energy = setting.CalicoefsPHD[channelIndex] * newPulse.height;
//                 }
//                 if (setting.CalicoefsPID[channelIndex] != 1)
//                 {
//                     newPulse.energy = setting.CalicoefsPID[channelIndex] * newPulse.totalIntegral;
//                 }
                
//                 events.push_back(newPulse);
//                 // voltagesTemp.push_back(voltageTemp);
//             }
//             timestampsTemp.clear();
//             EnergyShortTemp.clear();
//             EnergyLongTemp.clear();
//             //std::vector<std::vector<UInt_t>>().swap(headersTemp);
//             pulsesTemp.clear();
//             // std::vector<std::vector<UShort_t>>().swap(pulsesTemp);
//             std::cout << " Conversion success !" << std::endl;
//         }
//         coincidentEvents.push_back(events);
//         events.clear();
//         channelIndex ++;
//     }

//     return 0;
// }

// Int_t readEvents(const Parameters &setting,
//                 std::vector<std::vector<Event>> &multichannelevents)
// {
//     std::string filepath;
//     ULong64_t timestamptemp;
//     std::vector<ULong64_t> timestampsTemp;
//     UShort_t EnergyLong;
//     std::vector<UShort_t> EnergyLongTemp;
//     UShort_t EnergyShort;
//     std::vector<UShort_t> EnergyShortTemp;
//     UShort_t sample;
//     std::vector<UShort_t> samples(setting.NSamples);
//     std::vector<std::vector<UShort_t> > pulsesTemp;
//     std::vector<Float_t> voltageTemp(setting.NSamples);
//     //std::vector<std::vector<Float_t>> voltagesTemp;
//     std::vector<Event> eventsTemp;
//     UInt_t totalheadersize = std::accumulate(setting.Headersize.begin(),setting.Headersize.end(),0); // total header size
//     // UInt_t tailheadersize = std::accumulate(setting.Headersize.begin() + 3,setting.Headersize.end(),0); // size of Energy(ch) + Energy Short(ch) + Flags + Number of Samples
//     UInt_t eventsize = totalheadersize + setting.NSamples * setting.Samplesize;
    
//     Int_t PreTrig = setting.PreTrigger / setting.Delt;
//     Int_t PreGate = setting.PreGate / setting.Delt;
//     PreGate = PreTrig - PreGate;
//     if(PreGate < 0) PreGate = 0;
//     Int_t LongGate = setting.LongGate  / setting.Delt;
//     LongGate += PreGate;
//     // if(LongGate > setting.NSamples) LongGate = setting.NSamples;
//     Int_t ShortGate = setting.ShortGate / setting.Delt;
//     ShortGate += PreGate;
//     // if(ShortGate > setting.NSamples) ShortGate = setting.NSamples;

//     //read the data
//     UInt_t channelIndex(0);
//     for (auto channel : setting.Channels)
//     {
//         // std::string waveformname = "output/waveform_CH" + std::to_string(channel) + ".txt";
//         // std::ofstream waveformfile(waveformname);//save first 100 waveforms

//         std::vector<Event> events;
//         UInt_t currentNumber = 0;
//         for (int j = setting.Folders[0]; j <= setting.Folders[1]; j++)
//         {
//             filepath = setting.Directory + '/' + std::to_string(j) + '/' + "channel" + std::to_string(channel) + ".bin";
//             // std::cout << filepaths[i-setting.NFolders[0]] << std::endl;
//             // std::cout << filepaths[i-setting.NFolders[0]].length() << std::endl;

//             //binary
//             if (setting.Filetype == 0)
//             {
//                 std::ifstream fileptr;
//                 fileptr.open(filepath, std::ios::in | std::ios::binary);
//                 std::cout << "Trying to open binary file: " << filepath << std::endl;
//                 if (!fileptr.is_open())
//                 {
//                     std::cout << "Cannot open file! " << filepath << std::endl;
//                     exit(1);
//                 }
//                 else
//                 {
//                     std::cout << " Open success !\n" << " Reading data: " << std::endl;
//                 }

//                 //Bool_t badflag=false;

//                 //UInt_t offset = pairs[currentNumber][channel];
//                 // std::ofstream samplefile("rawsamples"+std::to_string(channel));
//                 while (!fileptr.eof() && currentNumber < setting.MaxNumPulses)
//                 {
//                     //read headers
//                     // for (int k = 0; k != setting.NHeaders && !fileptr.eof(); k++)
//                     // {
//                     //     fileptr.read((char *)&buffer, sizeof(UInt_t));
//                     //     header[k] = buffer;
//                     // }
//                     //fileptr.ignore(offset * eventsize);
//                     // ignore headers
//                     //fileptr.ignore(setting.Headersize);
//                     fileptr.ignore(setting.Headersize[0] + setting.Headersize[1]);
//                     fileptr.read((char *)&(timestamptemp), setting.Headersize[2]);
//                     fileptr.read((char *)&(EnergyLong), setting.Headersize[3]);
//                     fileptr.read((char *)&(EnergyShort), setting.Headersize[4]);
                    
//                     timestampsTemp.push_back(timestamptemp);
//                     EnergyLongTemp.push_back(EnergyLong);
//                     EnergyShortTemp.push_back(EnergyShort);
//                     fileptr.ignore(setting.Headersize[5] + setting.Headersize[6]);

//                     //read samples
//                     for (int k = 0; k != setting.NSamples && !fileptr.eof(); k++)
//                     {
//                         fileptr.read((char *)&sample, sizeof(UShort_t));
//                         samples[k] = sample;
//                     }
//                     if (samples.size() < setting.NSamples)
//                         continue;
//                     pulsesTemp.push_back(samples);
//                     // for(int k=0;k != setting.NSamples;k++)
//                     // {
//                     //     samplefile << samples[k] << '\t';
//                     // }
//                     // samplefile << '\n';

//                     currentNumber ++;
//                 }
//                 fileptr.close();
//                 // samplefile.close();
//             }
//             // ASCII
//             else if (setting.Filetype == 1)
//             {
//                 /* read data */
//             }

//             if (timestampsTemp.size() != pulsesTemp.size())
//             {
//                 std::cout << timestampsTemp.size() << '\t' << pulsesTemp.size()<< '\t' <<" read failed !" << std::endl;
//                 exit(1);
//             }
            
//             else
//             {
//                 std::cout << " Read success !" << std:: endl;
//             }
            

//             const UInt_t PulseNum = pulsesTemp.size();

//             Float_t baseline(0);
//             Float_t heightTemp(0);
//             // UShort_t heightIndex(0);

//             std::cout << "Converting the unit to Volts..." << std::endl;
//             const Double_t convertConst = setting.DynamicRange / (pow(2, setting.Resolution) - 1);
//             for (Int_t index = 0; index < PulseNum; index++)
//             {
//                 Event newPulse;
//                 newPulse.timeStampHeader = timestampsTemp[index];
//                 newPulse.ergLong = EnergyLongTemp[index];
//                 newPulse.egrShort = EnergyShortTemp[index];
//                 samples = pulsesTemp[index];

//                 // if (index < 100)
//                 // {
//                 //     for (int k = 0; k < setting.NSamples; k++)
//                 //     {
//                 //         waveformfile << samples[k] << '\t';
//                 //     }
//                 //     waveformfile << '\n';
//                 // }

//                 //calculate the baseline.
//                 baseline = 0;
//                 for (int k = 0; k < setting.Offset && k < setting.NSamples; k++)
//                 {
//                     baseline += samples[k];
//                 }
//                 baseline = baseline / setting.Offset;

//                 //convert to voltage
//                 for (int k = 0; k < setting.NSamples; k++)
//                 {
//                     heightTemp = setting.Polarity * (samples[k] - baseline) * convertConst; //fabs(samples[k] - baseline) * convertConst;
//                     newPulse.voltage.push_back(heightTemp);
//                     // newPulse.samples.push_back(samples[k]);
//                     //std::cout << newPulse.voltage[k] <<std::endl;
//                 }

//                 // calculate pulse height and height index
//                 newPulse.heightindex = std::max_element(newPulse.voltage.begin(), newPulse.voltage.end()) - newPulse.voltage.begin();
//                 newPulse.height = *(newPulse.voltage.begin() + newPulse.heightindex);
//                 // for (int j = newPulse.heightindex - setting.PreGate; j <= newPulse.heightindex + setting.LongGate && j < setting.NSamples; j++)
//                 // {
//                 //     newPulse.totalIntegral += newPulse.voltage[j];
//                 // }
//                 for (int j = PreGate; j < LongGate && j < setting.NSamples; j++)
//                 {
//                     newPulse.totalIntegral +=  newPulse.voltage[j];
//                 }
                
//                 // energy calibration
//                 if (setting.CalicoefsPHD[channelIndex] != 1)
//                 {
//                     newPulse.energy = setting.CalicoefsPHD[channelIndex] * newPulse.height;
//                 }
//                 if (setting.CalicoefsPID[channelIndex] != 1)
//                 {
//                     newPulse.energy = setting.CalicoefsPID[channelIndex] * newPulse.totalIntegral;
//                 }
                
//                 for (int j = PreGate; j < ShortGate && j < setting.NSamples; j++)
//                 {
//                     newPulse.tailIntegral += newPulse.voltage[j];
//                 }
//                 newPulse.tailIntegral = newPulse.totalIntegral - newPulse.tailIntegral;

//                 events.push_back(newPulse);
//                 // voltagesTemp.push_back(voltageTemp);
//             }
//             timestampsTemp.clear();
//             EnergyShortTemp.clear();
//             EnergyLongTemp.clear();
//             //std::vector<std::vector<UInt_t>>().swap(headersTemp);
//             pulsesTemp.clear();
//             // std::vector<std::vector<UShort_t>>().swap(pulsesTemp);
//             std::cout << " Conversion success !" << std::endl;
//         }
//         multichannelevents.push_back(events);
//         std::cout << currentNumber << " input pulses in channel" << std::to_string(channel)<< std::endl;
//         events.clear();
//         channelIndex ++;
//     }

//     return 0;
// }

// Int_t readEventsBuf(const Parameters &setting,
//                 std::vector<std::vector<Event>> &multichannelevents)
// {
//     std::string filepath;
//     ULong64_t timestamptemp;
//     // std::vector<ULong64_t> timestampsTemp;
//     UShort_t EnergyLong;
//     // std::vector<UShort_t> EnergyLongTemp;
//     UShort_t EnergyShort;
//     // std::vector<UShort_t> EnergyShortTemp;
//     UShort_t sample;
//     std::vector<UShort_t> samples(setting.NSamples);
//     // std::vector<std::vector<UShort_t> > pulsesTemp;
//     // std::vector<Float_t> voltageTemp(setting.NSamples);
//     //std::vector<std::vector<Float_t>> voltagesTemp;
//     std::vector<Event> eventsTemp;
//     UInt_t BOCH_SIZE = setting.Headersize[0] + setting.Headersize[1];
//     UInt_t totalheadersize = std::accumulate(setting.Headersize.begin(),setting.Headersize.end(),0); // total header size
//     // UInt_t tailheadersize = std::accumulate(setting.Headersize.begin() + 3,setting.Headersize.end(),0); // size of Energy(ch) + Energy Short(ch) + Flags + Number of Samples
//     UInt_t eventsize = totalheadersize + setting.NSamples * setting.Samplesize;
//     const Double_t convertConst = setting.DynamicRange / (pow(2, setting.Resolution) - 1);

//     // create a buffer
//     ULong_t BUFFERSIZE = 1024*1024*64 / eventsize; // buffer size = 64MB
//     BUFFERSIZE = BUFFERSIZE * eventsize;
//     char * buffer = new char[BUFFERSIZE]; 

//     Int_t PreTrig = setting.PreTrigger / setting.Delt;
//     Int_t PreGate = setting.PreGate / setting.Delt;
//     PreGate = PreTrig - PreGate;
//     if(PreGate < 0) PreGate = 0;
//     Int_t LongGate = setting.LongGate  / setting.Delt;
//     LongGate += PreGate;
//     // if(LongGate > setting.NSamples) LongGate = setting.NSamples;
//     Int_t ShortGate = setting.ShortGate / setting.Delt;
//     ShortGate += PreGate;
//     // if(ShortGate > setting.NSamples) ShortGate = setting.NSamples;

//     //read the data
//     UInt_t channelIndex(0);
//     for (auto channel : setting.Channels)
//     {
//         // std::string waveformname = "output/waveform_CH" + std::to_string(channel) + ".txt";
//         // std::ofstream waveformfile(waveformname);//save first 100 waveforms

//         std::vector<Event> events;
//         UInt_t currentNumber = 0;
//         for (int j = setting.Folders[0]; j <= setting.Folders[1]; j++)
//         {
//             filepath = setting.Directory + '/' + std::to_string(j) + '/' + "channel" + std::to_string(channel) + ".bin";
//             // std::cout << filepaths[i-setting.NFolders[0]] << std::endl;
//             // std::cout << filepaths[i-setting.NFolders[0]].length() << std::endl;

//             //binary
//             if (setting.Filetype == 0)
//             {
//                 std::ifstream fileptr;
//                 fileptr.open(filepath, std::ios::in | std::ios::binary);
//                 std::cout << "Trying to open binary file: " << filepath << std::endl;
//                 if (!fileptr.is_open())
//                 {
//                     std::cout << "Cannot open file! " << filepath << std::endl;
//                     exit(1);
//                 }
//                 else
//                 {
//                     std::cout << " Open success !\n" << " Reading data: " << std::endl;
//                 }

//                 //Bool_t badflag=false;

//                 //UInt_t offset = pairs[currentNumber][channel];
//                 // std::ofstream samplefile("rawsamples"+std::to_string(channel));
//                 ULong_t bufIndex = 0;
//                 while (!fileptr.eof() && currentNumber < setting.MaxNumPulses)
//                 {
//                     // fileptr.read((char *)&(timestamptemp), setting.Headersize[2]);
//                     // fileptr.read((char *)&(EnergyLong), setting.Headersize[3]);
//                     // fileptr.read((char *)&(EnergyShort), setting.Headersize[4]);

//                     fileptr.read(buffer, BUFFERSIZE);
//                     BUFFERSIZE = fileptr.gcount();
//                     bufIndex = 0;

//                     // extract pulses from buffer
//                     while (bufIndex < BUFFERSIZE && currentNumber < setting.MaxNumPulses)
//                     {
//                         // read header
//                         bufIndex += BOCH_SIZE;
//                         std::copy(&buffer[bufIndex], &buffer[bufIndex] + setting.Headersize[2], 
//                                 reinterpret_cast<char *>(&timestamptemp));
//                         bufIndex += setting.Headersize[2];
//                         std::copy(&buffer[bufIndex], &buffer[bufIndex] + setting.Headersize[3], 
//                                 reinterpret_cast<char *>(&EnergyLong));
//                         bufIndex += setting.Headersize[3];
//                         std::copy(&buffer[bufIndex], &buffer[bufIndex] + setting.Headersize[4], 
//                                 reinterpret_cast<char *>(&EnergyShort));
//                         bufIndex = bufIndex + setting.Headersize[4] + setting.Headersize[5] + setting.Headersize[6];
//                         // read samples
//                         for (int k = 0; k != setting.NSamples && bufIndex < BUFFERSIZE; k++)
//                         {
//                             std::copy(&buffer[bufIndex], &buffer[bufIndex] + setting.Samplesize, 
//                                     reinterpret_cast<char *>(&sample));
//                             // fileptr.read((char *)&sample, sizeof(UShort_t));
//                             samples[k] = sample;
//                             bufIndex += setting.Samplesize;
//                         }
//                         if (samples.size() < setting.NSamples)
//                         {
//                             continue;
//                         }
//                         currentNumber ++;

//                         // processing
//                         Float_t baseline(0);
//                         Float_t heightTemp(0);
//                         // UShort_t heightIndex(0);
//                         Float_t psdtemp(0);

//                         Event newPulse;
//                         newPulse.timeStampHeader = timestamptemp;
//                         newPulse.ergLong = EnergyLong;
//                         newPulse.egrShort = EnergyShort;

//                         //calculate the baseline.
//                         baseline = 0;
//                         for (int k = 0; k < setting.Offset && k < setting.NSamples; k++)
//                         {
//                             baseline += samples[k];
//                         }
//                         baseline = baseline / setting.Offset;

//                         //convert to voltage
//                         for (int k = 0; k < setting.NSamples; k++)
//                         {
//                             heightTemp = setting.Polarity * (samples[k] - baseline) * convertConst; //fabs(samples[k] - baseline) * convertConst;
//                             newPulse.voltage.push_back(heightTemp);
//                             // newPulse.samples.push_back(samples[k]);
//                             //std::cout << newPulse.voltage[k] <<std::endl;
//                         }

//                         // calculate pulse height and height index
//                         newPulse.heightindex = std::max_element(newPulse.voltage.begin(), newPulse.voltage.end()) - newPulse.voltage.begin();
//                         newPulse.height = *(newPulse.voltage.begin() + newPulse.heightindex);
//                         // for (int j = newPulse.heightindex - setting.PreGate; j <= newPulse.heightindex + setting.LongGate && j < setting.NSamples; j++)
//                         // {
//                         //     newPulse.totalIntegral += newPulse.voltage[j];
//                         // }
//                         for (int j = PreGate; j < LongGate && j < setting.NSamples; j++)
//                         {
//                             newPulse.totalIntegral +=  newPulse.voltage[j];
//                         }
                        
//                         // energy calibration
//                         if (setting.CalicoefsPHD[channelIndex] != 1)
//                         {
//                             newPulse.energy = setting.CalicoefsPHD[channelIndex] * newPulse.height;
//                         }
//                         if (setting.CalicoefsPID[channelIndex] != 1)
//                         {
//                             newPulse.energy = setting.CalicoefsPID[channelIndex] * newPulse.totalIntegral;
//                         }
                        
//                         for (int j = PreGate; j < ShortGate && j < setting.NSamples; j++)
//                         {
//                             newPulse.tailIntegral += newPulse.voltage[j];
//                         }
//                         newPulse.tailIntegral = newPulse.totalIntegral - newPulse.tailIntegral;

//                         events.push_back(newPulse);
//                     }
//                     std::cout << currentNumber << " pulses.."<< std::endl;
//                 }
//                 fileptr.close();
//                 // samplefile.close();
//             }
//             // ASCII
//             else if (setting.Filetype == 1)
//             {
//                 std::cout << "Sorry, ASCII is not supported.." << std::endl;
//             }
//         }
//         multichannelevents.push_back(events);
//         std::cout << currentNumber << " input pulses in channel" << std::to_string(channel)<< std::endl;
//         events.clear();
//         channelIndex ++;
//     }
//     delete[] buffer;

//     return 0;
// }

Int_t getSampleNum(Parameters &setting)
{
    // get number of samples from the header
    // for CoMPASS only
    std::string filepath;

    //read the data
    UInt_t channelIndex(0);
    Int_t sampleNum(0);
    UInt_t channel = setting.Channels[0];
    for (int j = setting.Folders[0]; j <= setting.Folders[1]; j++)
    {
        filepath = setting.Directory + '/' + std::to_string(j) + '/' + "channel" + std::to_string(channel) + ".bin";
        // std::cout << filepaths[i-setting.NFolders[0]] << std::endl;
        // std::cout << filepaths[i-setting.NFolders[0]].length() << std::endl;

        //binary
        if (setting.Filetype == 0)
        {
            std::ifstream fileptr;
            fileptr.open(filepath, std::ios::in | std::ios::binary);
            // std::cout << "Trying to open binary file: " << filepath << std::endl;
            if (!fileptr.is_open())
            {
                std::cout << "Cannot open file: " << filepath << std::endl;
                exit(1);
            }
            else
            {
                std::cout << " Number of samples in each pulse:";
            }

            //Bool_t badflag=false;

            //UInt_t offset = pairs[currentNumber][channel];
            // std::ofstream samplefile("rawsamples"+std::to_string(channel));
            while (!fileptr.eof())
            {
                //read header
                fileptr.ignore(std::accumulate(setting.Headersize.begin(),setting.Headersize.end()-1, 0));

                fileptr.read((char *)&(sampleNum), setting.Headersize[6]);
                // fileptr.ignore(setting.Headersize[5] + setting.Headersize[6]);

                break;
            }
            fileptr.close();
            // samplefile.close();
        }
    }
    setting.NSamples = sampleNum;
    std::cout << sampleNum << std::endl;
    return 0;
}

Int_t readData(const Parameters &setting, const UInt_t channelIndex, char *buffer,
               const ULong_t BUFFERSIZE, ULong_t& currentNumber,
               std::vector<Event> &Events, std::vector<std::vector< Float_t > > &Pulses)
{
    ULong_t bufIndex = 0;
    UInt_t BOCH_SIZE = setting.Headersize[0] + setting.Headersize[1];
    // UInt_t totalheadersize = std::accumulate(setting.Headersize.begin(),setting.Headersize.end(),0); // total header size
    // UInt_t tailheadersize = std::accumulate(setting.Headersize.begin() + 3,setting.Headersize.end(),0); // size of Energy(ch) + Energy Short(ch) + Flags + Number of Samples
    // UInt_t eventsize = totalheadersize + setting.NSamples * setting.Samplesize;
    const Double_t convertConst = setting.DynamicRange / (pow(2, setting.Resolution) - 1);
    Int_t PreTrig = setting.PreTrigger / setting.Delt;
    Int_t PreGate = setting.PreGate / setting.Delt;
    PreGate = PreTrig - PreGate;
    if(PreGate < 0) PreGate = 0;
    Int_t LongGate = setting.LongGate  / setting.Delt;
    LongGate += PreGate;
    // if(LongGate > setting.NSamples) LongGate = setting.NSamples;
    Int_t ShortGate = setting.ShortGate / setting.Delt;
    ShortGate += PreGate;

    // extract pulses from buffer
    ULong64_t timestamptemp;
    UShort_t EnergyLong;
    UShort_t EnergyShort;
    UShort_t sample;
    std::vector<UShort_t> samples(setting.NSamples);
    std::vector<Float_t> voltageTemp(setting.NSamples);
    ULong_t pulseIndex(0);
    while (bufIndex < BUFFERSIZE && currentNumber < setting.MaxNumPulses)
    {
        // read header
        bufIndex += BOCH_SIZE;
        std::copy(&buffer[bufIndex], &buffer[bufIndex] + setting.Headersize[2], 
                reinterpret_cast<char *>(&timestamptemp));
        bufIndex += setting.Headersize[2];
        std::copy(&buffer[bufIndex], &buffer[bufIndex] + setting.Headersize[3], 
                reinterpret_cast<char *>(&EnergyLong));
        bufIndex += setting.Headersize[3];
        std::copy(&buffer[bufIndex], &buffer[bufIndex] + setting.Headersize[4], 
                reinterpret_cast<char *>(&EnergyShort));
        bufIndex = bufIndex + setting.Headersize[4] + setting.Headersize[5] + setting.Headersize[6];
        // read samples
        for (int k = 0; k != setting.NSamples && bufIndex < BUFFERSIZE; k++)
        {
            std::copy(&buffer[bufIndex], &buffer[bufIndex] + setting.Samplesize, 
                    reinterpret_cast<char *>(&sample));
            // fileptr.read((char *)&sample, sizeof(UShort_t));
            samples[k] = sample;
            bufIndex += setting.Samplesize;
        }
        if (samples.size() < setting.NSamples)
        {
            std::cout << "Error when reading data!" << std::endl;
            exit(1);
        }
        currentNumber ++;
        pulseIndex ++;

        // processing
        Float_t baseline(0);
        Float_t heightTemp(0);
        // UShort_t heightIndex(0);
        // Float_t psdtemp(0);

        Event newPulse;
        newPulse.timeStampHeader = timestamptemp;
        newPulse.ergLong = EnergyLong;
        newPulse.egrShort = EnergyShort;

        //calculate the baseline.
        baseline = 0;
        for (int k = 0; k < setting.Offset && k < setting.NSamples; k++)
        {
            baseline += samples[k];
        }
        baseline = baseline / setting.Offset;

        //convert to voltage
        for (int k = 0; k < setting.NSamples; k++)
        {
            heightTemp = setting.Polarity * (samples[k] - baseline) * convertConst; //fabs(samples[k] - baseline) * convertConst;
            voltageTemp[k]=heightTemp;
            // newPulse.samples.push_back(samples[k]);
            //std::cout << newPulse.voltage[k] <<std::endl;
        }

        // calculate pulse height and height index
        newPulse.heightindex = std::max_element(voltageTemp.begin(), voltageTemp.end()) - voltageTemp.begin();
        newPulse.height = *(voltageTemp.begin() + newPulse.heightindex);
        // for (int j = newPulse.heightindex - setting.PreGate; j <= newPulse.heightindex + setting.LongGate && j < setting.NSamples; j++)
        // {
        //     newPulse.totalIntegral += newPulse.voltage[j];
        // }
        for (int j = PreGate; j < LongGate && j < setting.NSamples; j++)
        {
            newPulse.totalIntegral +=  voltageTemp[j];
        }
        
        // energy calibration
        if (setting.CalicoefsPHD[channelIndex] != 1)
        {
            newPulse.energy = setting.CalicoefsPHD[channelIndex] * newPulse.height;
        }
        if (setting.CalicoefsPID[channelIndex] != 1)
        {
            newPulse.energy = setting.CalicoefsPID[channelIndex] * newPulse.totalIntegral;
        }
        
        for (int j = PreGate; j < ShortGate && j < setting.NSamples; j++)
        {
            newPulse.tailIntegral += voltageTemp[j];
        }
        newPulse.tailIntegral = newPulse.totalIntegral - newPulse.tailIntegral;

        Events.push_back(newPulse);
        Pulses.push_back(voltageTemp);
    }
    std::cout << " " << pulseIndex << " input, ";
    return 0;
}
// Int_t newPulseHeight(const Parameters &setting,
//                 std::vector< std::vector< Event > > &coincidentEvents)
// {
//     Float_t integral(0);
//     for(auto channel : setting.CoincidenceChannels)
//     {
//         for(int index =0; index < coincidentEvents[channel].size(); index++)
//         {
//             integral = 0;
//             for(int j = 0; j < setting.NSamples; j++)
//             {
//                 integral += coincidentEvents[channel][index].voltage[j];
//             }
//             if(channel == 0)
//             {
//                 coincidentEvents[channel][index].height = (integral - 0.0176261) / 5.64301;
//             }else if (channel == 1)
//             {
//                 coincidentEvents[channel][index].height = (integral - 0.00832334) / 4.89559;
//             }
//         }
//     }
// }