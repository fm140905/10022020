#include "rejection.h"

Int_t rejection(const Parameters &setting, std::vector<Event> &events)
{
    //std::vector<Int_t> BadIndexes;
    Int_t badnumber(0);
    Int_t indexTemp(0);
    Float_t heightTemp(0);
    // const Float_t fraction = setting.Rejection;

    // std::cout << "Start filtering bad pulses !" << std::endl;

    if (!setting.FilterBad)
    {
        std::cout << "You choose not to filter bad pulses !" << std::endl;
        return 0;
    }

    // filter bad pulses
    for (int i = 0; i < events.size(); i++)
    {
        indexTemp = events[i].heightindex;
        heightTemp = events[i].voltage[indexTemp];

        //zero supression
        if (setting.ZeroSupression && heightTemp < setting.MinVoltage)
        {
            events[i].isBad = true;
        }

        // filter clipped pulses
        if (!events[i].isBad)
        {
            if (setting.Clipped &&
                (events[i].voltage[indexTemp - 1] == heightTemp ||
                 events[i].voltage[indexTemp + 1] == heightTemp))
            {
                events[i].isBad = true;
            }
        }

        // filter baseline-shifted pulses
        if (!events[i].isBad)
        {
            for (int j = 0; j < setting.Offset; j++)
            {
                if (events[i].voltage[j] > heightTemp * 0.05 /*setting.MinVoltage*/)
                {
                    events[i].isBad = true;
                    break;
                }
            }
        }
        if (!events[i].isBad)
        {
            for (int j = indexTemp+1; j < setting.NSamples; j++)
            {
                if (events[i].voltage[j] < -0.05 * heightTemp /*setting.MinVoltage*/)
                {
                    events[i].isBad = true;
                    break;
                }
            }
        }
        // if (!events[i].isBad)
        // {
        //     if (events[i].totalIntegral < 0 || events[i].tailIntegral < 0)
        //     {
        //         events[i].isBad = true;
        //     }
        // }

        // if (!events[i].isBad)
        // {
        //     if(events[i].voltage[indexTemp -1 ] >= fraction * events[i].voltage[indexTemp] || events[i].voltage[indexTemp + 1 ] >= fraction * events[i].voltage[indexTemp])
        //     {
        //         events[i].isBad = true;
        //     }
        // }

        if (events[i].isBad)
        {
            //BadIndexes.push_back(i);
            badnumber++;
        }
    }

    std::cout << " " << badnumber << " bad pulses are found!" << std::endl;
    //save bad pulses
    /*
    if(setting.SaveBad)
    {
        std::ofstream badpulsesfile("BadPulses.txt");
        for(int i=0;i<BadIndexes.size();i++)
        {
            indexTemp = BadIndexes[i];
            for(int j=0;j<events[indexTemp].voltage.size();j++)
            {
                badpulsesfile << events[indexTemp].voltage[j] <<"  ";
            }
            badpulsesfile << std::endl;
        }
        badpulsesfile.close();
        std::cout << " Bad pulses were saved to file: BadPulses.txt."
                  << std::endl;
    }
*/
    // BadIndexes.clear();
    // std::cout << "Finished filtering bad pulses ! " << std::endl;
    return 0;
}

Int_t energyCut(const Parameters &setting, std::vector< Event >& events )
{
    if(setting.EnergyCut)
    {
        for(int i=0; i<events.size(); i++)
        {
            if (events[i].energy < setting.EnergyLoW || events[i].energy > setting.EnergyHigh) {
                events[i].isBad = true;
            }
            
        }
    }
    return 0;
}

Int_t PSDCut(const Parameters &setting, const UInt_t index, std::vector< Event >& events )
{
    Float_t psdtemp(0);
    if(setting.PSDCut[index])
    {
        for(int i=0; i<events.size(); i++)
        {
            psdtemp = events[i].tailIntegral / events[i].totalIntegral;
            if (psdtemp < setting.PSDLow[index] || psdtemp > setting.PSDHigh[index]) {
                events[i].isGamma = true;
                events[i].isBad = true;
            }
            
        }
    }
    return 0;
}

Int_t simplePUR(const Parameters &setting, const UInt_t index, std::vector< Event >& events )
{
    if (!setting.FilterPiledup)
    {
        std::cout << "You choose not to filter pile-up pulses !" << std::endl;
        return 0;
    }

    Int_t badnumber(0);
    Int_t indexTemp(0);
    Float_t heightTemp(0);
    const int dpjump = setting.PUwindow / setting.Delt; // data point jump, ~ rising edge
    const float dpf = setting.PUfraction; // double pulse fraction
    const float threshV = setting.PUthreshold; // the noise level
    float deltaV = 0;
    for (int i = 0; i < events.size(); i++)
    {
        if (events[i].isBad)
        {
            continue;
        }
        indexTemp = events[i].heightindex;
        heightTemp = events[i].height;
        
        for (int j = indexTemp; j < events[i].voltage.size() - dpjump; j++)
        {
            // detect the rising edge of the second pulse that occurs after the triggering pulse
            deltaV = events[i].voltage[j+dpjump] - events[i].voltage[j];
            // threshV = 
            if (deltaV > dpf * heightTemp && deltaV > threshV) // && deltaV > setting.MinVoltage[0])
            {
                events[i].isPiled = true;
                events[i].isBad = true;
                badnumber ++;
                break;
            }
        }
        if (events[i].isBad)
        {
            continue;
        }
        for (int j = 0; j < indexTemp - 2 * dpjump; j++)
        {
            // detect the rising edge of the second pulse that occurs before the triggering pulse
            deltaV = events[i].voltage[j+dpjump] - events[i].voltage[j];
            // threshV = 
            if (deltaV > dpf * heightTemp && deltaV > threshV) // && deltaV > setting.MinVoltage[0])
            {
                events[i].isPiled = true;
                events[i].isBad = true;
                badnumber ++;
                break;
            }
        }
    }
    std::cout << " " << badnumber << " pile-up pulses are found!" << std::endl;
    return 0;
}