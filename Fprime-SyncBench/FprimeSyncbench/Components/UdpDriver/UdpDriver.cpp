// ======================================================================
// \title  UdpDriver.cpp
// \author krissal1234
// \brief  cpp file for UdpDriver component implementation class
// ======================================================================

#include "FprimeSyncbench/Components/UdpDriver/UdpDriver.hpp"

namespace FprimeSyncbench {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

UdpDriver ::UdpDriver(const char* const compName) : UdpDriverComponentBase(compName) {}

UdpDriver ::~UdpDriver() {}

// ----------------------------------------------------------------------
// Handler implementations for typed input ports
// ----------------------------------------------------------------------

void UdpDriver ::dataIn_handler(FwIndexType portNum, const FprimeSyncbench::BenchData& data) {
    this->log_ACTIVITY_HI_BufferEvent(data);


    if (m_sock_desc.fd == -1) {
        return;
    }

    Fw::Buffer buffer = this->allocate_out(0, FprimeSyncbench::BenchData::SERIALIZED_SIZE);

    if (buffer.getSize() > 0) {
        auto serializer = buffer.getSerializer();
        Fw::SerializeStatus stat = data.serializeTo(serializer);

        if (stat == Fw::FW_SERIALIZE_OK) {
            m_comm.send(m_sock_desc, buffer.getData(), buffer.getSize());
        }

        this->deallocate_out(0, buffer);

    } else {

        this->log_WARNING_LO_MemoryAllocationFailed();
    }
}

// ----------------------------------------------------------------------
// Handler implementations for commands
// ----------------------------------------------------------------------

void UdpDriver ::CONNECT_UDP_cmdHandler(FwOpcodeType opCode,
                                        U32 cmdSeq,
                                        const Fw::CmdStringArg& hostname,
                                        U16 hostport) {

    m_comm.configure(hostname.toChar(), hostport, 2, 0);

    Drv::SocketIpStatus status = m_comm.open(m_sock_desc);

    if (status == Drv::SOCK_SUCCESS && m_sock_desc.fd != -1) {
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    } else {
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::EXECUTION_ERROR);
    }


    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

}  // namespace FprimeSyncbench
