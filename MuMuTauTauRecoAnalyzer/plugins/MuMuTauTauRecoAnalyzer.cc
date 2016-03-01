// -*- C++ -*-
//
// Package:    GGHAA2Mu2TauAnalysis/MuMuTauTauRecoAnalyzer
// Class:      MuMuTauTauRecoAnalyzer
// 
/**\class MuMuTauTauRecoAnalyzer MuMuTauTauRecoAnalyzer.cc GGHAA2Mu2TauAnalysis/MuMuTauTauRecoAnalyzer/plugins/MuMuTauTauRecoAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Mengyao Shi
//         Created:  Wed, 17 Feb 2016 12:30:06 GMT
//
//


// system include files
#include <memory>
#include <string>
#include <sstream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "Tools/Common/interface/Common.h"
#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"

//
// class declaration
//

class MuMuTauTauRecoAnalyzer : public edm::EDAnalyzer {
   public:
      explicit MuMuTauTauRecoAnalyzer(const edm::ParameterSet&);
      ~MuMuTauTauRecoAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
      void reset(const bool);
      template<typename T>
      void FillPT(edm::Handle<edm::View<T>> &pView, TH1F* h1st)
      {
        for(int i=0; i<pView->size(); i++) h1st->Fill(pView->refAt(i)->pt());
      }
      template<typename T>
      void PlotDPhiMetCand(const reco::Candidate* candPtr, const edm::Handle<edm::View<T>> &pMET, TH1F* h1st)
      {
        h1st->Fill(abs(reco::deltaPhi(candPtr->phi(), pMET->refAt(0)->phi())));
      } 
      TH1F* h1st_;
      edm::InputTag missingET_;
      edm::InputTag MuonTag_;
      TFile* out_;
      std::string outFileName_;
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
MuMuTauTauRecoAnalyzer::MuMuTauTauRecoAnalyzer(const edm::ParameterSet& iConfig):
missingET_(iConfig.getParameter<edm::InputTag>("missingET")),
MuonTag_(iConfig.getParameter<edm::InputTag>("muonTag")),
outFileName_(iConfig.getParameter<std::string>("outFileName"))
{
   //now do what ever initialization is needed
   reset(false);
}
MuMuTauTauRecoAnalyzer::~MuMuTauTauRecoAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
   reset(true);
}


//
// member functions
//

// ------------ method called for each event  ------------
void
MuMuTauTauRecoAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace pat;


   edm::Handle<reco::MuonRefVector> pMuons;
   iEvent.getByLabel(MuonTag_,pMuons);


   edm::Handle<edm::View<pat::MET>> pMET;
   iEvent.getByLabel(missingET_, pMET);
   
   std::vector<reco::Candidate*> Candidates;
   for(reco::MuonRefVector::const_iterator iCandidate=pMuons->begin();
   iCandidate!=pMuons->end(); ++iCandidate)
   {
     Candidates.push_back(dynamic_cast<reco::Candidate*>
             (const_cast<reco::Muon*>((*iCandidate).get())));
   }
   PlotDPhiMetCand(const_cast<reco::Candidate*> (Candidates[Candidates.size()-1]), pMET,h1st_);
}


// ------------ method called once each job just before starting event loop  ------------
void 
MuMuTauTauRecoAnalyzer::beginJob()
{
 out_=new TFile(outFileName_.c_str(), "RECREATE");
 h1st_=new TH1F("testDR","test",100,0,3.0);
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MuMuTauTauRecoAnalyzer::endJob() 
{
  out_->cd();
  TCanvas DRCanvas("DRCanvas","",800,800);
  Common::draw1DHistograms(DRCanvas, h1st_);
  out_->Write();
  out_->Close();
}

void MuMuTauTauRecoAnalyzer::reset(bool doDelete)
{
if(doDelete&&(out_!=NULL)) delete out_;
out_=NULL;
}
// ------------ method called when starting to processes a run  ------------

void 
MuMuTauTauRecoAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}


// ------------ method called when ending the processing of a run  ------------

void 
MuMuTauTauRecoAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}


// ------------ method called when starting to processes a luminosity block  ------------

void 
MuMuTauTauRecoAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}


// ------------ method called when ending the processing of a luminosity block  ------------

void 
MuMuTauTauRecoAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}


// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MuMuTauTauRecoAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MuMuTauTauRecoAnalyzer);
