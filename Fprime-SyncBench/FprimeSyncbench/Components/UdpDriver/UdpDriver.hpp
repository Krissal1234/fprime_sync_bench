// ======================================================================
// \title  UdpDriver.hpp
// \author krissal1234
// \brief  hpp file for UdpDriver component implementation class
// ======================================================================

#ifndef FprimeSyncbench_UdpDriver_HPP
#define FprimeSyncbench_UdpDriver_HPP

#include "FprimeSyncbench/Components/UdpDriver/UdpDriverComponentAc.hpp"

namespace FprimeSyncbench {

class UdpDriver final : public UdpDriverComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct UdpDriver object
    UdpDriver(const char* const compName  //!< The component name
    );

    //! Destroy UdpDriver object
    ~UdpDriver();

  private:
    Drv::UdpComponentImpl m_comm;
    Drv::SocketDescriptor m_sock_desc;

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for dataIn
    //!
    //! Input port to receive data to be sent over UDP
    void dataIn_handler(FwIndexType portNum,  //!< The port number
                        const FprimeSyncbench::BenchData& data) override;

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for commands
    // ----------------------------------------------------------------------

    //! Handler implementation for command CONNECT_UDP
    //!
    //! Command to open UDP socket
    void CONNECT_UDP_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                                U32 cmdSeq,           //!< The command sequence number
                                const Fw::CmdStringArg& hostname,
                                U16 hostport) override;
};

}  // namespace FprimeSyncbench

#endif
