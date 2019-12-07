#include <vector>
#include <ctime>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* ROOT */
#include "TApplication.h"
#include "TCanvas.h"
#include "TH1F.h"
// #include "TF1.h"
// #include "TGraph.h"
// #include "TMultiGraph.h"

#include "getInputParameters.h"
#include "events.h"
#include "readData.h"
#include "output.h"
#include "rejection.h"
// #include "getTimeStamp.h"
// #include "coincidence.h"
#include "getPlot.h"

int main(int argc, char *argv[])
{
    struct stat st = {0};
    // create directories to save the output files and pics, works in Linux
    if (stat("output", &st) == -1) {
        mkdir("output", 0700);
    }

    auto startTime = std::chrono::high_resolution_clock::now();
    const std::string settingFilePath("input.txt");
    Parameters setting;
    //const std::string dataFilePath("/mnt/d/Projects/PSD/1/wave4.dat");
    //std::vector<Event> pulses;
    TApplication *myapp = new TApplication("myApp", 0, 0);

    // step 0 read input parameters
    getInputParameters(settingFilePath, setting);
    getSampleNum(setting);
    // return 0;

    //// step 1, read the data in binary/ASCII format, calculate the baseline, convert to voltage, calculate heightIndex.
    std::vector< std::vector< Event > > multiCHEvents(setting.Channels.size()); // store the data processing results
    std::vector<std::vector< Float_t > > Pulses; // store the pulses temporarily

    // iterate over channels
    // iterate over chuncks
    // process for each chuck
    // release memory occupied by Pulses
    // read data
    std::string filepath;
    std::string outname;
    for(int i=0;i<setting.Channels.size();i++)
    {
        UShort_t channel = setting.Channels[i];
        std::cout << "*****Reading pulses in Channel" << '\t';
        std::cout << channel << '\t' <<"*****"<< std::endl;
        UInt_t eventsize = std::accumulate(setting.Headersize.begin(),setting.Headersize.end(),0)+ setting.NSamples * setting.Samplesize;

        // store good, bad and piled-up pulses that will be saved in txt file later
        std::vector<std::vector< Float_t > > goodPulses;
        std::vector<std::vector< Float_t > > badPulses;
        std::vector<std::vector< Float_t > > pileupPulses;
        UInt_t goodCount(0);
        const UInt_t good2Save = (10 > setting.SavePulses ? 10 : setting.SavePulses);
        const UInt_t bad2Save = (10 > setting.SavePulses ? 10 : setting.SavePulses);
        const UInt_t piledup2Save = (10 > setting.SavePulses ? 10 : setting.SavePulses);
        UInt_t badCount(0);
        UInt_t pileupCount(0);
        
        // create a buffer
        ULong_t BUFFERSIZE = 1024*1024*64 / eventsize; // buffer size = 64MB
        BUFFERSIZE = BUFFERSIZE * eventsize;
        char * buffer = new char[BUFFERSIZE]; 
        ULong_t previousNumber = 0;
        ULong_t currentNumber = 0;
        ULong_t bufferNumber = 0;
        for (int j = setting.Folders[0]; j <= setting.Folders[1]; j++)
        {
            filepath = setting.Directory + '/' + std::to_string(j) + '/' + "channel" + std::to_string(channel) + ".bin";
            //binary
            if (setting.Filetype == 0)
            {
                std::ifstream fileptr;
                fileptr.open(filepath, std::ios::in | std::ios::binary);
                std::cout << "Trying to open binary file: " << filepath << std::endl;
                if (!fileptr.is_open())
                {
                    std::cout << "Cannot open file! " << filepath << std::endl;
                    exit(1);
                }
                else
                {
                    std::cout << " Open success !\n" << " Reading data: " << std::endl;
                }
                // ULong_t bufIndex = 0;
                while (!fileptr.eof() && currentNumber < setting.MaxNumPulses)
                {
                    fileptr.read(buffer, BUFFERSIZE);
                    BUFFERSIZE = fileptr.gcount();

                    previousNumber = currentNumber;
                    readData(setting, i, buffer, BUFFERSIZE, currentNumber, multiCHEvents[i], Pulses);

                    // filtering...
                    if (setting.FilterBad) 
                    {
                        rejection(setting, previousNumber, multiCHEvents[i], Pulses);
                    }
                    if (setting.FilterPiledup)
                    {
                        simplePUR(setting, previousNumber, multiCHEvents[i], Pulses);
                    }
                    std::cout << " " << currentNumber << " pulses in total." << std::endl;
                    if(setting.EnergyCut)
                    {
                        energyCut(setting, multiCHEvents[i]);
                    }
                    if(setting.PSDCut[i])
                    {
                        PSDCut(setting, i, multiCHEvents[i]);
                    }

                    // save good pulses
                    if(goodCount < good2Save)
                    {
                        // save N good pulses
                        // outname = "output/goodpulsesch" + std::to_string(setting.Channels[i]);
                        ULong_t jj(0);
                        std::vector<Float_t> pulse(setting.NSamples);
                        for (int j = 0; j < Pulses.size() && goodCount < good2Save; j++)
                        {
                            jj = j + previousNumber;
                            if (!multiCHEvents[i][jj].isBad)
                            {
                                for (int k = 0; k < setting.NSamples; k++)
                                {
                                    pulse[k] = Pulses[j][k];
                                }
                                goodPulses.push_back(pulse);
                                goodCount ++;
                            }
                        }
                    }
                    // save bad pulses
                    if(badCount < bad2Save)
                    {
                        // save N bad pulses
                        // outname = "output/goodpulsesch" + std::to_string(setting.Channels[i]);
                        ULong_t jj(0);
                        std::vector<Float_t> pulse(setting.NSamples);
                        for (int j = 0; j < Pulses.size() && badCount < bad2Save; j++)
                        {
                            jj = j + previousNumber;
                            if (multiCHEvents[i][jj].isBad)
                            {
                                for (int k = 0; k < setting.NSamples; k++)
                                {
                                    pulse[k] = Pulses[j][k];
                                }
                                badPulses.push_back(pulse);
                                badCount ++;
                            }
                        }
                    }
                    // save pile-up pulses
                    if(pileupCount < piledup2Save)
                    {
                        // save N pile-up pulses
                        // outname = "output/goodpulsesch" + std::to_string(setting.Channels[i]);
                        ULong_t jj(0);
                        std::vector<Float_t> pulse(setting.NSamples);
                        for (int j = 0; j < Pulses.size() && pileupCount < piledup2Save; j++)
                        {
                            jj = j + previousNumber;
                            if (multiCHEvents[i][jj].isBad)
                            {
                                for (int k = 0; k < setting.NSamples; k++)
                                {
                                    pulse[k] = Pulses[j][k];
                                }
                                pileupPulses.push_back(pulse);
                                pileupCount ++;
                            }
                        }
                    }
                    Pulses.clear();
                }
                fileptr.close();
            }
            // ASCII
            else if (setting.Filetype == 1)
            {
                std::cout << "Sorry, ASCII is not supported.." << std::endl;
            }
        }
        std::cout <<" "<<currentNumber << " input pulses in channel" << std::to_string(channel)<< std::endl;
        delete[] buffer;
        
        // save pulses
        if(setting.SavePulses)
        {
            // save N good pulses
            outname = "output/goodPulsesCH" + std::to_string(channel);
            savePulses(setting.SavePulses, goodPulses, outname);
        }
        if(setting.SaveBad)
        {
            // save N good pulses
            outname = "output/badPulsesCH" + std::to_string(channel);
            savePulses(setting.SaveBad, badPulses, outname);
        }
        if(setting.SavePiledup)
        {
            // save N good pulses
            outname = "output/pileupPulsesCH" + std::to_string(channel);
            savePulses(setting.SavePiledup, pileupPulses, outname);
        }
        
        // plot 10 good pulses
        outname = "Pulse_CH" + std::to_string(channel);
        plotPulse(setting, goodPulses, outname);
        std::cout << " Example pulses of CH" << std::to_string(channel) << " are plotted! " << std::endl;

        goodPulses.clear();
        badPulses.clear();
        pileupPulses.clear();

    }
    // save header
    if(std::accumulate(setting.SaveHeaders.begin(),setting.SaveHeaders.end(),0))
    {
        // save headers
        for(int i = 0;i<multiCHEvents.size();i++)
        {
            outname = "output/Headers" + std::to_string(setting.Channels[i]);
            saveHeaders(setting, multiCHEvents[i], outname, [](Event pulse) { return !pulse.isBad; });
        }
    }

    if(setting.SavePH)
    {
        // save the pulse heights
        for(int i = 0;i<multiCHEvents.size();i++)
        {
            outname = "output/PH_CH" + std::to_string(setting.Channels[i]);
            savePH(setting, multiCHEvents[i], outname, [](Event pulse) { return !pulse.isBad; });
            std::cout << " Pulse heights are saved to file: " << outname <<" !"<< std::endl;
        }
    }
    if(setting.SavePI)
    {
        // save the pulse integrals
        for(int i = 0;i<multiCHEvents.size();i++)
        {
            outname = "output/PI_CH" + std::to_string(setting.Channels[i]);
            savePI(setting, multiCHEvents[i], outname, [](Event pulse) { return !pulse.isBad; });
            std::cout << " Pulse integrals are saved to file: " << outname <<" !"<< std::endl;
        }
    }

    if (setting.PHD)
    {
        for(int i=0;i<multiCHEvents.size();i++)
        {
            outname = "CH" + std::to_string(setting.Channels[i])+"_PHD";
            TCanvas *PHcanvas = new TCanvas(outname.c_str(), "PHD Canvas", 200, 10, 700, 500);
            TH1F *PHhisto = new TH1F(outname.c_str(), outname.c_str(), setting.PHDBins, setting.PHmin, setting.PHmax);
            getPHD(PHhisto, PHcanvas, setting, multiCHEvents[i], setting.CalicoefsPHD[i], [](Event pulse) { return !pulse.isBad; });
            outname = "output/" + outname +".txt";
            if (setting.SavePHD)
            {
                saveHisto(PHhisto, outname);
                std::cout << " Pulse height distribution is saved to file: " << outname <<" !"<< std::endl;
            }
        }
    }
    if (setting.PID)
    {
        for(int i=0;i<multiCHEvents.size();i++)
        {
            outname = "CH" + std::to_string(setting.Channels[i]) + "_PID";
            TCanvas *PIcanvas = new TCanvas(outname.c_str(), "PID Canvas", 200, 10, 700, 500);
            TH1F *PIhisto = new TH1F(outname.c_str(), outname.c_str(), setting.PIDBins, setting.PImin, setting.PImax); //// 2000 kevee or 20.0 V
            getPID(PIhisto, PIcanvas, setting, multiCHEvents[i], setting.CalicoefsPID[i],[](Event pulse) { return !pulse.isBad; });
            outname = "output/" + outname +".txt";
            if (setting.SavePID)
            {
                saveHisto(PIhisto, outname);
                std::cout << " Pulse integral distribution is saved to file: " << outname <<" !"<< std::endl;
            }
        }
    }

    // TBD 
    // if (setting.TimeStamp)
    // {
    //     std::cout << "*****Calculating time stamps using DIACFD..*****" << std::endl;
    //     for(int i = 0;i<mul.size();i++)
    //     {
    //         getTimeStamp(setting, coincidentEvents[i]);
    //     }
    //     std::cout << "*****Finished calculaing time stamps!*****" << std::endl;
        
    //     //// TBD: multichannel support
    //     if (setting.Coincidence && setting.SaveDT) 
    //     {
    //         // save time differences
    //         SaveDT(coincidentEvents, "output/timediff_Header", "Header");
    //         SaveDT(coincidentEvents, "output/timediff_DIACFD", "DIACFD");
    //     }
    //     if (setting.Coincidence && setting.TOF) 
    //     {
    //         TCanvas *TOFcanvas = new TCanvas("c3", "TOF Canvas", 200, 10, 700, 500);
    //         TH1F *TOFhisto = new TH1F("TOF", "TOF", 400, -3.0, 5.0);
    //         getTOF(TOFhisto, TOFcanvas, setting, coincidentEvents);
    //         if (setting.SaveTOF) 
    //         {
    //             // save ToF
    //             saveTOF(TOFhisto, "output/TOF");
    //         }
    //     }
        
    // }
    // if(setting.SaveTimeStamp)
    // {
    //     // save time stamps
    //     for(int i = 0;i<multiCHEvents.size();i++)
    //     {
    //         outname = "output/timestamp_Header_CH" + std::to_string(setting.Channels[i]);
    //         saveTimestamp(multiCHEvents[i], outname, "Header", [](Event pulse) { return !pulse.isBad; });
    //         if(setting.TimeStamp)
    //         {
    //             outname = "output/timestamp_DIACFD_CH" + std::to_string(setting.Channels[i]);
    //             saveTimestamp(coincidentEvents[i], outname, "DIACFD", [](Event pulse) { return !pulse.isBad; });
    //         }
    //     }
    // }

    if(setting.PSD)
    {
        for(int i=0;i<multiCHEvents.size();i++)
        {
            outname = "CH" + std::to_string(setting.Channels[i]) +"_PSD";
            TCanvas *PSDcanvas = new TCanvas(outname.c_str(), "PSD Canvas", 200, 10, 700, 500);
            TH2F *PSDhisto = new TH2F(outname.c_str(), outname.c_str(), setting.PSDXBins, setting.PSDXmin, setting.PSDXmax,setting.PSDYBins, setting.PSDYmin, setting.PSDYmax); //// 2000 kevee or 20.0 V
            getPSD(PSDhisto, PSDcanvas, setting, multiCHEvents[i], setting.CalicoefsPID[i],[](Event pulse) { return !pulse.isBad; });
            std::cout << " PSD of CH" << std::to_string(setting.Channels[i]) << " is plotted! " << std::endl;
            outname = "output/Integrals_CH" + std::to_string(setting.Channels[i]) +".txt";
            if (setting.SaveIntegrals)
            {
                saveIntegrals(multiCHEvents[i], outname, [](Event pulse) { return !pulse.isBad; });
                std::cout << " Tail and total integrals of CH" << std::to_string(setting.Channels[i]) << " are saved to file " << outname << "!" << std::endl;
            }
            if (setting.SavePSD)
            {
                outname = "output/PSD_CH" + std::to_string(setting.Channels[i]) +".txt";
                saveHisto2D(PSDhisto, outname);
                std::cout << " PSD of CH" << std::to_string(setting.Channels[i]) << " are saved to file " << outname << "!" << std::endl;
            }
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms" << std::endl; 
    myapp->Run();
    return 0;
}