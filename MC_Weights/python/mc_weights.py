import FWCore.ParameterSet.Config as cms
process=cms.Process("analysis")
process.TFileService = cms.Service("TFileService", fileName = cms.string("MC_info.root"))

process.mc_weights=cms.EDAnalyzer("MC_Weights")
process.source=cms.Source("PoolSource",
    fileNames=cms.untracked.vstring('/store/mc/RunIIAutumn18MiniAOD/GluGluHToZZTo4L_M125_13TeV_powheg2_JHUGenV7011_pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v2/270000/E5E2F122-AA57-5248-8177-594EC87DD494.root')
)

process.path=cms.Path(process.mc_weights)
