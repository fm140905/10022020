#if !defined(OUTPUT_H)
#define OUTPUT_H

#include "events.h"
#include "getInputParameters.h"
#include <fstream>
#include <functional>
#include "TH1F.h"
#include "TH2F.h"

Int_t saveHeaders(const Parameters &setting, std::vector<Event> &events, std::string outName, std::function<Bool_t(Event)> cond);

Int_t savePH(const Parameters &setting, std::vector<Event> &events, std::string outName, std::function<Bool_t(Event)> cond);

Int_t savePI(const Parameters &setting, std::vector<Event> &events, std::string outName, std::function<Bool_t(Event)> cond);

// Int_t savePulses(const Parameters &setting, std::vector<Event> &events, const Int_t maxNum,std::string outName, std::function<Bool_t(Event)> cond);
Int_t savePulses(const UInt_t numSave, const std::vector<std::vector< Float_t > > &Pulses,std::string outName);

Int_t saveTimestamp(const std::vector<Event> &events, std::string outName, std::string timestampname, std::function<Bool_t(Event)> cond);

Int_t SaveDT(const std::vector< std::vector< Event > > &coincidentEvents, std::string outName, std::string timestammpname);

Int_t saveTOF(TH1F *histo, std::string outName); // save TOF histogram

Int_t saveHisto(TH1F *histo, std::string outName);

Int_t saveHisto2D(TH2F *histo, std::string outName);

Int_t saveIntegrals(const std::vector<Event> &events, std::string outName, std::function<Bool_t(Event)> cond);

template <typename Iterator>
    void stringstream_approach(Iterator begin, Iterator end, const std::string &fileName) {
        std::stringstream buffer;
        for (Iterator it = begin; it != end; ++it) {
            buffer << *it << "\n";
        }

        // Now write to the output file.
        std::ofstream fout(fileName);
        fout << buffer.str();
        fout.close();
    }
#endif // OUTPUT_H
