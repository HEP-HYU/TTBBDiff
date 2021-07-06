/*
 * SkimEvents.h
 *
 *  Created on: Dec 9, 2018
 *      Author: suyong
 */

#ifndef SKIMEVENTS_H_
#define SKIMEVENTS_H_

#include "NanoAODAnalyzerrdframe.h"
#include <string>

class SkimEvents: public NanoAODAnalyzerrdframe
{
	public:
		SkimEvents(TTree *t, std::string outfilename, std::string cat="", std::string year="", std::string jsonfname="", string globaltag="", int nthreads=1);
		void defineCuts();
		void defineMoreVars(); // define higher-level variables from
		void bookHists();
        private:
                std::string _cat;
                std::string _year;
};



#endif /* SKIMEVENTS_H_ */
