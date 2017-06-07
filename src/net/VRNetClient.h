#ifndef VRNETCLIENT_H
#define VRNETCLIENT_H

#include "VRNetInterface.h"

#ifndef WIN32
  #include <netinet/tcp.h>
  #include <arpa/inet.h>
#endif

namespace MinVR {

class VRNetClient : public VRNetInterface {
 public:

  VRNetClient(const std::string &serverIP, const std::string &serverPort);
  ~VRNetClient();

  void syncEventDataAcrossAllNodes(std::vector<VRDataIndex> *events);

  void syncSwapBuffersAcrossAllNodes();

 private:

  SOCKET _socketFD;

};


} // end namespace MinVR  


#endif
