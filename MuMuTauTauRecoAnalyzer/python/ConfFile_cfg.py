import FWCore.ParameterSet.Config as cms

process = cms.Process("RecoAnalyzer")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
'root://eoscms//eos/cms/store/user/mshi/data_no_selection.root'

    )
)

process.RecoAnalyzer = cms.EDAnalyzer('MuMuTauTauRecoAnalyzer',
missingET=cms.InputTag('pfMet'),
muonTag=cms.InputTag('OppositeSign'),
outFileName=cms.string('MuMuTauTauRecoAnalyzer.root') 
)


process.p = cms.Path(process.RecoAnalyzer)
