// ======================================================================
// \title  SyncBench.hpp
// \author krissal1234
// \brief  hpp file for SyncBench component implementation class
// ======================================================================

#ifndef FprimeSyncbench_SyncBench_HPP
#define FprimeSyncbench_SyncBench_HPP

#include "FprimeSyncbench/Components/SyncBench/SyncBenchComponentAc.hpp"

namespace FprimeSyncbench {

class SyncBench final : public SyncBenchComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct SyncBench object
    SyncBench(const char* const compName  //!< The component name
    );

    //! Destroy SyncBench object
    ~SyncBench();
    //Member variables
  private:
    U32 m_remaining_runs {0};
    U32 m_global_sequence_num {0};
    TransportType m_active_transmission_type {TransportType::TLM};

  private:

    // ----------------------------------------------------------------------
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for run
    //!
    //! Example port: receiving calls from the rate group
    void run_handler(FwIndexType portNum,  //!< The port number
                     U32 context           //!< The call order
                     ) override;

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for commands
    // ----------------------------------------------------------------------

    //! Handler implementation for command BENCH_TLM
    //!
    //! Begin Telemetry burst command
    void BENCH_TLM_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                              U32 cmdSeq,           //!< The command sequence number
                              U32 num_runs) override;

    //! Handler implementation for command BENCH_EVT
    //!
    //! Begin Event burst command
    void BENCH_EVT_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                              U32 cmdSeq,           //!< The command sequence number
                              U32 num_runs) override;

    //! Handler implementation for command BENCH_BUF
    //!
    //! Begin Buffer burst command
    void BENCH_BUF_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                              U32 cmdSeq,           //!< The command sequence number
                              U32 num_runs) override;
};

}  // namespace FprimeSyncbench

#endif
