#include "output.h"

Int_t savePH(const Parameters &setting, std::vector<Event> &events, std::string outName, std::function<Bool_t(Event)> cond)
{
    std::ofstream PHFile(outName.c_str());
    for (int i = 0; i < events.size(); i++)
    {
        if (cond(events[i]))
        {
            PHFile << *(events[i].voltage.begin() + events[i].heightindex) << std::endl;
        }
    }
    PHFile.close();

    return 0;
}

Int_t savePI(const Parameters &setting, std::vector<Event> &events, std::string outName, std::function<Bool_t(Event)> cond)
{
    std::ofstream PIFile(outName.c_str());
    for (int i = 0; i < events.size(); i++)
    {
        if (cond(events[i]))
        {
            PIFile << events[i].totalIntegral << std::endl;
        }
    }
    PIFile.close();

    return 0;
}

Int_t savePulses(const Parameters &setting, std::vector<Event> &events, std::string outName, std::function<Bool_t(Event)> cond)
{
    Int_t counts(0);
    std::ofstream voltageFile(outName.c_str());
    for (int i = 0; counts < 50 && i < events.size(); i++)
    {
        if (cond(events[i]))
        {
            for (int j = 0; j < events[i].voltage.size(); j++)
            {
                voltageFile << events[i].voltage[j] << '\t';
            }
            voltageFile << std::endl;
            counts++;
        }
    }
    voltageFile.close();
    std::cout << counts << " pulses are saved to file " << outName << "!" << std::endl;

    return 0;
}

Int_t saveTimestamp(const std::vector<Event> &events, std::string outName, std::string timestampname, std::function<Bool_t(Event)> cond)
{
    std::ofstream tFile(outName.c_str());
    tFile << "Time stamp ";
    Int_t totalcounts(0);
    Bool_t flag = (timestampname == "DIACFD");
    if(timestampname == "DIACFD")
    {
        tFile << "(ns), DIACFD" << std::endl;

        for (int i = 0; i < events.size(); i++)
        {
            if (cond(events[i]))
            {
                totalcounts ++;
                tFile << std::setprecision (15) << events[i].timeStampDACFD << std::endl;
            }
        }
    }
    else if (timestampname == "Header")
    {
        tFile << "(ps), Header"<< std::endl;
        for (int i = 0; i < events.size(); i++)
        {
            if (cond(events[i]))
            {
                totalcounts ++;
                tFile << events[i].timeStampHeader << std::endl;
            }
        }
    }
    std::cout << totalcounts << " " << timestampname << " time stamps are saved to file " << outName << "!" << std::endl;
    
    tFile.close();

    return 0;
}

Int_t SaveDT(const std::vector< std::vector< Event > > &coincidentEvents, std::string outName, std::string timestammpname)
{
    std::ofstream tFile(outName.c_str());
    tFile << "Ch1 - Ch0 (ns), " << timestammpname << std::endl;
    float_t timediff(0);
    Int_t totalcounts(0);
    // if(cond()) // add additional options
    Bool_t flag = (timestammpname == "DIACFD");

    for(int i=0;i<coincidentEvents[0].size();i++)
    {
        if (!coincidentEvents[0][i].isBad &&
            !coincidentEvents[1][i].isBad
            )
        {
            if(flag)
            {
                timediff = (coincidentEvents[1][i].timeStampDACFD - coincidentEvents[0][i].timeStampDACFD);
            }
            else
            {
                timediff = (Long64_t)(coincidentEvents[1][i].timeStampHeader - coincidentEvents[0][i].timeStampHeader) /1000.0;
            }
            totalcounts++;
            tFile << timediff <<'\n';
            // dtcfdfile << timediffdcfd  << '\n';
        }
    }
    std::cout << totalcounts << " " << timestammpname << " time differences are saved to file " << outName << "!" << std::endl;

    tFile.close();
    return 0;
}

Int_t saveTOF(TH1F *histo, std::string outName)
{
    FILE *out_file;
    out_file = fopen(outName.c_str(), "w");
    //cout << "Making output file: " << out_file_name << endl;
    fprintf(out_file, "%s\t%s\n", "DT (ns)", "Counts");
    for (int i = 1; i <= histo->GetNbinsX(); i++)
    {
        fprintf(out_file, "%g\t%g\n",
                histo->GetBinCenter(i), histo->GetBinContent(i));
    }
    fclose(out_file);

    return 0;
}

Int_t saveHisto(TH1F *histo, std::string outName)
{
    FILE *out_file;
    out_file = fopen(outName.c_str(), "w");
    //cout << "Making output file: " << out_file_name << endl;
    //fprintf(out_file, "%s\t%s\n", "BinCenter (V)", "Counts");
    for (int i = 1; i <= histo->GetNbinsX(); i++)
    {
        fprintf(out_file, "%g\t%g\n",
                histo->GetBinCenter(i), histo->GetBinContent(i));
    }
    fclose(out_file);

    return 0;
}

Int_t savePSD(const std::vector<Event> &events, std::string outName, std::function<Bool_t(Event)> cond)
{
    std::ofstream tFile(outName.c_str());
    Int_t totalcounts(0);
    tFile << "Total Integral (V)" << '\t' <<"PSD Ratio" << std::endl;

    for (int i = 0; i < events.size(); i++)
    {
        if (cond(events[i]))
        {
            totalcounts ++;
            tFile << events[i].totalIntegral << '\t' << events[i].tailIntegral / events[i].totalIntegral << std::endl;
        }
    }
    tFile.close();

    return 0;
}