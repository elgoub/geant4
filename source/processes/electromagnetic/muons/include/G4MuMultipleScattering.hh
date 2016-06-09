//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: G4MuMultipleScattering.hh,v 1.2 2008/04/13 17:19:13 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// -----------------------------------------------------------------------------
//
// GEANT4 Class header file
//
// File name:     G4MuMultipleScattering
//
// Author:        V.Ivanchenko using Laszlo Urban original code
//
// Creation date: 24.10.2007 cloned from G4MultipleScattering by VI
// 
// Modifications:
// 20.03.07 Remove local parameter skin (V.Ivanchenko) 
//
//
//------------------------------------------------------------------------------

// class description
//
//  The class simulates the multiple scattering for any kind
//  of charged particle.
//
// class description - end

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef G4MuMultipleScattering_h
#define G4MuMultipleScattering_h 1

#include "G4VMultipleScattering.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4VMscModel;

class G4MuMultipleScattering : public G4VMultipleScattering

{
public:    // with description

  G4MuMultipleScattering(G4double tet = 0.2, 
			 const G4String& processName="muMsc");

  virtual ~G4MuMultipleScattering();

  // returns true for charged particles, false otherwise
  G4bool IsApplicable (const G4ParticleDefinition& p);

  // Print few lines of informations about the process: validity range,
  void PrintInfo();

  // geom. step length distribution should be sampled or not
  void Setsamplez(G4bool value) { samplez = value;};

  // to reduce the energy/step dependence
  void Setdtrl(G4double value) { dtrl = value;};

  // 'soften' step limitation above lambdalimit
  void SetLimitTheta(G4double value) { thetaLimit = value;};

protected:

  // This function initialise models
  void InitialiseProcess(const G4ParticleDefinition*);

private:        // data members

  G4VMscModel* mscModel;

  G4double thetaLimit;
  G4double dtrl;

  G4bool   samplez;
  G4bool   isInitialized;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif