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
#include "LXeWLSSlab.hh"
#include "LXeWLSFiber.hh"

#include "globals.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"

G4LogicalVolume* LXeWLSSlab::ScintSlab_log=NULL;

LXeWLSSlab::LXeWLSSlab(G4RotationMatrix *pRot,
		       const G4ThreeVector &tlate,
		       G4LogicalVolume *pMotherLogical,
		       G4bool pMany,
		       G4int pCopyNo,
		       LXeDetectorConstruction* c)
  :G4PVPlacement(pRot,tlate,
		 new G4LogicalVolume(new G4Box("temp",1,1,1),
				     G4Material::GetMaterial("Vacuum"),
				     "temp",0,0,0),
		 "Slab",pMotherLogical,pMany,pCopyNo),constructor(c)
{
  CopyValues();
  
  if(!ScintSlab_log || updated){

    G4double slab_x = scint_x/2.;
    G4double slab_y = scint_y/2.; 
    
    G4Box* ScintSlab_box = new G4Box("Slab",slab_x,slab_y,slab_z);
    
    ScintSlab_log
      = new G4LogicalVolume(ScintSlab_box,
			    G4Material::GetMaterial("Polystyrene"),
			    "Slab",0,0,0);
  
    G4double spacing = 2*slab_y/nfibers;
    
    G4RotationMatrix* rm = new G4RotationMatrix();
    rm->rotateY(90*deg);
    
    //Place fibers
    for(G4int i=0;i<nfibers;i++){
      G4double Y=-(spacing)*(nfibers-1)*0.5 + i*spacing;
      new LXeWLSFiber(rm,G4ThreeVector(0.,Y,0.),ScintSlab_log,false,0,
		      constructor);
    }
  
  }
  
  SetLogicalVolume(ScintSlab_log);
}

void LXeWLSSlab::CopyValues(){
  updated=constructor->GetUpdated();
  
  scint_x=constructor->GetScintX();
  scint_y=constructor->GetScintY();
  scint_z=constructor->GetScintZ();
  nfibers=constructor->GetNFibers();
  slab_z=constructor->GetSlabZ();
}



