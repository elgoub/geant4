#ifndef DEFAULTHEPREPTREEID_H
#define DEFAULTHEPREPTREEID_H 1

#include "FreeHepTypes.h"

#include <string>

#include "HEPREP/HepRepTreeID.h"

/**
 *
 * @author M.Donszelmann
 */

class DefaultHepRepTreeID : public virtual HEPREP::HepRepTreeID {

    private:
        std::string name;
        std::string version;
        std::string qualifier;

    public:
        DefaultHepRepTreeID(std::string name, std::string version, std::string qualifier = "top_level");
        ~DefaultHepRepTreeID();

        std::string getQualifier();
        void setQualifier(std::string qualifier);
        std::string getName();
        std::string getVersion();
};

#endif
