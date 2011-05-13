/**
   \file
   Declaration of class EcalCleaningAlgo

   \author Stefano Argiro
   \version $Id: EcalCleaningAlgo.h,v 1.4 2011/05/12 19:00:34 vlimant Exp $
   \date 20 Dec 2010
*/

#ifndef __EcalCleaningAlgo_h_
#define __EcalCleaningAlgo_h_


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h" 
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h" 
#include <vector>

class DetId;


class EcalCleaningAlgo{


public:
  
  EcalCleaningAlgo(const edm::ParameterSet& p);
  
  /** check topology, return :
   *  kGood    : not anomalous
      kWeird   : spike
      kDiWeird : dispike */
  EcalRecHit::Flags checkTopology(const DetId& id,
				  const EcalRecHitCollection& rhs);

  void setFlags(EcalRecHitCollection& rhs);
  
  

private:
  
  
  float swissCross( const DetId id, const EcalRecHitCollection & recHits, 
		    float recHitThreshold , bool avoidIeta85);

  float recHitE( const DetId id, 
		 const EcalRecHitCollection & recHits,
		 int di, int dj );

  float recHitE( const DetId id, 
		 const EcalRecHitCollection &recHits );

  
  float recHitApproxEt( const DetId id, 
			const EcalRecHitCollection &recHits );

  double swissCrossThreshold_;
  double recHitThreshold_;
  bool   useieta85_;
  

};

#endif // __EcalCleaningAlgo_h_

// Configure (x)emacs for this file ...
// Local Variables:
// mode:c++
// compile-command: "make -C .. -k"
// End:
