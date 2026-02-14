// ======================================================================
// \title  TransportDriver.cpp
// \author krissal1234
// \brief  cpp file for TransportDriver component implementation class
// ======================================================================

#include "FprimeSyncbench/Components/TransportDriver/TransportDriver.hpp"

namespace FprimeSyncbench {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

TransportDriver ::TransportDriver(const char* const compName) : TransportDriverComponentBase(compName) {}

TransportDriver ::~TransportDriver() {
    m_sock_desc.fd = -1;
    m_active_comm = nullptr;
}

// ----------------------------------------------------------------------
// Handler implementations for typed input ports
// ----------------------------------------------------------------------

void TransportDriver ::dataIn_handler(FwIndexType portNum, const FprimeSyncbench::BenchData& data) {
    this->log_ACTIVITY_HI_BufferEvent(data);


    if (this->m_sock_desc.fd == -1 || this->m_active_comm == nullptr) {
        return;
    }

    Fw::Buffer buffer = this->allocate_out(0, FprimeSyncbench::BenchData::SERIALIZED_SIZE);

    if (buffer.getSize() > 0) {
        auto serializer = buffer.getSerializer();
        Fw::SerializeStatus stat = data.serializeTo(serializer);

        if (stat == Fw::FW_SERIALIZE_OK) {
            this->m_active_comm->send(this->m_sock_desc, buffer.getData(), buffer.getSize());
        }

        this->deallocate_out(0, buffer);

    } else {

        this->log_WARNING_LO_MemoryAllocationFailed();
    }
}

// ----------------------------------------------------------------------
// Handler implementations for commands
// ----------------------------------------------------------------------

void TransportDriver ::CONNECT_DRIVER_cmdHandler(FwOpcodeType opCode,
                                                 U32 cmdSeq,
                                                 FprimeSyncbench::TransportProtocol protocol,
                                                 const Fw::CmdStringArg& hostname,
                                                 U16 hostport) {

    if (this->m_sock_desc.fd != -1) {

        if (this->m_active_comm != nullptr) {
            this->m_active_comm->close(this->m_sock_desc);
        }

        this->m_sock_desc.fd = -1;
    }


    if (protocol == TransportProtocol::TCP) {
        this->m_active_comm = &m_tcp_comm;
        this->m_active_comm->configure(hostname.toChar(), hostport, (protocol == TransportProtocol::TCP) ? 2 : 0, 0);
    } else {
        this->m_udp_comm.configureSend(hostname.toChar(), hostport, 0 , 0);
        this->m_active_comm = &m_udp_comm;
    }


    Drv::SocketIpStatus status = this->m_active_comm->open(m_sock_desc);

    if (status == Drv::SOCK_SUCCESS && m_sock_desc.fd != -1) {
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    } else {
        this->log_WARNING_HI_SocketError(static_cast<I32>(status));
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::EXECUTION_ERROR);
    }
}

}  // namespace FprimeSyncbench
