#ifndef DEFAULTHEPREP_H
#define DEFAULTHEPREP_H 1

#include "FreeHepTypes.h"

#include <string>
#include <vector>
#include <set>

#include "HEPREP/HepRep.h"
#include "HEPREP/HepRepSelectFilter.h"
#include "HEPREP/HepRepWriter.h"
#include "HEPREP/HepRepType.h"
#include "HEPREP/HepRepTypeTree.h"
#include "HEPREP/HepRepInstanceTree.h"

/**
 *
 * @author M.Donszelmann
 */
class DefaultHepRep : public virtual HEPREP::HepRep {

    private:
        std::vector<std::string> layers;
        std::set<HEPREP::HepRepTypeTree*> typeTrees;
        std::set<HEPREP::HepRepInstanceTree*> instanceTrees;

    public:
        DefaultHepRep();
        ~DefaultHepRep();

        HEPREP::HepRep* copy(HEPREP::HepRepSelectFilter* filter);
        std::vector<std::string> getLayerOrder();
        void addLayer(std::string layer);
        void addTypeTree(HEPREP::HepRepTypeTree* typeTree);
        void removeTypeTree(HEPREP::HepRepTypeTree* typeTree);
        HEPREP::HepRepTypeTree* getTypeTree(std::string name, std::string version);
        std::set<HEPREP::HepRepTypeTree*> getTypeTrees();
        void addInstanceTree(HEPREP::HepRepInstanceTree* instanceTree);
        void overlayInstanceTree(HEPREP::HepRepInstanceTree * instanceTree);
        void removeInstanceTree(HEPREP::HepRepInstanceTree* instanceTree);
        HEPREP::HepRepInstanceTree* getInstanceTreeTop(std::string name, std::string version);
        HEPREP::HepRepInstanceTree* getInstances(std::string name, std::string version,
                                                 std::vector<std::string> typeNames);
        HEPREP::HepRepInstanceTree* getInstancesAfterAction(
                                            std::string name,
                                            std::string version,
                                            std::vector<std::string> typeNames,
                                            std::vector<HEPREP::HepRepAction*> actions,
                                            bool getPoints,
                                            bool getDrawAtts,
                                            bool getNonDrawAtts,
                                            std::vector<std::string> invertAtts);
        std::string checkForException();
        std::set<HEPREP::HepRepInstanceTree*> getInstanceTrees();
};

#endif
