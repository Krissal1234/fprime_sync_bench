// ======================================================================
// \title  SyncBench.cpp
// \author krissal1234
// \brief  cpp file for SyncBench component implementation class
// ======================================================================

#include "FprimeSyncbench/Components/SyncBench/SyncBench.hpp"

namespace FprimeSyncbench {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

SyncBench ::SyncBench(const char* const compName) : SyncBenchComponentBase(compName) {}

SyncBench ::~SyncBench() {}

// ----------------------------------------------------------------------
// Handler implementations for typed input ports
// ----------------------------------------------------------------------

void SyncBench ::run_handler(FwIndexType portNum, U32 context) {

    Fw::Time curr_time = this->getTime();
    U32 secs = curr_time.getSeconds();
    U32 usecs = curr_time.getUSeconds();

    //We have to convert seconds to microseconds and cast it into a U64
    //1 second is 1,000,000 microseconds
    //ULL tell the compiler that this multiplication must happen in 64 bit space
    U64 ts= (static_cast<U64>(secs) * 1000000ULL) + usecs;

    BenchData data = {this->m_global_sequence_num, this->m_active_transmission_type, ts};

    switch (m_active_transmission_type)
    {
    case TransportType::TLM:
        this->tlmWrite_TelemtrySender(data);
        break;
    case TransportType::EVT:
        this->log_ACTIVITY_HI_EventSender(data);
        break;
    case TransportType::BUF:
        /* code */
        break;
    default:
        break;
    }
}

// ----------------------------------------------------------------------
// Handler implementations for commands
// ----------------------------------------------------------------------

void SyncBench ::BENCH_TLM_cmdHandler(FwOpcodeType opCode, U32 cmdSeq, U32 num_runs) {
    this->m_remaining_runs = num_runs;
    this->m_active_transmission_type = TransportType::TLM;
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void SyncBench ::BENCH_EVT_cmdHandler(FwOpcodeType opCode, U32 cmdSeq, U32 num_runs) {
    this->m_remaining_runs = num_runs;
    this->m_active_transmission_type = TransportType::EVT;
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void SyncBench ::BENCH_BUF_cmdHandler(FwOpcodeType opCode, U32 cmdSeq, U32 num_runs) {
    this->m_remaining_runs = num_runs;
    this->m_active_transmission_type = TransportType::BUF;
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

}  // namespace FprimeSyncbench
