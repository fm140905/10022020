#include "rejection.h"

Int_t rejection(const Parameters &setting, const ULong_t start, std::vector< Event >& events, const std::vector<std::vector< Float_t > > &Pulses)
{
    //std::vector<Int_t> BadIndexes;
    Int_t badnumber(0);
    Int_t indexTemp(0);
    Float_t heightTemp(0);
    // const Float_t fraction = setting.Rejection;

    // std::cout << "Start filtering bad pulses !" << std::endl;
    if (Pulses.size()+start != events.size())
    {
        std::cout << "Error when reading data!" << std::endl;
        exit(1);
    }
    
    if (!setting.FilterBad)
    {
        std::cout << "You choose not to filter bad pulses !" << std::endl;
        return 0;
    }

    // filter bad pulses
    for (int i = 0; i < Pulses.size(); i++)
    {
        indexTemp = events[i+start].heightindex;
        heightTemp = events[i+start].height;

        //zero supression
        if (setting.ZeroSupression && heightTemp < setting.MinVoltage)
        {
            events[i+start].isBad = true;
        }

        // filter clipped pulses
        if (!events[i+start].isBad)
        {
            if (setting.Clipped &&
                (Pulses[i][indexTemp - 1] == heightTemp ||
                 Pulses[i][indexTemp + 1] == heightTemp))
            {
                events[i+start].isBad = true;
            }
        }

        // filter baseline-shifted pulses
        if (!events[i+start].isBad)
        {
            for (int j = 0; j < setting.Offset; j++)
            {
                if (Pulses[i][j] > heightTemp * 0.05 /*setting.MinVoltage*/)
                {
                    events[i+start].isBad = true;
                    break;
                }
            }
        }
        if (!events[i+start].isBad)
        {
            for (int j = indexTemp+1; j < setting.NSamples; j++)
            {
                if (Pulses[i][j] < -0.05 * heightTemp /*setting.MinVoltage*/)
                {
                    events[i+start].isBad = true;
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

        if (events[i+start].isBad)
        {
            //BadIndexes.push_back(i);
            badnumber++;
        }
    }

    std::cout << " " << badnumber << " bad, ";
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

Int_t simplePUR(const Parameters &setting, const ULong_t start, std::vector< Event >& events, const std::vector<std::vector< Float_t > > &Pulses)
{
    if (!setting.FilterPiledup)
    {
        std::cout << "0 pile-up, ";
        return 0;
    }

    Int_t badnumber(0);
    Int_t indexTemp(0);
    Float_t heightTemp(0);
    ULong_t ii(0);
    const int dpjump = setting.PUwindow / setting.Delt; // data point jump, ~ rising edge
    const float dpf = setting.PUfraction; // double pulse fraction
    const float threshV = setting.PUthreshold; // the noise level
    float deltaV = 0;
    for (int i = 0; i < Pulses.size(); i++)
    {
        ii = i+start;
        if (events[ii].isBad)
        {
            continue;
        }
        indexTemp = events[ii].heightindex;
        heightTemp = events[ii].height;
        
        for (int j = indexTemp; j < Pulses[i].size() - dpjump; j++)
        {
            // detect the rising edge of the second pulse that occurs after the triggering pulse
            deltaV = Pulses[i][j+dpjump] - Pulses[i][j];
            // threshV = 
            if (deltaV > dpf * heightTemp && deltaV > threshV) // && deltaV > setting.MinVoltage[0])
            {
                events[ii].isPiled = true;
                events[ii].isBad = true;
                badnumber ++;
                break;
            }
        }
        if (events[ii].isBad)
        {
            continue;
        }
        for (int j = 0; j < indexTemp - 2 * dpjump; j++)
        {
            // detect the rising edge of the second pulse that occurs before the triggering pulse
            deltaV = Pulses[i][j+dpjump] - Pulses[i][j];
            // threshV = 
            if (deltaV > dpf * heightTemp && deltaV > threshV) // && deltaV > setting.MinVoltage[0])
            {
                events[ii].isPiled = true;
                events[ii].isBad = true;
                badnumber ++;
                break;
            }
        }
    }
    std::cout << " " << badnumber << " pile-up, ";
    return 0;
}