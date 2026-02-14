// ======================================================================
// \title  TransportDriver.hpp
// \author krissal1234
// \brief  hpp file for TransportDriver component implementation class
// ======================================================================

#ifndef FprimeSyncbench_TransportDriver_HPP
#define FprimeSyncbench_TransportDriver_HPP

#include "FprimeSyncbench/Components/TransportDriver/TransportDriverComponentAc.hpp"
#include <Drv/Ip/TcpClientSocket.hpp>
#include <Drv/Ip/IpSocket.hpp>
#include <Drv/Ip/UdpSocket.hpp>

namespace FprimeSyncbench {

class TransportDriver final : public TransportDriverComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct TransportDriver object
    TransportDriver(const char* const compName  //!< The component name
    );

    //! Destroy TransportDriver object
    ~TransportDriver();

  private:

  Drv::TcpClientSocket m_tcp_comm;
  Drv::UdpSocket m_udp_comm;
  Drv::IpSocket* m_active_comm;
  Drv::SocketDescriptor m_sock_desc;

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for dataIn
    //!
    //! Input port for sending data to external server
    void dataIn_handler(FwIndexType portNum,  //!< The port number
                        const FprimeSyncbench::BenchData& data) override;

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for commands
    // ----------------------------------------------------------------------

    //! Handler implementation for command CONNECT_DRIVER
    //!
    //! Command to open port for TCP or UDP connection
    void CONNECT_DRIVER_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                                   U32 cmdSeq,           //!< The command sequence number
                                   FprimeSyncbench::TransportProtocol protocol,
                                   const Fw::CmdStringArg& hostname,
                                   U16 hostport) override;
};

}  // namespace FprimeSyncbench

#endif
