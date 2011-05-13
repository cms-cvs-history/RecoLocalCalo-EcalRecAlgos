/* Implementation of class EcalCleaningAlgo
   \author Stefano Argiro
   \version $Id: EcalCleaningAlgo.cc,v 1.8 2011/05/12 19:00:34 vlimant Exp $
   \date 20 Dec 2010
*/    


#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h" 

#include "RecoLocalCalo/EcalRecAlgos/interface/EcalCleaningAlgo.h"

EcalCleaningAlgo::EcalCleaningAlgo(const edm::ParameterSet&  p){
  
  swissCrossThreshold_  = p.getParameter<double>("swissCrossThreshold");
  recHitThreshold_      = p.getParameter<double>("recHitThreshold");
  useieta85_            = p.getParameter<bool>  ("useieta85");

  
}



EcalRecHit::Flags 
EcalCleaningAlgo::checkTopology(const DetId& id,
				const EcalRecHitCollection& rhs){

  double swc =swissCross( id, rhs, recHitThreshold_ , useieta85_);

  if ( swc>  swissCrossThreshold_ )  {
 
    return EcalRecHit::kWeird;
  }

  return EcalRecHit::kGood;

}






void EcalCleaningAlgo::setFlags(EcalRecHitCollection& rhs){
  EcalRecHitCollection::iterator rh;
  //changing the collection on place
  for (rh=rhs.begin(); rh!=rhs.end(); ++rh){
    EcalRecHit::Flags state=checkTopology(rh->id(),rhs);
    if (state!=EcalRecHit::kGood) { 
      rh->unsetFlag(EcalRecHit::kGood);
      rh->setFlag(state);
    }
  }
}



float EcalCleaningAlgo::swissCross( const DetId id, 
				    const EcalRecHitCollection & recHits, 
				    float recHitThreshold , 
				    bool avoidIeta85)
{
        // compute swissCross
        if ( id.subdetId() == EcalBarrel ) {
                EBDetId ebId( id );
                // avoid recHits at |eta|=85 where one side of the neighbours is missing
                // (may improve considering also eta module borders, but no
                // evidence for the time being that there the performance is
                // different)
                if ( abs(ebId.ieta())==85 && avoidIeta85) return 0;
                // select recHits with Et above recHitThreshold
                if ( recHitApproxEt( id, recHits ) < recHitThreshold ) return 0;
                float s4 = 0;
                float e1 = recHitE( id, recHits );
                // protect against nan (if 0 threshold is given above)
                if ( e1 == 0 ) return 0;
                s4 += recHitE( id, recHits,  1,  0 );
                s4 += recHitE( id, recHits, -1,  0 );
                s4 += recHitE( id, recHits,  0,  1 );
                s4 += recHitE( id, recHits,  0, -1 );
                return 1 - s4 / e1;
        } else if ( id.subdetId() == EcalEndcap ) {
                EEDetId eeId( id );
                // select recHits with E above recHitThreshold
                float e1 = recHitE( id, recHits );
                if ( e1 < recHitThreshold ) return 0;
                float s4 = 0;
                // protect against nan (if 0 threshold is given above)
                if ( e1 == 0 ) return 0;
                s4 += recHitE( id, recHits,  1,  0 );
                s4 += recHitE( id, recHits, -1,  0 );
                s4 += recHitE( id, recHits,  0,  1 );
                s4 += recHitE( id, recHits,  0, -1 );
                return 1 - s4 / e1;
        }
        return 0;
}



float EcalCleaningAlgo::recHitE( const DetId id, 
				 const EcalRecHitCollection & recHits,
				 int di, int dj )
{
        // in the barrel:   di = dEta   dj = dPhi
        // in the endcap:   di = dX     dj = dY
  
        DetId nid;
        if( id.subdetId() == EcalBarrel) nid = EBDetId::offsetBy( id, di, dj );
        else if( id.subdetId() == EcalEndcap) nid = EEDetId::offsetBy( id, di, dj );

        return ( nid == DetId(0) ? 0 : recHitE( nid, recHits ) );
}

float EcalCleaningAlgo::recHitE( const DetId id, 
				      const EcalRecHitCollection &recHits )
{
        if ( id == DetId(0) ) {
                return 0;
        } else {
                EcalRecHitCollection::const_iterator it = recHits.find( id );
                if ( it != recHits.end() ) return (*it).energy();
        }
        return 0;
}


float EcalCleaningAlgo::recHitApproxEt( const DetId id, 
					     const EcalRecHitCollection &recHits )
{
        // for the time being works only for the barrel
        if ( id.subdetId() == EcalBarrel ) {
                return recHitE( id, recHits ) / cosh( EBDetId::approxEta( id ) );
        }
        return 0;
}
