#ifndef nuchic__plugins__SherpaMEs_hh
#define nuchic__plugins__SherpaMEs_hh

#include "nuchic/ParticleInfo.hh"

#include <array>
#include <vector>
#include <string>

namespace ATOOLS { class Cluster_Amplitude; }
namespace PHASIC { class Process_Base; }
namespace SHERPA { class Sherpa; }

namespace nuchic {

struct Process_Info;

class SherpaMEs {
private:

  SHERPA::Sherpa *p_sherpa{};

  void addParameter(std::vector<char*>& argv,const std::string& val) const;
  int SherpaVerbosity(int loglevel) const;

  PHASIC::Process_Base *getProcess(ATOOLS::Cluster_Amplitude* const ampl);

public:

  SherpaMEs() = default;

  ~SherpaMEs();

  bool Initialize(const std::vector<std::string> &args);

  bool InitializeProcess(const Process_Info &info);

  std::vector<double> Calc
  (const std::vector<int> fl,
   const std::vector<std::array<double, 4> > &p,
   const double &mu2) const;

};// end of class SherpaMEs

}// end namespace nuchic

#endif