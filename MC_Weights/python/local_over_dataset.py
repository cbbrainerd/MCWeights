import FWCore.ParameterSet.Config as cms

from FWCore.ParameterSet.VarParsing import VarParsing

options=VarParsing('analysis')
options.register('dataset','',VarParsing.multiplicity.singleton,VarParsing.varType.string,'Dataset to process (e.g. DoubleMuon)')
options.register('number',0,VarParsing.multiplicity.singleton,VarParsing.varType.int,'Dataset number')

options.parseArguments()
dataset=options.dataset

import os
files=os.popen('dasgoclient --query="file dataset=%s"' % dataset).read()
files=[x for x in files.split('\n') if x]

dataset.replace('/','.')

output_filename="root://131.225.204.161:1094//store/user/cbrainer/MC_info/output_%i.root" % options.number

process=cms.Process("analysis")
process.TFileService = cms.Service("TFileService", fileName = cms.string(output_filename))

process.mc_weights=cms.EDAnalyzer("MC_Weights")
process.source=cms.Source("PoolSource",
    fileNames=cms.untracked.vstring(*files)
)

process.path=cms.Path(process.mc_weights)
