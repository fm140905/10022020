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
#include "output.h"
#include "readData.h"
#include "rejection.h"
#include "getTimeStamp.h"
#include "coincidence.h"
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
    std::vector< std::vector< Event > > coincidentEvents;
    // coincidence
    if(setting.Coincidence)
    {
        std::cout << "*****Picking up Coincidences between Channel";
        for(int i=0;i<setting.CoincidenceChannels.size();i++)
        {
            std::cout << setting.CoincidenceChannels[i] << '\t';
        }
        std::cout <<"*****"<< std::endl;

        std::vector< std::vector<UShort_t> > flags;
        std::vector< std::vector<ULong64_t> > timeStampsHeader;
        readTimeStamp(setting, flags, timeStampsHeader);

        std::vector< std::vector< UInt_t > > coincidentIndexes;
        find_coincidence(setting, timeStampsHeader, coincidentIndexes); // find coincidence between 2 channels
        std::cout << "*****Finished picking up coincidences. ";
        // std::ofstream pairfile("pairs");
        // for(int i=0; i<indexpairs.size();i++)
        // {
        //     pairfile << indexpairs[i][0] << '\t' << indexpairs[i][1] << '\n';
        // }
        // pairfile.close();
        std::vector< std::vector<ULong64_t> >().swap(timeStampsHeader);
        std::vector< std::vector<UShort_t> >().swap(flags); 
        if(coincidentIndexes.size() == 0)
        {
            std::cout << " No coincidences are found! *****" << std::endl;
            return 0;
        }
        else
        {
            std::cout << coincidentIndexes.size() <<" coincidences are found! *****" << std::endl;
            std::cout << "*****Reading coincident pulses..*****" << std::endl;

            // read coincident pulses
            readCoincidences(setting, coincidentIndexes, coincidentEvents);
            // newPulseHeight(setting, coincidentEvents);
            std::cout << "*****Finished reading coincident pulses!*****" << std::endl;
            std::vector< std::vector< UInt_t > >().swap(coincidentIndexes);
        }
    }
    else
    {
        // read data
        std::cout << "*****Reading pulses in Channel" << '\t';
        for(int i=0;i<setting.Channels.size();i++)
        {
            std::cout << setting.Channels[i] << '\t';
        }
        std::cout <<"*****"<< std::endl;

        readEvents(setting, coincidentEvents);
        std::cout << "*****Finished reading pulses. *****" << std::endl;
    }
    
    std::cout << "*****Filtering bad pulses..*****" << std::endl;
    if (setting.FilterBad) {
        for(int i = 0;i<coincidentEvents.size();i++)
        {
            rejection(setting, coincidentEvents[i]);
        }
    }
    for(int i = 0;i<coincidentEvents.size();i++)
    {
        if (setting.FilterPiledup) {
            std::cout << "*****Filtering pile-up pulses.." 
                << std::to_string(setting.Channels[i]) << "*****" << std::endl;
            simplePUR(setting, i, coincidentEvents[i]);
            std::cout << "*****Finished filtering pile-up pulses!*****" << std::endl;
        }
    }
    if(setting.EnergyCut)
    {
        for(int i = 0;i<coincidentEvents.size();i++)
        {
            energyCut(setting, coincidentEvents[i]);
        }
    }
    for(int i = 0;i<coincidentEvents.size();i++)
    {
        if(setting.PSDCut[i])
        {
            PSDCut(setting, i, coincidentEvents[i]);
        }
    }
    std::cout << "*****Finished filtering bad pulses!*****" << std::endl;
    std::string outname;
    if(std::accumulate(setting.SaveHeaders.begin(),setting.SaveHeaders.end(),0))
    {
        // save headers
        for(int i = 0;i<coincidentEvents.size();i++)
        {
            outname = "output/Headers" + std::to_string(setting.Channels[i]);
            saveHeaders(setting, coincidentEvents[i], outname, [](Event pulse) { return !pulse.isBad; });
        }
    }
    if(setting.SavePulses)
    {
        // save N good pulses
        for(int i = 0;i<coincidentEvents.size();i++)
        {
            outname = "output/goodpulsesch" + std::to_string(setting.Channels[i]);
            savePulses(setting, coincidentEvents[i], setting.SavePulses, outname, [](Event pulse) { return !pulse.isBad; });
        }
    }
    if(setting.SaveBad)
    {
        // save N bad pulses
        for(int i = 0;i<coincidentEvents.size();i++)
        {
            outname = "output/badpulsesch" + std::to_string(setting.Channels[i]);
            savePulses(setting, coincidentEvents[i], setting.SaveBad, outname, [](Event pulse) { return pulse.isBad; });
        }
    }
    if(setting.SavePiledup)
    {
        // save N pile-up pulses
        for(int i = 0;i<coincidentEvents.size();i++)
        {
            outname = "output/pileuppulsesch" + std::to_string(setting.Channels[i]);
            savePulses(setting, coincidentEvents[i], setting.SavePiledup, outname, [](Event pulse) { return pulse.isPiled; });
        }
    }
    
    for(int i = 0;i<coincidentEvents.size();i++)
    {
        outname = "Pulse_CH" + std::to_string(setting.Channels[i]);
        plotPulse(setting, coincidentEvents[i],outname,[](Event pulse) { return !pulse.isBad; });
        std::cout << " Example pulses of CH" << std::to_string(i) << " are plotted! " << std::endl;
    }

    if(setting.SavePH)
    {
        // save the pulse heights
        for(int i = 0;i<coincidentEvents.size();i++)
        {
            outname = "output/PH_CH" + std::to_string(setting.Channels[i]);
            savePH(setting, coincidentEvents[i], outname, [](Event pulse) { return !pulse.isBad; });
            std::cout << " Pulse heights are saved to file: " << outname <<" !"<< std::endl;
        }
    }
    if(setting.SavePI)
    {
        // save the pulse integrals
        for(int i = 0;i<coincidentEvents.size();i++)
        {
            outname = "output/PI_CH" + std::to_string(setting.Channels[i]);
            savePI(setting, coincidentEvents[i], outname, [](Event pulse) { return !pulse.isBad; });
            std::cout << " Pulse integrals are saved to file: " << outname <<" !"<< std::endl;
        }
    }

    //std::vector<float_t> calicoef{1,1};
    if (setting.PHD)
    {
        // calicoef[0] = 478 / 0.6579;
        // calicoef[1] = 478 / 0.5479;
        for(int i=0;i<coincidentEvents.size();i++)
        {
            outname = "CH" + std::to_string(setting.Channels[i])+"_PHD";
            TCanvas *PHcanvas = new TCanvas(outname.c_str(), "PHD Canvas", 200, 10, 700, 500);
            TH1F *PHhisto = new TH1F(outname.c_str(), outname.c_str(), setting.PHDBins, setting.PHmin, setting.PHmax);
            getPHD(PHhisto, PHcanvas, setting, coincidentEvents[i], setting.CalicoefsPHD[i], [](Event pulse) { return !pulse.isBad; });
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
        // calicoef[0] = 478 / 3.1440;
        // calicoef[1] = 478 / 3.1500;
        for(int i=0;i<coincidentEvents.size();i++)
        {
            outname = "CH" + std::to_string(setting.Channels[i]) + "_PID";
            TCanvas *PIcanvas = new TCanvas(outname.c_str(), "PID Canvas", 200, 10, 700, 500);
            TH1F *PIhisto = new TH1F(outname.c_str(), outname.c_str(), setting.PIDBins, setting.PImin, setting.PImax); //// 2000 kevee or 20.0 V
            getPID(PIhisto, PIcanvas, setting, coincidentEvents[i], setting.CalicoefsPID[i],[](Event pulse) { return !pulse.isBad; });
            outname = "output/" + outname +".txt";
            if (setting.SavePID)
            {
                saveHisto(PIhisto, outname);
                std::cout << " Pulse integral distribution is saved to file: " << outname <<" !"<< std::endl;
            }
        }
    }

    if (setting.TimeStamp)
    {
        std::cout << "*****Calculating time stamps using DIACFD..*****" << std::endl;
        for(int i = 0;i<coincidentEvents.size();i++)
        {
            getTimeStamp(setting, coincidentEvents[i]);
        }
        std::cout << "*****Finished calculaing time stamps!*****" << std::endl;
        
        //// TBD: multichannel support
        if (setting.Coincidence && setting.SaveDT) 
        {
            // save time differences
            SaveDT(coincidentEvents, "output/timediff_Header", "Header");
            SaveDT(coincidentEvents, "output/timediff_DIACFD", "DIACFD");
        }
        if (setting.Coincidence && setting.TOF) 
        {
            TCanvas *TOFcanvas = new TCanvas("c3", "TOF Canvas", 200, 10, 700, 500);
            TH1F *TOFhisto = new TH1F("TOF", "TOF", 400, -3.0, 5.0);
            getTOF(TOFhisto, TOFcanvas, setting, coincidentEvents);
            if (setting.SaveTOF) 
            {
                // save ToF
                saveTOF(TOFhisto, "output/TOF");
            }
        }
        
    }
    if(setting.SaveTimeStamp)
    {
        // save time stamps
        for(int i = 0;i<coincidentEvents.size();i++)
        {
            outname = "output/timestamp_Header_CH" + std::to_string(setting.Channels[i]);
            saveTimestamp(coincidentEvents[i], outname, "Header", [](Event pulse) { return !pulse.isBad; });
            if(setting.TimeStamp)
            {
                outname = "output/timestamp_DIACFD_CH" + std::to_string(setting.Channels[i]);
                saveTimestamp(coincidentEvents[i], outname, "DIACFD", [](Event pulse) { return !pulse.isBad; });
            }
        }
    }

    if(setting.PSD)
    {
        for(int i=0;i<coincidentEvents.size();i++)
        {
            outname = "CH" + std::to_string(setting.Channels[i]) +"_PSD";
            TCanvas *PSDcanvas = new TCanvas(outname.c_str(), "PSD Canvas", 200, 10, 700, 500);
            TH2F *PSDhisto = new TH2F(outname.c_str(), outname.c_str(), setting.PSDXBins, setting.PSDXmin, setting.PSDXmax,setting.PSDYBins, setting.PSDYmin, setting.PSDYmax); //// 2000 kevee or 20.0 V
            getPSD(PSDhisto, PSDcanvas, setting, coincidentEvents[i], setting.CalicoefsPID[i],[](Event pulse) { return !pulse.isBad; });
            std::cout << " PSD of CH" << std::to_string(i) << " is plotted! " << std::endl;
            outname = "output/Integrals_CH" + std::to_string(setting.Channels[i]) +".txt";
            if (setting.SaveIntegrals)
            {
                savePSD(coincidentEvents[i], outname, [](Event pulse) { return !pulse.isBad; });
                std::cout << " PSD ratios of CH" << std::to_string(i) << " are saved to file " << outname << "!" << std::endl;
            }
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms" << std::endl; 
    myapp->Run();
    return 0;
}