#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <set>
#include <string>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

int main() {
    std::ifstream infile;
    std::ofstream outfile;
    std::stringstream ss_buffer;
    std::set<int> failed={ 4,28,29,30,36,37,38,39 };
    infile.open("MC_datasets");
    std::vector<std::string> mc_datasets;
    std::string buffer;
    while (infile >> buffer) mc_datasets.push_back(buffer);
    infile.close();
    outfile.open("background_cards");
    int x=0;
    outfile << "dataset sum_weights sum_unit_weights sum_abs_weights sum_square_weights number_events\n";
    for( auto const& dataset : mc_datasets ) {
        std::cout << "Analyzing dataset " << ++x << ": " << dataset << std::endl;
        UInt_t run;
        UInt_t lumi;
        ULong64_t event;
        Double_t weight;
        std::vector<double> *weights;
        double sum_of_weights=0;
        double sum_of_unit_weights=0;
        double sum_of_abs_weights=0;
        double sum_square_weights=0;
        ULong_t number_of_events=0;
        if(failed.find(x)!=failed.end()) {
            std::cout << "Failed job.\n";
            continue;
        }
        ss_buffer.clear();
        ss_buffer << "root://131.225.204.161:1094//store/user/cbrainer/MC_info/output_" << x << ".root";
        std::string filename;
        ss_buffer >> filename;
        std::cout << "Opening file " << filename << std::endl;
        TFile *tf=TFile::Open(filename.c_str());
        if(!tf) exit(1);
        TDirectoryFile *tdf=(TDirectoryFile*)tf->Get("mc_weights");
        TTree *tt=(TTree*)tdf->Get("mc_tree");
        tt->SetBranchAddress("nRun",&run);
        tt->SetBranchAddress("nLumi",&lumi);
        tt->SetBranchAddress("nEvent",&event);
        tt->SetBranchAddress("weight",&weight);
        tt->SetBranchAddress("weights",&weights);
        number_of_events=tt->GetEntries();
        std::cout << "Running over " << number_of_events << " events.\n";
        for(std::size_t i=0;i<number_of_events;++i) {
            if(i && !(i%100000)) std::cout << "Event " << i << "/" << number_of_events << std::endl;
            tt->GetEntry(i);
            sum_of_weights+=weight;
            sum_of_unit_weights+=(weight > 0 ? 1 : -1);
            sum_square_weights+=weight*weight;
            sum_of_abs_weights+=fabs(weight);
        }
        outfile << dataset << " " << sum_of_weights << " " << sum_of_unit_weights << " " << sum_of_abs_weights << " " << sum_square_weights << " " << number_of_events << std::endl;
    }
}
