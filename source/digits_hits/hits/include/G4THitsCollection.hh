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
//
// $Id: G4THitsCollection.hh,v 1.4 2004/06/11 14:10:32 gcosmo Exp $
// GEANT4 tag $Name: geant4-06-02 $
//

#ifndef G4THitsCollection_h
#define G4THitsCollection_h 1

#include "G4VHitsCollection.hh"
#include "G4Allocator.hh"
#include "globals.hh"
//#include "g4rw/tpordvec.h"
#include <vector>

// class description:
//
//  This is a template class of hits collection and parametrized by
// The concrete class of G4VHit. This is a uniform collection for
// a particular concrete hit class objects.
//  An intermediate layer class G4HitsCollection appeared in this
// header file is used just for G4Allocator, because G4Allocator
// cannot be instansiated with a template class. Thus G4HitsCollection
// class MUST NOT be directly used by the user.

class G4HitsCollection : public G4VHitsCollection
{
  public:
      G4HitsCollection();
      G4HitsCollection(G4String detName,G4String colNam);
      virtual ~G4HitsCollection();
      G4int operator==(const G4HitsCollection &right) const;

  protected:
      void* theCollection;
};

#if defined G4DIGI_ALLOC_EXPORT
  extern G4DLLEXPORT G4Allocator<G4HitsCollection> anHCAllocator;
#else
  extern G4DLLIMPORT G4Allocator<G4HitsCollection> anHCAllocator;
#endif

template <class T> class G4THitsCollection : public G4HitsCollection 
{
  public:
      G4THitsCollection();
  public: // with description
      G4THitsCollection(G4String detName,G4String colNam);
      // constructor.
  public:
      virtual ~G4THitsCollection();
      G4int operator==(const G4THitsCollection<T> &right) const;
      
      inline void *operator new(size_t);
      inline void operator delete(void* anHC);
  public: // with description
      virtual void DrawAllHits();
      virtual void PrintAllHits();
      //  These two methods invokes Draw() and Print() methods of all of
      // hit objects stored in this collection, respectively.

  public: // with description
      inline T* operator[](size_t i) const
      { return (*((std::vector<T*>*)theCollection))[i]; }
      //  Returns a pointer to a concrete hit object.
      inline std::vector<T*>* GetVector() const
      { return (std::vector<T*>*)theCollection; }
      //  Returns a collection vector.
      inline G4int insert(T* aHit)
      {
        std::vector<T*>*theHitsCollection 
          = (std::vector<T*>*)theCollection;
        theHitsCollection->push_back(aHit);
        return theHitsCollection->size();
      }
      //  Insert a hit object. Total number of hit objects stored in this
      // collection is returned.
      inline G4int entries() const
      {
        std::vector<T*>*theHitsCollection
          = (std::vector<T*>*)theCollection;
        return theHitsCollection->size();
      }
      //  Returns the number of hit objects stored in this collection

  public:
      virtual G4VHit* GetHit(size_t i) const
      { return (*((std::vector<T*>*)theCollection))[i]; }
      virtual size_t GetSize() const
      { return ((std::vector<T*>*)theCollection)->size(); }

};

template <class T> inline void* G4THitsCollection<T>::operator new(size_t)
{
  void* anHC;
  anHC = (void*)anHCAllocator.MallocSingle();
  return anHC;
}

template <class T> inline void G4THitsCollection<T>::operator delete(void* anHC)
{
  anHCAllocator.FreeSingle((G4HitsCollection*)anHC);
}

template <class T> G4THitsCollection<T>::G4THitsCollection()
{ 
  std::vector<T*> * theHitsCollection
    = new std::vector<T*>;
  theCollection = (void*)theHitsCollection;
}

template <class T> G4THitsCollection<T>::G4THitsCollection(G4String detName,G4String colNam)
: G4HitsCollection(detName,colNam)
{ 
  std::vector<T*> * theHitsCollection
    = new std::vector<T*>;
  theCollection = (void*)theHitsCollection;
}

template <class T> G4THitsCollection<T>::~G4THitsCollection()
{
  std::vector<T*> * theHitsCollection 
    = (std::vector<T*>*)theCollection;
  //theHitsCollection->clearAndDestroy();
  for(size_t i=0;i<theHitsCollection->size();i++)
  { delete (*theHitsCollection)[i]; }
  theHitsCollection->clear();
  delete theHitsCollection;
}

template <class T> G4int G4THitsCollection<T>::operator==(const G4THitsCollection<T> &right) const
{ return (collectionName==right.collectionName); }

template <class T> void G4THitsCollection<T>::DrawAllHits() 
{
  std::vector<T*> * theHitsCollection 
    = (std::vector<T*>*)theCollection;
  size_t n = theHitsCollection->size();
  for(size_t i=0;i<n;i++)
  { (*theHitsCollection)[i]->Draw(); }
}

template <class T> void G4THitsCollection<T>::PrintAllHits() 
{
  std::vector<T*> * theHitsCollection 
    = (std::vector<T*>*)theCollection;
  size_t n = theHitsCollection->size();
  for(size_t i=0;i<n;i++)
  { (*theHitsCollection)[i]->Print(); }
}

#endif

