// -*- C++ -*-
//
// Package:    MC/MC_Weights
// Class:      MC_Weights
//
/**\class MC_Weights MC_Weights.cc MC/MC_Weights/plugins/MC_Weights.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Christopher Brainerd
//         Created:  Thu, 13 Feb 2020 02:16:36 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "TTree.h"
#include "TH1D.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

// user include files                                                                                                                                                                                                
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"
#include "PhysicsTools/CandUtils/interface/EventShapeVariables.h"
#include "PhysicsTools/CandUtils/interface/Thrust.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.


class MC_Weights : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit MC_Weights(const edm::ParameterSet&);
      ~MC_Weights();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      // ----------member data ---------------------------
      edm::EDGetTokenT<GenEventInfoProduct> genToken_;
      edm::Service<TFileService> fs;
      TTree* mc_tree;
      TH1D* hWeights;
      typedef decltype(((edm::Event*)nullptr)->id().run()) run_type;
      typedef decltype(((edm::Event*)nullptr)->id().luminosityBlock()) lumi_type;
      typedef decltype(((edm::Event*)nullptr)->id().event()) event_type;
      run_type nRun;
      lumi_type nLumi;
      event_type nEvent;
      double weight;
      std::vector<double> weights;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//


//
// constructors and destructor
//
MC_Weights::MC_Weights(const edm::ParameterSet& iConfig)
 :
  genToken_(consumes<GenEventInfoProduct>(edm::InputTag("generator", "", "SIM"))),
  fs(),
  mc_tree(fs->make<TTree>("mc_tree","mc_tree")),
  hWeights(fs->make<TH1D>("sum_of_weights","sum_of_weights",0,1,1))
{
   //now do what ever initialization is needed
   mc_tree->Branch("nRun",&nRun);
   mc_tree->Branch("nLumi",&nLumi);
   mc_tree->Branch("nEvent",&nEvent);
   mc_tree->Branch("weight",&weight);
   mc_tree->Branch("weights",&weights);
}


MC_Weights::~MC_Weights()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
MC_Weights::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   edm::Handle<GenEventInfoProduct> genEvtInfo;
   iEvent.getByToken(genToken_,genEvtInfo);
   auto id=iEvent.id();
   nRun=id.run();
   nLumi=id.luminosityBlock();
   nEvent=id.event();
   weights=genEvtInfo->weights();
   weight=genEvtInfo->weight();
   hWeights->Fill(1,weight);
   mc_tree->Fill();  
}


// ------------ method called once each job just before starting event loop  ------------
void
MC_Weights::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
MC_Weights::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MC_Weights::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
  //descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MC_Weights);
