//
//  peer_nts.h
//  P2PSP
//
//  This code is distributed under the GNU General Public License (see
//  THE_GENERAL_GNU_PUBLIC_LICENSE.txt for extending this information).
//  Copyright (C) 2016, the P2PSP team.
//  http://www.p2psp.org
//
//  NTS: NAT Traversal Set of rules
//

#ifndef P2PSP_CORE_PEER_NTS_H
#define P2PSP_CORE_PEER_NTS_H

#include "peer_dbs.h"
#include "common.h"
#include "common_nts.h"
#include "../util/trace.h"
#include <cstdint>
#include <list>
#include <set>
#include <vector>
#include <mutex>
#include <condition_variable>
// TODO: other include files

namespace p2psp {

class PeerNTS : public PeerDBS {
 protected:
  std::string peer_id_;
  std::mutex hello_messages_lock_;
  std::list<message_t> hello_messages_;
  std::map<message_t, timepoint_t> hello_messages_times_;
  std::map<message_t, std::vector<uint16_t> > hello_messages_ports_;
  std::condition_variable hello_messages_event_;
  std::mutex hello_messages_event_mutex_;
  // A list of peer_ids that contains the peers that were in the team when
  // starting incorporation and that are not connected yet
  std::list<std::string> initial_peer_list_;

  virtual void SayHello(const ip::udp::endpoint& peer,
      std::vector<uint16_t> additional_ports = {});
  virtual void SendMessage(const message_t& message_data);
  virtual void ReceiveId();
  virtual void SendHelloThread();
  virtual void StartSendHelloThread();
  virtual void ReceiveTheListOfPeers2();
  virtual void DisconnectFromTheSplitter() override;
  virtual void TryToDisconnectFromTheSplitter();

  virtual std::set<uint16_t>&& GetFactors(uint16_t n);
  virtual uint16_t CountCombinations(const std::set<uint16_t>& factors);
  virtual std::set<uint16_t>&& GetProbablePortDiffs(uint16_t port_diff,
      uint16_t peer_number);
  virtual std::vector<uint16_t>&& GetProbableSourcePorts(
      uint16_t source_port_to_splitter, uint16_t port_diff,
      uint16_t peer_number);

  virtual int ProcessMessage(const std::vector<char>& message,
      const ip::udp::endpoint& sender) override;

 public:
  PeerNTS();
  ~PeerNTS();
  virtual void Init() override;
};
}

#endif  // P2PSP_CORE_PEER_NTS_H