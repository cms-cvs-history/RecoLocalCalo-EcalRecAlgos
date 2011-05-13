
import FWCore.ParameterSet.Config as cms


cleaningAlgoConfig = cms.PSet(
    # cut on 1-e4/e1 variable
    swissCrossThreshold = cms.double(0.99),
    # Et threshold 
    recHitThreshold     = cms.double(4.0),
    # 
    useieta85           = cms.bool(False)
  
    )
