//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
// Code developed by:
//  S.Guatelli
//
//    *******************************
//    *                             *
//    *    BrachyAnalysisManager.cc *
//    *                             *
//    *******************************
//
// $Id: BrachyAnalysisManager.cc,v 1.2 2004/05/25 08:36:18 guatelli Exp $
// GEANT4 tag $Name: geant4-06-02 $
//
#ifdef  G4ANALYSIS_USE
#include <stdlib.h>
#include <fstream>
#include "BrachyAnalysisManager.hh"

#include "G4ios.hh"

#include "AIDA/IHistogram1D.h"
#include "AIDA/IHistogram2D.h"

#include "AIDA/IManagedObject.h"
#include "AIDA/IAnalysisFactory.h"
#include "AIDA/IHistogramFactory.h"
#include "AIDA/ITupleFactory.h"
#include "AIDA/ITreeFactory.h"
#include "AIDA/ITree.h"
#include "AIDA/ITuple.h"

BrachyAnalysisManager* BrachyAnalysisManager::instance = 0;

BrachyAnalysisManager::BrachyAnalysisManager() : 
  aFact(0), theTree(0), histFact(0), h1(0), h2(0)
  
{
  //build up  the  factories
  aFact = AIDA_createAnalysisFactory();

  AIDA::ITreeFactory *treeFact = aFact -> createTreeFactory(); 
 
  //parameters for the TreeFactory
 
  std::string fileName = "brachytherapy.xml";
  theTree = treeFact -> create(fileName,"xml",false, true, "uncompressed");

  delete treeFact;
 
  histFact = aFact -> createHistogramFactory( *theTree );
}

BrachyAnalysisManager::~BrachyAnalysisManager() 
{ 
  delete histFact;
  histFact = 0;

  delete theTree;
  histFact = 0;

  delete aFact;
  aFact = 0;
}

BrachyAnalysisManager* BrachyAnalysisManager::getInstance()
{
  if (instance == 0) instance = new BrachyAnalysisManager;
  return instance;
}

void BrachyAnalysisManager::book() 
{
  //creating a 1D histogram ...
  h1 = histFact -> createHistogram2D("10","Energy, pos", //histoID,histo name
				     300 ,-150.,150.,   //bins'number,xmin,xmax 
				     300,-150.,150.    );//bins'number,ymin,ymax 
  //creating a 2D histogram ...
  h2 = histFact -> createHistogram1D("20","Initial Energy", //histoID,histo name 
				     100,0.,1.); //bins' number, xmin, xmax
  
  h3 = histFact -> createHistogram1D("30","Dose Distribution", 
				      300,-150.,150.); //bins' number, xmin, xmax
}
 
void BrachyAnalysisManager::FillHistogramWithEnergy(G4double x,
                                                    G4double z, 
                                                    G4double energyDeposit)
{
  h1 -> fill(x,z,energyDeposit);
}

void BrachyAnalysisManager::PrimaryParticleEnergySpectrum(G4double primaryParticleEnergy)
{
  //1DHisotgram: energy spectrum of primary particles  
  h2 -> fill(primaryParticleEnergy);
}
void BrachyAnalysisManager::DoseDistribution(G4double x,G4double energy)
{
  //1DHisotgram: energy spectrum of primary particles  
  h3 -> fill(x,energy);
}

void BrachyAnalysisManager::finish() 
{  
  // write all histograms to file ...
  theTree -> commit();

  // close (will again commit) ...
  theTree -> close();
}
#endif











