#ifndef PROCESS_INFO_HH
#define PROCESS_INFO_HH

#include "Achilles/ParticleInfo.hh"
#include "fmt/format.h"

#include <map>
#include <string>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include "yaml-cpp/yaml.h"
#pragma GCC diagnostic pop

namespace achilles {

class PID;

struct Process_Info {
    using leptonic_state = std::pair<achilles::PID, std::vector<achilles::PID>>;
    using hadronic_state = std::pair<std::vector<achilles::PID>, std::vector<achilles::PID>>;
    Process_Info(std::vector<achilles::PID> leptons={}) {
        if(leptons.size() < 2) return;
        m_leptonic.first = leptons[0];
        m_leptonic.second.assign(leptons.begin() + 1, leptons.end());
    }

    leptonic_state m_leptonic{};
    hadronic_state m_hadronic{};
    size_t Multiplicity() const;
    std::vector<double> Masses() const;
    std::vector<long> Ids() const;
    std::map<size_t, long> m_mom_map;
    int LeptonicCharge() const;

    bool operator==(const Process_Info &other) const {
        return m_leptonic == other.m_leptonic && m_hadronic == other.m_hadronic;
    }

    template<typename OStream>
    friend OStream& operator<<(OStream &os, const Process_Info &info) {
        os << fmt::format("Process_Info([{}, {}] -> [{}, {}])", info.m_leptonic.first,
                          fmt::join(info.m_hadronic.first.begin(), info.m_hadronic.first.end(), ", "),
                          fmt::join(info.m_leptonic.second.begin(), info.m_leptonic.second.end(), ", "),
                          fmt::join(info.m_hadronic.second.begin(), info.m_hadronic.second.end(), ", "));
        return os;
    }
};

}

namespace YAML {

template<>
struct convert<achilles::Process_Info> {
    static bool decode(const Node &node, achilles::Process_Info &info) {
        if(!node.IsMap()) return false;
        if(!node["Leptons"].IsSequence()) return false;
        info = achilles::Process_Info(node["Leptons"].as<std::vector<achilles::PID>>());
        return true;
    }
};

}

#endif
