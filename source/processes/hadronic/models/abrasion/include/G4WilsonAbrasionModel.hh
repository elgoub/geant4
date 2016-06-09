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
// *                                                                  *
// * Parts of this code which have been  developed by QinetiQ Ltd     *
// * under contract to the European Space Agency (ESA) are the	      *
// * intellectual property of ESA. Rights to use, copy, modify and    *
// * redistribute this software for general public use are granted    *
// * in compliance with any licensing, distribution and development   *
// * policy adopted by the Geant4 Collaboration. This code has been   *
// * written by QinetiQ Ltd for the European Space Agency, under ESA  *
// * contract 17191/03/NL/LvH (Aurora Programme). 		      *
// *                                                                  *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
#ifndef G4WilsonAbrasionModel_h
#define G4WilsonAbrasionModel_h
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// MODULE:              G4WilsonAbrasionModel.hh
//
// Version:		B.1
// Date:		15/04/04
// Author:		P R Truscott
// Organisation:	QinetiQ Ltd, UK
// Customer:		ESA/ESTEC, NOORDWIJK
// Contract:		17191/03/NL/LvH
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// CHANGE HISTORY
// --------------
//
// 6 October 2003, P R Truscott, QinetiQ Ltd, UK
// Created.
//
// 15 March 2004, P R Truscott, QinetiQ Ltd, UK
// Beta release
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Class Description
//
//
// Class Description - End
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
////////////////////////////////////////////////////////////////////////////////
//
#include "globals.hh"

#include "G4HadronicInteraction.hh"
#include "G4WilsonAblationModel.hh"
#include "G4ExcitationHandler.hh"
#include "G4HadFinalState.hh"
#include "G4Track.hh"
#include "G4Nucleus.hh"
#include "G4Fragment.hh"
#include "G4HadProjectile.hh"
////////////////////////////////////////////////////////////////////////////////
//
class G4WilsonAbrasionModel : public G4HadronicInteraction
{
  public:
    G4WilsonAbrasionModel (G4bool useAblation1 = false);
    G4WilsonAbrasionModel (G4ExcitationHandler *);
    ~G4WilsonAbrasionModel ();

    G4WilsonAbrasionModel(const G4WilsonAbrasionModel &right);
                                                                                
    const G4WilsonAbrasionModel& operator=(G4WilsonAbrasionModel &right);

    virtual G4HadFinalState *ApplyYourself
      (const G4HadProjectile &, G4Nucleus &);
    void SetVerboseLevel (G4int);
    void SetUseAblation (G4bool);
    G4bool GetUseAblation ();
    void SetConserveMomentum (G4bool);
    G4bool GetConserveMomentum ();    
    void SetExcitationHandler (G4ExcitationHandler *);
    G4ExcitationHandler *GetExcitationHandler ();

  private:
    void PrintWelcomeMessage ();
    G4Fragment *GetAbradedNucleons (G4int, G4double, G4double, G4double);
    G4double GetNucleonInducedExcitation (G4double, G4double, G4double);
    void SetConserveEnergy (G4bool);
    G4bool GetConserveEnergy ();
    
  private:
    G4double               r0sq;
    G4double               npK;
    G4bool                 useAblation;
    G4WilsonAblationModel *theAblation;
    G4ExcitationHandler   *theExcitationHandler;
    G4ExcitationHandler   *theExcitationHandlerx;
    G4bool                 conserveEnergy;
    G4bool                 conserveMomentum;
    G4double               B;
    G4double               third;
};
////////////////////////////////////////////////////////////////////////////////
//
inline void G4WilsonAbrasionModel::SetExcitationHandler
  (G4ExcitationHandler *aExcitationHandler)
  {theExcitationHandler = aExcitationHandler;}
  
inline G4ExcitationHandler *G4WilsonAbrasionModel::GetExcitationHandler ()
  {return theExcitationHandler;}
  
inline G4bool G4WilsonAbrasionModel::GetUseAblation ()
  {return useAblation;}
  
inline void G4WilsonAbrasionModel::SetConserveEnergy (G4bool conserveEnergy1)
  {conserveEnergy = conserveEnergy1;}

inline G4bool G4WilsonAbrasionModel::GetConserveEnergy ()
  {return conserveEnergy;}
  
inline void G4WilsonAbrasionModel::SetConserveMomentum
  (G4bool conserveMomentum1)
  {conserveMomentum = conserveMomentum1;}

inline G4bool G4WilsonAbrasionModel::GetConserveMomentum ()
  {return conserveMomentum;}
  
inline void G4WilsonAbrasionModel::SetVerboseLevel (G4int verboseLevel1)
{
  verboseLevel = verboseLevel1;
  if (useAblation)
    theAblation->SetVerboseLevel(verboseLevel);
}
////////////////////////////////////////////////////////////////////////////////
//  
#endif
