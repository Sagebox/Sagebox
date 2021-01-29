// This file copyright(c) 2021 Rob Nelson, All Rights Reserved.    E-mail rob@projectsagebox.com for more information.
//

// --------------------------------------------------------------------------------------------------------------
// PRELIMINARY VERSION.  This file has been brought into the Sagebox project from the original sources and has 
// not yet been commented for Sagebox, or properly formatted (I used tabs and am converting to spaces).
// --------------------------------------------------------------------------------------------------------------
//
// ****** UNDER CONSTRUCTION ******
//
// This file is still under construction and may not yet include specifics, awaiting proper testing and integration into Sagebox.
//

//#pragma once
#if !defined(_EventOpt_h_)
#define _EventOpt_h_
#include <cstdio>
#include <math.h>
#include <cstdlib>
#include <memory>

namespace Sage
{
class cwfEvent
{
	char sOpt[100];
	char * spOpt;
	int iLength;
	int ipLength;
private:
	void AddOptString(const char * sOptString);
public:
	char * GetString() { return spOpt; }
	void SetMemNull() { spOpt = nullptr; }
	cwfEvent ShowEvents		();
	cwfEvent ShowErrors		();
	cwfEvent ShowTimerError	();
	cwfEvent Button			();
	cwfEvent Slider			();
	cwfEvent Any			();
	cwfEvent Timer			();
	cwfEvent ButtonPress	();
	cwfEvent MouseClick		();
	cwfEvent AnyMouse		();
	cwfEvent MouseMove		();
	cwfEvent MouseDown		();
	cwfEvent MouseUp		();
	cwfEvent WindowClose	();
	cwfEvent MouseWheel		();
	cwfEvent AnyMenu		();
	cwfEvent EditBox		();
	cwfEvent AnyKey			();


	cwfEvent & operator + (cwfEvent & Opt) { AddOpt(Opt); return((cwfEvent &) *this); }
	cwfEvent & operator << (cwfEvent & Opt) { AddOpt(Opt); return((cwfEvent &) *this); }
	cwfEvent & operator | (cwfEvent & Opt) { AddOpt(Opt); return((cwfEvent &) *this); }
	const char * operator * () { return spOpt ? spOpt : (const char *) sOpt; }

	cwfEvent(const cwfEvent &opt2);
	inline bool Active() { return iLength > 0 || ipLength > 0; }
	void ClearMem() 
	{ 
		if (spOpt) free(spOpt);
		sOpt[0] = 0;; 
		iLength = ipLength = 0;
		spOpt = nullptr;
	}
	~cwfEvent() { ClearMem(); }
	void AddOpt(cwfEvent & wfOpt);
	cwfEvent() { sOpt[0] = 0; sOpt[1] = 0; spOpt = nullptr; ipLength=0; }
};


namespace event
{
	static cwfEvent ShowEvents		() { return cwfEvent().ShowEvents		();	};
	static cwfEvent ShowErrors		() { return cwfEvent().ShowErrors		();	};
	static cwfEvent ShowTimerError	() { return cwfEvent().ShowTimerError	();	};
	static cwfEvent Button			() { return cwfEvent().Button			();	};
	static cwfEvent Slider			() { return cwfEvent().Slider			();	};
	static cwfEvent Any				() { return cwfEvent().Any				();	};
	static cwfEvent Timer			() { return cwfEvent().Timer			();	};
	static cwfEvent ButtonPress		() { return cwfEvent().ButtonPress		();	};
	static cwfEvent MouseClick		() { return cwfEvent().MouseClick		();	};
	static cwfEvent AnyMouse		() { return cwfEvent().AnyMouse			();	};
	static cwfEvent MouseMove		() { return cwfEvent().MouseMove		();	};
	static cwfEvent MouseDown		() { return cwfEvent().MouseDown		();	};
	static cwfEvent MouseUp			() { return cwfEvent().MouseUp			();	};
	static cwfEvent WindowClose		() { return cwfEvent().WindowClose		();	};
	static cwfEvent MouseWheel		() { return cwfEvent().MouseWheel		();	};
	static cwfEvent AnyMenu			() { return cwfEvent().AnyMenu			();	};
	static cwfEvent EditBox			() { return cwfEvent().EditBox			();	};
	static cwfEvent AnyKey			() { return cwfEvent().AnyKey			();	};
};
}; // namespace Sage
#endif // _EventOpt_h_