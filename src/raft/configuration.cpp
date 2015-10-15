/*
 * =====================================================================================
 *
 *       Filename:  configuration.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年09月28日 18时38分28秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  WangYao (fisherman), wangyao02@baidu.com
 *        Company:  Baidu, Inc
 *
 * =====================================================================================
 */

#include "raft/configuration.h"

namespace raft {

std::ostream& operator<<(std::ostream& os, const Configuration& a) {
    //std::copy(a.peers.begin(), a.peers.end(), std::ostream_iterator<PeerId>(os, ","));
    os << "Configuration{";
    for (size_t i = 0; i < a.peers.size(); i++) {
        os << a.peers[i];
        if (i < a.peers.size() - 1) {
            os << ",";
        }
    }
    os << "}";
    return os;
}

void ConfigurationManager::add(const int64_t index, const Configuration& config) {
    _configurations.insert(std::pair<int64_t, Configuration>(index, config));
}

void ConfigurationManager::truncate_prefix(const int64_t first_index_kept) {
    _configurations.erase(_configurations.begin(), _configurations.lower_bound(first_index_kept));
}

void ConfigurationManager::truncate_suffix(const int64_t last_index_kept) {
    _configurations.erase(_configurations.upper_bound(last_index_kept), _configurations.end());
}

void ConfigurationManager::set_snapshot(const int64_t index, const Configuration& config) {
    assert(index >= _snapshot.first);
    _snapshot = std::pair<int64_t, Configuration>(index, config);
}

std::pair<int64_t, Configuration> ConfigurationManager::get_configuration(
        const int64_t last_included_index) {
    if (_configurations.empty()) {
        return std::pair<int64_t, Configuration>(0, Configuration());
    }
    ConfigurationMap::iterator it = _configurations.upper_bound(last_included_index);
    if (it == _configurations.begin()) {
        return std::pair<int64_t, Configuration>(0, Configuration());
    }
    --it;
    return *it;
}

}

