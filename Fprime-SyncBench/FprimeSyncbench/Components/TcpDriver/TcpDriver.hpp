// ======================================================================
// \title  TcpDriver.hpp
// \author krissal1234
// \brief  hpp file for TcpDriver component implementation class
// ======================================================================

#ifndef FprimeSyncbench_TcpDriver_HPP
#define FprimeSyncbench_TcpDriver_HPP

#include "FprimeSyncbench/Components/TcpDriver/TcpDriverComponentAc.hpp"
#include <Drv/Ip/TcpClientSocket.hpp>
#include <Drv/Ip/IpSocket.hpp>

namespace FprimeSyncbench {

class TcpDriver final : public TcpDriverComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct TcpDriver object
    TcpDriver(const char* const compName  //!< The component name
    );

    //! Destroy TcpDriver object
    ~TcpDriver();

  private:

  Drv::TcpClientSocket m_comm;
  Drv::SocketDescriptor m_sock_desc;


  private:
    // ----------------------------------------------------------------------
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for dataIn
    void dataIn_handler(FwIndexType portNum,  //!< The port number
                        const FprimeSyncbench::BenchData& data) override;

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for commands
    // ----------------------------------------------------------------------

    //! Handler implementation for command CONNECT_TCP
    void CONNECT_TCP_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                                U32 cmdSeq,           //!< The command sequence number
                                const Fw::CmdStringArg& hostname,
                                U16 hostport) override;


};

}  // namespace FprimeSyncbench

#endif
