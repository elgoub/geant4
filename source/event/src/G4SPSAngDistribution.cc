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
///////////////////////////////////////////////////////////////////////////////
//
// MODULE:        G4SPSAngDistribution.cc
//
// Version:      1.0
// Date:         5/02/04
// Author:       Fan Lei 
// Organisation: QinetiQ ltd.
// Customer:     ESA/ESTEC
//
///////////////////////////////////////////////////////////////////////////////
//
//
// CHANGE HISTORY
// --------------
//
//
// Version 1.0, 05/02/2004, Fan Lei, Created.
//    Based on the G4GeneralParticleSource class in Geant4 v6.0
//
///////////////////////////////////////////////////////////////////////////////
//
#include "Randomize.hh"
//#include <math.h>

#include "G4SPSAngDistribution.hh"

G4SPSAngDistribution::G4SPSAngDistribution()
{
  // Angular distribution Variables
  G4ThreeVector zero;
  particle_momentum_direction = G4ParticleMomentum(0,0,-1);

  AngDistType = "planar"; 
  AngRef1 = HepXHat;
  AngRef2 = HepYHat;
  AngRef3 = HepZHat;
  MinTheta = 0.;
  MaxTheta = pi;
  MinPhi = 0.;
  MaxPhi = twopi;
  DR = 0.;
  DX = 0.;
  DY = 0.;
  UserDistType = "NULL";
  UserWRTSurface = true;
  UserAngRef = false;
  IPDFThetaExist = false;
  IPDFPhiExist = false;
  verbosityLevel = 0 ;
}

G4SPSAngDistribution::~G4SPSAngDistribution()
{}

//
void G4SPSAngDistribution::SetAngDistType(G4String atype)
{
  if(atype != "iso" && atype != "cos" && atype != "user" && atype != "planar"
     && atype != "beam1d" && atype != "beam2d")
    G4cout << "Error, distribution must be iso, cos, planar, beam1d, beam2d or user" << G4endl;
  else
    AngDistType = atype;
  if (AngDistType == "cos") MaxTheta = pi/2. ;
  if (AngDistType == "user") {
    UDefThetaH = IPDFThetaH = ZeroPhysVector ;
    IPDFThetaExist = false ;
    UDefPhiH = IPDFPhiH = ZeroPhysVector ;
    IPDFPhiExist = false ;
  }
}

void G4SPSAngDistribution::DefineAngRefAxes(G4String refname, G4ThreeVector ref)
{
  if(refname == "angref1")
    AngRef1 = ref.unit(); // x'
  else if(refname == "angref2")
    AngRef2 = ref.unit(); // vector in x'y' plane

  // User defines x' (AngRef1) and a vector in the x'y'
  // plane (AngRef2). Then, AngRef1 x AngRef2 = AngRef3
  // the z' vector. Then, AngRef3 x AngRef1 = AngRef2
  // which will now be y'.

  AngRef3 = AngRef1.cross(AngRef2); // z'
  AngRef2 = AngRef3.cross(AngRef1); // y'
  UserAngRef = true ;
  if(verbosityLevel == 2)
    {
      G4cout << "Angular distribution rotation axes " << AngRef1 << " " << AngRef2 << " " << AngRef3 << G4endl;
    }
}

void G4SPSAngDistribution::SetMinTheta(G4double mint)
{
  MinTheta = mint;
}

void G4SPSAngDistribution::SetMinPhi(G4double minp)
{
  MinPhi = minp;
}

void G4SPSAngDistribution::SetMaxTheta(G4double maxt)
{
  MaxTheta = maxt;
}

void G4SPSAngDistribution::SetMaxPhi(G4double maxp)
{
  MaxPhi = maxp;
}

void G4SPSAngDistribution::SetBeamSigmaInAngR(G4double r)
{
  DR = r;
}

void G4SPSAngDistribution::SetBeamSigmaInAngX(G4double r)
{
  DX = r;
}

void G4SPSAngDistribution::SetBeamSigmaInAngY(G4double r)
{
  DY = r;
}

void G4SPSAngDistribution::UserDefAngTheta(G4ThreeVector input)
{
  if(UserDistType == "NULL") UserDistType = "theta";
  if(UserDistType == "phi") UserDistType = "both";  
  G4double thi, val;
  thi = input.x();
  val = input.y();
  if(verbosityLevel >= 1)
    G4cout << "In UserDefAngTheta" << G4endl;
  UDefThetaH.InsertValues(thi, val);
}

void G4SPSAngDistribution::UserDefAngPhi(G4ThreeVector input)
{
  if(UserDistType == "NULL") UserDistType = "phi";
  if(UserDistType == "theta") UserDistType = "both";  
  G4double phhi, val;
  phhi = input.x();
  val = input.y();
  if(verbosityLevel >= 1)
    G4cout << "In UserDefAngPhi" << G4endl;
  UDefPhiH.InsertValues(phhi, val); 
}

void G4SPSAngDistribution::SetUserWRTSurface(G4bool wrtSurf)
{
  // This is only applied in user mode?
  // if UserWRTSurface = true then the user wants momenta with respect
  // to the surface normals.
  // When doing this theta has to be 0-90 only otherwise there will be
  // errors, which currently are flagged anywhere.
  UserWRTSurface = wrtSurf;
}

void G4SPSAngDistribution::SetUseUserAngAxis(G4bool userang)
{
  // if UserAngRef = true  the angular distribution is defined wrt 
  // the user defined co-ordinates
  UserAngRef = userang;
}

void G4SPSAngDistribution::GenerateBeamFlux()
{
  G4double theta, phi;
  G4double px, py, pz;
  if (AngDistType == "beam1d") 
    { 
      theta = G4RandGauss::shoot(0.0,DR);
      phi = twopi * G4UniformRand();
    }
  else 
    { 
      px = G4RandGauss::shoot(0.0,DX);
      py = G4RandGauss::shoot(0.0,DY);
      theta = sqrt (px*px + py*py);
      if (theta != 0.) { 
	phi = acos(px/theta);
	if ( py < 0.) phi = -phi;
      }
      else
	{ 
	  phi = 0.0;
	}
    }
  px = -sin(theta) * cos(phi);
  py = -sin(theta) * sin(phi);
  pz = -cos(theta);
  G4double finx, finy,  finz ;
  finx = px, finy =py, finz =pz;
  if (UserAngRef){
    // Apply Angular Rotation Matrix
    // x * AngRef1, y * AngRef2 and z * AngRef3
    finx = (px * AngRef1.x()) + (py * AngRef2.x()) + (pz * AngRef3.x());
    finy = (px * AngRef1.y()) + (py * AngRef2.y()) + (pz * AngRef3.y());
    finz = (px * AngRef1.z()) + (py * AngRef2.z()) + (pz * AngRef3.z());
    G4double ResMag = sqrt((finx*finx) + (finy*finy) + (finz*finz));
    finx = finx/ResMag;
    finy = finy/ResMag;
    finz = finz/ResMag;
  }
  particle_momentum_direction.setX(finx);
  particle_momentum_direction.setY(finy);
  particle_momentum_direction.setZ(finz);

  // particle_momentum_direction now holds unit momentum vector.
  if(verbosityLevel >= 1)
    G4cout << "Generating beam vector: " << particle_momentum_direction << G4endl;
}

void G4SPSAngDistribution::GenerateIsotropicFlux()
{
  // generates isotropic flux.
  // No vectors are needed.
  G4double rndm, rndm2;
  G4double px, py, pz;

  //
  G4double sintheta, sinphi,costheta,cosphi;
  rndm = angRndm->GenRandTheta();
  costheta = cos(MinTheta) - rndm * (cos(MinTheta) - cos(MaxTheta));
  sintheta = sqrt(1. - costheta*costheta);
  
  rndm2 = angRndm->GenRandPhi();
  Phi = MinPhi + (MaxPhi - MinPhi) * rndm2; 
  sinphi = sin(Phi);
  cosphi = cos(Phi);

  px = -sintheta * cosphi;
  py = -sintheta * sinphi;
  pz = -costheta;

  // for volume and ponit source use mother or user defined co-ordinates
  // for plane and surface source user surface-normal or userdefined co-ordinates
  //
  G4double finx, finy, finz;
  if (posDist->SourcePosType == "Point" || posDist->SourcePosType == "Volume") {
    if (UserAngRef){
      // Apply Rotation Matrix
      // x * AngRef1, y * AngRef2 and z * AngRef3
      finx = (px * AngRef1.x()) + (py * AngRef2.x()) + (pz * AngRef3.x());
      finy = (px * AngRef1.y()) + (py * AngRef2.y()) + (pz * AngRef3.y());
      finz = (px * AngRef1.z()) + (py * AngRef2.z()) + (pz * AngRef3.z());
    } else {
      finx = px;
      finy = py;
      finz = pz;
    }
  } else {    // for plane and surface source   
    if (UserAngRef){
      // Apply Rotation Matrix
      // x * AngRef1, y * AngRef2 and z * AngRef3
      finx = (px * AngRef1.x()) + (py * AngRef2.x()) + (pz * AngRef3.x());
      finy = (px * AngRef1.y()) + (py * AngRef2.y()) + (pz * AngRef3.y());
      finz = (px * AngRef1.z()) + (py * AngRef2.z()) + (pz * AngRef3.z());
    } else {
      finx = (px*posDist->SideRefVec1.x()) + (py*posDist->SideRefVec2.x()) + (pz*posDist->SideRefVec3.x());
      finy = (px*posDist->SideRefVec1.y()) + (py*posDist->SideRefVec2.y()) + (pz*posDist->SideRefVec3.y());
      finz = (px*posDist->SideRefVec1.z()) + (py*posDist->SideRefVec2.z()) + (pz*posDist->SideRefVec3.z());
    }
  }
  G4double ResMag = sqrt((finx*finx) + (finy*finy) + (finz*finz));
  finx = finx/ResMag;
  finy = finy/ResMag;
  finz = finz/ResMag;

  particle_momentum_direction.setX(finx);
  particle_momentum_direction.setY(finy);
  particle_momentum_direction.setZ(finz);

  // particle_momentum_direction now holds unit momentum vector.
  if(verbosityLevel >= 1)
    G4cout << "Generating isotropic vector: " << particle_momentum_direction << G4endl;
}

void G4SPSAngDistribution::GenerateCosineLawFlux()
{
  // Method to generate flux distributed with a cosine law
  G4double px, py, pz;
  G4double rndm, rndm2;
  //
  G4double sintheta, sinphi,costheta,cosphi;
  rndm = angRndm->GenRandTheta();
  sintheta = sqrt( rndm * (sin(MaxTheta)*sin(MaxTheta) - sin(MinTheta)*sin(MinTheta) ) 
		   +sin(MinTheta)*sin(MinTheta) );
  costheta = sqrt(1. -sintheta*sintheta);
  
  rndm2 = angRndm->GenRandPhi();
  Phi = MinPhi + (MaxPhi - MinPhi) * rndm2; 
  sinphi = sin(Phi);
  cosphi = cos(Phi);

  px = -sintheta * cosphi;
  py = -sintheta * sinphi;
  pz = -costheta;

  // for volume and ponit source use mother or user defined co-ordinates
  // for plane and surface source user surface-normal or userdefined co-ordinates
  //
  G4double finx, finy, finz;
  if (posDist->SourcePosType == "Point" || posDist->SourcePosType == "Volume") {
    if (UserAngRef){
      // Apply Rotation Matrix
      finx = (px * AngRef1.x()) + (py * AngRef2.x()) + (pz * AngRef3.x());
      finy = (px * AngRef1.y()) + (py * AngRef2.y()) + (pz * AngRef3.y());
      finz = (px * AngRef1.z()) + (py * AngRef2.z()) + (pz * AngRef3.z());
    } else {
      finx = px;
      finy = py;
      finz = pz;
    }
  } else {    // for plane and surface source   
    if (UserAngRef){
      // Apply Rotation Matrix
      finx = (px * AngRef1.x()) + (py * AngRef2.x()) + (pz * AngRef3.x());
      finy = (px * AngRef1.y()) + (py * AngRef2.y()) + (pz * AngRef3.y());
      finz = (px * AngRef1.z()) + (py * AngRef2.z()) + (pz * AngRef3.z());
    } else {
      finx = (px*posDist->SideRefVec1.x()) + (py*posDist->SideRefVec2.x()) + (pz*posDist->SideRefVec3.x());
      finy = (px*posDist->SideRefVec1.y()) + (py*posDist->SideRefVec2.y()) + (pz*posDist->SideRefVec3.y());
      finz = (px*posDist->SideRefVec1.z()) + (py*posDist->SideRefVec2.z()) + (pz*posDist->SideRefVec3.z());
    }
  }
  G4double ResMag = sqrt((finx*finx) + (finy*finy) + (finz*finz));
  finx = finx/ResMag;
  finy = finy/ResMag;
  finz = finz/ResMag;

  particle_momentum_direction.setX(finx);
  particle_momentum_direction.setY(finy);
  particle_momentum_direction.setZ(finz);

  // particle_momentum_direction now contains unit momentum vector.
  if(verbosityLevel >= 1)
    {
      G4cout << "Resultant cosine-law unit momentum vector " << particle_momentum_direction << G4endl;
    }
}

void G4SPSAngDistribution::GeneratePlanarFlux()
{
  // particle_momentum_direction now contains unit momentum vector.
  // nothing need be done here as the m-directions have been set directly
  // under this option
  if(verbosityLevel >= 1)
    {
      G4cout << "Resultant Planar wave  momentum vector " << particle_momentum_direction << G4endl;
    }
}

void G4SPSAngDistribution::GenerateUserDefFlux()
{
  G4double rndm, px, py, pz, pmag;

  if(UserDistType == "NULL")
    G4cout << "Error: UserDistType undefined" << G4endl;
  else if(UserDistType == "theta") {
    Theta = 10.;
    while(Theta > MaxTheta || Theta < MinTheta)
      Theta = GenerateUserDefTheta();
    Phi = 10.;
    while(Phi > MaxPhi || Phi < MinPhi) {
      rndm = angRndm->GenRandPhi();
      Phi = twopi * rndm;
    }
  }
  else if(UserDistType == "phi") {
    Theta = 10.;
    while(Theta > MaxTheta || Theta < MinTheta)
      {
	rndm = angRndm->GenRandTheta();
	Theta = acos(1. - (2. * rndm));
      }
    Phi = 10.;
    while(Phi > MaxPhi || Phi < MinPhi)
      Phi = GenerateUserDefPhi();
  }
  else if(UserDistType == "both")
    {
      Theta = 10.;
      while(Theta > MaxTheta || Theta < MinTheta)      
	Theta = GenerateUserDefTheta();
      Phi = 10.;
      while(Phi > MaxPhi || Phi < MinPhi)
	Phi = GenerateUserDefPhi();
    }
  px = -sin(Theta) * cos(Phi);
  py = -sin(Theta) * sin(Phi);
  pz = -cos(Theta);

  pmag = sqrt((px*px) + (py*py) + (pz*pz));

  if(!UserWRTSurface) {
    G4double finx, finy, finz;      
    if (UserAngRef) {
      // Apply Rotation Matrix
      // x * AngRef1, y * AngRef2 and z * AngRef3
      finx = (px * AngRef1.x()) + (py * AngRef2.x()) + (pz * AngRef3.x());
      finy = (px * AngRef1.y()) + (py * AngRef2.y()) + (pz * AngRef3.y());
      finz = (px * AngRef1.z()) + (py * AngRef2.z()) + (pz * AngRef3.z());
    } else {  // use mother co-ordinates
      finx = px;
      finy = py;
      finz = pz;
    }
    G4double ResMag = sqrt((finx*finx) + (finy*finy) + (finz*finz));
    finx = finx/ResMag;
    finy = finy/ResMag;
    finz = finz/ResMag;
    
    particle_momentum_direction.setX(finx);
    particle_momentum_direction.setY(finy);
    particle_momentum_direction.setZ(finz);     
  } 
  else  {  // UserWRTSurface = true
    G4double pxh = px/pmag;
    G4double pyh = py/pmag;
    G4double pzh = pz/pmag;
    if(verbosityLevel > 1) {
      G4cout <<"SideRefVecs " <<posDist->SideRefVec1<<posDist->SideRefVec2<<posDist->SideRefVec3<<G4endl;
      G4cout <<"Raw Unit vector "<<pxh<<","<<pyh<<","<<pzh<<G4endl;
    }
    G4double resultx = (pxh*posDist->SideRefVec1.x()) + (pyh*posDist->SideRefVec2.x()) + 
      (pzh*posDist->SideRefVec3.x());
    
    G4double resulty = (pxh*posDist->SideRefVec1.y()) + (pyh*posDist->SideRefVec2.y()) + 
      (pzh*posDist->SideRefVec3.y());
    
    G4double resultz = (pxh*posDist->SideRefVec1.z()) + (pyh*posDist->SideRefVec2.z()) + 
      (pzh*posDist->SideRefVec3.z());
    
    G4double ResMag = sqrt((resultx*resultx) + (resulty*resulty) + (resultz*resultz));
    resultx = resultx/ResMag;
    resulty = resulty/ResMag;
    resultz = resultz/ResMag;
    
    particle_momentum_direction.setX(resultx);
    particle_momentum_direction.setY(resulty);
    particle_momentum_direction.setZ(resultz);
  }
  
  // particle_momentum_direction now contains unit momentum vector.
  if(verbosityLevel > 0 )
    {
      G4cout << "Final User Defined momentum vector " << particle_momentum_direction << G4endl;
    }
}

G4double G4SPSAngDistribution::GenerateUserDefTheta()
{
  // Create cumulative histogram if not already done so. Then use RandFlat
  //::shoot to generate the output Theta value.
  if(UserDistType == "NULL" || UserDistType == "phi")
    {
      // No user defined theta distribution
      G4cout << "Error ***********************" << G4endl;
      G4cout << "UserDistType = " << UserDistType << G4endl;
      return (0.);
    }
  else
    {
      // UserDistType = theta or both and so a theta distribution
      // is defined. This should be integrated if not already done.
      if(IPDFThetaExist == false)
	{
	  // IPDF has not been created, so create it
	  G4double bins[1024],vals[1024], sum;
	  G4int ii;
	  G4int maxbin = G4int(UDefThetaH.GetVectorLength());
	  bins[0] = UDefThetaH.GetLowEdgeEnergy(size_t(0));
	  vals[0] = UDefThetaH(size_t(0));
	  sum = vals[0];
	  for(ii=1;ii<maxbin;ii++)
	    {
	      bins[ii] = UDefThetaH.GetLowEdgeEnergy(size_t(ii));
	      vals[ii] = UDefThetaH(size_t(ii)) + vals[ii-1];
	      sum = sum + UDefThetaH(size_t(ii));
	    }
	  for(ii=0;ii<maxbin;ii++)
	    {
	      vals[ii] = vals[ii]/sum;
	      IPDFThetaH.InsertValues(bins[ii], vals[ii]);
	    }
	  // Make IPDFThetaExist = true
	  IPDFThetaExist = true;
	}
      // IPDF has been create so carry on
      G4double rndm = G4UniformRand();
      return(IPDFThetaH.GetEnergy(rndm));
    }
}

G4double G4SPSAngDistribution::GenerateUserDefPhi()
{
  // Create cumulative histogram if not already done so. Then use RandFlat
  //::shoot to generate the output Theta value.

  if(UserDistType == "NULL" || UserDistType == "theta")
    {
      // No user defined phi distribution
      G4cout << "Error ***********************" << G4endl;
      G4cout << "UserDistType = " << UserDistType << G4endl;
      return(0.);
    }
  else
    {
      // UserDistType = phi or both and so a phi distribution
      // is defined. This should be integrated if not already done.
      if(IPDFPhiExist == false)
	{
	  // IPDF has not been created, so create it
	  G4double bins[1024],vals[1024], sum;
	  G4int ii;
	  G4int maxbin = G4int(UDefPhiH.GetVectorLength());
	  bins[0] = UDefPhiH.GetLowEdgeEnergy(size_t(0));
	  vals[0] = UDefPhiH(size_t(0));
	  sum = vals[0];
	  for(ii=1;ii<maxbin;ii++)
	    {
	      bins[ii] = UDefPhiH.GetLowEdgeEnergy(size_t(ii));
	      vals[ii] = UDefPhiH(size_t(ii)) + vals[ii-1];
	      sum = sum + UDefPhiH(size_t(ii));
	    }

	  for(ii=0;ii<maxbin;ii++)
	    {
	      vals[ii] = vals[ii]/sum;
	      IPDFPhiH.InsertValues(bins[ii], vals[ii]);
	    }
	  // Make IPDFPhiExist = true
	  IPDFPhiExist = true;
	}
      // IPDF has been create so carry on
      G4double rndm = G4UniformRand();
      return(IPDFPhiH.GetEnergy(rndm));
    }
}
//
void G4SPSAngDistribution::ReSetHist(G4String atype)
{
  if (atype == "theta") {
    UDefThetaH = IPDFThetaH = ZeroPhysVector ;
    IPDFThetaExist = false ;}
  else if (atype == "phi"){    
    UDefPhiH = IPDFPhiH = ZeroPhysVector ;
    IPDFPhiExist = false ;} 
  else {
    G4cout << "Error, histtype not accepted " << G4endl;
  }
}


G4ParticleMomentum G4SPSAngDistribution::GenerateOne()
{
  // Angular stuff
  if(AngDistType == "iso")
    GenerateIsotropicFlux();
  else if(AngDistType == "cos")
    GenerateCosineLawFlux();
  else if(AngDistType == "planar")
    GeneratePlanarFlux();
  else if(AngDistType == "beam1d" || AngDistType == "beam2d" )
    GenerateBeamFlux();
  else if(AngDistType == "user")
    GenerateUserDefFlux();
  else
    G4cout << "Error: AngDistType has unusual value" << G4endl;
  return particle_momentum_direction;
}









