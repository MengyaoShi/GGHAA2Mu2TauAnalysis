import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
mylist = FileUtils.loadListFromFile('/afs/cern.ch/user/m/mshi/CMSSW_7_4_12_patch4/src/GGHAA2Mu2TauAnalysis/MuMuTauTauGenAnalyzer/allInfoIWant_2mu2tau_19GeV.txt')
process = cms.Process("Demo")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(3189) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(*mylist)
)


process.demo = cms.EDAnalyzer('AmumuAnalyzer',
    genParticleTag=cms.InputTag("genParticles"),
    outFileName = cms.string('/afs/cern.ch/user/m/mshi/CMSSW_7_4_12_patch4/src/GGHAA2Mu2TauAnalysis/MuMuTauTauGenAnalyzer/python/heavyHiggs_125_light19.root')  
)

process.TFileService = cms.Service("TFileService",
					fileName = cms.string('histodemo_heavyHiggs_125_light19.root')
)
process.p = cms.Path(process.demo)
